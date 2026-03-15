#pragma once

#include <memory>

#include "Eventhandler.h"
#include "Event.h"

class ExecutionHandler : public EventHandler {

protected:
	EventQueue* eventQueue = nullptr;

public:

	virtual ~ExecutionHandler() = default;

	inline void setEventQueue(EventQueue* q) override {
		eventQueue = q;
	}
		
	inline void onEvent(std::shared_ptr<Event> event) override {
		if (event->type == EventType::OrderEvent) {
            auto orderEvent = std::static_pointer_cast<OrderEvent>(event);
            executeOrder(orderEvent);

		}
	}

	virtual void executeOrder(std::shared_ptr<OrderEvent> order) = 0;

};