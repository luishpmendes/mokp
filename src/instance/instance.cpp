#include "instance/instance.hpp"

namespace mokp {

void Instance::init() {
    // Compute the minimum weight
    this->min_weight = std::vector<double>(this->num_dimensions, 0.0);

    for (unsigned j = 0; j < this->num_dimensions; j++) {
        this->min_weight[j] = this->weight[0][j];

        for (unsigned i = 1; i < this->num_items; i++) {
            if (this->min_weight[j] > this->weight[i][j]) {
                this->min_weight[j] = this->weight[i][j];
            }
        }
    }
}

Instance::Instance(unsigned num_dimensions,
                   unsigned num_items,
                   const std::vector<double> & capacity,
                   const std::vector<std::vector<double>> & weight,
                   const std::vector<std::vector<double>> & value) :
    num_dimensions(num_dimensions),
    num_items(num_items),
    capacity(capacity),
    weight(weight),
    value(value) {
    this->init();
}

Instance::Instance(const Instance & instance) = default;

Instance::Instance() = default;

Instance Instance::operator = (const Instance & instance) {
    this->num_dimensions = instance.num_dimensions;
    this->num_items = instance.num_items;
    this->capacity = instance.capacity;
    this->weight = instance.weight;
    this->value = instance.value;
    this->min_weight = instance.min_weight;
    return *this;
}

bool Instance::is_valid() const {
    if (this->num_dimensions == 0 || this->num_items == 0) {
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

    for (unsigned i = 0; i < this->num_items; i++) {
        if (this->weight[i].size() != this->num_dimensions) {
            return false;
        }

        if (this->value[i].size() != this->num_dimensions) {
            return false;
        }
    }

    for (unsigned j = 0; j < this->num_dimensions; j++) {
        double sum_weight = 0;

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

            sum_weight += this->weight[i][j];
        }

        if (sum_weight < this->capacity[j]) {
            return false;
        }
    }

    return true;
}

std::istream & operator >>(std::istream & is, Instance & instance) {
    is >> instance.num_dimensions >> instance.num_items;

    instance.capacity = std::vector<double>(instance.num_dimensions);
    instance.weight = std::vector<std::vector<double>>(instance.num_items,
                                                       std::vector<double>(instance.num_dimensions));
    instance.value = std::vector<std::vector<double>>(instance.num_items,
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

std::ostream & operator <<(std::ostream & os, Instance & instance) {
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
