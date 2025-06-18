#pragma once

#include <memory>
#include <string_view>
#include <spdlog/logger.h>

namespace spdlog {
namespace sinks { class sink; }
}

/**
 * @brief Global logging facility (thread-safe).
 *
 * Wrapper around spdlog; keeps a single sink to stdout with color.
 */
class LogSystem
{
public:
    enum class Level { Debug, Info, Warn, Error };

    /** Return the singleton instance (constructed on first call). */
    static LogSystem& Instance();

    /** Configure minimum severity displayed at runtime. */
    void SetLevel(Level level);

    /** Log helpers */
    template<class... Args>
    void Debug(std::string_view fmt, Args&&... args);
    template<class... Args>
    void Info (std::string_view fmt, Args&&... args);
    template<class... Args>
    void Warn (std::string_view fmt, Args&&... args);
    template<class... Args>
    void Error(std::string_view fmt, Args&&... args);

    /** Flush all sinks (called during shutdown). */
    void Flush();

    /** Return current severity level. */
    [[nodiscard]] Level GetLevel() const;

#ifdef TESTING
    /** Replace logger sink (testing only). */
    void SetCustomSinkForTesting(std::shared_ptr<spdlog::sinks::sink> sink);
#endif

private:
    LogSystem();                      // hidden
    ~LogSystem();                     // flush + drop sink
    LogSystem(const LogSystem&) = delete;
    LogSystem& operator=(const LogSystem&) = delete;

    struct Impl;
    std::unique_ptr<Impl> m_impl;     ///< PIMPL – contains spdlog::logger

    spdlog::logger& GetLogger();
    const spdlog::logger& GetLogger() const;
};

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

// Template definitions

template<class... Args>
inline void LogSystem::Debug(std::string_view fmt, Args&&... args)
{
    auto& lg = GetLogger();
    if (lg.should_log(spdlog::level::debug))
        lg.debug(fmt, std::forward<Args>(args)...);
}

template<class... Args>
inline void LogSystem::Info(std::string_view fmt, Args&&... args)
{
    auto& lg = GetLogger();
    if (lg.should_log(spdlog::level::info))
        lg.info(fmt, std::forward<Args>(args)...);
}

template<class... Args>
inline void LogSystem::Warn(std::string_view fmt, Args&&... args)
{
    auto& lg = GetLogger();
    if (lg.should_log(spdlog::level::warn))
        lg.warn(fmt, std::forward<Args>(args)...);
}

template<class... Args>
inline void LogSystem::Error(std::string_view fmt, Args&&... args)
{
    auto& lg = GetLogger();
    if (lg.should_log(spdlog::level::err))
        lg.error(fmt, std::forward<Args>(args)...);
}

