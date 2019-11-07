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

int randNums() 
{
	const gsl_rng_type * T;
	gsl_rng * r;

	int i, n = 10;
	double sigma = 0.25;

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	for (i = 0; i < n; i++)
	{
		double x = gsl_ran_gaussian(r, sigma);
		printf(" %f", x);
	}

	printf("\n");
	gsl_rng_free(r);
	return 0;
}

int main(void)
{
	randNums();
	return 0;
}
