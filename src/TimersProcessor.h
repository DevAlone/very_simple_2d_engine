#pragma once

#include "Module.h"
#include "Timer.h"

#include <memory>

class TimersProcessor : public Module {
public:
    TimersProcessor();

    /**
     * @brief createTimer - creates and enables new timer
     * @param intervalMicroseconds - interval in microseconds
     * @return - timer itself
     */
    auto createTimer(size_t intervalMicroseconds) -> std::shared_ptr<Timer>;

    virtual void processFrame(int32_t deltaTime) override;

private:
    std::vector<std::shared_ptr<Timer>> timers;
};
