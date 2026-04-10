#pragma once

#include "DataHandler.h"
#include "Event.h"

class Strategy {

private:

	inline void bindEngineDependencies(std::shared_ptr<EventQueue> queue, std::shared_ptr<DataHandler> data) {
        m_eventQueue = queue;
        m_dataHandler = data;
    }
	
	friend class Backtester;

protected:
	std::shared_ptr<EventQueue> m_eventQueue;
    std::shared_ptr<DataHandler> m_dataHandler;



public:
    
    const std::string Strategyname;

	inline Strategy(const std::string& name) : Strategyname(name) {}

	virtual ~Strategy() = default;

    virtual void onEvent(std::shared_ptr<Event> event) = 0;

};
