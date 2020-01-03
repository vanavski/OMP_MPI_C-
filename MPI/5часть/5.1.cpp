/*
Реализовать при помощи операций MPI_Send и MPI_Recv коллективный сбор
данных - операцию MPI_Gather. Обратите внимание, функция MPI_Gather
производит сборку блоков данных, посылаемых всеми процессами группы, в
один массив процесса с номером root. Длина блоков предполагается
одинаковой. Объединение происходит в порядке увеличения номеров
процессов-отправителей. То есть данные, посланные процессом i из своего
буфера sendbuf, помещаются в i-ю порцию буфера recvbuf процесса root. Длина
массива, в который собираются данные, должна быть достаточной для их
размещения.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc, char** argv)
{
	int integer, size, rank;

	MPI_Status stat;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
   	
    int count =  16 / size;
   	int* b = new int[count];

   	if (rank == 0)
   	{
        int a[16];
        integer = 0 + count;

        for(int i = 0; i < count; i++)
		{
			a[i] = rand() / (100 + rank) + rank;				
		}
            

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&a[integer], count, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
            integer += count;
        }
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			b[i] = rand() / (100 + rank) + rank;
		}

		MPI_Send(b, count, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}
