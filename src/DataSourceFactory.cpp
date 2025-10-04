#include "DataSourceFactory.h"
#include <iostream>


CSVDataSource::CSVDataSource(const std::string& path) : path(path) {}

void CSVDataSource::readData()
{
	std::cout << "Reading data from CSV file: " << this->path << std::endl;
}

URLDataSource::URLDataSource(const std::string& source) : url(source) {}

void URLDataSource::readData() 
{
	std::cout << "Reading data from URL: " << this->url << std::endl;
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
