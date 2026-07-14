#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>

#include "mainwindow.h"
#include "GeneticAlgorithm.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(MainWindow* window);

private slots:
    void startGA();
    void nextGeneration();
    void prevGeneration();
    void firstGeneration();
    void lastGeneration();
    void saveResults();
    void loadParameters();
    void generateParameters();

private:
    void showGeneration(size_t index);
    void drawFunction(const ProblemDefinition& problem);

    MainWindow* window_;
    std::vector<GenerationSnapshot> history_;
    size_t currentGeneration_ = 0;
};

#endif