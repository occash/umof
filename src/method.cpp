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

#include "method.h"


Method::Method(const MethodTable *table) :
	_table(table)
{
}

bool Method::valid() const
{
	return (_table != nullptr);
}

const char *Method::name() const
{
	return _table->name;
}

std::string Method::signature() const
{
	std::string sig(_table->name);
	int pos = sig.rfind(':');
	if (pos != -1)
		sig = sig.substr(pos + 1, sig.size() - pos);
	sig += '(';

	for (int i = 0; i < _table->argc; ++i)
	{
		Type arg(_table->types[i + 1]);
		sig += arg.id().name();
		sig += ',';
	}
	if (_table->argc > 0)
		sig[sig.size() - 1] = ')';
	else
		sig += ')';

	return sig;
}

Type Method::returnType() const
{
	return Type(_table->types[0]);
}

int Method::parameterCount() const
{
	return _table->argc;
}

/*TypeList Method::parameterTypes() const
{
	if (_table)
		return _table->types;
	else
		return TypeList();
}*/

Type Method::parmaeterType(int index) const
{
	return Type(_table->types[index + 1]);
}

Any Method::invoke(Object *obj, int argc, const Any *args) const
{
	if (argc != _table->argc)
		return Any();

	return Any(); //TODO
	//return _table->invoker(obj, args);
}

int res, a, b;
void *argData[] = { &res, nullptr, &a, &b };

Any Method::invoke(Object *obj, std::initializer_list<Any> args) const
{
	//Any result(returnType());
	
	/*argData[0] = result.pointer();
	argData[1] = obj;
	for (std::size_t i = 0; i < args.size(); ++i)
		argData[i + 2] = (args.begin() + i)->object();*/

	//_table->invoker(argData);
	return 5;
}
