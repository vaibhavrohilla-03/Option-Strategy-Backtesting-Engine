#include <iostream>
#include <chrono>
#include "DataHandler.h"

int main()
{
	DataHandler dataHandler;

    std::cout << "Starting data load..." << std::endl;

    auto loadStart = std::chrono::high_resolution_clock::now();

    dataHandler.addDataSource(DataSourceType::CSV, "D:\\Quant\\OptionStrategy Backtesting Engine\\data\\nse\\options\\NIFTY.csv");

    auto loadEnd = std::chrono::high_resolution_clock::now();
    auto loadDuration = std::chrono::duration_cast<std::chrono::milliseconds>(loadEnd - loadStart);

    std::cout << "Starting data processing (sorting)..." << std::endl;

    auto processStart = std::chrono::high_resolution_clock::now();

    dataHandler.prepareData(); 

    auto processEnd = std::chrono::high_resolution_clock::now();
    auto processDuration = std::chrono::duration_cast<std::chrono::milliseconds>(processEnd - processStart);


    std::cout << "\n--- Backtest Performance Report ---" << std::endl;
    std::cout << "Total Events Loaded:  " << dataHandler.getSize() << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Data Loading Time:    " << loadDuration.count() << " ms" << std::endl;
    std::cout << "Data Processing Time: " << processDuration.count() << " ms" << std::endl;

    auto totalDuration = loadDuration + processDuration;
    std::cout << "Total Setup Time:     " << totalDuration.count() << " ms" << std::endl;
    std::cout << "-------------------------------------\n" << std::endl;


    std::cout << "Starting backtest event loop..." << std::endl;
    int eventCount = 0;

    while (dataHandler.hasNextOption())
    {
        OptionContract currentEvent = dataHandler.getNextOption();
        eventCount++;


        std::cout << "Event " << std::setw(5) << eventCount
            << " [" << currentEvent.getDate() << "]"
            << " Symbol: " << currentEvent.getSymbol()
            << " Strike: " << currentEvent.getStrike()
            << " Type: " << currentEvent.getType()
            << " Close: " << currentEvent.getMarketdata().close
            << " Underlying: " << currentEvent.getMarketdata().underlying_price
            << std::endl;
    }
    std::cout << "Backtest complete. Processed " << eventCount << " events." << std::endl;

    return 0;
}