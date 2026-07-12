#include "controller.h"
#include "Polynomial.h"

Controller::Controller(MainWindow *window)
    : window_(window)
{
    connect(window_, &MainWindow::startRequested,
            this, &Controller::startGA);

    connect(window_, &MainWindow::nextRequested,
            this, &Controller::nextGeneration);

    connect(window_, &MainWindow::prevRequested,
            this, &Controller::prevGeneration);

    connect(window_, &MainWindow::toBeginRequested,
            this, &Controller::firstGeneration);

    connect(window_, &MainWindow::toEndRequested,
            this, &Controller::lastGeneration);
            
    connect(window_, &MainWindow::skipRequested,
        this, &Controller::lastGeneration);
}


void Controller::startGA()
{
    auto [params, poly] = window_->inputHandle();

    ProblemDefinition problem;

    problem.l = poly.leftBorder;
    problem.r = poly.rightBorder;
    problem.coeffs = parsePolynomial(poly.polynom);

    GeneticAlgorithm ga(problem, params);

    while (!ga.converged()) {
        ga.step();
    }

    history_ = ga.getHistory();
    currentGeneration_ = 0;
    drawFunction(problem);
    showGeneration(currentGeneration_);
}

void Controller::showGeneration(size_t index)
{
    if (index >= history_.size())
        return;

    window_->updatePlots(history_, index);

    std::vector<std::vector<double>> population(1);

    for (const auto& ind : history_[index].population)
        population[0].push_back(ind.x);

    std::vector<double> locals;

    for (const auto& ind : history_[index].maxima)
        locals.push_back(ind.x);

    double best = history_[index].bestFitness;

    window_->updateResults(population, locals, best);

    size_t total = history_.size();
    size_t current = index + 1;

    window_->arrowHandle(total, current);
}

void Controller::nextGeneration()
{
    if (currentGeneration_ + 1 < history_.size())
    {
        currentGeneration_++;
        showGeneration(currentGeneration_);
    }
}

void Controller::prevGeneration()
{
    if (currentGeneration_ > 0)
    {
        currentGeneration_--;
        showGeneration(currentGeneration_);
    }
}

void Controller::firstGeneration()
{
    currentGeneration_ = 0;
    showGeneration(currentGeneration_);
}

void Controller::lastGeneration()
{
    if (!history_.empty())
    {
        currentGeneration_ = history_.size() - 1;
        showGeneration(currentGeneration_);
    }
}

void Controller::drawFunction(const ProblemDefinition& problem)
{
    std::vector<double> xValues;
    std::vector<double> yValues;

    constexpr int points = 500;
    double step = (problem.r - problem.l) / points;

    for (int i = 0; i <= points; ++i)
    {
        double x = problem.l + i * step;
        xValues.push_back(x);
        yValues.push_back(evaluatePolynomial(problem.coeffs, x));
    }

    window_->drawPolynomPlot(yValues, xValues);
}