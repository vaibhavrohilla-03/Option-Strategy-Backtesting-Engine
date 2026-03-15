#pragma once

#include <chrono>
#include <unordered_map>

#include "Event.h"

struct PortfolioHoldings {

	std::chrono::year_month_day timestamp;
	
	double cash = 0.0;
	double commision = 0.0;
	double total_equity = 0.0;

	std::unordered_map<std::string, double> market_value;

};

class Portfolio {

private:

	std::chrono::year_month_day startDate = std::chrono::year_month_day{};

	std::chrono::year_month_day endDate = std::chrono::year_month_day{};

	std::unordered_map<std::string, int> current_positions;

	double initialcapital = 0.0;

	double currentcapital = 0.0;

	std::vector<PortfolioHoldings> all_holdings;

public:
	

	Portfolio(double capital);

	~Portfolio() = default;


	void on_signal(std::shared_ptr<SignalEvent> event);

	void on_fill(std::shared_ptr<FillEvent> event);

	void UpdatePositions(std::shared_ptr<Event> event);

	void GenerateOrder(std::shared_ptr<SignalEvent> event);

	void UpdateTimeindex(std::shared_ptr<Event> event);

	inline float GetCurrentCapital() const {return currentcapital; }

	void create_equity_dataframe_CSV(const std::string& filepath);


};