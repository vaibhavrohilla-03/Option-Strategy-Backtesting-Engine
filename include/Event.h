#pragma once
#ifndef EVENT_H

#define EVENT_H

enum class EventType
{
	MarketEvent,
	SignalEvent,
	OrderEvent,
	FillEvent
};

class Event
{
public:

	EventType type;
	virtual ~Event() = default;
};

class MarketEvent : public Event
{
public:
	MarketEvent();

};

class SignalEvent : public Event
{
public:
	SignalEvent();
};

class OrderEvent : public Event
{
public: 
	OrderEvent();
};

class FillEvent : public Event
{
public:
	FillEvent();

};
#endif
