#pragma once
#include <boost/mpl/vector.hpp>
#include <ubik/compose.hpp>

namespace ubik
{

template<class T>
class loop
{
public:
	enum { bit_width = 0 };
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

