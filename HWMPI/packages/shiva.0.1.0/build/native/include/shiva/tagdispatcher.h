
//          Copyright Tunc Bahcecioglu 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <limits>

namespace shiva {

class tag_dispatcher
{
public:
	tag_dispatcher() : _tag(0)	{}

	int operator() ()
	{
		return _tag++;
	}

private:
	int _tag;
};

}