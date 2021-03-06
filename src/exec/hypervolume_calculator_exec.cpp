#include "instance/instance.hpp"
#include "utils/argument_parser.hpp"
#include <pagmo/utils/hypervolume.hpp>
#include <fstream>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);

    if (arg_parser.option_exists("--instance")) {
        std::ifstream ifs;
        mokp::Instance instance;
        ifs.open(arg_parser.option_value("--instance"));

        if (ifs.is_open()) {
            ifs >> instance;

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--instance") + " not found.");
        }

        std::vector<std::vector<std::vector<double>>> paretos;
        std::vector<std::vector<unsigned>> iteration_snapshots;
        std::vector<std::vector<double>> time_snapshots;
        std::vector<std::vector<std::vector<std::vector<double>>>>
            best_solutions_snapshots;
        std::vector<double> max_value(instance.num_dimensions, 0.0),
                            reference_point(instance.num_dimensions, 0.1);
        unsigned num_solvers;

        for (num_solvers = 0;
             arg_parser.option_exists("--pareto-" +
                                      std::to_string(num_solvers)) ||
             arg_parser.option_exists("--best-solutions-snapshots-" +
                                      std::to_string(num_solvers)) ||
             arg_parser.option_exists("--hypervolume-" +
                                      std::to_string(num_solvers)) ||
             arg_parser.option_exists("--hypervolume-snapshots-" +
                                      std::to_string(num_solvers));
            num_solvers++) {}

        paretos.resize(num_solvers);
        iteration_snapshots.resize(num_solvers);
        time_snapshots.resize(num_solvers);
        best_solutions_snapshots.resize(num_solvers);

        for (unsigned i = 0; i < num_solvers; i++) {
            if (arg_parser.option_exists("--pareto-" + std::to_string(i))) {
                ifs.open(arg_parser.option_value("--pareto-" +
                                                 std::to_string(i)));

                if (ifs.is_open()) {
                    for (std::string line; std::getline(ifs, line);) {
                        std::istringstream iss(line);
                        std::vector<double> value(instance.num_dimensions, 0.0);

                        for (unsigned j = 0; j < instance.num_dimensions; j++) {
                            iss >> value[j];

                            if (max_value[j] < value[j]) {
                                max_value[j] = value[j];
                            }
                        }

                        paretos[i].push_back(value);
                    }

                    ifs.close();
                } else {
                    throw std::runtime_error("File " +
                            arg_parser.option_value("--pareto-" +
                                std::to_string(i)) + " not found.");
                }
            }
        }

        for (unsigned i = 0; i < num_solvers; i++) {
            if (arg_parser.option_exists("--best-solutions-snapshots-" +
                                         std::to_string(i))) {
                std::string best_solutions_snapshots_filename =
                    arg_parser.option_value("--best-solutions-snapshots-" +
                                            std::to_string(i));

                for (unsigned j = 0; ; j++) {
                    ifs.open(best_solutions_snapshots_filename +
                             std::to_string(j) + ".txt");

                    if (ifs.is_open()) {
                        unsigned iteration;
                        double time;

                        ifs >> iteration >> time;

                        iteration_snapshots[i].push_back(iteration);
                        time_snapshots[i].push_back(time);
                        best_solutions_snapshots[i].emplace_back();

                        ifs.ignore();

                        for (std::string line; std::getline(ifs, line);) {
                            std::istringstream iss(line);
                            std::vector<double> value(instance.num_dimensions,
                                                      0.0);

                            for (unsigned j = 0;
                                j < instance.num_dimensions;
                                j++) {
                                iss >> value[j];

                                if (max_value[j] < value[j]) {
                                    max_value[j] = value[j];
                                }
                            }

                            best_solutions_snapshots[i].back().push_back(value);
                        }

                        ifs.close();
                    } else {
                        break;
                    }
                }
            }
        }

        for (unsigned i = 0; i < num_solvers; i++) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--hypervolume-" +
                                             std::to_string(i)));

            if (ofs.is_open()) {
                std::vector<std::vector<double>> normalized_pareto(
                        paretos[i].size());
                for (unsigned j = 0; j < paretos[i].size(); j++) {
                    normalized_pareto[j] = std::vector<double>(
                            paretos[i][j].size(), 0.0);
                    for (unsigned k = 0; k < instance.num_dimensions; k++) {
                        normalized_pareto[j][k] = - paretos[i][j][k]
                                                  / max_value[k];
                    }
                }

                pagmo::hypervolume hv(normalized_pareto);
                double hypervolume = hv.compute(reference_point);

                for (unsigned j = 0; j < instance.num_dimensions; j++) {
                    hypervolume /= 1.1;
                }

                ofs << hypervolume << std::endl;

                if (ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--hypervolume-" +
                                std::to_string(i)) + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--hypervolume-" +
                            std::to_string(i)) + " not created.");
            }
        }

        for (unsigned i = 0; i < num_solvers; i++) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--hypervolume-snapshots-" +
                                             std::to_string(i)));

            if (ofs.is_open()) {
                for (unsigned j = 0;
                     j < best_solutions_snapshots[i].size();
                     j++) {
                    std::vector<std::vector<double>>
                        normalized_pareto_snapshot(
                                best_solutions_snapshots[i][j].size());
                    for (unsigned k = 0;
                         k < best_solutions_snapshots[i][j].size();
                         k++) {
                        normalized_pareto_snapshot[k] = std::vector<double>(
                                best_solutions_snapshots[i][j][k].size(), 0.0);
                        for (unsigned l = 0; l < instance.num_dimensions; l++) {
                            normalized_pareto_snapshot[k][l] =
                                - best_solutions_snapshots[i][j][k][l]
                                / max_value[l];
                        }
                    }

                    pagmo::hypervolume hv(normalized_pareto_snapshot);
                    double hypervolume = hv.compute(reference_point);

                    for (unsigned k = 0; k < instance.num_dimensions; k++) {
                        hypervolume /= 1.1;
                    }

                    ofs << iteration_snapshots[i][j] << ","
                        << time_snapshots[i][j] << ","
                        << hypervolume << std::endl;

                    if (ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                arg_parser.option_value(
                                    "--hypervolume-snapshots-" +
                                    std::to_string(i)) + ".");
                    }
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--hypervolume-snapshots-" +
                            std::to_string(i)) + " not created.");
            }
        }
    } else {
        std::cerr << "./hypervolume_calculator_exec "
                  << "--instance <instance_filename> "
                  << "--pareto-i <pareto_filename> "
                  << "--best-solutions-snapshots-i <best_solutions_snapshots_filename> "
                  << "--hypervolume-i <hypervolume_filename> "
                  << "--hypervolume-snapshots-i <hypervolume_snapshots_filename> "
                  << std::endl;
    }

    return 0;
}
