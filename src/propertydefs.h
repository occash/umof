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

#ifndef PROPERTYDEFS_H
#define PROPERTYDEFS_H

#include "type_traits.h"
#include "methoddefs.h"

template<typename T, typename Function, Function func>
struct Reader
{
    using Args = typename MethodArguments<Function>;
    using Return = typename Args::Return;
    using Class = typename Args::Class;

    using ReturnType = typename Table<Return>::Type;
    using Type = typename Table<T>::Type;

    static_assert(Args::count == 0, "Reader method should not receive arguments");
    static_assert(std::is_same<Type, ReturnType>::value, "Reader type is different");

    inline static void read(void *obj, void *ret)
    {
        *(T*)ret = (static_cast<Class *>(obj)->*func)();
    }
};

template<typename T, typename Function, Function func>
struct Writer
{
    using Args = typename MethodArguments<Function>;
    using Class = typename Args::Class;
    using Type = typename Table<T>::Type;

    static_assert(Args::count == 1, "Writer should have one argument");
    //static_assert(std::is_same<typename Args::Type<0>, Type>::value, "Writer type is different");

    inline static void write(void *obj, void *val)
    {
        (static_cast<Class *>(obj)->*func)(*(typename Args::Type<0> *)val);
    }
};

#endif