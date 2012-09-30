#pragma once
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
		// write this: BOOST_STATIC_ASSERT( T in TL );
		return static_cast<T*>(this)->get();
	}

	template<class T>
	typename T::return_type begin()
	{
		return T::begin();
	}

	template<class T>
	typename T::return_type end()
	{
		return T::end();
	}
};

}

