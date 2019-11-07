#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <math.h>

gsl_rng * r;
int t = 0;

struct Entry
{
	/**
	 * 	active = {0, 1}, initialized to 1, set to 0 on termination
	 *
	 * 	A_i = arrival time of process
	 *
	 * 	T_i = total CPU time
	 *
	 * 	R_i = remaining CPU time
	 *
	 * 	TT_i = turnaround time of process (time from arrival to current time)
	 *
	 **/
	int active, A_i, T_i, R_i;
	float TT_i;
};

// Checks every process in table to see if all processes are finished
// Returns 1 if they are, 0 otherwise

void printEntry(struct Entry *e)
{
	printf("| \t%d\t | %d\t | %d\t | %d\t | %.2f\t |\n", e->active, e->A_i, e->T_i, e->R_i, e->TT_i);
}

void printTable(struct Entry *Table, int n)
{
	printf("Process Table:\n\n");
	printf("+------------------------------------------------+\n");
	printf("| # | Active\t | A_i\t | T_i\t | R_i\t | TT_i\t |\n");
	printf("+------------------------------------------------+\n");

	for(int i = 0; i < n - 1; i++)
	{
		printf("| %d ", i);
		printEntry(&Table[i]);
	}
	printf("+------------------------------------------------+\n");
}

int checkTerm(struct Entry *Table, int n)
{
	printf("Checking if all processes are finished...\n");
	int allProcsFinished = 1;
	for (int i = 0; i < n - 1; i++)
	{
		int entryTR = Table[i].R_i;
		if(entryTR > 0){
			allProcsFinished = 0;
		}
	}
	return allProcsFinished;
}

int randGaussian(double sigma)
{

	double x = gsl_ran_gaussian(r, sigma);
	int randInt = abs(floor(x));

	return randInt;
}

// returns an int drawn from a uniform distribution [0, n]
int randUniform(int n)
{
	int randInt = gsl_rng_uniform_int(r, n);
  return randInt;
}

struct Entry * initializeTable(int n, int k, int v)
{
	static struct Entry * Table;
	Table = malloc(sizeof(struct Entry)*n);
	for (int i = 0; i < n - 1; i++)
	{
		int tot_time = randGaussian(v);
		struct Entry newEntry = {0, randUniform(k), tot_time, tot_time, 0.0};
		Table[i] = newEntry;
	}
	return Table;
}

int checkActiveProcesses(struct Entry *Table, int n)
{
	int active = 0;
	for(int i = 0; i < n - 1; i++)
	{
		if(Table[i].active == 1)
		{
			active++;
		} else {
			if(Table[i].A_i <= t)
			{
				Table[i].active = 1;
			}
		}
	}
	return active;
}
/*
 *
 *  Inputs:
 *  	- d : average of distribution of T_i
 *  	- v : std dev of distribution of T_i
 *  	- k : upper bound of A_i
 *		- n : number of processes
 *
 *	Command Line Usage:
 *		- sched_sim d v k n
 *
 */

int main(int argc, char *argv[])
{
	int d, k, n;
	float v;

	// Setup RNG --
	const gsl_rng_type * T;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);
	// --

	// Parse Args --
	char *curArg = "";
	int i = 0;
	printf("%d\n", argc);
	while(curArg != NULL)
	{
		curArg = argv[i];
		switch(i){
			case 1:
				d = atoll(curArg);
				break;
			case 2:
				v = atof(curArg);
				break;
			case 3:
				k = atoll(curArg);
				break;
			case 4:
				n = atoll(curArg);
				break;
		}
		i++;
	}
	// --

	// Initialize Table with n processes --
	struct Entry *Table = initializeTable(n, k, v);
	// --

	// Begin simulation --
	int allProcsFinished = 0;
	while(allProcsFinished != 1) {
		printf("\n\nTime: %d\n", t);
		printTable(Table, n);

		int activeProcesses = 0;
		do {
			activeProcesses = checkActiveProcesses(Table, n);
		} while(activeProcesses < 0);


		// TODO: Schedule active processes

		allProcsFinished = checkTerm(Table, n);
		if(activeProcesses == n - 1)
		{
			allProcsFinished = 1;
		}
		t++;
	}
	// --

	// Free rng
	gsl_rng_free (r);

	return 0;
}
