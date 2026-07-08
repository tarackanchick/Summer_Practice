#include "Polynomial.h"
#include <cmath>

double evaluatePolynomial(const std::vector<double>& coeffs, double x) {
    double result = 0.0;
    double current_power = 1.0;
    
    for (double coeff : coeffs) {
        result += coeff * current_power;
        current_power *= x; 
    }
    
    return result;
}