#pragma once


#include "Options.h"

class IPricingModel {

public:

	virtual ~IPricingModel() = default;

	virtual double calculatePrice(const OptionContract& option) = 0;

	virtual void calculateGreeks(OptionContract& option) = 0;

};