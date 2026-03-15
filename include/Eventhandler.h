#pragma once

#include <memory>

#include "Event.h"

class EventHandler
{
public:
	virtual ~EventHandler() = default;
	
	virtual void onEvent(std::shared_ptr<Event> event) = 0;

	virtual void setEventQueue(EventQueue* q) = 0;
};


