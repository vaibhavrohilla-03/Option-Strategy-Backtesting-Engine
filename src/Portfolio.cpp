#include "Portfolio.h"
#include "DataHandler.h"

Portfolio::Portfolio(std::shared_ptr<EventQueue> eventQueue, std::shared_ptr<DataHandler> marketData, double capital, 
					std::chrono::year_month_day start, std::chrono::year_month_day end) 
					: events(eventQueue), data(marketData), initialcapital(capital), startDate(start), endDate(end) { 

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

    double commission = event->calculatecommision(); 
    double slippage = event->calculateslippage();

    double transactionCost = (quantity * lot_size * price);

    if (direction == OrderType::Buy) {
        
        current_positions[sym] += quantity; // We hold 2 lots
        
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

