#include <GE/neuralparser.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

static double sig(double x)
{
	return 1.0/(1.0+exp(-x));
}

static double sigder(double x)
{
	double s=sig(x);
	return s*(1-s);
}


NeuralParser::NeuralParser(int Dimension)
    :Problem(dimension)
{
	dimension = Dimension;
    trainSet = NULL;
}

double	NeuralParser::valError()
{
	return 0.0;
}

int	NeuralParser::getRbfDimension() const
{
	return dimension;
}

// Συνάρτηση που εξάγει τις παραμέτρους από τη συμβολοσειρά
std::vector<RbfParameter> NeuralParser::extractParameters(const std::string &input)
{
    std::vector<RbfParameter> parameters;

    // Κανονική έκφραση για την αναγνώριση αριθμών και παραμέτρων της μορφής xk
    std::regex number_regex(R"(([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)|x\d+)");

    try {
        // Χρήση sregex_iterator για την εύρεση όλων των αριθμών και των παραμέτρων της μορφής xk που ταιριάζουν με την κανονική έκφραση
        auto words_begin = std::sregex_iterator(input.begin(), input.end(), number_regex);
        auto words_end = std::sregex_iterator();

        // Βρόχος για την αποθήκευση των αριθμών και των παραμέτρων στο διάνυσμα parameters
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string match_str = match.str();

            // Έλεγχος αν η αντιστοιχία είναι παράμετρος της μορφής xk
            if (match_str[0] == 'x') {
                int index = std::stoi(match_str.substr(1));  // Αφαιρούμε το 'x' και παίρνουμε τον αριθμό
                parameters.push_back(RbfParameter(index));
            } else {
                double number = std::stod(match_str);
                parameters.push_back(RbfParameter(number));
            }
        }
    } catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
        return parameters;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return parameters;
    }

    return parameters;
}

/*
// Συνάρτηση που εξάγει τις παραμέτρους από τη συμβολοσειρά
std::vector<Parameter> NeuralParser::extractParameters(const std::string &input) {
    std::vector<Parameter> parameters;

    // Κανονική έκφραση για την αναγνώριση αριθμών (δεκαδικών και μη) και παραμέτρων της μορφής xk
    std::regex number_regex(R"(([-+]?\d*\.?\d+([eE][-+]?\d+)?)|x(\d+))");

    // Χρήση sregex_iterator για την εύρεση όλων των αριθμών και των παραμέτρων της μορφής xk που ταιριάζουν με την κανονική έκφραση
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), number_regex);
    auto words_end = std::sregex_iterator();

    // Βρόχος για την αποθήκευση των αριθμών και των παραμέτρων στο διάνυσμα parameters
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();

        // Έλεγχος αν η αντιστοιχία είναι παράμετρος της μορφής xk
        if (match_str[0] == 'x') {
            int index = std::stoi(match[3].str());
            parameters.push_back(Parameter(index));
        } else {
            double number = std::stod(match_str);
            parameters.push_back(Parameter(number));
        }
    }
    return parameters;
}
*/
/*
// Συνάρτηση που εξάγει τις παραμέτρους από τη συμβολοσειρά
vector<double> NeuralParser::extractParameters(const string &input)
{
    vector<double> parameters;
    // Κανονική έκφραση για την αναγνώριση αριθμών (δεκαδικών και μη)
    regex number_regex(R"(([-+]?\d*\.?\d+([eE][-+]?\d+)?)|x\d+)");
//    regex number_regex(R"([-+]?\d*\.?\d+([eE][-+]?\d+)?)");
    // Χρήση sregex_iterator για την εύρεση όλων των αριθμών που ταιριάζουν με την κανονική έκφραση
    auto words_begin = sregex_iterator(input.begin(), input.end(), number_regex);
    auto words_end = sregex_iterator();
    // Βρόχος για την αποθήκευση των αριθμών στο διάνυσμα parameters
    for (sregex_iterator i = words_begin; i != words_end; ++i)
    {
        smatch match = *i;
        string match_str = match.str();
        // Αν η αντιστοιχία ταιριάζει με τη μορφή xk, την αγνοούμε
        if (match_str[0] == 'x') {
            continue;
        }
        double number = stod(match_str);
        parameters.push_back(number);
    }
    return parameters;
}
*/

