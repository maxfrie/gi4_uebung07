#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include "init_matrix.h"

#define MATRIX_SIZE (1024)
#define MAX_THREADS 2

pthread_barrier_t barrier;

typedef struct {
  double** A;
  double* b;
  double* X;
  double* X_old;
  unsigned int start;
  unsigned int end;
} thread_param;

void* thread_func(void* arg)
{
  unsigned int i, j, iterations;
  unsigned int start = ((thread_param*) arg)->start;
  unsigned int end = ((thread_param*) arg)->end;
  double** A = ((thread_param*) arg)->A;
  double* b = ((thread_param*) arg)->b;
  double* X = ((thread_param*) arg)->X;
  double* X_old = ((thread_param*) arg)->X_old;
  double* temp = NULL;
  double xi, norm;
	
  /* Jacobi iterations */
  iterations = 0;

  while (1) {
    iterations++;
    temp = X_old;
    X_old = X;
    X = temp;
    
    for (i = start; i < end; i++) {
      for (j = 0, xi = 0.0; j < i; j++) {
        xi += A[i][j] * X_old[j];
      }
    
      for (j = i + 1; j < MATRIX_SIZE; j++) {
        xi += A[i][j] * X_old[j];
      }

      X[i] = (b[i] - xi) / A[i][i];
    }
    	
    pthread_barrier_wait(&barrier);
    
    /* calculate Euclidean norm between X_old and X */
    if (iterations % 500 == 0) {
      norm = 0.0;
    
      for (i = 0; i < MATRIX_SIZE; i++) {
        norm += (X_old[i] - X[i]) * (X_old[i] - X[i]);
      }
    
      /* check break condition */
      norm /= (double)MATRIX_SIZE;
    
      if (norm < 0.0000001) {
        break;
      }
    }
  }
  
  return (void*) iterations;
}

int main(int argc, char **argv)
{
  unsigned int i, j;
  unsigned int iterations;
  double error, max;
  struct timeval start, end;
  double** A = NULL;
  double* b = NULL;
  double* X = NULL;
  double* X_old = NULL;
  void* ret = NULL;

  pthread_t threads[MAX_THREADS];
  thread_param thr_arg[MAX_THREADS];
 
 /* initialize and allocate */ 
  printf("\nInitialize system of linear equations...\n");
  init_matrix(&A, &b, MATRIX_SIZE);
  X = (double*) malloc(sizeof(double) * MATRIX_SIZE);
  X_old = (double*) malloc(sizeof(double) * MATRIX_SIZE);
  
  /* get a "random" solution vector */
  for (i = 0; i < MATRIX_SIZE; i++) {
    X[i] = ((double) rand()) / ((double) RAND_MAX) * 10.0;
    X_old[i] = 0.0;
  }
  
  printf("Start Jacobi method...\n");
  gettimeofday(&start, NULL);

/* ################################################################################## */
  pthread_barrier_init(&barrier, NULL, MAX_THREADS);

  for (i = 0; i < MAX_THREADS; i++) {
    /* initialize arguments of the thread */
    thr_arg[i].A = A;
    thr_arg[i].b = b;
    thr_arg[i].X = X;
    thr_arg[i].X_old = X_old;
    thr_arg[i].start = i * (MATRIX_SIZE / MAX_THREADS);
    thr_arg[i].end = (i+1) * (MATRIX_SIZE / MAX_THREADS);

    /* create current thread */
    pthread_create(&(threads[i]), NULL, thread_func, &(thr_arg[i]));		
  }

  /* wait until all threads have terminated */
  for (i = 0; i < MAX_THREADS; i++) {
    pthread_join(threads[i], &ret);
  }

  iterations = (unsigned int) ret;
  pthread_barrier_destroy(&barrier);
/* ################################################################################## */
  gettimeofday(&end, NULL);
  
  if (MATRIX_SIZE < 16) {
    printf("Print the solution...\n");

    /* print solution */
    for (i = 0; i < MATRIX_SIZE; i++) {
      for (j = 0; j < MATRIX_SIZE; j++) {
        printf("%8.2f\t", A[i][j]);
      }

      printf("*\t%8.2f\t=\t%8.2f\n", X[i], b[i]);
    }
  }
  
  /* 
   * check result
   * X[i] has to be 1.0
   */
  printf("Check the result...\n");
  max = 0.0;

  for (i = 0; i < MATRIX_SIZE; i++) {
    error = fabs(X[i] - 1.0f);
  
    if (max < error) {
      max = error;
    }

    if (error > 0.01f) {
      printf("Result is on position %d wrong (%f != 1.0)\n", i, X[i]);
    }
  }

  printf("maximal error is %f\n", max);
  
  printf("\nmatrix size: %d x %d\n", MATRIX_SIZE, MATRIX_SIZE);
  printf("number of iterations: %d\n", iterations);
  printf("Time : %lf sec\n", (double) (end.tv_sec - start.tv_sec) + (double) (end.tv_usec - start.tv_usec) / 1000000.0);
  
  /* free allocated memory */
  free(X_old);
  free(X);
  clean_matrix(&A);
  clean_vector(&b);
  
  return 0;
}

/* eof */

