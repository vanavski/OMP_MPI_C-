// HWMPI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// 6 ЧАСТЬ 2е ЗАДАНИЕ

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

struct LinesType
{
	int line1[8];
	int line2[8];
};

int main(int argc, char **argv)
{
	int size, rank;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int tag = 1;
	struct LinesType linesType;
	const int n = 8;
	int blockLengths[2] = { n, n };

	MPI_Datatype types[2] = { MPI_INT, MPI_INT };
	MPI_Aint adr[2];

	adr[0] = offsetof(struct LinesType, line1);
	adr[1] = offsetof(struct LinesType, line2);

	MPI_Datatype arrType;

	MPI_Type_create_struct(2, blockLengths, adr, types, &arrType);
	MPI_Type_commit(&arrType);

	int a[n][n];
	int d[2][n];

	if (rank == 3) {
		printf("matrix A: \n");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				a[i][j] = random();
				printf("%d ", a[i][j]);
			}
			printf("\n");
		}

		for (int i = 0; i < size - 1; i++) {
			for (int j = 0; j < n; j++) {
				linesType.line1[j] = a[i][j];
				linesType.line2[j] = a[i + 4][j];
			}
			MPI_Send(&linesType, 2 * n, arrType, i, tag, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Status status;

		MPI_Recv(&linesType, 2 * n, arrType, 3, tag, MPI_COMM_WORLD, &status);
		int matrixD[2][n];
		for (int j = 0; j < n; j++) {
			matrixD[0][j] = linesType.line1[j];
			matrixD[1][j] = linesType.line2[j];
		}

		printf("matrix D from process %d: \n", rank);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 8; j++) {
				printf("%d ", matrixD[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Type_free(&arrType);
	MPI_Finalize();
	return 0;
}