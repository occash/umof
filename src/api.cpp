#include "api.h"
#include "object.h"

Api::Api(const char *name, const Api *super, const MethodDef *exp) :
	_name(name),
	_super(super)
{
	if (exp)
	{
		for(auto d = exp; d->name && d->table; ++d)
		{
			_methods.insert(std::make_pair(
				std::string(d->name),
				d->table));
		}
	}
}

const char *Api::name() const
{
	return _name;
}

const Api *Api::super() const
{
	return _super;
}

Method Api::method(const char *signature) const
{
	const Api *a = this;

	std::string sig = signature;
	int idx = sig.find('(');
	sig = sig.substr(0, idx);

	while (a)
	{
		auto mrange = a->_methods.equal_range(sig);
		for (auto i = mrange.first; i != mrange.second; ++i)
		{
			Method m((*i).first.c_str(), (*i).second);
			if (m.signature() == signature)
				return m;
		}

		a = a->_super;
	}

	return Method("", nullptr);
}

Property Api::property(const char *name) const
{
	const Api *a = this;

	while (a)
	{
		auto i = a->_props.find(name);
		if (i != a->_props.end())
			return Property((*i).first.c_str(), (*i).second);

		a = a->_super;
	}

	return Property("", nullptr);
}

Any Api::invoke(Object *obj, const char *name, ArgPack args)
{
	const Api *api = obj->api();

	std::string sig(name);
	sig += '(';
	for (Any arg : args)
	{
		sig += arg.type().name();
		sig += ',';
	}
	if (args.size() > 0)
		sig[sig.size() - 1] = ')';
	else
		sig += ')';

	/*Method func = api->method(sig.c_str());
	if (func.valid())
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
	}*/

	return Any();

	throw std::runtime_error("No such method");
}
