#include "Event.h"

MarketEvent::MarketEvent(const std::chrono::year_month_day& ts, std::shared_ptr<OptionContract> data) : contract(data)
{	

	type = EventType::MarketEvent;
	timestamp = ts;
}

SignalEvent::SignalEvent(const std::chrono::year_month_day ts, std::string& sym, SignalType stype, int qty) : symbol(sym), signaltype(stype), quantity(qty)
{
	type = EventType::SignalEvent;
	timestamp = ts;
}

OrderEvent::OrderEvent(const std::chrono::year_month_day ts, std::string& sym, OrderType ordtype, int qty) : symbol(sym), ordertype(ordtype), quantity(qty)
{
	type = EventType::OrderEvent;
	timestamp = ts;
}

FillEvent::FillEvent(const std::chrono::year_month_day ts, std::string& sym, int qty, OrderType orddirection) : symbol(sym), quantity(qty), direction(orddirection)
{	
	type = EventType::FillEvent;
	timestamp = ts;
}

double FillEvent::calculatecommision()
{

}

double FillEvent::calculateslippage()
{

}