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

#ifndef OBJECT_H
#define OBJECT_H

#include "objectdefs.h"

//#define OBJECT_CHECK
#define OBJECT(Class, Super) \
public: \
	static const Api *classApi() \
		{ \
		static const Api staticApi(Super::classApi()); \
		return &staticApi; \
		} \
	virtual const Api *api() const \
		{ \
		return Class::classApi(); \
		} \
private:

#define EXPOSE(Class, ...) \
public: \
	static const Expose<Class> *expose() \
	{ \
		static const Expose<Class> exposer \
		{ \
			__VA_ARGS__ \
		}; \
		return &exposer; \
	} \
private:

class Object
{
public:
	static const Api *classApi()
	{
		static const Api staticApi(nullptr);
		return &staticApi;
	}

	virtual const Api *api() const
	{
		return Object::classApi();
	}

	int op(int e, int f)
	{
		return e * f;
	}

	Expose<Object> methods{
			{
				METHOD(Object::op)
			}
	};
};

#endif