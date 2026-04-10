#include <fstream>

#include "Portfolio.h"
#include "DataHandler.h"

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

	auto current_date = event->timestamp;
	std::vector<std::string> expired_symbols;

	for (const auto& [sym, qty] : current_positions) {
		
		try {
				const OptionContract& contract = data->getContract(sym);

			if (current_date >= contract.getExpiration()) {
				
				// if the option expired ITM or OTM
				double underlying_price_at_expiry = contract.getMarketdata().underlying_price;
				double value_at_expiry = contract.valueAtExpiration(underlying_price_at_expiry);

				if (value_at_expiry > 0.0) {

                    int multiplier = contract.getMultiplier();
                    
                    double settlement_cash = value_at_expiry * qty * multiplier;
                    
                    currentcapital += settlement_cash;
                }

                // Regardless of ITM or OTM, the option has expired and must be removed
                expired_symbols.push_back(sym);
			}
		} catch (const std::exception& e) {
			continue;
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

void Portfolio::UpdateTimeindex(std::shared_ptr<Event> event) {

    if (event->type != EventType::MarketEvent) return;

    PortfolioHoldings snapshot;
    snapshot.timestamp = event->timestamp; 
    snapshot.cash = currentcapital;        
    
    snapshot.commision = 0.0; 

    double total_market_value = 0.0;

    for (const auto& [sym, qty] : current_positions) {
        
        double latest_price = data->getLatestPrice(sym); 

        int multiplier = data->getMultiplier(sym);

        double position_value = qty * latest_price * multiplier;
        
        snapshot.market_value[sym] = position_value;
        
        total_market_value += position_value;
    }

    snapshot.total_equity = snapshot.cash + total_market_value;

    all_holdings.push_back(snapshot);
}

void Portfolio::create_equity_dataframe_CSV(const std::string& filepath) {

	std::ofstream file(filepath);

	if(!file.is_open()) {
		std::cerr << "Portfolio :: equityDataframe :: file error";
		return;
	}

	file << "datetime,cash,commission,total_equity,returns,equity_curve\n";

	double previousTotal = initialcapital;

	for(size_t i = 0; i < all_holdings.size(); ++i) {

		const auto& holding = all_holdings[i];

		double returns = 0.0;
        if (i == 0) {
        
            returns = (holding.total_equity - initialcapital) / initialcapital;
        } 
        else {
            
            previousTotal = all_holdings[i-1].total_equity;
            
            if (previousTotal != 0.0) {
                returns = (holding.total_equity - previousTotal) / previousTotal;
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

}


