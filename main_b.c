#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include "init_matrix.h"

#define MATRIX_SIZE (1024)
#define MAX_THREADS 4

void euklid_dist(int size, double* sum, double* X, double* X_old);

pthread_barrier_t bstart, bend;

double **A;
double *b;
double *X;
double *X_old;
double *temp;

struct thread_arg {
	unsigned int start;
	unsigned int end;
};

void* thread_func(void* void_arg)
{
	struct thread_arg args = *((struct thread_arg*) void_arg);
	unsigned int i, j;
	double sum;	
	
	while(1)
	{
	 /* calculate next iteration step of X */
         pthread_barrier_wait(&bstart);
	       for (i = args.start; i < args.end; i++) {
                        sum = 0.0; // initialisiere Summe mit 0
                        for (j = 0; j < MATRIX_SIZE; j++) {
                                 if (i != j) {
                                        sum += A[i][j] * X_old[j];
                                }
                        }

                        X[i] = (b[i] - sum) / A[i][i];
                }
	pthread_barrier_wait(&bend);
	}

	return NULL;
}	

int main(int argc, char **argv)
{
	unsigned int i, j;
	unsigned int iterations = 0;
	double error, max = 0.0;
	struct timeval start, end;

	double sum;
	struct thread_arg thr_arg[MAX_THREADS];
	pthread_t threads[MAX_THREADS];

	printf("\nInitialize system of linear equations...\n");
	/* allocate memory for the system of linear equations */
	init_matrix(&A, &b, MATRIX_SIZE);
	X = (double *)malloc(sizeof(double) * MATRIX_SIZE);
	X_old = (double *)malloc(sizeof(double) * MATRIX_SIZE);

	/* a "random" solution vector */
	for (i = 0; i < MATRIX_SIZE; i++) {
		X[i] = ((double)rand()) / ((double)RAND_MAX) * 10.0;
		X_old[i] = 0.0;
	}

	printf("Start Jacobi method...\n");

	gettimeofday(&start, NULL);

	/* TODO: Hier muss die Aufgabe geloest werden */
	pthread_barrier_init(&bstart, NULL, MAX_THREADS+1);
	pthread_barrier_init(&bend, NULL, MAX_THREADS+1);

	for (i = 0; i < MAX_THREADS; i++) {
		thr_arg[i].start = i * (MATRIX_SIZE/MAX_THREADS);
		thr_arg[i].end = (1+i) * (MATRIX_SIZE/MAX_THREADS);

		pthread_create(&threads[i], NULL, thread_func, &thr_arg[i]);
	}

	while(1)
	{
		// Start threads
		pthread_barrier_wait(&bstart);
		// end threads for critical area
		pthread_barrier_wait(&bend);
		iterations++;
		/* Berechne Euklidischen Abstand */
		sum = 0.0;
		euklid_dist(MATRIX_SIZE, &sum, X, X_old);

		/* Prüfe Abbruchbedingung */
		if ( sum < 0.0000001 * MATRIX_SIZE)
			break;
	
		/* swap pointers to save X_old */
		for( i = 0; i < MATRIX_SIZE; i++) {
			X_old[i] = X[i];
		}

	}
	
	/* cancel used threads */	
	for (i = 0; i < MAX_THREADS; i++) {
		pthread_cancel(threads[i]);
	}	

	/* destroy barriers */
	pthread_barrier_destroy(&bstart);
	pthread_barrier_destroy(&bend);

	gettimeofday(&end, NULL);

	if (MATRIX_SIZE < 16) {
		printf("Print the solution...\n");
		/* print solution */
		for (i = 0; i < MATRIX_SIZE; i++) {
			for (j = 0; j < MATRIX_SIZE; j++)
				printf("%8.2f\t", A[i][j]);
			printf("*\t%8.2f\t=\t%8.2f\n", X[i], b[i]);
		}
	}

	printf("Check the result...\n");
	/* 
	 * check the result 
	 * X[i] have to be 1
	 */
	for (i = 0; i < MATRIX_SIZE; i++) {
		error = fabs(X[i] - 1.0f);

		if (max < error)
			max = error;
		if (error > 0.01f)
			printf("Result is on position %d wrong (%f != 1.0)\n",
			       i, X[i]);
	}
	printf("maximal error is %f\n", max);

	printf("\nmatrix size: %d x %d\n", MATRIX_SIZE, MATRIX_SIZE);
	printf("number of iterations: %d\n", iterations);
	printf("Time : %lf sec\n",
	       (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec -
							      start.tv_usec) /
	       1000000.0);

	/* frees the allocated memory */
	free(X_old);
	free(X);
	clean_matrix(&A);
	clean_vector(&b);

	return 0;
}
