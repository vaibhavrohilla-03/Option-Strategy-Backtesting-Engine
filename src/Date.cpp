#include "Date.h"
#include <stdexcept>

Date::Date(int year, int month, int day) : m_year(year), m_month(month), m_day(day)
{
	if (month < 1 || month > 12 || day < 1 || day > daysinMonth(year, month))
	{
		throw std::invalid_argument("Invalid date provided to constructor.");
	}
	calculateDayOfWeek();
}

Date::Date(const std::string& iso_date)
{
	if (iso_date.length() != 10 || iso_date[4] != '-' || iso_date[7] != '-')
	{
		throw std::invalid_argument("Invalid ISO date format. Expected YYYY-MM-DD.");
	}

	try
	{
		m_year = std::stoi(iso_date.substr(0, 4));
		m_month = std::stoi(iso_date.substr(5, 2));
		m_day = std::stoi(iso_date.substr(8, 2));
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Could not parse date components from string.");
	}

	if (m_month < 1 || m_month > 12 || m_day < 1 || m_day > daysinMonth(m_year, m_month))
	{
		throw std::invalid_argument("Invalid date parsed from string.");
	}

	calculateDayOfWeek();
}

Date::~Date()
{

}

Date::Date(const Date& other) : m_year(other.m_year), m_month(other.m_month), m_day(other.m_day), m_dayofweek(other.m_dayofweek)
{
}

Date& Date::operator=(const Date& other) 
{
	if (this != &other)
	{
		m_year = other.m_year;
		m_month = other.m_month;
		m_day = other.m_day;
		m_dayofweek = other.m_dayofweek;
	}

	return *this;
}

Date& Date::operator++()
{
	m_day++;
	if (m_day > daysinMonth(m_year, m_month))
	{
		m_day = 1;
		m_month++;
		if (m_month > 12)
		{
			m_month = 1;
			m_year++;
		}
	}
	if (m_dayofweek != DayofTheWeek::None)
	{
		m_dayofweek = (m_dayofweek == DayofTheWeek::Saturday) ? DayofTheWeek::Sunday : static_cast<DayofTheWeek>(static_cast<int>(m_dayofweek) + 1);
	}
	return *this;
}

Date& Date::operator--()
{
	m_day--;
	if (m_day < 1)
	{
		m_month--;
		if (m_month < 1)
		{
			m_month = 12;
			m_year--;
		}
		m_day = daysinMonth(m_year, m_month);
	}

	if (m_dayofweek != DayofTheWeek::None)
	{
		m_dayofweek = (m_dayofweek == DayofTheWeek::Sunday) ? DayofTheWeek::Saturday : static_cast<DayofTheWeek>(static_cast<int>(m_dayofweek) - 1);
	}
	return *this;
}


int Date::operator-(const Date& other) const
{
	return this->datetoDays() - other.datetoDays();
}



bool Date::operator<(const Date& other) const
{
	if (m_year < other.m_year) return true;
	if (m_year > other.m_year) return false;
	if (m_month < other.m_month) return true;
	if (m_month > other.m_month) return false;
	return m_day < other.m_day;
}

bool Date::operator>(const Date& other) const
{	
	return other < *this;
}

bool Date::operator==(const Date& other) const
{	
	return m_year == other.m_year && m_month == other.m_month && m_day == other.m_day;
}

bool Date::operator!=(const Date& other) const
{	
	return !(*this == other);
}

std::string Date::toString() const
{
	std::string year_str = std::to_string(m_year);
	std::string month_str = (m_month < 10 ? "0" : "") + std::to_string(m_month);
	std::string day_str = (m_day < 10 ? "0" : "") + std::to_string(m_day);
	return year_str + "-" + month_str + "-" + day_str;
}

DayofTheWeek Date::dayoftheWeek() const
{
	return m_dayofweek;
}

bool Date::isWeekday()
{
	return m_dayofweek != DayofTheWeek::Saturday && m_dayofweek != DayofTheWeek::Sunday;
}

bool Date::isLeapYear()
{
	return isLeapYear(m_year);
}

bool Date::isLeapYear(int year)
{	
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysinMonth(int year, int month)
{	
	if (month == 2)
	{
		return isLeapYear(year) ? 29 : 28;
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		return 30;
	}
	else
	{
		return 31;
	}
}

void Date::calculateDayOfWeek()
{ 
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };                            // REMIND ME : STUDY THIS ALGO ONCE MORE 
	int y = m_year;
	int m = m_month;
	int d = m_day;
	if (m < 3)
	{
		y -= 1;
	}
	int dayIndex = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;  
	m_dayofweek = static_cast<DayofTheWeek>(dayIndex + 1);
}

long Date::datetoDays() const
{
	long y = m_year;
	long m = m_month;
	long d = m_day;                         
	if (m < 3) {
		y--;
		m += 12;
	}
	//Gregorian to Julian Day Number formula                                   // REMIND ME : LOOK IT UP ONCE MORE 
	return 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5 + d - 306;
}
