#include "GeneticAlgorithm.h"
#include "Polynomial.h"
#include <random>
#include <cmath>
#include <algorithm>

GeneticAlgorithm::GeneticAlgorithm(const ProblemDefinition& problem,
                                   const GAParameters& params) :
    problem_(problem),
    params_(params)
{
    initializePopulation();
}

void GeneticAlgorithm::initializePopulation() {
    population_.clear();
    population_.reserve(params_.populationSize);

    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_real_distribution<double> distribution(
        problem_.l,
        problem_.r
    );

    for (int i = 0; i < params_.populationSize; i++) {
        Individual ind;

        ind.x = distribution(rng);
        ind.rawFitness = evaluatePolynomial(problem_.coeffs, ind.x);

        population_.push_back(ind);
    }

    history_.push_back(buildSnapshot());
}

void GeneticAlgorithm::step() {
    computeSharedFitness();

    std::vector<Individual> newPopulation;
    newPopulation.reserve(params_.populationSize);
    for (int i = 0; i < params_.populationSize; i++) {
        Individual p1 = selectParent();
        Individual p2 = selectParent();
        Individual child = crossover(p1, p2);
        mutate(child);
        child.rawFitness = evaluatePolynomial(problem_.coeffs, child.x);

        newPopulation.push_back(child);
    }

    population_ = newPopulation;

    history_.push_back(buildSnapshot());
}

void GeneticAlgorithm::computeSharedFitness() {
    // Ниши: штраф, если особи слишком близко друг к другу
    for (size_t i = 0; i < population_.size(); ++i) {
        double nicheCount = 0.0;
        
        for (size_t j = 0; j < population_.size(); ++j) {
            double distance = std::abs(population_[i].x - population_[j].x);
            
            if (distance < params_.nicheRadius) {
                // Линейное убывание штрафа (альфа = 1)
                nicheCount += (1.0 - (distance / params_.nicheRadius));
            }
        }
        
        population_[i].sharedFitness = population_[i].rawFitness / nicheCount;
    }
}

Individual GeneticAlgorithm::selectParent() {
    if (params_.selection == SelectionType::Tournament) {
        return tournamentSelect();
    } else {
        return rouletteSelect();
    }
}

Individual GeneticAlgorithm::tournamentSelect() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, population_.size() - 1);
    
    Individual p1 = population_[dist(rng)];
    Individual p2 = population_[dist(rng)];
    
    return (p1.sharedFitness > p2.sharedFitness) ? p1 : p2;
}

Individual GeneticAlgorithm::rouletteSelect() {
    // поиск мин приспособленности, чтобы сдвинуть значения (если есть отрицательные)
    double minFitness = population_[0].sharedFitness;
    for (const auto& ind : population_) {
        if (ind.sharedFitness < minFitness) {
            minFitness = ind.sharedFitness;
        }
    }

    // Сумма приспособленностей 
    double sumFitness = 0.0;
    for (const auto& ind : population_) {
        sumFitness += (ind.sharedFitness - minFitness);
    }

    std::random_device rd;
    std::mt19937 rng(rd());

    if (sumFitness == 0.0) {
        std::uniform_int_distribution<int> intDist(0, population_.size() - 1);
        return population_[intDist(rng)];
    }

    // выбираем случайное число от 0 до суммы
    std::uniform_real_distribution<double> dist(0.0, sumFitness);
    double randomValue = dist(rng);

    double currentSum = 0.0;
    for (const auto& ind : population_) {
        currentSum += (ind.sharedFitness - minFitness);
        if (currentSum >= randomValue) {
            return ind; // победитель
        }
    }

    return population_.back(); 
}

Individual GeneticAlgorithm::crossover(const Individual& p1, const Individual& p2) {
    if (params_.crossover == CrossoverType::Arithmetic) {
        return arithmeticCrossover(p1, p2);
    } else {
        return blxCrossover(p1, p2);
    }
}

Individual GeneticAlgorithm::arithmeticCrossover(const Individual& p1, const Individual& p2) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    
    Individual child;
    if (chance(rng) < params_.crossoverProb) {
        // среднее арифметическое
        child.x = (p1.x + p2.x) / 2.0;
    } else {
        child.x = p1.x; 
    }
    return child;
}

Individual GeneticAlgorithm::blxCrossover(const Individual& p1, const Individual& p2) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    
    Individual child;
    
    if (chance(rng) < params_.crossoverProb) {
        double alpha = 0.5;
        
        double min_x = std::min(p1.x, p2.x);
        double max_x = std::max(p1.x, p2.x);
        double distance = max_x - min_x;
        
        double lower_bound = std::max(problem_.l, min_x - alpha * distance);
        double upper_bound = std::min(problem_.r, max_x + alpha * distance);
        
        // ребенок в случайной точке расширенного отрезка
        std::uniform_real_distribution<double> dist(lower_bound, upper_bound);
        child.x = dist(rng);
    } else {
        child.x = p1.x; 
    }
    
    return child;
}

void GeneticAlgorithm::mutate(Individual& ind) {
    if (params_.mutation == MutationType::Gaussian) {
        gaussianMutate(ind);
    } else {
        uniformMutate(ind);
    }
}

void GeneticAlgorithm::uniformMutate(Individual& ind) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    
    if (chance(rng) < params_.mutationProb) {
        // замена на полностью случайное число
        std::uniform_real_distribution<double> dist(problem_.l, problem_.r);
        ind.x = dist(rng);
    }
}

void GeneticAlgorithm::gaussianMutate(Individual& ind) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    
    if (chance(rng) < params_.mutationProb) {
        double sigma = (problem_.r - problem_.l) * 0.1; 
        
        std::normal_distribution<double> dist(0.0, sigma);
        
        ind.x += dist(rng);
        
        if (ind.x < problem_.l) {
            ind.x = problem_.l;
        } else if (ind.x > problem_.r) {
            ind.x = problem_.r;
        }
    }
}

GenerationSnapshot GeneticAlgorithm::buildSnapshot() {
    GenerationSnapshot snap;
    snap.generationNumber = history_.size();
    snap.population = population_;
    
    double sumFitness = 0.0;
    double maxFitness = -1e9; 
    
    for (const auto& ind : population_) {
        sumFitness += ind.rawFitness;
        if (ind.rawFitness > maxFitness) {
            maxFitness = ind.rawFitness;
        }
    }
    
    snap.bestFitness = maxFitness;
    snap.avgFitness = population_.empty() ? 0.0 : (sumFitness / population_.size());
    // snap.maxima - пока пусто, заполните позже логикой поиска локальных пиков для вывода на график
    
    return snap;
}