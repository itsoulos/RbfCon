#ifndef __NEURALPROGRAM__H
#define __NEURALPROGRAM__H

#include <GE/program.h>
#include <GE/sigprogram.h>
# include <GE/neuralparser.h>
// ///////////////////////
// /////////////////
# include <GE/program.h>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
# include <vector>
using namespace std;
typedef vector<double> Data;

class NeuralProgram :
	public Program
{
	protected:
		int 		dimension;
		SigProgram	*program;
        // /////////////
//        GaussProgram  * program1;
        // /////////////
		int		multiple;
	public:
		NeuralParser		*neuralparser;
		vector<NeuralParser*> 	nparser;
		NeuralProgram(int Dimension);
        NeuralProgram();
        void    setDimension(int Dimension);
		void		enableMultiple(int K);
		int		ismultiple();
		string  printProgram(vector<int> &genome);
		int	getElements(vector<int> &genome,string &str1);
		double 	fitness(vector<int> &genome);
		double  TestError(vector<int>  &genome);
		virtual void	getDeriv(Data &g);

		virtual double	getTrainError();
		virtual double	getTrainError(Data x);
		virtual double	getTestError();
		virtual double 	penalty1();
		virtual double 	penalty2();
		virtual double 	penalty3();
		virtual double 	penalty4();
		virtual double  getPartError();
		virtual double  getPartError(Data &value);
        virtual void    printOutput(QString filename)=0;
        virtual double adaptive_mutation_rate(int generation, int max_generations) = 0;
		~NeuralProgram();
};

# endif
