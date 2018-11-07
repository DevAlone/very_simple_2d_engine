#ifndef MODULE_H
#define MODULE_H

#include <cstdint>

class Module {

public:
    Module();
    virtual ~Module() {}

    /**
     * @brief onModuleCreation - called on module creation
     */
    virtual void onModuleCreation() {}

    /**
     * @brief onModuleDestruction - called on module destroy
     */
    virtual void onModuleDestruction() {}

    /**
     * @brief processFrame - called for each module per each frame
     * @param deltaTime - time in microseconds since previous frame
     */
    virtual void processFrame(int32_t deltaTime);
};

#endif // MODULE_H
