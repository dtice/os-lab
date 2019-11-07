#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <limits.h>

enum direction{TO_BRIDGER, TO_BOZEMAN};
const char *directions[] = {"To Bridger", "To Bozeman"};

/*
	Queue code credit: GeeksForGeeks (https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/)
*/

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

// END Queue code
typedef struct
{
	enum direction dir;
	//pthread_t *tid;
	int tid;
	int time_to_arrive;
} car;

typedef struct
{
	// cars_on_oneway acts as our semaphore
	int cars_on_oneway;

	// combo and last_car help with starvation
	int combo;
	enum direction last_car;


	enum direction dir;
	int to_bridger;
	int to_bozeman;
	int cars_passed;

	struct Queue* to_bridger_queue;
	struct Queue* to_bozeman_queue;

} sim_state;

void print_state(sim_state *state)
{
	printf("\nSTATE-------------------------------------------------\n");
	printf("[INFO] Direction of One-Way: %s\n", directions[state->dir]);
	printf("[INFO] Cars waiting to go to Bridger: %d\n", state->to_bridger);
	printf("[INFO] Cars waiting to go to Bozeman: %d\n", state->to_bozeman);
	printf("[INFO] Cars on One-way: %d\n", state->cars_on_oneway);
	printf("[INFO] Cars passed in total: %d\n", state->cars_passed);
	printf("[INFO] Direction of last car: %s\n", directions[state->last_car]);
	printf("[INFO] Car direction streak: %d", state->combo);
	printf("\n------------------------------------------------------\n\n");
}

// Global argument variables
int MAXCARS;
int NUMCARS;
int VERB;
int RAND_SEED;
int THREAD_WAIT;

pthread_mutex_t mainLock;
pthread_mutex_t onewayLock;

/* The state variable acts as our critical section, lock prevents 2 threads
from occupying it */
sim_state state;

struct Queue* to_bozeman_queue;
struct Queue* to_bridger_queue;




int *parse_unnamed_args(int argc, char **argv)
{
	static int parsed_args[5];
	int i = 1;
	while(argv[i] != NULL)
	{
		int parsed_arg = atoi(argv[i]);
		if(parsed_arg == 0 && i != 3)
		{
			parsed_args[0] = -1;
			return parsed_args;
		}

		parsed_args[i-1] = parsed_arg;
		if(VERB > 0)
		{
			printf("Parsed Arg #%d: %d", i-1, parsed_arg);
		}
		i++;
	}

	return parsed_args;
}

int *parse_named_args(int argc, char **argv)
{
	int opt;
	// -c for NUMCARS, -m for MAXCARS, -v: verbosity flag, -r: random seed
	static int parsed_args[5];
	int i = 0;
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
				//printf("Unknown argument: %c\n\nUsage: bccp -m MAXCARS -c NUMCARS\n", optopt);
				switch(optopt){
					case 'v':
						parsed_args[2] = 1;
						break;
					case 'r':
						parsed_args[3] = 1;
						break;
					default:
						printf("Unknown argument: %c", optopt);
						exit(1);
				}
				break;
		}
	}
	return parsed_args;
}

