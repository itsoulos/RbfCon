# include <GE/population.h>
# include <GE/integeranneal.h>
# include <math.h>
# include <iostream>

//# define MAX_RULE	65536
# define MAX_RULE	256


/* Population constructor */
/* Input: genome count , genome size, pointer to Program instance */
Population::Population(int gcount,int gsize,Program *p)
{
	elitism=1;
	selection_rate = 0.1;
	mutation_rate  = 0.05;
    chromosome_count   = gcount;
    chromosome_size    = gsize;
	generation     = 0;
	program        = p;

	/* Create the population and based on genome count and size */
	/* Initialize the genomes to random */
    chromosome=new int*[chromosome_count];
    children=new int*[chromosome_count];
    for(int i=0;i<chromosome_count;i++)
	{
        chromosome[i]=new int[chromosome_size];
        children[i]=new int[chromosome_size];
            for(int j=0;j<chromosome_size;j++)
                chromosome[i][j]=rand()%MAX_RULE;
	}
    fitness_array=new double[chromosome_count];
}

/* Reinitialize the population to random */
void	Population::reset()
{
	generation = 0;
    for(int i=0;i<chromosome_count;i++)
        for(int j=0;j<chromosome_size;j++)
                chromosome[i][j]=rand()%MAX_RULE;
    for(int i=0;i<chromosome_count;i++)
			fitness_array[i]=-1e+8;
}

/* Return the fitness of a genome */
double 	Population::fitness(vector<int> &g)
{

	double f=program->fitness(g);
	return f;
}

/* The selection of the chromosomes according to the fitness value is performed */
void	Population::select()
{
    int itemp[chromosome_size];
    for(int i=0;i<chromosome_count;i++)
	{
        for(int j=0;j<chromosome_count-1;j++)
		{
			if(fitness_array[j+1]>fitness_array[j])
			{
				double dtemp;
				dtemp=fitness_array[j];
				fitness_array[j]=fitness_array[j+1];
				fitness_array[j+1]=dtemp;
				
                memcpy(itemp,chromosome[j],chromosome_size*sizeof(int));
                memcpy(chromosome[j],chromosome[j+1],chromosome_size*sizeof(int));
                memcpy(chromosome[j+1],itemp,chromosome_size*sizeof(int));
			}
		}
	}
}

/* Crossover operation: based on tournament selection */
/* Select the tournament_size based on the genome count : */
/*     (if genome_count > 100 ) tournament_size = 10   else   tournament_size = 4 */
/* Select 2 chromosomes based on the tournament size and cross them over based on the crossover probability */
/* There is 1 crossover point and it is random */
void	Population::crossover()
{
        int parent[2];
        int nchildren=(int)((1.0 - selection_rate) * chromosome_count);
	if(!(nchildren%2==0)) nchildren++;
        const int tournament_size =(chromosome_count<=100)?4:10;
        int count_children=0;
        while(1)
        {
		// The two parents are selected here according to the tournament selection procedure
                for(int i=0;i<2;i++)
                {
                        double max_fitness=-1e+10;
                        int    max_index=-1;
			int r;
			// Select the best parents of  the candidates 
                        for(int j=0;j<tournament_size;j++)
                        {
                r=rand() % (chromosome_count);
                                if(j==0 || fitness_array[r]>max_fitness)
                                {
                                        max_index=r;
                                        max_fitness=fitness_array[r];
                                }
                        }
                        parent[i]=max_index;
                }
        int pt1;
		// The one-point crossover is performed here (the point is pt1)
        pt1=rand() % chromosome_size;
		memcpy(children[count_children],
                chromosome[parent[0]],pt1 * sizeof(int));
		memcpy(&children[count_children][pt1],
            &chromosome[parent[1]][pt1],(chromosome_size-pt1)*sizeof(int));
		memcpy(children[count_children+1],
                chromosome[parent[1]],pt1 * sizeof(int));
		memcpy(&children[count_children+1][pt1],
            &chromosome[parent[0]][pt1],(chromosome_size-pt1)*sizeof(int));
		count_children+=2;
		if(count_children>=nchildren) break;
	}
	
	

	for(int i=0;i<nchildren;i++)
	{
        memcpy(chromosome[chromosome_count-i-1],
                children[i],chromosome_size * sizeof(int));
	}
}

