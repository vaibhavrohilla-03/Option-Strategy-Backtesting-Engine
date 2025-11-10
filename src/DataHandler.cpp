#include "DataHandler.h"
#include <iostream>
#include <algorithm>
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
		Data.insert(Data.end(), newData.begin(), newData.end());
			
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error adding Data from path :" << path << e.what();
	}
}

void DataHandler::prepareData()
{
	std::cout << "preparing Timeline of size :  " << Data.size() << '\n';
	std::sort(Data.begin(), Data.end());
	std::cout << " Timeline complete." << std::endl;
}

bool DataHandler::hasNextOption() const { return index < Data.size(); }


OptionContract DataHandler::getNextOption()
{
	if (!hasNextOption())
		throw std::out_of_range("No options to process in timeline");

	return Data[index++];
}

size_t DataHandler::getSize() const
{
	return Data.size();
}