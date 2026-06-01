#include "Process/Process.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
extern char** environ;

// `posix_spawn_file_actions_addchdir_np` is macOS 10.15+ and glibc 2.29+. The macro check must
// be nested rather than `defined() && __GLIBC_PREREQ(...)` in one expression — Clang token-checks
// the function-like macro invocation even when the LHS of `&&` is false, so the unguarded
// expression fatals on macOS where `__GLIBC_PREREQ` is undefined.
#if defined(__APPLE__)
#define OCTARINE_HAS_SPAWN_ADDCHDIR 1
#elif defined(__GLIBC__)
#include <features.h>
#if __GLIBC_PREREQ(2, 29)
#define OCTARINE_HAS_SPAWN_ADDCHDIR 1
#endif
#endif
#ifndef OCTARINE_HAS_SPAWN_ADDCHDIR
#define OCTARINE_HAS_SPAWN_ADDCHDIR 0
#endif
#endif

namespace octarine::process {
namespace {
#ifdef _WIN32
std::wstring Utf8ToWide(const std::string& s) {
  if (s.empty()) {
    return {};
  }
  const int needed = MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), nullptr, 0);
  std::wstring out(static_cast<size_t>(needed), L'\0');
  MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), out.data(), needed);
  return out;
}

// Microsoft's documented argv quoting rules (CommandLineToArgvW round-trip):
// - Wrap in quotes when the arg is empty or contains whitespace/quotes.
// - Inside the quoted region, runs of backslashes are doubled when followed by a quote
//   (including the closing quote), and a literal quote becomes (2N+1) backslashes + ".
std::wstring QuoteArg(const std::wstring& s) {
  if (!s.empty() && s.find_first_of(L" \t\n\v\"") == std::wstring::npos) {
    return s;
  }
  std::wstring out;
  out.push_back(L'"');
  size_t i = 0;
  while (i < s.size()) {
    size_t bs = 0;
    while (i < s.size() && s[i] == L'\\') {
      ++bs;
      ++i;
    }
    if (i == s.size()) {
      out.append(bs * 2, L'\\');
      break;
    }
    if (s[i] == L'"') {
      out.append(bs * 2 + 1, L'\\');
      out.push_back(L'"');
    } else {
      out.append(bs, L'\\');
      out.push_back(s[i]);
    }
    ++i;
  }
  out.push_back(L'"');
  return out;
}

std::wstring BuildCommandLine(const std::vector<std::string>& argv) {
  std::wstring cmdline;
  for (size_t i = 0; i < argv.size(); ++i) {
    if (i) {
      cmdline.push_back(L' ');
    }
    cmdline += QuoteArg(Utf8ToWide(argv[i]));
  }
  return cmdline;
}

// Builds a UTF-16 environment block (KEY=VAL\0KEY=VAL\0\0). If inherit_env, starts from
// the parent's environment and overrides matching keys.
std::wstring BuildEnvBlock(const std::vector<std::pair<std::string, std::string>>& env, bool inherit) {
  std::vector<std::wstring> entries;
  if (inherit) {
    LPWCH parent = GetEnvironmentStringsW();
    if (parent) {
      for (wchar_t* p = parent; *p;) {
        std::wstring entry(p);
        entries.push_back(std::move(entry));
        p += entries.back().size() + 1;
      }
      FreeEnvironmentStringsW(parent);
    }
  }
  for (const auto& [k, v] : env) {
    std::wstring wk = Utf8ToWide(k);
    std::wstring entry = wk + L"=" + Utf8ToWide(v);
    // Case-insensitive prefix match on Windows ("KEY=").
    auto it = std::find_if(entries.begin(), entries.end(), [&](const std::wstring& e) {
      if (e.size() <= wk.size() || e[wk.size()] != L'=') {
        return false;
      }
      return CompareStringOrdinal(e.c_str(), static_cast<int>(wk.size()), wk.c_str(), static_cast<int>(wk.size()),
                                  TRUE) == CSTR_EQUAL;
    });
    if (it != entries.end()) {
      *it = std::move(entry);
    } else {
      entries.push_back(std::move(entry));
    }
  }
  std::wstring block;
  for (const auto& e : entries) {
    block.append(e);
    block.push_back(L'\0');
  }
  block.push_back(L'\0');
  return block;
}
#endif  // _WIN32
}  // namespace

