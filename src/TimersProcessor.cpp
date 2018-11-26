#include "TimersProcessor.h"
#include "Core.h"

TimersProcessor::TimersProcessor()
{
}

auto TimersProcessor::createTimer(size_t intervalMicroseconds)
    -> std::shared_ptr<Timer>
{
    auto timer = std::make_shared<Timer>(intervalMicroseconds);
    timers.push_back(timer);
    return timer;
}

void TimersProcessor::processFrame(int32_t)
{
    auto currentTime = Core::getTimeMicroseconds();

    for (auto& timer : timers) {
        if (timer->lastProcessingTime + timer->intervalMicroseconds < currentTime) {
            for (const auto& handler : timer->getHandlers()) {
                handler();
            }
            timer->lastProcessingTime = currentTime;
        }
    }
}
