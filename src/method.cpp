#include "method.h"


Method::Method(const char *name, MethodTable *table) :
	_name(name),
	_table(table)
{
}

bool Method::valid() const
{
	return (_table != nullptr);
}

std::string Method::name() const
{
	return _name;
}

std::string Method::signature() const
{
	if (_table == nullptr)
		return _name;

	std::string sig(_name);
	int pos = sig.rfind(':');
	if (pos != -1)
		sig = sig.substr(pos + 1, sig.size() - pos);
	sig += '(';

	for (Type arg : _table->types)
	{
		sig += arg.id.name();
		sig += ',';
	}
	if (_table->types.size() > 0)
		sig[sig.size() - 1] = ')';
	else
		sig += ')';

	return sig;
}

Type Method::returnType() const
{
	if (_table)
		return _table->rettype;
	else
		return Type();
}

int Method::parameterCount() const
{
	if (_table)
		return _table->types.size();
	else
		return 0;
}

TypeList Method::parameterTypes() const
{
	if (_table)
		return _table->types;
	else
		return TypeList();
}

Type Method::parmaeterType(int index) const
{
	if (_table)
		return *std::next(_table->types.begin(), index);
	else
		return Type();
}

Any Method::invoke(Object *obj, int argc, const Any *args) const
{
	if (_table)
		return _table->invoker(obj, argc, args);
	else
		return Any();
}

Any Method::invoke(Object *obj, std::initializer_list<Any> args) const
{
	if (_table)
		return _table->invoker(obj, args.size(), args.begin());
	else
		return Any();
}
