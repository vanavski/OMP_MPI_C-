
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <mpi.h>
#include <tuple>

#include <shiva\Communicator.h>
#include <shiva\message.h>
#include <shiva\utility.h>


namespace shiva {

template <typename T>
inline void broadcast(int root, const shiva::communicator& comm, const shiva::message<T>& msg)
{
	call_mpi( MPI_Bcast(msg.data, msg.size, msg.datatype, root, comm.mpi_communicator()) );
}

inline void barrier(const communicator& comm)
{
	call_mpi( MPI_Barrier(comm.mpi_communicator()) );
}

template <typename T1, typename T2>
inline void gather(int root, const shiva::communicator& comm, const shiva::message<T1>& send_msg, const shiva::message<T2>& recv_msg)
{
	const int size = comm.size();
	call_mpi( MPI_Gather(send_msg.data, send_msg.size, send_msg.datatype, recv_msg.data, recv_msg.size / size, recv_msg.datatype, root, comm.mpi_communicator() ));
}

template <typename T>
inline void gather(int root, const shiva::communicator& comm, const shiva::message<T>& send_msg)
{
	call_mpi( MPI_Gather(send_msg.data, send_msg.size, send_msg.datatype,  nullptr, 0, 0, root, comm.mpi_communicator() ));
}

template <typename T1, typename T2, typename V1, typename V2>
inline void gatherv(int root, const shiva::communicator& comm, const std::tuple<V1, V2>& distribution, const shiva::message<T1>& send_msg, const shiva::message<T2>& recv_msg)
{
	call_mpi( MPI_Gatherv(send_msg.data, send_msg.size, send_msg.datatype, recv_msg.data, 
		const_cast<int*>(&(std::get<0>(distribution)[0])), const_cast<int*>(&(std::get<1>(distribution)[0])), 
		recv_msg.datatype, root, comm.mpi_communicator() )); 
}

template <typename T, typename V1, typename V2>
inline void gatherv(int root, const shiva::communicator& comm, const std::tuple<V1, V2>& distribution, const shiva::message<T>& send_msg)
{
	call_mpi( MPI_Gatherv(send_msg.data, send_msg.size, send_msg.datatype, nullptr, 
		const_cast<int*>(&(std::get<0>(distribution)[0])), const_cast<int*>(&(std::get<1>(distribution)[0])), 
		0, root, comm.mpi_communicator() )); 
}

template <typename T1, typename T2>
inline void allgather(const shiva::communicator& comm, const shiva::message<T1>& send_msg, const shiva::message<T2>& recv_msg)
{
	call_mpi( MPI_Allgather(send_msg.data, send_msg.size, send_msg.datatype, recv_msg.data, recv_msg.size, recv_msg.datatype, comm.mpi_communicator() ) ); 
}

template <typename T1, typename T2>
inline void scatter(int root, const shiva::communicator& comm, const shiva::message<T1>& send_msg, const shiva::message<T2>& recv_msg)
{
	const int size = comm.size();
	call_mpi( MPI_Scatter(send_msg.data, send_msg.size / size, send_msg.datatype, recv_msg.data, recv_msg.size, recv_msg.datatype, root, comm.mpi_communicator() ));
}

template <typename T>
inline void scatter(int root, const shiva::communicator& comm, const shiva::message<T>& recv_msg)
{
	call_mpi( MPI_Scatter(nullptr, 0, 0, recv_msg.data, recv_msg.size, recv_msg.datatype, root, comm.mpi_communicator() ));
}

template <typename T1, typename T2, typename V1, typename V2>
inline void scatterv(int root, const shiva::communicator& comm, const shiva::message<T1>& send_msg, const std::tuple<V1, V2>& distribution, const shiva::message<T2>& recv_msg)
{
	call_mpi( MPI_Scatterv(send_msg.data, const_cast<int*>(&(std::get<0>(distribution)[0])), const_cast<int*>(&(std::get<1>(distribution)[0])), 
		send_msg.datatype, recv_msg.data, recv_msg.size, recv_msg.datatype, root, comm.mpi_communicator() )); 
}

template <typename T, typename V1, typename V2>
inline void scatterv(int root, const shiva::communicator& comm,  const std::tuple<V1, V2>& distribution, const shiva::message<T>& recv_msg)
{
	call_mpi( MPI_Scatterv(nullptr, const_cast<int*>(&(std::get<0>(distribution)[0])), const_cast<int*>(&(std::get<1>(distribution)[0])), 
		MPI_DATATYPE_NULL, recv_msg.data, recv_msg.size, recv_msg.datatype, root, comm.mpi_communicator() )); 
}

template <typename OpType, typename T1, typename T2>
inline void allreduce(const shiva::communicator& comm, const shiva::message<T1>& send_msg, const shiva::message<T2>& recv_msg)
{
	call_mpi( MPI_Allreduce(send_msg.data, recv_msg.data, send_msg.size, send_msg.datatype, shiva::detail::mpi_operation_helper<OpType>::value, comm.mpi_communicator() ));
}

}