#pragma once
#include <stdint.h> // FIXME: not portable!
#include <ubik/convert.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/if.hpp>

namespace ubik
{

template<class T, unsigned start_bit, unsigned num_bits>
struct get_from_bits_simple
{
	static T eval(const unsigned char* bits)
	{
		// TODO: test this
		// FIXME: when start_bit+num_bits > 32, some data is lost
		return convert<T, uint32_t>((*reinterpret_cast<const uint32_t*>(bits) & ((1<<num_bits)-1)) << start_bit);
	}
};

template<class T, unsigned start_bit, unsigned num_bits>
struct get_from_bits_complex
{
	static T eval(const unsigned char* bits)
	{
		return convert<T>(bits, num_bits/8);
	}
};

class bit_accessor
{
public:
	bit_accessor()
		: m_bits(NULL)
	{
	}
	
	void set_bits(const unsigned char* bits)
	{
		m_bits = bits;
	}

	template<class T, unsigned start_bit, unsigned num_bits>
	T get_from_bits(unsigned byte_offset) const
	{
		BOOST_STATIC_ASSERT(start_bit < 7);
		return boost::mpl::if_c<
				num_bits <= 32,
				get_from_bits_simple<T, start_bit, num_bits>,
				get_from_bits_complex<T, start_bit, num_bits>
			>::type::eval(m_bits+byte_offset);
	}

private:
	const unsigned char* m_bits;
};

}

