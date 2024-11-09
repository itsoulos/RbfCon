#include <QCoreApplication>
#include <GE/nncneuralprogram.h>
#include <GE/population.h>
#include <GE/converter.h>
#include <QFile>
# include <QDebug>
#include <QTextStream>
#include <QIODevice>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
# include <CORE/parameterlist.h>
# include <METHODS/bfgs.h>
# include <METHODS/lbfgs.h>
# include <METHODS/gradientdescent.h>
# include <METHODS/adam.h>
using namespace std;


NeuralProgram *program=nullptr;
Population *pop=nullptr;
vector<double> parameters;
typedef vector<int> chromosome;
ParameterList mainParams;
QString trainfile="";
QString testfile="";
int ik=0;
Dataset *trainSet=NULL,*testSet=NULL;
double average_train_error = 0.0;
double average_test_error  = 0.0;
double average_class_error = 0.0;

void    makeMainParams()
{
    mainParams.addParam(Parameter("trainfile","","The used train dataset"));
    mainParams.addParam(Parameter("testfile","","The used test dataset"));
    mainParams.addParam(Parameter("chromosome_count",200,10,1000,"The number of chromosomes"));
    mainParams.addParam(Parameter("chromosome_size",100,50,500,"The size of chromosomes"));
    mainParams.addParam(Parameter("selection_rate",0.1,0.0,1.0,"The used selection rate"));
    mainParams.addParam(Parameter("mutation_rate",0.05,0.0,1.0,"The used mutation rate"));
    mainParams.addParam(Parameter("generations",200,10,2000,"The maximum number of allowed generations"));
    QStringList localMethod;
    localMethod<<"bfgs"<<"adam"<<"lbfgs"<<"gradient"<<"none";
    mainParams.addParam(Parameter("local_method",localMethod[0],localMethod,"The used local optimizer"));
    mainParams.addParam(Parameter("margin_factor",2.0,1.0,10.0,"The factor used to construct margins"));
    mainParams.addParam(Parameter("iterations",30,1,100,"Number of execution iterations"));
}
int getDimension(QString filename)
{
    int d=0;
    QFile fp(filename);
    if(!fp.open(QIODevice::ReadOnly|QIODevice::Text)) return 0;
    QTextStream st(&fp);
    st>>d;
    fp.close();
    return d;
}
void done()
{
    if(program!=nullptr)
        delete program;
    if(pop!=nullptr)
        delete pop;
    if(trainSet != NULL)
        delete trainSet;
    if(testSet !=NULL)
        delete testSet;
}

void shouldTerminate()
{
    done();
    qApp->exit(0);
    exit(0);
}

void error(QString message)
{
    printf("Fatal error %s \n",message.toStdString().c_str());
    shouldTerminate();
}
void init()
{
    trainfile = mainParams.getParam("trainfile").getValue();
    testfile  = mainParams.getParam("testfile").getValue();
    if(trainfile.isEmpty()
     ||testfile.isEmpty())
    {
        error("Train or test are empty");
    }
    if(!QFile::exists(trainfile))
        error(QString("Train file %1 does not exist").arg(trainfile)
                    );

    if(!QFile::exists(testfile))
        error(QString("Test file %1 does not exist").arg(testfile)
                    );

    if(trainSet ==NULL)
    {
        trainSet=new Dataset();
        trainSet->loadFromDataFile(trainfile);
        testSet = new Dataset();
        testSet->loadFromDataFile(testfile);
    }

    int d=0;
    d=getDimension(trainfile);
    program = new NNCNeuralProgram (d,trainfile,testfile);
    pop=new Population (
                mainParams.getParam("chromosome_count").getValue().toInt(),
                mainParams.getParam("chromosome_size").getValue().toInt(),
                program);
    pop->setSelectionRate(
                mainParams.getParam("selection_rate").getValue().toDouble());
    pop->setMutationRate(
                mainParams.getParam("mutation_rate").getValue().toDouble());
}


void printOption(Parameter param)
{
    qDebug().noquote()<<"Parameter name:           "<<param.getName();
    qDebug().noquote()<<"\tParameter default value:"<<param.getValue();
    qDebug().noquote()<<"\tParameter purpose:      "<<param.getHelp();
}

