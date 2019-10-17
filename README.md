# OS 460 Repo

Programming Assignments:

1. Recreate Bridger Canyon Drive Construction with Threads

## TODO (PA1):
1. Random time to arrive at one-way
2. Starvation detection and handling
  - Create a variable, combo, that increments when the previous car to go is the same
    direction as the current
  - Set to 0 when a car of opposite direction enters
  - When a waiting car detects that combo is above a certain value, it switches
    the direction of the one-way

### Done (PA1):
1. Get C up and running and ready to compile
2. Include pthreads.h
3. Process commandline args for NUMCARS and MAXCARS (Possibly verbosity and random seed)
4. Struct created for each thread representing input arguments
5. State represented as global struct instance
6. Printing of state
7. Implement direction
8. Improve printing of state
