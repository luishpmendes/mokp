#include "instance/instance.hpp"
#include <algorithm>

namespace mokp {

void Instance::init() {
    this->senses.resize(this->num_dimensions, NSBRKGA::Sense::MAXIMIZE);
    this->senses.assign(this->num_dimensions, NSBRKGA::Sense::MAXIMIZE);

    // Compute the minimum weight
    for (unsigned j = 0; j < this->num_dimensions; j++) {
        this->min_weight[j] = this->weight[0][j];

        for (unsigned i = 1; i < this->num_items; i++) {
            if (this->min_weight[j] > this->weight[i][j]) {
                this->min_weight[j] = this->weight[i][j];
            }
        }
    }

    // Compute the greedy permutation
    std::vector<std::pair<double, unsigned>> permutation (this->num_items);

    for (unsigned i = 0; i < this->num_items; i++) {
        permutation[i] = std::make_pair(this->value[i][0] / this->weight[i][0],
                                        i);

        for (unsigned j = 1; j < this->num_dimensions; j++) {
            if (permutation[i].first < this->value[i][j] / this->weight[i][j]) {
                permutation[i].first = this->value[i][j] / this->weight[i][j];
            }
        }
    }

    std::sort(permutation.begin(), permutation.end());

    for (unsigned i = 0; i < this->num_items; i++) {
        this->greedy_permutation[i] = permutation[i].second;
        this->greedy_weight[i] = this->weight[this->greedy_permutation[i]];
        this->greedy_value[i] = this->value[this->greedy_permutation[i]];
    }

    // Compute the primal bound
    this->primal_bound.resize(this->num_dimensions, 0.0);
    this->primal_bound.assign(this->num_dimensions, 0.0);

    for (unsigned j = 0; j < this->num_dimensions; j++) {
        this->primal_bound[j] = this->value[0][j];

        for (unsigned i = 1; i < this->num_items; i++) {
            if (this->primal_bound[j] > this->value[i][j]) {
                this->primal_bound[j] = this->value[i][j];
            }
        }
    }
}

Instance::Instance(const std::vector<double> & capacity,
                   const std::vector<std::vector<double>> & weight,
                   const std::vector<std::vector<double>> & value) :
    capacity(capacity),
    weight(weight),
    value(value),
    num_dimensions(capacity.size()),
    num_items(weight.size()),
    min_weight(capacity.size()),
    greedy_permutation(num_items),
    greedy_weight(num_items),
    greedy_value(num_items) {
    this->init();
}

Instance::Instance(const Instance & instance) = default;

Instance::Instance() = default;

Instance Instance::operator = (const Instance & instance) {
    this->capacity = instance.capacity;
    this->weight = instance.weight;
    this->value = instance.value;
    this->num_dimensions = instance.num_dimensions;
    this->num_items = instance.num_items;
    this->senses = instance.senses;
    this->min_weight = instance.min_weight;
    this->greedy_permutation = instance.greedy_permutation;
    this->greedy_weight = instance.greedy_weight;
    this->greedy_value = instance.greedy_value;
    return *this;
}

bool Instance::is_valid() const {
    if (this->num_dimensions == 0) {
        return false;
    }

    if (this->num_items == 0) {
        return false;
    }

    if (this->capacity.size() != this->num_dimensions) {
        return false;
    }

    if (this->weight.size() != this->num_items) {
        return false;
    }

    if (this->value.size() != this->num_items) {
        return false;
    }

    if (this->greedy_weight.size() != this->num_items) {
        return false;
    }

    if (this->greedy_value.size() != this->num_items) {
        return false;
    }

    for (unsigned i = 0; i < this->num_items; i++) {
        if (this->weight[i].size() != this->num_dimensions) {
            return false;
        }

        if (this->value[i].size() != this->num_dimensions) {
            return false;
        }

        if (this->greedy_permutation[i] >= this->num_items) {
            return false;
        }

        if (this->greedy_weight[i].size() != this->num_dimensions) {
            return false;
        }

        if (this->greedy_value[i].size() != this->num_dimensions) {
            return false;
        }
    }

    for (unsigned j = 0; j < this->num_dimensions; j++) {
        double sum_weight = 0,
               sum_greedy_weight = 0;

        if (this->min_weight[j] <= 0.0) {
            return false;
        }

        for (unsigned i = 0; i < this->num_items; i++) {
            if (this->weight[i][j] < min_weight[j]) {
                return false;
            }

            if (this->value[i][j] <= 0.0) {
                return false;
            }

            if (this->greedy_weight[i][j] < min_weight[j]) {
                return false;
            }

            if (this->greedy_value[i][j] <= 0.0) {
                return false;
            }

            sum_weight += this->weight[i][j];
            sum_greedy_weight += this->greedy_weight[i][j];
        }

        if (sum_weight < this->capacity[j]) {
            return false;
        }

        if (sum_greedy_weight < this->capacity[j]) {
            return false;
        }
    }

    return true;
}

std::istream & operator >>(std::istream & is, Instance & instance) {
    is >> instance.num_dimensions >> instance.num_items;

    instance.capacity.resize(instance.num_dimensions);
    instance.weight.resize(instance.num_items,
                           std::vector<double>(instance.num_dimensions));
    instance.weight.assign(instance.num_items,
                           std::vector<double>(instance.num_dimensions));
    instance.value.resize(instance.num_items,
                          std::vector<double>(instance.num_dimensions));
    instance.value.assign(instance.num_items,
                          std::vector<double>(instance.num_dimensions));
    instance.min_weight.resize(instance.num_dimensions);
    instance.greedy_permutation.resize(instance.num_items);
    instance.greedy_weight.resize(instance.num_items,
                                  std::vector<double>(instance.num_dimensions));
    instance.greedy_weight.assign(instance.num_items,
                                  std::vector<double>(instance.num_dimensions));
    instance.greedy_value.resize(instance.num_items,
                                 std::vector<double>(instance.num_dimensions));
    instance.greedy_value.assign(instance.num_items,
                                 std::vector<double>(instance.num_dimensions));

    for (unsigned j = 0; j < instance.num_dimensions; j++) {
        is >> instance.capacity[j];

        for (unsigned i = 0; i < instance.num_items; i++) {
            is >> instance.weight[i][j] >> instance.value[i][j];
        }
    }

    instance.init();

    return is;
}

std::ostream & operator <<(std::ostream & os, const Instance & instance) {
    os << instance.num_dimensions << ' ' << instance.num_items << std::endl;

    for (unsigned j = 0; j < instance.num_dimensions; j++) {
        os << instance.capacity[j] << std::endl;

        for (unsigned i = 0; i < instance.num_items; i++) {
            os << instance.weight[i][j] << ' '
               << instance.value[i][j] << std::endl;
        }
    }

    return os;
}

}
