/***********************************************************************
 *   Includes                                                          *
 ***********************************************************************/
#include "matrix.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>


/***********************************************************************
 *   Defines                                                           *
 ***********************************************************************/
#define BS 200
#define min(a, b) (((a) > (b)) ? (b) : (a))


/***********************************************************************
 *   Implementation of exported functions                              *
 ***********************************************************************/
extern fpv_t *
matrix_Alloc(uint32_t dimX, uint32_t dimY)
{
	fpv_t *dummy;

	dummy = malloc(sizeof(fpv_t) * dimX * dimY);
	if (dummy == NULL) {
		fprintf(stderr,
		        "ERROR: Failed to malloc %fkB in %s:%i\n",
		        sizeof(fpv_t) * dimX * dimY / 1024.,
		        __func__, __LINE__);
		exit(EXIT_FAILURE);
	}

	printf("Allocated %fkB for a matrix.\n",
	       sizeof(fpv_t) * dimX * dimY / 1024.);

	return dummy;
} /* matrix_Alloc */

extern void
matrix_Free(fpv_t **mat)
{
	if ((mat == NULL) || (*mat == NULL))
		return;

	free(*mat);
	*mat = NULL;

	return;
}

extern double
matrix_MulQuad0(const fpv_t *a,
                const fpv_t *b,
                fpv_t       *c,
                uint32_t    n)
{
	uint32_t i, j, k;
	double   timing;

	timing = -omp_get_wtime();
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			for (k = 0; k < n; k++) {
				c[MATRIX_ACCESS(n, i, j)] += a[MATRIX_ACCESS(n, i, k)]
				                             * b[MATRIX_ACCESS(n, k, j)];
			}
		}
	}
	timing += omp_get_wtime();

	return timing;
} /* matrix_MulQuad0 */

extern double
matrix_MulQuad1(const fpv_t *a,
                const fpv_t *b,
                fpv_t       *c,
                uint32_t    n)
{
	uint32_t i, j, k;
	double   timing;
	fpv_t    tmp;

	timing = -omp_get_wtime();
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			tmp = 0.0;
			for (k = 0; k < n; k++) {
				tmp += a[MATRIX_ACCESS(n, i, k)]
				       * b[MATRIX_ACCESS(n, k, j)];
			}
			c[MATRIX_ACCESS(n, i, j)] = tmp;
		}
	}
	timing += omp_get_wtime();

	return timing;
} /* matrix_MulQuad1 */

extern double
matrix_MulQuad2(const fpv_t *a,
                const fpv_t *b,
                fpv_t       *c,
                uint32_t    n)
{
	uint32_t i, j, k;
	double   timing;

	timing = -omp_get_wtime();
#pragma omp parallel for \
	schedule(static)     \
	private(i,j,k)       \
	shared(a,b,c)
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			for (k = 0; k < n; k++) {
				c[MATRIX_ACCESS(n, i, j)] += a[MATRIX_ACCESS(n, i, k)]
				                             * b[MATRIX_ACCESS(n, k, j)];
			}
		}
	}
	timing += omp_get_wtime();

	return timing;
} /* matrix_MulQuad2 */

extern double
matrix_MulQuad3(const fpv_t *a,
                const fpv_t *b,
                fpv_t       *c,
                uint32_t    n)
{
	uint32_t i, j, k;
	double   timing;
	fpv_t    tmp;

	timing = -omp_get_wtime();
#pragma omp parallel for \
	schedule(static)     \
	private(i,j,k,tmp)   \
	shared(a,b,c)
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			tmp = 0.0;
			for (k = 0; k < n; k++) {
				tmp += a[MATRIX_ACCESS(n, i, k)]
				       * b[MATRIX_ACCESS(n, k, j)];
			}
			c[MATRIX_ACCESS(n, i, j)] = tmp;
		}
	}
	timing += omp_get_wtime();

	return timing;
} /* matrix_MulQuad3 */

extern double
matrix_MulQuad4(const fpv_t *a,
                const fpv_t *b,
                fpv_t       *c,
                uint32_t    n)
{
	uint32_t ii, jj, kk, i, j, k;
	double   timing;
	fpv_t    tmp;

	timing = -omp_get_wtime();
	for (ii = 0; ii < n; ii += BS) {
		for (jj = 0; jj < n; jj += BS) {
			for (kk = 0; kk < n; kk += BS) {
				uint32_t msub = min(n, ii + BS) - ii;
				uint32_t nsub = min(n, jj + BS) - jj;
				uint32_t psub = min(n, kk + BS) - kk;

				for (i = 0; i < msub; i++) {
					for (j = 0; j < nsub; j++) {
						tmp = c[MATRIX_ACCESS(n, ii + i, jj + j)];
						for (k = 0; k < psub; k++) {
							tmp += a[MATRIX_ACCESS(n, ii + i, kk + k)]
							       * b[MATRIX_ACCESS(n, kk + k, jj + j)];
						}
						c[MATRIX_ACCESS(n, ii + i, jj + j)] = tmp;
					}
				}
			}
		}
	}
	timing += omp_get_wtime();

	return timing;
} /* matrix_MulQuad4 */

extern double
matrix_MulQuad5(const fpv_t *a,
                const fpv_t *b,
                fpv_t       *c,
                uint32_t    n)
{
	uint32_t ii, jj, kk, i, j, k;
	double   timing;
	fpv_t    tmp;

	timing = -omp_get_wtime();
#pragma omp parallel for        \
	schedule(static)            \
	private(ii,jj,kk,i,j,k,tmp) \
	shared(a,b,c)
	for (ii = 0; ii < n; ii += BS) {
		for (jj = 0; jj < n; jj += BS) {
			for (kk = 0; kk < n; kk += BS) {
				uint32_t msub = min(n, ii + BS) - ii;
				uint32_t nsub = min(n, jj + BS) - jj;
				uint32_t psub = min(n, kk + BS) - kk;

				for (i = 0; i < msub; i++) {
					for (j = 0; j < nsub; j++) {
						tmp = c[MATRIX_ACCESS(n, ii + i, jj + j)];
						for (k = 0; k < psub; k++) {
							tmp += a[MATRIX_ACCESS(n, ii + i, kk + k)]
							       * b[MATRIX_ACCESS(n, kk + k, jj + j)];
						}
						c[MATRIX_ACCESS(n, ii + i, jj + j)] = tmp;
					}
				}
			}
		}
	}
	timing += omp_get_wtime();

	return timing;
} /* matrix_MulQuad5 */
