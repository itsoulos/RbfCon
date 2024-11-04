#ifndef OFALGORITHM_H
#define OFALGORITHM_H
# include <METHODS/problem.h>
# include <METHODS/optimizer.h>
class OFAlgorithm:  public Optimizer
{

private:
    double localsearchRate;
    int maxGenerations;
    vector<vector<double>> population;
    vector<double> fitness;
    long D;
    vector<double> newX;
    Data x;
    double **temp;
    double y=1e+10;
    Optimizer *local=NULL;
    double besty;
    vector<double> bestValues;
    int N;
    vector<double> a;
    vector<double> b;
    int similaritySize;
    double similarityValue;
    int similarityCount;
    double bestFitness;
    Data bestX;
    vector<vector<double>> QOP;
    vector<double> MergeFitness;
    int M;
    vector<vector<double>>  MergePopulation;
    int generation;
    vector<double> CalculateQOS(vector<double>& xi, int N);
    vector<double> calculateChildren(const vector<double>& xt, const vector<double>& x_best, double K_t, int D);
    vector<vector<double>> selectOptimalSolutions(const vector<vector<double>>& P, const vector<vector<double>>& QOP);
    vector<vector<double>> newPopulation;
public:
    OFAlgorithm();
    double evaluate( vector<double>& solution, double& bestFitness);
    double calculateK(int generation, int  maxGenerations) ;
    virtual void init();
    double randomInRange();
    void quicksort(vector<vector<double>>& population, vector<double>& fitness, int left, int right);
    void updatePopulationByComparison(vector<vector<double>>& population, vector<double>& fitness,  vector<vector<double>>& children);
    int PartitionPopulation(vector<vector<double>>& population, vector<double>& fitness, int left, int right);
    bool BetterSolution(double fitness_xt, double fitness_xt1, int generation);
    bool CheckFeasibility(const vector<double>& solution);
    void CalcFitnessArray();
    virtual void step();
    virtual void   done();
    virtual bool terminated();
    void Selection();
    void ChildrenArray();
    virtual ~OFAlgorithm();
};

#endif // OFALGORITHM_H



