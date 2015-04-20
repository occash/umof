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

#ifndef UMOF_DETAIL_TYPE_H
#define UMOF_DETAIL_TYPE_H

#include "table.h"

#include <typeinfo>
#include <type_traits>

#if defined(__GNUC__)
#include <cxxabi.h>
#include <cstdlib>
struct memfree
{
    memfree(const char *n) : name(n) {}
    ~memfree() { free((void*)name); }
    const char *name;
};
#endif

//Bool template type
typedef std::true_type True;
typedef std::false_type False;

//Basic function implementation
template<typename T, typename Small>
struct TypeFuncs;

//Basic functions for small type
template<typename T>
struct TypeFuncs<T, True>
{
    static const char *get_name()
    {
#if defined(__GNUC__)
        static int status = -4;
        static memfree mem(abi::__cxa_demangle(typeid(T).name(), 0, 0, &status));
        return (status == 0) ? mem.name : typeid(T).name();
#else
        return typeid(T).name();
#endif
    }
    static int get_size()
    {
        return sizeof(T);
    }
    static void static_new(void** dest)
    {
        new (*dest) T();
    }
    static void construct(void** dest)
    {
        new (*dest) T();
    }
    static void static_delete(void** x)
    {
        reinterpret_cast<T*>(x)->~T();
    }
    static void destruct(void** x)
    {
        reinterpret_cast<T*>(x)->~T();
    }
    static void clone(void* const* src, void** dest)
    {
        new (dest)T(*reinterpret_cast<T const*>(src));
    }
    static void move(void* const* src, void** dest)
    {
        *reinterpret_cast<T*>(dest) =
            *reinterpret_cast<T const*>(src);
    }
};

//Basic functions for larger type
template<typename T>
struct TypeFuncs<T, False>
{
    static const char *get_name()
	{
#if defined(__GNUC__)
        static int status = -4;
        static memfree mem(abi::__cxa_demangle(typeid(T).name(), 0, 0, &status));
        return (status == 0) ? mem.name : typeid(T).name();
#else
        return typeid(T).name();
#endif
	}
	static int get_size()
	{
		return sizeof(T);
	}
	static void static_new(void** dest)
	{
		*dest = new T();
	}
	static void construct(void** dest)
	{
		new (*dest) T();
	}
	static void static_delete(void** x)
	{
		// destruct and free memory
		delete (*reinterpret_cast<T**>(x));
	}
	static void destruct(void** x)
	{
		// destruct only, we'll reuse memory
		(*reinterpret_cast<T**>(x))->~T();
	}
	static void clone(void* const* src, void** dest)
	{
		*dest = new T(**reinterpret_cast<T* const*>(src));
	}
	static void move(void* const* src, void** dest)
	{
		**reinterpret_cast<T**>(dest) =
			**reinterpret_cast<T* const*>(src);
	}
};

template<typename Type>
struct TableHelper
{
    using Small = typename std::integral_constant<bool, (sizeof(Type) <= sizeof(void*))>;

    inline static TypeTable *get()
    {
        static TypeTable staticTable
        {
            Small::value,
            TypeFuncs<Type, Small>::get_name,
            TypeFuncs<Type, Small>::get_size,
            TypeFuncs<Type, Small>::static_new,
            TypeFuncs<Type, Small>::construct,
            TypeFuncs<Type, Small>::static_delete,
            TypeFuncs<Type, Small>::destruct,
            TypeFuncs<Type, Small>::clone,
            TypeFuncs<Type, Small>::move
        };
        return &staticTable;
    }

    template<typename S = Small>
    inline static typename std::enable_if<S::value, void>::type clone(const Type **src, void **dest)
    {
        new (dest)Type(*reinterpret_cast<Type const*>(*src));
    }

    template<typename S = Small>
    inline static typename std::enable_if<!S::value, void>::type clone(const Type **src, void **dest)
    {
        *dest = new Type(**src);
    }

    template<typename S = Small>
    inline static typename std::enable_if<S::value, Type*>::type cast(void **object)
    {
        return reinterpret_cast<Type*>(object);
    }

    template<typename S = Small>
    inline static typename std::enable_if<!S::value, Type*>::type cast(void **object)
    {
        return reinterpret_cast<Type*>(*object);
    }
};

//Get the table for type
template<typename T>
struct Table
{
    using Decay = typename std::decay<T>::type;
    using Pointer = typename std::is_pointer<T>::type;
    using DecayPointer = typename std::add_pointer<
                         typename std::decay<
                         typename std::remove_pointer<Decay>
                         ::type>::type>::type;
    using Type = typename std::conditional<Pointer::value, DecayPointer, Decay>::type;

    inline static TypeTable *get()
    {
        return TableHelper<Type>::get();
    }

    inline static void clone(const Decay **src, void **dest)
    {
        TableHelper<Type>::clone(src, dest);
    }

    inline static Decay *cast(void **object)
    {
        return const_cast<Decay*>(TableHelper<Type>::cast(object));
    }
};

#endif //UMOF_DETAIL_TYPE_H