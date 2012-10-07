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
public:
	compose(const unsigned char* bits)
	{
		bit_accessor::set_bits(bits);
	}

	template<class T>
	typename T::return_type get() const
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		typedef typename mpl::erase<
				TL,
				typename mpl::find<TL, T>::type,
				typename mpl::end<TL>::type
			>::type preds; // fields before T
		static const int idx = const_bit_width<preds>::value;
		return T::template get<idx%8>(idx/8);
	}

	template<class T>
	typename T::iterator begin() const
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		// TODO: calculate pointer and pass it here
		return T::begin(NULL);
	}

	template<class T>
	typename T::iterator end() const
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		return T::end();
	}
};

}

