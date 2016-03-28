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

#pragma once

template<typename T>
struct UHolder
{
    static_assert(std::is_same<T, T>::value, "Api is not declared");
};

#define UP_DECLARE_HAS(F, FT) \
template<typename T> \
struct uHas ## F \
{ \
    template<class C> static std::true_type test(typename C::F *); \
    template<class C> static std::false_type test(...); \
    using Type = const FT; \
    using Yes = decltype(test<T>(0)); \
    template<typename Has = Yes> \
    static auto table() -> typename std::enable_if<Has::value, Type>::type \
    { return T::F::table; } \
    template<typename Has = Yes> \
    static auto table() -> typename std::enable_if<!Has::value, Type>::type \
    { return nullptr; } \
    template<typename Has = Yes> \
    static auto size() -> typename std::enable_if<Has::value, unsigned int>::type \
    { return sizeof(T::F::table) / sizeof(T::F::table[0]); } \
    template<typename Has = Yes> \
    static auto size() -> typename std::enable_if<!Has::value, unsigned int>::type \
    { return 0; } \
};

UP_DECLARE_HAS(UMethods, umof::detail::MethodTable *)
UP_DECLARE_HAS(UProperties, umof::detail::PropertyTable *)
UP_DECLARE_HAS(UEnums, umof::detail::EnumTable *)

#define UP_HAS(F, T) uHas ## F<UHolder<T>>

#define UP_STRINGIFY(S) #S
#define UP_NARG(...)  (UP_NARG_(__VA_ARGS__,UP_RSEQ_N()) - (sizeof(#__VA_ARGS__) == 1))
#define UP_NARG_(...)  UP_ARG_N(__VA_ARGS__)  
#define UP_ARG_N( \
_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
_21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
_31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
_41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
_51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
_61, _62, _63, N, ...) N
#define UP_RSEQ_N() \
63, 62, 61, 60, \
59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define UP_GET_MEMBERS_(_1,_2,_3, N,...) N
#define UP_GET_MEMBERS(tuple) UP_GET_MEMBERS_ tuple
#define UP_MEMBER_0()
#define UP_MEMBER_1(var1) var1
#define UP_MEMBER_2(var1,var2) var2 var1
#define UP_MEMBER_3(var1,var2,var3) var1 var2 var3
#define UP_MEMBERS(...) UP_GET_MEMBERS((__VA_ARGS__, \
    UP_MEMBER_3, UP_MEMBER_2, \
    UP_MEMBER_1, UP_MEMBER_0))(__VA_ARGS__)

#define METHODS \
    struct UMethods { static const umof::detail::MethodTable table[]; };
#define PROPS \
    struct UProperties { static const umof::detail::PropertyTable table[]; };
#define ENUMS \
    struct UEnums { static const umof::detail::EnumTable table[]; };

#define U_DECLARE_API(C, ...) \
template<> \
struct UHolder<C> \
{ \
    using UClass = C; \
    UP_MEMBERS(__VA_ARGS__) \
    struct UApi { static const umof::detail::ApiTable table; }; \
    static const umof::Api *api() \
    { \
        static const umof::Api a(&UApi::table); \
        return &a; \
    } \
}; \
const umof::detail::ApiTable UHolder<C>::UApi::table \
{ \
    #C, \
    nullptr, \
    UP_HAS(UMethods, UClass)::table(), \
    UP_HAS(UProperties, UClass)::table(), \
    UP_HAS(UEnums, UClass)::table(), \
    UP_HAS(UMethods, UClass)::size(), \
    UP_HAS(UProperties, UClass)::size(), \
    UP_HAS(UEnums, UClass)::size(), \
};

#define U_API(C) UHolder<C>::api()

/*! This macro exposes class methods in Api.
\relates Object
*/
#define U_DECLARE_METHODS(C) \
    const umof::detail::MethodTable UHolder<C>::UMethods::table[]

/*! This macro exposes class properties in Api.
\relates Object
*/
#define U_DECLARE_PROPERTIES(C) \
    const umof::detail::PropertyTable UHolder<C>::UProperties::table[]

/*! This macro exposes class enums in Api.
\relates Object
*/
#define U_DECLARE_ENUMS(C) \
    const umof::detail::EnumTable UHolder<C>::UEnums::table[]

#define U_METHOD(method) \
{ \
	#method, \
	&umof::detail::MethodCall<decltype(&UClass::method), &UClass::method>::call, \
	umof::detail::MethodArguments<decltype(&UClass::method)>::count, \
	umof::detail::MethodArguments<decltype(&UClass::method)>::types() \
}

#define U_OVERLOAD(method, signature) \
{ \
	#method, \
	&umof::detail::MethodCall<signature, &UClass::method>::call, \
	umof::detail::MethodArguments<signature>::count, \
	umof::detail::MethodArguments<signature>::types() \
}

#define U_FUNCTION(function) \
{ \
    #function, \
    &umof::detail::MethodCall<decltype(&function), &function>::call, \
    umof::detail::MethodArguments<decltype(&function)>::count, \
    umof::detail::MethodArguments<decltype(&function)>::types() \
}

#define U_CONSTRUCTOR(...) \
{ \
    UHolder<UClass>::UApi::table.name, \
    &umof::detail::ConstructorCall<UClass, __VA_ARGS__>::call, \
    umof::detail::MethodArguments<decltype(&Constructor<UClass, __VA_ARGS__>::call)>::count, \
    umof::detail::MethodArguments<decltype(&Constructor<UClass, __VA_ARGS__>::call)>::types() \
}

#define UP_NULL
#define UP_PROPS_1(type, member) \
    umof::detail::PropertyAccessor<type, decltype(&UClass::member), &UClass::member>::read, \
    umof::detail::PropertyAccessor<type, decltype(&UClass::member), &UClass::member>::write
#define UP_PROPS_2(type, getter, setter) \
    &umof::detail::MethodReader<type, decltype(&UClass::getter), &UClass::getter>::read, \
    &umof::detail::MethodWriter<type, decltype(&UClass::setter), &UClass::setter>::write
#define UP_PROPS(type, ...) UP_GET_MEMBERS((__VA_ARGS__, \
    UP_NULL, UP_PROPS_2, \
    UP_PROPS_1, UP_NULL))(type, __VA_ARGS__)

#define MEMBER(member) member
#define READ(getter) getter
#define WRITE(setter) setter

#define U_PROPERTY(type, name, ...) \
{ \
	#name, \
	umof::detail::Type<type>::table(), \
	UP_PROPS(type, __VA_ARGS__) \
}

#define U_VALUE(E, V) { UP_STRINGIFY(V), E::V }

#define U_ENUMERATE(E, ...) \
{ \
	UP_STRINGIFY(E), UP_NARG(__VA_ARGS__), \
	[](){ \
		static const umof::detail::EnumeratorTable table[] \
		{ \
			__VA_ARGS__ \
		}; \
		return table; \
	}() \
}
