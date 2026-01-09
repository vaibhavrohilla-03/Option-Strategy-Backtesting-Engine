#pragma once

#ifndef STRATEGY_H
#define STRATEGY_H

#include "Eventhandler.h"
#include "Event.h"

class Strategy : public EventHandler
{
protected:
	EventQueue* eventQueue;

public:
	Strategy() : eventQueue(nullptr) {}

};


#endif