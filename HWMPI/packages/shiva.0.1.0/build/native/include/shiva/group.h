
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <shiva\communicator.h>


namespace shiva {

class group
{
public:
	group(const communicator& comm)
	{
		call_mpi(MPI_Comm_group(comm.mpi_communicator(), &_mpi_group) );
	}

	int rank() const 
	{
		int rank;
		call_mpi( MPI_Group_rank(_mpi_group, &rank) );
		return rank;
	}

	int size() const
	{
		int size;
		call_mpi( MPI_Group_size(_mpi_group, &size) );
		return size;
	}

	MPI_Group mpi_group() const
	{
		return _mpi_group;
	}
private:
	MPI_Group _mpi_group;
};


}