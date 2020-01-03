
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <mpi.h>
#include <shiva\error.h>

namespace shiva {

class communicator
{
public:
	communicator() : _comm(MPI_COMM_WORLD)
	{ }

	int rank() const 
	{
		int rank;
		call_mpi( MPI_Comm_rank(_comm, &rank) );
		return rank;
	}

	int size() const
	{
		int size;
		call_mpi( MPI_Comm_size(_comm, &size) );
		return size;
	}

	MPI_Comm mpi_communicator() const
	{
		return _comm;
	}

	MPI_Comm fortran_mpi_communicator() const
	{
		return MPI_Comm_c2f(_comm);
	}

private:
	MPI_Comm _comm;
};



}