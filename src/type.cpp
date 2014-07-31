#include "type.h"

Type::Type(TypeTable *table) : 
	_table(table)
{
}

bool Type::valid() const
{
	return _table != nullptr;
}

std::type_info const& Type::id() const
{
	if (_table)
		return _table->get_type();
	else
		return typeid(void);
}

int Type::size() const
{
	if (_table)
		return _table->get_size();
}

void *Type::construct(void *where, void *const copy) const
{
	if (_table)
	{
		if (copy)
		{
			_table->clone(&copy, &where);
			return where;
		}
		else
		{
			_table->construct(&where);
			return where;
		}
	}
	else
		return nullptr;
}

void *Type::create(void *const copy) const
{
	if (_table)
	{
		void *where = nullptr;
		_table->static_new(&where);
		if (copy)
			_table->clone(&copy, &where);
		return where;
	}
	else
		return nullptr;
}

void Type::destroy(void *data) const
{
	if (_table)
		_table->static_delete(&data);
}

void Type::destruct(void *data) const
{
	if (_table)
		_table->destruct(&data);
}
