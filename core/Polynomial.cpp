#include "Polynomial.h"
#include <cmath>
#include <algorithm>
#include <cctype>

double evaluatePolynomial(const std::vector<double>& coeffs, double x) {
    double result = 0.0;
    double current_power = 1.0;
    
    for (double coeff : coeffs) {
        result += coeff * current_power;
        current_power *= x; 
    }
    return result;
}

std::vector<double> parsePolynomial(const std::string& expression) {
    std::string s;
    for (char c : expression) {
        if (!std::isspace(c)) s += c;
    }
    
    std::vector<double> coeffs;
    if (s.empty()) return coeffs;

    int i = 0;
    while (i < s.size()) {
        double sign = 1.0;
        if (s[i] == '+') { sign = 1.0; i++; }
        else if (s[i] == '-') { sign = -1.0; i++; }
        
        double coef = 0.0;
        bool hasNumber = false;
        size_t num_len = 0;
        try {
            coef = std::stod(s.substr(i), &num_len);
            i += num_len;
            hasNumber = true;
        } catch (...) {
            coef = 1.0; 
        }

        if (i < s.size() && s[i] == '*') i++; 
        
        int power = 0;
        if (i < s.size() && (s[i] == 'x' || s[i] == 'X')) {
            i++;
            power = 1; 
            if (i < s.size() && s[i] == '^') {
                i++;
                size_t p_len = 0;
                try {
                    power = std::stoi(s.substr(i), &p_len);
                    i += p_len;
                } catch (...) {
                    power = 1;
                }
            }
        } else if (!hasNumber) {
            break;
        }
        
        if (power >= coeffs.size()) {
            coeffs.resize(power + 1, 0.0);
        }
        
        coeffs[power] += sign * coef;
    }

    return coeffs;
}