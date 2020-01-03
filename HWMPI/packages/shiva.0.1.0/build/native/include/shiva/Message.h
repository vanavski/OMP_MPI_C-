
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <mpi.h>
#include <shiva\utility.h>
#include <type_traits>
#include <algorithm>
#include <memory>

namespace shiva {



	
template<typename T>
struct message
{
	message(T* data, MPI_Datatype datatype, int size) : data(data), datatype(datatype), size(size) {}
	
	T* data;
	MPI_Datatype datatype;
	int size;
};

template <typename T>
inline message<T> make_message(T* data, MPI_Datatype datatype, int size)
{
	return message<T>(data, datatype, size);
}

template <typename T, typename D>
inline message<T> make_message(std::unique_ptr<T, D>& data)
{
	detail::check_for_unusuable_types<T>();
	return shiva::message<T>(data.get(), detail::mpi_datatype<T>::value, 1);
}

// make array for fundemental types
template <typename T>
inline typename std::enable_if<std::is_fundamental<T>::value, shiva::message<T>>::type make_message(T& data)
{
	detail::check_for_unusuable_types<T>();
	return shiva::message<T>(&data, detail::mpi_datatype<T>::value, 1);
}
// make array for static arrays
template <typename T>
inline typename std::enable_if<std::is_fundamental<T>::value, shiva::message<T>>::type make_message(T* data, size_t size)
{
	detail::check_for_unusuable_types<T>();
	return shiva::message<T>(data, detail::mpi_datatype<T>::value, (int)(size)); 
}

template <typename T, typename D>
inline message<T> make_message(std::unique_ptr<T[], D>& data, size_t size)
{
	detail::check_for_unusuable_types<T>();
	return shiva::message<T>(data.get(), detail::mpi_datatype<T>::value, (int)size);
}

// make array for standard container types
template <typename T>
inline typename std::enable_if<detail::is_standard_container<T>::value, shiva::message<typename T::value_type>>::type make_message(T& container)
{
	detail::check_for_unusuable_types<typename T::value_type>();
	return shiva::message<typename T::value_type>(container.data(), detail::mpi_datatype<typename T::value_type>::value, (int) (container.size()) );
}

inline shiva::message<void> inplace_message() 
{
	return shiva::message<void>(MPI_IN_PLACE, MPI_DATATYPE_NULL, 0);
}

}