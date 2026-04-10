// i am using the implementation as shown in 
//**SOURCE** https://www.quantstart.com/articles/European-vanilla-option-pricing-with-C-and-analytic-formulae/ 

#define _USE_MATH_DEFINES

#include "Roots.h"
#include "Pricing/BlackScholes.h"
#include "Options.h"

double BlackScholesModel::PDF(double value) {

	return (1 / (std::sqrt(2*M_PI))) * std::exp(-0.5 * value * value);

}


double BlackScholesModel::CDF(double x) {

	bool isNegative = x < 0.0;
    if (isNegative) {
        x = -x;
    }

    double k = 1.0 / (1.0 + 0.2316419 * x);
    double k_sum = k * (0.319381530 + k * (-0.356563782 + k * (1.781477937 + k * (-1.821255978 + 1.330274429 * k))));

    double cdf = 1.0 - (1 / (std::sqrt(2*M_PI))) * std::exp(-0.5 * x * x) * k_sum;

    return isNegative ? 1.0 - cdf : cdf;

}

double BlackScholesModel::calculate_D_j(const int j, double asset_price, double strike_price, double risk_free_rate, double TimeToMaturity, double volatility, double dividend_yield) {

	if (TimeToMaturity <= 0.0 || volatility <= 0.0) {
        throw std::invalid_argument("Time to maturity and volatility not positive.");
    }

    double d1 = 
    (std::log(asset_price / strike_price) + (risk_free_rate - dividend_yield + (volatility * volatility) / 2.0) * TimeToMaturity) /
     (volatility * std::sqrt(TimeToMaturity));


    if (j == 1) {    
        return d1;
    
    } 
    else if (j == 2) {
        return d1 - volatility * std::sqrt(TimeToMaturity);
    
    } 
    else {
        throw std::invalid_argument("j must be strictly 1 or 2.");
    }

}

double  BlackScholesModel::call_price(const double S, const double K, const double r, const double v, const double T, const double q) {

	return S * std::exp(-q * T) * CDF(calculate_D_j(1, S, K, r, T, v, q)) - K * std::exp(-r*T) * 
	CDF(calculate_D_j(2, S, K, r, T, v, q));

}

double BlackScholesModel::put_price(const double S, const double K, const double r, const double v, const double T, const double q) {

	 return -S * std::exp(-q * T) * CDF(-calculate_D_j(1, S, K, r, T, v, q)) + K * std::exp(-r*T) * 
	 CDF(-calculate_D_j(2, S, K, r, T, v, q));

}


double BlackScholesModel::calculatePrice(const OptionContract& option)  {

	double S = option.getMarketdata().underlying_price;
    double K = option.getStrike();
    double v = option.getMarketdata().implied_volatility;

    double r = this->riskFreeRate;

    std::chrono::sys_days currentDate = option.getDate();
    std::chrono::sys_days expiryDate = option.getExpiration();
    auto daysToExpiry = (expiryDate - currentDate).count();

    double T = std::max(daysToExpiry / 365.0, 0.0001);


	OptionType type = option.getType();

	switch (type) {

		case OptionType::Option_Call: {

			return call_price(S, K, r, v, T);
		}
		
		case OptionType::Option_Put: {

			return put_price(S, K, r, v, T);
		}

	default:{

		return 0.0;
	}

	}
}


void BlackScholesModel::calculateGreeks(OptionContract& option) {

    double S = option.getMarketdata().underlying_price;
    double K = option.getStrike();
    double market_price = option.getMarketdata().close;
    double r = this->riskFreeRate;
    OptionType type = option.getType();

    std::chrono::sys_days currentDate = option.getDate();
    std::chrono::sys_days expiryDate = option.getExpiration();
    auto daysToExpiry = (expiryDate - currentDate).count();
    
    double T = std::max(daysToExpiry / 365.0, 0.0001);

    ImpliedVolFunctor functor = {this, S, K, r, T, type};


    double initial_guess = 0.20; 
    double tolerance = 1e-5;

    double impliedVol = Utils::newtonraphson<ImpliedVolFunctor, &ImpliedVolFunctor::getPrice, &ImpliedVolFunctor::getVega>
                                            (market_price, initial_guess, tolerance, functor, newton_raphson_iterations);

   
    if (impliedVol <= 0.0) { impliedVol = 0.0001; }


    double d1 = calculate_D_j(1, S, K, r, T, impliedVol);
    double d2 = calculate_D_j(2, S, K, r, T, impliedVol);

    double pdf_d1 = PDF(d1);
    double cdf_d1 = CDF(d1);
    double cdf_d2 = CDF(d2);

    Greeks greeks;


    greeks.vega = S * pdf_d1 * std::sqrt(T);
    greeks.gamma = pdf_d1 / (S * impliedVol * std::sqrt(T));

    if (type == OptionType::Option_Call) {
    
        greeks.delta = cdf_d1;
        greeks.theta = (-S * pdf_d1 * impliedVol) / (2.0 * std::sqrt(T)) - r * K * std::exp(-r * T) * cdf_d2;
        greeks.rho   = K * T * std::exp(-r * T) * cdf_d2;
    
    } 
    else {
    
        greeks.delta = cdf_d1 - 1.0;
        greeks.theta = (-S * pdf_d1 * impliedVol) / (2.0 * std::sqrt(T)) + r * K * std::exp(-r * T) * CDF(-d2);
        greeks.rho   = -K * T * std::exp(-r * T) * CDF(-d2);
    
    }

    Marketdata md = option.getMarketdata();
    md.implied_volatility = impliedVol;

    option.setMarketdata(md);
    option.setGreeks(greeks);  

}