#include "Backtester.h"

#include "OptionChain.h"


Backtester::Backtester(double initial_capital, 
                       std::shared_ptr<Strategy> strat, 
                       BrokerConfig brokerConfig) : strategy(strat)
{
    eventQueue = std::make_shared<EventQueue>();
    
    dataHandler = std::make_shared<DataHandler>();
    
    portfolio = std::make_shared<Portfolio>(eventQueue, dataHandler, initial_capital);
    
    executionHandler = std::make_shared<ExecutionHandler>(eventQueue.get(), dataHandler, brokerConfig);

    strategy->bindEngineDependencies(eventQueue, dataHandler, portfolio);
}

void Backtester::addData(DataSourceType type, const std::string& path) {
    dataHandler->addDataSource(type, path);
}

void Backtester::run() {

    if (!strategy) {
        std::cerr << "no strategy set.\n";
        return;
    }

    dataHandler->prepareData();

    std::cout << "Starting Backtest..." << std::endl;

    while (dataHandler->hasNext() || !eventQueue->empty()) {
        
        if (eventQueue->empty() && dataHandler->hasNext()) {
            const OptionChain& nextChain = dataHandler->getNextChain();
            auto chainPtr = std::make_shared<OptionChain>(nextChain);
            auto marketEvent = std::make_shared<MarketEvent>(nextChain.getDate(), chainPtr);
            eventQueue->push(marketEvent);
        }

        while (!eventQueue->empty()) {
            auto event = eventQueue->front();
            eventQueue->pop();

            switch (event->type) {

                case EventType::MarketEvent:
                    strategy->onEvent(event);
                    portfolio->UpdatePositions(event); 
                    portfolio->UpdateTimeindex(event); 
                    break;

                case EventType::SignalEvent: {
                    auto signalEvent = std::static_pointer_cast<SignalEvent>(event);
                    portfolio->on_signal(signalEvent);
                    break;
                }

                case EventType::OrderEvent:
                    executionHandler->onEvent(event);
                    break;

                case EventType::FillEvent: {
                    auto fillEvent = std::static_pointer_cast<FillEvent>(event);
                    portfolio->on_fill(fillEvent);
                    strategy->onEvent(event);
                    break;
                }

                default:
                    break;
            
            }
        }
    }
    

    std::cout << "Backtest Completed." << std::endl;

    std::string filepath = ROOT_DIR + strategy->Strategyname + ".csv";

    portfolio->create_equity_dataframe_CSV_and_Metrics(filepath);
}
