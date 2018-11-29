#pragma once

#include "Module.h"

#include <memory>
#include <type_traits>
#include <vector>

class Core {
public:
    static Core* getCore();
    ~Core();

    template <typename T, typename... Args>
    std::shared_ptr<T> addModuleOfType(Args... args);

    template <typename T>
    std::shared_ptr<T> getModuleOfType();

    void loop();

    void requestExit();

    static size_t getTimeMicroseconds();

private:
    Core();
    Core(const Core& other) = delete;
    Core(Core&& other) = delete;

    bool exitRequested = false;
    std::vector<std::shared_ptr<Module>> modules;
};

template <typename T, typename... Args>
std::shared_ptr<T> Core::addModuleOfType(Args... args)
{
    static_assert(std::is_base_of<Module, T>::value, "have to be subclass of Module");

    auto module = std::make_shared<T>(args...);
    modules.push_back(module);
    return module;
}

template <typename T>
std::shared_ptr<T> Core::getModuleOfType()
{
    static_assert(std::is_base_of<Module, T>::value, "have to be subclass of Module");

    for (const auto& module : modules) {
        if (auto moduleOfType = std::dynamic_pointer_cast<T>(module)) {
            return moduleOfType;
        }
    }

    return {};
}
