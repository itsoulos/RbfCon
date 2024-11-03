# include <GE/sigprogram.h>
# include <math.h>


static double sig(const double *X)
{

	return 1.0/(1.0+exp(-X[0]));
}

static double remf(const double *X)
{
	return X[0]-(int)X[0];
}

static double mPow(const double *X)
{
	return pow(X[0],X[1]);
}

SigProgram::SigProgram(int d)
	:Cprogram(d)
{
	int r;
    Start.cut(0);
    parser.AddFunction("sig",sig,1);
	parser.AddFunction("remf",remf,1);
	parser.AddFunction("pow",mPow,2);

    Sig.set("sig",1);
	s0.set("s0",0);
	sx.set("sx",0);
	SigFunction.set("SIGF",0);
	Abs.set("remf",1);
    RBFexpr.set("RBFEXPR",0);
    Gauss.set("GAUSS",0);
    Distance.set("DISTANCE",0);


    r=newRule();
    rule[r]->addSymbol(&RBFexpr);
    Start.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Weight);
    rule[r]->addSymbol(&Mult);
    rule[r]->addSymbol(&Gauss);
    rule[r]->addSymbol(&Plus);
    rule[r]->addSymbol(&Bias);
    rule[r]->addSymbol(&Rpar);
    RBFexpr.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Weight);
    rule[r]->addSymbol(&Mult);
    rule[r]->addSymbol(&Gauss);
    rule[r]->addSymbol(&Plus);
    rule[r]->addSymbol(&Bias);
    rule[r]->addSymbol(&Rpar);
    rule[r]->addSymbol(&Plus);
    rule[r]->addSymbol(&RBFexpr);
    RBFexpr.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Exp);
        rule[r]->addSymbol(&Lpar);
            rule[r]->addSymbol(&Lpar);
            rule[r]->addSymbol(&Minus);
            rule[r]->addSymbol(&Distance);
        rule[r]->addSymbol(&Rpar);
        rule[r]->addSymbol(&Div);
            rule[r]->addSymbol(&Lpar);
                rule[r]->addSymbol(&Digit[2]);
                rule[r]->addSymbol(&Mult);
                rule[r]->addSymbol(&Pow);
                rule[r]->addSymbol(&Lpar);
                    rule[r]->addSymbol(&Out);
                    rule[r]->addSymbol(&Comma);
                    rule[r]->addSymbol(&Digit[2]);
                rule[r]->addSymbol(&Rpar);
            rule[r]->addSymbol(&Rpar);
        rule[r]->addSymbol(&Rpar);
    rule[r]->addSymbol(&Rpar);
    Gauss.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Pow);
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&XXlist);
    rule[r]->addSymbol(&Minus);
    rule[r]->addSymbol(&Out);
    rule[r]->addSymbol(&Comma);
    rule[r]->addSymbol(&Digit[2]);
    rule[r]->addSymbol(&Rpar);
    rule[r]->addSymbol(&Rpar);
    Distance.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Pow);
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&XXlist);
    rule[r]->addSymbol(&Minus);
    rule[r]->addSymbol(&Out);
    rule[r]->addSymbol(&Comma);
    rule[r]->addSymbol(&Digit[2]);
    rule[r]->addSymbol(&Rpar);
    rule[r]->addSymbol(&Rpar);
    rule[r]->addSymbol(&Plus);
    rule[r]->addSymbol(&Distance);
    rule[r]->addSymbol(&Rpar);
    Distance.addRule(rule[r]);

    DigitList.cut(0);

    r=newRule();
    rule[r]->addSymbol(&Digit0);
    DigitList.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Digit0);
    rule[r]->addSymbol(&Digit0);

//    rule[r]->addSymbol(&DigitList);
    DigitList.addRule(rule[r]);


    Out.set("OUT",0);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Rpar);
    Out.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Minus);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Rpar);
    Out.addRule(rule[r]);

    SmllNmbr.set("SMLLNMBR",0);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Digit[0]);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&Digit1);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Rpar);
    SmllNmbr.addRule(rule[r]);

    Bias.set("BIAS",0);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Digit[0]);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&Digit[0]);
    rule[r]->addSymbol(&Digit[0]);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Rpar);
    Bias.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Minus);
    rule[r]->addSymbol(&Digit[0]);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&Digit[0]);
    rule[r]->addSymbol(&Digit[0]);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Rpar);
    Bias.addRule(rule[r]);

    Weight.set("WEIGHT",0);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Rpar);
    Weight.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Minus);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Rpar);
    Weight.addRule(rule[r]);


}
