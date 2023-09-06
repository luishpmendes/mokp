#include "solver/nsbrkga/decoder.hpp"
#include <algorithm>

namespace mokp {

Decoder::Decoder(const Instance & instance,
                 unsigned decoder_type,
                 unsigned num_threads)
    : instance(instance),
      decoder_type(decoder_type),
      greedy_knapsacks(num_threads,
             std::vector<bool>(instance.num_items, false)),
      values(num_threads,
            std::vector<double>(instance.num_dimensions, 0.0)),
      weights(num_threads,
            std::vector<double>(instance.num_dimensions, 0.0)),
      permutations(num_threads,
                   std::vector<std::pair<double, unsigned>>(
                        instance.num_items)) {}

std::vector<double> Decoder::decode(NSBRKGA::Chromosome & chromosome,
                                    bool /* not used */) {
#   ifdef _OPENMP
        std::vector<bool> & greedy_knapsack
                = this->greedy_knapsack[omp_get_thread_num()];
        std::vector<double> & value = this->values[omp_get_thread_num()];
        std::vector<double> & weight = this->weights[omp_get_thread_num()];
        std::vector<std::pair<double, unsigned>> & permutation =
                this->permutations[omp_get_thread_num()];
#   else
        std::vector<bool> & greedy_knapsack = this->greedy_knapsacks.front();
        std::vector<double> & value = this->values.front();
        std::vector<double> & weight = this->weights.front();
        std::vector<std::pair<double, unsigned>> & permutation = 
                this->permutations.front();
#   endif

    greedy_knapsack.assign(this->instance.num_items, false);
    value.assign(this->instance.num_dimensions, 0.0);
    weight.assign(this->instance.num_dimensions, 0.0);

    if (this->decoder_type == 0) {
        bool has_space = true;

        for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
            if (chromosome[i] >= 0.5) {
                bool item_fits = true;

                for (unsigned j = 0;
                    j < this->instance.num_dimensions && item_fits;
                    j++) {
                    if (weight[j] + this->instance.min_weight[j]
                            > this->instance.capacity[j]) {
                        has_space = false;
                        item_fits = false;
                    }

                    if (weight[j] + this->instance.greedy_weight[i][j]
                            > this->instance.capacity[j]) {
                        item_fits = false;
                    }
                }

                if (item_fits) {
                    greedy_knapsack[i] = true;

                    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                        value[j] += this->instance.greedy_value[i][j];
                        weight[j] += this->instance.greedy_weight[i][j];
                    }
                }
            }
        }

        for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
            if (!greedy_knapsack[i]) {
                bool item_fits = true;

                for (unsigned j = 0;
                    j < this->instance.num_dimensions && item_fits;
                    j++) {
                    if (weight[j] + this->instance.min_weight[j]
                            > this->instance.capacity[j]) {
                        has_space = false;
                        item_fits = false;
                    }

                    if (weight[j] + this->instance.greedy_weight[i][j]
                            > this->instance.capacity[j]) {
                        item_fits = false;
                    }
                }

                if (item_fits) {
                    greedy_knapsack[i] = true;

                    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                        value[j] += this->instance.greedy_value[i][j];
                        weight[j] += this->instance.greedy_weight[i][j];
                    }
                }
            }
        }
    } else { // decoder_type == 1
        bool has_space = true;

        for (unsigned i = 0; i < this->instance.num_items; i++) {
            permutation[i] = std::make_pair(chromosome[i], i);
        }

        std::sort(permutation.begin(), permutation.end());

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
                for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                    value[j] += this->instance.value[permutation[i].second][j];
                    weight[j] += this->instance.weight[permutation[i].second][j];
                }
            }
        }
    }

    return value;
}

}
