#pragma once

#ifndef OPTIONS_H
#define OPTIONS_H

#include <chrono>
#include <iostream>
#include <string>
enum class OptionType
{
	Option_Call,
	Option_Put
};

std::ostream& operator<<(std::ostream& os, OptionType type);


inline OptionType Type(const std::string& str)
{
	if (str == "CE") {
		return OptionType::Option_Call;
	}
	if (str == "PE") {
		return OptionType::Option_Put;
	}

	throw std::invalid_argument("Invalid Option type string");
}



struct Greeks
{
	double delta = 0;
	double gamma = 0;
	double theta = 0;
	double vega = 0;
	double rho = 0;

};

struct Marketdata
{	
	double underlying_price = 0;

	double open = 0;
	double high = 0;
	double low = 0;
	double close = 0;
	double implied_volatility = 0;
	long long volume = 0;
	long long open_interest = 0;
};


class OptionContract
{
private:
	
	std::chrono::year_month_day Date;

	double strike_price;
	
	OptionType TypeofOption;
	std::string symbol;

	std::chrono::year_month_day expiration;

	Marketdata marketdata;
	Greeks greeks;

public:
	OptionContract() = default;

	OptionContract(std::chrono::year_month_day date, double strike, OptionType type, std::string symbol, std::chrono::year_month_day expiry, Marketdata data, Greeks greeks);

	~OptionContract() = default;

	OptionContract &operator=(const OptionContract& other) = default;

	bool operator<(const OptionContract& other);

	double valueAtExpiration(double underlyingAtExpiration);
	double profitAtExpiration(double underlyingAtExpiration);

	const std::chrono::year_month_day& getDate() const { return Date; }

	double getStrike() const { return strike_price; }
	OptionType getType() const { return TypeofOption; }

	const std::string& getSymbol() const { return symbol; }

	const std::chrono::year_month_day& getExpiration() const { return expiration; }

	const Marketdata& getMarketdata() const { return marketdata; }

	const Greeks& getGreeks() const { return greeks; }

	void setMarketdata(const Marketdata& data){ marketdata = data; }

	void setGreeks(const Greeks& g) {greeks = g;}
};

#endif 