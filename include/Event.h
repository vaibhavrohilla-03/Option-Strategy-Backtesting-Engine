#pragma once

#include <string>
#include <chrono>
#include <queue>
#include <memory>

#include "Options.h"

class Event;
using EventQueue = std::queue<std::shared_ptr<Event>>;

enum class EventType
{
	MarketEvent,
	SignalEvent,
	OrderEvent,
	FillEvent,
	None
};

enum class SignalType
{
	Long,
	Short,
	Exit
};

enum class OrderType
{
	Buy,
	Sell
};

enum class OrderMode 
{
	Market,
	Limit,
	Stop

};

class Event
{
public:

	EventType type = EventType::None;
	std::chrono::year_month_day timestamp = std::chrono::year_month_day{};

	virtual ~Event() = default;
};



class MarketEvent : public Event
{
public:
	std::shared_ptr<OptionContract> contract;
	
	MarketEvent() = default;
	MarketEvent(const std::chrono::year_month_day& ts, std::shared_ptr<OptionContract> data);

};

class SignalEvent : public Event
{
public:
	std::string symbol;
	SignalType signaltype;
	
	int quantity;

	OrderMode suggestedmode;
    double limitprice;

	SignalEvent() = default;
	SignalEvent(const std::chrono::year_month_day ts, std::string& sym, SignalType stype, int qty, OrderMode mode = OrderMode::Market, double price = 0.0);
};

class OrderEvent : public Event
{
public:
	
	std::string symbol;
	OrderType ordertype;
	int quantity;

	double targetprice; 

	OrderMode ordermode;

	OrderEvent() = default;
	OrderEvent(const std::chrono::year_month_day ts, const std::string& sym, OrderType ordtype, OrderMode mode,  int qty, double tprice = 0.0);


};

class FillEvent : public Event
{
public:

	std::string symbol;
	int  quantity;
	OrderType direction;


	double fillPrice = 0.0;

	double slippage = 0.0;
	double commision = 0.0;

	int multiplier;
	
	FillEvent() = default;
	FillEvent(const std::chrono::year_month_day ts, std::string& sym, int qty, double price, OrderType orddirection, double slippage, double commision, int mlptier);
};