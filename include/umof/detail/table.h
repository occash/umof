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

#include "../conststring.h"

using InvokeMem = void(*)(const void *, const void *, const void **);
using ReadMem = void(*)(const void *, const void *);
using WriteMem = void(*)(const void *, const void *);

class Api;

//Table with basic functions
struct TypeTable
{
    bool is_small;
    const char *(*get_name)();
    int(*get_size)();
    void(*static_new)(void**);
    void(*construct)(void**);
    void(*static_delete)(void**);
    void(*destruct)(void**);
    void(*clone)(void* const*, void**);
    void(*move)(void* const*, void**);
};

struct EnumeratorTable
{
    ConstString name;
    const unsigned int value;
};

struct EnumTable
{
    ConstString name;
    const unsigned int count;
    const EnumeratorTable *table;
};

/* \breif Internal struct to store method meta information.
*/
struct MethodTable
{
    ConstString name;
    InvokeMem invoker;
    const unsigned int argc;
    const TypeTable **types;
};

/* \breif Internal struct to store property meta information.
*/
struct PropertyTable
{
    ConstString name;
    const TypeTable *type;
    ReadMem reader;
    WriteMem writer;
};

/*! \breif Internal struct to store class meta information.
*/
struct ApiTable
{
    ConstString name;
    const Api *super;
    const MethodTable *methods;
    const PropertyTable *props;
    const EnumTable *enums;
    const unsigned int methodCount;
    const unsigned int propCount;
    const unsigned int enumCount;
};
