 #ifndef UNIFORMSAMPLER_H
#define UNIFORMSAMPLER_H
#include "METHODS/problemsampler.h"
/**
 * @brief The UniformSampler class samples point from
 *        the objective problem using uniform distribution.
 */
class UniformSampler : public ProblemSampler
{
private:
    Matrix xpoint;
    Data   ypoint;
public:
    UniformSampler(Problem *p);
    void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);
    void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);
    double  eval(Data &xpoint);
    void    addSampleFromProblem(Data &x,double y);
    ~UniformSampler();
};

#endif // UNIFORMSAMPLER_H
