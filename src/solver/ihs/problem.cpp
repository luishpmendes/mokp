#include "solver/nsga2/problem.hpp"
#include "solution/solution.hpp"

namespace mokp {

Problem::Problem(const Instance & instance, unsigned decoder_type) :
    instance(instance), decoder_type(decoder_type) {}

Problem::Problem() {}

pagmo::vector_double Problem::fitness(const pagmo::vector_double & dv) const {
    Solution solution(this->instance, dv, this->decoder_type);

    for (double & v : solution.value) {
        v = -v;
    }

    return solution.value;
}

std::pair<pagmo::vector_double, pagmo::vector_double> Problem::get_bounds()
    const {
    return std::make_pair(
            pagmo::vector_double(this->instance.num_items, 0.0),
            pagmo::vector_double(this->instance.num_items, 1.0));
}

pagmo::vector_double::size_type Problem::get_nobj() const {
    return this->instance.num_dimensions;
}

}
