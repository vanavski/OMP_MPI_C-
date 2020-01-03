
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <mpi.h>

#include <array>

namespace shiva {

struct max;
struct min;
struct sum;
struct prod;
struct maxloc;
struct minloc;
struct bor;
struct bxor;
struct land;
struct lor;
struct lxor;
struct replace;

}


namespace shiva { namespace detail {

template <typename T> struct mpi_datatype { };
template <> struct mpi_datatype<char   > { static const MPI_Datatype value = MPI_CHAR; };
template <> struct mpi_datatype<short  > { static const MPI_Datatype value = MPI_SHORT; };
template <> struct mpi_datatype<int    > { static const MPI_Datatype value = MPI_INT; };
template <> struct mpi_datatype<long   > { static const MPI_Datatype value = MPI_LONG; };
template <> struct mpi_datatype<float  > { static const MPI_Datatype value = MPI_FLOAT; };
template <> struct mpi_datatype<double > { static const MPI_Datatype value = MPI_DOUBLE; };
template <> struct mpi_datatype<__int64> { static const MPI_Datatype value = MPI_LONG_LONG_INT; };
#if _WIN64
template <> struct mpi_datatype<size_t> { static const MPI_Datatype value = MPI_UNSIGNED_LONG_LONG; };
#else
template <> struct mpi_datatype<size_t> { static const MPI_Datatype value = MPI_UNSIGNED; };
#endif

template <typename T> struct is_standard_container {static const bool value = false;};
template <typename T, typename A> struct is_standard_container<std::vector<T,A>> {static const bool value = true;};
template <typename T, size_t N  > struct is_standard_container<std::array<T,N> > {static const bool value = true;};

template <typename T>
void check_for_unusuable_types()
{
	static_assert(!std::is_same<T, bool>::value, "Mpi does not understand bool type, use int instead");
}

template <typename T> struct mpi_operation_helper {};
template <> struct mpi_operation_helper<shiva::max> 	{ static const MPI_Op value = MPI_MAX; };
template <> struct mpi_operation_helper<shiva::min> 	{ static const MPI_Op value = MPI_MIN; };
template <> struct mpi_operation_helper<shiva::sum> 	{ static const MPI_Op value = MPI_SUM; };
template <> struct mpi_operation_helper<shiva::prod> 	{ static const MPI_Op value = MPI_PROD; };
template <> struct mpi_operation_helper<shiva::maxloc> 	{ static const MPI_Op value = MPI_MAXLOC; };
template <> struct mpi_operation_helper<shiva::minloc> 	{ static const MPI_Op value = MPI_MINLOC; };
template <> struct mpi_operation_helper<shiva::bor> 	{ static const MPI_Op value = MPI_BOR; };
template <> struct mpi_operation_helper<shiva::bxor> 	{ static const MPI_Op value = MPI_BXOR; };
template <> struct mpi_operation_helper<shiva::land> 	{ static const MPI_Op value = MPI_LAND; };
template <> struct mpi_operation_helper<shiva::lor> 	{ static const MPI_Op value = MPI_LOR; };
template <> struct mpi_operation_helper<shiva::lxor> 	{ static const MPI_Op value = MPI_LXOR; };
template <> struct mpi_operation_helper<shiva::replace> { static const MPI_Op value = MPI_REPLACE; };

}}
