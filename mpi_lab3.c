#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 9000

void print(int ** matrix, int size) {
  printf("Matrix: \n");
  int i, j;
  for ( i = 0; i < size; ++i ) {
    for ( j = 0; j < size; ++j ) { 
      printf("%d ", matrix[j][i]); 
    }
    printf("\n\n");
  }
  printf("\n");
}


void printChunk(int * matrixChunk, int size) {
  for (int i = 0; i < size; ++i) {
    printf("%d ", matrixChunk[i]); 
  }
  printf("\n");
}

int main(int argc, char *argv[] ) {

  // Init matrix
  int ** matrix = (int**)malloc(sizeof(int*) * MATRIX_SIZE);
  for(int i = 0; i < MATRIX_SIZE; ++i)
  {
    matrix[i] = (int*)malloc(sizeof(int) * MATRIX_SIZE);
  }

  // Init local mas
  int * local_arr = (int*)malloc(sizeof(int) * MATRIX_SIZE);

  // MPI Init
  MPI_Init(NULL, NULL);

  MPI_Status status;

  // Process rank
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);


  clock_t clocks;
  double time_elapsed;

  int i, j;

  int mod = MATRIX_SIZE % (world_size - 1);
  int rangeSize = (MATRIX_SIZE - mod) / (world_size - 1);
  int start = 0;
  int end = 0;

  if (world_rank == 0) {
    
    srand(time(NULL)); 

    printf("MATRIX_SIZE: %d, world_size: %d, mod: %d, rangeSize: %d\n", MATRIX_SIZE, world_size, mod, rangeSize);

    for (i = 0; i < MATRIX_SIZE; i++) {
      for (j = 0; j < MATRIX_SIZE; j++) {
        matrix[i][j] = j;
      }
    }
    printf("Initialized matrix\n");

    // Start clock
    clocks = clock();

    for (i = 1; i < world_size; ++i) {
      start = (i == 1) ? start : end;
      end = start + rangeSize;

      if (i == world_size - 1) {
        end += mod;
      }

      for (j = start; j < end; ++j) {
        // Sending need rows to child processes
        MPI_Send(&matrix[j][0], MATRIX_SIZE + 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      }
    }

  }

  else if (world_rank > 0) {  
    int size = (world_rank == world_size - 1) ? rangeSize + mod : rangeSize;

    for (i = 0; i < size; i++) {

      //Receive row
      MPI_Recv(&local_arr[0], MATRIX_SIZE + 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
      
      
      // Convert row
      for (j = 0; j < MATRIX_SIZE / 2; ++j) {
        int temp = local_arr[j];
        local_arr[j] = local_arr[MATRIX_SIZE - j - 1];
        local_arr[MATRIX_SIZE - 1 - j] = temp;
      }
    
      // Send converted row
      MPI_Send(&local_arr[0], MATRIX_SIZE + 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
  }

  // Receiving converted rows with main process
  if (world_rank == 0) {
    int shift = -1;
    int tmp = rangeSize;
    for (i = 1; i < world_size; ++i) {
      tmp = (i == world_size - 1) ? rangeSize + mod : rangeSize;
      
      for (j = 0; j < tmp; j++) {
        MPI_Recv(&matrix[i + j + shift][0], MATRIX_SIZE + 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);  
      }
      shift++;
    }
    
    // Stop clock
    clocks = clock() - clocks;
    time_elapsed = ( (double) clocks ) / CLOCKS_PER_SEC;
    
    printf ("%f\n", time_elapsed ); 
    printf("Inverted matrix\n");
  }

  MPI_Finalize();
}
