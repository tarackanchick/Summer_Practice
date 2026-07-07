#include "GeneticAlgorithm.h"
#include "Polynomial.h"
#include <random>


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
