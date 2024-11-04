#ifndef GWOOPTIMIZER_H
#define GWOOPTIMIZER_H
# include <METHODS/collection.h>
# include <METHODS/optimizer.h>
class GWOoptimizer : public Optimizer
{
private:
    Data Alpha_pos,Beta_pos,Delta_pos;
    vector<Data> Positions;
    Data fitnessArray;
    double Alpha_score,Beta_score,Delta_score;
    int SearchAgents_no, Max_iter;
    int iter;
    double sumMean;
public:
    GWOoptimizer();
    virtual void init();
    virtual void step();
    virtual bool terminated();
    virtual void showDebug();
    virtual void done();
    virtual ~GWOoptimizer();
};

#endif // GWOOPTIMIZER_H
