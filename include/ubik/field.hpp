#pragma once
#include <boost/type.hpp>

namespace ubik
{

template<unsigned bits, class RT>
class field
{
public:
	typedef RT return_type;

	return_type get()
	{
		return return_type();
	}
};

}

