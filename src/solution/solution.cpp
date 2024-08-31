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

    bool at_least_as_good = true, better = false;

    for (std::size_t i = 0; i < valueA.size() && at_least_as_good; i++) {
        if (valueA[i] < valueB[i] - std::numeric_limits<double>::epsilon()) {
            at_least_as_good =  false;
        } else if (valueA[i] > valueB[i] + std::numeric_limits<double>::epsilon()) {
            better = true;
        }
    }

    return at_least_as_good && better;
}

Solution::Solution(const Instance & instance,
                   const std::vector<bool> & knapsack) :
        instance(instance),
        knapsack(knapsack),
        greedy_knapsack(knapsack.size()),
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

        this->greedy_knapsack[i]
                = this->knapsack[this->instance.greedy_permutation[i]];
    }
}

Solution::Solution(const Instance & instance,
                   const std::vector<double> & key,
                   unsigned decoder_type) :
        instance(instance),
        knapsack(instance.num_items, false),
        greedy_knapsack(instance.num_items, false),
        value(instance.num_dimensions, 0.0),
        weight(instance.num_dimensions, 0.0) {
    if (decoder_type == 0) {
        bool has_space = true;

        for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
            if (key[i] >= 0.5) {
                bool item_fits = true;

                for (unsigned j = 0;
                    j < this->instance.num_dimensions && item_fits;
                    j++) {
                    if (this->weight[j] + this->instance.min_weight[j]
                            > this->instance.capacity[j]) {
                        has_space = false;
                        item_fits = false;
                    }

                    if (this->weight[j] + this->instance.greedy_weight[i][j]
                            > this->instance.capacity[j]) {
                        item_fits = false;
                    }
                }

                if (item_fits) {
                    this->greedy_knapsack[i] = true;

                    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                        this->value[j] += this->instance.greedy_value[i][j];
                        this->weight[j] += this->instance.greedy_weight[i][j];
                    }
                }
            }
        }

        for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
            if (!this->greedy_knapsack[i]) {
                bool item_fits = true;

                for (unsigned j = 0;
                    j < this->instance.num_dimensions && item_fits;
                    j++) {
                    if (this->weight[j] + this->instance.min_weight[j]
                            > this->instance.capacity[j]) {
                        has_space = false;
                        item_fits = false;
                    }

                    if (this->weight[j] + this->instance.greedy_weight[i][j]
                            > this->instance.capacity[j]) {
                        item_fits = false;
                    }
                }

                if (item_fits) {
                    this->greedy_knapsack[i] = true;

                    for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                        this->value[j] += this->instance.greedy_value[i][j];
                        this->weight[j] += this->instance.greedy_weight[i][j];
                    }
                }
            }
        }

        for (unsigned i = 0; i < this->instance.num_items; i++) {
            this->knapsack[this->instance.greedy_permutation[i]] =
                    this->greedy_knapsack[i];
        }
    } else { // decoder_type == 1
        std::vector<std::pair<double, unsigned>> permutation(this->instance.num_items);
        bool has_space = true;

        for (unsigned i = 0; i < this->instance.num_items; i++) {
            permutation[i] = std::make_pair(key[i], i);
        }

        std::sort(permutation.begin(), permutation.end());

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

        for (unsigned i = 0; i < this->instance.num_items; i++) {
            this->greedy_knapsack[i]
                    = this->knapsack[this->instance.greedy_permutation[i]];
        }
    }
}

Solution::Solution(const Instance & instance) :
        instance(instance),
        knapsack(instance.num_items, false),
        greedy_knapsack(instance.num_items, false),
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

        if (fabs(sum_weight - this->weight[j])
                > std::numeric_limits<double>::epsilon()) {
            return false;
        }

        if (fabs(sum_value - this->value[j])
                > std::numeric_limits<double>::epsilon()) {
            return false;
        }
    }

    for (unsigned i = 0; i < this->instance.num_items; i++) {
        if (this->greedy_knapsack[i] 
                != this->knapsack[this->instance.greedy_permutation[i]]) {
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

        solution.greedy_knapsack[i]
                = solution.knapsack[solution.instance.greedy_permutation[i]];
    }

    return is;
}

std::ostream & operator <<(std::ostream & os, const Solution & solution) {
    for (unsigned i = 0; i < solution.instance.num_items - 1; i++) {
        os << solution.knapsack[i] << ' ';
    }

    os << solution.knapsack.back() << std::endl;

    return os;
}

}
