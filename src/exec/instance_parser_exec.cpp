#include "instance/instance.hpp"
#include <cstdio>
#include <iostream>
#include <vector>

int main () {
    unsigned num_dimensions, num_items, aux;
    std::vector<double> capacity;
    std::vector<std::vector<double>> weight, profit;

    scanf("knapsack problem specification (%u knapsacks, %u items)\n",
          &num_dimensions, &num_items);

    capacity.resize(num_dimensions);
    weight.resize(num_items, std::vector<double>(num_dimensions));
    profit.resize(num_items, std::vector<double>(num_dimensions));
    
    for (unsigned j = 0; j < num_dimensions; j++) {
        scanf("=\nknapsack %u:\n capacity: %lf\n", &aux, &capacity[j]);

        for (unsigned i = 0; i < num_items; i++) {
            scanf(" item %*u:\n  weight: %lf\n  profit: %lf\n", &weight[i][j],
                                                                &profit[i][j]);
        }
    }

    mokp::Instance instance(capacity, weight, profit);

    std::cout << instance;

    return 0;
}
