#include "DataSourceFactory.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <filesystem>

#include <databento/dbn.hpp>
#include <databento/historical.hpp>
#include <databento/symbol_map.hpp>

#include <map>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "TimeUtil.h"

namespace db = databento;

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
		dateStream >> std::chrono::parse("%d-%b-%y", Optiondate);

		std::getline(ss, currentcell, ',');
		std::string OptionSymbol = currentcell;

		std::getline(ss, currentcell, ',');
		OptionData.underlying_price = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		std::string typeStr = currentcell;  
		OptionType Optiontype = Type(currentcell);

		std::getline(ss, currentcell, ',');
		std::string strikeStr = currentcell;
		double OptionStrike = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		std::string expiryStr = currentcell;
		std::chrono::year_month_day expiration;
		std::stringstream expStream(currentcell);
		expStream >> std::chrono::parse("%d-%b-%y", expiration);

		std::string uniqueContractSymbol = OptionSymbol + "_" + typeStr + "_" + strikeStr + "_" + expiryStr;


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

		std::getline(ss, currentcell, ',');
		OptionData.risk_free_rate = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		OptionData.dividend_yield = std::stod(currentcell);

		std::getline(ss, currentcell, ',');
		int lotSize = std::stoi(currentcell);


		return  OptionContract(Optiondate, OptionStrike, Optiontype, uniqueContractSymbol, OptionSymbol, expiration, lotSize, OptionData, Greeks());
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
		throw std::invalid_argument("Attempted to register an unsupported DataSourceType");
		break;
	}
}

URLDataSource::URLDataSource(const std::string& urlsource) : url(urlsource)
{
	std::stringstream ss(urlsource);
	std::string token;
	std::vector<std::string> tokens;
	
	while (std::getline(ss, token, ':')) {
		tokens.push_back(token);
	}
	
	if (tokens.size() == 4 && tokens[0] == "databento") {
		ticker = tokens[1];
		start_date = tokens[2];
		end_date = tokens[3];
	} 
	else {
		throw std::invalid_argument("Invalid URL format for URLDataSource: " + urlsource);
	}
}



std::string URLDataSource::fetchAndCache()
{

    const std::string cache_dir = std::string(ROOT_DIR) + "data/nse/options/" + ticker;
    std::filesystem::create_directories(cache_dir);

    const std::string cache_file = cache_dir + "/" + ticker + "_" + start_date + "_" + end_date + ".csv";

    if (std::filesystem::exists(cache_file)) {
        return cache_file;
    }

    std::string db_key;
    {
        std::ifstream config_file(std::string(ROOT_DIR) + "config.json");
        if (!config_file.is_open()) { throw std::runtime_error("Cannot open config.json"); }
        
        nlohmann::json config;
        config_file >> config;
        db_key = config.value("DATABENTO_API_KEY", "");
        
        if (db_key.empty()) { throw std::runtime_error("DATABENTO_API_KEY missing in config.json"); }
    
    }

    auto client = db::Historical::Builder().SetKey(db_key).Build();
    const std::vector<std::string> symbols = {ticker};

    // --- 1. Fetch underlying daily OHLCV from Nasdaq ITCH ---
    std::map<std::string, double> underlying_prices;
    try {
        
        client.TimeseriesGetRange("XNAS.ITCH", {start_date, end_date}, symbols,
            db::Schema::Ohlcv1D, db::SType::RawSymbol, db::SType::InstrumentId, {}, {}, 
            
            [&underlying_prices](const db::Record& record) {
                
                const auto& bar = record.Get<db::OhlcvMsg>();
                const std::string date_str = Utils::fromNanos(bar.hd.ts_event.time_since_epoch().count());

                underlying_prices[date_str] = static_cast<double>(bar.close) / 1e9;
                
                return db::KeepGoing::Continue;
            });
    
    } 

    catch (const db::Exception& e) {
        std::cerr << "[WARN] Underlying price fetch failed: " << e.what() << '\n';
    }

    // --- 2. Stream option chain OHLCV from OPRA and write CSV ---
    const std::string tmp_file = cache_file + ".tmp";
    std::ofstream out(tmp_file);
    
    if (!out.is_open()) { throw std::runtime_error("Cannot open temp file: " + tmp_file); }

    out << "Date,underlying_symbol,underlying_price,option_type,strike,expiration,open,high,low,close,volume,open_interest,Risk_Free_Rate,Dividend_Yield,LotSize\n";

    try {
    
        db::TsSymbolMap symbol_map;

        const std::vector<std::string> opra_symbols = { ticker + ".OPT" };
        client.TimeseriesGetRange("OPRA.PILLAR", {start_date, end_date}, opra_symbols,
            db::Schema::Ohlcv1D, db::SType::Parent, db::SType::InstrumentId, {},   
            
            
            [&symbol_map](const db::Metadata& metadata) {
            	symbol_map = metadata.CreateSymbolMap();
            },
            [&](const db::Record& record) {
              
                const auto& bar = record.Get<db::OhlcvMsg>();
                const std::string occ = symbol_map.At(bar);

                // OCC : 6-char root, 6-char date (YYMMDD),
                //                1-char type (C/P), 8-char strike (x1000)
                if (occ.length() < 21) {
                    return db::KeepGoing::Continue;
                }

                std::string root = occ.substr(0, 6);
                size_t endpos = root.find_last_not_of(" ");
                if (std::string::npos != endpos) {
                    root = root.substr(0, endpos + 1);
                }

                const std::string date_str = Utils::fromNanos(bar.hd.ts_event.time_since_epoch().count());

                const std::string exp_ymd = "20" + occ.substr(6, 2) + "-" + occ.substr(8, 2) + "-" + occ.substr(10, 2);
                const std::string option_type = (occ[12] == 'C') ? "CE" : "PE";
                
                const double strike = std::stod(occ.substr(13, 8)) / 1000.0;
                
                if (underlying_prices.find(date_str) == underlying_prices.end()) { return db::KeepGoing::Continue; }

                const double ul_price = underlying_prices.at(date_str);

                constexpr double rfr = 5.0;

                out << Utils::ymdToDMonY(date_str) << ',' << root << ','
                    << ul_price << ',' << option_type << ','
                    << strike << ',' << Utils::ymdToDMonY(exp_ymd) 
                    << ',' << (bar.open   / 1e9) << ',' << (bar.high   / 1e9) << ','
                    << (bar.low    / 1e9) << ',' 
                    << (bar.close  / 1e9) << ',' 
                    << bar.volume << ',' << 0 << ','   
                    << rfr << ',' << 0.0 << ',' << 100 << '\n';

                return db::KeepGoing::Continue;
            });
    } 

    catch (const db::Exception& e) { std::cerr << "[WARN] Options chain fetch failed: " << e.what() << '\n'; }

    out.close();
    std::filesystem::rename(tmp_file, cache_file);
    return cache_file;
}

std::vector<OptionContract> URLDataSource::loadData()
{
	std::string cache_path = fetchAndCache();
	CSVDataSource csv(cache_path);
	return csv.loadData();
}
