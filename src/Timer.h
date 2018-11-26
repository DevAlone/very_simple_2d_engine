#pragma once

#include <functional>
#include <vector>

class Timer {
    friend class TimersProcessor;

public:
    Timer(size_t intervalMicroseconds);

    void addHandler(std::function<void()> handler);
    auto getHandlers() const -> const std::vector<std::function<void()>>&;
    auto getIntervalMicroseconds() const -> size_t;

private:
    std::vector<std::function<void()>> handlers;
    size_t intervalMicroseconds;
    size_t lastProcessingTime = 0;
};
