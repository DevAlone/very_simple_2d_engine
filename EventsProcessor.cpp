#include "EventsProcessor.h"

void EventsProcessor::processFrame(int32_t deltaTime)
{
    generateEvents(deltaTime);
    // TODO: move events processing here
}

void EventsProcessor::unsubscribeFromEvent(size_t subscriptionId)
{
    // TODO: implement
}
