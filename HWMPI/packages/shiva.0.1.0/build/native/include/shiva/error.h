
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <stdexcept>

namespace shiva {

class mpi_error : public std::runtime_error
{
public:
	explicit mpi_error(int error_code) : std::runtime_error( "MPI error!" ), err(error_code)
	{}
	const int error_code() const throw()
	{
		return err;
	}
private:
	int err;
};

inline void call_mpi(int err)
{
	if (err != 0) {
		throw mpi_error(err);
	}
}

}