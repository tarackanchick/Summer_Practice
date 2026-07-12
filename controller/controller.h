#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Types.h"
#include "GeneticAlgorithm.h"
#include "mainwindow.h"

class Controller{
    GeneticAlgorithm gaAlg;
    MainWindow gui;
    PolynomData polynomData;
    GAParameters gaParams;

    std::vector<GenerationSnapshot> gaHistory;

public:
    Controller();

    void run();
    void loadData();
    bool checkInputData();
    std::vector<double> calcFunc(std::vector<double> coeffs, std::vector<double> funcArgs);
    std::vector<double> polynomParser(std::string polynom);

    ~Controller();
};

#endif
