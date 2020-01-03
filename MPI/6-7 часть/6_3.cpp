#include <iostream>
#include <mpi.h>
#include <random>
using namespace std;

int random()
{
	static auto random = default_random_engine(random_device()());
	static auto distribution = uniform_int_distribution<>(-10, 10);
	return distribution(random);
}

template<size_t colsCount>
void printMatrix(
	const string& title,
	int matrix[][colsCount],
	size_t rowsCount)
{
	cout << title << endl;

	int processNumber;
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

	for (auto row = 0; row < rowsCount; ++row)
	{
		cout << processNumber << ":";
		for (auto col = 0; col < colsCount; ++col)
			cout << " " << matrix[row][col];
		cout << endl;
	}
}

int main(int argc, char** argv)
{
	MPI_Init(nullptr, nullptr);

	const size_t matrixSize = 8;

	int matrix[matrixSize][matrixSize];
	for (auto row = 0; row < matrixSize; ++row)
		for (auto col = 0; col < matrixSize; ++col)
			matrix[row][col] = 0;

	int blockLengths[matrixSize];
	int indices[matrixSize];
	for (auto i = 0; i < matrixSize; ++i)
	{
		blockLengths[i] = matrixSize - i;
		indices[i] = matrixSize * i + i;
	}

	MPI_Datatype matrixType;
	MPI_Type_indexed(matrixSize, blockLengths, indices, MPI_INT, &matrixType);
	MPI_Type_commit(&matrixType);

	int processNumber;
	int processesCount;
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
	MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

	if (processNumber == 0)
	{
		for (auto row = 0; row < matrixSize; ++row)
			for (auto col = 0; col < matrixSize; ++col)
				matrix[row][col] = random();
		printMatrix("Source matrix", matrix, matrixSize);
	}

	MPI_Bcast(&matrix, 1, matrixType, 0, MPI_COMM_WORLD);

	if (processNumber != 0)
		printMatrix("Received matrix", matrix, matrixSize);

	MPI_Type_free(&matrixType);
	MPI_Finalize();
	return 0;
}