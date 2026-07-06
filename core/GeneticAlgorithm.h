#include <vector>
#include "Types.h"


class GeneticAlgorithm {
public:
    GeneticAlgorithm(const ProblemDefinition& problem, const GAParameters& params);

    void step();
    bool stop() const;

    const std::vector<GenerationSnapshot>& getHistory() const { return history_; }
    
private:
    ProblemDefinition problem_;
    GAParameters params_;
    std::vector<Individual> population_;
    std::vector<GenerationSnapshot> history_;
};
