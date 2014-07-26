#include "property.h"

Property::Property(const char *name, PropertyTable *table) :
	_name(name),
	_table(table)
{
}

std::string Property::name() const
{
	return _name;
}

bool Property::valid() const
{
	return (_table != nullptr);
}

Type Property::type() const
{
	if (_table)
		return _table->type;
	else
		return Type();
}

Any Property::read(Object *obj) const
{
	if (_table)
		return _table->reader(obj);
	else
		return Any();
}

void Property::write(Object *obj, const Any& value) const
{
	if (_table)
		_table->writer(obj, value);
}
