To install MPI on Ubuntu:

```
https://jetcracker.wordpress.com/2012/03/01/how-to-install-mpi-in-ubuntu/
```

To compile mpi lab:
```
mpicc mpi_lab3.c -o mpi_lab3 -std=c99
```
To run:
```
mpirun -np 5 ./mpi_lab3
```

To compile OpenMP:
```
gcc lab3omp.c -o ./lab3omp -fopenmp
```
