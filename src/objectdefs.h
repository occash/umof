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

#ifndef OBJECTDEFS_H
#define OBJECTDEFS_H

#include <vector>
#include <list>
#include <map>
#include <initializer_list>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <algorithm>

#include "defines.h"
#include "any.h"
#include "enum.h"
#include "type.h"
#include "converters.h"
#include "methoddefs.h"
#include "propertydefs.h"
#include "method.h"
#include "property.h"
#include "api.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//Expose functions
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ReadSig, ReadSig RS, typename WriteSig, WriteSig WS>
struct PTable
{
	static PropertyTable *get()
	{
		static PropertyTable staticTable
		{
			Reader<ReadSig, RS>::type(),
			&Reader<ReadSig, RS>::read,
			&Writer<WriteSig, WS>::write
		};
		return &staticTable;
	}
};

#define METHOD(m) \
{ \
	#m, \
	(InvokeMem)&Invoker<decltype(&m)>::invoke<&m>, \
	Invoker<decltype(&m)>::argCount(), \
	Invoker<decltype(&m)>::types() \
}
#define OVERLOAD(c, m, r, ...) \
{ \
	#m, \
	(InvokeMem)&Invoker<r(c::*)(__VA_ARGS__)>::invoke<&m>, \
	Invoker<r(c::*)(__VA_ARGS__)>::argCount(), \
	Invoker<r(c::*)(__VA_ARGS__)>::types() \
}
#define FUNCTION(f) method<decltype(&f), &f>(#f)
#define PROPERTY(p, r, w) property<decltype(&r), &r, decltype(&w), &w>(#p)

template<typename T>
struct expose_method
{
private:
	typedef std::true_type yes;
	typedef std::false_type no;
	template<typename U>
	static auto test(int) -> decltype(std::declval<U>().expose(), yes())
	{
		return yes();
	}
	template<typename>
	static no test(...)
	{
		return no();
	}
	static const MethodTable *exec_impl(std::true_type)
	{
		return T::expose();
	}
	static const MethodTable *exec_impl(...)
	{
		return nullptr;
	}
public:
	static const MethodTable *exec()
	{
		return exec_impl(test<T>(0));
	}
	enum { exists = std::is_same<decltype(test<T>(0)), yes>::value };
};

#endif