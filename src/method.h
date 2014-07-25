#ifndef METHOD_H
#define METHOD_H

#include "defines.h"
#include "any.h"
#include "type.h"

class Object;

struct MethodTable
{
	InvokeMem invoker;
	Type rettype;
	TypeList types;
};

class Method
{
public:
	Method(const char *name, MethodTable *table);

	bool valid() const;

	std::string name() const;
	std::string signature() const;

	Type returnType() const;
	int parameterCount() const;
	TypeList parameterTypes() const;
	Type parmaeterType(int index) const;

	Any invoke(Object *obj, int argc, const Any *args) const;
	Any invoke(Object *obj, std::initializer_list<Any> args) const;

private:
	std::string _name;
	MethodTable *_table;

};

#endif