#pragma once

#include <cmath>

namespace Utils {

	template<typename T, double (T::*Func)(double) const, double (T::*DerivativeFunc)(double) const >
	double newtonraphson(double target_price, double initialV, double tolerance, const T& root_func) {

		double fx = (root_func.*Func)(initialV); 
    	double dx = (root_func.*DerivativeFunc)(initialV);
		

    	while (std::abs(fx - target_price) > tolerance) {

    		initialV = initialV + (target_price - fx) / dx;

    		fx = (root_func.*Func)(initialV); 
    		
    		dx = (root_func.*DerivativeFunc)(initialV);

    	}

    	return initialV;

	}
	

}