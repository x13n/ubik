#include <ubik/ubik.hpp>
#include <stdint.h>
#include <iostream>

using namespace ubik;
using namespace std;

struct tag : public field<8, uint8_t> {};
struct length : public field<8, uint8_t> {};
struct someval : public field<16, uint16_t> {};
// TODO: design a way to specify iteration end condition
struct test_loop : public loop<boost::mpl::vector<someval> > {};

typedef compose<boost::mpl::vector<tag, length, test_loop> > desc;

void foo(unsigned char* bytes)
{
	desc d(bytes);
	uint8_t t = d.get<tag>();
	cout << "tag: " << (unsigned)t << endl;
	uint8_t l = d.get<length>();
	cout << "len: " << (unsigned)l << endl;
	for(test_loop::iterator it = d.begin<test_loop>(); it != d.end<test_loop>(); ++it)
	{
		uint16_t s = it.get<someval>();
		cout << "someval: " << (unsigned)s << endl;
	}
}

int main(int argc, char** argv)
{
	foo(NULL);
	return 0;
}
