#include "any.h"

Any::Any() :
	_table(nullptr),
	_object(nullptr)
{
}

Any::Any(Any const& x) :
	_table(nullptr),
	_object(nullptr)
{
	/*if (&x != this)
	{
	if (_table == x._table)
	_table->move(&x._object, &_object);
	else
	{
	reset();*/
	if (x._table != nullptr)
	{
		x._table->clone(&x._object, &_object);
		_table = x._table;
	}
	/*}
	}*/
}

Any::~Any()
{
	_table->static_delete(&_object);
}

void Any::reset()
{
	if (_table != nullptr)
	{
		_table->static_delete(&_object);
		_table = nullptr;
		_object = nullptr;
	}
}

std::type_info const& Any::type() const
{
	return _table->get_type();
}