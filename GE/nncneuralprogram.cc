# include <GE/nncneuralprogram.h>
# include <math.h>
# include <QFile>
# include <QTextStream>
#include <iostream>
NNCNeuralProgram::NNCNeuralProgram(int Dimension,QString TrainFile,QString TestFile):
	NeuralProgram(Dimension)
{

	isvalidation=0;
    char cfile1[1024],cfile2[1024];
    strcpy(cfile1,TrainFile.toStdString().c_str());
    strcpy(cfile2,TestFile.toStdString().c_str());
    if(TrainFile!=nullptr)
	{
        FILE *fp=fopen(cfile1,"r");
		if(!fp)  exit(EXIT_FAILURE);
//		int d;
		fscanf(fp,"%d",&dimension);
		xtemp=new double[dimension];
		int tcount;
		fscanf(fp,"%d",&tcount);
		if(tcount<=0) {fclose(fp); return ;}
		train_xpoint.resize(tcount);
		train_ypoint.resize(tcount);
		xmax.resize(dimension);
		xmin.resize(dimension);
		for(int i=0;i<tcount;i++)
		{
			train_xpoint[i].resize(dimension);
			for(int j=0;j<dimension;j++) 
			{
				fscanf(fp,"%lf",&train_xpoint[i][j]);	
				if(i==0 || train_xpoint[i][j]>xmax[j]) xmax[j]=train_xpoint[i][j];
				if(i==0 || train_xpoint[i][j]<xmin[j]) xmin[j]=train_xpoint[i][j];
			}
			fscanf(fp,"%lf",&train_ypoint[i]);
		}

		fclose(fp);
	}
    if(TestFile!="")
	{
        FILE *fp=fopen(cfile2,"r");
		if(!fp) return;
		int d;
		fscanf(fp,"%d",&d);
		if(d!=dimension) {fclose(fp); return ;}
		int tcount;
		fscanf(fp,"%d",&tcount);
		if(tcount<=0) {fclose(fp); return ;}
		test_xpoint.resize(tcount);
		test_ypoint.resize(tcount);
		categ.resize(0);
		for(int i=0;i<tcount;i++)
		{
			test_xpoint[i].resize(dimension);
			for(int j=0;j<dimension;j++)
				fscanf(fp,"%lf",&test_xpoint[i][j]);
			fscanf(fp,"%lf",&test_ypoint[i]);
			int found=0;
			for(int j=0;j<categ.size();j++)
				if(fabs(categ[j]-test_ypoint[i])<1e-7)
				{
					found=1;
					break;
				}
			if(!found)
			{
				int s=categ.size();
				categ.resize(s+1);
				categ[s]=test_ypoint[i];
			}
		}
		fclose(fp);
	}
    program=new SigProgram(dimension);
    setStartSymbol(program->getStartSymbol());
    neuralparser=new NeuralParser(dimension);

}

static void mymap(Data x,Data &x1)
{
	for(int i=0;i<x.size();i++)
		x1[i]=x[i];
}


void	NNCNeuralProgram::getDeriv(Data &g)
{
	for(int i=0;i<g.size();i++) g[i]=0.0;
	Data tempg;
	tempg.resize(g.size());
	int start=0;
	int end=train_ypoint.size();
	if(isvalidation) end=4*train_ypoint.size()/5;
	for(int i=start;i<end;i++)
	{
		double v=neuralparser->eval(train_xpoint[i])-train_ypoint[i];
		neuralparser->getDeriv(train_xpoint[i],tempg);
		for(int j=0;j<g.size();j++) g[j]+=2.0*v*tempg[j];
	}
}

double	NNCNeuralProgram::getTestError()
{
//    cout<<"Είμαστε στο NNCNeuralProgram::getTestError()\n";
    int t;
	double value=0.0;
    double *xx=new double[dimension];
	for(int i=0;i<test_ypoint.size();i++)
	{
        for(int j=0;j<dimension;j++) xx[j]=test_xpoint[i][j];
        if(program!=nullptr)
		{
            double v = program->Eval(xx)-test_ypoint[i];
//            cout<<"To neuralparser->eval(xx) είναι "<<neuralparser->eval(xx)<<endl;
            value=value+v*v;
		}
	}
//    cout<<"Είμαστε στο NNCNeuralProgram::getTestError(). Κάναμε "<<test_ypoint.size()<<" επαναλήψεις"<<endl;
	delete[] xx;
//    cout<<"Είμαστε στο NNCNeuralProgram::getTestError(). Το TestError είναι "<<value<<endl;
    return value/test_ypoint.size();
}

