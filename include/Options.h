#pragma once

#ifndef OPTION_H
#define OPTION_H


enum OptionType
{
	Option_Call,
	Option_Put
};

class Option
{
private:
	
	double m_Strike;
	OptionType TypeofOption;
	double m_Cost;

public:
	Option(double m_Strike,OptionType type, double m_Cost);

	Option(const Option& other);

	~Option();

	Option &operator=(const Option& other);

	double valueAtExpiration(double underlyingAtExpiration);
	double profitAtExpirtation(double underlyingAtExpiration);

};

#endif 