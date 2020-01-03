#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>
using namespace std;

int random()
{
	static auto random = default_random_engine(random_device()());
	static auto distribution = uniform_int_distribution<>(-30, 30);
	return distribution(random);
}

unique_ptr<vector<int>> getRandomArray(size_t size)
{
	auto array = unique_ptr<vector<int>>(new vector<int>(size));
	for (auto& element : *array)
		element = random();
	return array;
}

namespace MPI5_1
{
	int MPI_Gather(
		void* sendbuf,
		int sendcount,
		MPI_Datatype sendtype,
		void* recvbuf,
		int recvcount,
		MPI_Datatype recvtype,
		int root,
		MPI_Comm comm
	)
	{
		if (sendcount != recvcount)
			MPI_Abort(comm, 1);

		int sentBytes;
		MPI_Type_size(sendtype, &sentBytes);
		sentBytes *= sendcount;
		int receiveBytes;
		MPI_Type_size(recvtype, &receiveBytes);
		receiveBytes *= recvcount;

		int processesCount, processNumber;
		MPI_Comm_size(comm, &processesCount);
		MPI_Comm_rank(comm, &processNumber);

		auto buffer = make_unique<vector<unsigned char>>(receiveBytes);
		if (processNumber == root)
			for (auto receivingProcessNumber = 0; receivingProcessNumber < processesCount; ++receivingProcessNumber)
				if (receivingProcessNumber == root)
					memcpy(recvbuf, sendbuf, receiveBytes);
				else
				{
					MPI_Recv(buffer->data(), recvcount, recvtype, receivingProcessNumber, 0, comm, MPI_STATUS_IGNORE);
					memcpy(
						((unsigned char*)recvbuf) + receivingProcessNumber * receiveBytes,
						buffer->data(),
						receiveBytes);
				}
		else
			MPI_Send(sendbuf, sendcount, sendtype, root, 0, comm);

		return 0;
	}
}

int main()
{
	const size_t partSize = 5;

	MPI_Init(nullptr, nullptr);

	int processNumber;
	int processesCount;
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
	MPI_Comm_size(MPI_COMM_WORLD, &processesCount);
	
	auto part = getRandomArray(partSize);
	cout << processNumber << ": ";
	for (auto& element : *part)
		cout << " " << element;
	cout << endl;

	auto array = make_unique<vector<int>>();
	if (processNumber == 0)
		array = make_unique<vector<int>>(partSize * processesCount);

	MPI5_1::MPI_Gather(part->data(), partSize, MPI_INT, array->data(), partSize, MPI_INT, 0, MPI_COMM_WORLD);

	if (processNumber == 0)
	{
		cout << "Result: ";
		for (auto& element : *array)
			cout << " " << element;
		cout << endl;
	}

	MPI_Finalize();
	return 0;
}