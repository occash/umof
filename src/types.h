/*********************************************************************
This file is part of the uMOF library.
Copyright (C) 2014 Artem Shal
artiom.shal@gmail.com

The uMOF library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.
**********************************************************************/

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