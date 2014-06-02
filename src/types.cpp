#include "types.h"

boost::any Converter<int>::convert(const boost::any& a)
{
	if (typeid(float) == a.type())
		return helper<int, double>::convert(a);
	else if (typeid(const char *) == a.type())
		return helper<int, const char *>::convert(a);

	return boost::any_cast<int>(a);
}

bool Converter<int>::canConvert(const boost::any& a)
{
	return (typeid(int) == a.type()
		|| typeid(float) == a.type()
		|| typeid(const char *) == a.type());
}

boost::any Converter<float>::convert(const boost::any& a)
{
	if (typeid(int) == a.type())
		return boost::any(helper<float, int>::convert(a));
	else if (typeid(const char *) == a.type())
		return boost::any(helper<float, const char *>::convert(a));

	return boost::any_cast<float>(a);
}

bool Converter<float>::canConvert(const boost::any& a)
{
	return (typeid(float) == a.type()
		|| typeid(int) == a.type()
		|| typeid(const char *) == a.type());
}