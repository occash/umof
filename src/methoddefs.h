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

#ifndef METHODDEFS_H
#define METHODDEFS_H

#include "type_traits.h"
#include "any.h"

class Object;

//Helper stuff
namespace unpack
{
	template<unsigned...> struct indices {};

	template<unsigned N, unsigned... Is>
	struct indices_gen : indices_gen < N - 1, N - 1, Is... > {};

	template<unsigned... Is>
	struct indices_gen<0, Is...> : indices < Is... > {};
}

//Function to unpack args properly
template<typename T>
inline static TypeTable *getTable()
{
	return Table<T>::get();
}

template<typename T, typename Ptr>
struct TypeHelper;

template<typename T>
struct TypeHelper<T, True>
{
	inline static T *cast(void **arg)
	{
		return reinterpret_cast<T *>(arg);
	}
};

template<typename T>
struct TypeHelper<T, False>
{
	inline static T *cast(void **arg)
	{
		return reinterpret_cast<T *>(*arg);
	}
};

template<typename T>
struct ArgType
{
	typedef Bool<(sizeof(T) <= sizeof(void*))> is_small;

	inline static T *cast(void **arg)
	{
		return TypeHelper<T, is_small>::cast(arg);
	}
};

//Base template for function and methods invocation
template<typename Signature>
struct Invoker;

//Free functions
template<typename Return, typename... Args>
struct Invoker<Return(*)(Args...)>
{
	// Workaround for the Visual Studio bug
	typedef Return(*Fun)(Args...);

	inline static bool isStatic()
	{
		return true;
	}

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get(),
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static void invoke(F f, void **args, unpack::indices<Is...>)
	{
		*reinterpret_cast<Return *>(args[0]) = f(*ArgType<Args>::cast(&args[Is + 1])...);
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		invoke(fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Free function with no parameters
template<typename Return>
struct Invoker<Return(*)()>
{
	typedef Return(*Fun)();

	inline static bool isStatic()
	{
		return true;
	}

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get()
		};
		return staticTypes;
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		*reinterpret_cast<Return *>(args[0]) = fun();
	}
};

//Void free function
template<typename... Args>
struct Invoker<void(*)(Args...)>
{
	typedef void(*Fun)(Args...);

	inline static bool isStatic()
	{
		return true;
	}

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr,
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static void invoke(F f, void **args, unpack::indices<Is...>)
	{
		f(*ArgType<Args>::cast(&args[Is + 1])...);
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		invoke(fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Void free function with no arguments
template<>
struct Invoker<void(*)()>
{
	typedef void(*Fun)();

	inline static bool isStatic()
	{
		return true;
	}

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr
		};
		return staticTypes;
	}

	template<Fun fun>
	static void invoke(void ** /*args*/)
	{
		fun();
	}
};

//Base method template
template<typename Class, typename Return, typename... Args>
struct Invoker<Return(Class::*)(Args...)>
{
	typedef Return(Class::*Fun)(Args...);

	inline static bool isStatic()
	{
		return false;
	}

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get(),
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static void invoke(F f, void **args, unpack::indices<Is...>)
	{
		*reinterpret_cast<Return *>(args[0]) = 
			(reinterpret_cast<Class *>(args[1])->*f)(*ArgType<Args>::cast(&args[Is + 2])...);
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		invoke(fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Method with no parameters
template<typename Class, typename Return>
struct Invoker<Return(Class::*)()>
{
	typedef Return(Class::*Fun)();

	inline static bool isStatic()
	{
		return false;
	}

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get()
		};
		return staticTypes;
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		*reinterpret_cast<Return *>(args[0]) =
			(reinterpret_cast<Class *>(args[1])->*fun)();
	}
};

//Void method
template<typename Class, typename... Args>
struct Invoker<void(Class::*)(Args...)>
{
	typedef void(Class::*Fun)(Args...);

	inline static bool isStatic()
	{
		return false;
	}

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr,
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static void invoke(F f, void **args, unpack::indices<Is...>)
	{
		(reinterpret_cast<Class *>(args[1])->*f)(*ArgType<Args>::cast(&args[Is + 2])...);
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		return invoke(fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Void method with no parameters
template<typename Class>
struct Invoker<void(Class::*)()>
{
	typedef void(Class::*Fun)();

	inline static bool isStatic()
	{
		return false;
	}

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr
		};
		return staticTypes;
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		(reinterpret_cast<Class *>(args[1])->*fun)();
	}
};

//Base const method template
template<typename Class, typename Return, typename... Args>
struct Invoker<Return(Class::*)(Args...)const>
{
	typedef Return(Class::*Fun)(Args...)const;

	inline static bool isStatic()
	{
		return false;
	}

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get(),
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static void invoke(F f, void **args, unpack::indices<Is...>)
	{
		*reinterpret_cast<Return *>(args[0]) =
			(reinterpret_cast<const Class *>(args[1])->*f)(*ArgType<Args>::cast(&args[Is + 2])...);
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		invoke(fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Const method with no parameters
template<typename Class, typename Return>
struct Invoker<Return(Class::*)()const>
{
	typedef Return(Class::*Fun)()const;

	inline static bool isStatic()
	{
		return false;
	}

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			Table<Return>::get()
		};
		return staticTypes;
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		*reinterpret_cast<Return *>(args[0]) =
			(reinterpret_cast<const Class *>(args[1])->*fun)();
	}
};

//Void const method
template<typename Class, typename... Args>
struct Invoker<void(Class::*)(Args...)const>
{
	typedef void(Class::*Fun)(Args...)const;

	inline static bool isStatic()
	{
		return false;
	}

	inline static int argCount()
	{
		return sizeof...(Args);
	}

	template<typename T>
	inline static TypeTable *getTable()
	{
		return Table<T>::get();
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr,
			getTable<Args>()...
		};
		return staticTypes;
	}

	template<typename F, unsigned... Is>
	inline static void invoke(F f, void **args, unpack::indices<Is...>)
	{
		(reinterpret_cast<const Class *>(args[1])->*f)(*ArgType<Args>::cast(&args[Is + 2])...);
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		return invoke(fun, args, unpack::indices_gen<sizeof...(Args)>());
	}
};

//Void const method with no parameters
template<typename Class>
struct Invoker<void(Class::*)()const>
{
	typedef void(Class::*Fun)()const;

	inline static bool isStatic()
	{
		return false;
	}

	inline static int argCount()
	{
		return 0;
	}

	inline static const TypeTable **types()
	{
		static const TypeTable *staticTypes[] =
		{
			nullptr
		};
		return staticTypes;
	}

	template<Fun fun>
	static void invoke(void **args)
	{
		(reinterpret_cast<const Class *>(args[1])->*fun)();
	}
};

#endif