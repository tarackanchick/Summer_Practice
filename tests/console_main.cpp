#include "../core/GeneticAlgorithm.h" // Л, Е
#include <iostream>
#include <iomanip>

int main() {
    ProblemDefinition problem;
    // problem.coeffs = {1.0, 0.0, -5.0, 0.0, 0.5};
    // problem.coeffs = {5.0, 0.0, -20.0, 0.0, 10.0, 0.0, -1.0};
    problem.coeffs = {1.0, 0.5, 20.0, 0.0, -25.0, 0.0, 9.0, 0.0, -1.0};
    // problem.coeffs = {1.0, 0.5, 20.0, 0.0, -25.0, 0.0, 10.0, 0.0, -1.0};

    problem.l = -4.0;
    problem.r = 4.0;

    GAParameters params;

    params.populationSize = 200;      
    params.maxGeneration = 300;      
    params.stagnationWindow = 1000;    
    params.stagnationEps = 1e-5;     
    
    params.nicheRadius = 1.0;        
    params.crossoverProb = 0.10;      
    params.mutationProb = 0.80;

    GeneticAlgorithm ga(problem, params);
    while (!ga.converged()) {
        ga.step();
    }

    const auto& history = ga.getHistory();
    const auto& last = history.back();

    std::cout << "Generations completed: " << history.size() << "\n";
    std::cout << "Best fitness: " << last.bestFitness << "\n";
    std::cout << "Found maxima:\n";
    for (const auto& m : last.maxima) {
        std::cout << "  x = " << std::fixed << std::setprecision(7) << m.x
                  << ", f(x) = " << m.rawFitness << "\n";
    }

    return 0;
}