void	Population::setElitism(int s)
{
	elitism = s;
}

/* Mutate the current population */
/* Standard mutation algorithm: mutate all chromosomes in the population based on the mutation probability */
void	Population::mutate()
{
    int start = elitism * (int)(chromosome_count*selection_rate);
	
	start = elitism;
	start = 1;

    for(int i=start;i<chromosome_count;i++)
	{
        for(int j=0;j<chromosome_size;j++)
		{
			double r=rand()*1.0/RAND_MAX;
			if(r<mutation_rate)
			{
                chromosome[i][j]=rand() % MAX_RULE;
			}
		}
	}
}

/* Evaluate the fitness for all chromosomes in the current population */
void	Population::calcFitnessArray()
{
	vector<int> g;
    g.resize(chromosome_size);

	double dmin = 1e+100;
    for(int i=0;i<chromosome_count;i++)
	{
        for(int j=0;j<chromosome_size;j++) g[j]=chromosome[i][j];
			fitness_array[i]=fitness(g);	
		if(fabs(fitness_array[i])<dmin) dmin=fabs(fitness_array[i]);
      //  if(rand() *1.0/RAND_MAX<=0.001)
       //     localSearch(i);
/*
        if(i%10==0)
		{
			printf("%d:%lf ",i,dmin);
			fflush(stdout);
		}
*/
	}
}

/* Return the current generation */
int	Population::getGeneration() const
{
	return generation;
}

/* Return the genome count */
int	Population::getCount() const
{
    return chromosome_count;
}

/* Return the size of the population */
int	Population::getSize() const
{
    return chromosome_size;
}

/* Evolve the next generation */
void	Population::nextGeneration()
{
	calcFitnessArray();
	select();
	crossover();
	mutate();
/*    if((generation+1)%20==0)
    {
        for(int i=0;i<20;i++)
        localSearch(rand() % chromosome_count);
    }*/
    localSearch(0);
	++generation;
}

void	Population::localSearch(int gpos)
{
	vector<int> g;
    g.resize(chromosome_size);
	int pos=gpos;
    double f= fitness_array[pos];
    double fold = f;/*
    IntegerAnneal lt(program);
    lt.setPoint(g,f);
    lt.Solve();
    lt.getPoint(g,f);
    for(int i=0;i<(int)g.size();i++)
        chromosome[gpos][i]=g[i];
    fitness_array[gpos]=f;
    printf("ANNEAL[%d] [%lf]=>[%lf]\n",gpos,fold,f);
    return;*/

	for(int iters=1;iters<=100;iters++)
	{
        int randgenome=rand() % chromosome_count;
        int randpos=rand() % chromosome_size;
        for(int i=0;i<randpos;i++) g[i]=chromosome[pos][i];
        for(int i=randpos;i<chromosome_size;i++) g[i]=chromosome[randgenome][i];
		double f=fitness(g);
		if(fabs(f)<fabs(fitness_array[pos]))
		{
            for(int i=0;i<chromosome_size;i++) chromosome[pos][i]=g[i];
            fitness_array[pos]=f;
		}
		else
		{
            for(int i=0;i<randpos;i++) g[i]=chromosome[randgenome][i];
            for(int i=randpos;i<chromosome_size;i++) g[i]=chromosome[pos][i];
			f=fitness(g);
			if(fabs(f)<fabs(fitness_array[pos]))
			{
                for(int i=0;i<chromosome_size;i++) chromosome[pos][i]=g[i];
				fitness_array[pos]=f;

			}
		}
	}
	return;
}

/* Set the mutation rate */
void	Population::setMutationRate(double r)
{
	if(r>=0 && r<=1) mutation_rate = r;
}

/* Set the selection rate */
void	Population::setSelectionRate(double r)
{
	if(r>=0 && r<=1) selection_rate = r;
}

/* Return the selection rate */
double	Population::getSelectionRate() const
{
	return selection_rate;
}

/* Return the mutation rate */
double	Population::getMutationRate() const
{
	return mutation_rate;
}

/* Return the best fitness for this population */
double	Population::getBestFitness() const
{
	return  fitness_array[0];
}

