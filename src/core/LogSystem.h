#pragma once
#include <memory>
#include <string_view>
#include <spdlog/logger.h>

namespace spdlog { namespace sinks { class sink; } }

/**
 * @brief Global logging facility (thread-safe).
 *
 * Thin wrapper around spdlog.
 */
class LogSystem
{
public:
    enum class Level { Debug, Info, Warn, Error };

    /// Access singleton
    static LogSystem& Instance();

    /// Runtime severity
    void SetLevel(Level level);
    [[nodiscard]] Level GetLevel() const;

    /// Log helpers ----------------------------------------------------------
    template<class... Args>
    void Debug (std::string_view fmt, Args&&... args);
    template<class... Args>
    void Info  (std::string_view fmt, Args&&... args);
    template<class... Args>
    void Warn  (std::string_view fmt, Args&&... args);
    template<class... Args>
    void Error (std::string_view fmt, Args&&... args);

    /// Flush sinks (called at shutdown)
    void Flush();

    /// ——— TESTING ——————————————————————————————————————————————
    /// Replace sink so the unit-tests can capture output.
    void SetCustomSinkForTesting(std::shared_ptr<spdlog::sinks::sink> sink);
    /// ————————————————————————————————————————————————————————

private:
    LogSystem();                         // hidden
    ~LogSystem();
    LogSystem(const LogSystem&)            = delete;
    LogSystem& operator=(const LogSystem&) = delete;

    struct Impl;
    std::unique_ptr<Impl> m_impl;       ///< PIMPL – owns spdlog::logger

    spdlog::logger&       GetLogger();
    const spdlog::logger& GetLogger() const;
};

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

// ── Inline logging wrappers ────────────────────────────────────────────────
template<class... Args>
inline void LogSystem::Debug(std::string_view fmt, Args&&... args)
{
    if (GetLogger().should_log(spdlog::level::debug))
        GetLogger().debug(fmt, std::forward<Args>(args)...);
}
template<class... Args>
inline void LogSystem::Info(std::string_view fmt, Args&&... args)
{
    if (GetLogger().should_log(spdlog::level::info))
        GetLogger().info(fmt, std::forward<Args>(args)...);
}
template<class... Args>
inline void LogSystem::Warn(std::string_view fmt, Args&&... args)
{
    if (GetLogger().should_log(spdlog::level::warn))
        GetLogger().warn(fmt, std::forward<Args>(args)...);
}
template<class... Args>
inline void LogSystem::Error(std::string_view fmt, Args&&... args)
{
    if (GetLogger().should_log(spdlog::level::err))
        GetLogger().error(fmt, std::forward<Args>(args)...);
}
