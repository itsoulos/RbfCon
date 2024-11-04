#ifndef __NEURALPARSER__H
#define __NEURALPARSER__H
# include <METHODS/problem.h>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using namespace std;


struct Center
{
	double	m;
	int	index;
};

struct Node
{
	double out;
    vector<Center> cntr;
    double spread;
	double bias;
};

// Δομή για την αποθήκευση αριθμών και παραμέτρων της μορφής xk
struct RbfParameter {
    bool is_variable;
    double value;
    int variable_index;

    RbfParameter(double val) : is_variable(false), value(val), variable_index(-1) {}
    RbfParameter(int index) : is_variable(true), value(0.0), variable_index(index) {}
};

class NeuralParser : public Problem
{
	private:
		int dimension;
		vector<int> 	fixstatus;
		vector<Node> 	node;
		vector<double>	weight;
	public:
		NeuralParser(int Dimension);
//        vector<double> extractParameters(const string &input);
        vector<RbfParameter> extractParameters(const string &input);
        int     getRbfDimension() const;
		void	makeVector(string str);
		void	getWeights(vector<double> &w);
		void	getFixStatus(vector<int> &status);	
		void	setWeights(vector<double> w);
		double	eval(vector<double> xpoint);
		double	eval(double *xpoint);
		double  evalDeriv(vector<double> xpoint,int pos);
		double	evalDeriv2(vector<double> xpoint,int pos);
        double  evalDeriv3(vector<double> xpoint,int pos);
		void	getDeriv(vector<double> xpoint,vector<double> &g);
		void	getXDeriv(vector<double> xpoint,int pos,vector<double> &g);
		void	getX2Deriv(vector<double> xpoint,int pos,vector<double> &g);
        void	getX3Deriv(vector<double> xpoint,int pos,vector<double> &g);
		virtual double	valError();
		void	getMargins(Data &l,Data &r);
		string  print();

        virtual double funmin(Data &x);
        virtual Data   gradient(Data &x);
		~NeuralParser();
};

# endif
