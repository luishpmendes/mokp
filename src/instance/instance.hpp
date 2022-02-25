#pragma once

#include <istream>
#include <ostream>
#include <vector>

namespace mokp {
/***********************************************
 * The Instance class represents an instance of
 * the Multi-Objective Binary Knapsack Problem.
 ***********************************************/
class Instance {
    public:
    /**************************
     * The number of knapsack.
     **************************/
    unsigned num_knapsacks;

    /**************************
     * The number of items.
     **************************/
    unsigned num_items;

    /*********************************
     * The capacity of each knapsack.
     *********************************/
    std::vector<double> capacity;

    /********************************************
     * The weight of each item on each knapsack.
     ********************************************/
    std::vector<std::vector<double>> weight;

    /********************************************
     * The value of each item on each knapsack.
     ********************************************/
    std::vector<std::vector<double>> value;

    /*****************************************************************
     * Constructs a new instance.
     *
     * @param num_knapsacks The number of knapsacks.
     * @param num_items     The number of items.
     * @param capacity      The capacity of each knapsack.
     * @param weight        The weight of each item on each knapsack.
     * @param value         The profit of each item on each knapsack.
     *****************************************************************/
    Instance(unsigned num_knapsacks,
             unsigned num_items,
             const std::vector<double> & capacity,
             const std::vector<std::vector<double>> & weight,
             const std::vector<std::vector<double>> & value);

    /*********************************************
     * Copy constructor.
     *
     * @param instance the instance been copied.
     ********************************************/
    Instance(const Instance & instance);

    /********************************
     * Constructs an empty instance.
     ********************************/
    Instance();

    /************************************************
     * Copy assignment operator.
     *
     * @param instance the instance been copied.
     *
     * @return this instance
     ************************************************/
    Instance operator = (const Instance & instance);

    /***********************************************************
     * Verifies whether this instance is valid.
     *
     * @return true if this instance is valid; false otherwise.
     ***********************************************************/
    bool is_valid() const;

    /**************************************************************************
     * Standard input operator.
     *
     * @param is       standard input stream object.
     * @param instance the instance.
     *
     * @return the stream object.
     **************************************************************************/
    friend std::istream & operator >>(std::istream & is, Instance & instance);

    /**************************************************************************
     * Standard output operator.
     *
     * @param os       standard output stream object.
     * @param instance the instance.
     *
     * @return the stream object.
     **************************************************************************/
    friend std::ostream & operator <<(std::ostream & os, Instance & instance);
};

}
