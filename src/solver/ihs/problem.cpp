#include "solver/nsga2/problem.hpp"
#include <algorithm>

namespace mokp {

Problem::Problem(const Instance & instance, unsigned decoder_type) :
    instance(instance), decoder_type(decoder_type) {}

Problem::Problem() {}

pagmo::vector_double Problem::fitness(const pagmo::vector_double & dv) const {
    pagmo::vector_double value(this->instance.num_dimensions, 0.0),
                         weight(this->instance.num_dimensions, 0.0);

    if (this->decoder_type == 0) {
        bool has_space = true;
        std::vector<bool> greedy_knapsack(this->instance.num_items, false);

        for (unsigned i = 0; i < this->instance.num_items && has_space; i++) {
            if (dv[i] >= 0.5) {
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
                        value[j] -= this->instance.greedy_value[i][j];
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
                        value[j] -= this->instance.greedy_value[i][j];
                        weight[j] += this->instance.greedy_weight[i][j];
                    }
                }
            }
        }
    } else { // this->decoder_type == 1
        std::vector<std::pair<double,
                              std::pair<std::vector<double>,
                                        std::vector<double>>>> permutation(
                                                this->instance.num_items);
        bool has_space = true;

        for (unsigned i = 0; i < this->instance.num_items; i++) {
            permutation[i] = std::make_pair(dv[i],
                                            std::make_pair(this->instance.weight[i],
                                                           this->instance.value[i]));
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

                if (weight[j] + permutation[i].second.first[j]
                        > this->instance.capacity[j]) {
                    item_fits = false;
                }
            }

            if (item_fits) {
                for (unsigned j = 0; j < this->instance.num_dimensions; j++) {
                    value[j] -= permutation[i].second.second[j];
                    weight[j] += permutation[i].second.first[j];
                }
            }
        }
    }

    return value;
}

std::pair<pagmo::vector_double, pagmo::vector_double> Problem::get_bounds()
    const {
    return std::make_pair(
            pagmo::vector_double(this->instance.num_items, 0.0),
            pagmo::vector_double(this->instance.num_items, 1.0));
}

pagmo::vector_double::size_type Problem::get_nobj() const {
    return this->instance.num_dimensions;
}

}
