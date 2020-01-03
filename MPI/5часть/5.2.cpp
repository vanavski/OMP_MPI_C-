/*
Пусть A(n,m) – матрица, созданная на нулевом процессе. Пусть есть 4 процесса
и процесс 0 посылает части этой матрицы другим процессам. Процессор 1
получает A(i,j) для i=n/2+1,...,n, и j=1,...,m/2. Процессор 2 получает A(i,j) для
i=1,...,n/2 и j=m/2+1,...,m и процессор 3 получает A(i,j) для i=n/2+1,...,n and
j=m/2,...,m . Это двумерная декомпозиция А на четыре процесса. Написать
программу рассылки частей матрицы по процессам, используя функцию
MPI_Scatterv.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc, char** argv)
{
	int integer;
	const int n = 4, m = 4;
	int size, rank;

	MPI_Status stat;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int count =  n * m / size;
	int* b = new int[count];
	int* scounts = new int[size];
	int* displs = new int[size];
	int a[n][m];

	if (rank == 0)
	{
		integer += count;
		
		for(int i = 0; i < n; i++)
		{	
			for (int j = 0; j < m; j++)
			{
				a[i][j] = rand();
			}

			for (int i = 0; i < size; i++)
			{
				scounts[i] = count;
				displs[i] = i * count;
			}
		}    
   	}
    
   	MPI_Scatterv(&a, scounts, displs, MPI_INT, b, count, MPI_INT, 0, MPI_COMM_WORLD);
   	MPI_Finalize();
}
