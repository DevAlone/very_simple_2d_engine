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

    void loop();

    void requestExit();

    size_t getTimeMicroseconds() const;

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