void	NeuralParser::makeVector(string str)
{
    int t;
    vector<RbfParameter> parameters = extractParameters(str);
//    cout<<"Είμαστε στο NeuralParser::makeVector. Το str είναι: "<<str<<endl;
//    cin>>t;
//Έδώ είναι το σύστημα με τις παραμέτρους///////////////////////////
/*
    for (const Parameter &param : parameters)
    {
        if (param.is_variable) {
//            std::cout << "Variable x" << param.variable_index <<  endl;
            std::cout << "x" << param.variable_index <<  " ";
        } else {
//            std::cout << param.value << std::endl;
            std::cout << param.value << " ";
        }
    }
*/
  //  cout<<endl;
//    cin>>t;

    int nodesize=0;
    int cntrsize=0;
    node.resize(nodesize);
    int nmnd=0;
    int ist=1;
    int i = 0;

    for (int j=0; j<parameters.size(); j++)
    {
        if(ist == 1)
        {
            nodesize++;
            node.resize(nodesize);
            node[nmnd].out = parameters[j].value;
//            cout<<"Eimaste sto NeuralParser::makeVector(string str). To node["<<nmnd<<"].out eivai="<<node[nmnd].out <<endl;
            ist = 0;
        }
        else
        {
            if ( (parameters[j].is_variable)  )
            {
                cntrsize++;
                node[nmnd].cntr.resize(cntrsize);
                node[nmnd].cntr[i].index = parameters[j].variable_index;
//                cout<<"Eimaste sto NeuralParser::makeVector(string str). To node["<<nmnd<<"].cntr["<<i<<"].index eivai="<<node[nmnd].cntr[i].index<<endl;
                j++;
                node[nmnd].cntr[i].m = parameters[j].value;
//                cout<<"Eimaste sto NeuralParser::makeVector(string str). To node["<<nmnd<<"].cntr["<<i<<"].m eivai="<<node[nmnd].cntr[i].m <<endl;
                i++;
                j++;
            }
            else
            {
                j++;
                node[nmnd].spread = parameters[j].value;
//                cout<<"Eimaste sto NeuralParser::makeVector(string str). To node["<<nmnd<<"].spread eivai="<<node[nmnd].spread<<endl;
                j = j+2;
                node[nmnd].bias = parameters[j].value;
//                cout<<"Eimaste sto NeuralParser::makeVector(string str). To node["<<nmnd<<"].bias eivai="<<node[nmnd].bias<<endl;
                i=0;
                ist = 1;
                nmnd++;
            }
        }
    }
/*
    cout<<"Το node.size είναι: "<<node.size()<<endl;
    for(int k; k< node.size(); k++)
    {
        cout<<"Το node["<<k<<"].center.size είναι: "<<node[k].cntr.size()<<endl;
    }
    cout<<node[1].cntr.size()<<endl;
*/

    weight.clear();
    weight.resize(node.size() * (dimension + 3), 0.0); // Αρχικοποίηση με 0
//    weight.resize(( node.size() * 3 + 2*node[node.size() - 1].cntr.size()*2 ), 0.0); // Αρχικοποίηση με 0

    fixstatus.resize(weight.size(), 0);

    nmnd = 0;
    ist = 1;
    i = 0;
    int j = 0;
/*
 * Node
    while (nmnd < node.size())
    {
        cout<<nmnd<<" ";
        if (ist == 1)
        {
            //            weight[(dimension + 2) * nmnd + 0] = parameters[j].value; // Θέση out
            weight[(dimension + 3) * nmnd + 0] = node[nmnd].out;
            fixstatus[(dimension + 3) * nmnd + 0] = 1; // Ενημέρωση του fixstatus
            std::cout << "Weight " << (dimension + 3) * nmnd + 0 << ": " << weight[nmnd + 0] << std::endl;
            ist = 0;
        }
        else
        {
            if (i < node[nmnd].cntr.size() )
            {
                weight[(dimension + 3) * nmnd + node[nmnd].cntr[i].index] +=  node[nmnd].cntr[i].m; // Προσθήκη τιμής για mult
                fixstatus[(dimension + 3) * nmnd + parameters[j].variable_index] = 1; // Ενημέρωση του fixstatus
//                cout<<parameters[j].variable_index<<endl;
                std::cout << "Weight " << (dimension + 3) * nmnd + node[nmnd].cntr[i].index << ": " << weight[(dimension + 3) * nmnd + node[nmnd].cntr[i].index] << std::endl;
                cout<<"i: "<<i<<" "<<node[nmnd].cntr[i].index<<endl;
                i++;
            }
            else
            {
                weight[(dimension + 3) * nmnd + (dimension + 1)] = node[nmnd].spread; // Θέση spread
                weight[(dimension + 3) * nmnd + (dimension + 2)] = node[nmnd].bias; // Θέση bias
                fixstatus[(dimension + 3) * nmnd + (dimension + 1)] = 1; // Ενημέρωση του fixstatus
//                fixstatus[(dimension + 3) * nmnd + (dimension + 2)] = 1;
                cout << "Weight " << (dimension + 3) * nmnd + (dimension + 1) << ": " << weight[(dimension + 3) * nmnd + (dimension + 1)] <<endl;
                cout << "Weight " << (dimension + 3) * nmnd + (dimension + 2) << ": " << weight[(dimension + 3) * nmnd + (dimension + 2)] <<endl;
                ist = 1;
                nmnd++;
                i=0;
            }
        }
    }
*/
//parametres

    while (j < parameters.size())
    {
//        cout<<j<<" ";
        if (ist == 1)
        {
//            weight[(dimension + 2) * nmnd + 0] = parameters[j].value; // Θέση out
            weight[(dimension + 3) * nmnd + 0] = parameters[j].value;
            fixstatus[(dimension + 3) * nmnd + 0] = 1; // Ενημέρωση του fixstatus
//            std::cout << "Weight " << (dimension + 3) * nmnd + 0 << ": " << weight[j] << std::endl;
            ist = 0;
            j=j+1;
        }
        else
        {
            if ((j + 1 < parameters.size()) && parameters[j].is_variable)
            {
                weight[(dimension + 3) * nmnd + parameters[j].variable_index] +=  parameters[j+1].value; // Προσθήκη τιμής για mult
                fixstatus[(dimension + 3) * nmnd + parameters[j].variable_index] = 1; // Ενημέρωση του fixstatus
//                cout<<parameters[j].variable_index<<endl;
//                std::cout << "Weight " << (dimension + 3) * nmnd + parameters[j].variable_index << ": " << weight[(dimension + 3) * nmnd + parameters[j].variable_index] << std::endl;
                j = j + 3;
            }
            else
            {
                weight[(dimension + 3) * nmnd + (dimension + 1)] = parameters[j+1].value; // Θέση spread
                weight[(dimension + 3) * nmnd + (dimension + 2)] = parameters[j+3].value; // Θέση bias
                fixstatus[(dimension + 3) * nmnd + (dimension + 1)] = 1; // Ενημέρωση του fixstatus
//                std::cout << "Weight " << (dimension + 3) * nmnd + (dimension + 1) << ": " << weight[(dimension + 3) * nmnd + (dimension + 1)] << std::endl;
//               std::cout << "Weight " << (dimension + 3) * nmnd + (dimension + 2) << ": " << weight[(dimension + 3) * nmnd + (dimension + 2)] << std::endl;
                ist = 1;
                nmnd++;
                j = j+4;
            }
        }
    }

/*
    // Εκτύπωση όλου του διανύσματος weight
    std::cout << "Final weight vector:" << std::endl;
    for (size_t idx = 0; idx < weight.size(); ++idx) {
        std::cout << "weight[" << idx << "] = " << weight[idx] << std::endl;
    }
*/

    parameters.clear();
//    cin>>t;

 //  Εδώ είναι το πρόγραμμα για τα weight///////////////////////
 //   cin>>t;
/*	weight.resize(node.size()*(dimension+2));
	fixstatus.resize(weight.size());
	for(int i=0;i<weight.size();i++) fixstatus[i]=0;
	for(int i=1;i<=node.size();i++)
	{
		weight[(dimension+2)*i-(dimension+1)-1]=node[i-1].out;
        cout<<"Eimaste sto NeuralParser::makeVector(string str). To weight["<<(dimension+2)*i-(dimension+1)-1<<"]="<<weight[(dimension+2)*i-(dimension+1)-1]<<endl;
//        cin>>t;
		weight[(dimension+2)*i-1]=node[i-1].bias;
        cout<<"Eimaste sto NeuralParser::makeVector(string str). To weight["<<(dimension+2)*i-1<<"]="<<weight[(dimension+2)*i-1]<<endl;
//        cin>>t;
		for(int j=1;j<=dimension;j++)
		{
			double sum=0.0;
			for(int k=0;k<node[i-1].mult.size();k++)
			{
                if(node[i-1].mult[k].index==j)
                {
					sum+=node[i-1].mult[k].m;
                    cout<<"Eimaste sto NeuralParser::makeVector(string str). To sum+="<<sum<<endl;
 //                   cin>>t;
                }
			}
			weight[(dimension+2)*i-(dimension+1)+j-1]=sum;
            cout<<"Eimaste sto NeuralParser::makeVector(string str). To weight["<<(dimension+2)*i-(dimension+1)+j-1<<"]="<<weight[(dimension+2)*i-(dimension+1)+j-1]<<endl;
//            cin>>t;
			if(sum==0.0) 
			  fixstatus[(dimension+2)*i-(dimension+1)+j-1]=1;
		}
    }*/

    /** PROBLEM CODE **/
    setDimension(weight.size());
    Data lm,rm;
    lm.resize(weight.size());
    rm.resize(weight.size());
    for(int i=0;i<weight.size();i++)
    {
        lm[i]=-2.0 *fabs(weight[i]);
        rm[i]= 2.0 *fabs(weight[i]);
    }
    setLeftMargin(lm);
    setRightMargin(rm);
    /** END OF PROBLEM CODE **/
}
/** PROBLEM CODE **/

