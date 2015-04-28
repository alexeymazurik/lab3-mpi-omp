#include <omp.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MATRIX_SIZE 10000
#define THREADS_NUM 4

void init(int ** matrix, int size) {
	int i, j;
	for ( i = 0; i < size; ++i ) {
		for ( j = 0; j < size; ++j ) { 
			matrix[i][j] = rand() % 10; 
		}
	}
}

void transpose(int ** matrix, int size) {
	int i, j, temp;

    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size / 2; ++j) {
        	temp = matrix[j][i];
        	matrix[i][j] = matrix[i][size -1 - j];
        	matrix[i][size - 1 - j] = temp;
        }
    }
}  

void omp_transpose(int ** matrix, int size) {
	int i, j, temp;

#pragma omp parallel for private(i, j, temp) shared(matrix)

    for (i = 0; i < size - 1; ++i) {
        for (j = i + 1; j < size / 2; ++j) {
        	temp = matrix[j][i];
        	matrix[i][j] = matrix[i][size -1 - j];
        	matrix[i][size - 1 - j] = temp;
        }
    }
}  

void print(int ** matrix, int size) {
	int i, j;
	for ( i = 0; i < size; ++i ) {
		for ( j = 0; j < size; ++j ) { 
			printf("%d    ", matrix[i][j]); 
		}
		printf("\n\n");
	}
	printf("\n");
}

int main(int argc, char const *argv[]) { 
 	int i;
 	clock_t tmp; 
	double time_elapsed;

 	int** matrix = (int**) malloc( sizeof(int*) * MATRIX_SIZE );
 	for ( i = 0; i < MATRIX_SIZE; ++i ) { 
 		matrix[i] = (int*) malloc( sizeof(int) * MATRIX_SIZE ); 
 	}

 	srand(time(NULL));
 	init(matrix, MATRIX_SIZE);

	tmp = clock();
 	transpose(matrix, MATRIX_SIZE);
 	tmp = clock() - tmp;
    time_elapsed = ( (double) tmp ) / CLOCKS_PER_SEC;; 
	printf ("%f\n", time_elapsed ); 

	time_elapsed = omp_get_wtime();
	omp_transpose(matrix, MATRIX_SIZE);
	time_elapsed = omp_get_wtime() - time_elapsed;
	printf ("%f\n", time_elapsed ); 

 	return 0;
 } 
