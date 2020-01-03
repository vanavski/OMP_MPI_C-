// HWMPI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <vector>
using namespace std;


int main(int argc, char **argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int a = 10;
	int b = 100;
	int n = 12;
	int loc_n = n / (size - 1);
	vector<int> loc_x;
	vector<int> loc_y;
	vector<int> loc_z;

	loc_x.resize(loc_n);
	loc_y.resize(loc_n);
	loc_z.resize(loc_n);
	printf("The number of processes: %d, my number is %d", size, rank);
	if (rank == 0) {

		int tag = 0;
		vector<int> x;
		vector<int> y;
		x.resize(n);
		y.resize(n);

		for (int j = 0; j < n; j++)
		{
			x[j] = rand();
			y[j] = rand();
		}


		for (int j = 0; j < n; j++)
		{
			printf("x[%d}: %d: ", j, x[j]);
		}

		cout << "\n";

		for (int j = 0; j < n; j++)
		{
			printf("y[%d}: %d: ", j, y[j]);
		}
		cout << "\n";
		cout << "\n";


		int first = 0;

		for (int j = 1; j < size; j++)
		{
			MPI_Send(&x[first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD);
			MPI_Send(&y[first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD);
			first += loc_n;
		}
	}


	if (rank == 0)
	{
		vector<int> x;
		vector<int> y;
		vector<int> z;
		x.resize(n);
		y.resize(n);
		z.resize(n);

		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int tag = status.MPI_TAG;

		int first = 0;
		for (int j = 1; j < size; j++)
		{
			MPI_Recv(&x[0 + first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&y[0 + first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&z[0 + first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			first += loc_n;
		}
		for (int j = 0; j < n; j++)
		{
			printf("x[%d}: %d: ", j, x[j]);
		}
		cout << "\n";
		for (int j = 0; j < n; j++)
		{
			printf("y[%d}: %d: ", j, y[j]);
		}
		cout << "\n";
		for (int j = 0; j < n; j++)
		{
			printf("z[%d}: %d: ", j, z[j]);
		}
		cout << "\n";
	}
	else
	{
		MPI_Status status;
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int tag = status.MPI_TAG;
		MPI_Recv(loc_x.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(loc_y.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

		if (tag == 0)
		{
			for (int j = 0; j < loc_n; j++)
			{
				loc_z[j] = a * loc_x[j] + b * loc_y[j];
			}
		}

		MPI_Send(loc_x.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Send(loc_y.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Send(loc_z.data(), loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
