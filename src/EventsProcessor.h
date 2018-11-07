#pragma once

#include "Key.h"
#include "Module.h"

#include <functional>

struct Event {
};

enum class KeyState : bool {
    PRESSED,
    RELEASED,
};

struct KeyEvent : public Event {
    Key key;
    KeyState keyState;
};

class EventsProcessor : public Module {
public:
    /**
     * @brief generateEvents - should be using pushEvent to generate events.
     * Implement in your input processor
     * @param deltaTime - time since previous frame
     */
    virtual void generateEvents(int32_t deltaTime) = 0;

    virtual void processFrame(int32_t deltaTime) override;

    /**
     * @brief subscribeOnEvent - subscribes on event and returns subscription id
     * @param callback - function which will be called when event happens
     * @return - subscription id, used to unsubscribe from event
     */
    template <typename EventType>
    size_t subscribeOnEvent(
        const std::function<void(const EventType& event)>& callback);

    void unsubscribeFromEvent(size_t subscriptionId);

    template <typename EventType>
    void pushEvent(const EventType& event);

private:
    // std::unordered_map<size_t,
    struct FunctionWrapper {
    };
    std::unordered_map<
        size_t,
        std::vector<std::function<void(const Event* event)>>>

        subscriptionsByEventType;
    size_t lastSubscriptionId = 0;
};

template <typename EventType>
size_t EventsProcessor::subscribeOnEvent(
    const std::function<void(const EventType&)>& callback)
{
    subscriptionsByEventType[typeid(EventType).hash_code()].push_back(
        [callback](const Event* event) {
            callback(static_cast<const EventType&>(*event));
        });

    return ++lastSubscriptionId;
}

template <typename EventType>
void EventsProcessor::pushEvent(const EventType& event)
{
    static_assert(
        std::is_base_of<Event, EventType>::value,
        "have to be subclass of Event");

    auto it = subscriptionsByEventType.find(typeid(EventType).hash_code());
    if (it != subscriptionsByEventType.end()) {
        for (const auto& function : it->second) {
            function(&event);
        }
    }
}
