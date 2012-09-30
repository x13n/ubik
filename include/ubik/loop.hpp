#pragma once
#include <boost/mpl/vector.hpp>
#include "compose.hpp"

namespace ubik
{

template<class T>
class loop
{
public:
	class iterator : public compose<T>
	{
	public:
		iterator(unsigned char* bytes)
			: compose<T>(bytes)
		{
		}

		void operator++() {}
		bool operator!=(const iterator& rhs) { return false; }
	};

	iterator begin()
	{
		return iterator(NULL);
	}

	iterator end()
	{
		return iterator(NULL);
	}
};

}

