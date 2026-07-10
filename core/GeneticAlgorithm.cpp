#include "GeneticAlgorithm.h"
#include "Polynomial.h"
#include <cmath>
#include <algorithm>

GeneticAlgorithm::GeneticAlgorithm(const ProblemDefinition& problem,
                                   const GAParameters& params) : // Л
    problem_(problem),
    params_(params),
    rng_(std::random_device{}())
{
    initializePopulation();
}

void GeneticAlgorithm::initializePopulation() { // Л
    population_.clear();
    population_.reserve(params_.populationSize);

    std::uniform_real_distribution<double> distribution(
        problem_.l,
        problem_.r
    );

    for (int i = 0; i < params_.populationSize; i++) {
        Individual ind;

        ind.x = distribution(rng_);
        ind.rawFitness = evaluatePolynomial(problem_.coeffs, ind.x);

        population_.push_back(ind);
    }

    history_.push_back(buildSnapshot());
}

void GeneticAlgorithm::step() { // Л
    computeSharedFitness();

    std::vector<Individual> newPopulation;
    newPopulation.reserve(params_.populationSize);

    // лучшая особь - элита
    Individual bestInd = population_[0];
    for (const auto& ind : population_) {
        if (ind.rawFitness > bestInd.rawFitness) {
            bestInd = ind;
        }
    }
    newPopulation.push_back(bestInd);

    for (int i = 1; i < params_.populationSize; i++) {
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

bool GeneticAlgorithm::converged() const { // Л, Е
    int n = history_.size();
    if (n >= params_.stagnationWindow) {
        bool stagnant = true;
        for (int i = n - params_.stagnationWindow + 1; i < n; ++i) {
            double diff = history_[i].bestFitness - history_[i-1].bestFitness;
            if (diff > params_.stagnationEps) { // улучшение больше порога (только рост считается прогрессом, так что без модуля)
                stagnant = false;
                break;
            }
        }
        if (stagnant) return true;
    }
    return (n >= params_.maxGeneration);
} // Переделал так, чтобы проверять, что на *каждом* шаге внутри окна изменение 
// лучшего фитнеса не превышало stagnationEps, чтобы не пропускать колебания внутри окна
// и учитывать возможность того, что улучшение произошло, но было потеряно
// мб стоит хранить значение лучшего индивидума-элиты, чтоб лучшее решение не терялось 

void GeneticAlgorithm::computeSharedFitness() {
    // Находим минимальное значение приспособленности
    double minFitness = population_[0].rawFitness;
    for (const auto& ind : population_) {
        if (ind.rawFitness < minFitness) {
            minFitness = ind.rawFitness;
        }
    }
    
    // Если есть отрицательные значения, вычисляем величину сдвига вверх, 
    // чтобы весь график был положительным и алгоритм не поощрял скученность при отрицательной приспособленности 
    // (+ небольшая константа, чтобы не ноль)
    double shift = (minFitness < 0) ? std::abs(minFitness) + 1e-5 : 0.0;

    for (size_t i = 0; i < population_.size(); ++i) {
        double nicheCount = 0.0;
        for (size_t j = 0; j < population_.size(); ++j) {
            double distance = std::abs(population_[i].x - population_[j].x);
            if (distance < params_.nicheRadius) {
                nicheCount += (1.0 - (distance / params_.nicheRadius));
            }
        }
        
        population_[i].sharedFitness = (population_[i].rawFitness + shift) / nicheCount;
    }
}

Individual GeneticAlgorithm::selectParent() { // Е
    if (params_.selection == SelectionType::Tournament) {
        return tournamentSelect();
    } else {
        return rouletteSelect();
    }
}

Individual GeneticAlgorithm::tournamentSelect() { // Е
    std::uniform_int_distribution<int> dist(0, population_.size() - 1);
    
    Individual p1 = population_[dist(rng_)];
    Individual p2 = population_[dist(rng_)];
    
    return (p1.sharedFitness > p2.sharedFitness) ? p1 : p2;
}

Individual GeneticAlgorithm::rouletteSelect() { // Е
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

    if (sumFitness == 0.0) {
        std::uniform_int_distribution<int> intDist(0, population_.size() - 1);
        return population_[intDist(rng_)];
    }

    // выбираем случайное число от 0 до суммы
    std::uniform_real_distribution<double> dist(0.0, sumFitness);
    double randomValue = dist(rng_);

    double currentSum = 0.0;
    for (const auto& ind : population_) {
        currentSum += (ind.sharedFitness - minFitness);
        if (currentSum >= randomValue) {
            return ind; // победитель
        }
    }

    return population_.back(); 
}

Individual GeneticAlgorithm::crossover(const Individual& p1, const Individual& p2) { // Е
    if (params_.crossover == CrossoverType::Arithmetic) {
        return arithmeticCrossover(p1, p2);
    } else {
        return blxCrossover(p1, p2);
    }
}

Individual GeneticAlgorithm::arithmeticCrossover(const Individual& p1, const Individual& p2) { // Е
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    
    Individual child;
    if (chance(rng_) < params_.crossoverProb) {
        // среднее арифметическое
        child.x = (p1.x + p2.x) / 2.0;
    } else {
        child.x = p1.x; 
    }
    return child;
}

Individual GeneticAlgorithm::blxCrossover(const Individual& p1, const Individual& p2) { // Е
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    
    Individual child;
    
    if (chance(rng_) < params_.crossoverProb) {
        double alpha = 0.5;
        
        double min_x = std::min(p1.x, p2.x);
        double max_x = std::max(p1.x, p2.x);
        double distance = max_x - min_x;
        
        double lower_bound = std::max(problem_.l, min_x - alpha * distance);
        double upper_bound = std::min(problem_.r, max_x + alpha * distance);
        
        // ребенок в случайной точке расширенного отрезка
        std::uniform_real_distribution<double> dist(lower_bound, upper_bound);
        child.x = dist(rng_);
    } else {
        child.x = p1.x; 
    }
    
    return child;
}

void GeneticAlgorithm::mutate(Individual& ind) { // Е
    if (params_.mutation == MutationType::Gaussian) {
        gaussianMutate(ind);
    } else {
        uniformMutate(ind);
    }
}

void GeneticAlgorithm::uniformMutate(Individual& ind) { // Е
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    
    if (chance(rng_) < params_.mutationProb) {
        // замена координаты на полностью случайное число
        std::uniform_real_distribution<double> dist(problem_.l, problem_.r);
        ind.x = dist(rng_);
    }
}

void GeneticAlgorithm::gaussianMutate(Individual& ind) {
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    
    if (chance(rng_) < params_.mutationProb) { // Е
        double sigma = (problem_.r - problem_.l) * 0.1; //отклонение сигма - 10% от всего интервала
        
        std::normal_distribution<double> dist(0.0, sigma);
        
        ind.x += dist(rng_); // прибалвяет к коорд случ число по Гауссу
        
        if (ind.x < problem_.l) {
            ind.x = problem_.l;
        } else if (ind.x > problem_.r) {
            ind.x = problem_.r;
        }
    }
}

GenerationSnapshot GeneticAlgorithm::buildSnapshot() {  // Е
    GenerationSnapshot snap;
    snap.generationNumber = history_.size();
    snap.population = population_;
    snap.maxima = extractMaxima();
    
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
    
    return snap;
}

std::vector<Individual> GeneticAlgorithm::extractMaxima() const { // Л
    if (population_.empty()) return {};

    std::vector<Individual> sorted = population_;
    std::sort(sorted.begin(), sorted.end(),
              [](const Individual& a, const Individual& b) { return a.rawFitness > b.rawFitness; }); // лямбда-фнукция, сравнивает координаты
    
    std::vector<Individual> maxima;

    for (const auto& ind : sorted) {
        bool isNewPeak = true;
        
        // проверка, не накрывается ли эта особь радиусом уже найденного более высокого пика
        for (const auto& maxInd : maxima) {
            if (std::abs(ind.x - maxInd.x) < params_.nicheRadius) {
                isNewPeak = false;
                break;
            }
        }

        if (isNewPeak) {
            // отступаем на 0.1 в обе стороны 
            // Это игнорирует погрешности га, но отсекает точки на склонах
            double center = ind.rawFitness;
            double left = evaluatePolynomial(problem_.coeffs, ind.x - 0.1);
            double right = evaluatePolynomial(problem_.coeffs, ind.x + 0.1);
            
            if (center > left && center > right) {
                maxima.push_back(ind);
            }
        }
    }

    std::sort(maxima.begin(), maxima.end(),
              [](const Individual& a, const Individual& b) { return a.x < b.x; });

    return maxima;
}

bool GeneticAlgorithm::isLocalMaximum(double x, double eps) const { // Л
    double center = evaluatePolynomial(problem_.coeffs, x);
    double left   = evaluatePolynomial(problem_.coeffs, x - eps);
    double right  = evaluatePolynomial(problem_.coeffs, x + eps);
    return center >= left && center >= right;
}
