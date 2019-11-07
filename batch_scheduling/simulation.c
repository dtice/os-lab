#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

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

double * randNums(int n) 
{
	const gsl_rng_type * T;
	gsl_rng * r;

	double sigma = 0.25;

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	static double *randNums;
	randNums = (double*)malloc(n*sizeof(double));

	for (int i = 0; i < n; i++)
	{
		double x = gsl_ran_gaussian(r, sigma);
		randNums[i] = x;
	}

	gsl_rng_free(r);
	return randNums;
}

int main(int argc, char *argv[])
{
	double *randNumArray;
	int numRands = 10;
	randNumArray = randNums(numRands);
	for (int i = 0; i < numRands - 1; i++)
	{
		printf("Random number #%d: %f\n", i, randNumArray[i]);
	}
	return 0;
}
