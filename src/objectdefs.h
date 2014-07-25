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

#ifndef OBJECTDEFS_H
#define OBJECTDEFS_H

#include <vector>
#include <list>
#include <map>
#include <initializer_list>
#include <typeinfo>
#include <typeindex>
#include <iostream>

#include "defines.h"
#include "any.h"
#include "enum.h"
#include "type.h"
#include "converters.h"
#include "method.h"
#include "property.h"
#include "api.h"

class Object;
struct Type;

////////////////////////////////////////////////////////////////////////////////////////////////
//TYPE, METHOD, API, EXPOSE
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Class>
struct Expose
{
	Expose(std::initializer_list<Method> methods)
	{
		Api *_api = const_cast<Api *>(Class::classApi());

		for (Method method : methods)
		{
			std::string sig = method.signature();
			int idx = sig.find('(');
			sig = sig.substr(0, idx);
			_api->data.methods.insert(std::make_pair(sig, method));
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//UNPACK HELPER
////////////////////////////////////////////////////////////////////////////////////////////////
namespace unpack
{
	template<unsigned...> struct indices {};

	template<unsigned N, unsigned... Is>
	struct indices_gen : indices_gen <N - 1, N - 1, Is...> {};

	template<unsigned... Is>
	struct indices_gen<0, Is...> : indices <Is...>{};
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Base template
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Signature>
struct Invoker;

////////////////////////////////////////////////////////////////////////////////////////////////
//Free function
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Return, typename... Args>
struct Invoker<Return(Args...)>
{
	typedef Return(*Fun)(Args...);

	inline static TypeList types()
	{
		return TypeList({
				{
					std::type_index(typeid(Args)),
					&Converter<Args>::canConvert,
					&Converter<Args>::convert
				}... });
	}

	inline static Type returnType()
	{
		return{
			std::type_index(typeid(Return)),
			&Converter<Args>::canConvert,
			&Converter<Args>::convert
		};
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(F f, const ArgPack& args, unpack::indices<Is...>)
	{
		return f(any_cast<Args>(args[Is])...);
	}

	template<Fun fun>
	static Any invoke(const ArgPack& args)
	{
		if (args.size() != sizeof...(Args))
			throw std::runtime_error("bad argument count");
		return invoke(fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//Base method template
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Class, typename Return, typename... Args>
struct Invoker <Return(Class::*)(Args...)>
{
	typedef Return(Class::*Fun)(Args...);

	inline static TypeList types()
	{
		return TypeList({
			Type{ 
				std::type_index(typeid(Args)), 
				&Converter<Args>::canConvert, 
				&Converter<Args>::convert
			}... });
	}

	inline static Type returnType()
	{
		return{
			std::type_index(typeid(Return)),
			&Converter<Return>::canConvert,
			&Converter<Return>::convert
		};
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		return (static_cast<Class *>(obj)->*f)(any_cast<Args>(args[Is])...);
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != sizeof...(Args))
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//Method with no parameters
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Class, typename Return>
struct Invoker <Return(Class::*)()>
{
	typedef Return(Class::*Fun)();

	inline static TypeList types()
	{
		return TypeList();
	}

	inline static Type returnType()
	{
		return{
			std::type_index(typeid(Return)),
			&Converter<Return>::canConvert,
			&Converter<Return>::convert
		};
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *, unpack::indices<Is...>)
	{
		return (static_cast<Class *>(obj)->*f)();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//Void method
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Class, typename... Args>
struct Invoker <void(Class::*)(Args...)>
{
	typedef void(Class::*Fun)(Args...);

	inline static TypeList types()
	{
		return TypeList({ 
				{ 
					std::type_index(typeid(Args)),
					&Converter<Args>::canConvert,
					&Converter<Args>::convert
				}... });
	}

	inline static Type returnType()
	{
		return{ 
			std::type_index(typeid(void)),
			nullptr,
			nullptr
		};
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		(static_cast<Class *>(obj)->*f)(any_cast<Args>(args[Is])...);
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != sizeof...(Args))
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//Void method with no parameters
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Class>
struct Invoker <void(Class::*)()>
{
	typedef void(Class::*Fun)();

	inline static TypeList types()
	{
		return TypeList();
	}

	inline static Type returnType()
	{
		return{ 
			std::type_index(typeid(void)),
			nullptr,
			nullptr 
		};
	}

	template<typename F, unsigned... Is>
	inline static Any invoke(Object *obj, F f, const Any *args, unpack::indices<Is...>)
	{
		(static_cast<Class *>(obj)->*f)(any_cast<Args>(args[Is])...);
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//Expose functions
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Signature, Signature S>
struct MTable
{
	static MethodTable *get()
	{
		static MethodTable staticTable
		{
			&Invoker<Signature>::invoke<S>,
			Invoker<Signature>::returnType(),
			Invoker<Signature>::types()
		};
		return &staticTable;
	}
};

template<typename Signature, Signature& S>
InvokeFun free_function()
{
	return &Invoker<Signature>::invoke<S>;
}

template<typename Signature, Signature S>
Method method(const char *name)
{
	return Method(name, MTable<Signature, S>::get());
}

#define METHOD(m) method<decltype(&m), &m>(#m)
#define OVERLOAD(m, c, r, ...) method<r(c::*)(__VA_ARGS__), &m>(#m)

/*template<typename GetSignature, typename SetSignature>
struct Holder;

template<typename Class, typename T>
Holder <T(Class::*)(), void(Class::*)(T const&)>
{
	typedef T(Class::*)() Getter;
	typedef void(Class::*)(T const&) Setter;
};*/

#endif