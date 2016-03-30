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

template<typename T, typename F, typename Yes>
struct Members
{
    template<typename Has = Yes>
    static auto table() -> typename std::enable_if<Has::value, F>::type
    {
        return T::table;
    }
    template<typename Has = Yes>
    static auto table() -> typename std::enable_if<!Has::value, F>::type
    {
        return nullptr;
    }
    template<typename Has = Yes>
    static auto size() -> typename std::enable_if<Has::value, unsigned int>::type
    {
        return sizeof(T::table) / sizeof(T::table[0]);
    }
    template<typename Has = Yes>
    static auto size() -> typename std::enable_if<!Has::value, unsigned int>::type
    {
        return 0;
    }
};

template<typename T>
struct Has
{
    template<class C> static std::true_type test(decltype(C::table));
    template<class C> static std::false_type test(...);
    using Table = decltype(test<T>(0));
};

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

#define U_DECLARE(Member, Class) \
namespace umof \
{ \
    template<> \
    struct Member::Holder<Class> \
    { \
        using UClass = Class; \
        static const detail::Member ## Table table[]; \
    }; \
} \
const umof::detail::Member ## Table umof::Member::Holder<Class>::table[]

#define U_DECALRE_METHODS(Class) U_DECLARE(Method, Class)
#define U_DECALRE_PROPERTIES(Class) U_DECLARE(Property, Class)
#define U_DECALRE_ENUMS(Class) U_DECLARE(Enum, Class)

#define U_DECLARE_API(Class) \
namespace umof \
{ \
    template<> \
    struct Api::Holder<Class> \
    { \
        using UClass = Class; \
        static const detail::ApiTable table; \
    }; \
    const detail::ApiTable Api::Holder<MTest>::table \
    { \
        #Class, \
        nullptr, \
        Members<Method::Holder<Class>, const detail::MethodTable *, Has<Method::Holder<Class>>::Table>::table(), \
        Members<Property::Holder<Class>, const detail::PropertyTable *, Has<Property::Holder<Class>>::Table>::table(), \
        Members<Enumerator::Holder<Class>, const detail::EnumTable *, Has<Enumerator::Holder<Class>>::Table>::table(), \
        Members<Method::Holder<Class>, const detail::MethodTable *, Has<Method::Holder<Class>>::Table>::size(), \
        Members<Property::Holder<Class>, const detail::PropertyTable *, Has<Property::Holder<Class>>::Table>::size(), \
        Members<Enumerator::Holder<Class>, const detail::EnumTable *, Has<Enumerator::Holder<Class>>::Table>::size(), \
    }; \
}

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
    umof::Api::Holder<UClass>::UApi::table.name, \
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