struct StreamBuffer {
  std::mutex mu;
  std::string data;
  OutputCallback cb;
};

struct Process::Impl {
#ifdef _WIN32
  HANDLE process = nullptr;
  DWORD pid = 0;
#else
  pid_t pid = -1;
#endif
  std::atomic<bool> exited{false};
  int exit_code = 0;

  StreamBuffer stdout_buf;
  StreamBuffer stderr_buf;
  std::thread stdout_thread;
  std::thread stderr_thread;
};

Process::Process() = default;
Process::Process(Process&&) noexcept = default;
Process& Process::operator=(Process&&) noexcept = default;

Process::~Process() {
  if (!impl_) {
    return;
  }
  if (IsRunning()) {
    Kill();
  }
  if (impl_->stdout_thread.joinable()) {
    impl_->stdout_thread.join();
  }
  if (impl_->stderr_thread.joinable()) {
    impl_->stderr_thread.join();
  }
#ifdef _WIN32
  if (impl_->process) {
    WaitForSingleObject(impl_->process, INFINITE);
    CloseHandle(impl_->process);
  }
#else
  if (impl_->pid > 0 && !impl_->exited.load()) {
    int status = 0;
    waitpid(impl_->pid, &status, 0);
  }
#endif
}

#ifdef _WIN32
std::optional<Process> Process::Spawn(const SpawnOptions& opts) {
  if (opts.argv.empty()) {
    return std::nullopt;
  }

  SECURITY_ATTRIBUTES sa{};
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = TRUE;

  HANDLE out_r = nullptr;
  HANDLE out_w = nullptr;
  HANDLE err_r = nullptr;
  HANDLE err_w = nullptr;
  if (!CreatePipe(&out_r, &out_w, &sa, 0)) {
    return std::nullopt;
  }
  if (!CreatePipe(&err_r, &err_w, &sa, 0)) {
    CloseHandle(out_r);
    CloseHandle(out_w);
    return std::nullopt;
  }
  // Parent ends of the pipes must not be inherited by the child.
  SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
  SetHandleInformation(err_r, HANDLE_FLAG_INHERIT, 0);

  std::wstring cmdline = BuildCommandLine(opts.argv);
  std::wstring cwd_w = Utf8ToWide(opts.cwd);
  std::wstring env_block;
  LPVOID env_ptr = nullptr;
  if (!opts.env.empty()) {
    env_block = BuildEnvBlock(opts.env, opts.inherit_env);
    env_ptr = const_cast<wchar_t*>(env_block.c_str());
  }

  STARTUPINFOW si{};
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
  si.hStdOutput = out_w;
  si.hStdError = err_w;

  PROCESS_INFORMATION pi{};

  // cmdline buffer must be writable per CreateProcessW contract.
  std::vector<wchar_t> cmd_buf(cmdline.begin(), cmdline.end());
  cmd_buf.push_back(L'\0');

  const BOOL ok =
      CreateProcessW(nullptr, cmd_buf.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW,
                     env_ptr, cwd_w.empty() ? nullptr : cwd_w.c_str(), &si, &pi);

  CloseHandle(out_w);
  CloseHandle(err_w);

  if (!ok) {
    CloseHandle(out_r);
    CloseHandle(err_r);
    return std::nullopt;
  }

  CloseHandle(pi.hThread);

  Process p;
  p.impl_ = std::make_unique<Impl>();
  p.impl_->process = pi.hProcess;
  p.impl_->pid = pi.dwProcessId;

  auto pump_pipe = [](HANDLE h, StreamBuffer* buf) {
    char chunk[4096];
    DWORD n = 0;
    while (ReadFile(h, chunk, sizeof(chunk), &n, nullptr) && n > 0) {
      std::lock_guard lk(buf->mu);
      buf->data.append(chunk, n);
    }
    CloseHandle(h);
  };

  p.impl_->stdout_thread = std::thread(pump_pipe, out_r, &p.impl_->stdout_buf);
  p.impl_->stderr_thread = std::thread(pump_pipe, err_r, &p.impl_->stderr_buf);

  return p;
}

