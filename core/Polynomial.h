#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <string>

double evaluatePolynomial(const std::vector<double>& coeffs, double x);

std::vector<double> parsePolynomial(const std::string& expression);

#endif