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

#include "api.h"
#include "object.h"

Api::Api(const char *name, 
	const Api *super, 
	const MethodTable *methods, 
	const PropertyTable *props) :
	_name(name),
	_super(super),
	_methods(methods),
	_props(props),
	_methodCount(0),
	_propCount(0)
{
	while (methods && methods[_methodCount].invoker) _methodCount++;
	while (props && props[_propCount].type) _propCount++;
}

const char *Api::name() const
{
	return _name;
}

const Api *Api::super() const
{
	return _super;
}

int Api::indexOfMethod(const char *signature) const
{
	const Api *s = this;
	std::string sig = signature;

	while (s)
	{
		for (int i = 0; i < s->_methodCount; ++i)
			if (Method(_methods + i).signature() == signature)
				return i + s->methodOffset();

		s = s->_super;
	}

	return -1;
}

Method Api::method(int index) const
{
	int i = index - methodOffset();
	if (i < 0 && _super)
		return _super->method(index);

	if (i >= 0 && i < _methodCount)
		return Method(_methods + index);

	return Method(nullptr);
}

int Api::methodCount() const
{
	int count = _methodCount;
	const Api *s = _super;
	while (s)
	{
		count += s->_methodCount;
		s = s->_super;
	}
	return count;
}

int Api::methodOffset() const
{
	int offset = 0;
	const Api *s = _super;
	while (s)
	{
		offset += s->_methodCount;
		s = s->_super;
	}
	return offset;
}

int Api::indexOfProperty(const char *name) const
{
	const Api *s = this;

	while (s)
	{
		for (int i = 0; i < s->_propCount; ++i)
			if (strcmp(s->_props[i].name, name) == 0)
				return i + propertyOffset();

		s = s->_super;
	}

	return -1;
}

Property Api::property(int index) const
{
	int i = index - propertyOffset();
	if (i < 0 && _super)
		return _super->property(index);

	if (i >= 0 && i < _methodCount)
		return Property(_props + index);

	return Property(nullptr);
}

int Api::propertyCount() const
{
	int count = _propCount;
	const Api *s = _super;
	while (s)
	{
		count += s->_propCount;
		s = s->_super;
	}
	return count;
}

int Api::propertyOffset() const
{
	int offset = 0;
	const Api *s = _super;
	while (s)
	{
		offset += s->_propCount;
		s = s->_super;
	}
	return offset;
}

Any Api::invoke(Object *obj, const char *name, std::initializer_list<Any> args)
{
	const Api *api = obj->api();

	std::string sig(name);
	sig += '(';
	for (Any arg : args)
	{
		sig += arg.type().name();
		sig += ',';
	}
	if (args.size() > 0)
		sig[sig.size() - 1] = ')';
	else
		sig += ')';

	int index = api->indexOfMethod(sig.c_str());
	if (index >= 0)
		throw std::runtime_error("No such method");

	Method m = api->method(index);
	return Any();//m.invoke(obj, args.size(), args.begin());

	//TODO try to convert parameters 
	/*Method func = api->method(sig.c_str());
	if (func.valid())
		return func.invoke(obj, args);

	auto mrange = api->data.methods.equal_range(name);
	auto m = api->data.methods.end();
	int conform = std::numeric_limits<int>::min();
	for (auto i = mrange.first; i != mrange.second; ++i)
	{
		int a = 0;
		int c = -args.size();
		for (auto type : (*i).second.data.types)
		{
			if (type.checker(args[a]))
			{
				c++;
				if (type.id == args[a].type())
					c++;
			}
			a++;
		}

		if (c >= 0 && c > conform)
		{
			conform = c;
			m = i;
		}
	}

	if (m != api->data.methods.end())
	{
		ArgPack newArgs(args.size());
		int a = 0;
		for (auto type : (*m).second.data.types)
		{
			newArgs[a] = type.converter(args[a]);
			a++;
		}

		return (*m).second.invoke(obj, newArgs);
	}*/
}
