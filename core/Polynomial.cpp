#include "Polynomial.h"
#include <cmath>
#include <sstream>
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

    if (s.empty()) return {};

    std::vector<double> coeffs(9, 0.0);
    
    int i = 0;
    while (i < s.size()) {
        int sign = 1;
        if (s[i] == '+') {
            sign = 1;
            i++;
        } else if (s[i] == '-') {
            sign = -1;
            i++;
        }

        double coef = 0;
        bool hasNumber = false;
        while (i < s.size() && std::isdigit(s[i])) {
            coef = coef * 10 + (s[i] - '0');
            i++;
            hasNumber = true;
        }
        if (!hasNumber) coef = 1;
        
        if (i < s.size() && s[i] == 'x') {
            i++;
            int power = 1;
            if (i < s.size() && s[i] == '^') {
                i++;
                power = 0;
                while (i < s.size() && std::isdigit(s[i])) {
                    power = power * 10 + (s[i] - '0');
                    i++;
                }
            } 
            if (power <= 8) coeffs[power] += sign * coef;

        } else {
            coeffs[0] += sign * coef;
        }
    }

    while (coeffs.size() > 1 && coeffs.back() == 0) coeffs.pop_back();

    return coeffs;
}