#include "Event.h"

MarketEvent::MarketEvent(const std::chrono::year_month_day& ts, std::shared_ptr<OptionContract> data) : contract(data)
{	

	type = EventType::MarketEvent;
	timestamp = ts;
}

SignalEvent::SignalEvent(const std::chrono::year_month_day ts, const std::string& sym, SignalType stype, int qty, OrderMode mode, double price)
						 : symbol(sym), signaltype(stype), quantity(qty), suggestedmode(mode), limitprice(price)
{
	type = EventType::SignalEvent;
	timestamp = ts;
}

OrderEvent::OrderEvent(const std::chrono::year_month_day ts, const std::string& sym, OrderType ordtype, OrderMode mode, int qty, double tprice)
						 : symbol(sym), ordertype(ordtype), ordermode(mode),  quantity(qty), targetprice(tprice)
{
	type = EventType::OrderEvent;
	timestamp = ts;
}

FillEvent::FillEvent(const std::chrono::year_month_day ts, const std::string& sym, int qty, double price, OrderType orddirection, double slippage, double commision, int mlptier)
					: symbol(sym), quantity(qty), fillPrice(price), direction(orddirection), slippage(slippage), commision(commision), multiplier(mlptier)
{	
	type = EventType::FillEvent;
	timestamp = ts;
}
