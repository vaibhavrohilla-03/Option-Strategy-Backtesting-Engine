#pragma once

#include <cmath>

namespace Utils {

	template<typename T, double (T::*Func)(double) const, double (T::*DerivativeFunc)(double) const >
	double newtonraphson(double target_price, double initialV, double tolerance, const T& root_func, int max_iterations) {

		double fx = (root_func.*Func)(initialV); 
    	double dx = (root_func.*DerivativeFunc)(initialV);
		
		int iterations = 0;

    	while (std::abs(fx - target_price) > tolerance && iterations < max_iterations) {
    		
    		if (dx == 0.0) {
    			break; // Prevent division by zero
    		}

    		initialV = initialV + (target_price - fx) / dx;

    		fx = (root_func.*Func)(initialV); 
    		
    		dx = (root_func.*DerivativeFunc)(initialV);

    		iterations++;
    	}

    	return initialV;

	}
	

}