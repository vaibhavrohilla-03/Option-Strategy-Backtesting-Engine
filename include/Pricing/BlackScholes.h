#pragma once

#include "IPricingModel.h"
#include <cmath>

class OptionContract;

class BlackScholesModel : public IPricingModel {

public:

	inline BlackScholesModel(double riskRate) : riskFreeRate(riskRate) {}

	double calculatePrice(const OptionContract& option) override;

	void calculateGreeks(OptionContract& option) override;

	struct ImpliedVolFunctor {
    BlackScholesModel* model;
    double S;
    double K;
    double r;
    double T;
    OptionType type;

    inline double getPrice(double vol) const {
        
        if (vol <= 0.0) vol = 0.0001;

        if (type == OptionType::Option_Call) {
            return model->call_price(S, K, r, vol, T);
        } 
        else {
            return model->put_price(S, K, r, vol, T);
        }
    }

    inline double getVega(double vol) const {

        if (vol <= 0.0) vol = 0.0001; 
        
        double d1 = model->calculate_D_j(1, S, K, r, T, vol);
        return S * model->PDF(d1) * std::sqrt(T);
    }
};



private:

	double riskFreeRate;

	double CDF(double x);

	double PDF(double x);

	double calculate_D_j(const int j, double asset_price, double strike_price, double risk_free_rate, double TimeToMaturity, double volatility, double dividend_yield = 0);

	double call_price(const double S, const double K, const double r, const double v, const double T, const double q = 0);

	double put_price(const double S, const double K, const double r, const double v, const double T, const double q = 0);

};
