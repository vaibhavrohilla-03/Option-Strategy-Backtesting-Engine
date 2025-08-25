#pragma once

#ifndef OPTION_H
#define OPTION_H

#include <stdio.h>


enum OptionType
{
	Option_Call,
	Option_Put
};

class Option
{
private:
	
	double Strke;
	OptionType TypeofOption;
	double Cost;

public:
	Option();

	Option(const Option& other);

	~Option();

	Option &operator=(const Option& other);

	double valueAtExpiration(double underlyingAtExpiration);
	double profitAtExpirtation(double underlyingAtExpiration);

};

#endif //OPTION_H