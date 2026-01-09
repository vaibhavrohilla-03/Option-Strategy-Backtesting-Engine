#pragma once
#ifndef EVENT_H

#define EVENT_H

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

class Event
{
public:

	EventType type = EventType::None;
	std::chrono::year_month_day timestamp = std::chrono::year_month_day{};

	virtual ~Event() = default;
};



class MarketEvent : public Event
{
private:
	std::shared_ptr<OptionContract> contract;
	
public:
	MarketEvent() = default;
	MarketEvent(const std::chrono::year_month_day& ts, std::shared_ptr<OptionContract> data);

};

class SignalEvent : public Event
{
private:
	std::string symbol;
	SignalType signaltype;
	int quantity;

public:
	SignalEvent() = default;
	SignalEvent(const std::chrono::year_month_day ts, std::string& sym, SignalType stype, int qty);
};

class OrderEvent : public Event
{
private:
	std::string symbol;
	OrderType ordertype;
	int quantity;

public:
	OrderEvent() = default;
	OrderEvent(const std::chrono::year_month_day ts, std::string& sym, OrderType ordtype,  int qty);


};

class FillEvent : public Event
{
private:

	std::string symbol;
	int  quantity;
	OrderType direction;

public:
	double slippage = 0.0;
	double commision = 0.0;

	FillEvent() = default;
	FillEvent(const std::chrono::year_month_day ts, std::string& sym, int qty, OrderType orddirection);

private:
	double calculatecommision();
	double calculateslippage();

};
#endif
