#include <chrono>
#include <iostream>
#include <mpi.h>
#include <random>
using namespace std;

int random()
{
	static auto random = default_random_engine(random_device()());
	static auto distribution = uniform_int_distribution<>(-30, 30);
	return distribution(random);
}

int main()
{
	MPI_Init(nullptr, nullptr);

	int processNumber;
	int processesCount;
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
	MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

	auto x = random();
	int sum;

	chrono::time_point<chrono::steady_clock> startTime;
	chrono::time_point<chrono::steady_clock> endTime;
	chrono::duration<double, milli> executionTime;

	if (processNumber == 0)
		startTime = chrono::steady_clock::now();
	MPI_Reduce(&x, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (processNumber == 0)
	{
		endTime = chrono::steady_clock::now();
		executionTime = chrono::duration<double, milli>(endTime - startTime);
		cout << "With MPI_Reduce and MPI_Bcast: " << executionTime.count() << " ms" << endl;
	}

	if (processNumber == 0)
		startTime = chrono::steady_clock::now();
	MPI_Allreduce(&x, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	if (processNumber == 0)
	{
		endTime = chrono::steady_clock::now();
		executionTime = chrono::duration<double, milli>(endTime - startTime);
		cout << "With MPI_Allreduce: " << executionTime.count() << " ms" << endl;
	}

	MPI_Finalize();
	return 0;
}