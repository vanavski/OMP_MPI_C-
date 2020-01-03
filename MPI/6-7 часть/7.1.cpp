// HWMPI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// 7 ЧАСТЬ 1е ЗАДАНИЕ

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
	int rank, size, new_rank = -1, new_size = -1;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Group allGroup, newGroup;
	MPI_Comm newCom;
	int a[5] = { 8,3,9,1,6 };
	MPI_Comm_group(MPI_COMM_WORLD, &allGroup);
	MPI_Group_incl(allGroup, 4, a, &newGroup);
	MPI_Comm_create(MPI_COMM_WORLD, newGroup, &newCom);

	if (MPI_COMM_NULL != newCom) {
		MPI_Comm_rank(newCom, &new_rank);
		MPI_Comm_size(newCom, &new_size);
	}

	int n = 8;
	vector<double> arr;
	arr.resize(n);

	printf("the number of process: %d, my number is %d ", new_size, new_rank);
	MPI_Barrier;
	if (new_rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			arr[i] = random();
		}

		for (int i = 0; i < n; i++)
		{
			printf("arr[%d] = %d", i, arr[i]);
		}
		cout << "\n";
	}

	if (MPI_COMM_NULL != newCom) {
		MPI_Bcast(arr.data(), n, MPI_DOUBLE, 0, newCom);
		for (int i = 0; i < n; i++)
		{
			cout << arr[i] << " ";
		}
		cout << "\n";
	}


	if (new_rank == new_size - 1) {
		MPI_Send(arr.data(), n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	if (rank == 0) {
		MPI_Status status;
		MPI_Recv(arr.data(), n, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		for (int i = 0; i < n; i++)
		{
			cout << arr[i] << " ";
		}
		cout << "\n";
	}

	if (MPI_COMM_NULL != newCom)
	{
		MPI_Comm_free(&newCom);
	}

	MPI_Group_free(&newGroup);
	MPI_Group_free(&allGroup);

	MPI_Finalize();
	return 0;
}