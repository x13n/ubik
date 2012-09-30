#pragma once
#include <boost/mpl/assert.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>

namespace ubik
{
namespace mpl = boost::mpl;

template<class TL>
class compose : public mpl::inherit_linearly<TL, mpl::inherit<mpl::_1, mpl::_2> >::type
{
public:
	compose(unsigned char*)
	{
	}

	template<class T>
	typename T::return_type get()
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		return T::get();
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

