#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class OptionChain;

class OptionContract;

enum class DataSourceType;

class DataHandler
{

private:

	size_t index;

	std::vector<OptionContract> rawData;

	std::vector<OptionChain> timeline;

	const OptionChain* currentChain = nullptr;
	
	void registerDataSources();
	void buildTimeline();

public:

	DataHandler();
	void addDataSource(DataSourceType type, const std::string& path);
	void prepareData();

	bool hasNext() const;
	const OptionChain& getNextChain();

	size_t getChainCount() const;
	size_t getTotalContracts() const;
	
	double getLatestPrice(const std::string& symbol);
	
	int getMultiplier(const std::string& symbol);
	
	const OptionContract* getContract(const std::string& symbol);

};
