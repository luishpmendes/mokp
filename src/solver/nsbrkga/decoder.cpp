#include "solver/nsbrkga/decoder.hpp"
#include <algorithm>

namespace mokp {

Decoder::Decoder(const Instance & instance,
                 unsigned num_threads)
    : instance(instance),
      knapsacks(num_threads,
             std::vector<bool>(instance.num_items, false)),
      values(num_threads,
            std::vector<double>(instance.num_dimensions, 0.0)),
      weights(num_threads,
            std::vector<double>(instance.num_dimensions, 0.0)),
      permutations(num_threads,
            std::vector<std::pair<double, unsigned>>(instance.num_items)) {}

std::vector<double> Decoder::decode(BRKGA::Chromosome & chromosome,
                                    bool /* not used */) {
#   ifdef _OPENMP
        std::vector<bool> & knapsack = this->knapsack[omp_get_thread_num()];
        std::vector<double> & value = this->values[omp_get_thread_num()];
        std::vector<double> & weight = this->weights[omp_get_thread_num()];
        std::vector<std::pair<double, unsigned>> & permutation
                = this->permutations[omp_get_thread_num()];
#   else
        std::vector<bool> & knapsack = this->knapsacks.front();
        std::vector<double> & value = this->values.front();
        std::vector<double> & weight = this->weights.front();
        std::vector<std::pair<double, unsigned>> & permutation
                = this->permutations.front();
#   endif

    for (unsigned i = 0; i < this->instance.num_items; i++) {
        permutation[i] = std::make_pair(chromosome[i], i);
    }

    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
        value[j] = 0;
        weight[j] = 0;
    }

    std::sort(permutation.begin(), permutation.end());

    bool has_space = true;

    for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
        bool item_fits = true;

        for (unsigned j = 0;
             j < this->instance.num_dimensions && item_fits;
             j++) {
            if (weight[j] + this->instance.min_weight[j]
                    > this->instance.capacity[j]) {
                has_space = false;
                item_fits = false;
            }

            if (weight[j] + this->instance.weight[permutation[i].second][j]
                    > this->instance.capacity[j]) {
                item_fits = false;
            }
        }

        if (item_fits) {
            knapsack[permutation[i].second] = true;

            for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                value[j] += this->instance.value[permutation[i].second][j];
                weight[j] += this->instance.weight[permutation[i].second][j];
            }
        }
    }

    return value;
}

}
