#pragma once
#include <boost/type.hpp>
#include <ubik/bits.hpp>

namespace ubik
{

template<unsigned BitWidth, class RT>
class field
	: protected virtual bit_accessor
{
protected:
	field() { }

	typedef RT return_type;
	enum { bit_width = BitWidth };
	static const bool const_sized = true;

	template<unsigned bit_offset>
	return_type get(unsigned byte_offset) const
	{
		return get_from_bits<return_type, bit_offset, bit_width>(byte_offset);
	}
};

}

