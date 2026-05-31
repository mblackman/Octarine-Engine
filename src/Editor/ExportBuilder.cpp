#include "Editor/ExportBuilder.h"

#ifdef OCTARINE_WITH_EDITOR

#include "General/Logger.h"
#include "Project/ProjectIni.h"

#include <system_error>
#include <utility>

namespace octarine::editor
{
    namespace
    {
#ifdef _WIN32
        constexpr const char* kBuildScriptFile = "build-desktop.ps1";
#else
        constexpr const char* kBuildScriptFile = "build-desktop.sh";
#endif

        bool FileExists(const std::filesystem::path& p)
        {
            std::error_code ec;
            return std::filesystem::exists(p, ec) && !std::filesystem::is_directory(p, ec);
        }
    } // namespace

    std::optional<std::filesystem::path>
    ExportBuilder::ResolveBuildScript(const std::filesystem::path& project_dir)
    {
        std::filesystem::path candidate = project_dir / "scripts" / kBuildScriptFile;
        if (FileExists(candidate))
        {
            return candidate;
        }
        return std::nullopt;
    }

    std::vector<std::string> ExportBuilder::Validate(const std::filesystem::path& project_dir)
    {
        std::vector<std::string> errors;

        auto ini = octarine::project::ProjectIni::Load(project_dir);
        if (!ini)
        {
            errors.push_back("project.ini not found at " + (project_dir / "project.ini").string());
        }
        else
        {
            for (auto& err : ini->ValidateForShipping())
            {
                errors.push_back("project.ini: " + err);
            }
        }

        if (!ResolveBuildScript(project_dir))
        {
            errors.push_back(
                "missing scripts/" + std::string(kBuildScriptFile) +
                " — run scripts/octarine-init-build to scaffold it");
        }

        return errors;
    }

    bool ExportBuilder::Run(const ExportOptions& opts)
    {
        if (status_ == ExportStatus::Building)
        {
            return true;
        }

        last_exit_code_.reset();
        stdout_partial_.clear();
        stderr_partial_.clear();

        auto errors = Validate(opts.project_dir);
        if (!errors.empty())
        {
            for (auto& e : errors)
            {
                AppendLine(true, "[export] " + e);
            }
            status_ = ExportStatus::Failed;
            return false;
        }

        auto resolved = ResolveBuildScript(opts.project_dir);
        // ResolveBuildScript already non-null because Validate passed; double-check to silence the
        // optional unwrap and to keep behaviour explicit if Validate's policy diverges later.
        if (!resolved)
        {
            AppendLine(true, "[export] build script vanished between validation and spawn");
            status_ = ExportStatus::Failed;
            return false;
        }
        resolved_script_ = *resolved;

        octarine::process::SpawnOptions po;
#ifdef _WIN32
        // PowerShell launcher: -ExecutionPolicy Bypass so a freshly scaffolded unsigned .ps1 runs
        // without the dev having to relax their machine-wide policy.
        po.argv.push_back("powershell.exe");
        po.argv.push_back("-NoProfile");
        po.argv.push_back("-ExecutionPolicy");
        po.argv.push_back("Bypass");
        po.argv.push_back("-File");
        po.argv.push_back(resolved_script_.string());
#else
        po.argv.push_back("bash");
        po.argv.push_back(resolved_script_.string());
#endif
        po.cwd = opts.project_dir.string();

        if (!opts.version_name.empty())
        {
            po.env.emplace_back("OCTARINE_VERSION_NAME", opts.version_name);
        }
        if (!opts.version_code.empty())
        {
            po.env.emplace_back("OCTARINE_VERSION_CODE", opts.version_code);
        }
        if (!opts.preset.empty())
        {
            po.env.emplace_back("OCTARINE_PRESET", opts.preset);
        }

        auto p = octarine::process::Process::Spawn(po);
        if (!p)
        {
            AppendLine(true, "[export] failed to spawn " + resolved_script_.string());
            status_ = ExportStatus::Failed;
            return false;
        }

        p->OnStdout([this](std::string_view sv) { SplatChunk(sv, /*is_stderr=*/false); });
        p->OnStderr([this](std::string_view sv) { SplatChunk(sv, /*is_stderr=*/true); });

        process_ = std::move(p);
        status_ = ExportStatus::Building;
        AppendLine(false, "[export] launched " + resolved_script_.string());
        Logger::Info("ExportBuilder: spawned " + resolved_script_.string() + " in " + po.cwd);
        return true;
    }

    void ExportBuilder::Stop()
    {
        if (status_ != ExportStatus::Building || !process_)
        {
            return;
        }
        process_->Kill();
        AppendLine(false, "[export] stop requested");
    }

    void ExportBuilder::Pump()
    {
        if (!process_)
        {
            return;
        }
        const bool still_running = process_->Pump();
        if (!still_running && status_ == ExportStatus::Building)
        {
            FlushPartialLines();
            last_exit_code_ = process_->ExitCode();
            const int code = last_exit_code_.value_or(-1);
            status_ = (code == 0) ? ExportStatus::Succeeded : ExportStatus::Failed;
            AppendLine(false, "[export] exited with code " + std::to_string(code));
            process_.reset();
        }
    }

    void ExportBuilder::SplatChunk(std::string_view chunk, bool is_stderr)
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

    void ExportBuilder::FlushPartialLines()
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

    void ExportBuilder::AppendLine(bool is_stderr, std::string text)
    {
        if (log_.size() >= kLogCap)
        {
            log_.pop_front();
            ++dropped_;
        }
        log_.push_back(ExportLogLine{is_stderr, std::move(text)});
    }

    ExportBuilder::LogSnapshot ExportBuilder::LogCopy() const
    {
        LogSnapshot snap;
        snap.total_dropped = dropped_;
        snap.lines.assign(log_.begin(), log_.end());
        return snap;
    }

    void ExportBuilder::ClearLog()
    {
        log_.clear();
        dropped_ = 0;
    }
} // namespace octarine::editor

#endif // OCTARINE_WITH_EDITOR
