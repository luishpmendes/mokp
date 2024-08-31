#include "utils/argument_parser.hpp"
#include <chrono>
#include <iostream>
#include <random>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);
    unsigned m, n, min_p, max_p, min_w, max_w, seed, i, j, c;
    std::vector<unsigned> p, w;

    if (arg_parser.option_exists("--m") &&
        arg_parser.option_exists("--n") &&
        arg_parser.option_exists("--min-p") &&
        arg_parser.option_exists("--max-p") &&
        arg_parser.option_exists("--min-w") &&
        arg_parser.option_exists("--max-w")){
        m = std::stoul(arg_parser.option_value("--m"));
        n = std::stoul(arg_parser.option_value("--n"));
        min_p = std::stoul(arg_parser.option_value("--min-p"));
        max_p = std::stoul(arg_parser.option_value("--max-p"));
        min_w = std::stoul(arg_parser.option_value("--min-w"));
        max_w = std::stoul(arg_parser.option_value("--max-w"));
        p = std::vector<unsigned>(n);
        w = std::vector<unsigned>(n);

        if (arg_parser.option_exists("--seed")) {
            seed = std::stoul(arg_parser.option_value("--seed"));
        } else {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
        }

        std::mt19937 rng(seed);
        rng.discard(10000);
        std::uniform_int_distribution<unsigned> p_dist(min_p, max_p);
        std::uniform_int_distribution<unsigned> w_dist(min_w, max_w);

        std::cout << m << " " << n << std::endl;

        for (i = 0; i < m; i++) {
            c = 0;

            for (j = 0; j < n; j++) {
                p[j] = p_dist(rng);
                w[j] = w_dist(rng);
                c += w[j];
            }

            c >>= 1;

            std::cout << c << std::endl;

            for (j = 0; j < n; j++) {
                std::cout << w[j] << " " << p[j] << std::endl;
            }
        }
    }

    return 0;
}
