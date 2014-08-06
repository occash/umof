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

#ifndef API_H
#define API_H

#include <map>

#include "defines.h"
#include "method.h"
#include "property.h"

struct MethodDef;

class Api
{
public:
	Api(const char *name, const Api *super, const MethodTable *exp);

	const char *name() const; //Class name
	const Api *super() const;
	//Any data(const char *) const; //Additional data connected to class
	//Method constructor(const char *) const; //find constructor by signature
	Method method(const char *) const; //find method by signature
	Property property(const char *) const; //find property by name
	//Enum enum(const char *) const; //find enum by name
	
	//static Object *create(ArgPack args) const;
	static Any invoke(Object *obj, const char *name, ArgPack args);

private:
	typedef std::multimap<std::string, const MethodTable *> MethodMap;
	typedef std::map<std::string, PropertyTable *> PropertyMap;
	const char *_name;
	const Api *_super;
	MethodMap _methods;
	PropertyMap _props;

};

#endif