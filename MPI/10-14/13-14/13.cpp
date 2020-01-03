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

	const int n = 6;

	int x[n][n];
	int y[n][n];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			x[i][j] = random();
			y[i][j] = random();
		}
	}

	int bufX[n], bufY[n], bufZ[n], result[n][n];

	MPI_Scatter(x, n, MPI_INT, bufX, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(y, n, MPI_INT, bufY, n, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++) {
		bufZ[i] = bufX[i] * bufY[i];
	}

	MPI_Gather(bufZ, n, MPI_INT, result, n, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("Matrix Z: \n");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				printf("%d ", result[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Finalize();
	return 0;
}