#pragma once

#include <memory>
#include <string>
#include "Event.h"
#include "DataHandler.h"
#include "Strategy.h"
#include "Portfolio.h"
#include "Executionhandler.h"
#include "DataSourceFactory.h"
#include "BrokerModel.h"

class Backtester {
private:

    std::shared_ptr<EventQueue> eventQueue;
    std::shared_ptr<DataHandler> dataHandler;
    std::shared_ptr<Strategy> strategy;
    std::shared_ptr<Portfolio> portfolio;
    std::shared_ptr<ExecutionHandler> executionHandler;

public:
    Backtester(double initial_capital, 
               std::shared_ptr<Strategy> strat, 
               BrokerConfig brokerConfig);
    
    virtual ~Backtester() = default;

    void addData(DataSourceType type, const std::string& path);

    void run();

    Portfolio& getPortfolio() { return *portfolio; }
};