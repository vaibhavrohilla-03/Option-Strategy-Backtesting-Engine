#pragma once

#ifndef DATASOURCEFACTORY_H

#define DATASOURCEFACTORY_H

#include <memory>
#include <unordered_map>
#include <utility>
#include <string>
#include "Options.h"

enum class DataSourceType
{
	CSV,
	URL
};

class DataSourceFactory;

class DataSource
{	
public:

	virtual ~DataSource() = default;
	virtual std::vector<OptionContract> loadData() = 0;
};

class CSVDataSource : public DataSource
{
public:
	CSVDataSource(const std::string& path);
	std::vector<OptionContract> loadData() override;

private:
	std::string path;
	OptionContract parseline(const std::string& line);
};

class URLDataSource : public DataSource
{
public:
	URLDataSource(const std::string& urlsource);
	std::vector<OptionContract> loadData() override;
	
private:
	std::string url;
};



class DataSourceFactory
{
public:
	virtual ~DataSourceFactory() = default;

	virtual std::unique_ptr<DataSource> createDataSource(const std::string& source) = 0;

};

class CSVDataSourceFactory : public DataSourceFactory
{
public:
	std::unique_ptr<DataSource> createDataSource(const std::string& path) override;
};

class URLDataSourceFactory : public DataSourceFactory
{
public:
	std::unique_ptr<DataSource> createDataSource(const std::string& url) override;
};


class FactoryRegistry
{	
private:

	FactoryRegistry() = default;
	~FactoryRegistry() = default;

	FactoryRegistry(const FactoryRegistry&) = delete;
	FactoryRegistry& operator=(const FactoryRegistry) = delete;
	
	std::unordered_map<DataSourceType, std::unique_ptr<DataSourceFactory>> factories;

public:

	static FactoryRegistry& getInstance();

	void registerFactory(DataSourceType type, std::unique_ptr<DataSourceFactory> factory);

	std::unique_ptr<DataSource> createDataSource(DataSourceType type, const std::string& source);

};

class DataRegister
{
public:
	explicit DataRegister(DataSourceType type);
};

#endif