bool Process::IsRunning() {
  if (!impl_ || !impl_->process) {
    return false;
  }
  if (impl_->exited.load()) {
    return false;
  }
  const DWORD rc = WaitForSingleObject(impl_->process, 0);
  if (rc == WAIT_OBJECT_0) {
    DWORD code = 0;
    GetExitCodeProcess(impl_->process, &code);
    impl_->exit_code = static_cast<int>(code);
    impl_->exited.store(true);
    return false;
  }
  return true;
}

std::optional<int> Process::ExitCode() {
  if (!impl_) {
    return std::nullopt;
  }
  if (!impl_->exited.load()) {
    (void)IsRunning();
  }
  if (impl_->exited.load()) {
    return impl_->exit_code;
  }
  return std::nullopt;
}

void Process::Kill() {
  if (!impl_ || !impl_->process) {
    return;
  }
  TerminateProcess(impl_->process, 1);
}
#else  // POSIX
std::optional<Process> Process::Spawn(const SpawnOptions& opts) {
  if (opts.argv.empty()) {
    return std::nullopt;
  }

  int out_pipe[2];
  int err_pipe[2];
  if (pipe(out_pipe) != 0) {
    return std::nullopt;
  }
  if (pipe(err_pipe) != 0) {
    close(out_pipe[0]);
    close(out_pipe[1]);
    return std::nullopt;
  }

  posix_spawn_file_actions_t actions;
  posix_spawn_file_actions_init(&actions);
  posix_spawn_file_actions_adddup2(&actions, out_pipe[1], STDOUT_FILENO);
  posix_spawn_file_actions_adddup2(&actions, err_pipe[1], STDERR_FILENO);
  posix_spawn_file_actions_addclose(&actions, out_pipe[0]);
  posix_spawn_file_actions_addclose(&actions, err_pipe[0]);
  posix_spawn_file_actions_addclose(&actions, out_pipe[1]);
  posix_spawn_file_actions_addclose(&actions, err_pipe[1]);
#if OCTARINE_HAS_SPAWN_ADDCHDIR
  if (!opts.cwd.empty()) {
    posix_spawn_file_actions_addchdir_np(&actions, opts.cwd.c_str());
  }
#endif

  posix_spawnattr_t attr;
  posix_spawnattr_init(&attr);

  std::vector<char*> argv_c;
  argv_c.reserve(opts.argv.size() + 1);
  for (const auto& a : opts.argv) {
    argv_c.push_back(const_cast<char*>(a.c_str()));
  }
  argv_c.push_back(nullptr);

  std::vector<std::string> env_store;
  std::vector<char*> envp_v;
  char** envp_ptr = environ;
  if (!opts.env.empty()) {
    if (opts.inherit_env) {
      for (char** e = environ; *e; ++e) {
        env_store.emplace_back(*e);
      }
    }
    for (const auto& [k, v] : opts.env) {
      std::string prefix = k + "=";
      auto it = std::find_if(env_store.begin(), env_store.end(),
                             [&](const std::string& s) { return s.compare(0, prefix.size(), prefix) == 0; });
      std::string entry = prefix + v;
      if (it != env_store.end()) {
        *it = std::move(entry);
      } else {
        env_store.push_back(std::move(entry));
      }
    }
    envp_v.reserve(env_store.size() + 1);
    for (auto& s : env_store) {
      envp_v.push_back(s.data());
    }
    envp_v.push_back(nullptr);
    envp_ptr = envp_v.data();
  }

  pid_t child = -1;
  const int rc = posix_spawnp(&child, opts.argv[0].c_str(), &actions, &attr, argv_c.data(), envp_ptr);

  posix_spawn_file_actions_destroy(&actions);
  posix_spawnattr_destroy(&attr);

  close(out_pipe[1]);
  close(err_pipe[1]);

  if (rc != 0) {
    close(out_pipe[0]);
    close(err_pipe[0]);
    errno = rc;
    return std::nullopt;
  }

  Process p;
  p.impl_ = std::make_unique<Impl>();
  p.impl_->pid = child;

  auto pump_pipe = [](int fd, StreamBuffer* buf) {
    char chunk[4096];
    for (;;) {
      ssize_t n = read(fd, chunk, sizeof(chunk));
      if (n > 0) {
        std::lock_guard lk(buf->mu);
        buf->data.append(chunk, static_cast<size_t>(n));
      } else if (n == 0) {
        break;
      } else if (errno != EINTR) {
        break;
      }
    }
    close(fd);
  };

  p.impl_->stdout_thread = std::thread(pump_pipe, out_pipe[0], &p.impl_->stdout_buf);
  p.impl_->stderr_thread = std::thread(pump_pipe, err_pipe[0], &p.impl_->stderr_buf);

  return p;
}

