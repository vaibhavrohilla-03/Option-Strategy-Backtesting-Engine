#pragma once

#include <chrono>
#include <format>
#include <sstream>
#include <string>

namespace Utils {

	inline std::string fromNanos(unsigned long long ns) {
    	
    	std::chrono::system_clock::time_point tp{ std::chrono::duration_cast<std::chrono::system_clock::duration>
    										( std::chrono::nanoseconds(ns)) };
    	
    	std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(tp)};
    	return std::format("{:%Y-%m-%d}", ymd);
	
	}

	inline std::string formatTime(long long ms) {
		
		std::chrono::system_clock::time_point tp{std::chrono::milliseconds(ms)};
	
		std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(tp)};
		return std::format("{:%d-%b-%y}", ymd);
	
	}

	inline std::string ymdToDMonY(const std::string& ymd_str) {
		
		std::chrono::year_month_day ymd_date;
		std::istringstream in{ymd_str};
	
		std::chrono::from_stream(in, "%Y-%m-%d", ymd_date);
		return std::format("{:%d-%b-%y}", ymd_date);
	
	}

} 