void    NeuralParser::setTrainSet(Dataset *tr)
{
    trainSet = tr;
}
double  NeuralParser::funmin(Data &x)
{
    if(trainSet == NULL)
        return 0.0;
    else
    {
        double sum = 0.0;
        Matrix xall = trainSet->getAllXpoint();
        for(int i=0;i<(int)xall.size();i++)
        {
            double per = eval(xall[i]);
            double y   = trainSet->getYpoint(i);
            printf("diffs %lf %lf \n",per,y);
            sum+=(per-y)*(per-y);
        }
        return sum;
    }
}

Data    NeuralParser::gradient(Data &x)
{
    Data g;
    g.resize(x.size());
   for(int i=0;i<(int)x.size();i++)
    {
            double eps=pow(1e-18,1.0/3.0)*qMax(1.0,fabs(x[i]));
            x[i]+=eps;
            double v1=funmin(x);
            x[i]-=2.0 *eps;
            double v2=funmin(x);
            g[i]=(v1-v2)/(2.0 * eps);
            x[i]+=eps;
    }
    return g;
}
/** END OF PROBLEM CODE **/

string	NeuralParser::print()
{
	int nodes=weight.size()/(dimension+2);
	string s="";
	for(int i=1;i<=nodes;i++)
	{
		string k="";
		for(int j=1;j<=dimension;j++)
		{
			char ss[100];
			sprintf(ss,"%.5lf",weight[(dimension+2)*i-(dimension+1)+j-1]);
			char s1[100];
			sprintf(s1,"%d",j);
			k=k+"("+ss+")"+"*x["+s1+"]";
		}
		char ss[100];
		sprintf(ss,"%.5lf",weight[(dimension+2)*i-1]);
		k=k+"+("+ss+")";
		sprintf(ss,"%.5lf",weight[(dimension+2)*i-(dimension+1)-1]);
		s=s+"("+ss+")*sig("+k+")";
		if(i!=nodes) s=s+"+";
	}
	return s;
}

