#pragma once

#include "DataHandler.h"
#include "Event.h"
#include "Portfolio.h"

class Strategy {

private:

	inline void bindEngineDependencies(
		std::shared_ptr<EventQueue> queue, 
		std::shared_ptr<DataHandler> data,
		std::shared_ptr<Portfolio> portfolio) {
        m_eventQueue = queue;
        m_dataHandler = data;
		m_portfolio = portfolio;
    }
	
	friend class Backtester;

protected:
	std::shared_ptr<EventQueue> m_eventQueue;
    std::shared_ptr<DataHandler> m_dataHandler;
	std::shared_ptr<Portfolio> m_portfolio;

	inline void pushSignal(std::shared_ptr<SignalEvent> signal) {
		m_eventQueue->push(signal);
	}

public:
    
    const std::string Strategyname;

	inline Strategy(const std::string& name) : Strategyname(name) {}

	virtual ~Strategy() = default;

    virtual void onEvent(std::shared_ptr<Event> event) = 0;

};
