#include <vector>


struct Individual {
    double x;
    double rawFitness;
    double sharedFitness;
};

struct ProblemDefinition {
    std::vector<double> coeffs;
    double l, r;
};

enum class CrossoverType { Arithmetic, BLX };
enum class MutationType { Gaussian, Uniform };

struct GAParameters {
    int maxGeneration = 200;
    int eps = 1e-6;
    int window = 20;
    int populationSize = 100;
    double crossoverProb = 0.8;
    double mutationProb = 0.2;
    CrossoverType crossover = CrossoverType::Arithmetic;
    MutationType mutation = MutationType::Gaussian;
};

struct GenerationSnapshot {
    int generationNumber;
    std::vector<Individual> population;
    std::vector<Individual> currentMaximum;
    double bestFitness;
    double avgFitness;
};
