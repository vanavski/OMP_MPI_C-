
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <shiva\Communicator.h>
#include <shiva\message.h>
#include <shiva\utility.h>
#include <shiva\status.h>
#include <limits>

#ifndef SHIVA_IGNORE_TAG
#undef max
#define SHIVA_IGNORE_TAG std::numeric_limits<int>::max()
#endif


namespace shiva {

namespace protocol {
	enum send_protocol { standard, buffered, synchronous, ready };
}

template <typename T>
inline void send(const shiva::communicator& comm, shiva::message<T>& msg, int destination, int tag = SHIVA_IGNORE_TAG)
{
	call_mpi( MPI_Send(msg.data, msg.size, msg.datatype, destination, tag, comm.mpi_communicator() )  );
}

template <typename T>
inline void send(const shiva::communicator& comm, shiva::message<T>& msg, int destination, shiva::protocol::send_protocol protocol, int tag = SHIVA_IGNORE_TAG)
{
	switch (protocol) {
	case shiva::protocol::buffered:
		call_mpi( MPI_Bsend(msg.data, msg.size, msg.datatype, destination, tag, comm.mpi_communicator() )  );
		break;
	case shiva::protocol::synchronous:
		call_mpi( MPI_Ssend(msg.data, msg.size, msg.datatype, destination, tag, comm.mpi_communicator() )  );
		break;
	case shiva::protocol::ready:
		call_mpi( MPI_Rsend(msg.data, msg.size, msg.datatype, destination, tag, comm.mpi_communicator() )  );
		break;
	default:
		call_mpi( MPI_Send(msg.data, msg.size, msg.datatype, destination, tag, comm.mpi_communicator() )  );
	};
}

template <typename T>
inline void receive(const shiva::communicator& comm, shiva::message<T>& msg, int source = MPI_ANY_SOURCE, int tag = MPI_ANY_TAG)
{
	call_mpi( MPI_Recv(msg.data, msg.size, msg.datatype, source, tag, comm.mpi_communicator(), MPI_STATUSES_IGNORE)  );
}

template <typename T>
inline shiva::status receive_with_status(const shiva::communicator& comm, shiva::message<T>& msg, int source = MPI_ANY_SOURCE, int tag = MPI_ANY_TAG)
{
	shiva::status status;
	call_mpi( MPI_Recv(msg.data, msg.size, msg.datatype, source, tag, comm.mpi_communicator(), &status.mpi_status) );
	return status;
}



}