void    printHelp()
{
    qDebug().noquote()<<"MAIN PARAMETERS\n=================================================";
    for(int i=0;i<mainParams.countParameters();i++)
        printOption(mainParams.getParam(i));
    shouldTerminate();
}
void parseCmdLine(QStringList args)
{
    QString lastParam="";
    for(int i=1;i<args.size();i++)
    {
        if(args[i]=="--help") printHelp();
        QStringList lst = args[i].split("=");
        if(lst.size()<=1)
            error(QString("Fatal error %1 not an option").arg(args[i]));
        QString name = lst[0];
        QString value = lst[1];
        if(name.startsWith("--"))
            name = name.mid(2);
        if(value=="")
        {
            error(QString("Param %1 is empty.").arg(value));
        }
        bool foundParameter = false;
        //check in mainParams
        if(mainParams.contains(name))
        {
            mainParams.setParam(name,value);
            foundParameter=true;
        }
        if(!foundParameter)
                 error(QString("Parameter %1 not found.").arg(name));
    }
}
void run()
{
    const int max_generations=mainParams.getParam("generations").getValue().toInt();
    vector<int> genome;
    genome.resize(mainParams.getParam("chromosome_size").getValue().toInt());
    string str;
    double f;
    double old_test_error=0.0;
    Data bestWeights;
    double bestError=1e+100;
    pop->reset();
    for(int i=1;i<=max_generations;i++)
    {
        pop->nextGeneration();
        f=pop->getBestFitness();
        genome=pop->getBestChromosome();

        str =  program->printProgram(genome);
        program->fitness(genome);
        if(fabs(f)<bestError)
        {
            bestError=fabs(f);
            program->neuralparser->getWeights(bestWeights);
        }
        if(i%50==0)
            printf("Generation %4d Best Error:  %20.10lg \n",i,bestError);
        if(fabs(bestError)<1e-6) break;
    }

    old_test_error = program->getTestError();
    str = program->printProgram(genome);
    NeuralParser *parser = dynamic_cast<NNCNeuralProgram*>(program)->neuralparser;
    parser->setMarginFactor(mainParams.getParam("margin_factor").getValue().toDouble());
    parser->makeVector(str);
    Data weights;

    parser->setTrainSet(trainSet);
    weights.resize(parser->getRbfDimension());
    parser->getWeights(weights);
    double fstart = parser->funmin(weights);
    QString localMethod = mainParams.getParam("local_method").getValue();
    if(localMethod=="bfgs")
    {
        Bfgs *m = new Bfgs();
        m->setProblem(dynamic_cast<Problem*>(parser));
        m->setPoint(weights,fstart);
        m->solve();
        m->getPoint(weights,fstart);
        delete m;
    }
    else
    if(localMethod == "lbfgs")
    {
        Lbfgs *m = new Lbfgs();
        m->setProblem(dynamic_cast<Problem*>(parser));
        m->setPoint(weights,fstart);
        m->solve();
        m->getPoint(weights,fstart);
        delete m;
    }
    else
    if(localMethod == "adam")
    {
        Adam *m = new Adam();
        m->setProblem(dynamic_cast<Problem*>(parser));
        m->setPoint(weights,fstart);
        m->solve();
        m->getPoint(weights,fstart);
        delete m;
    }
    else
    if(localMethod == "gradient")
    {
        GradientDescent *m = new GradientDescent();
        m->setProblem(dynamic_cast<Problem*>(parser));
        m->setPoint(weights,fstart);
        m->solve();
        m->getPoint(weights,fstart);
        delete m;
    }
    double class_test;
    if(localMethod!="none")
    {
    average_train_error+=fstart;
    old_test_error=parser->getTestError(testSet);
    average_test_error+=old_test_error;
     class_test=parser->getClassError(testSet);//p->getClassTestError(genome);
    average_class_error+=class_test;
    }
    else
    {
	    average_train_error+=bestError;
	    average_test_error+=old_test_error;
   		NNCNeuralProgram *p=(NNCNeuralProgram*)program;
    		class_test=p->getClassTestError(genome);
		average_class_error+=class_test;
    }

    printf("Iteration: %4d TRAIN ERROR: %20.10lg\n",ik,fstart);
    printf("Iteration: %4d TEST  ERROR: %20.10lg\n",ik,old_test_error);
    printf("Iteration: %4d CLASS ERROR: %20.2lf%%\n",ik,class_test);
    printf("Iteration: %4d SOLUTION:    %20s\n",ik,str.c_str());
}


void report()
{
    int iters = mainParams.getParam("iterations").getValue().toInt();
    printf("Average Train Error: %20.10lg\n",average_train_error/iters);
    printf("Average Test  Error: %20.10lg\n",average_test_error/iters);
    printf("Average Class Error: %20.2lf%%\n",average_class_error/iters);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL,"C");

    makeMainParams();
    parseCmdLine(a.arguments());
    init();
    int iters = mainParams.getParam("iterations").getValue().toInt();
    for( ik=1;ik<=iters;ik++)
    {
        srand(ik);
        run();
    }
    report();
    done();
    return 0;
}