void	NeuralParser::getWeights(vector<double> &w)
{
	w.resize(weight.size());
	w = weight;
}

void	NeuralParser::setWeights(vector<double> w)
{
	weight = w;
}

void	NeuralParser::getFixStatus(vector<int> &status)
{
	status.resize(fixstatus.size());
	status = fixstatus;
}

double	NeuralParser::eval(vector<double> xpoint)
{
	int nodes=weight.size()/ (dimension + 2);
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int pos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[pos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		per+=weight[(dimension+2)*i-(dimension+1)-1]*sig(arg);
	}
	return per;
}

/* Η ΑΡΧΙΚΗ ΣΥΝΑΡΤΗΣΗ
double	NeuralParser::eval(double  *xpoint)
{
//    cout<<"Είμαστε στο NeuralParser::eval(double  *xpoint)\n";
//    int t;
//    cin>>t;

	int nodes=weight.size()/ (dimension + 2);
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int pos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[pos-1];
		}
      //  if(fabs(arg)>10) return 1e+100;
		arg+=weight[(dimension+2)*i-1];
		per+=weight[(dimension+2)*i-(dimension+1)-1]*sig(arg);
	}
	return per;
}
*/

double	NeuralParser::eval(double  *xpoint)
{
/*    cout<<"Είμαστε στο NeuralParser::eval(double  *xpoint)\n";
    int t;
    cin>>t;
*/
    int nodes = weight.size()/(dimension + 3);
    double per = 0.0;
 //   double gss = 0.0;
    for(int i=0;i<nodes;i++)
    {
        double dstnc = 0.0;
        for(int j=1;j<=dimension;j++)
        {
            if (weight[(dimension+3)*i + j] !=0)
            {
//                cout<<"xpoint["<<j-1<<"] = "<<xpoint[j-1]<<" kai weight["<<(dimension+3)*i + j<<"] = "<<weight[(dimension+3)*i + j]<<endl;
                dstnc += pow((xpoint[j-1] - weight[(dimension+3)*i + j]), 2);
            }
        }
//        if(fabs(arg)>10) return 1e+100;
//  Λάθος gss += weight[(dimension+3)*i]*(exp( -dstnc / (2*pow(weight[(dimension+3)*i + 5],2)) )) + weight[(dimension+3)*i + 6];
//        gss = weight[(dimension+3)*i]*(exp( -dstnc / (2*pow(weight[(dimension+3)*i + 5],2)) )) + weight[(dimension+3)*i + 6];
//        per += gss;
        per += weight[(dimension+3)*i]*(exp( -dstnc / (2*pow(weight[(dimension+3)*i + 5],2)) )) + weight[(dimension+3)*i + 6];
    }
    return per;
}

