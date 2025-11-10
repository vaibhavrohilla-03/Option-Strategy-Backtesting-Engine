#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> split(const std::string& s, char delimiter);

int main(int argc, char* argv[]) 
{   

    std::string optionTicker = "NIFTY";
    std::string directory = "D:\\Quant\\OptionStrategy Backtesting Engine\\data\\nse\\Bhavcopies-raw";
    std::string outputfile = "D:\\Quant\\OptionStrategy Backtesting Engine\\data\\nse\\options\\" + (optionTicker + ".csv");

    std::string header = "date,underlying_symbol,underlying_price,option_type,strike,expiration,open,high,low,close,volume,open_interest";

    std::ofstream output(outputfile);

    output << header << "\n";

    for (const auto& entry : fs::directory_iterator(directory))
    {   
        std::string currentfile = entry.path().string();
        std::string currentline;

        std::ifstream fs(currentfile);

        if (!fs.is_open())
        {
            std::cout<< "didn't open";
            return 1;
        }
        std::getline(fs, currentline);

        while (std::getline(fs, currentline))
        {
            std::vector<std::string> coloumns = split(currentline, ',');
            
            if (coloumns.size() < 15)
                continue;

            const std::string instrument = coloumns[0];

            if (instrument == "OPTIDX" && coloumns[1] == optionTicker)
            {
                std::stringstream ss;
                ss << coloumns[14] << ","
                    << coloumns[1] << ","
                    << " " << ","
                    << coloumns[4] << ","
                    << coloumns[3] << ","
                    << coloumns[2] << ","
                    << coloumns[5] << ","
                    << coloumns[6] << ","
                    << coloumns[7] << ","
                    << coloumns[8] << ","
                    << coloumns[10] << ","
                    << coloumns[12] << "," << "\n";

                output << ss.str();

                std::cout << "Wrote" <<  currentline << "\n";
            }
        }

    }
    std::cout << "done";
    output.close();

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