void ArriveBridgerOneWay(void *c)
{
	car *carAd = (car*)c;
	int tid = carAd->tid;

	// Delay arriving at bridger one way
	if(VERB > 0)
	{
		printf("[Car #%d] Waiting %d to arrive at one-way\n", tid, carAd->time_to_arrive);
	}
	sleep(carAd->time_to_arrive);

	printf("[Car #%d] Arrived at One-Way\n", tid);


	pthread_mutex_lock(&mainLock);
	int front_of_queue;

	switch((int)carAd->dir)
	{
		case 0:
			state.to_bridger++;
			enqueue(state.to_bridger_queue, tid);
			front_of_queue = front(state.to_bridger_queue);
			break;
		case 1:
			state.to_bozeman++;
			enqueue(state.to_bozeman_queue, tid);
			front_of_queue = front(state.to_bozeman_queue);
			break;
	}
	pthread_mutex_unlock(&mainLock);

	if(VERB > 0)
	{
		printf("\n[INFO] Cars waiting to go to Bridger: %d\n", state.to_bridger);
		printf("[INFO] Cars waiting to go to Bozeman: %d\n\n", state.to_bozeman);
	}

	int i = 0;
	int can_proceed = 0;

	/*
		Loop Procedure:
			1. Obtain mutex lock (blocks until)
			2. Check state to see if we can proceed
			3. If we can, but would starve one-way, wait until one-way clear then
				 reverse oneway
			4. If we can, terminate loop after modifying state
			5. Else, GOTO 1
			6. Release mutex lock
	*/

	while(can_proceed != 1)
	{
		int err = pthread_mutex_lock(&mainLock);
		if(err != 0)
		{
			printf("[ERROR] Could not obtain mutex lock: %d\n", err);
			return;
		}

		// Obtained lock

		// Oneway not full
		if(state.cars_on_oneway < MAXCARS)
		{
			// Check starvation detection variable
			if(state.combo >= 2)
			{
				printf("[Car #%d] Detected starvation, preempting\n", tid);
				pthread_mutex_unlock(&mainLock);
				int check = -1;
				while(check != 0)
				{
					pthread_mutex_lock(&onewayLock);
					check = state.cars_on_oneway;
					pthread_mutex_unlock(&onewayLock);
				}
				pthread_mutex_lock(&mainLock);
				if(carAd->dir == TO_BOZEMAN)
				{
					state.dir = TO_BRIDGER;
				}
				else
				{
					state.dir = TO_BOZEMAN;
				}
				state.combo = 0;
				pthread_mutex_unlock(&mainLock);
				continue;

			}
			// Direction of oneway matches current cars direction
			if(state.dir == carAd->dir && tid == front_of_queue)
			{
				printf("\n[Car #%d] Can proceed\n", tid);
				can_proceed = 1;
				pthread_mutex_lock(&onewayLock);
				state.cars_on_oneway++;
				pthread_mutex_unlock(&onewayLock);
				if(state.last_car == carAd->dir){
					state.combo++;
				}

				state.last_car = carAd->dir;

				switch((int)carAd->dir)
				{
					case 0:
					state.to_bridger--;
					dequeue(state.to_bridger_queue);
					break;
					case 1:
					state.to_bozeman--;
					dequeue(state.to_bozeman_queue);
					break;
				}
				print_state(&state);
			}
			// Flip direction if no cars on roadway but state.dir != carAd->dir
			else if(state.cars_on_oneway == 0 && front_of_queue == tid)
			{
				if(VERB > 0)
				{
					printf("\n[Car #%d] Setting flow of oneway to %s\n", tid, directions[(int)carAd->dir]);
				}
				state.dir = carAd->dir;
				if(state.last_car == carAd->dir){
					state.combo++;
				}
				state.last_car = carAd->dir;
				printf("\n[Car #%d] Can proceed\n", tid);
				can_proceed = 1;
				pthread_mutex_lock(&onewayLock);
				state.cars_on_oneway++;
				pthread_mutex_unlock(&onewayLock);
				switch((int)carAd->dir)
				{
					case 0:
						state.to_bridger--;
						break;
					case 1:
						state.to_bozeman--;
						break;
				}
				print_state(&state);
			}
			// Cant go, road not full but wrong direction
			else
			{
				if(i % 9000000 == 0)
				{
					printf("\n[Car #%d] Waiting to go %s.\n", tid, directions[(int)carAd->dir]);
					if(VERB > 1)
					{
						print_state(&state);
					}
				}
			}
		}
		else
		{
			if(i % 9000000 == 0)
			{
				printf("\n[Car #%d] Waiting to go %s. Road is full.\n", tid, directions[(int)carAd->dir]);
			}
		}
		pthread_mutex_unlock(&mainLock);
		i++;
	}
}

void OnBridgerOneWay(void *c)
{
	car *carAd = (car *)c;
	int tid = carAd->tid;
	print_state(&state);

	if(THREAD_WAIT > 0){
		printf("[Car #%d] Traversing over %dsec(s)\n", tid, THREAD_WAIT);
		sleep(1 * THREAD_WAIT);
	}

	int err = pthread_mutex_lock(&mainLock);
	if(err != 0)
	{
		printf("[ERROR] Could not obtain mutex lock\n");
		return;
	}
	if(VERB > 0)
	{
		printf("[Car #%d] On Bridger One-Way Heading %s\n", tid, directions[carAd->dir]);
		printf("\n[INFO] Cars on One-way: %d\n\n", state.cars_on_oneway);
	}
	pthread_mutex_unlock(&mainLock);
}

