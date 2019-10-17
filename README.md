# OS 460 Repo

Programming Assignments:

PA1: Recreate Bridger Canyon Drive Construction with Threads

# TESTING PA1

1. Run with NUMCARS = 5, MAXCARS = 1:
```
  Num Cars: 5
  Max Cars: 1

  RANDOM SEED: 1
  [Car #3] Arrived at One-Way

  [Car #3] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bridger
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 0
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 0
  [INFO] Direction of last car: To Bridger
  [INFO] Car direction streak: 1
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bridger
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 0
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 0
  [INFO] Direction of last car: To Bridger
  [INFO] Car direction streak: 1
  ------------------------------------------------------

  [Car #3] Traversing over 1sec(s)
  [Car #4] Arrived at One-Way

  [Car #4] Waiting to go To Bozeman. Road is full.
  [Car #2] Arrived at One-Way
  [Car #1] Arrived at One-Way
  [Car #0] Arrived at One-Way
  [Car #3] Exiting Bridger One-Way Heading To Bridger

  [Car #2] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 3
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 1
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 3
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 1
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------

  [Car #2] Traversing over 1sec(s)

  [Car #1] Waiting to go To Bozeman. Road is full.

  [Car #0] Waiting to go To Bozeman. Road is full.
  [Car #2] Exiting Bridger One-Way Heading To Bozeman

  [Car #1] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 2
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 2
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 2
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 2
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 2
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 2
  ------------------------------------------------------

  [Car #1] Traversing over 1sec(s)

  [Car #4] Waiting to go To Bozeman. Road is full.

  [Car #0] Waiting to go To Bozeman. Road is full.
  [Car #1] Exiting Bridger One-Way Heading To Bozeman
  [Car #0] Detected starvation, preempting

  [Car #0] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 1
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 3
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 1
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 3
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------

  [Car #0] Traversing over 1sec(s)

  [Car #4] Waiting to go To Bozeman. Road is full.

  [Car #4] Waiting to go To Bozeman. Road is full.
  [Car #0] Exiting Bridger One-Way Heading To Bozeman

  [Car #4] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 0
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 4
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 2
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 0
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 4
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 2
  ------------------------------------------------------

  [Car #4] Traversing over 1sec(s)
  [Car #4] Exiting Bridger One-Way Heading To Bozeman
```

# Testing with NUMCARS = 5 and MAXCARS = 3

```
  Num Cars: 5
  Max Cars: 3

  RANDOM SEED: 1
  [Car #3] Arrived at One-Way

  [Car #3] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bridger
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 0
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 0
  [INFO] Direction of last car: To Bridger
  [INFO] Car direction streak: 1
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bridger
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 0
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 0
  [INFO] Direction of last car: To Bridger
  [INFO] Car direction streak: 1
  ------------------------------------------------------

  [Car #3] Traversing over 1sec(s)
  [Car #4] Arrived at One-Way

  [Car #4] Waiting to go To Bozeman.
  [Car #2] Arrived at One-Way
  [Car #1] Arrived at One-Way
  [Car #3] Exiting Bridger One-Way Heading To Bridger

  [Car #2] Waiting to go To Bozeman.
  [Car #0] Arrived at One-Way

  [Car #1] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 3
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 1
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 3
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 1
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------

  [Car #1] Traversing over 1sec(s)

  [Car #4] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 2
  [INFO] Cars on One-way: 2
  [INFO] Cars passed in total: 1
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 2
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 2
  [INFO] Cars on One-way: 2
  [INFO] Cars passed in total: 1
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 2
  ------------------------------------------------------

  [Car #4] Traversing over 1sec(s)
  [Car #0] Detected starvation, preempting
  [Car #2] Detected starvation, preempting
  [Car #1] Exiting Bridger One-Way Heading To Bozeman
  [Car #4] Exiting Bridger One-Way Heading To Bozeman

  [Car #0] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 1
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 3
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 1
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 3
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------

  [Car #0] Traversing over 1sec(s)
  [Car #0] Exiting Bridger One-Way Heading To Bozeman

  [Car #2] Can proceed

  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 0
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 4
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------


  STATE-------------------------------------------------
  [INFO] Direction of One-Way: To Bozeman
  [INFO] Cars waiting to go to Bridger: 0
  [INFO] Cars waiting to go to Bozeman: 0
  [INFO] Cars on One-way: 1
  [INFO] Cars passed in total: 4
  [INFO] Direction of last car: To Bozeman
  [INFO] Car direction streak: 1
  ------------------------------------------------------

  [Car #2] Traversing over 1sec(s)
  [Car #2] Exiting Bridger One-Way Heading To Bozeman
```
