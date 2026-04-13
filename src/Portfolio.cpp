#include <fstream>

#include <iostream>
#include <iomanip>

#include "Metrics.h"
#include "Portfolio.h"
#include "DataHandler.h"
#include "OptionChain.h"

Portfolio::Portfolio(std::shared_ptr<EventQueue> eventQueue, std::shared_ptr<DataHandler> marketData, double capital) 
					: events(eventQueue), data(marketData), initialcapital(capital) { 

	currentcapital = initialcapital; 

}

void Portfolio::on_signal(std::shared_ptr<SignalEvent> event) {

	auto order = GenerateOrder(event);
    
    if (order) {
        events->push(std::move(order));
    }
    
}

void Portfolio::on_fill(std::shared_ptr<FillEvent> event) {

	std::string sym = event->symbol;
    int quantity = event->quantity;          
    int lot_size = event->multiplier;    
    double price = event->fillPrice;    
    OrderType direction = event->direction;

    double commission = event->commision; 
    double slippage = event->slippage;
    
    total_commission_paid += commission;
    total_slippage_paid += slippage;

    double transactionCost = (quantity * lot_size * price);

    if (direction == OrderType::Buy) {
        
        current_positions[sym] += quantity; 
        
        currentcapital -= transactionCost;
        currentcapital -= (commission + slippage); 
        
    } 
    else if (direction == OrderType::Sell) {
        
        current_positions[sym] -= quantity;
        
        currentcapital += transactionCost;
        currentcapital -= (commission + slippage); 
    }

    if (current_positions[sym] == 0) {
        current_positions.erase(sym);
    }
}

void Portfolio::UpdatePositions(std::shared_ptr<Event> event) {

	if (event->type != EventType::MarketEvent) return;

	auto marketEvent = std::static_pointer_cast<MarketEvent>(event);
	auto& chain = marketEvent->chain;
	auto current_date = event->timestamp;
	std::vector<std::string> expired_symbols;

	for (const auto& [sym, qty] : current_positions) {
		
		// Try chain first, fall back to DataHandler cache
		const OptionContract* contract = chain->findBySymbol(sym);
		if (!contract) {
			contract = data->getContract(sym);
		}
		if (!contract) continue;

		if (current_date >= contract->getExpiration()) {
			
			// if the option expired ITM or OTM
			double underlying_price_at_expiry = contract->getMarketdata().underlying_price;
			double value_at_expiry = contract->valueAtExpiration(underlying_price_at_expiry);

			if (value_at_expiry > 0.0) {

                int multiplier = contract->getMultiplier();
                
                double settlement_cash = value_at_expiry * qty * multiplier;
                
                currentcapital += settlement_cash;
            }

            // Regardless of ITM or OTM, the option has expired and must be removed
            expired_symbols.push_back(sym);
		}
	}

	for (const auto& sym : expired_symbols) {
		current_positions.erase(sym);
	}
}

std::unique_ptr<OrderEvent> Portfolio::GenerateOrder(std::shared_ptr<SignalEvent> event) {
	OrderType direction;
	int sizing = event->quantity;

	switch (event->signaltype) {
		case SignalType::Long: {

			direction = OrderType::Buy;
			break;

		}
		case SignalType::Short: {

			direction = OrderType::Sell;
			break;

		}
		case SignalType::Exit: {
			
			auto it = current_positions.find(event->symbol);
			if (it == current_positions.end() || it->second == 0) {
				return nullptr;
			}
			direction = (it->second > 0) ? OrderType::Sell : OrderType::Buy;
			sizing = std::abs(it->second);
			break;
		}
		
		default:
			return nullptr;
	}

	if (sizing <= 0) {
		sizing = 1;
	}

	OrderMode mode = event->suggestedmode;
	double target_price = event->limitprice;

	return std::make_unique<OrderEvent>(event->timestamp, event->symbol, direction, mode, sizing, target_price);
}

int Portfolio::getPositionQuantity(const std::string& symbol) const {
    auto it = current_positions.find(symbol);
    if (it != current_positions.end()) {
        return it->second;
    }
    return 0;
}

