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
	int n = 4;
	int loc_n = n / (size - 1);
	vector<vector<int>> loc_x;
	vector<vector<int>> loc_y;
	vector<vector<int>> loc_z;

	loc_x.resize(loc_n);
	loc_y.resize(loc_n);
	loc_z.resize(loc_n);

	for (int j = 0; j < loc_n; j++) 
	{
		loc_x[j].resize(n);
		loc_y[j].resize(n);
		loc_z[j].resize(n);
	}

	if (rank == 0) {

		int tag = 1;
		vector<vector<int>> x;
		vector<vector<int>> y;

		x.resize(n);
		y.resize(n);
		for (int j = 0; j < n; j++) 
		{
			x[j].resize(n);
			y[j].resize(n);
		}

		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < n; j++) 
			{
				x[i][j] = rand();
				y[i][j] = rand();
			}
		}

		int first = 0;
		for (int j = 1; j < size; j++) 
		{
			for (int i = first; i < first + loc_n; i++) 
			{
				MPI_Send(&x[i][0], n, MPI_INT, j, tag, MPI_COMM_WORLD);
				if (tag != 2) 
				{
					MPI_Send(&y[i][0], n, MPI_INT, j, tag, MPI_COMM_WORLD);
				}
			}
			first += loc_n;
		}

		if (tag == 1) 
		{
			first = 0;
			for (int j = 1; j < size; j++) 
			{
				for (int i = first + loc_n; i < n; i++) 
				{
					MPI_Send(&y[i][0], n, MPI_INT, j, tag, MPI_COMM_WORLD);
				}
			}
			first += loc_n;
		}


		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < n; j++) 
			{
				printf("x[%d][%d]: %d ", i, j, x[i][j]);
			}
			cout << "\n";
		}
		cout << "\n";

		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < n; j++) 
			{
				printf("y[%d][%d]: %d ", i, j, y[i][j]);
			}
			cout << "\n";
		}
		cout << "\n";
	}


	if (rank == 0)
	{
		vector<vector<int>> z;
		z.resize(n);
		for (int j = 0; j < n; j++)
		{
			z[j].resize(n);
		}


		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int tag = status.MPI_TAG;

		if (tag != 2) 
		{
			int first = 0;
			for (int j = 1; j < size; j++) 
			{
				for (int i = first; i < first + loc_n; i++) 
				{
					MPI_Recv(&z[i][0], n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
				first += loc_n;
			}
		}
		else 
		{
			int first = 0;
			for (int i = 0; i < n; i++) 
			{
				int first = 0;
				for (int j = 1; j < size; j++) 
				{
					MPI_Recv(&z[i][first], loc_n, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					first += loc_n;
				}
			}
		}

		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < n; j++) 
			{
				printf("z[%d][%d]: %d ", i, j, z[i][j]);
			}
			cout << "\n";
		}
		cout << "\n";

	}
	else 
	{
		MPI_Status status;
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int tag = status.MPI_TAG;

		for (int i = 0; i < loc_n; i++) 
		{
			MPI_Recv(&loc_x[i][0], n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			if (tag != 2) 
			{
				MPI_Recv(&loc_y[i][0], n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			}
		}

		if (tag == 0) 
		{
			for (int i = 0; i < loc_n; i++) 
			{
				for (int j = 0; j < n; j++) 
				{
					loc_z[i][j] = loc_x[i][j] * loc_y[i][j];
				}
			}
		}

		if (tag == 1) 
		{
			loc_y.resize(n);
			for (int i = loc_n; i < n; i++) 
			{
				loc_y[i].resize(n);
			}
			for (int i = loc_n; i < n; i++) 
			{
				MPI_Recv(&loc_y[i][0], n, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			}

			for (int i = 0; i < loc_n; i++) 
			{
				for (int j = 0; j < n; j++) 
				{
					for (int k = 0; k < n; k++) 
					{
						loc_z[i][j] += loc_x[i][k] * loc_y[k][j];
					}
				}
			}
		}

		if (tag == 2) 
		{
			loc_z.resize(n);
			for (int i = 0; i < n; i++) 
			{
				loc_z[i].resize(loc_n);
			}
			for (int i = 0; i < n; i++) 
			{
				for (int j = 0; j < loc_n; j++) 
				{
					loc_z[i][j] = loc_x[j][i];
				}
			}

		}

		if (tag != 2)
		{
			for (int i = 0; i < loc_n; i++) {
				MPI_Send(&loc_z[i][0], n, MPI_INT, 0, tag, MPI_COMM_WORLD);
			}
		}
		else
		{
			for (int i = 0; i < n; i++)
			{
				MPI_Send(&loc_z[i][0], loc_n, MPI_INT, 0, tag, MPI_COMM_WORLD);
			}
		}
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
