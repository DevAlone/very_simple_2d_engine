#include "InputProcessor.h"

#include "EventsProcessor.h"
#include "Exception.h"

#include <functional>

InputProcessor::InputProcessor(const std::shared_ptr<EventsProcessor>& eventsProcessor)
    : eventsProcessor(eventsProcessor)
{
    if (!eventsProcessor) {
        throw Exception("invalid events processor");
    }
}

void InputProcessor::subscribeOnKey(
    Key key,
    const std::function<void(const KeyEvent&)> onKeyPress,
    const std::function<void(const KeyEvent&)> onKeyHold,
    const std::function<void(const KeyEvent&)> onKeyRelease)
{
    if (onKeyPress) {
        keyPressSubscriptions[key].push_back(onKeyPress);
    }
    if (onKeyHold) {
        keyHoldSubscriptions[key].push_back(onKeyHold);
    }
    if (onKeyRelease) {
        keyReleaseSubscriptions[key].push_back(onKeyRelease);
    }
}

void InputProcessor::subscribeOnKey(
    Key key,
    const std::function<void(const KeyEvent&)> onKeyPress,
    const std::function<void(const KeyEvent&)> onKeyRelease)
{
    subscribeOnKey(key, onKeyPress, nullptr, onKeyRelease);
}

void InputProcessor::subscribeOnKey(Key key, const std::function<void()> onKeyPress, const std::function<void()> onKeyHold, const std::function<void()> onKeyRelease)
{
    if (onKeyPress) {
        keyPressSubscriptions[key].push_back(
            [onKeyPress](const KeyEvent&) {
                onKeyPress();
            });
    }
    if (onKeyHold) {
        keyHoldSubscriptions[key].push_back(
            [onKeyHold](const KeyEvent&) {
                onKeyHold();
            });
    }
    if (onKeyRelease) {
        keyReleaseSubscriptions[key].push_back(
            [onKeyRelease](const KeyEvent&) {
                onKeyRelease();
            });
    }
}

void InputProcessor::subscribeOnKey(Key key, const std::function<void()> onKeyPress, const std::function<void()> onKeyRelease)
{
    subscribeOnKey(key, onKeyPress, nullptr, onKeyRelease);
}

void InputProcessor::onModuleCreation()
{
    subscriptionId = eventsProcessor->subscribeOnEvent<KeyEvent>(
        [this](const KeyEvent& keyEvent) {
            this->processKeyEvent(keyEvent);
        });
}

void InputProcessor::onModuleDestruction()
{
    eventsProcessor->unsubscribeFromEvent(subscriptionId);
}

void InputProcessor::processFrame(int32_t)
{
    for (Key key : heldKeys) {
        auto it = keyHoldSubscriptions.find(key);
        if (it != keyHoldSubscriptions.end()) {
            for (const auto& function : it->second) {
                KeyEvent dummyEvent;
                dummyEvent.key = key;
                dummyEvent.keyState = KeyState::PRESSED;
                function(dummyEvent);
            }
        }
    }
}

void InputProcessor::processKeyEvent(const KeyEvent& keyEvent)
{
    switch (keyEvent.keyState) {
    case KeyState::PRESSED: {
        auto it = keyPressSubscriptions.find(keyEvent.key);
        if (it != keyPressSubscriptions.end() && !keyStates[keyEvent.key]) {
            for (const auto& function : it->second) {
                function(keyEvent);
            }
        }
        keyStates[keyEvent.key] = true;
        heldKeys.insert(keyEvent.key);
    } break;
    case KeyState::RELEASED: {
        auto it = keyReleaseSubscriptions.find(keyEvent.key);
        if (it != keyReleaseSubscriptions.end()) {
            for (const auto& function : it->second) {
                function(keyEvent);
            }
        }
        keyStates[keyEvent.key] = false;
        heldKeys.erase(keyEvent.key);
    } break;
    }
}
