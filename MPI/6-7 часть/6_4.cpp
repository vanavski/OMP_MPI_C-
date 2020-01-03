#include <iostream>
#include <mpi.h>
#include <random>
using namespace std;

static auto random = default_random_engine(random_device()());

template<typename TNumber>
void printMatrix(
	const string& title,
	TNumber array[],
	size_t arrayLength)
{
	cout << title << endl;

	int processNumber;
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

	cout << processNumber << ":";
	for (auto i = 0; i < arrayLength; ++i)
		cout << " " << array[i];
	cout << endl;
}

struct TwoArrays
{
	int intArray[10];
	double doubleArray[8];
};

int main(int argc, char** argv)
{
	MPI_Init(nullptr, nullptr);

	int blockLengths[2] = { 10, 8 };
	MPI_Datatype types[2] = { MPI_INT, MPI_DOUBLE };
	MPI_Aint adr[2] =
	{
		offsetof(struct TwoArrays, intArray),
		offsetof(struct TwoArrays, doubleArray)
	};

	MPI_Datatype arraysType;
	MPI_Type_create_struct(2, blockLengths, adr, types, &arraysType);
	MPI_Type_commit(&arraysType);

	int processNumber;
	int processesCount;
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
	MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

	TwoArrays arrays;
	if (processNumber == 0)
	{
		auto intDistribution = uniform_int_distribution<>(-10, 10);
		auto doubleDistribution = uniform_real_distribution<>(-10, 10);
		
		for (auto i = 0; i < 10; ++i)
			arrays.intArray[i] = intDistribution(random);
		printMatrix("Int array", arrays.intArray, 8);
		for (auto i = 0; i < 8; ++i)
			arrays.doubleArray[i] = doubleDistribution(random);
		printMatrix("Double array", arrays.doubleArray, 8);
	}

	MPI_Bcast(&arrays, 1, arraysType, 0, MPI_COMM_WORLD);

	if (processNumber != 0)
	{
		printMatrix("Int array", arrays.intArray, 8);
		printMatrix("Double array", arrays.doubleArray, 8);
	}

	MPI_Type_free(&arraysType);
	MPI_Finalize();
	return 0;
}