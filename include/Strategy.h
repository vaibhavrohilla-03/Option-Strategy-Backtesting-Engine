#pragma once

#include "Eventhandler.h"
#include "Event.h"

class Strategy : public EventHandler
{
protected:
	EventQueue* eventQueue;

public:
	Strategy() : eventQueue(nullptr) {}

	
};
