#pragma once

#include "EventsProcessor.h"

class SDLEventsProcessor : public EventsProcessor {
public:
    virtual void generateEvents(int32_t deltaTime) override;
};
