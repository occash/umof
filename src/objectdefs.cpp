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

#include "objectdefs.h"
#include "object.h"

bool Method::isValid() const
{
	return (data.invoker != nullptr);
}

boost::any Method::invoke(Object *obj, const ArgPack& args) const
{
	if (data.invoker)
		return data.invoker(obj, args);
	else
		return boost::any();
}

StringList Method::parameterNames() const
{
	return StringList();
}

TypeList Method::parameterTypes() const
{
	return data.types;
}

unsigned int Method::parameterCount() const
{
	return data.types.size();
}

Type Method::returnType() const
{
	return data.rettype;
}

std::string Method::signature() const
{
	return data.signature;
}

Api::Api(const Api *super)
{
	data.super = super;
}

Method Api::method(const char *signature) const
{
	const Api *a = this;

	std::string sig = signature;
	int idx = sig.find('(');
	sig = sig.substr(0, idx);

	while (a)
	{
		auto mrange = a->data.methods.equal_range(sig);
		for (auto i = mrange.first; i != mrange.second; ++i)
		{
			if ((*i).second.signature() == signature)
				return (*i).second;
		}

		a = a->data.super;
	}
	
	return Method();
}

boost::any Api::invoke(Object *obj, const char *name, ArgPack args)
{
	const Api *api = obj->api();

	std::string sig(name);
	sig += '(';
	for (boost::any arg : args)
	{
		sig += arg.type().name();
		sig += ',';
	}
	if (args.size() > 0)
		sig[sig.size() - 1] = ')';
	else
		sig += ')';

	Method func = api->method(sig.c_str());
	if (func.isValid())
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
	}

	throw std::runtime_error("No such method");
}
