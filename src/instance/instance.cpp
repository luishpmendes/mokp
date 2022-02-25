#include "instance/instance.hpp"

namespace mokp {

Instance::Instance(unsigned num_knapsacks,
                   unsigned num_items,
                   const std::vector<double> & capacity,
                   const std::vector<std::vector<double>> & weight,
                   const std::vector<std::vector<double>> & value) :
    num_knapsacks(num_knapsacks),
    num_items(num_items),
    capacity(capacity),
    weight(weight),
    value(value) {}

Instance::Instance(const Instance & instance) = default;

Instance::Instance() = default;

Instance Instance::operator = (const Instance & instance) {
    this->num_knapsacks = instance.num_knapsacks;
    this->num_items = instance.num_items;
    this->capacity = instance.capacity;
    this->weight = instance.weight;
    this->value = instance.value;
    return *this;
}

bool Instance::is_valid() const {
    if (this->num_knapsacks == 0 || this->num_items == 0) {
        return false;
    }

    if (this->capacity.size() != this->num_knapsacks) {
        return false;
    }

    if (this->weight.size() != this->num_items) {
        return false;
    }

    if (this->value.size() != this->num_items) {
        return false;
    }

    for (unsigned i = 0; i < this->num_items; i++) {
        if (this->weight[i].size() != this->num_knapsacks) {
            return false;
        }

        if (this->value[i].size() != this->num_knapsacks) {
            return false;
        }
    }

    for (unsigned j = 0; j < this->num_knapsacks; j++) {
        double sum_weight = 0;

        for (unsigned i = 0; i < this->num_items; i++) {
            if (this->weight[i][j] <= 0.0) {
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
    is >> instance.num_knapsacks >> instance.num_items;

    instance.capacity = std::vector<double>(instance.num_knapsacks);
    instance.weight = std::vector<std::vector<double>>(instance.num_items, std::vector<double>(instance.num_knapsacks));
    instance.value = std::vector<std::vector<double>>(instance.num_items, std::vector<double>(instance.num_knapsacks));

    for (unsigned j = 0; j < instance.num_knapsacks; j++) {
        is >> instance.capacity[j];

        for (unsigned i = 0; i < instance.num_items; i++) {
            is >> instance.weight[i][j] >> instance.value[i][j];
        }
    }

    return is;
}

std::ostream & operator <<(std::ostream & os, Instance & instance) {
    os << instance.num_knapsacks << ' ' << instance.num_items << std::endl;

    for (unsigned j = 0; j < instance.num_knapsacks; j++) {
        os << instance.capacity[j] << std::endl;

        for (unsigned i = 0; i < instance.num_items; i++) {
            os << instance.weight[i][j] << ' '
               << instance.value[i][j] << std::endl;
        }
    }

    return os;
}

}
