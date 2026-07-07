#include <vector>
#include "Types.h"


class GeneticAlgorithm {
public:
    GeneticAlgorithm(const ProblemDefinition& problem, const GAParameters& params);

    void step();
    bool converged() const;

    const std::vector<GenerationSnapshot>& getHistory() const { return history_; }
    
private:
    void computeSharedFitness();
    Individual selectParent();
    Individual crossover(const Individual& p1, const Individual& p2);
    void mutate(Individual& ind);
    GenerationSnapshot buildSnapshot();

    ProblemDefinition problem_;
    GAParameters params_;
    std::vector<Individual> population_;
    std::vector<GenerationSnapshot> history_;
};
