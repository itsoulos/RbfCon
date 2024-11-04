#ifndef INTEGERANNEAL_H
#define INTEGERANNEAL_H
# include <GE/program.h>
class IntegerAnneal
{
private:

    Program *myProblem;
    vector<int> xpoint;
    double ypoint;
    vector<int> bestx;
    double besty;
    int neps=100;
    double T0;
    int k;
public:
    IntegerAnneal(Program *pr);
    void    setNeps(int n);
    void    setT0(double t);
    void setPoint(vector<int> &g,double &y);
    void getPoint(vector<int> &g,double &y);
    void Solve();
    void    updateTemp();
    ~IntegerAnneal();
};
#endif // INTEGERANNEAL_H
