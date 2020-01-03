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
	MPI_Init(&argc, &argv);

	int size, rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int tag = 1;
	const int n = 8;
	int b[n / 2][n], c[n / 2][n];

	MPI_Datatype linesType;

	MPI_Type_vector(n / 2, n, n * 2, MPI_INT, &linesType);
	MPI_Type_commit(&linesType);

	if (rank == 0) {
		int a[n][n];
		printf("matrix a: \n");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				a[i][j] = random();
				printf("%d ", a[i][j]);
			}
			printf("\n");
		}

		for (int i = 0; i < 2; i++) {
			MPI_Send(&a[i][0], 1, linesType, i + 1, tag, MPI_COMM_WORLD);
		}
	}


	if (rank == 1) {
		MPI_Status status;
		int size;

		MPI_Probe(0, tag, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &size);

		int *tmpArray = new int[size];
		MPI_Recv(tmpArray, size, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

		int count = 0;
		for (int i = 0; i < n / 2; i++) {
			for (int j = 0; j < n; j++) {
				b[i][j] = tmpArray[count];
				count++;
			}
		}

		printf("matrix b: \n");
		for (int i = 0; i < n / 2; i++) {
			for (int j = 0; j < n; j++) {
				printf("%d ", b[i][j]);
			}
			printf("\n");
		}
	}

	if (rank == 2) {

		MPI_Status status;
		int k;

		MPI_Probe(0, tag, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &k);

		int *tmpArray = (int *)malloc(sizeof(int) * k);
		MPI_Recv(tmpArray, k, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

		int count = 0;
		printf("matrix c: \n");
		for (int i = 0; i < n / 2; i++) {
			for (int j = 0; j < n; j++) {
				c[i][j] = tmpArray[count];
				count++;
				printf("%d ", c[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Finalize();
	return 0;
}