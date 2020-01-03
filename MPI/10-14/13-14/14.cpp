// HWMPI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>
#include <random>
using namespace std;

int random()
{
	static auto random = default_random_engine(random_device()());
	static auto distribution = uniform_int_distribution<>(-10, 10);
	return distribution(random);
}

int main(int argc, char **argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int n = 4;

	int A[n][n], x[n], z[n], buff[n], localResult = 0;

	for (int i = 0; i < n; i++)
	{
		x[i] = random();
		for (int j = 0; j < n; j++)
		{
			A[i][j] = random();
		}
	}
	MPI_Scatter(A, n, MPI_INT, buff, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(A, n, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++)
		localResult += buff[i] * x[i];

	printf("local result = %d from process %d \n", localResult, rank);

	MPI_Gather(&localResult, 1, MPI_INT, z, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) 
	{
		printf("Z = ");
		for (int i = 0; i < n; i++) {
			printf("%d ", z[i]);
		}
		printf("\n");
	}

	MPI_Finalize();
	return 0;
}