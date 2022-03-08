#pragma once

#include "solution/solution.hpp"
#include "chromosome.hpp"

namespace mokp {

class Decoder {
    public:
    const Instance & instance;

    std::vector<std::vector<bool>> knapsacks;

    std::vector<std::vector<double>> values;

    std::vector<std::vector<double>> weights;

    std::vector<std::vector<std::pair<double, unsigned>>> qs;

    Decoder(const Instance & instance,
            unsigned num_threads);

    std::vector<double> decode(BRKGA::Chromosome & chromosome, bool rewrite);
};

}
