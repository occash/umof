#include "method.h"

bool Method::isValid() const
{
	return (data.invoker != nullptr);
}

Any Method::invoke(Object *obj, const ArgPack& args) const
{
	if (data.invoker)
		return data.invoker(obj, args);
	else
		return Any();
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