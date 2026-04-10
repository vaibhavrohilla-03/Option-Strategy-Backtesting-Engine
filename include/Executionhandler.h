#pragma once

#include <memory>

#include "Eventhandler.h"
#include "Event.h"
#include "DataHandler.h"
#include "BrokerModel.h"

class ExecutionHandler : public EventHandler {

private:

	EventQueue* eventQueue;
    std::shared_ptr<DataHandler> dataHandler;

    BrokerConfig brokerConfig;

    inline virtual void executeOrder(std::shared_ptr<OrderEvent> order) {
		
		const std::string symbol = order->symbol;

		double price = dataHandler->getLatestPrice(symbol);
		int multiplier = dataHandler->getMultiplier(symbol);

		double slippage = brokerConfig.slippage(price);

		double fillprice = price;
        switch (order->ordertype) {
            
            case OrderType::Buy:
                fillprice += slippage;
                break;
           
            case OrderType::Sell:
                fillprice -= slippage;
                break;
        }

        double commision = brokerConfig.commission(order->quantity, fillprice, multiplier);

        auto fillevent = std::make_shared<FillEvent>
        				(order->timestamp, order->symbol, order->quantity, fillprice, order->ordertype, slippage, commision, multiplier);

        eventQueue->push(fillevent);

	}


public:

	inline ExecutionHandler(EventQueue* queue, std::shared_ptr<DataHandler> datahandler, BrokerConfig config) 
							: dataHandler(datahandler), brokerConfig(config) {

			setEventQueue(queue);
	}

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

};