// Parallel12.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <omp.h>

int main()
{
#pragma omp parallel for ordered num_threads(8)
	for (int i = 7; i > -1; i--)
	{
		{
#pragma omp ordered
			printf("Hello World!, %d, %d;\n", i, omp_get_num_threads());
		}
	}

	printf("\n");

	int g = 7;
#pragma omp parallel for num_threads(8)
	for (int i = 7; i > -1; i--)
	{
#pragma omp critical
		{
			printf("Hello World!, %d, %d;\n", g, omp_get_num_threads());
			g -= 1;
		}
	}

	printf("\n");

#pragma omp parallel num_threads(8)
	{
		for (int i = 7; i > -1; i--)
		{
#pragma omp barrier
			if (omp_get_thread_num() == i)
			{
#pragma omp critical
				printf("Hello World!, %d, %d;\n", i, omp_get_num_threads());
			}
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
