#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

int main (void){
	const gsl_rng_type * T;
	gsl_rng * r;
        int i, n = 10;
	double sigma = 0.25; 

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	/*
	 *      *  Print n random variates chosen from the gaussian distribution
	 *           *  with mean zero and standard deviation sigma.
	 *                */
	for (i = 0; i < n; i++) 
	{
		double x = gsl_ran_gaussian(r, sigma);
		printf (" %f", x);
	}
	printf ("\n");
	
	gsl_rng_free (r);
	return 0;
}