void ExitBridgerOneWay(void *c)
{
	car *carAd = (car *)c;
	int tid = carAd->tid;
	printf("[Car #%d] Exiting Bridger One-Way Heading %s\n", tid, directions[carAd->dir]);
	int err = pthread_mutex_lock(&mainLock);
	if(err != 0)
	{
		printf("[ERROR] Could not obtain mutex lock\n");
		return;
	}
	pthread_mutex_lock(&onewayLock);
	state.cars_on_oneway--;
	pthread_mutex_unlock(&onewayLock);
	state.cars_passed++;
	if(VERB > 0){
		printf("\n[INFO] Cars on One-way: %d\n", state.cars_on_oneway);
		printf("[INFO] Cars passed in total: %d\n\n", state.cars_passed);
	}
	pthread_mutex_unlock(&mainLock);
}

void *OneVehicle(void *c)
{
	car *car_addr = (car*)c;
	int tid = car_addr->tid;

	if(VERB > 0)
	{
		printf("\nBEGIN THREAD #%d-----\n", car_addr->tid);
		printf("[Car #%d] Heading %s\n", tid, directions[car_addr->dir]);
	}

	ArriveBridgerOneWay(c);
	OnBridgerOneWay(c);
	ExitBridgerOneWay(c);

	if(VERB > 0)
	{
		printf("END THREAD #%d-----\n", car_addr->tid);
	}
}

/*

	Usage: bccp NUMCARS MAXCARS [THREAD_WAIT] [RAND_SEED] [TIME_TO_ARRIVE]

 */
int main(int argc, char **argv)
{
	int err = pthread_mutex_init(&mainLock, NULL);
	err = pthread_mutex_init(&onewayLock, NULL);
	if(err != 0)
	{
		printf("\n [ERROR] Mutex Lock Failed: %d\n", err);
		return 1;
	}
	// Use if you want named arguments
	//int* parsed_args = parse_args(argc, argv);

	int* parsed_args = parse_unnamed_args(argc, argv);

	if(parsed_args[0] == -1)
	{
		printf("Usage: bccp NUMCARS MAXCARS [VERBOSITY] [RAND_SEED] [THREAD_WAIT] \n\nMAXCARS and NUMCARS must be > 0\n\n");
		exit(1);
	}

	// Initialize number of cars to be simulated
	NUMCARS = parsed_args[0];

	// Initialize MAXCARS and semaphore value
	MAXCARS = parsed_args[1];

	VERB = parsed_args[2];

	RAND_SEED = parsed_args[3];

	THREAD_WAIT = parsed_args[4];

	to_bozeman_queue = createQueue(NUMCARS);
	to_bridger_queue = createQueue(NUMCARS);

	state.to_bozeman_queue = to_bozeman_queue;
	state.to_bridger_queue = to_bridger_queue;

	printf("Num Cars: %d\nMax Cars: %d\n\n", NUMCARS, MAXCARS);

	if(VERB > 0)
	{
		printf("VERBOSITY LEVEL: %d\n", VERB);
	}

	pthread_t *cars;

	cars = (pthread_t *)malloc(NUMCARS * sizeof(pthread_t )); //Initialize thread pointers

	car carArray[NUMCARS]; // Array for car structs

	if(RAND_SEED != 0)
	{
		printf("RANDOM SEED: %d\n", RAND_SEED);
		srand(RAND_SEED);
	}
	else
	{
		srand(time(0));
	}

	for(int i = 0; i < NUMCARS; i++)
	{
		//Initialize carArray[NUMCARS] with randomized directions
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
		// random time to arrive at one_way
		curCar->time_to_arrive = rand() % 3;
		carArray[i] = *curCar;

		pthread_create(&cars[i], NULL, OneVehicle, (void *)&carArray[i]);
		free(curCar);
	}

	for(int i = 0; i < NUMCARS; i++)
	{
		pthread_join(cars[i], NULL);
	}

	pthread_mutex_destroy(&mainLock);

	return 0;
}
