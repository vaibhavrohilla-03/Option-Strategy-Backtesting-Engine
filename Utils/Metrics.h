#pragma once

#include <vector>
#include <cmath>
#include "Portfolio.h"

namespace Utils {


	struct DrawdownMetrics {

		double max_drawdown = 0.0;
		double max_drawdown_percnt = 0.0;
		double max_duration = 0.0;
		
		std::vector<double> drawdown_series;

	};

	inline const double calculateMean(const std::vector<double>& returns) {

		double sum = 0;
    	
    	int n = returns.size();
    
	    for (int i = 0; i < n; i++) {
	        
	        sum += returns[i];
	    }
    	
    	return sum / n;
	}

	inline const double calculateStdDev(const std::vector<double>& returns, const double mean) {

		double sum = 0;
    	int n = returns.size();
	    
	    for (double ret : returns) {
	        	
	    	double diff = ret - mean;
	        sum += diff * diff;
	    }
    
    	return sqrt(sum / (n - 1));
	}

	inline const double CalculateSharpeRatio(const std::vector<double>& returns, const double riskFreeRate) {

		double mean = calculateMean(returns);
    	double stdDev = calculateStdDev(returns, mean);
    	double excessReturns = mean - riskFreeRate;
    
    	return excessReturns / stdDev;
	}

	inline const double CalculateSharpeRatio(const std::vector<double>& returns, double riskFreeRate, const int period) {

		double mean = calculateMean(returns);
    	double stdDev = calculateStdDev(returns, mean);
    	double excessReturns = mean - riskFreeRate;
    
    	return std::sqrt(period) * (excessReturns / stdDev);

	}

	inline DrawdownMetrics create_drawdown(const std::vector<PortfolioHoldings>& equity_curve) {

		DrawdownMetrics metrics;
	    
	    double high_water_mark = 0.0;

	    int current_duration = 0;
		int max_duration = 0;

	    metrics.drawdown_series.reserve(equity_curve.size());



	    for (const auto& snapshot : equity_curve) {
	        double current_equity = snapshot.total_equity;

	        if (current_equity > high_water_mark) {
	            high_water_mark = current_equity;
	        }

	        double current_drawdown = 0.0;
	        if (high_water_mark > 0.0) {
	            
	            // (Peak - Current) / Peak
	            current_drawdown = (high_water_mark - current_equity) / high_water_mark;
	        }

	        metrics.drawdown_series.push_back(current_drawdown);

	        if(current_drawdown > 0.0) {
	        	current_duration++;
	        	
	        	if (current_duration > metrics.max_duration) {
                	metrics.max_duration = current_duration;
            	}
	        } 
	        else {
	        	current_duration = 0;
	        }

	        if (current_drawdown > metrics.max_drawdown) {
	            metrics.max_drawdown = current_drawdown;
	        }
	    }

	    metrics.max_drawdown_percnt = metrics.max_drawdown * 100.0;

	    return metrics;
	}

	inline double CalculateROI() {


		return 0.0;
	}

	inline double CalculateAnnualizedVolatility() {

		return 0.0;
	}

}