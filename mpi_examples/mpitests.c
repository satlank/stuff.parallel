/**
 * \brief  This program will perform a couple of MPI test.
 *
 *
 * (C) 2009, Steffen R. Knollmann (steffen.knollmann@uam.es)
 */


/***********************************************************************
 *   Includes                                                          *
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "config.h"


/***********************************************************************
 *   Defines                                                           *
 ***********************************************************************/
#define MAX_DATA 27
#define MAX_PING_PONG 10000


/***********************************************************************
 *   Prototypes of local functions                                     *
 ***********************************************************************/


/***********************************************************************
 *   MAIN                                                              *
 ***********************************************************************/
int
main(int argc, char **argv)
{
	int        rank, size;
	double     timing;
	double     *buffSend, *buffRecv, *buff;
	int        n, i, j, k, target;
	int        buffIntSend, buffIntRecv;
	MPI_Status status;

	/* Initialize the MPI environmet */
	MPI_Init(&argc, &argv);

	/* Get the rank and size of the MPI domain */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* First test, Send bandwidth with different sizes */
	if (rank == 0)
		fprintf(stdout, "Testing MPI_Send\n----------------------\n");
	for (i = 0; i < MAX_DATA; i++) {
		n    = 1 << i;
		buff = malloc(sizeof(double) * n);
		for (j = 0; j < n; j++)
			buff[j] = 1234.567890;

		MPI_Barrier(MPI_COMM_WORLD);

		if (rank == 0) {
			if (n * sizeof(double) < 1024)
				fprintf(stdout, "Sending %5ib : ",
				        (int)(n * sizeof(double)));
			else if (n * sizeof(double) < 1024 * 1024)
				fprintf(stdout, "Sending %5.1fkb: ",
				        n * sizeof(double) / 1024.);
			else if (n * sizeof(double) < 1024 * 1024 * 1024)
				fprintf(stdout, "Sending %5.1fMb: ",
				        n * sizeof(double) / (1024. * 1024));
			for (target = 1; target < size; target++) {
				timing = -MPI_Wtime();
				for (k = 0; k < 10; k++)
					MPI_Send(buff, n, MPI_DOUBLE, target,
					         99, MPI_COMM_WORLD);
				timing += MPI_Wtime();
				timing /= k;
				fprintf(stdout, " %12.7fMb/s",
				        (n * sizeof(double)) / (1024. * 1024. * timing));
			}
			fprintf(stdout, "\n");
		} else {
			for (k = 0; k < 10; k++)
				MPI_Recv(buff, n, MPI_DOUBLE, 0, 99,
				         MPI_COMM_WORLD, &status);
		}

		free(buff);
	}
	if (rank == 0)
		fprintf(stdout, "\n\n");

	/* Send test, Sendrecv bandwidth with different sizes */
	if (rank == 0)
		fprintf(stdout, "Testing MPI_Sendrecv\n----------------------\n");
	for (i = 0; i < MAX_DATA; i++) {
		n        = 1 << i;
		buffSend = malloc(sizeof(double) * n);
		buffRecv = malloc(sizeof(double) * n);
		for (j = 0; j < n; j++)
			buffSend[j] = 1234.567890;

		MPI_Barrier(MPI_COMM_WORLD);

		if (rank == 0) {
			if (n * sizeof(double) < 1024)
				fprintf(stdout, "Sending %5ib : ",
				        (int)(n * sizeof(double)));
			else if (n * sizeof(double) < 1024 * 1024)
				fprintf(stdout, "Sending %5.1fkb: ",
				        n * sizeof(double) / 1024.);
			else if (n * sizeof(double) < 1024 * 1024 * 1024)
				fprintf(stdout, "Sending %5.1fMb: ",
				        n * sizeof(double) / (1024. * 1024));
			for (target = 1; target < size; target++) {
				timing = -MPI_Wtime();
				for (k = 0; k < 10; k++)
					MPI_Sendrecv(buffSend, n, MPI_DOUBLE, target, 99,
					             buffRecv, n, MPI_DOUBLE, target, 98,
					             MPI_COMM_WORLD, &status);
				timing += MPI_Wtime();
				timing /= k;
				fprintf(stdout, " %12.7fMb/s",
				        (n * sizeof(double)) / (1024. * 1024. * timing));
			}
			fprintf(stdout, "\n");
		} else {
			for (k = 0; k < 10; k++)
				MPI_Sendrecv(buffSend, n, MPI_DOUBLE, 0, 98,
				             buffRecv, n, MPI_DOUBLE, 0, 99,
				             MPI_COMM_WORLD, &status);
		}

		free(buffRecv);
		free(buffSend);
	}
	if (rank == 0)
		fprintf(stdout, "\n\n");

	/* Third test, Ping-pong */
	if (rank == 0)
		fprintf(stdout, "Ping pong\n----------------------\n");
	buffIntSend = 2314;
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		fprintf(stdout, "Ping-pong time:  ");
		for (target=1; target<size; target++) {
			timing = -MPI_Wtime();
			for (k = 0; k < MAX_PING_PONG; k++)
				MPI_Sendrecv(&buffIntSend, 1, MPI_INT, target, 99,
				             &buffIntRecv, 1, MPI_INT, target, 98,
				             MPI_COMM_WORLD, &status);
			timing += MPI_Wtime();
			timing /= (double)k;
			fprintf(stdout, " %12.2fμs", timing*1e6);
		}
		fprintf(stdout, "\n");
	} else {
		for (k = 0; k < 10000; k++)
			MPI_Sendrecv(&buffIntSend, 1, MPI_INT, 0, 98,
			             &buffIntRecv, 1, MPI_INT, 0, 99,
			             MPI_COMM_WORLD, &status);
	}
	if (rank == 0)
		fprintf(stdout, "\n\n");


	/* Clean */
	MPI_Finalize();

	/* All done! */
	return EXIT_SUCCESS;
} /* main */

/***********************************************************************
 *   Implementation of local functions                                 *
 ***********************************************************************/
