#pragma once

#ifndef DATE_H
#define DATE_H

#include <string>
enum class DayofTheWeek
{
	Sunday = 1,
	Monday = 2,
	Tuesday = 3,
	Wednesday = 4,
	Thursday = 5,
	Friday = 6,
	Saturday = 7,
	None = 8
};

enum class Month
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December,
};


class Date
{
private:

	int m_year;
	int m_month;
	int m_day;

	DayofTheWeek m_dayofweek;

	bool isLeapYear(int year);

	int daysinMonth(int year, int month);

	void calculateDayOfWeek();

	long datetoDays() const;


public:

	Date(int year, int month, int day);
	Date(const std::string& iso_date);
	~Date();
	Date(const Date& other);

	Date& operator=(const Date& other);
	Date& operator++();
	Date& operator--();
	int operator-(const Date& other) const;
	bool operator<(const Date& other) const;
	bool operator>(const Date& other) const;
	bool operator==(const Date& other) const;
	bool operator!=(const Date& other) const;

	std::string toString() const;
	DayofTheWeek dayoftheWeek() const;
	bool isWeekday();
	bool isLeapYear();


};


#endif 
