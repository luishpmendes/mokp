#include "solution/solution.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>

int main() {
    std::ifstream ifs;
    mokp::Instance instance;
    std::vector<double> key;
    std::mt19937 rng(2351389233);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (const std::string filename : {"instances/zlt_100_2.txt",
                                       "instances/zlt_100_3.txt",
                                       "instances/zlt_250_2.txt",
                                       "instances/zlt_500_2.txt"}) {
        std::cout << filename << std::endl;

        ifs.open(filename);

        assert(ifs.is_open());

        ifs >> instance;

        ifs.close();

        mokp::Solution empty_solution(instance);

        assert(empty_solution.is_feasible());

        for (unsigned j = 0; j < instance.num_dimensions; j++) {
            assert(fabs(empty_solution.weight[j]) 
                    <= std::numeric_limits<double>::epsilon());
            assert(fabs(empty_solution.value[j])
                    <= std::numeric_limits<double>::epsilon());
        }

        key = std::vector<double>(instance.num_items, 1.0);

        mokp::Solution full_solution(instance, key, 0);

        if (instance.num_items == 100 && instance.num_dimensions == 2) {
            assert(full_solution.value[0]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[0] <= 4266.0);
            assert(full_solution.value[1]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[1] <= 4037.0);
        } else if (instance.num_items == 100 && instance.num_dimensions == 3) {
            assert(full_solution.value[0]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[0] <= 4081.0);
            assert(full_solution.value[1]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[1] <= 4149.0);
            assert(full_solution.value[2]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[2] <= 3948.0);
        } else if (instance.num_items == 250 && instance.num_dimensions == 2) {
            assert(full_solution.value[0]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[0] <= 9893.0);
            assert(full_solution.value[1]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[1] <= 10103.0);
        } else if (instance.num_items == 500 && instance.num_dimensions == 2) {
            assert(full_solution.value[0]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[0] <= 20094.0);
            assert(full_solution.value[1]
                    > std::numeric_limits<double>::epsilon());
            assert(full_solution.value[1] <= 20490.0);
        }

        for (double & k : key) {
            k = distribution(rng);
        }

        mokp::Solution solution0(instance, key, 0);

        assert(solution0.is_feasible());

        if (instance.num_items == 100 && instance.num_dimensions == 2) {
            assert(solution0.value[0] >= 0.0);
            assert(solution0.value[0] <= 4266.0);
            assert(solution0.value[1] >= 0.0);
            assert(solution0.value[1] <= 4037.0);
        } else if (instance.num_items == 100 && instance.num_dimensions == 3) {
            assert(solution0.value[0] >= 0.0);
            assert(solution0.value[0] <= 4081.0);
            assert(solution0.value[1] >= 0.0);
            assert(solution0.value[1] <= 4149.0);
            assert(solution0.value[2] >= 0.0);
            assert(solution0.value[2] <= 3948.0);
        } else if (instance.num_items == 250 && instance.num_dimensions == 2) {
            assert(solution0.value[0] >= 0.0);
            assert(solution0.value[0] <= 9893.0);
            assert(solution0.value[1] >= 0.0);
            assert(solution0.value[1] <= 10103.0);
        } else if (instance.num_items == 500 && instance.num_dimensions == 2) {
            assert(solution0.value[0] >= 0.0);
            assert(solution0.value[0] <= 20094.0);
            assert(solution0.value[1] >= 0.0);
            assert(solution0.value[1] <= 20490.0);
        }

        mokp::Solution solution1(instance, key, 1);

        assert(solution1.is_feasible());

        if (instance.num_items == 100 && instance.num_dimensions == 2) {
            assert(solution1.value[0] >= 0.0);
            assert(solution1.value[0] <= 4266.0);
            assert(solution1.value[1] >= 0.0);
            assert(solution1.value[1] <= 4037.0);
        } else if (instance.num_items == 100 && instance.num_dimensions == 3) {
            assert(solution1.value[0] >= 0.0);
            assert(solution1.value[0] <= 4081.0);
            assert(solution1.value[1] >= 0.0);
            assert(solution1.value[1] <= 4149.0);
            assert(solution1.value[2] >= 0.0);
            assert(solution1.value[2] <= 3948.0);
        } else if (instance.num_items == 250 && instance.num_dimensions == 2) {
            assert(solution1.value[0] >= 0.0);
            assert(solution1.value[0] <= 9893.0);
            assert(solution1.value[1] >= 0.0);
            assert(solution1.value[1] <= 10103.0);
        } else if (instance.num_items == 500 && instance.num_dimensions == 2) {
            assert(solution1.value[0] >= 0.0);
            assert(solution1.value[0] <= 20094.0);
            assert(solution1.value[1] >= 0.0);
            assert(solution1.value[1] <= 20490.0);
        }
    }

    std::cout << std::endl << "Solution Test PASSED" << std::endl;

    return 0;
}
