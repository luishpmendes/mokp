#include "solver/moead/moead_solver.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>

int main() {
    std::ifstream ifs;
    mokp::Instance instance;
    mokp::MOEAD_Solver solver;

    for (const std::string filename : {"instances/zlt_100_2.txt",
                                       "instances/zlt_100_3.txt",
                                       "instances/zlt_250_2.txt",
                                       "instances/zlt_500_2.txt"}) {
        std::cout << filename << std::endl;

        ifs.open(filename);

        assert(ifs.is_open());

        ifs >> instance;

        ifs.close();

        solver = mokp::MOEAD_Solver(instance);

        solver.set_seed(2351389233);
        solver.time_limit = 5.0;
        solver.iterations_limit = 100;
        solver.max_num_solutions = 128;
        solver.population_size = 28;
        solver.max_num_snapshots = 16;

        assert((solver.seed = 2351389233));
        assert(fabs(solver.time_limit - 5.0) <
            std::numeric_limits<double>::epsilon());
        assert(solver.iterations_limit == 100);
        assert(solver.max_num_solutions == 128);
        assert(solver.population_size == 28);
        assert(solver.max_num_snapshots == 16);
        assert(solver.weight_generation == "random");
        assert(solver.decomposition == "tchebycheff");
        assert(solver.neighbours == 20);
        assert(fabs(solver.cr - 1.0) < std::numeric_limits<double>::epsilon());
        assert(fabs(solver.f - 0.5) < std::numeric_limits<double>::epsilon());
        assert(fabs(solver.eta_m - 20.0) < std::numeric_limits<double>::epsilon());
        assert(fabs(solver.realb - 0.9) < std::numeric_limits<double>::epsilon());
        assert(solver.limit == 2);
        assert(solver.preserve_diversity);

        solver.solve();

        assert(solver.solving_time > 0);

        assert(solver.num_iterations > 0);
        assert(solver.num_iterations <= solver.iterations_limit);

        assert(solver.best_solutions.size() > 0);
        assert(solver.best_solutions.size() <= solver.max_num_solutions);

        assert(solver.num_snapshots == solver.max_num_snapshots);

        assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
        assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
        assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);
        assert(solver.populations_snapshots.size() == solver.num_snapshots);

        for (const auto & s1 : solver.best_solutions) {
            assert(s1.is_feasible());

            if (instance.num_items == 100 &&
                instance.num_dimensions == 2) {
                assert(s1.value[0] >= 0.0);
                assert(s1.value[0] <= 4266.0);
                assert(s1.value[1] >= 0.0);
                assert(s1.value[1] <= 4037.0);
            } else if (instance.num_items == 100 &&
                       instance.num_dimensions == 3) {
                assert(s1.value[0] >= 0.0);
                assert(s1.value[0] <= 4081.0);
                assert(s1.value[1] >= 0.0);
                assert(s1.value[1] <= 4149.0);
                assert(s1.value[2] >= 0.0);
                assert(s1.value[2] <= 3948.0);
            } else if (instance.num_items == 250 &&
                       instance.num_dimensions == 2) {
                assert(s1.value[0] >= 0.0);
                assert(s1.value[0] <= 9893.0);
                assert(s1.value[1] >= 0.0);
                assert(s1.value[1] <= 10103.0);
            } else if (instance.num_items == 500 &&
                       instance.num_dimensions == 2) {
                assert(s1.value[0] >= 0.0);
                assert(s1.value[0] <= 20094.0);
                assert(s1.value[1] >= 0.0);
                assert(s1.value[1] <= 20490.0);
            }

            for (const auto & s2 : solver.best_solutions) {
                assert(!s1.dominates(s2));
                assert(!s2.dominates(s1));
            }
        }

        for (const auto & snapshot : solver.best_solutions_snapshots) {
            assert(std::get<0>(snapshot) >= 0);
            assert(std::get<0>(snapshot) <= solver.num_iterations);
            assert(std::get<1>(snapshot) >= 0.0);
            assert(std::get<1>(snapshot) <= solver.solving_time);
            assert(std::get<2>(snapshot).size() > 0);
            assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

            for (const auto & s : std::get<2>(snapshot)) {
                assert(s.size() == instance.num_dimensions);

                if (instance.num_items == 100 &&
                    instance.num_dimensions == 2) {
                    assert(s[0] >= 0.0);
                    assert(s[0] <= 4266.0);
                    assert(s[1] >= 0.0);
                    assert(s[1] <= 4037.0);
                } else if (instance.num_items == 100 &&
                           instance.num_dimensions == 3) {
                    assert(s[0] >= 0.0);
                    assert(s[0] <= 4081.0);
                    assert(s[1] >= 0.0);
                    assert(s[1] <= 4149.0);
                    assert(s[2] >= 0.0);
                    assert(s[2] <= 3948.0);
                } else if (instance.num_items == 250 &&
                           instance.num_dimensions == 2) {
                    assert(s[0] >= 0.0);
                    assert(s[0] <= 9893.0);
                    assert(s[1] >= 0.0);
                    assert(s[1] <= 10103.0);
                } else if (instance.num_items == 500 &&
                           instance.num_dimensions == 2) {
                    assert(s[0] >= 0.0);
                    assert(s[0] <= 20094.0);
                    assert(s[1] >= 0.0);
                    assert(s[1] <= 20490.0);
                }
            }
        }

        for (const auto & snapshot : solver.num_non_dominated_snapshots) {
            assert(std::get<0>(snapshot) >= 0);
            assert(std::get<0>(snapshot) <= solver.num_iterations);
            assert(std::get<1>(snapshot) >= 0.0);
            assert(std::get<1>(snapshot) <= solver.solving_time);
            assert(std::get<2>(snapshot).size() > 0);
            assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

            for (const unsigned & num_non_dominated : std::get<2>(snapshot)) {
                assert(num_non_dominated > 0);
                assert(num_non_dominated <= solver.population_size);
            }
        }

        for (const auto & snapshot : solver.num_fronts_snapshots) {
            assert(std::get<0>(snapshot) >= 0);
            assert(std::get<0>(snapshot) <= solver.num_iterations);
            assert(std::get<1>(snapshot) >= 0.0);
            assert(std::get<1>(snapshot) <= solver.solving_time);
            assert(std::get<2>(snapshot).size() > 0);
            assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

            for (const unsigned & num_fronts : std::get<2>(snapshot)) {
                assert(num_fronts > 0);
                assert(num_fronts < solver.population_size);
            }
        }

        for (const auto & snapshot : solver.populations_snapshots) {
            assert(std::get<0>(snapshot) >= 0);
            assert(std::get<0>(snapshot) <= solver.num_iterations);
            assert(std::get<1>(snapshot) >= 0.0);
            assert(std::get<1>(snapshot) <= solver.solving_time);
            assert(std::get<2>(snapshot).size() > 0);
            assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

            for (const auto & population : std::get<2>(snapshot)) {
                assert(population.size() == solver.population_size);

                for (const auto & s : population) {
                    assert(s.size() == instance.num_dimensions);

                    if (instance.num_items == 100 &&
                        instance.num_dimensions == 2) {
                        assert(s[0] >= 0.0);
                        assert(s[0] <= 4266.0);
                        assert(s[1] >= 0.0);
                        assert(s[1] <= 4037.0);
                    } else if (instance.num_items == 100 &&
                               instance.num_dimensions == 3) {
                        assert(s[0] >= 0.0);
                        assert(s[0] <= 4081.0);
                        assert(s[1] >= 0.0);
                        assert(s[1] <= 4149.0);
                        assert(s[2] >= 0.0);
                        assert(s[2] <= 3948.0);
                    } else if (instance.num_items == 250 &&
                               instance.num_dimensions == 2) {
                        assert(s[0] >= 0.0);
                        assert(s[0] <= 9893.0);
                        assert(s[1] >= 0.0);
                        assert(s[1] <= 10103.0);
                    } else if (instance.num_items == 500 &&
                               instance.num_dimensions == 2) {
                        assert(s[0] >= 0.0);
                        assert(s[0] <= 20094.0);
                        assert(s[1] >= 0.0);
                        assert(s[1] <= 20490.0);
                    }
                }
            }
        }

        std::cout << solver << std::endl;

        std::cout << "Num non dominated snapshots: ";
        for(unsigned i = 0;
            i < solver.num_non_dominated_snapshots.size() - 1;
            i++) {
            std::cout << "(" 
                    << std::get<0>(solver.num_non_dominated_snapshots[i])
                    << ", "
                    << std::get<1>(solver.num_non_dominated_snapshots[i])
                    << ", "
                    << std::accumulate(
                std::get<2>(solver.num_non_dominated_snapshots[i]).begin(),
                std::get<2>(solver.num_non_dominated_snapshots[i]).end(),
                0) / std::get<2>(solver.num_non_dominated_snapshots[i]).size()
                    << "), ";
        }
        std::cout << "("
                << std::get<0>(solver.num_non_dominated_snapshots.back())
                << ", "
                << std::get<1>(solver.num_non_dominated_snapshots.back())
                << ", "
                << std::accumulate(
            std::get<2>(solver.num_non_dominated_snapshots.back()).begin(),
            std::get<2>(solver.num_non_dominated_snapshots.back()).end(),
            0) / std::get<2>(solver.num_non_dominated_snapshots.back()).size()
                << ")" << std::endl;

        std::cout << "Num fronts snapshots: ";
        for(unsigned i = 0; i < solver.num_fronts_snapshots.size() - 1; i++) {
            std::cout << "("
                    << std::get<0>(solver.num_fronts_snapshots[i])
                    << ", "
                    << std::get<1>(solver.num_fronts_snapshots[i])
                    << ", "
                    << std::accumulate(
                std::get<2>(solver.num_fronts_snapshots[i]).begin(),
                std::get<2>(solver.num_fronts_snapshots[i]).end(),
                0) / std::get<2>(solver.num_fronts_snapshots[i]).size()
                    << "), ";
        }
        std::cout << "("
                << std::get<0>(solver.num_fronts_snapshots.back())
                << ", "
                << std::get<1>(solver.num_fronts_snapshots.back())
                << ", "
                << std::accumulate(
            std::get<2>(solver.num_fronts_snapshots.back()).begin(),
            std::get<2>(solver.num_fronts_snapshots.back()).end(),
            0) / std::get<2>(solver.num_fronts_snapshots.back()).size()
                << ")" << std::endl;
    }

    std::cout << std::endl << "MOEAD Solver Test PASSED" << std::endl;

    return 0;
}
