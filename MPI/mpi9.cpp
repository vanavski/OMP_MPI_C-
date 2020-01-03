#include "mpi.h"
#include <iostream>
#include <cstdlib>

using namespace std;

#define N 10

int main(int argc, char** argv)
{
	int rank, size;
	int sendbuf, recbuf;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	sendbuf = rand() % 10;
	recbuf = rand() % 10;

	auto start = MPI_Wtime();
	if (rank == 0)
	{
		MPI_Send(&sendbuf, 1, MPI_INT, 1, 99, MPI_COMM_WORLD);
	}
	if (rank == 1)
	{
		MPI_Send(&sendbuf, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
	}
	if (rank == 0)
	{
		MPI_Recv(&sendbuf, 1, MPI_INT, 1, 99, MPI_COMM_WORLD, &status);
	}
	if (rank == 1)
	{
		MPI_Recv(&sendbuf, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
	}
	printf("proc# %d time for Ping-Ping is %f\n", rank, MPI_Wtime() - start);

	start = MPI_Wtime();
	
	MPI_Sendrecv(&sendbuf, 1, MPI_INT, 1, 10, &recbuf, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

	printf("proc# %d time for Ping-Pong is %f\n", rank, MPI_Wtime() - start);

	MPI_Finalize();
}