#pragma once

#define NSBRKGA_MULTIPLE_INCLUSIONS

#include <istream>
#include <ostream>
#include <vector>
#include "nsbrkga.hpp"

namespace mokp {
/***********************************************
 * The Instance class represents an instance of
 * the Multi-Objective Binary Knapsack Problem.
 ***********************************************/
class Instance {
    public:
    /**********************************
     * The capacity on each dimension.
     **********************************/
    std::vector<double> capacity;

    /*********************************************
     * The weight of each item on each dimension.
     *********************************************/
    std::vector<std::vector<double>> weight;

    /********************************************
     * The value of each item on each dimension.
     ********************************************/
    std::vector<std::vector<double>> value;

    /***************************
     * The number of dimenions.
     ***************************/
    unsigned num_dimensions;

    /**************************
     * The number of items.
     **************************/
    unsigned num_items;

    /*********************************
     * The optimization senses.
     *********************************/
    std::vector<NSBRKGA::Sense> senses;

    /********************************
     * This instance primal bounds.
     ********************************/
    std::vector<double> primal_bound;

    /*******************************************************
     * The minimum weight over all items on each dimension.
     *******************************************************/
    std::vector<double> min_weight;

    /*********************************************************************
     * The permutation in increasing order of maximum value/weight ratio.
     *********************************************************************/
    std::vector<unsigned> greedy_permutation;

    /*************************************************************
     * The weight of each item in greedy order on each dimension.
     *************************************************************/
    std::vector<std::vector<double>> greedy_weight;

    /************************************************************
     * The value of each item in greedy order on each dimension.
     ************************************************************/
    std::vector<std::vector<double>> greedy_value;

    private:
    /*****************************
     * Initializes a new intance.
     *****************************/
    void init();

    public:
    /******************************************************************
     * Constructs a new instance.
     *
     * @param capacity       The capacity of each knapsack.
     * @param weight         The weight of each item on each knapsack.
     * @param value          The profit of each item on each knapsack.
     ******************************************************************/
    Instance(const std::vector<double> & capacity,
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

    /*************************************************************
     * Standard output operator.
     *
     * @param os       standard output stream object.
     * @param instance the instance.
     *
     * @return the stream object.
     *************************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const Instance & instance);
};

}
