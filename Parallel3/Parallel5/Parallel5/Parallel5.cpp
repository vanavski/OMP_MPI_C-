// Parallel5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <omp.h>
#include <stdlib.h>

int main()
{
	omp_set_dynamic(0);
	omp_set_num_threads(3);

	int arr[6][8];

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			arr[i][j] = rand() % 10;
			printf("%d", arr[i][j]);
		}
		printf("%d");
	}

#pragma omp parallel sections
	{
		#pragma omp section
		{
			int avg = 0;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					avg += arr[i][j];
				}
			}
			printf(" thread: %d avg: %d\n", omp_get_thread_num, avg / (6 * 8));
		}

#pragma omp section
		{
			int max = 0;
			int min = 100;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (arr[i][j] > max)
						max = arr[i][j];
					if (arr[i][j] < min)
						min = arr[i][j];
				}
			}
			printf(" thread: %d min: %d max: %d\n", omp_get_thread_num, min, max);
		}

#pragma omp section
		{
			int count = 0;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (arr[i][j] % 3 == 0)
					{
						count++;
					}
				}
			}
			printf(" thread: %d count: %d\n", omp_get_thread_num, count);
		}
	}
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
