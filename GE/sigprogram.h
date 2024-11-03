# ifndef __SIGPROGRAM__H
# define __SIGPROGRAM__H

# include <GE/cprogram.h>
# include <string>

using namespace std;

class	SigProgram :public Cprogram
{
	private:
		Symbol  Sig,SigFunction,Tanh,Abs,
            s0,sx,Out,smallNumber;
        Symbol RBFexpr, Weight, Gauss, Distance, SmllNmbr, Bias;
	public:
		SigProgram(int d);
};

# endif
