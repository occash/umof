#ifndef OBJECTDEFS_H
#define OBJECTDEFS_H

#include <boost/any.hpp>
#include <vector>
#include <list>
#include <map>
#include <initializer_list>
#include <typeinfo>
#include <typeindex>
#include <iostream>

#include "types.h"

class Object;
struct Type;

////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
////////////////////////////////////////////////////////////////////////////////////////////////
typedef boost::any(*InvokeFun)(std::vector<boost::any>);
typedef boost::any(*InvokeMem)(Object *, const std::vector<boost::any>&);
typedef boost::any(*ConvertFun)(const boost::any&);
typedef bool(*CheckFun)(const boost::any&);
typedef std::vector<boost::any> ArgPack;
typedef std::list<std::string> StringList;
typedef std::list<Type> TypeList;

////////////////////////////////////////////////////////////////////////////////////////////////
//TYPE, METHOD, API, EXPOSE
////////////////////////////////////////////////////////////////////////////////////////////////
struct Type
{
	Type(std::type_index i = std::type_index(typeid(void)), 
		CheckFun c = nullptr, ConvertFun f = nullptr)
		: id(i), checker(c), converter(f) {}

	std::type_index id = typeid(void);
	CheckFun checker;
	ConvertFun converter;
};

class Method
{
public:
	Method() : data() {}

	bool isValid() const;
	boost::any invoke(Object *, const ArgPack&) const;
	StringList parameterNames() const;
	TypeList parameterTypes() const;
	unsigned int parameterCount() const;
	Type returnType() const;
	std::string signature() const;

	struct MethodData {
		MethodData() : invoker(nullptr) {}

		InvokeMem invoker;
		Type rettype;
		TypeList types;
		std::string signature;
	} data;
};

class Api
{
public:
	Api(const Api *super);

	//Api *super() const;
	//boost::any data(const char *) const; //Additional data connected to class
	//std::string name() const; //Class name
	//Method constructor(const char *) const; //find constructor by signature
	Method method(const char *) const; //find method by signature
	//Enum enum(const char *) const; //find enum by name
	//Property property(const char *) const; //find property by name
	//Object *create(ArgPack args) const;

	static boost::any invoke(Object *obj, const char *name, ArgPack args);

	struct
	{
		typedef std::multimap<std::string, Method> MethodMap;
		const Api *super;
		const char *name;
		MethodMap methods;
		//properties goes here
	} data;
};

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
	inline static boost::any invoke(F f, std::vector<boost::any> args, unpack::indices<Is...>)
	{
		return f(boost::spirit::any_cast<Args>(args[Is])...);
	}

	template<Fun fun>
	static boost::any invoke(std::vector<boost::any> args)
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
	inline static boost::any invoke(Object *obj, F f, const std::vector<boost::any>& args, unpack::indices<Is...>)
	{
		return (static_cast<Class *>(obj)->*f)(boost::any_cast<Args>(args[Is])...);
	}

	template<Fun fun>
	static boost::any invoke(Object *obj, const std::vector<boost::any>& args)
	{
		if (args.size() != sizeof...(Args))
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
	inline static boost::any invoke(Object *obj, F f, const std::vector<boost::any>& args, unpack::indices<Is...>)
	{
		return (static_cast<Class *>(obj)->*f)();
	}

	template<Fun fun>
	static boost::any invoke(Object *obj, const std::vector<boost::any>& args)
	{
		if (args.size() != 0)
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
	inline static boost::any invoke(Object *obj, F f, const std::vector<boost::any>& args, unpack::indices<Is...>)
	{
		(static_cast<Class *>(obj)->*f)(boost::spirit::any_cast<Args>(args[Is])...);
		return boost::any();
	}

	template<Fun fun>
	static boost::any invoke(Object *obj, const std::vector<boost::any>& args)
	{
		if (args.size() != sizeof...(Args))
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
	inline static boost::any invoke(Object *obj, F f, const std::vector<boost::any>& args, unpack::indices<Is...>)
	{
		(static_cast<Class *>(obj)->*f)(boost::spirit::any_cast<Args>(args[Is])...);
		return boost::any();
	}

	template<Fun fun>
	static boost::any invoke(Object *obj, const std::vector<boost::any>& args)
	{
		if (args.size() != 0)
			throw std::runtime_error("Bad argument count");
		return invoke(obj, fun, args, unpack::indices_gen<0>());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//Expose functions
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Signature, Signature& S>
InvokeFun free_function()
{
	return &Invoker<Signature>::invoke<S>;
}

template<typename Signature, Signature S>
Method method(const char *name)
{
	Method m;

	std::string sig(name);
	int pos = sig.rfind(':');
	if (pos != -1)
		sig = sig.substr(pos + 1, sig.size() - pos);
	sig += '(';

	TypeList args = Invoker<Signature>::types();
	for (Type arg : args)
	{
		sig += arg.id.name();
		sig += ',';
	}
	if (args.size() > 0)
		sig[sig.size() - 1] = ')';
	else
		sig += ')';

	m.data.signature = sig;
	m.data.invoker = &Invoker<Signature>::invoke<S>;
	m.data.rettype = Invoker<Signature>::returnType();
	m.data.types = Invoker<Signature>::types();

	return m;
}

#define declare(m) method<decltype(&m), &m>(#m)
#define overload(m, s) method<s, &m>(#m)

#endif