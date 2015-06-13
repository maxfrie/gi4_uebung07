#include "init_matrix.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAXVALUE    2.0

int generate_empty_matrix(double ***A, double **b, unsigned int N)
{
	unsigned int iCnt;

	if ((A == NULL) || (b == NULL))	/* No memory allocated */
		return -1;	/* Error */

	*A = (double **)malloc(N * sizeof(double *));

	if (*A == NULL)
		return -2;	/* Error */

	**A = (double *)malloc(N * N * sizeof(double));

	if (**A == NULL) {
		free(*A);	/* Clean up */
		*A = NULL;
		return -2;	/* Error */
	}

	for (iCnt = 1; iCnt < N; iCnt++) {	/* Assign pointers in the first "real index"; Value from 1 to N (0 yet set, value N means N+1) */
		(*A)[iCnt] = &((*A)[0][iCnt * N]);
	}

	memset(**A, 0, N * N * sizeof(double));	/* Fill matrix values with 0 */

	*b = (double *)malloc(N * sizeof(double));
	if (*b == NULL) {
		free(**A);
		free(*A);
		*A = NULL;
		*b = NULL;
		return -2;	/* Error */
	}

	memset(*b, 0, N * sizeof(double));	/* Fill vector values with 0 */

	return 0;
}

void clean_matrix(double ***A)
{
	if (A != NULL) {
		if (*A != NULL) {
			if (**A != NULL)
				free(**A);
			free(*A);
			*A = NULL;
		}
	}
}

/* frees the (result) vector */
void clean_vector(double **b)
{
	if ((b != NULL) && (*b != NULL)) {
		free(*b);
		*b = NULL;
	}
}

int generate_base_matrix(double ***A, double **b, unsigned int N)
{
	int errorcode;

	errorcode = generate_empty_matrix(A, b, N);

	if (errorcode < 0) {	/* Forward the errorcode */
		return errorcode;
	} else {
		unsigned int i;

		for (i = 0; i < N; i++) {
			(*b)[i] = 0.0;
			(*A)[i][i] = 1.0;	/* Fill diag of identity matrix */
		}

		return 0;
	}
}

int init_matrix(double ***A, double **b, unsigned int N)
{
	unsigned int j, i, errorcode;

	errorcode = generate_empty_matrix(A, b, N);
	if (errorcode < 0)	/* forward errorcode */
		return errorcode;

	srand((unsigned)time(NULL));	/* init random number generator */

	/* 
	 * initialize the system of linear equations
	 * the result vector is one
	 */
	for (i = 0; i < N; i++) {
		double sum = 0.0;
		(*b)[i] = 0.0f;

		for (j = 0; j < N; j++) {
			if (i != j) {
				double c =
				    ((double)rand()) / ((double)RAND_MAX) *
				    MAXVALUE;
				sum += fabs(c);
				(*A)[i][j] = c;
				(*b)[i] += c;
			}
		}
		/*
		 * The Jacobi method will always converge if the matrix A is strictly or irreducibly diagonally dominant. 
		 * Strict row diagonal dominance means that for each row, the absolute value of the diagonal term is 
		 * greater than the sum of absolute values of other terms: |A[i][i]| > Sum |A[i][j]| with (i != j)
		 */
		(*A)[i][i] = sum + 2.0;
		(*b)[i] += sum + 2.0;
	}

	return 0;
}