int	NNCNeuralProgram::getTrainSize() const
{
	return train_ypoint.size();
}

int	NNCNeuralProgram::getTestSize() const
{
	return test_ypoint.size();
}


double	NNCNeuralProgram::getTrainError()
{
	double value=0.0;
	int start=0;
	int end=train_ypoint.size();
	for(int i=start;i<end;i++)
	{
		for(int j=0;j<dimension;j++) xtemp[j]=train_xpoint[i][j];
        double v =program->Eval(xtemp);// neuralparser->eval(xtemp);
        if(program->EvalError() || std::isnan(v) || std::isinf(v))
		{
			return 1e+8;
		}
		v=(v-train_ypoint[i]);
		value=value+v*v;

	}
    return value;
}

double	NNCNeuralProgram::getClassTestError(vector<int> &genome)
{
	double value=0.0;
	string str;

	if(!getElements(genome,str)) return -1e+8;
	program->Parse(str);

    int d = train_xpoint[0].size();
	double *xx=new double[train_xpoint[0].size()];
	for(int i=0;i<test_ypoint.size();i++)
	{
        for(int j=0;j<d;j++)
        {
            xx[j]=test_xpoint[i][j];
        }

        double v=program->Eval(xx);// neuralparser->eval(xx);
        if(program->EvalError() || std::isnan(v) || std::isinf(v))
        {
			value+=1.0;
			continue;
		}
		double minValue=1e+10;
		int index=-1;
		for(int j=0;j<categ.size();j++)
		{
			if(fabs(categ[j]-v)<minValue)
			{
				minValue=fabs(categ[j]-v);
				index = j;
			}
        }
		double myclass=categ[index];
		value+=fabs(test_ypoint[i]-myclass)>1e-5;
	}
	delete[] xx;
	return value*100.0/test_ypoint.size();
}

void     NNCNeuralProgram::printOutput(QString filename)
{
    QFile fp(filename);
    if(!fp.open(QIODevice::WriteOnly |QIODevice::Text)) return;
    QTextStream st(&fp);
    double *xx=new double[train_xpoint[0].size()];
    for(int i=0;i<test_ypoint.size();i++)
    {
        for(int j=0;j<dimension;j++) st<<test_xpoint[i][j]<<" ";
        for(int j=0;j<test_xpoint[i].size();j++) xx[j]=test_xpoint[i][j];
        double v=program->Eval(xx);
//        st<<test_ypoint[i]<<" "<<v<<endl;
    }
    delete[] xx;
    fp.close();
}

void NNCNeuralProgram::normalizeData()
{
    // Κανονικοποίηση δεδομένων εκπαίδευσης
    for (int j = 0; j < dimension; ++j) {
        double min_val = train_xpoint[0][j];
        double max_val = train_xpoint[0][j];
        for (int i = 1; i < train_xpoint.size(); ++i) {
            if (train_xpoint[i][j] < min_val) min_val = train_xpoint[i][j];
            if (train_xpoint[i][j] > max_val) max_val = train_xpoint[i][j];
        }
        for (int i = 0; i < train_xpoint.size(); ++i) {
            train_xpoint[i][j] = (train_xpoint[i][j] - min_val) / (max_val - min_val);
        }
    }

    // Κανονικοποίηση δεδομένων δοκιμής
    for (int j = 0; j < dimension; ++j) {
        double min_val = test_xpoint[0][j];
        double max_val = test_xpoint[0][j];
        for (int i = 1; i < test_xpoint.size(); ++i) {
            if (test_xpoint[i][j] < min_val) min_val = test_xpoint[i][j];
            if (test_xpoint[i][j] > max_val) max_val = test_xpoint[i][j];
        }
        for (int i = 0; i < test_xpoint.size(); ++i) {
            test_xpoint[i][j] = (test_xpoint[i][j] - min_val) / (max_val - min_val);
        }
    }
}

double NNCNeuralProgram::adaptive_mutation_rate(int generation, int max_generations) {
    double initial_rate = 0.1;
    double final_rate = 0.01;
    return initial_rate + (final_rate - initial_rate) * ((double)generation / max_generations);
}

NNCNeuralProgram::~NNCNeuralProgram()
{
	delete[] xtemp;
}
