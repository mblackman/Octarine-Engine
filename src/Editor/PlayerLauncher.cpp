#include "Editor/PlayerLauncher.h"

#ifdef OCTARINE_WITH_EDITOR

#include "General/Logger.h"

#include <SDL3/SDL_filesystem.h>

#include <system_error>
#include <utility>

namespace octarine::editor
{
    namespace
    {
#ifdef _WIN32
        constexpr const char* kPlayerExeName = "OctarineEngine-player.exe";
#else
        constexpr const char* kPlayerExeName = "OctarineEngine-player";
#endif

        bool FileExists(const std::filesystem::path& p)
        {
            std::error_code ec;
            return std::filesystem::exists(p, ec) && !std::filesystem::is_directory(p, ec);
        }
    } // namespace

    std::optional<std::filesystem::path> PlayerLauncher::ResolvePlayerBinary(const std::filesystem::path& editor_dir)
    {
        // Probe order matches plan recommendation: prefer a side-by-side install layout, then
        // fall back to sibling dev presets. The first existing candidate wins.
        //
        // Editor exe lives at `<build>/<editor-preset>/bin/<config>/OctarineEngine[.exe]`. Player
        // exe (renamed via CMakeLists.txt OUTPUT_NAME) lives at the mirror path under a sibling
        // `player-*` preset dir with the matching `<config>` subdir.

        // (1) Same directory (installed layout, or dev manually copied next to the editor).
        {
            std::filesystem::path candidate = editor_dir / kPlayerExeName;
            if (FileExists(candidate))
            {
                return candidate;
            }
        }

        // Compute `<build>` and the config-name string (e.g. "debug", "release") from the editor
        // exe layout. The structure can be wrong (installed dropped editor in some other place);
        // we just skip the sibling-preset probes if it doesn't look like a dev tree.
        std::filesystem::path config_dir = editor_dir;            // ".../bin/debug"
        std::filesystem::path bin_dir = config_dir.parent_path(); // ".../bin"
        std::filesystem::path preset_dir = bin_dir.parent_path(); // ".../editor-debug"
        std::filesystem::path build_dir = preset_dir.parent_path(); // ".../build"

        if (config_dir.empty() || bin_dir.filename() != "bin" || build_dir.empty())
        {
            return std::nullopt;
        }

        const std::string config_name = config_dir.filename().string();

        // (2) Sibling player-* presets in the same build dir. Try a matching-config preset first,
        // then the others as a fallback.
        const std::string_view candidates[] = {
            "player-debug",
            "player-release",
            "player-profile",
        };
        for (const auto preset : candidates)
        {
            std::filesystem::path candidate =
                build_dir / preset / "bin" / config_name / kPlayerExeName;
            if (FileExists(candidate))
            {
                return candidate;
            }
        }
        // Cross-config fallback: the matching config dir may not exist (e.g. editor built Debug,
        // player built Release). Try the canonical config names.
        const std::string_view configs[] = {"debug", "release", "relwithdebinfo", "minsizerel"};
        for (const auto preset : candidates)
        {
            for (const auto cfg : configs)
            {
                std::filesystem::path candidate =
                    build_dir / preset / "bin" / cfg / kPlayerExeName;
                if (FileExists(candidate))
                {
                    return candidate;
                }
            }
        }
        return std::nullopt;
    }

    bool PlayerLauncher::Run(const std::filesystem::path& project_dir)
    {
        if (status_ == PlayerStatus::Running)
        {
            return true;
        }

        last_exit_code_.reset();
        stdout_partial_.clear();
        stderr_partial_.clear();

        std::filesystem::path editor_dir;
        if (const char* base = SDL_GetBasePath())
        {
            editor_dir = std::filesystem::path(base);
        }
        else
        {
            editor_dir = std::filesystem::current_path();
        }

        auto resolved = ResolvePlayerBinary(editor_dir);
        if (!resolved)
        {
            AppendLine(true,
                       "[player] cannot find OctarineEngine-player[.exe] beside the editor or under "
                       "a sibling `player-*` preset. Build it with: "
                       "cmake --preset player-debug && cmake --build build/player-debug");
            status_ = PlayerStatus::FailedToLaunch;
            return false;
        }
        resolved_binary_ = *resolved;

        octarine::process::SpawnOptions opts;
        opts.argv.push_back(resolved_binary_.string());
        opts.argv.push_back(project_dir.string());

        auto p = octarine::process::Process::Spawn(opts);
        if (!p)
        {
            AppendLine(true, "[player] failed to spawn " + resolved_binary_.string());
            status_ = PlayerStatus::FailedToLaunch;
            return false;
        }

        p->OnStdout([this](std::string_view sv) { SplatChunk(sv, /*is_stderr=*/false); });
        p->OnStderr([this](std::string_view sv) { SplatChunk(sv, /*is_stderr=*/true); });

        process_ = std::move(p);
        status_ = PlayerStatus::Running;
        AppendLine(false, "[player] launched " + resolved_binary_.string() + " " + project_dir.string());
        Logger::Info("PlayerLauncher: spawned " + resolved_binary_.string() + " for " + project_dir.string());
        return true;
    }

    void PlayerLauncher::Stop()
    {
        if (status_ != PlayerStatus::Running || !process_)
        {
            return;
        }
        process_->Kill();
        AppendLine(false, "[player] stop requested");
    }

    void PlayerLauncher::Pump()
    {
        if (!process_)
        {
            return;
        }
        const bool still_running = process_->Pump();
        if (!still_running && status_ == PlayerStatus::Running)
        {
            FlushPartialLines();
            last_exit_code_ = process_->ExitCode();
            status_ = PlayerStatus::Exited;
            const std::string code = last_exit_code_ ? std::to_string(*last_exit_code_) : std::string("?");
            AppendLine(false, "[player] exited with code " + code);
            process_.reset();
        }
    }

    void PlayerLauncher::SplatChunk(std::string_view chunk, bool is_stderr)
    {
        std::string& partial = is_stderr ? stderr_partial_ : stdout_partial_;
        partial.append(chunk);
        std::size_t start = 0;
        while (true)
        {
            const std::size_t nl = partial.find('\n', start);
            if (nl == std::string::npos)
            {
                break;
            }
            std::string line(partial, start, nl - start);
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }
            AppendLine(is_stderr, std::move(line));
            start = nl + 1;
        }
        partial.erase(0, start);
    }

    void PlayerLauncher::FlushPartialLines()
    {
        if (!stdout_partial_.empty())
        {
            AppendLine(false, std::move(stdout_partial_));
            stdout_partial_.clear();
        }
        if (!stderr_partial_.empty())
        {
            AppendLine(true, std::move(stderr_partial_));
            stderr_partial_.clear();
        }
    }

    void PlayerLauncher::AppendLine(bool is_stderr, std::string text)
    {
        if (log_.size() >= kLogCap)
        {
            log_.pop_front();
            ++dropped_;
        }
        log_.push_back(PlayerLogLine{is_stderr, std::move(text)});
    }

    PlayerLauncher::LogSnapshot PlayerLauncher::LogCopy() const
    {
        LogSnapshot snap;
        snap.total_dropped = dropped_;
        snap.lines.assign(log_.begin(), log_.end());
        return snap;
    }

    void PlayerLauncher::ClearLog()
    {
        log_.clear();
        dropped_ = 0;
    }
} // namespace octarine::editor

#endif // OCTARINE_WITH_EDITOR
