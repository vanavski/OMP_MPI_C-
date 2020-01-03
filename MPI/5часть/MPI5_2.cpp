#include <iostream>
#include <mpi.h>
#include <random>
#include <string>
#include <vector>
using namespace std;

int random()
{
	static auto random = default_random_engine(random_device()());
	static auto distribution = uniform_int_distribution<>(-30, 30);
	return distribution(random);
}

unique_ptr<vector<int>> getRandomArray(size_t size)
{
	auto array = make_unique<vector<int>>(size);
	for (auto& element : *array)
		element = random();
	return array;
}

void printMatrix(
	const string& title,
	const unique_ptr<vector<unique_ptr<vector<int>>>>& matrix,
	size_t rowsCount,
	size_t colsCount)
{
	cout << title << endl;

	int processNumber;
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

	for (auto row = 0; row < rowsCount; ++row)
	{
		cout << processNumber << ":";
		for (auto col = 0; col < colsCount; ++col)
			cout << " " << (*(*matrix)[row])[col];
		cout << endl;
	}
}

int main()
{
	const size_t rowsCount = 10;
	const size_t colsCount = 10;

	const size_t rowsCountInPart = rowsCount / 2;
	const size_t colsCountInPart = colsCount / 2;

	MPI_Init(nullptr, nullptr);

	MPI_Datatype lineType;
	MPI_Type_vector(1, colsCountInPart, colsCountInPart, MPI_INT, &lineType);
	MPI_Type_commit(&lineType);

	int processNumber;
	int processesCount;
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
	MPI_Comm_size(MPI_COMM_WORLD, &processesCount);
	
	auto matrix = make_unique<vector<unique_ptr<vector<int>>>>();
	if (processNumber == 0)
	{
		matrix = make_unique<vector<unique_ptr<vector<int>>>>(rowsCount);
		for (auto& row : *matrix)
			row = getRandomArray(colsCount);

		printMatrix("Matrix", matrix, rowsCount, colsCount);

		for (auto row = 0; row < rowsCountInPart; ++row)
		{
			MPI_Send((*matrix)[row + rowsCountInPart]->data(), 1, lineType, 1, 0, MPI_COMM_WORLD);
			MPI_Send((*matrix)[row]->data() + colsCountInPart, 1, lineType, 2, 0, MPI_COMM_WORLD);
			MPI_Send((*matrix)[row + rowsCountInPart]->data() + colsCountInPart, 1, lineType, 3, 0, MPI_COMM_WORLD);
		}

		printMatrix("Process 0:", matrix, rowsCountInPart, colsCountInPart);
	}
	else
	{
		auto matrixPart = make_unique<vector<unique_ptr<vector<int>>>>(rowsCountInPart);
		for (auto& row : *matrixPart)
		{
			row = make_unique<vector<int>>(colsCountInPart);
			MPI_Recv(row->data(), 1, lineType, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		printMatrix("Process " + to_string(processNumber) + ":", matrixPart, rowsCountInPart, colsCountInPart);
	}

	MPI_Finalize();
	return 0;
}