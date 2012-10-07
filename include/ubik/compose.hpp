#pragma once
#include <boost/mpl/assert.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/transform.hpp>

namespace ubik
{
namespace mpl = boost::mpl;

template<class TL>
class compose : public mpl::inherit_linearly<TL, mpl::inherit<mpl::_1, mpl::_2> >::type
{
private:
	template<class T>
	struct get_bit_width
	{
		typedef mpl::int_<T::bit_width> type;
	};
public:
	compose(unsigned char* bits)
	{
		bit_accessor::set_bits(bits);
	}

	template<class T>
	typename T::return_type get()
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		typedef typename mpl::erase<
				TL,
				typename mpl::find<TL, T>::type,
				typename mpl::end<TL>::type
			>::type preds; // fields before T...
		typedef typename mpl::transform<
				preds,
				get_bit_width<mpl::_1>
			>::type bit_widths; // ...and their bit widths...
		static const int idx = mpl::fold<
				bit_widths,
				mpl::int_<0>,
				mpl::plus<mpl::_1, mpl::_2>
			>::type::value; // ...summed.
		BOOST_STATIC_ASSERT(idx >= 0);
		return T::template get<idx%8>(idx/8);
	}

	template<class T>
	typename T::iterator begin()
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		return T::begin();
	}

	template<class T>
	typename T::iterator end()
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		return T::end();
	}
};

}

