#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

enum direction{TO_BRIDGER, TO_BOZEMAN};
const char *directions[] = {"To Bridger", "To Bozeman"};

typedef struct
{
	enum direction dir;

} car;

int MAXCARS;
int NUMCARS;

pthread_mutex_t lock;

// cars_on_oneway acts as a semaphore
int cars_on_oneway;

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

void ArriveOneWay(void * direction)
{
	pthread_mutex_lock(&lock);
	int dir = (int)*((enum direction*)direction);
	switch(dir)
	{
		case 0:
			printf("\n Traversing One-Way To Bridger\n");
			sleep(1);
			break;
		case 1:
			printf("\n Traversing One-Way to Bozeman\n");
			sleep(1);
			break;
	}
	pthread_mutex_unlock(&lock);
}

void *OneVehicle(void *c)
{
	//int *argPtr = c;
	car *car_addr = (car*)c;
	car curCar = *car_addr;
	//TODO: Thread ID
	//int tid;
	//printf("\nBEGIN THREAD #%d-----\n", tid);
	printf("Direction: %s\n", directions[curCar.dir]);
	//ArriveBridgerOneWay(direction);
	//OnBridgerOneWay(direction);
	//ExitBridgerOneWay(direction);
	printf("END THREAD-----\n");
}

int main(int argc, char **argv) 
{
	int* parsed_args = parse_args(argc, argv);

	NUMCARS = parsed_args[0]; // Number of cars to simulate
	MAXCARS = parsed_args[1]; // Maximum amount of cars allowed on one-way stretch

	cars_on_oneway = MAXCARS;

	// printf("Max Cars: %d\nNum Cars: %d\n", MAXCARS, NUMCARS);

	pthread_t *cars;

	cars = (pthread_t *)malloc(NUMCARS * sizeof(pthread_t )); //Initialize thread pointers

	car carArray[NUMCARS]; // Array for random direction values of each thread

	srand(time(0));	

	for(int i = 0; i < NUMCARS; i++)
	{
		int randBit = rand() % 2;

		enum direction dir;

		if(randBit == 0)
		{
			dir = TO_BOZEMAN;
		}
		else
		{
			dir = TO_BRIDGER;
		}

		car *curCar = malloc(sizeof(*curCar));
		curCar->dir = dir;
		carArray[i] = *curCar;
		pthread_create(&cars[i], NULL, OneVehicle, &carArray[i]);
	}

	for(int i = 0; i < NUMCARS; i++)
	{
		pthread_join(cars[i], NULL);
	}

	return 0;
}
