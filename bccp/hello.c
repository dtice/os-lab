#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

enum direction{TO_BRIDGER, TO_BOZEMAN};
const char *directions[] = {"To Bridger", "To Bozeman"};

typedef struct
{
	enum direction dir;
	//pthread_t *tid;
	int tid;

} car;

typedef struct
{
	int cars_on_oneway;
	enum direction dir;
	int to_bridger;
	int to_bozeman;
	int cars_passed;
} sim_state;

int MAXCARS;
int NUMCARS;

pthread_mutex_t lock;
sim_state state;

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

void ArriveBridgerOneWay(void *c)
{
	car *carAd = (car*)c;
	int tid = carAd->tid;
	printf("[Car #%d] Arrived at One-Way\n", tid);
	switch((int)carAd->dir)
	{
		case 0:
			state.to_bridger++;
			break;
		case 1:
			state.to_bozeman++;
			break;
	}
	int can_proceed = 0;
	int i = 0;
	printf("\nCars waiting to go to Bridger: %d\n", state.to_bridger);
	printf("Cars waiting to go to Bozeman: %d\n\n", state.to_bozeman);

	while(can_proceed != 1)
	{
		int err = pthread_mutex_lock(&lock);
		if(err != 0)
		{
			printf("error obtaining mutex lock: %d\n", err);
			return;
		}
		// Obtained lock
		if(state.cars_on_oneway < MAXCARS)
		{
			printf("\n[Car #%d] Can proceed\n", tid);
			can_proceed = 1;
			state.cars_on_oneway++;
			switch((int)carAd->dir)
			{
				case 0:
					state.to_bridger--;
					break;
				case 1:
					state.to_bozeman--;
					break;
			}
		}
		else
		{
			if(i % 9000000 == 0)
			{
				printf("\n[Car #%d] Waiting. Road is full (%d/%d)\n", tid, state.cars_on_oneway, MAXCARS);
				printf("\nCars waiting to go to Bridger: %d\n", state.to_bridger);
				printf("Cars waiting to go to Bozeman: %d\n\n", state.to_bozeman);
			}
		}
		pthread_mutex_unlock(&lock);
		i++;
	}
}

void OnBridgerOneWay(void *c)
{
	car *carAd = (car *)c;
	int tid = carAd->tid;
	printf("[Car #%d] On Bridger One-Way Heading %s\n", tid, directions[carAd->dir]);
	printf("\nCars on One-way: %d\n\n", state.cars_on_oneway);
	//int randSec = floor(5 * rand() / ((double)RAND_MAX));
	//sleep(randSec);
	sleep(1);
	int err = pthread_mutex_lock(&lock);
	if(err != 0)
	{
		printf("error obtaining mutex lock\n");
		return;
	}
	pthread_mutex_unlock(&lock);
}

void ExitBridgerOneWay(void *c)
{
	car *carAd = (car *)c;
	int tid = carAd->tid;
	printf("[Car #%d] Exiting Bridger One-Way Heading %s\n", tid, directions[carAd->dir]);
	int err = pthread_mutex_lock(&lock);
	if(err != 0)
	{
		printf("error obtaining mutex lock\n");
		return;
	}
	state.cars_on_oneway--;
	state.cars_passed++;
	printf("\nCars on One-way: %d\n", state.cars_on_oneway);
	printf("Cars passed in total: %d\n\n", state.cars_passed);
	pthread_mutex_unlock(&lock);
}

void *OneVehicle(void *c)
{
	car *car_addr = (car*)c;
	int tid = car_addr->tid;
	//printf("\nBEGIN THREAD #%d-----\n", car_addr->tid);
	printf("[Car #%d] Heading %s\n", tid, directions[car_addr->dir]);
	ArriveBridgerOneWay(c);
	OnBridgerOneWay(c);
	ExitBridgerOneWay(c);
	//printf("END THREAD #%d-----\n", car_addr->tid);
}

int main(int argc, char **argv) 
{
	int err = pthread_mutex_init(&lock, NULL);
	if(err != 0)
	{
		printf("\n Mutex Lock Failed: %d\n", err);
		return 1;
	}
	int* parsed_args = parse_args(argc, argv);

	// Initialize number of cars to be simulated
	NUMCARS = parsed_args[0]; 

	// Initialize MAXCARS and semaphore value
	MAXCARS = parsed_args[1];

	// printf("Max Cars: %d\nNum Cars: %d\n", MAXCARS, NUMCARS);

	pthread_t *cars;

	cars = (pthread_t *)malloc(NUMCARS * sizeof(pthread_t )); //Initialize thread pointers

	car carArray[NUMCARS]; // Array for car structs

	srand(time(0));	

	for(int i = 0; i < NUMCARS; i++)
	{
		//Initialize NUMCARS with randomized directions
		int randBit = rand() % 2;

		enum direction dir;

		if(randBit == 0)
		{
			dir = TO_BRIDGER;
		}
		else
		{
			dir = TO_BOZEMAN;
		}

		car *curCar = malloc(sizeof(*curCar));
		curCar->dir = dir;
		//curCar->tid = &cars[i];
		curCar->tid = i;
		carArray[i] = *curCar;
		pthread_create(&cars[i], NULL, OneVehicle, (void *)&carArray[i]);
		free(curCar);
	}

	for(int i = 0; i < NUMCARS; i++)
	{
		pthread_join(cars[i], NULL);
	}

	pthread_mutex_destroy(&lock);

	return 0;
}
