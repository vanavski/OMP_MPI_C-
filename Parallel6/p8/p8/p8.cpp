#include "pch.h"
#include <iostream>
#include <omp.h>
#include <ctime>

int main()
{
	int vector[20];
	int matr[20][20];
	int resultVector[20];

	double parallelTime = 0;
	double consistentTime = 0;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			vector[i] = i;
			matr[i][j] = i + 2;
		}
	}

	for (int i = 0; i < 20; i++) {
		resultVector[i] = 0;
	}

	omp_set_dynamic(0);
	omp_set_num_threads(3);


	parallelTime = omp_get_wtime();

#pragma omp parallel for
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			resultVector[i] += matr[i][j] * vector[j];
		}
	}

	parallelTime = omp_get_wtime() - parallelTime;

	for (int i = 0; i < 20; i++) {
		resultVector[i] = 0;
	}


	consistentTime = clock();

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			resultVector[i] = matr[i][j] * vector[j];
		}
	}

	consistentTime = (clock() - consistentTime) / CLOCKS_PER_SEC;

	printf("parallel: %d,\nconsistent: %d\n", parallelTime, consistentTime);

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
