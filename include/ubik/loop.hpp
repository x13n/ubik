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
	enum { bit_width = 0 };
	static const bool const_sized = false;

	class iterator
	{
	private:
		typedef compose<T> contents_type;

		const unsigned char* m_bytes;
		const unsigned char* m_bytes_end;
		contents_type m_contents;

	public:
		iterator(const unsigned char* bytes, const unsigned char* bytes_end)
			: m_bytes(bytes)
			, m_bytes_end(bytes_end)
			, m_contents(bytes)
		{
		}

		void operator++()
		{
			m_bytes += m_contents.get_size();
			if(m_bytes >= m_bytes_end) {
				assert(m_bytes == m_bytes_end && "Loop is not aligned!");
				m_bytes = NULL;
			}
			new (&m_contents) contents_type(m_bytes); // initialize underlying content with m_bytes
			// looks kinda bizarre, but avoids moving set_bits to public interface
		}

		bool operator!=(const iterator& rhs)
		{
			return m_bytes != rhs.m_bytes;
		}

		const contents_type* operator->() const
		{
			return &m_contents;
		}

		const contents_type& operator*() const
		{
			return m_contents;
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
};

}

