#pragma once

#include <functional>

using CommissionModel = std::function<double(int, double, int)>;
using SlippageModel = std::function<double(double)>;

struct BrokerConfig {
    
    CommissionModel commission;
    SlippageModel slippage;

};