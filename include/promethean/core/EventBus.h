#pragma once

#include "core/LogSystem.h"
#include <any>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <typeindex>
#include <vector>
#include <algorithm>

/**
 * @brief Global publish/subscribe event bus (thread-safe, non-blocking).
 *
 * Events are plain structs passed by const reference inside std::any.
 */
class EventBus
{
public:
    /// Singleton accessor
    static EventBus& Instance();

    /** Type alias for a generic handler */
    using Handler = std::function<void(const std::any&)>;

    /**
     * @brief Subscribe to all events of type T.
     * @tparam T  Plain struct representing the event
     * @param handler Callback executed on publication (executed synchronously).
     * @return subscription id; retain if you want to unsubscribe later.
     */
    template<class T>
    size_t Subscribe(Handler handler);

    /**
     * @brief Unsubscribe via id returned by Subscribe.
     */
    void Unsubscribe(size_t id);

    /**
     * @brief Publish an event of type T (synchronous).
     */
    template<class T>
    void Publish(const T& event);

    /** Disable copy */
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

private:
    EventBus() = default;
    ~EventBus() = default;

    struct Subscription {
        size_t id;
        Handler handler;
    };

    std::unordered_map<std::type_index, std::vector<Subscription>> m_subs;
    std::mutex m_mutex;
    size_t m_nextId{0};
};

// ===== Template definitions =====

template<class T>
inline size_t EventBus::Subscribe(Handler handler)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const size_t id = m_nextId++;
    auto& vec = m_subs[std::type_index(typeid(T))];
    vec.push_back({id, std::move(handler)});
    LogSystem::Instance().Debug("Subscribed handler {} for {}", id, typeid(T).name());
    return id;
}

template<class T>
inline void EventBus::Publish(const T& event)
{
    std::vector<Subscription> subsCopy;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_subs.find(std::type_index(typeid(T)));
        if (it != m_subs.end())
            subsCopy = it->second;
    }

    LogSystem::Instance().Debug("Publishing {} to {} subscriber(s)", typeid(T).name(), subsCopy.size());

    std::any payload = std::cref(event);
    for (const auto& sub : subsCopy)
    {
        try {
            sub.handler(payload);
        } catch (...) {
            LogSystem::Instance().Error("Exception in event handler {}", sub.id);
        }
    }
}

