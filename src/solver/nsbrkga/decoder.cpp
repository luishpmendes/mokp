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
      qs(num_threads,
         std::vector<std::pair<double, unsigned>>(instance.num_items)) {}

std::vector<double> Decoder::decode(BRKGA::Chromosome & chromosome,
                                    bool rewrite) {
#   ifdef _OPENMP
        std::vector<bool> & knapsack = this->knapsack[omp_get_thread_num()];
        std::vector<double> & value = this->values[omp_get_thread_num()];
        std::vector<double> & weight = this->weights[omp_get_thread_num()];
        std::vector<std::pair<double, unsigned>> & q 
                = this->qs[omp_get_thread_num()];
#   else
        std::vector<bool> & knapsack = this->knapsacks.front();
        std::vector<double> & value = this->values.front();
        std::vector<double> & weight = this->weights.front();
        std::vector<std::pair<double, unsigned>> & q = this->qs.front();
#   endif

    // Compute knapsack
    knapsack = std::vector<bool>(this->instance.num_items, false);

    for (unsigned i = 0; i < this->instance.num_items; i++) {
        if (chromosome[i] >= 0.5) {
            knapsack[i] = true;
        }
    }

    // Compute the value and weight
    value = std::vector<double>(this->instance.num_dimensions, 0.0);
    weight = std::vector<double>(this->instance.num_dimensions, 0.0);

    for (unsigned i = 0; i < this->instance.num_items; i++) {
        if (knapsack[i]) {
            for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                value[j] += this->instance.value[i][j];
                weight[j] += this->instance.weight[i][j];
            }
        }
    }

    // Repair
    for (unsigned i = 0; i < this->instance.num_items; i++) {
        q[i].second = i;
        q[i].first = this->instance.value[i].front()
                        / this->instance.weight[i].front();

        for (unsigned j = 1; j < this->instance.num_dimensions; j++) {
            if (q[i].first < this->instance.value[i][j]
                            / this->instance.weight[i][j]) {
                q[i].first = this->instance.value[i][j]
                            / this->instance.weight[i][j];;
            }
        }
    }

    std::sort(q.begin(), q.end());

    unsigned i = 0;
    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
        while (weight[j] > this->instance.capacity[j]) {
            if (knapsack[q[i].second]) {
                knapsack[q[i].second] = false;

                for (unsigned k = 0; k < this->instance.num_dimensions; k++) {
                    value[k] -= this->instance.value[q[i].second][k];
                    weight[k] -= this->instance.weight[q[i].second][k];
                }

                if (rewrite) {
                    chromosome[q[i].second] = 1.0 - chromosome[q[i].second];
                }
            }

            i++;
        }
    }

    return value;
}

}
