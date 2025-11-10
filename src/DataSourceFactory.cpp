#include "DataSourceFactory.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

CSVDataSource::CSVDataSource(const std::string& path) : path(path) {}



OptionContract CSVDataSource::parseline(const std::string& line)
{
	Marketdata OptionData;

	std::stringstream ss(line);
	std::string currentcell;
	std::getline(ss, currentcell, ',');
	
	//getting data
	try
	{
		std::chrono::year_month_day Optiondate;
		std::stringstream dateStream(currentcell);
		dateStream >> std::chrono::parse("%d-%b-%Y", Optiondate);

		std::getline(ss, currentcell, ',');
		std::string OptionSymbol = currentcell;

		std::getline(ss, currentcell, ',');
		OptionData.underlying_price = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		OptionType Optiontype = Type(currentcell);

		std::getline(ss, currentcell, ',');
		double OptionStrike = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		std::chrono::year_month_day expiration;
		std::stringstream expStream(currentcell);
		expStream >> std::chrono::parse("%d-%b-%Y", expiration);


		std::getline(ss, currentcell, ',');
		OptionData.open = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		OptionData.high = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		OptionData.low = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		OptionData.close = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		OptionData.volume = std::stoll(currentcell);

		std::getline(ss, currentcell, ',');
		OptionData.open_interest = std::stoll(currentcell);

		return  OptionContract(Optiondate, OptionStrike, Optiontype, OptionSymbol, expiration, OptionData, Greeks());
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(std::string("Failed to parsedata") + e.what());
	}
	
}

std::vector<OptionContract> CSVDataSource::loadData()
{
	int row = 0;
	std::vector<OptionContract> data;
	std::ifstream file(path);

	if (!file.is_open())
		throw std::runtime_error("Couldn't Open file at path " + path);

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		try
		{
			data.push_back(parseline(line));
		}
		catch (const std::exception& e)
		{
			std::cerr << "Skipping row " << row << ": " << e.what() << '\n';
		}

		++row;
	}

	std::cout << "Successfully read" << data.size() << "Options" << "from" << path;

	return data;
}




std::unique_ptr<DataSource> CSVDataSourceFactory::createDataSource(const std::string& path)
{
	return std::make_unique<CSVDataSource>(path);
}

std::unique_ptr<DataSource> URLDataSourceFactory::createDataSource(const std::string& url)
{
	return std::make_unique<URLDataSource>(url);
}


FactoryRegistry& FactoryRegistry::getInstance()
{
	static FactoryRegistry instance; 
	return instance;
}

void FactoryRegistry::registerFactory(DataSourceType type, std::unique_ptr<DataSourceFactory> factory)
{
	factories[type] = std::move(factory);
}

std::unique_ptr<DataSource> FactoryRegistry::createDataSource(DataSourceType type, const std::string& source)
{
	auto it = factories.find(type);
	if (it == factories.end())
	{
		throw std::runtime_error("No factory registered for the given datasource type");
	}
	return it->second->createDataSource(source);
}




DataRegister::DataRegister(DataSourceType type)
{
	switch (type)
	{
	case DataSourceType::CSV:
		FactoryRegistry::getInstance().registerFactory(type, std::make_unique<CSVDataSourceFactory>());
		break;

	case DataSourceType::URL:
			FactoryRegistry::getInstance().registerFactory(type, std::make_unique<URLDataSourceFactory>());
			break;
	
	default:
		throw std::invalid_argument("Attempted to register an unsupported DataSourceType.");
		break;
	}
}

URLDataSource::URLDataSource(const std::string& urlsource)
{

}

std::vector<OptionContract> URLDataSource::loadData()
{
	return std::vector<OptionContract>();
}
