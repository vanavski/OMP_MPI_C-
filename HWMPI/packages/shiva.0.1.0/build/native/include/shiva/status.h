
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <mpi.h>

namespace shiva {

class status
{
public:
	MPI_Status mpi_status;

	int tag()
	{
		return mpi_status.MPI_TAG;
	}

};

}