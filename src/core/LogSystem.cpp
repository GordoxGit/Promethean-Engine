#include "core/LogSystem.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

struct LogSystem::Impl {
    std::shared_ptr<spdlog::logger> logger;
    Level currentLevel{Level::Info};
};

static spdlog::level::level_enum ToSpd(LogSystem::Level lvl)
{
    switch(lvl) {
    case LogSystem::Level::Debug: return spdlog::level::debug;
    case LogSystem::Level::Info:  return spdlog::level::info;
    case LogSystem::Level::Warn:  return spdlog::level::warn;
    case LogSystem::Level::Error: return spdlog::level::err;
    }
    return spdlog::level::info;
}

LogSystem& LogSystem::Instance()
{
    static LogSystem instance;
    return instance;
}

LogSystem::LogSystem() : m_impl(std::make_unique<Impl>())
{
    auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    m_impl->logger = std::make_shared<spdlog::logger>("Promethean", std::move(sink));
    m_impl->logger->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%^%l%$] %v");
    m_impl->logger->set_level(spdlog::level::info);
    spdlog::register_logger(m_impl->logger);
}

LogSystem::~LogSystem()
{
    Flush();
    spdlog::drop("Promethean");
}

spdlog::logger& LogSystem::GetLogger()
{
    return *m_impl->logger;
}

const spdlog::logger& LogSystem::GetLogger() const
{
    return *m_impl->logger;
}

void LogSystem::SetLevel(Level level)
{
    m_impl->currentLevel = level;
    m_impl->logger->set_level(ToSpd(level));
}

LogSystem::Level LogSystem::GetLevel() const
{
    return m_impl->currentLevel;
}

void LogSystem::Flush()
{
    m_impl->logger->flush();
}

#ifdef TESTING
void LogSystem::SetCustomSinkForTesting(std::shared_ptr<spdlog::sinks::sink> sink)
{
    m_impl->logger->sinks().clear();
    m_impl->logger->sinks().push_back(std::move(sink));
}
#endif

