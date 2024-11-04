#ifndef DOUBLEBOX_H
#define DOUBLEBOX_H
# include <METHODS/problem.h>

/**
 * @brief The DoubleBox class implements the DoubleBox termination rule.
 */
class DoubleBox
{
private:
    double xx1;
    double xx2;
    double variance;
    double stopat;
    double oldBesty;
    double besty;
    int iteration;
    int miniters;
public:
    /**
     * @brief DoubleBox the constructor of the DoubleBox termination
     * algorithm.
     * @param miniters the minimum number of iterations before the double box
     * initiates.
     */
    DoubleBox(int miniters=5);
    /**
     * @brief init, initialize the doublebox rule
     */
    void init();
    /**
     * @brief setMinIters define the number of minimum iterations.
     * @param m
     */
    void setMinIters(int m);
    /**
     * @brief terminate, check the termination rule
     * @param value
     * @return
     */
    bool terminate(double value);
    /**
     * @brief terminate, it is the same as the previous function,
     * except that the user provide the terminate iteration.
     * @param value
     * @param userIter
     * @return
     */
    bool terminate(double value,int userIter);
    ~DoubleBox();
};

#endif // DOUBLEBOX_H
