#pragma once

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "DataSourceFactory.h"
#include "Options.h"

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


#endif 