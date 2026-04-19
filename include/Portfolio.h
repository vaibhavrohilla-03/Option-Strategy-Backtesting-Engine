#pragma once

#include <chrono>
#include <unordered_map>

#include "Event.h"

class DataHandler;

struct PortfolioHoldings {

	std::chrono::year_month_day timestamp;
	
	double cash = 0.0;
	double commision = 0.0;
	double total_equity = 0.0;

	double riskfree_rate = 0.0;

	std::unordered_map<std::string, double> market_value;

};

class Portfolio {

private:

	std::shared_ptr<EventQueue> events;
    
    std::shared_ptr<DataHandler> data;

	std::unordered_map<std::string, int> current_positions;

	double initialcapital = 0.0;

	double currentcapital = 0.0;

	double total_commission_paid = 0.0;
	double total_slippage_paid = 0.0;

	double pending_commission = 0.0;

	std::vector<PortfolioHoldings> all_holdings;

	void create_equity_dataframe_CSV_and_Metrics(const std::string& filepath);

	friend class Backtester;

public:
	
	Portfolio(std::shared_ptr<EventQueue> eventQueue, std::shared_ptr<DataHandler> data, double capital);

	void on_signal(std::shared_ptr<SignalEvent> event);

	void on_fill(std::shared_ptr<FillEvent> event);

	void UpdatePositions(std::shared_ptr<Event> event);

	std::unique_ptr<OrderEvent> GenerateOrder(std::shared_ptr<SignalEvent> event);

	void UpdateTimeindex(std::shared_ptr<Event> event);

	const std::unordered_map<std::string, int>& getPositions() const { return current_positions; }
	bool hasPosition(const std::string& symbol) const { return current_positions.count(symbol) > 0; }
	
	int getPositionQuantity(const std::string& symbol) const;
	double getCurrentCapital() const { return currentcapital; }


};