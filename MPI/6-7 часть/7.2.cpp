// HWMPI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// 7 ЧАСТЬ 2е ЗАДАНИЕ

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
	int rank, size, newRank = -1, newSize = -1;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int color = rank % 4;
	int dest;
	MPI_Comm new_comm, icomm;
	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);

	MPI_Comm_rank(new_comm, &newRank);
	MPI_Comm_size(new_comm, &newSize);

	int n = 2;
	int m = n * newSize;
	vector<double> array;
	vector<double> arrayGroup;
	array.resize(n);

	for (int i = 0; i < n; i++) 
	{
		array[i] = color;
	}

	if (newRank == 0) 
	{ 
		arrayGroup.resize(m); 
	}
	MPI_Gather(array.data(), n, MPI_DOUBLE, arrayGroup.data(), n, MPI_DOUBLE, 0, new_comm);

	if (newRank == 0) 
	{
		printf("My group number: %d my number", color);
		for (int i = 0; i < m; i++) 
		{
			cout << arrayGroup[i] << " ";
		}
		cout << "\n";

	}

	switch (color) 
	{
		case 0: dest = 1;
			break;
		case 1: dest = 0;
			break;
		case 2: dest = 3;
			break;
		case 3: dest = 2;
	}

	MPI_Intercomm_create(new_comm, 0, MPI_COMM_WORLD, dest, 10, &icomm);

	if (newRank == 0) 
	{
		MPI_Send(arrayGroup.data(), m, MPI_DOUBLE, 0, 5, icomm);
		vector<double> array_input;
		array_input.resize(m);
		MPI_Status status;
		MPI_Recv(array_input.data(), m, MPI_DOUBLE, 0, 5, icomm, &status);

		printf("My group number: %d my input array ", color);
		for (int i = 0; i < m; i++) 
		{
			cout << array_input[i] << " ";
		}
		cout << "\n";
	}

	MPI_Comm_free(&new_comm);

	MPI_Finalize();
	return 0;
}