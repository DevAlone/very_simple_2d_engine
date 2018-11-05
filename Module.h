#ifndef MODULE_H
#define MODULE_H

#include <cstdint>

class Module {
public:
    Module();
    virtual ~Module() {}

    /**
     * @brief onModuleCreated - called on module creation
     */
    virtual void onModuleCreated() {}

    /**
     * @brief processFrame - called for each module per each frame
     * @param deltaTime - time in microseconds
     */
    virtual void processFrame(int32_t deltaTime);
};

#endif // MODULE_H
