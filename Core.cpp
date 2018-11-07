#include "Core.h"
#include "Module.h"

#include <chrono>

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
    using clock = std::chrono::high_resolution_clock;
    auto previousTime = clock::now();

    while (!exitRequested) {
        auto currentTime = clock::now();
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
