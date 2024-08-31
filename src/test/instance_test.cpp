#include "instance/instance.hpp"
#include <cassert>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream ifs;
    mokp::Instance instance;

    for (const std::string filename : {"instances/zlt_100_2.txt",
                                       "instances/zlt_100_3.txt",
                                       "instances/zlt_100_4.txt",
                                       "instances/zlt_250_2.txt",
                                       "instances/zlt_250_3.txt",
                                       "instances/zlt_250_4.txt",
                                       "instances/zlt_500_2.txt",
                                       "instances/zlt_500_3.txt",
                                       "instances/zlt_500_4.txt",
                                       "instances/zlt_750_2.txt",
                                       "instances/zlt_750_3.txt",
                                       "instances/zlt_750_4.txt",
                                       "instances/zlt_1000_2.txt",
                                       "instances/zlt_1000_3.txt",
                                       "instances/zlt_1000_4.txt"}) {
        std::cout << filename << std::endl;

        ifs.open(filename);

        assert(ifs.is_open());

        ifs >> instance;

        ifs.close();

        assert(instance.num_dimensions >= 2);
        assert(instance.num_dimensions <= 4);
        assert(instance.num_items >= 100);
        assert(instance.num_items <= 1000);
        assert(instance.is_valid());
    }

    std::cout << std::endl << "Instance Test PASSED" << std::endl;

    return 0;
}
