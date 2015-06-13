/* 
 * Errorcodes in all integer functions:
 * 
 *  -1: Given pointer is 0 (can't work on it)
 *  -2: Allocation error
 * 
*/

#ifndef __INIT_MATRIX_H__
#define __INIT_MATRIX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Creates identity matrix (NxN) and an empty result vector
 * simply solveable
 * 
 *  *********
 *  * 1   0 *
 *  *   1   *
 *  * 0   1 *
 *  *********                 
*/
	int generate_base_matrix(double ***A, double **b, unsigned int N);

/* Creates empty NxN matrix and empty result vector */
	int generate_empty_matrix(double ***A, double **b, unsigned int N);

/* creates a solveable system of linear equations */
	int init_matrix(double ***A, double **b, unsigned int N);

/* frees all pointers below upper level of A */
	void clean_matrix(double ***A);

/* frees the (result) vector */
	void clean_vector(double **b);

#ifdef __cplusplus
}
#endif
#endif
