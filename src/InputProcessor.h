#pragma once

#include "Module.h"

#include "Key.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class EventsProcessor;
struct KeyEvent;

class InputProcessor : public Module {
public:
    InputProcessor(
        const std::shared_ptr<EventsProcessor>& eventsProcessor);

    void subscribeOnKey(
        Key key,
        const std::function<void(const KeyEvent& event)> onKeyPress,
        const std::function<void(const KeyEvent& event)> onKeyHold,
        const std::function<void(const KeyEvent& event)> onKeyRelease);

    void subscribeOnKey(
        Key key,
        const std::function<void(const KeyEvent& event)> onKeyPress,
        const std::function<void(const KeyEvent& event)> onKeyRelease);

    void subscribeOnKey(
        Key key,
        const std::function<void(const KeyEvent& event)> onKeyPress);

    void subscribeOnKey(
        Key key,
        const std::function<void()> onKeyPress,
        const std::function<void()> onKeyHold,
        const std::function<void()> onKeyRelease);

    void subscribeOnKey(
        Key key,
        const std::function<void()> onKeyPress,
        const std::function<void()> onKeyRelease);

    void subscribeOnKey(
        Key key,
        const std::function<void()> onKeyPress);

    virtual void onModuleCreation() override;

    virtual void onModuleDestruction() override;

    virtual void processFrame(int32_t deltaTime) override;

private:
    void processKeyEvent(const KeyEvent& keyEvent);

    const std::shared_ptr<EventsProcessor> eventsProcessor;
    size_t subscriptionId = 0;
    std::unordered_map<Key, std::vector<std::function<void(const KeyEvent& event)>>> keyPressSubscriptions;
    std::unordered_map<Key, std::vector<std::function<void(const KeyEvent& event)>>> keyHoldSubscriptions;
    std::unordered_map<Key, std::vector<std::function<void(const KeyEvent& event)>>> keyReleaseSubscriptions;
    std::unordered_map<Key, bool> keyStates;
    std::unordered_set<Key> heldKeys;
};
