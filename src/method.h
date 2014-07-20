#ifndef METHOD_H
#define METHOD_H

#include "defines.h"
#include "any.h"
#include "type.h"

class Object;

class Method
{
public:
	bool isValid() const;
	Any invoke(Object *, const ArgPack&) const;
	StringList parameterNames() const;
	TypeList parameterTypes() const;
	unsigned int parameterCount() const;
	Type returnType() const;
	std::string signature() const;

	struct MethodData {
		InvokeMem invoker;
		Type rettype;
		TypeList types;
		std::string signature;
	} data;
};

#endif