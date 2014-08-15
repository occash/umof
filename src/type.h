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

#ifndef TYPE_H
#define TYPE_H

#include "defines.h"
#include "type_traits.h"

class Any;

class UMOF_EXPORT Type
{
public:
	//typedef Any(*ConvertFun)(const Any&);
	//typedef bool(*CheckFun)(const Any&);

	Type(const TypeTable *table);

	bool operator==(const Type& other);

	bool valid() const;
	std::type_info const& id() const;
	int size() const;
	void *construct(void *where, void *const copy = nullptr) const;
	void *create(void *const copy = nullptr) const;
	void destroy(void *data) const;
	void destruct(void *data) const;

private:
	const TypeTable *_table;
	//std::type_index id = typeid(void);
	//CheckFun checker;
	//ConvertFun converter;
};

#endif