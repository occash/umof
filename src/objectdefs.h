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

#include "defines.h"
#include "method.h"
#include "methoddefs.h"
#include "property.h"
#include "propertydefs.h"
#include "enum.h"
#include "enumdefs.h"
#include "api.h"

#include <type_traits>
#include <utility>

#define U_METHOD(m) \
{ \
	#m, \
	(InvokeMem)&Invoker<decltype(&m)>::template invoke<&m>, \
	Invoker<decltype(&m)>::argCount(), \
	Invoker<decltype(&m)>::types() \
}
#define U_OVERLOAD(c, m, s) \
{ \
	#m, \
	(InvokeMem)&Invoker<s>::template invoke<&m>, \
	Invoker<s>::argCount(), \
	Invoker<s>::types() \
}
#define U_FUNCTION(f) U_METHOD(f)
#define U_PROPERTY(p, r, w) \
{ \
	#p, \
	Reader<decltype(&r), &r>::type(), \
	&Reader<decltype(&r), &r>::read, \
	&Writer<decltype(&w), &w>::write \
}
#define U_STRINGIFY(S) #S
#define U_NARG(...)  (U_NARG_(__VA_ARGS__,U_RSEQ_N()) - (sizeof(#__VA_ARGS__) == 1))
#define U_NARG_(...)  U_ARG_N(__VA_ARGS__)  
#define U_ARG_N( \
_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
_21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
_31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
_41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
_51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
_61, _62, _63, N, ...) N
#define U_RSEQ_N() \
63, 62, 61, 60, \
59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define U_VALUE(E, V) { U_STRINGIFY(V), E::V }
#define U_ENUMERATE(E, ...) \
{ \
	U_STRINGIFY(E), U_NARG(__VA_ARGS__), \
	[](){ \
		static const EnumeratorTable table[] \
		{ \
			__VA_ARGS__ \
		}; \
		return table; \
	}() \
}

/*! This macro exposes class methods in Api.
\relates Object
*/
#define U_EXPOSE(...) \
public: \
	static const std::pair<int, const MethodTable *> expose() \
		{ \
		static const MethodTable methods[] \
				{ \
			__VA_ARGS__ \
				}; \
		return { sizeof(methods) / sizeof(MethodTable), methods }; \
		} \
private:

/*! This macro exposes class properties in Api.
\relates Object
*/
#define U_PROPERTIES(...) \
public: \
	static const std::pair<int, const PropertyTable *> expose_props() \
		{ \
		static const PropertyTable props[] \
				{ \
			__VA_ARGS__ \
				}; \
		return { sizeof(props) / sizeof(PropertyTable), props }; \
		} \
private:

/*! This macro exposes class enums in Api.
\relates Object
*/
#define U_ENUMERATORS(...) \
public: \
	static const std::pair<int, const EnumTable *> expose_enums() \
		{ \
		static const EnumTable enums[] \
				{ \
			__VA_ARGS__ \
				}; \
		return { sizeof(enums) / sizeof(EnumTable), enums }; \
		} \
private:

#define U_DECLARE_API(Class) class Class ## Api : public ApiHolder<Class>
#define U_API(Class) Class ## Api::api()

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
	static const std::pair<int, const MethodTable *> exec_impl(std::true_type)
	{
		return T::expose();
	}
	static const std::pair<int, const MethodTable *> exec_impl(...)
	{
		return{ 0, nullptr };
	}
public:
	static const std::pair<int, const MethodTable *> exec()
	{
		return exec_impl(test<T>(0));
	}
	enum { exists = std::is_same<decltype(test<T>(0)), yes>::value };
};

template<typename T>
struct expose_props_method
{
private:
	typedef std::true_type yes;
	typedef std::false_type no;
	template<typename U>
	static auto test(int) -> decltype(std::declval<U>().expose_props(), yes())
	{
		return yes();
	}
	template<typename>
	static no test(...)
	{
		return no();
	}
	static const std::pair<int, const PropertyTable *> exec_impl(std::true_type)
	{
		return T::expose_props();
	}
	static const std::pair<int, const PropertyTable *> exec_impl(...)
	{
		return{ 0, nullptr };
	}
public:
	static const std::pair<int, const PropertyTable *> exec()
	{
		return exec_impl(test<T>(0));
	}
	enum { exists = std::is_same<decltype(test<T>(0)), yes>::value };
};

template<typename T>
struct expose_enums_method
{
private:
	typedef std::true_type yes;
	typedef std::false_type no;
	template<typename U>
	static auto test(int) -> decltype(std::declval<U>().expose_enums(), yes())
	{
		return yes();
	}
	template<typename>
	static no test(...)
	{
		return no();
	}
	static const std::pair<int, const EnumTable *> exec_impl(std::true_type)
	{
		return T::expose_enums();
	}
	static const std::pair<int, const EnumTable *> exec_impl(...)
	{
		return{ 0, nullptr };
	}
public:
	static const std::pair<int, const EnumTable *> exec()
	{
		return exec_impl(test<T>(0));
	}
	enum { exists = std::is_same<decltype(test<T>(0)), yes>::value };
};

template<typename T>
struct ApiHolder
{
	static const Api *api()
	{
		static const ApiTable apiTable
		{
			"Test",
			nullptr,
			expose_method<T>::exec().second,
			expose_props_method<T>::exec().second,
			expose_enums_method<T>::exec().second,
			expose_method<T>::exec().first,
			expose_props_method<T>::exec().first,
			expose_enums_method<T>::exec().first
		};
		static const Api staticApi(&apiTable);
		return &staticApi;
	}
};

#endif