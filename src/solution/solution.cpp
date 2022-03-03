#include "solution/solution.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace mokp {

bool Solution::dominates(const std::vector<double> & valueA,
                         const std::vector<double> & valueB) {
    if(valueA.size() != valueB.size()) {
        return false;
    }

    // Checks if valueA is at least as good as valueB
    for(unsigned i = 0; i < valueA.size(); i++) {
        if(valueA[i] < valueB[i] - std::numeric_limits<double>::epsilon()) {
            return false;
        }
    }

    // Checks if valueA is better than valueB
    for(unsigned i = 0; i < valueA.size(); i++) {
        if(valueA[i] > valueB[i] + std::numeric_limits<double>::epsilon()) {
            return true;
        }
    }

    return false;
}

void Solution::compute_knapsack(const std::vector<double> & key) {
    this->knapsack = std::vector<bool>(this->instance.num_items, false);

    for (unsigned i = 0; i < this->instance.num_items; i++) {
        if (key[i] >= 0.5) {
            this->knapsack[i] = true;
        }
    }
}

void Solution::init() {
    // Compute the value and weight
    this->value = std::vector<double>(this->instance.num_knapsacks, 0.0);
    this->weight = std::vector<double>(this->instance.num_knapsacks, 0.0);

    for(unsigned i = 0; i < this->instance.num_items; i++) {
        if(this->knapsack[i]) {
            for(unsigned j = 0; j < this->instance.num_knapsacks; j++) {
                this->value[j] += this->instance.value[i][j];
                this->weight[j] += this->instance.weight[i][j];
            }
        }
    }
}

Solution::Solution(const Instance & instance,
                   const std::vector<bool> & knapsack) :
        instance(instance),
        knapsack(knapsack) {
    this->init();
    this->repair();
}

Solution::Solution(const Instance & instance,
                   const std::vector<double> & key) :
        instance(instance) {
    this->compute_knapsack(key);
    this->init();
    this->repair();
}

Solution::Solution(const Instance & instance) : instance(instance) {
    this->knapsack = std::vector<bool>(this->instance.num_items, false);
    this->init();
}

void Solution::repair() {
    std::vector<std::pair<double, unsigned>> q(this->instance.num_items);

    for(unsigned i = 0; i < this->instance.num_items; i++) {
        q[i].second = i;
        q[i].first = this->instance.value[i].front()/this->instance.weight[i].front();

        for(unsigned j = 1; j < this->instance.num_knapsacks; j++) {
            if(q[i].first < this->instance.value[i][j]/this->instance.weight[i][j]) {
                q[i].first = this->instance.value[i][j]/this->instance.weight[i][j];;
            }
        }
    }

    std::sort(q.begin(), q.end());

    unsigned i = 0;
    for(unsigned j = 0; j < this->instance.num_knapsacks; j++) {
        while(this->weight[j] > this->instance.capacity[j]) {
            if(this->knapsack[q[i].second]) {
                this->knapsack[q[i].second] = false;

                for (unsigned k = 0; k < this->instance.num_knapsacks; k++) {
                    this->value[k] -= this->instance.value[q[i].second][k];
                    this->weight[k] -= this->instance.weight[q[i].second][k];
                }
            }

            i++;
        }
    }
}

bool Solution::is_feasible() const {
    if(!this->instance.is_valid()) {
        return false;
    }

    if (this->knapsack.size() != this->instance.num_items) {
        return false;
    }

    if (this->value.size() != this->instance.num_knapsacks) {
        return false;
    }

    if (this->weight.size() != this->instance.num_knapsacks) {
        return false;
    }

    for(unsigned j = 0; j < this->instance.num_knapsacks; j++) {
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

        for(unsigned i = 0; i < this->instance.num_items; i++) {
            if (this->knapsack[i]) {
                sum_weight += this->instance.weight[i][j];
                sum_value += this->instance.value[i][j];
            }
        }

        if(sum_weight > this->weight[j] + std::numeric_limits<double>::epsilon()) {
            return false;
        }

        if(fabs(sum_value - this->value[j]) > std::numeric_limits<double>::epsilon()) {
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

    for(unsigned i = 0; i < solution.instance.num_items; i++) {
        is >> aux;

        if (aux) {
            solution.knapsack[i] = true;
        }
    }

    return is;
}

std::ostream & operator <<(std::ostream & os, Solution & solution) {
    for(unsigned i = 0; i < solution.instance.num_items - 1; i++) {
        os << solution.knapsack[i] << ' ';
    }

    os << solution.knapsack.back() << std::endl;

    return os;
}

}
