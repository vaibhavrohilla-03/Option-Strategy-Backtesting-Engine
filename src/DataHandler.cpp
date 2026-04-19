#include <iostream>
#include <algorithm>

#include "DataHandler.h"
#include "Options.h"
#include "DataSourceFactory.h"

DataHandler::DataHandler() : index(0)
{
	registerDataSources();
}

void DataHandler::registerDataSources()
{
	static DataRegister csv_register(DataSourceType::CSV);
	static DataRegister url_register(DataSourceType::URL);
}

void DataHandler::addDataSource(DataSourceType type, const std::string& path)
{
	try
	{
		FactoryRegistry& registry = FactoryRegistry::getInstance();

		std::unique_ptr<DataSource> datasource = registry.createDataSource(type, path);

		std::vector<OptionContract> newData = datasource->loadData();
		
		rawData.insert(rawData.end(), 
			std::make_move_iterator(newData.begin()), 
			std::make_move_iterator(newData.end()));
			
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error adding Data from path :" << path << e.what();
	}
}

void DataHandler::prepareData()
{
	std::cout << "Preparing timeline from " << rawData.size() << " contracts\n";

	std::sort(rawData.begin(), rawData.end());

	buildTimeline();

	std::cout << "Timeline complete: " << timeline.size() << " trading days, " << getTotalContracts() << " total contracts.\n";
}

void DataHandler::buildTimeline()
{
	if (rawData.empty()) return;

	auto currentDate = rawData[0].getDate();
	std::vector<OptionContract> batch;

	for (auto& contract : rawData) {

		if (contract.getDate() != currentDate) {

			timeline.emplace_back(currentDate, std::move(batch));
			batch.clear();
			currentDate = contract.getDate();
		}

		batch.push_back(std::move(contract));
	}

	if (!batch.empty()) {
		timeline.emplace_back(currentDate, std::move(batch));
	}

	rawData.clear(); 
	rawData.shrink_to_fit();
}

bool DataHandler::hasNext() const { return index < timeline.size(); }


const OptionChain& DataHandler::getNextChain()
{
	if (!hasNext())
		throw std::out_of_range("No more trading days to process in timeline");

    const OptionChain& chain = timeline[index++];
    currentChain = &chain;
	return chain;
}

size_t DataHandler::getChainCount() const
{
	return timeline.size();
}

size_t DataHandler::getTotalContracts() const
{
	size_t total = 0;
	for (const auto& chain : timeline) {
		total += chain.getAllContracts().size();
	}
	return total;
}

double DataHandler::getLatestPrice(const std::string& symbol)
{
    if (currentChain) {
        const OptionContract* contract = currentChain->findBySymbol(symbol);
        if (contract) {
            return contract->getMarketdata().close;
        }
    }
	return 0.0;
}

int DataHandler::getMultiplier(const std::string& symbol)
{
    if (currentChain) {
        const OptionContract* contract = currentChain->findBySymbol(symbol);
        if (contract) {
            return contract->getMultiplier();
        }
    }
	return 1; 
}

const OptionContract* DataHandler::getContract(const std::string& symbol)
{
    if (currentChain) {
        return currentChain->findBySymbol(symbol);
    }
	return nullptr;
}