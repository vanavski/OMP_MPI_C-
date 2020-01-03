﻿// Parallel15.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <omp.h>
#include <stdio.h>
#include "pch.h"
#include <stdio.h>


int main()
{
	int i, j, end, simpleValue;

	printf("Enter N: ");
	scanf_s("%d", &end);
	printf("All simple numbers between 1 to %d are:\n", end);

#pragma omp parallel for
	for (i = 2; i <= end; i++)
	{
		simpleValue = 1;
		for (j = 2; j <= i / 2; j++)
		{
			if (i%j == 0)
			{
				simpleValue = 0;
				break;
			}
		}

		if (simpleValue == 1) {
			printf("%d, ", i);
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
