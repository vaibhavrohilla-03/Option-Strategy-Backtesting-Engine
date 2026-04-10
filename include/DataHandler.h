#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class OptionContract;
enum class DataSourceType;

class DataHandler
{

private:

	size_t index;
	std::unordered_map<std::string, size_t> latest_contract_indices;
	
	void registerDataSources();

public:

	std::vector<OptionContract> Data;
	DataHandler();
	void addDataSource(DataSourceType type, const std::string& path);
	void prepareData();

	bool hasNextOption() const;

	OptionContract getNextOption();
	
	size_t getSize() const;

	double getLatestPrice(const std::string& symbol);
	int getMultiplier(const std::string& symbol);
	OptionContract getContract(const std::string& symbol);

};
