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

enum class SelectionType { Tournament, Roulette };
enum class CrossoverType { Arithmetic, BLX };
enum class MutationType { Gaussian, Uniform };

struct GAParameters {
    int maxGeneration = 200;
    double stagnationEps = 1e-6;
    int stagnationWindow = 20;
    int populationSize = 100;
    double nicheRadius = 0.5;
    double crossoverProb = 0.8;
    double mutationProb = 0.2;
    SelectionType selection = SelectionType::Tournament;
    CrossoverType crossover = CrossoverType::Arithmetic;
    MutationType mutation = MutationType::Gaussian;
};

struct GenerationSnapshot {
    int generationNumber;
    std::vector<Individual> population;
    std::vector<Individual> maxima;
    double bestFitness;
    double avgFitness;
};
