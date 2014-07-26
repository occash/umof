#include "enum.h"


Enumerator::Enumerator(const char *name, EnumeratorTable *table) :
	_name(name),
	_table(table)
{
}

std::string Enumerator::name() const
{
	return _name;
}

bool Enumerator::valid() const
{
	return (_table != nullptr);
}

int Enumerator::keyCount() const
{
	if (_table)
		return _table->size;
	else
		return 0;
}

StringList Enumerator::keys() const
{
	if (_table)
	{
		return StringList(_table->keys, 
			_table->keys + _table->size);
	}
	else
		return StringList();
}

std::string Enumerator::key(int value) const
{
	if (_table)
		return _table->keys[value];
	else
		return std::string();
}

int Enumerator::value(const char *key) const
{
	/*if (_table)
	{
		char **key =
			std::find(
			&_table->keys[0],
			&_table->keys[0] + _table->size,
			key);
		return _table->values[key - _table->keys];
	}
	else*/
		return 0;
}

int Enumerator::value(int value) const
{
	if (_table)
		return _table->values[value];
	else
		return 0;
}
