#include "core/EventBus.h"

EventBus& EventBus::Instance()
{
    static EventBus instance;
    return instance;
}

void EventBus::Unsubscribe(size_t id)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& [type, vec] : m_subs)
    {
        auto it = std::find_if(vec.begin(), vec.end(), [&](const Subscription& s) { return s.id == id; });
        if (it != vec.end())
        {
            vec.erase(it);
            LogSystem::Instance().Debug("Unsubscribed handler {}", id);
            return;
        }
    }
    LogSystem::Instance().Warn("Attempted to unsubscribe unknown id {}", id);
}

