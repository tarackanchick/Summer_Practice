#include <vector>
#include "Types.h"


class GeneticAlgorithm {
public:
    GeneticAlgorithm(const ProblemDefinition& problem, const GAParameters& params);

    void step();
    bool converged() const;

    const std::vector<GenerationSnapshot>& getHistory() const { return history_; }
    
private:
    void initializePopulation();

    void computeSharedFitness();

    Individual selectParent();
    Individual tournamentSelect();
    Individual rouletteSelect();

    Individual crossover(const Individual& p1, const Individual& p2);
    Individual arithmeticCrossover(const Individual& p1, const Individual& p2);
    Individual blxCrossover(const Individual& p1, const Individual& p2);

    void mutate(Individual& ind);
    void gaussianMutate(Individual& ind);
    void uniformMutate(Individual& ind);

    GenerationSnapshot buildSnapshot();

    ProblemDefinition problem_;
    GAParameters params_;
    std::vector<Individual> population_;
    std::vector<GenerationSnapshot> history_;
};
