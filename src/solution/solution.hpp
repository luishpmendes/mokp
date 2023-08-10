#pragma once

#include "instance/instance.hpp"

namespace mokp {
/***********************************************
 * The Solution class represents a solution for
 * the Multi-Objective Binary Knapsack Problem.
 ***********************************************/
class Solution {
    public:
    /************************************************************
     * Returns true if valueA dominates valueB; false otherwise.
     *
     * @param valueA the first value been compared.
     * @param valueB the second value been compared.
     *
     * @return true if valueA dominates valueB; false otherwise.
     ************************************************************/
    static bool dominates(const std::vector<double> & valueA,
                          const std::vector<double> & valueB);

    /****************************
     * The instance been solved.
     ****************************/
    const Instance & instance;

    /****************************************
     * The knapsack assignment of each item.
     ****************************************/
    std::vector<bool> knapsack;

    /********************************************************
     * The knapsack assignment of each item in greedy order.
     ********************************************************/
    std::vector<bool> greedy_knapsack;

    /**************************
     * The solution's value.
     **************************/
    std::vector<double> value;

    /********************************
     * The weight on each dimension.
     ********************************/
    std::vector<double> weight;

    /********************************************************
     * Constructs a new solution.
     *
     * @param instance the instance been solved.
     * @param knapsack the knapsack assignment of each item.
     ********************************************************/
    Solution(const Instance & instance,
             const std::vector<bool> & knapsack);

    /********************************************************
     * Constructs a new solution.
     *
     * @param instance     the instance been solved.
     * @param key          the key representing the
     *                     knapsack assignment of each item.
     * @param decoder_type the decoder type.
     ********************************************************/
    Solution(const Instance & instance,
             const std::vector<double> & key,
             unsigned decoder_type);

    /********************************************
     * Constructs a new solution.
     *
     * @param instance the instance been solved.
     ********************************************/
    Solution(const Instance & instance);

    /*********************************************
     * Verifies whether this solution is
     * feasible for the instance been solved.
     *
     * @return true if this solution is feasible;
     *         false otherwise.
     *********************************************/
    bool is_feasible() const;

    /*******************************************************************
     * Verifies whether this solution dominates the specified one.
     *
     * @param solution the solution whose domination is to be verified.
     *
     * @return true if this instance dominated the specified one;
     *         false otherwise.
     *******************************************************************/
    bool dominates(const Solution & solution) const;

    /*******************************************************
     * Standard input operator.
     *
     * @param is       standard input stream object.
     * @param solution the solution.
     *
     * @return the stream object.
     *******************************************************/
    friend std::istream & operator >>(std::istream & is,
                                      Solution & solution);

    /*************************************************************
     * Standard output operator.
     *
     * @param os       standard output stream object.
     * @param solution the solution.
     *
     * @return the stream object.
     *************************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const Solution & solution);
};

}
