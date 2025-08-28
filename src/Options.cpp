#include "Options.h"

Option::Option(double m_Strike, OptionType type, double m_Cost) : m_Strike(m_Strike), TypeofOption(type), m_Cost(m_Cost){}

Option::Option(const Option& other) : m_Strike(other.m_Strike), TypeofOption(other.TypeofOption), m_Cost(other.m_Cost) {}

Option::~Option(){}

Option& Option::operator=(const Option& other)
{
	if (this != &other)
	{
		TypeofOption = other.TypeofOption;
		m_Strike = other.m_Strike;
		m_Cost = other.m_Cost;
	}
	return *this;
}

double Option::valueAtExpiration(double underlyingatExpirtation)
{	
	double value = 0.0;
	switch (TypeofOption)
	{
		case Option_Call:

			if(m_Strike < underlyingatExpirtation)
			{
				value = underlyingatExpirtation - m_Strike;
			}
			break;
		case Option_Put:

			if (m_Strike > underlyingatExpirtation)
			{
				value = m_Strike - underlyingatExpirtation;
			}
			break;
	}
	return value;
}

double Option::profitAtExpirtation(double underlyingatExpiration)
{
	double IntrensicValue = valueAtExpiration(underlyingatExpiration);
	if (IntrensicValue > m_Cost)
	{
		return IntrensicValue - m_Cost;
	}
	return 0.0;
}