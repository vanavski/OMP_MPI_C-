// Parallel3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <omp.h>

int main()
{
	int a = 2, b = 4;
	printf("a: %d  b: %d\n", a, b);

#pragma omp parallel num_threads(2) private(a) firstprivate(b)
	{
		a = 2;
		a += omp_get_thread_num();
		b += omp_get_thread_num();
		printf("thread: %d a: %d  b: %d\n", omp_get_thread_num(), a, b);
	}
	printf("after 1st parallel: a %d, b %d\n", a, b);

#pragma omp parallel num_threads(4) shared(a) private(b)
	{
		b = 4;
		a -= omp_get_thread_num();
		b -= omp_get_thread_num();
		printf("thread: %d a: %d  b: %d\n", omp_get_thread_num(), a, b);
	}
	printf("after 2nd parallel: a %d, b %d\n", a, b);

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
