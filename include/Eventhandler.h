#pragma once 
#ifndef EVENTHANDLER_H


#define EVENTHANDLER_H

#include <memory>

class Event;

class EventHandler
{
public:
	virtual ~EventHandler() = default;
	
	virtual void onEvent(std::shared_ptr<Event> event) = 0;

	virtual void setEventQueue() = 0;
};


#endif
