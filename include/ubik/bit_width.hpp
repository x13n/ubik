#pragma once
#include <boost/mpl/transform.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/remove_if.hpp>

namespace ubik
{
namespace mpl = boost::mpl;

	// TODO: move contents of this file to some namespace ,,details''?
	template<class T>
	struct get_bit_width
	{
		typedef mpl::int_<T::bit_width> type;
	};

	template<class T>
	struct const_bit_width
	{
	private:
		typedef typename mpl::transform<
				T,
				get_bit_width<mpl::_1>
			>::type bit_widths;

		static int const int_value = mpl::fold<
				bit_widths,
				mpl::int_<0>,
				mpl::plus<mpl::_1, mpl::_2>
			>::type::value;

		BOOST_STATIC_ASSERT(int_value >= 0);
	public:
		static unsigned const value = static_cast<unsigned>(int_value);
	};

	template<class T>
	struct is_const_sized
	{
		typedef mpl::bool_<T::const_sized> type;
	};

	template<class TL>
	struct get_var_sized
	{
		typedef typename mpl::remove_if<
				TL,
				is_const_sized<mpl::_1>
			>::type type;
	};
}

