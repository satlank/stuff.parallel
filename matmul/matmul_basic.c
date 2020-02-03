/**
 * \brief  Simple example program for multiplying NxN matrices.
 *
 *
 * (C) 2009, Steffen R. Knollmann (steffen.knollmann@uam.es)
 */


/***********************************************************************
 *   Includes                                                          *
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <omp.h>
#include "matrix.h"
#include "config.h"

/***********************************************************************
 *   Defines                                                           *
 ***********************************************************************/
#define TWOPI 6.28318530717958647692528677


/***********************************************************************
 *   Prototypes of local functions                                     *
 ***********************************************************************/
static void
local_ParseCmdline(int argc, char **argv, uint32_t *dim, uint32_t *mode);

static void
local_InitMatrices(fpv_t *a, fpv_t *b, fpv_t *c, uint32_t dim);

static void
local_MatMul(const fpv_t *a,
             const fpv_t *b,
             fpv_t       *c,
             uint32_t    dim,
             uint32_t    mode);

static void
local_Verify(const fpv_t *c, uint32_t dim);


/***********************************************************************
 *   MAIN                                                              *
 ***********************************************************************/
int
main(int argc, char **argv)
{
	fpv_t    *a;
	fpv_t    *b;
	fpv_t    *c;
	uint32_t dim, mode;

	/* Parse the command line */
	local_ParseCmdline(argc, argv, &dim, &mode);

	/* Get memory for matrices */
	a = matrix_Alloc(dim, dim);
	b = matrix_Alloc(dim, dim);
	c = matrix_Alloc(dim, dim);

	/* Initialize the matrices */
	local_InitMatrices(a, b, c, dim);

	/* Now multiply the a and b, write the result to c */
	local_MatMul(a, b, c, dim, mode);

	/* Verify the result */
	local_Verify(c, dim);

	/* Clean up */
	matrix_Free(&c);
	matrix_Free(&b);
	matrix_Free(&a);

	/* All done! */
	return EXIT_SUCCESS;
} /* main */

/***********************************************************************
 *   Implementation of local functions                                 *
 ***********************************************************************/
static void
local_ParseCmdline(int argc, char **argv, uint32_t *dim, uint32_t *mode)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <dim> <mode>\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	*dim  = (uint32_t)atoi(argv[1]);
	*mode = (uint32_t)atoi(argv[2]);

	return;
}

static void
local_InitMatrices(fpv_t *a, fpv_t *b, fpv_t *c, uint32_t dim)
{
	uint32_t i, j;
	double   timing;

	printf("Initializing a...");
	fflush(stdout);
	timing = -omp_get_wtime();
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			a[MATRIX_ACCESS(dim, i, j)] = 1.0;
		}
	}
	timing += omp_get_wtime();
	printf(" done (%fs)\n", timing);

	printf("Initializing b...");
	fflush(stdout);
	timing = -omp_get_wtime();
	for (j = 0; j < dim; j++) {
		for (i = 0; i < dim; i++) {
			b[MATRIX_ACCESS(dim, i, j)] = 1.0;
		}
	}
	timing += omp_get_wtime();
	printf(" done (%fs)\n", timing);

	printf("Initializing c...");
	fflush(stdout);
	timing = -omp_get_wtime();
	for (i = 0; i < dim * dim; i++) {
		c[i] = 0.0;
	}
	timing += omp_get_wtime();
	printf(" done (%fs)\n", timing);

	return;
} /* local_InitMatrices */

static void
local_MatMul(const fpv_t *a,
             const fpv_t *b,
             fpv_t       *c,
             uint32_t    dim,
             uint32_t    mode)
{
	double timing;

	printf("Calculating c = a x b...");
	fflush(stdout);
	switch (mode) {
	case 0:
		timing = matrix_MulQuad0(a, b, c, dim);
		break;
	case 1:
		timing = matrix_MulQuad1(a, b, c, dim);
		break;
	case 2:
		timing = matrix_MulQuad2(a, b, c, dim);
		break;
	case 3:
		timing = matrix_MulQuad3(a, b, c, dim);
		break;
	case 4:
		timing = matrix_MulQuad4(a, b, c, dim);
		break;
	case 5:
		timing = matrix_MulQuad5(a, b, c, dim);
		break;
	default:
		fprintf(stderr, "Sorry.  No clue about mode %" PRIu32 "\n", mode);
		timing = 0;
	} /* switch */
	printf("done (%fs, %fGflops)\n",
	       timing,
	       (double)dim * (double)dim * (double)dim * 2.0 / timing * 1e-9);

	return;
} /* local_MatMul */

static void
local_Verify(const fpv_t *c, uint32_t dim)
{
	double   diff = 0.0;
	uint32_t i, j;

	printf("Verifying result... ");
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			diff = c[MATRIX_ACCESS(dim, i, j)] - (double)dim;
			if (diff < 0.0)
				diff = -diff;
			if (diff > 1e-8) {
				printf("FAILED\n");
				return;
			}
		}
	}

	printf("passed\n");

	return;
} /* local_Verify */
