#pragma once
#include <boost/mpl/assert.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <ubik/bit_width.hpp>

namespace ubik
{
namespace mpl = boost::mpl;

template<class TL>
class compose : public mpl::inherit_linearly<TL, mpl::inherit<mpl::_1, mpl::_2> >::type
{
private:
	template<class T>
	struct predecessors
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		typedef typename mpl::erase<
				TL,
				typename mpl::find<TL, T>::type,
				typename mpl::end<TL>::type
			>::type type; // fields before T
	};

public:
	compose(const unsigned char* bits)
	{
		bit_accessor::set_bits(bits);
	}

	template<class T>
	typename T::return_type get() const
	{
		static const unsigned idx = const_bit_width<typename predecessors<T>::type>::value;
		return T::template get<idx%8>(idx/8);
	}

	template<class T>
	typename T::iterator begin() const
	{
		static const unsigned idx = const_bit_width<typename predecessors<T>::type>::value;
		assert(0 == idx%8);
		const unsigned char* bytes = bit_accessor::get_bits() + idx/8;
		return T::begin(bytes, *this);
	}

	template<class T>
	typename T::iterator end() const
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		return T::end();
	}
};

}