void Portfolio::UpdateTimeindex(std::shared_ptr<Event> event) {

    if (event->type != EventType::MarketEvent) return;

    auto marketEvent = std::static_pointer_cast<MarketEvent>(event);
    auto& chain = marketEvent->chain;

    PortfolioHoldings snapshot;
    snapshot.timestamp = event->timestamp; 
    snapshot.cash = currentcapital;        
    
    snapshot.commision = 0.0; 

    double total_market_value = 0.0;

    snapshot.riskfree_rate = 0.0;
    if (!chain->getAllContracts().empty()) {
        snapshot.riskfree_rate = chain->getAllContracts()[0].getMarketdata().risk_free_rate / 100.0;
    }

    for (const auto& [sym, qty] : current_positions) {
        
        const OptionContract* contract = chain->findBySymbol(sym);
        double latest_price = 0.0;
        int multiplier = 1;

        if (contract) {
            latest_price = contract->getMarketdata().close;
            multiplier = contract->getMultiplier();
        } else {
            latest_price = data->getLatestPrice(sym);
            multiplier = data->getMultiplier(sym);
        }

        double position_value = qty * latest_price * multiplier;
        
        snapshot.market_value[sym] = position_value;
        
        total_market_value += position_value;

    }

    snapshot.total_equity = snapshot.cash + total_market_value;

    if (!all_holdings.empty() && all_holdings.back().timestamp == snapshot.timestamp) {
        
        all_holdings.back() = std::move(snapshot);
    } 

    else {
        all_holdings.push_back(std::move(snapshot));
    }
}

void Portfolio::create_equity_dataframe_CSV_and_Metrics(const std::string& filepath) {

	std::ofstream file(filepath);

	if(!file.is_open()) {
		std::cerr << "Portfolio :: equityDataframe :: file error";
		return;
	}

	file << "datetime,cash,commission,total_equity,returns,equity_curve\n";

	double previousTotal = initialcapital;
	double sumRisk = 0.0;
	
	std::vector<double> all_returns;
	all_returns.reserve(all_holdings.size());

	for(size_t i = 0; i < all_holdings.size(); ++i) {

		const auto& holding = all_holdings[i];
		
		sumRisk += holding.riskfree_rate;

		double returns = 0.0;

        if (i == 0) {
        
            returns = (holding.total_equity - initialcapital) / initialcapital;
            all_returns.push_back(returns);
        } 
        else {
            
            previousTotal = all_holdings[i-1].total_equity;
            
            if (previousTotal != 0.0) {
            
                returns = (holding.total_equity - previousTotal) / previousTotal;
                all_returns.push_back(returns);
            }
        }

        double equity_curve = holding.total_equity / initialcapital;

        unsigned int y = static_cast<int>(holding.timestamp.year());
        unsigned int m = static_cast<unsigned>(holding.timestamp.month());
        unsigned int d = static_cast<unsigned>(holding.timestamp.day());

        file << y << "-" << std::setfill('0') << std::setw(2) << m << "-" << std::setw(2) << d << ","
             << std::fixed << std::setprecision(4) << holding.cash << ","
             << holding.commision << ","
             << holding.total_equity << ","
             << returns << ","
             << equity_curve << "\n";

	}

	file.close();
    std::cout << "Equity curve successfully saved to: " << filepath << std::endl;

    double final_equity = all_holdings.empty() ? initialcapital : all_holdings.back().total_equity;

    double avgRisk = sumRisk / all_holdings.size();
    avgRisk = avgRisk / 252.0; // 252 trading days

    double sharpe_ratio = Utils::CalculateSharpeRatio(all_returns, avgRisk, 252);
    Utils::DrawdownMetrics drawdowns = Utils::create_drawdown(all_holdings);
    
    double roi = Utils::CalculateROI(initialcapital, final_equity);
    double years_traded = all_holdings.size() / 252.0;
    double cagr = Utils::CalculateCAGR(initialcapital, final_equity, years_traded);
    double profit_factor = Utils::CalculateProfitFactor(all_returns);

    double Annual_volitility = Utils::CalculateAnnualizedVolatility(all_returns);

    std::cout << "Initial Capital:    $" << initialcapital << "\n";
    std::cout << "Final Capital:      $" << final_equity << "\n";
    std::cout << "\n";
    std::cout << "Total ROI:          " << roi << "%\n";
    std::cout << "CAGR:               " << cagr << "%\n";
    std::cout << "Sharpe Ratio:       " << sharpe_ratio << "\n";
    std::cout << "Profit Factor:      " << profit_factor << "\n";
    std::cout << "Annual Volitility" << Annual_volitility << "\n";
    std::cout << "\n";
    std::cout << "Total Commission:   $" << total_commission_paid << "\n";
    std::cout << "Total Slippage:     $" << total_slippage_paid << "\n";
    std::cout << "Max Drawdown:       " << drawdowns.max_drawdown_percnt << "%\n";
    std::cout << "Max Drawdown Dur.:  " << drawdowns.max_duration << " days\n";



}
