#include <ubik/ubik.hpp>
#include <stdint.h>
#include <iostream>

using namespace ubik;
using namespace std;

struct tag : public field<8, uint8_t> {};
struct length : public field<8, uint8_t> {};
struct someval : public field<8, uint16_t> {};
struct test_loop : public loop<boost::mpl::vector<someval>, test_loop>
{
	template<class Context>
	unsigned get_size(const Context& c) const
	{
		return c.template get<length>();
	}
};

struct outer_loop : public loop<boost::mpl::vector<length, test_loop>, outer_loop>
{
	template<class Context>
	unsigned get_size(const Context& c) const
	{
		return c.template get<length>();
	}
};

typedef compose<boost::mpl::vector<tag, length, outer_loop> > desc;

void foo(unsigned char* bytes)
{
	desc d(bytes);
	uint8_t t = d.get<tag>();
	cout << "tag: " << (unsigned)t << endl;
	uint8_t l = d.get<length>();
	cout << "len: " << (unsigned)l << endl;
	for(outer_loop::iterator it = d.begin<outer_loop>(); it != d.end<outer_loop>(); ++it)
	{
		uint8_t l2 = it->get<length>();
		cout << "outer loop len: " << (unsigned)l2 << endl;
		for(test_loop::iterator it2 = it->begin<test_loop>(); it2 != it->end<test_loop>(); ++it2)
		{
			uint16_t s = it2->get<someval>();
			cout << "someval: " << (unsigned)s << endl;
		}
	}
}

struct text : public field<40, std::string> {};
struct texts : public loop<boost::mpl::vector<text>, texts>
{
	template<class Context>
	unsigned get_size(const Context& c) const
	{
		return c.template get<length>();
	}
};
typedef compose<boost::mpl::vector<length, texts> > desc2;

void bar(unsigned char* bytes)
{
	desc2 d(bytes);
	cout << endl;
	cout << "desc2 len: " << (unsigned)d.get<length>() << endl;
	for(texts::iterator it = d.begin<texts>(); it != d.end<texts>(); ++it)
	{
		std::string s = it->get<text>();
		cout << "text: " << s << endl;
	}
}

int main(int argc, char** argv)
{
	unsigned char data[] = { 1, 12, 2, 9, 8, 3, 7, 6, 5, 4, 4, 3, 2, 1 };
	foo(data);
	unsigned char data2[] = "\012Helloworld";
	bar(data2);
	return 0;
}

