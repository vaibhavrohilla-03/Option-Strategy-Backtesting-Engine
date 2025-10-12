#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

std::vector<std::string> split(const std::string& s, char delimiter);
std::string formatDate(const std::string& dateStr);

int main(int argc, char* argv[]) 
{
    if (argc != 3) 
        return 1;

    std::string inputFile = argv[1];
    std::filesystem::path outputDir = argv[2];

    std::ifstream file(inputFile);
    if (!file.is_open()) 
        return 1;

    std::map<std::string, std::ofstream> openFiles;
    std::string line;

    std::getline(file, line);
    const std::string newHeader = "timestamp,underlying,type,strike,expiration,last,volume,open_interest,implied_vol,delta,gamma,theta,vega\n";

    while (std::getline(file, line))
    {
        std::vector<std::string> columns = split(line, ',');

        if (columns.size() < 15)
            continue;
        

        const std::string& instrument = columns[0];
        if (instrument != "OPTSTK")
            continue;

        const std::string& symbol = columns[1];
        const std::string& expiry = columns[2];
        const std::string& strike = columns[3];
        const std::string& type = columns[4];
        const std::string& last = columns[8];
        const std::string& volume = columns[10];
        const std::string& open_interest = columns[12];
        const std::string& timestamp = columns[14];

        std::string fileDate = formatDate(timestamp);
        std::filesystem::path stockDir = outputDir / symbol;
        std::filesystem::path outputFile = stockDir / (fileDate + ".csv");
        std::string outputFilePathStr = outputFile.string();

        if (openFiles.find(outputFilePathStr) == openFiles.end())
        {
            try
            {
                std::filesystem::create_directories(stockDir);
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                std::cerr << "Error creating directory " << stockDir << ": " << e.what() << std::endl;
                continue;
            }

            openFiles[outputFilePathStr].open(outputFile);
            if (!openFiles[outputFilePathStr].is_open())
            {
                std::cerr << "Error: Could not open output file for writing: " << outputFile << std::endl;
                openFiles.erase(outputFilePathStr);
                continue;
            }
            openFiles[outputFilePathStr] << newHeader;
        }

        std::stringstream ss;
        ss << timestamp << ","
            << symbol << ","
            << type << ","
            << strike << ","
            << expiry << ","
            << last << ","
            << volume << ","
            << open_interest << ","
            << ",,,," << "\n";

        openFiles[outputFilePathStr] << ss.str();
    }
    file.close();

    std::cout << "Data successfully processed and written to " << outputDir << std::endl;

    return 0;
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::string formatDate(const std::string& dateStr)
{
    std::string trimmedDateStr = dateStr;
    if (!trimmedDateStr.empty() && (trimmedDateStr.back() == '\r' || trimmedDateStr.back() == '\n'))
    {
        trimmedDateStr.pop_back();
    }

    if (trimmedDateStr.length() < 11) return "INVALID_DATE";

    std::string day = trimmedDateStr.substr(0, 2);
    std::string monthStr = trimmedDateStr.substr(3, 3);
    std::string year = trimmedDateStr.substr(7, 4);

    std::map<std::string, std::string> monthMap =
    {
        {"JAN", "01"}, {"FEB", "02"}, {"MAR", "03"}, {"APR", "04"},
        {"MAY", "05"}, {"JUN", "06"}, {"JUL", "07"}, {"AUG", "08"},
        {"SEP", "09"}, {"OCT", "10"}, {"NOV", "11"}, {"DEC", "12"}
    };

    std::string monthNum = "00";
    if (monthMap.count(monthStr))
    {
        monthNum = monthMap[monthStr];
    }

    return year + "-" + monthNum + "-" + day;
}

