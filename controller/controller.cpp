#include "controller.h"
#include "Polynomial.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <random>

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

    connect(window_, &MainWindow::saveRequested, 
        this, &Controller::saveResults);
    
    connect(window_, &MainWindow::loadRequested, 
        this, &Controller::loadParameters);
    
    connect(window_, &MainWindow::generateRequested, 
        this, &Controller::generateParameters);
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

    std::vector<std::pair<double, double>> locals;
    for (const auto& ind : history_[index].maxima) {
        locals.push_back({ind.x, ind.rawFitness});
    }

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

void Controller::saveResults()
{
    if (history_.empty()) return;

    QString fileName = QFileDialog::getSaveFileName(window_, "Сохранить результаты", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        auto [params, poly] = window_->inputHandle();

        out << "    Параметры:\n";
        out << "Polynom=" << QString::fromStdString(poly.polynom) << "\n";
        out << "L=" << poly.leftBorder << "\n";
        out << "R=" << poly.rightBorder << "\n";
        out << "Population=" << params.populationSize << "\n";
        out << "Generations=" << params.maxGeneration << "\n";
        out << "CrossProb=" << params.crossoverProb << "\n";
        out << "MutProb=" << params.mutationProb << "\n";
        out << "NicheRadius=" << params.nicheRadius << "\n";
        out << "StagnationEps=" << params.stagnationEps << "\n";
        out << "StagnationWindow=" << params.stagnationWindow << "\n";

        out << "\n    Результаты (Поколение " << currentGeneration_ + 1 << "):\n";
        out << "Глобальный максимум: " << history_[currentGeneration_].bestFitness << "\n";
        out << "Локальные максимумы (x, y):\n";
        for (const auto& max : history_[currentGeneration_].maxima) {
            out << "x=" << max.x << " y=" << max.rawFitness << "\n";
        }
        file.close();
    }
}

void Controller::loadParameters()
{
    QString fileName = QFileDialog::getOpenFileName(window_, "Загрузить параметры", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        auto [params, poly] = window_->inputHandle(); 

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith("-")) continue; 
            
            QStringList parts = line.split('=');
            if (parts.size() != 2) continue;
            
            QString key = parts[0].trimmed();
            QString val = parts[1].trimmed();

            if (key == "Polynom") poly.polynom = val.toStdString();
            else if (key == "L") poly.leftBorder = val.toDouble();
            else if (key == "R") poly.rightBorder = val.toDouble();
            else if (key == "Population") params.populationSize = val.toInt();
            else if (key == "Generations") params.maxGeneration = val.toInt();
            else if (key == "CrossProb") params.crossoverProb = val.toDouble();
            else if (key == "MutProb") params.mutationProb = val.toDouble();
            else if (key == "NicheRadius") params.nicheRadius = val.toDouble();
            else if (key == "StagnationEps") params.stagnationEps = val.toDouble();
            else if (key == "StagnationWindow") params.stagnationWindow = val.toInt();
            else if (key == "CrossType") {
                if (val == "Арифметическое") params.crossover = CrossoverType::Arithmetic;
                else params.crossover = CrossoverType::BLX;
            }
            else if (key == "MutType") {
                if (val == "Гауссова") params.mutation = MutationType::Gaussian;
                else params.mutation = MutationType::Uniform;
            }
            else if (key == "SelType") {
                if (val == "Турнир") params.selection = SelectionType::Tournament;
                else params.selection = SelectionType::Roulette;
            }
        }
        file.close();
        
        window_->outHandle(params, poly);
    }
}

void Controller::generateParameters()
{
    GAParameters params;
    PolynomData poly;

    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Генерация случайного полинома (степень от 2 до 5)
    std::uniform_int_distribution<> degDist(2, 5);
    std::uniform_real_distribution<> coefDist(-15.0, 15.0);
    
    int degree = degDist(gen);
    QString polyStr = "";
    
    for (int i = degree; i >= 0; --i) {
        double c = coefDist(gen);
        if (i == degree && std::abs(c) < 1.0) c = (c > 0 ? 1.5 : -1.5); 
        
        if (c > 0 && i != degree) polyStr += "+";
        polyStr += QString::number(c, 'f', 2);
        
        if (i > 1) polyStr += "x^" + QString::number(i);
        else if (i == 1) polyStr += "x";
    }
    poly.polynom = polyStr.toStdString();
    
    std::uniform_real_distribution<> borderDist(-50.0, 0.0);
    poly.leftBorder = std::floor(borderDist(gen));
    poly.rightBorder = poly.leftBorder + std::floor(std::uniform_real_distribution<>(20.0, 80.0)(gen));
    
    params.populationSize = std::uniform_int_distribution<>(30, 150)(gen);
    params.maxGeneration = std::uniform_int_distribution<>(100, 500)(gen);
    params.crossoverProb = std::uniform_real_distribution<>(0.5, 0.95)(gen);
    params.mutationProb = std::uniform_real_distribution<>(0.05, 0.3)(gen);
    params.nicheRadius = std::uniform_real_distribution<>(0.1, 5.0)(gen);
    params.stagnationEps = std::uniform_real_distribution<>(0.0001, 0.1)(gen);
    params.stagnationWindow = std::uniform_int_distribution<>(5, 50)(gen);
    params.crossover = std::uniform_int_distribution<>(0, 1)(gen) == 0 ? CrossoverType::Arithmetic : CrossoverType::BLX;
    params.mutation = std::uniform_int_distribution<>(0, 1)(gen) == 0 ? MutationType::Gaussian : MutationType::Uniform;
    params.selection = std::uniform_int_distribution<>(0, 1)(gen) == 0 ? SelectionType::Tournament : SelectionType::Roulette;
    
    window_->outHandle(params, poly);
}
