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
	static auto distribution = uniform_int_distribution<>(-30, 30);
	return distribution(random);
}

int main(int argc, char **argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int n = 12;
	int loc_n = n / size;
	vector<double> locX_array;
	vector<double> locY_array;
	vector<double> x;
	vector<double> y;
	locX_array.resize(loc_n);
	locY_array.resize(loc_n);

	double result;

	if (rank == 0) 
	{
		x.resize(n);
		y.resize(n);
		for (int j = 0; j < n; j++)
		{
			x[j] = random();
			y[j] = random();
			printf("x[%d]: %f", j, x[j]);
			cout << "\n";
			printf("y[%d]: %f", j, y[j]);
			cout << "\n";
		}
		cout << "\n";

	}
	MPI_Scatter(x.data(), loc_n, MPI_DOUBLE, locX_array.data(), loc_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(y.data(), loc_n, MPI_DOUBLE, locY_array.data(), loc_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double sumX = 0;

	for (int i = 0; i < loc_n; i++)
	{
		sumX += locX_array[i] * locY_array[i];
	}


	MPI_Reduce(&sumX, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("sumX: %f ", result);
		cout << "\n";
	}

	MPI_Finalize();
	return 0;
}