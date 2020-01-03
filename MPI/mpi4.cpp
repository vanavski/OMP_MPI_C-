// HWMPI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv)
{
	int rank;
	int size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("process number: %d, number of thread: %d ", size, rank);

	if (rank == 1)
	{
		int a[10];

		for (int j = 0; j < 10; j++)
			a[j] = rand();

		for (int j = 0; j < size; j++)
			if (j != 1)
				MPI_Send(a, 10, MPI_INT, j, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Status status;
		MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
		int number_amount;
		MPI_Get_count(&status, MPI_INT, &number_amount);
		int* number_buf = (int*)malloc(sizeof(int) * number_amount);
		MPI_Recv(number_buf, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int j = 0; j < number_amount; j++) 
		{
			printf(" %d ", number_buf[j]);
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
