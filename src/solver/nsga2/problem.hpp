#pragma once

#include "instance/instance.hpp"
#include <pagmo/types.hpp>

namespace mokp {

class Problem {
    public:
    Instance instance;

    unsigned decoder_type;

    Problem(const Instance & instance, unsigned decoder_type);

    Problem();

    pagmo::vector_double fitness(const pagmo::vector_double & dv) const;

    std::pair<pagmo::vector_double, pagmo::vector_double> get_bounds() const;

    pagmo::vector_double::size_type get_nobj() const;
};

}
