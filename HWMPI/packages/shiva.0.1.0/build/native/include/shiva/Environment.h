
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <mpi.h>
#include <shiva\error.h>

#ifndef SHIVA_VERSION
#define SHIVA_VERSION 4
#endif

namespace shiva {

class environment 
{
public:
	environment()
	{
		if (! this->is_initialized()) {
			call_mpi( MPI_Init(0,0) );
		}
	}
	~environment()
	{
		if (this->is_initialized()) {
			MPI_Finalize();
		}
	}
	bool is_initialized()
	{
		int is_initialized = 0;
		call_mpi( MPI_Initialized(&is_initialized) );
		if (is_initialized == 0)
			return false;
		else
			return true;
	}

	std::string get_processor_name()
	{
		char name[MPI_MAX_PROCESSOR_NAME];
		int len;
		call_mpi(MPI_Get_processor_name(name, &len));
		return std::string(name, len);
	}

	int get_shiva_version()
	{
		return SHIVA_VERSION;
	}
};



}