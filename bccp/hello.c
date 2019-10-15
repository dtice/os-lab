#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

enum direction{TO_BRIDGER, TO_BOZEMAN};

int MAXCARS;
int NUMCARS;

pthread_mutex_t lock;

int *parse_args(int argc, char **argv)
{
	int opt;
	static int parsed_args[2];
	while((opt = getopt(argc, argv, ":c:m:")) != -1)
	{
		switch(opt)
		{
			case 'c':
				parsed_args[0] = atoi(optarg);
				break;
			case 'm':
				parsed_args[1] = atoi(optarg);
				break;
			case '?':
				printf("Unknown argument: %c\n\nUsage: bccp -m MAXCARS -c NUMCARS\n", optopt);
				exit(1);
		}
	}
	return parsed_args;
}

void *OneVehicle()
{
	int dir = 0;
	switch(dir)
	{
		case 0:
			printf("\n Traversing One-Way To Bridger\n");
			break;
		case 1:
			printf("\n Traversing One-Way to Bozeman\n");
			break;
	}
}

int main(int argc, char **argv) 
{
	int* parsed_args = parse_args(argc, argv);

	NUMCARS = parsed_args[0];
	MAXCARS = parsed_args[1];

	enum direction dir = TO_BOZEMAN;

	printf("Max Cars: %d\nNum Cars: %d\n", MAXCARS, NUMCARS);

	pthread_t *cars;

	cars = (pthread_t *)malloc(NUMCARS * sizeof(pthread_t ));

	for(int i = 0; i < NUMCARS; i++)
	{
		pthread_create(&cars[i], NULL, OneVehicle, NULL);
	}

	for(int i = 0; i < NUMCARS; i++)
	{
		pthread_join(cars[i], NULL);
	}

	return 0;
}
