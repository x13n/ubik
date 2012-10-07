#pragma once
#include <boost/mpl/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <ubik/compose.hpp>

namespace ubik
{

template<class T, class Derived>
class loop
{
public:
	enum { bit_width = 0 };

	class iterator
	{
	private:
		typedef Derived parent_type;
		typedef compose<T> contents_type;

		const unsigned char* m_bytes;
		const parent_type* m_parent;
		boost::shared_ptr<const contents_type> m_contents;

	public:
		iterator(const parent_type* parent, const unsigned char* bytes)
			: m_bytes(bytes)
			, m_parent(parent)
			, m_contents(new contents_type(bytes))
		{
		}

		void operator++()
		{
			m_bytes += byte_var_size();
			m_contents.reset(new contents_type(m_bytes)); // TODO: avoid reallocation
		}

		bool operator!=(const iterator& rhs)
		{
			return m_bytes != rhs.m_bytes;
		}

		const contents_type* operator->() const
		{
			return m_contents.get();
		}

		unsigned byte_var_size() const
		{
			static const unsigned bit_const_size = const_bit_width<T>::value;
			BOOST_STATIC_ASSERT( 0 == bit_const_size % 8 );
			static const unsigned const_size = bit_const_size / 8;
			static const unsigned var_size = 0; // TODO: implement calculating inner var size
			return const_size + var_size;
		}
	};

	iterator begin(const unsigned char* bytes) const
	{
		return make_iter(bytes);
	}

	iterator end() const
	{
		return make_iter(NULL);
	}

	unsigned byte_var_size()
	{
		unsigned size = 0;
		iterator it = begin();
		while( it != end() ) {
			size += it->byte_var_size();
			++it;
		}
		return size;
	}

private:
	iterator make_iter(const unsigned char* bytes) const
	{
		return iterator(static_cast<const Derived*>(this), bytes);
	}
};

}

