#include "Core.h"
#include "Module.h"

#include <chrono>

using CurrentClock = std::chrono::high_resolution_clock;

Core::Core()
{
}

Core* Core::getCore()
{
    static Core* core = nullptr;
    if (core == nullptr) {
        core = new Core();
    }
    return core;
}

Core::~Core()
{
    for (const auto& module : modules) {
        module->onModuleDestruction();
    }
}

void Core::loop()
{
    for (const auto& module : modules) {
        module->onModuleCreation();
    }

    auto previousTime = CurrentClock::now();

    while (!exitRequested) {
        auto currentTime = CurrentClock::now();
        int64_t deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime).count();

        for (const auto& module : modules) {
            module->processFrame(static_cast<int32_t>(deltaTime));
        }

        previousTime = currentTime;
    }
}

void Core::requestExit()
{
    exitRequested = true;
}

size_t Core::getTimeMicroseconds()
{
    return static_cast<size_t>(
        std::chrono::time_point_cast<std::chrono::microseconds>(
            CurrentClock::now())
            .time_since_epoch()
            .count());
}
