#pragma once

#include "chromosome.hpp"
#include "instance/instance.hpp"

namespace mokp {

class Decoder {
    public:
    const Instance & instance;

    unsigned decoder_type;

    std::vector<std::vector<bool>> greedy_knapsacks;

    std::vector<std::vector<double>> values;

    std::vector<std::vector<double>> weights;

    std::vector<std::vector<std::pair<double,
                                      std::pair<std::vector<double>,
                                                std::vector<double>>>>> permutations;

    Decoder(const Instance & instance,
            unsigned decoder_type,
            unsigned num_threads);

    std::vector<double> decode(BRKGA::Chromosome & chromosome, bool rewrite);
};

}
