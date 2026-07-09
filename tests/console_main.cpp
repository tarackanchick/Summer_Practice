#include "../core/GeneticAlgorithm.h"
#include <iostream>
#include <iomanip>

int main() {
    ProblemDefinition problem;
    problem.coeffs = {1.0, 0.0, -5.0, 0.0, 0.5};
    problem.l = -4.0;
    problem.r = 4.0;

    GAParameters params;

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