bool Process::IsRunning() {
  if (!impl_ || impl_->pid <= 0) {
    return false;
  }
  if (impl_->exited.load()) {
    return false;
  }
  int status = 0;
  const pid_t r = waitpid(impl_->pid, &status, WNOHANG);
  if (r == impl_->pid) {
    if (WIFEXITED(status)) {
      impl_->exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      impl_->exit_code = 128 + WTERMSIG(status);
    }
    impl_->exited.store(true);
    return false;
  }
  return true;
}

std::optional<int> Process::ExitCode() {
  if (!impl_) {
    return std::nullopt;
  }
  if (!impl_->exited.load()) {
    (void)IsRunning();
  }
  if (impl_->exited.load()) {
    return impl_->exit_code;
  }
  return std::nullopt;
}

void Process::Kill() {
  if (!impl_ || impl_->pid <= 0 || impl_->exited.load()) {
    return;
  }
  ::kill(impl_->pid, SIGKILL);
}
#endif

void Process::OnStdout(OutputCallback cb) {
  if (!impl_) {
    return;
  }
  std::lock_guard lk(impl_->stdout_buf.mu);
  impl_->stdout_buf.cb = std::move(cb);
}

void Process::OnStderr(OutputCallback cb) {
  if (!impl_) {
    return;
  }
  std::lock_guard lk(impl_->stderr_buf.mu);
  impl_->stderr_buf.cb = std::move(cb);
}

namespace {
void DrainStream(StreamBuffer& buf) {
  std::string chunk;
  OutputCallback cb;
  {
    std::lock_guard lk(buf.mu);
    chunk.swap(buf.data);
    cb = buf.cb;
  }
  if (cb && !chunk.empty()) {
    cb(chunk);
  }
}
}  // namespace

bool Process::Pump() {
  if (!impl_) {
    return false;
  }
  DrainStream(impl_->stdout_buf);
  DrainStream(impl_->stderr_buf);
  return IsRunning();
}

int Process::Wait() {
  if (!impl_) {
    return -1;
  }
#ifdef _WIN32
  if (impl_->process) {
    WaitForSingleObject(impl_->process, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(impl_->process, &code);
    impl_->exit_code = static_cast<int>(code);
    impl_->exited.store(true);
  }
#else
  if (impl_->pid > 0 && !impl_->exited.load()) {
    int status = 0;
    while (waitpid(impl_->pid, &status, 0) < 0 && errno == EINTR) {
    }
    if (WIFEXITED(status)) {
      impl_->exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      impl_->exit_code = 128 + WTERMSIG(status);
    }
    impl_->exited.store(true);
  }
#endif
  if (impl_->stdout_thread.joinable()) {
    impl_->stdout_thread.join();
  }
  if (impl_->stderr_thread.joinable()) {
    impl_->stderr_thread.join();
  }
  DrainStream(impl_->stdout_buf);
  DrainStream(impl_->stderr_buf);
  return impl_->exit_code;
}
}  // namespace octarine::process
