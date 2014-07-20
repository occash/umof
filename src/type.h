#ifndef TYPE_H
#define TYPE_H

#include <typeinfo>
#include <typeindex>

class Any;

struct Type
{
	typedef Any(*ConvertFun)(const Any&);
	typedef bool(*CheckFun)(const Any&);

	Type(std::type_index i = std::type_index(typeid(void)),
		CheckFun c = nullptr, ConvertFun f = nullptr)
		: id(i), checker(c), converter(f) {}

	std::type_index id = typeid(void);
	CheckFun checker;
	ConvertFun converter;
};

#endif