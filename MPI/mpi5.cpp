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
	int n = 12;
	int loc_n = 12 / size;
	vector<int> loc_array;
	vector<int> array;
	loc_array.resize(loc_n);
	printf("proccess number: %d, number of thread %d ", size, rank);
	if (rank == 0)
	{
		array.resize(n);
		for (int j = 0; j < n; j++)
			array[j] = rand();
		int first = 0;
		MPI_Request request;
		for (int j = 0; j < size; j++)
		{
			MPI_Isend(&array[first], loc_n, MPI_INT, j, 0, MPI_COMM_WORLD, &request);
			first += loc_n;
		}
	}
	MPI_Status status;
	MPI_Recv(loc_array.data(), loc_n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	//MPI_Scatter(array.data(), loc_n, MPI_INT, loc_array.data(), loc_n, MPI_INT, 0, MPI_COMM_WORLD);
	for (int j = 0; j < loc_n; j++)
		printf(" %d ", loc_array[j]);
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
