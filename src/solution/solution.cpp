#include "solution/solution.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace mokp {

bool Solution::dominates(const std::vector<double> & valueA,
                         const std::vector<double> & valueB) {
    if (valueA.size() != valueB.size()) {
        return false;
    }

    // Checks if valueA is at least as good as valueB
    for (unsigned i = 0; i < valueA.size(); i++) {
        if (valueA[i] < valueB[i] - std::numeric_limits<double>::epsilon()) {
            return false;
        }
    }

    // Checks if valueA is better than valueB
    for (unsigned i = 0; i < valueA.size(); i++) {
        if (valueA[i] > valueB[i] + std::numeric_limits<double>::epsilon()) {
            return true;
        }
    }

    return false;
}

Solution::Solution(const Instance & instance,
                   const std::vector<bool> & knapsack) :
        instance(instance),
        knapsack(knapsack),
        value(instance.num_dimensions, 0.0),
        weight(instance.num_dimensions, 0.0) {
    // Compute the value and weight
    for (unsigned i = 0; i < this->instance.num_items; i++) {
        if (this->knapsack[i]) {
            for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                this->value[j] += this->instance.value[i][j];
                this->weight[j] += this->instance.weight[i][j];
            }
        }
    }
}

Solution::Solution(const Instance & instance,
                   const std::vector<double> & key,
                   unsigned decoder_type) :
        instance(instance),
        knapsack(instance.num_items, false),
        value(instance.num_dimensions, 0.0),
        weight(instance.num_dimensions, 0.0) {
    if (decoder_type == 0) {
        bool has_space = true;

        for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
            if (key[this->instance.greedy_permutation[i]] >= 0.5) {
                bool item_fits = true;

                for (unsigned j = 0;
                    j < this->instance.num_dimensions && item_fits;
                    j++) {
                    if (this->weight[j] + this->instance.min_weight[j]
                            > this->instance.capacity[j]) {
                        has_space = false;
                        item_fits = false;
                    }

                    if (this->weight[j] + 
                            this->instance.weight[this->instance.greedy_permutation[i]][j]
                            > this->instance.capacity[j]) {
                        item_fits = false;
                    }
                }

                if (item_fits) {
                    this->knapsack[this->instance.greedy_permutation[i]] = true;

                    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                        this->value[j] +=
                            this->instance.value[this->instance.greedy_permutation[i]][j];
                        this->weight[j] +=
                            this->instance.weight[this->instance.greedy_permutation[i]][j];
                    }
                }
            }
        }

        for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
            if (!this->knapsack[this->instance.greedy_permutation[i]]) {
                bool item_fits = true;

                for (unsigned j = 0;
                    j < this->instance.num_dimensions && item_fits;
                    j++) {
                    if (this->weight[j] + this->instance.min_weight[j]
                            > this->instance.capacity[j]) {
                        has_space = false;
                        item_fits = false;
                    }

                    if (this->weight[j] + 
                            this->instance.weight[this->instance.greedy_permutation[i]][j]
                            > this->instance.capacity[j]) {
                        item_fits = false;
                    }
                }

                if (item_fits) {
                    this->knapsack[this->instance.greedy_permutation[i]] = true;

                    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                        this->value[j] +=
                            this->instance.value[this->instance.greedy_permutation[i]][j];
                        this->weight[j] +=
                            this->instance.weight[this->instance.greedy_permutation[i]][j];
                    }
                }
            }
        }
    } else { // decoder_type == 1
        std::vector<std::pair<double, unsigned>> permutation(this->instance.num_items);

        for (unsigned i = 0; i < this->instance.num_items; i++) {
            permutation[i] = std::make_pair(key[i], i);
        }

        std::sort(permutation.begin(), permutation.end());

        bool has_space = true;

        for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
            bool item_fits = true;

            for (unsigned j = 0;
                j < this->instance.num_dimensions && item_fits;
                j++) {
                if (this->weight[j] + this->instance.min_weight[j]
                        > this->instance.capacity[j]) {
                    has_space = false;
                    item_fits = false;
                }

                if (this->weight[j] + this->instance.weight[permutation[i].second][j]
                        > this->instance.capacity[j]) {
                    item_fits = false;
                }
            }

            if (item_fits) {
                this->knapsack[permutation[i].second] = true;

                for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                    this->value[j] += this->instance.value[permutation[i].second][j];
                    this->weight[j] += this->instance.weight[permutation[i].second][j];
                }
            }
        }
    }
}

Solution::Solution(const Instance & instance) :
        instance(instance),
        knapsack(instance.num_items, false),
        value(instance.num_dimensions, 0.0),
        weight(instance.num_dimensions, 0.0) {}

bool Solution::is_feasible() const {
    if (!this->instance.is_valid()) {
        return false;
    }

    if (this->knapsack.size() != this->instance.num_items) {
        return false;
    }

    if (this->value.size() != this->instance.num_dimensions) {
        return false;
    }

    if (this->weight.size() != this->instance.num_dimensions) {
        return false;
    }

    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
        if (this->weight[j] < 0.0) {
            return false;
        }

        if (this->weight[j] > this->instance.capacity[j]) {
            return false;
        }

        if (this->value[j] < 0.0) {
            return false;
        }

        double sum_weight = 0,
               sum_value = 0;

        for (unsigned i = 0; i < this->instance.num_items; i++) {
            if (this->knapsack[i]) {
                sum_weight += this->instance.weight[i][j];
                sum_value += this->instance.value[i][j];
            }
        }

        if (sum_weight
                > this->weight[j] + std::numeric_limits<double>::epsilon()) {
            return false;
        }

        if (fabs(sum_value - this->value[j])
                > std::numeric_limits<double>::epsilon()) {
            return false;
        }
    }

    return true;
}

bool Solution::dominates(const Solution & solution) const {
    return Solution::dominates(this->value, solution.value);
}

std::istream & operator >>(std::istream & is, Solution & solution) {
    unsigned aux = 0;

    for (unsigned i = 0; i < solution.instance.num_items; i++) {
        is >> aux;

        if (aux) {
            solution.knapsack[i] = true;
        }
    }

    return is;
}

std::ostream & operator <<(std::ostream & os, Solution & solution) {
    for (unsigned i = 0; i < solution.instance.num_items - 1; i++) {
        os << solution.knapsack[i] << ' ';
    }

    os << solution.knapsack.back() << std::endl;

    return os;
}

}