void	NeuralParser::getDeriv(vector<double> xpoint,vector<double> &g)
{
	int nodes=weight.size()/ (dimension + 2);
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int pos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[pos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		double s2=sigder(arg);
		g[(dimension+2)*i-(dimension+1)-1]=s;
		g[(dimension+2)*i-1]=weight[(dimension+2)*i-(dimension+1)-1]*s2;
		for(int j=1;j<=dimension;j++)
		{
			int pos=(dimension+2)*i-(dimension+1)+j;
			g[pos-1]=weight[(dimension+2)*i-(dimension+1)-1]*xpoint[j-1]*s2;
		}
	}
}

double  NeuralParser::evalDeriv(vector<double> xpoint,int pos)
{
	int nodes=weight.size()/ (dimension + 2);
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[mypos-1];
            if(fabs(weight[mypos-1])>100) return 1e+100;
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		per+=weight[(dimension+2)*i-(dimension+1)-1]*s*(1.0-s)*weight[(dimension+2)*i-(dimension+1)+pos-1];
	}
	return per;
}

void	NeuralParser::getXDeriv(vector<double> xpoint,int pos,vector<double> &g)
{
	int nodes=weight.size()/ (dimension + 2);
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[mypos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		double w1=weight[(dimension+2)*i-(dimension+1)-1];
		double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
		double sigder=s*(1-s);
		double sigder2=s*(1-s)*(1-2.0*s);
//		double sigder3=sigder*(6*s*s-6*s+1);
		g[(dimension+2)*i-1]=w1*w2*sigder2;
		g[(dimension+2)*i-(dimension+1)-1]=w2*sigder;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			if(j!=pos)
			g[mypos-1]=w1*w2*xpoint[j-1]*sigder2;
			else
			g[mypos-1]=w1*sigder+w1*w2*xpoint[j-1]*sigder2;
		}
	}
}

