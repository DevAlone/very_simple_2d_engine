#ifndef CORE_H
#define CORE_H

#include <memory>
#include <type_traits>
#include <vector>

class Module;

class Core {
public:
    static Core* getCore();

    template <typename T, typename... Args>
    std::shared_ptr<Module> addModuleOfType(Args... args);

    void loop();

    void requestExit();

private:
    Core();
    Core(const Core& other) = delete;
    Core(Core&& other) = delete;

    bool exitRequested = false;
    std::vector<std::shared_ptr<Module>> modules;
};

template <typename T, typename... Args>
std::shared_ptr<Module> Core::addModuleOfType(Args... args)
{
    static_assert(std::is_base_of<Module, T>::value, "have to be subclass of Module");

    auto module = std::make_shared<T>(args...);
    module->onModuleCreated();
    modules.push_back(module);
    return module;
}

#endif // CORE_H
