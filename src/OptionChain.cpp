#include "OptionChain.h"

#include <algorithm>
#include <cmath>
#include <limits>

OptionChain::OptionChain(std::chrono::year_month_day date, std::vector<OptionContract> contracts) : Date(date), chain(std::move(contracts)) {

		for(size_t i = 0; i < chain.size(); ++i) {

			symbol_index[chain[i].getSymbol()] = i; 

		}

	}

std::vector<const OptionContract*> OptionChain::getContracts(OptionType type, std::chrono::year_month_day expiry) const {

	std::vector<const OptionContract*> query;

	for(const auto& contract : chain) {

		if(contract.getType() == type) {

			if(contract.getExpiration() == expiry) { query.push_back(&contract);}

		}

	}

	return query;
}


std::vector<const OptionContract*> OptionChain::getContractsByStrikeRange(double min_strike, double max_strike, 
																			OptionType type, std::chrono::year_month_day expiry) const {
    
    std::vector<const OptionContract*> query;
    for (const auto& contract : chain) {
        
        if (contract.getType() == type && contract.getExpiration() == expiry && contract.getStrike() >= min_strike && contract.getStrike() <= max_strike) {
            query.push_back(&contract);
        }
    }
    return query;
}

std::vector<std::chrono::year_month_day> OptionChain::getAvailableExpiries() const {

	std::vector<std::chrono::year_month_day> query;

	for(const auto& contract : chain) {
		query.push_back(contract.getExpiration());
	}

    std::sort(query.begin(), query.end());
    query.erase(std::unique(query.begin(), query.end()), query.end());

	return query;

}

std::vector<double> OptionChain::getAvailableStrikes(std::chrono::year_month_day expiry, OptionType type) const {
    
    std::vector<double> query;
    
    for (const auto& contract : chain) {
        if (contract.getType() == type && contract.getExpiration() == expiry) {query.push_back(contract.getStrike());}
    }
    return query;
}

const OptionContract* OptionChain::findContract(double strike, OptionType type, std::chrono::year_month_day expiry) const {
    
    for (const auto& contract : chain) {
        
        if (contract.getType() == type && contract.getExpiration() == expiry && contract.getStrike() == strike) { return &contract; }
    }
    return nullptr;
}

const OptionContract* OptionChain::findBySymbol(const std::string& symbol) const {
    
    auto it = symbol_index.find(symbol);
    
    if (it != symbol_index.end()) {
        
        return &chain[it->second];
    }
    return nullptr;
}

OptionContract* OptionChain::findBySymbol(const std::string& symbol) {
    
    auto it = symbol_index.find(symbol);
    
    if (it != symbol_index.end()) {
        return &chain[it->second];
    }
    return nullptr;
}

double OptionChain::getUnderlyingPrice() const {
    
    if (!chain.empty()) {
        
        return chain.front().getMarketdata().underlying_price;
    }
    
    return 0.0;
}

const OptionContract* OptionChain::getATM(OptionType type, std::chrono::year_month_day expiry) const {
    
    double underlying = getUnderlyingPrice();
    
    if (underlying == 0.0) return nullptr;

    const OptionContract* atm = nullptr;
    double min_diff = std::numeric_limits<double>::max();

    for (const auto& contract : chain) {
        
        if (contract.getType() == type && contract.getExpiration() == expiry) {
            
            double diff = std::abs(contract.getStrike() - underlying);
            
            if (diff < min_diff) {
                min_diff = diff;
                atm = &contract;
            }
        }
    }
    return atm;
}