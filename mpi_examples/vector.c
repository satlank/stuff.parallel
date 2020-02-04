/**
 * \brief  Simple example program for calculating the length of 
 *         an vector.
 *
 *
 * (C) 2009, Steffen R. Knollmann (steffen.knollmann@uam.es)
 */


/***********************************************************************
 *   Includes                                                          *
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "config.h"


/***********************************************************************
 *   Defines                                                           *
 ***********************************************************************/


/***********************************************************************
 *   Prototypes of local functions                                     *
 ***********************************************************************/


/***********************************************************************
 *   MAIN                                                              *
 ***********************************************************************/
int
main(int argc, char **argv)
{
	int rank, size, i;
	double *vec;
	double len;
	double convRandToDouble = 1./RAND_MAX;
	double buffSend, buffRecv;
	double timing;
	int numElementsTotal, numElementsLocal;

	/* Initialize the MPI environmet */
	MPI_Init(&argc, &argv);

	/* Get the rank and size of the MPI domain */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Get the number of elements of the vector */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <num elements>\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	numElementsTotal = atoi(argv[1]);
	if (numElementsTotal % size != 0)
		numElementsTotal =  (numElementsTotal+size)
		                   -(numElementsTotal%size);
	numElementsLocal = numElementsTotal/size;

	/* Allocate the vector */
	vec = malloc(sizeof(double) * numElementsLocal);

	/* Fill the vector */
	srand(rank);
	for (i=0; i<numElementsLocal; i++) {
		vec[i] = rand()*convRandToDouble;
	}

	/* Activate the stop watch */
	MPI_Barrier(MPI_COMM_WORLD);
	timing = -MPI_Wtime();

	/* Compute the sum of squares */
	buffSend = 0.0;
	for (i=0; i<numElementsLocal; i++)
		buffSend += (vec[i] * vec[i]);

	/* Get the result */
	MPI_Reduce(&buffSend, &buffRecv, 1, MPI_DOUBLE, MPI_SUM, 0,
	           MPI_COMM_WORLD);

	/* Calculate the length now on the master */
	if (rank == 0) {
		len = sqrt(buffRecv);
		timing += MPI_Wtime();
		fprintf(stdout,
		        "The result is %f.\n"
		        "The calculation took %fs)\n",
		        len, timing);
	}

	/* Clean */
	free(vec);
	MPI_Finalize();

	/* All done! */
	return EXIT_SUCCESS;
}

/***********************************************************************
 *   Implementation of local functions                                 *
 ***********************************************************************/
