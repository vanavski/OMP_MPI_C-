
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <vector>
#include <tuple>

#include <shiva/message.h>
#include <shiva/Communicator.h>

namespace shiva {

inline std::tuple<std::vector<int>, std::vector<int>> make_equal_distribution(const shiva::communicator& comm, size_t size)
{
	const int mpi_size = comm.size();
	const int rank = comm.rank();
	const size_t object_per_process = (size_t)floor((double)(size) / (double)(mpi_size));
	const size_t num_scrubs = size - object_per_process * mpi_size;

	int part_start = 0;
	int part_end;
	
	if ((size_t)rank < num_scrubs) { // scrubs are not finished 
		part_start += (size_t)rank * (object_per_process + 1);
		part_end = part_start + (object_per_process + 1);
	} else {// all scrubs are finished
		part_start += (size_t)rank * object_per_process + num_scrubs;
		part_end = part_start + (object_per_process);
	}

	int local_size = part_end - part_start;
	std::vector<int> sizes(mpi_size);
	call_mpi( MPI_Allgather(&local_size, 1, MPI_INT,&(sizes[0]), 1, MPI_INT, comm.mpi_communicator() ));

	std::vector<int> displacements(mpi_size);
	call_mpi( MPI_Allgather(&part_start, 1, MPI_INT,&(displacements[0]), 1, MPI_INT, comm.mpi_communicator() ));

	return std::make_tuple(sizes, displacements);
}

}