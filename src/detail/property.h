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

#ifndef UMOF_DETAIL_PROPERTY_H
#define UMOF_DETAIL_PROPERTY_H

#include "type.h"
#include "method.h"

template<typename T, typename C>
struct FieldBase
{
    using Type = T;
    using Class = C;
};

template<typename T>
struct Field;

template<typename T, typename C>
struct Field<T(C::*)> : public FieldBase<T, C> {};

template<typename T, typename Method, Method method>
struct MethodReader
{
    using Args = MethodArguments<Method>;
    using Return = typename Args::Return;
    using Class = typename Args::Class;

    using ReturnType = typename Table<Return>::Type;
    using Type = typename Table<T>::Type;

    static_assert(Args::count == 0, "Reader method should not receive arguments");
    static_assert(std::is_same<Type, ReturnType>::value, "Reader type is different");

    inline static void read(const void *obj, const void *ret)
    {
        *(T*)ret = (static_cast<Class *>(const_cast<void *>(obj))->*method)();
    }
};

template<typename T, typename Method, Method method>
struct MethodWriter
{
    using Args = MethodArguments<Method>;
    using Class = typename Args::Class;
    using Type = typename Table<T>::Type;
    using SetType = typename Table<typename Args::SetArg>::Type;

    static_assert(Args::count == 1, "Writer should have one argument");
    static_assert(std::is_same<SetType, Type>::value, "Writer type is different");

    inline static void write(const void *obj, const void *val)
    {
        (static_cast<Class *>(const_cast<void *>(obj))->*method)(*Args::type<0>(&val));
    }
};

template<typename T, typename Member, Member field>
struct FieldAccessor
{
    using MemberField = typename Field<Member>::Type;
    using Class = typename Field<Member>::Class;
    using FieldType = typename Table<MemberField>::Type;
    using Type = typename Table<T>::Type;

    static_assert(std::is_same<Type, FieldType>::value, "Field type is different");

    inline static void read(const void *obj, const void *ret)
    {
        *(T*)ret = static_cast<Class *>(const_cast<void *>(obj))->*field;
    }

    inline static void write(const void *obj, const void *ret)
    {
        static_cast<Class *>(const_cast<void *>(obj))->*field = *(T*)ret;
    }
};

template<typename T, typename Member, Member mem>
struct PropertyAccessor
{
    using IsMethod = typename std::is_member_function_pointer<Member>::type;
    using IsField = typename std::is_member_object_pointer<Member>::type;

    using ReadMethod = MethodReader<T, Member, mem>;
    using WriteMethod = MethodWriter<T, Member, mem>;
    using MemberField = FieldAccessor<T, Member, mem>;

    using Read = typename std::conditional<IsMethod::value, ReadMethod, MemberField>::type;
    using Write = typename std::conditional<IsMethod::value, WriteMethod, MemberField>::type;

    static_assert(!std::is_same<IsField, IsMethod>::value, "Property should be field or method");

    inline static void read(const void *obj, const void *ret)
    {
        Read::read(obj, ret);
    }

    inline static void write(const void *obj, const void *ret)
    {
        Write::write(obj, ret);
    }
};

#endif //UMOF_DETAIL_PROPERTY_H