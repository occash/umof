#ifndef TYPES_H
#define TYPES_H

#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

template<class U, class Y>
struct helper
{
	inline static U convert(boost::any a)
	{
		return static_cast<U>(boost::any_cast<Y>(a));
	}
};

template<class U>
struct helper <U, const char *>
{
	inline static U convert(boost::any a)
	{
		return boost::lexical_cast<U>(boost::any_cast<const char *>(a));
	}
};

template<class T>
struct Converter
{
	static boost::any convert(const boost::any &a)
	{
		return boost::any_cast<T>(a);
	}

	static bool canConvert(const boost::any &a)
	{
		return typeid(T) == a.type();
	}
};

template<>
struct Converter<int>
{
	static boost::any convert(const boost::any &a);
	static bool canConvert(const boost::any &a);
};

template<>
struct Converter <float>
{
	static boost::any convert(const boost::any &a);
	static bool canConvert(const boost::any &a);
};

#endif