double	NeuralParser::evalDeriv2(vector<double> xpoint,int pos)
{
	int nodes=weight.size()/ (dimension + 2);
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[mypos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		double w1=weight[(dimension+2)*i-(dimension+1)-1];
		double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
		per+=w1*w2*w2*s*(1.0-s)*(1.0-2*s);
	}
	return per;
}

double  NeuralParser::evalDeriv3(vector<double> xpoint,int pos)
{
    int nodes=weight.size()/ (dimension + 2);
    double per=0.0;
    for(int i=1;i<=nodes;i++)
    {
        double arg=0.0;
        for(int j=1;j<=dimension;j++)
        {
            int mypos=(dimension+2)*i-(dimension+1)+j;
            arg+=xpoint[j-1]*weight[mypos-1];
        }
        arg+=weight[(dimension+2)*i-1];
        double s=sig(arg);
        double w1=weight[(dimension+2)*i-(dimension+1)-1];
        double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
        per+=w1*w2*w2*((1-s)*(1-2*s)+2*(1-s)*(1-s));
    }
    return per;
}

void	NeuralParser::getX2Deriv(vector<double> xpoint,int pos,vector<double> &g)
{
//	double v=0.0;
//	double maxv=-1e+100;
	int nodes=weight.size()/ (dimension + 2);
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[mypos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		double sigder=s*(1-s);
		double sigder2=s*(1-s)*(1-2.0*s);
		double sigder3=sigder*((1-2*s)*(1-2*s)-2*sigder);
		double w1=weight[(dimension+2)*i-(dimension+1)-1];
		double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
		g[(dimension+2)*i-1]=w1 * w2 * w2 * sigder3;
		g[(dimension+2)*i-(dimension+1)-1]=w2*w2*s*(1-s)*(1-2*s);
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			if(j!=pos)
			g[mypos-1]=w1*w2*w2*xpoint[j-1]*sigder3;
			else
			g[mypos-1]=2*w1*w2*sigder2+w1*w2*w2*xpoint[j-1]*sigder3;
		}
	}
}


void	NeuralParser::getX3Deriv(vector<double> xpoint,int pos,vector<double> &g)
{
    vector<double> g2=g;
    vector<double> g3=g;
    for(int i=0;i<(int)xpoint.size();i++)
    {
            double eps=pow(1e-18,1.0/3.0)*fmax(1.0,fabs(xpoint[i]));
            xpoint[i]+=eps;
            getX2Deriv(xpoint,pos,g2);
            xpoint[i]-=2.0 *eps;
            getX2Deriv(xpoint,pos,g3);
            g[i]=(g2[i]-g3[i])/(2.0 * eps);
            xpoint[i]+=eps;
    }
}


void	NeuralParser::getMargins(Data &l,Data &r)
{
	l.resize(weight.size());
	r.resize(weight.size());

	int nodes=weight.size()/ (dimension + 2);
	for(int i=1;i<=nodes;i++)
	{
		int mypos;
		for(int j=1;j<=dimension;j++)
		{
			mypos=(dimension+2)*i-(dimension+1)+j;
            l[mypos-1]=-100;
            r[mypos-1]= 100;
		}
        l[(dimension+2)*i-1]=-100;
        r[(dimension+2)*i-1]= 100;

		l[(dimension+2)*i-(dimension+1)-1]=-100;
		r[(dimension+2)*i-(dimension+1)-1]= 100;
    }

}

NeuralParser::~NeuralParser()
{
}

