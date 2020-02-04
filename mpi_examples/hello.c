/**
 * \brief  Simple example program for doing basic MPI things.
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
	int rank, size;

	/* Initialize the MPI environmet */
	MPI_Init(&argc, &argv);

	/* Get the rank and size of the MPI domain */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Say hi */
	fprintf(stdout, "Hello from %i of %i!\n", rank, size);

	/* Clean */
	MPI_Finalize();

	/* All done! */
	return EXIT_SUCCESS;
}

/***********************************************************************
 *   Implementation of local functions                                 *
 ***********************************************************************/
