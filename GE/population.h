# ifndef __POPULATION__H
# include <GE/program.h>
class Population
{
	private:
		int	**children;
        int	**chromosome;
		double *fitness_array;
		double	mutation_rate,selection_rate;
        int	chromosome_count;
        int	chromosome_size;
		int	generation;
		Program	*program;

		double 	fitness(vector<int> &g);
		void	crossover();
		void	mutate();
		void	calcFitnessArray();
		int	elitism;
	public:
		Population(int gcount,int gsize,Program *p);
		void	setElitism(int s);
        int     getGeneration() const;
        int     getCount() const;
        int     getSize() const;
		void	nextGeneration();
		void	setMutationRate(double r);
		void	setSelectionRate(double r);
		double	getSelectionRate() const;
		double	getMutationRate() const;
		double	getBestFitness() const;
		double	evaluateBestFitness();
        vector<int> getBestChromosome() const;
		void	reset();
		void	setBest(vector<int> &g,double f);
        void	getChromosome(int pos,vector<int> &genome);
        void	setChromosome(int pos,vector<int> &genome,double f);
        void	setChromosome(int pos,vector<int> &genome,double f,int k);
		void	select();
		void	localSearch(int x);
		~Population();
		
};
# define __POPULATION__H
# endif
