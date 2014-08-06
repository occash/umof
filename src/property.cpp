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

#include "property.h"

Property::Property(const char *name, PropertyTable *table) :
	_name(name),
	_table(table)
{
}

std::string Property::name() const
{
	return _name;
}

bool Property::valid() const
{
	return (_table != nullptr);
}

Type Property::type() const
{
	if (_table)
		return _table->type;
	else
		return Type(nullptr);
}

Any Property::read(Object *obj) const
{
	if (_table)
		return _table->reader(obj);
	else
		return Any();
}

void Property::write(Object *obj, const Any& value) const
{
	if (_table)
		_table->writer(obj, value);
}
