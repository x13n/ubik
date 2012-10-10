#pragma once
#include <boost/mpl/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <ubik/compose.hpp>

namespace ubik
{

template<class T, class Derived>
class loop
{
public:
	enum { bit_width = 0 }; // TODO: remove this?

	class iterator
	{
	private:
		typedef compose<T> contents_type;

		const unsigned char* m_bytes;
		const unsigned char* m_bytes_end;
		boost::shared_ptr<const contents_type> m_contents;

	public:
		iterator(const unsigned char* bytes, const unsigned char* bytes_end)
			: m_bytes(bytes)
			, m_bytes_end(bytes_end)
			, m_contents(new contents_type(bytes))
		{
		}

		void operator++()
		{
			m_bytes += byte_var_size();
			if(m_bytes >= m_bytes_end) {
				assert(m_bytes == m_bytes_end && "Loop is not aligned!");
				m_bytes = NULL;
			}
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

		const contents_type& operator*() const
		{
			return *m_contents;
		}

		unsigned byte_var_size() const
		{
			static const unsigned bit_const_size = const_bit_width<T>::value;
			BOOST_STATIC_ASSERT( 0 == bit_const_size % 8 );
			static const unsigned const_size = bit_const_size / 8;
			unsigned var_size = 0; // TODO: implement calculating inner var size
			return const_size + var_size;
		}
	};

	template<class Context>
	iterator begin(const unsigned char* bytes, const Context& c) const
	{
		BOOST_STATIC_ASSERT((boost::is_base_of<loop, Derived>::value));
		const unsigned char* bytes_end = bytes +
			static_cast<const Derived* const>(this)->template get_size(c);
		return iterator(bytes, bytes_end);
	}

	iterator end() const
	{
		return iterator(NULL, NULL);
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
};

}

