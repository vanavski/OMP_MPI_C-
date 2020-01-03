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

	int n = 6;
	int loc_n = n / size;

	vector<double> finalArray;
	vector<double> final_loc_array;

	vector<vector<double>> locX_array;
	vector<vector<double>> x;

	locX_array.resize(n);
	finalArray.resize(n);
	final_loc_array.resize(loc_n);

	double sumX = 0;

	for (int i = 0; i < n; i++)
	{
		locX_array[i].resize(loc_n);
	}

	double result;

	x.resize(n);
	if (rank == 0) 
	{
		for (int j = 0; j < n; j++)
		{
			x[j].resize(n);
			for (int i = 0; i < n; i++)
			{
				x[j][i] = random();
				printf("x[%d][%d]: %f", j, i, x[j][i]);
				cout << "\n";
			}
		}
		cout << "\n";
	}

	for (int i = 0; i < n; i++)
	{
		MPI_Scatter(x[i].data(), loc_n, MPI_DOUBLE, locX_array[i].data(), loc_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		for (int j = 0; j < loc_n; j++)
			sumX += abs(locX_array[i][j]);

		MPI_Reduce(&sumX, &finalArray[i], 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	}

	MPI_Scatter(finalArray.data(), loc_n, MPI_DOUBLE, final_loc_array.data(), loc_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double max = finalArray[0];
	for (int j = 0; j < loc_n; j++)
		max = std::max(finalArray[j], max);

	MPI_Reduce(&max, &result, loc_n, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);


	if (rank == 0) {
		printf("result: %f ", result);
		cout << "\n";
	}

	MPI_Finalize();
	return 0;
}