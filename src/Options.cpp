#include "Options.h"
#include <utility>
#include <iostream>
#include <chrono>


std::ostream& operator<<(std::ostream& os, OptionType type)
{
	switch (type)
	{
	case OptionType::Option_Call:
		os << "CE";
		break;

	case OptionType::Option_Put:
		os << "PE";
		break;

	default:
		os << "not defined";
		break;

	}

	return os;
}


OptionContract::OptionContract(std::chrono::year_month_day date, double strike, OptionType type, std::string symbol, std::chrono::year_month_day expiry, Marketdata data, Greeks greeks)
	: Date(date),
	strike_price(strike),
	TypeofOption(type),
	symbol(std::move(symbol)),
	expiration(expiry),
	marketdata(data),
	greeks(greeks)
{}



bool OptionContract::operator<(const OptionContract& other)
{
	return this->Date < other.Date;
}

double OptionContract::valueAtExpiration(double underlyingAtExpiration)
{
	double value = 0.0;

	if (TypeofOption == OptionType::Option_Call)
	{
		if (strike_price < underlyingAtExpiration)
		{
			value = underlyingAtExpiration - strike_price;
		}
	}
	else 
	{
		if (strike_price > underlyingAtExpiration)
		{
			value = strike_price - underlyingAtExpiration;
		}
	}
	return value;
}

double OptionContract::profitAtExpiration(double underlyingAtExpiration)
{
	double finalValue = valueAtExpiration(underlyingAtExpiration);
	double premiumPaid = marketdata.close; 

	return finalValue - premiumPaid;
}
