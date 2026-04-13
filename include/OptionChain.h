#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>

#include "Options.h"

class OptionChain {

private:

	std::chrono::year_month_day Date;
	std::vector<OptionContract> chain;

	std::unordered_map<std::string, size_t> symbol_index;

public:

	OptionChain(std::chrono::year_month_day date, std::vector<OptionContract> contracts);

	inline const std::chrono::year_month_day& getDate() const { return Date; }
    
    inline const std::vector<OptionContract>& getAllContracts() const {return chain; }


    std::vector<const OptionContract*> getContracts(OptionType type, std::chrono::year_month_day expiry) const;
    
    std::vector<const OptionContract*> getContractsByStrikeRange(double min_strike, double max_strike, OptionType type, std::chrono::year_month_day expiry) const;

    std::vector<std::chrono::year_month_day> getAvailableExpiries() const;

    std::vector<double> getAvailableStrikes(std::chrono::year_month_day expiry, 
                                             OptionType type) const;

    const OptionContract* findContract(double strike, OptionType type, std::chrono::year_month_day expiry) const;
    
    const OptionContract* findBySymbol(const std::string& symbol) const;

    OptionContract* findBySymbol(const std::string& symbol);
   
    const OptionContract* getATM(OptionType type, std::chrono::year_month_day expiry) const;
    
    double getUnderlyingPrice() const;



};