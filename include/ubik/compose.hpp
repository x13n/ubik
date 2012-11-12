#pragma once
#include <boost/mpl/assert.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <boost/mpl/for_each.hpp>
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

	typedef compose<TL> this_type;

	struct size_getter
	{
		template<class T>
		void operator() (T t)
		{
			*m_size += t.template get_size(*m_parent);
		}

		size_getter(const this_type *parent, unsigned *size)
			: m_size(size)
			, m_parent(parent)
		{}
	private:
		unsigned *m_size;
		const this_type *m_parent;
	};

	template<class SubTL>
	unsigned get_var_byte_size() const
	{
		typedef typename get_var_sized<SubTL>::type var_sized;

		unsigned size = 0u;
		size_getter s(this, &size);
		mpl::for_each<var_sized>(s);
		return size;
	}

public:
	compose(const unsigned char* bits)
	{
		bit_accessor::set_bits(bits);
	}

	template<class T>
	typename T::return_type get() const
	{
		typedef typename predecessors<T>::type preds;
		static const unsigned idx = const_bit_width<preds>::value;
		return T::template get<idx%8>(get_var_byte_size<preds>() + idx/8);
	}

	template<class T>
	typename T::iterator begin() const
	{
		typedef typename predecessors<T>::type preds;
		static const unsigned idx = const_bit_width<preds>::value;
		BOOST_STATIC_ASSERT(0 == idx%8);

		const unsigned char* bytes = bit_accessor::get_bits() + get_var_byte_size<preds>() + idx/8;
		return T::begin(bytes, *this);
	}

	template<class T>
	typename T::iterator end() const
	{
		BOOST_MPL_ASSERT( (mpl::contains<TL,T>) );
		return T::end();
	}

	unsigned get_size() const
	{
		static unsigned const const_bit_size = const_bit_width<TL>::value;
		BOOST_STATIC_ASSERT(0 == const_bit_size%8);

		return get_var_byte_size<TL>() + const_bit_size/8;
	}
};

}

