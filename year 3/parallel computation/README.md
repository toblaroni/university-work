# Parallel Computing
This module covered shared memory parallelism, distributed memory parallelism and general-purpose graphics processing unit. 

### Coursework 1
This was the shared memory parallelism project. In this coursework we were given a serial solution to Conway's Game of Life and we had to use OpenMP to parallelise each function in [coursework 1/cwk1.c](./coursework%201/cwk1.c).
I used locks to initialise the grid of cells where each row gets its own lock. 
For the modified rules I used the red-black pattern to check the neighbor count of each cell and update the grid.

### Coursework 2
This was the distributed memory parallelism project. We had to use MPI to calculate the mean and variance of a dataset, testing the time it took in parallel vs serial and the speed-up for varying numbers of procedures. We also had to implement our own version of MPI's broadcast function which uses a binary tree to send the data across all procedures. 

### Coursework 3
This was a small OpenCL program that applied a heat equation to a small grid of values using a simple kernel. 