/* Return the best chromosome */
vector<int> Population::getBestChromosome() const
{
    vector<int> g;g.resize(chromosome_size);
    for(int i=0;i<chromosome_size;i++) g[i]=chromosome[0][i];
	return g;
}

/* Evaluate and return the best fitness for all chromosomes in the population */
double	Population::evaluateBestFitness() 
{
    vector<int> g;g.resize(chromosome_size);
    for(int i=0;i<chromosome_size;i++) g[i]=chromosome[0][i];
	return fitness(g);
}

void	Population::setBest(vector<int> &g,double f)
{
    /*double tf=fitness(g);
	if(tf<fitness_array[0] && fabs(tf-f)>1e-4) 
	{
		return;
	}
    if(g.size()>chromosome_size)
	{
        int *old=new int[chromosome_size];
        for(int i=0;i<chromosome_count;i++)
		{
            for(int j=0;j<chromosome_size;j++) old[j]=chromosome[i][j];
            delete[] chromosome[i];
            chromosome[i]=new int[g.size()];
            for(int j=0;j<g.size();j++) 	chromosome[i][j]=0;
            for(int j=0;j<chromosome_size;j++) chromosome[i][j]=old[j];
			delete[] children[i];
			children[i]=new int[g.size()];
		}
        chromosome_size=g.size();
		extern int genome_length;
        genome_length=chromosome_size;
		delete[] old;
	}
    int pos=0;
    for(int i=0;i<chromosome_size;i++)
	{
        chromosome[pos][i]=g[i];
	}
    fitness_array[pos]=f;*/
}

void	Population::setChromosome(int pos,vector<int> &g,double f,int k)
{
    /*
	double tf=fitness(g);
	if(tf<fitness_array[pos] && fabs(tf-f)>1e-4) return;
    if(g.size()>chromosome_size)
	{
        int *old=new int[chromosome_size];
        for(int i=0;i<chromosome_count;i++)
		{
            for(int j=0;j<chromosome_size;j++) old[j]=chromosome[i][j];
            delete[] chromosome[i];
            chromosome[i]=new int[g.size()];
            for(int j=0;j<g.size();j++) chromosome[i][j]=0;
			int ipos=0;
			for(int l=0;l<k;l++)
			{
                for(int j=0;j<chromosome_size/k;j++)
				{
                    chromosome[i][l*g.size()/k+j]=old[ipos];
					ipos++;
				}
			}
			delete[] children[i];
			children[i]=new int[g.size()];
		}
        chromosome_size=g.size();
		extern int genome_length;
        genome_length=chromosome_size;
		delete[] old;
	}
    for(int i=0;i<chromosome_size;i++)
	{
        chromosome[pos][i]=g[i];
	}
    fitness_array[pos]=f;*/
}

void	Population::setChromosome(int pos,vector<int> &g,double f)
{
    /*
	double tf=fitness(g);
	if(tf<fitness_array[pos] && fabs(tf-f)>1e-4) return;
    if(g.size()>chromosome_size)
	{
        int *old=new int[chromosome_size];
        for(int i=0;i<chromosome_count;i++)
		{
            for(int j=0;j<chromosome_size;j++) old[j]=chromosome[i][j];
            delete[] chromosome[i];
            chromosome[i]=new int[g.size()];
            for(int j=0;j<g.size();j++) chromosome[i][j]=0;
            for(int j=0;j<chromosome_size;j++) chromosome[i][j]=old[j];
			delete[] children[i];
			children[i]=new int[g.size()];
		}
        chromosome_size=g.size();
		extern int genome_length;
        genome_length=chromosome_size;
		delete[] old;
	}
    for(int i=0;i<chromosome_size;i++)
	{
        chromosome[pos][i]=g[i];
	}
    fitness_array[pos]=f;*/
}

void	Population::getChromosome(int pos,vector<int> &g)
{
    g.resize(chromosome_size);
    for(int i=0;i<g.size();i++) g[i]=chromosome[pos][i];
}


/* Destructor */
Population::~Population()
{
    for(int i=0;i<chromosome_count;i++)
	{
		delete[] children[i];
        delete[] chromosome[i];
	}
    delete[] chromosome;
	delete[] children;
	delete[] fitness_array;
}
