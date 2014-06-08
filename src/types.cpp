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