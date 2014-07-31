#ifndef METHODDEFS_H
#define METHODDEFS_H

class Object;
struct Type;

//Helper stuff
namespace unpack
{
	template<unsigned...> struct indices {};

	template<unsigned N, unsigned... Is>
	struct indices_gen : indices_gen < N - 1, N - 1, Is... > {};

	template<unsigned... Is>
	struct indices_gen<0, Is...> : indices < Is... > {};
}

//Base method template
template<typename Signature>
struct Invoker;

//Free functions
template<typename Class, typename Return, typename... Args>
struct Invoker < Return(*)(Args...) >
{
	typedef Return(*Fun)(Args...);

	inline static Type *types()
	{
		static const Type staticTypes[] =
		{
			{
				std::type_index(typeid(Args)),
				&Converter<Args>::canConvert,
				&Converter<Args>::convert
			}...
		};
		return staticTypes;
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

template<>
struct Invoker < void(*)() >
{
	typedef void(*Fun)();

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

	template<unsigned... Is>
	inline static Any invoke(Object *obj, Fun f, const Any *args, unpack::indices<Is...>)
	{
		f();
		return Any();
	}

	template<Fun fun>
	static Any invoke(Object *obj, int argc, const Any *args)
	{
		if (argc != 0)
			throw std::runtime_error("bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//Base method template
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Class, typename Return, typename... Args>
struct Invoker < Return(Class::*)(Args...) >
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
struct Invoker < Return(Class::*)() >
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
struct Invoker < void(Class::*)(Args...) >
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
struct Invoker < void(Class::*)() >
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
//Property read/write 
////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Signature, Signature S>
struct Reader;

template<typename T, typename Class, T(Class::*ReadFunc)()>
struct Reader < T(Class::*)(), ReadFunc >
{
	inline static Type type()
	{
		return{
			std::type_index(typeid(T)),
			nullptr,
			nullptr
		};
	}

	inline static Any read(Object *obj)
	{
		return (static_cast<Class *>(obj)->*ReadFunc)();
	}
};

template<typename T, typename Class, T(Class::*ReadFunc)()const>
struct Reader < T(Class::*)()const, ReadFunc >
{
	inline static Type type()
	{
		return{
			std::type_index(typeid(T)),
			nullptr,
			nullptr
		};
	}

	inline static Any read(Object *obj)
	{
		return (static_cast<Class *>(obj)->*ReadFunc)();
	}
};

#endif