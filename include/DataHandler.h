#pragma once

#include <vector>
#include <string>

class OptionContract;
enum class DataSourceType;

class DataHandler
{

private:

	size_t index;
	void registerDataSources();

public:

	std::vector<OptionContract> Data;
	DataHandler();
	void addDataSource(DataSourceType type, const std::string& path);
	void prepareData();

	bool hasNextOption() const;

	OptionContract getNextOption();
	
	size_t getSize() const;

};
