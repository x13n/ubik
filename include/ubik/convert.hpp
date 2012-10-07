#pragma once

namespace ubik
{

template<class T, class From>
T convert(From source) // for up to 32/64 bits
{
	return T(source);
}

template<class T>
T convert(unsigned char* source, unsigned byte_size) // for more than 32/64 bits
{
	T t(source, byte_size);
	return t;
}

}

