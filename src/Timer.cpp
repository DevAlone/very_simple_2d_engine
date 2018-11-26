#include "Timer.h"

Timer::Timer(size_t intervalMicroseconds)
    : intervalMicroseconds(intervalMicroseconds)
{
}

void Timer::addHandler(std::function<void()> handler)
{
    handlers.push_back(handler);
}

auto Timer::getHandlers() const
    -> const std::vector<std::function<void()>>&
{
    return handlers;
}

size_t Timer::getIntervalMicroseconds() const
{
    return intervalMicroseconds;
}
