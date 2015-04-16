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

#ifndef METHODDEFS_H
#define METHODDEFS_H

#include "type_traits.h"

//Helper stuff
namespace unpack
{
	template<unsigned...> struct indices {};

	template<unsigned N, unsigned... Is>
	struct indices_gen : indices_gen <N - 1, N - 1, Is...> {};

	template<unsigned... Is>
	struct indices_gen<0, Is...> : indices <Is...> {};

    template<unsigned N, typename... R>
    struct type_at;

    template<unsigned N, typename T, typename... R>
    struct type_at<N, T, R...>
    {
        using type = typename type_at<N - 1, R...>::type;
    };

    template<typename T, typename... R>
    struct type_at<0, T, R...>
    {
        using type = T;
    };
}

//Get return type table
template<typename T, typename Void>
struct ReturnTable;

template<typename T>
struct ReturnTable<T, True>
{
    inline static TypeTable *get()
    {
        return nullptr;
    }
};

template<typename T>
struct ReturnTable<T, False>
{
    inline static TypeTable *get()
    {
        return Table<T>::get();
    }
};

//Function to unpack args properly
template<typename T>
inline static TypeTable *getTable()
{
	return Table<T>::get();
}

template<typename C, typename R, typename... Args>
struct ArgumentsBase
{
    template<unsigned Is>
    using Type = typename unpack::type_at<Is, Args...>::type;
    using Class = typename C;
    using Return = typename R;

    using IsFree = typename std::is_void<Class>::type;
    using IsVoid = typename std::is_void<Return>::type;

    constexpr static unsigned count = sizeof...(Args);

    inline static const TypeTable **types()
    {
        static const TypeTable *staticTypes[] =
        {
            ReturnTable<Return, IsVoid>::get(),
            getTable<Args>()...
        };
        return staticTypes;
    }
};

template<typename Signature>
struct MethodArguments
{
    static_assert(std::is_function<Signature>::value, "The argument should be a function pointer");

    constexpr static unsigned count = 0;
    constexpr static unsigned size = 0;
    inline static const TypeTable **types();
};

//Regular function
template<typename Return, typename... Args>
struct MethodArguments<Return(*)(Args...)> : ArgumentsBase<void, Return, Args...> {};

//Function with vararg
template<typename Return, typename... Args>
struct MethodArguments<Return(*)(Args..., ...)> : ArgumentsBase<void, Return, Args...> {};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args..., ...)> : ArgumentsBase<Class, Return, Args...>{};

//Member function
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)> : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)const> : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)volatile> : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)const volatile> : ArgumentsBase<Class, Return, Args...>{};

//Function with lvalue ref qualifier
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)&> : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)const&> : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)volatile&> : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)const volatile&> : ArgumentsBase<Class, Return, Args...>{};

//Function with rvalue ref qualifier
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...) && > : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)const&&> : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)volatile&&> : ArgumentsBase<Class, Return, Args...>{};
template<typename Return, typename Class, typename... Args>
struct MethodArguments<Return(Class::*)(Args...)const volatile&&> : ArgumentsBase<Class, Return, Args...>{};

template<typename Class, typename Function, Function func>
struct CallBase;

template<typename Function, Function func>
struct CallBase<True, Function, func>
{
    using Args = typename MethodArguments<Function>;
    using Return = typename Args::Return;

    template<typename R = Return, typename F, unsigned... Is>
    inline static typename std::enable_if<!std::is_void<R>::value, void>::type
        call_impl(F f, void *object, void *ret, void **stack, unpack::indices<Is...>)
    {
        *(Return*)ret = f(
            *(typename Args::Type<Is> *)stack[Is]...
        );
    }

    template<typename R = Return, typename F, unsigned... Is>
    inline static typename std::enable_if<std::is_void<R>::value, void>::type
        call_impl(F f, void *object, void *ret, void **stack, unpack::indices<Is...>)
    {
        f(
            *(typename Args::Type<Is> *)stack[Is]...
        );
    }

    inline static void call(void *object, void *ret, void **stack)
    {
        call_impl(func, object, ret, stack, unpack::indices_gen<Args::count>());
    }
};

template<typename Function, Function func>
struct CallBase<False, Function, func>
{
    using Args = typename MethodArguments<Function>;
    using Return = typename Args::Return;
    using Class = typename Args::Class;

    template<typename R = Return, typename F, unsigned... Is>
    inline static typename std::enable_if<!std::is_void<R>::value, void>::type
        call_impl(F f, void *object, void *ret, void **stack, unpack::indices<Is...>)
    {
        *(Return*)ret = (static_cast<Class *>(object)->*f)(
            *(typename Args::Type<Is> *)stack[Is]...
        );
    }

    template<typename R = Return, typename F, unsigned... Is>
    inline static typename std::enable_if<std::is_void<R>::value, void>::type
        call_impl(F f, void *object, void *ret, void **stack, unpack::indices<Is...>)
    {
        (static_cast<Class *>(object)->*f)(
            *(typename Args::Type<Is> *)stack[Is]...
        );
    }

    inline static void call(void *object, void *ret, void **stack)
    {
        call_impl(func, object, ret, stack, unpack::indices_gen<Args::count>());
    }
};

template<typename S, S s>
struct MethodCall : public CallBase<typename MethodArguments<S>::IsFree, S, s> {};

#endif