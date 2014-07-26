#ifndef ENUM_H
#define ENUM_H

#include "defines.h"

struct EnumeratorTable
{
	int size;
	char **keys;
	int *values;
};

class Enumerator
{
public:
	Enumerator(const char *name, EnumeratorTable *table);
	
	std::string name() const;
	bool valid() const;
	
	int keyCount() const;
	StringList keys() const;

	std::string key(int) const;
	int value(const char *) const;
	int value(int) const;

private:
	std::string _name;
	EnumeratorTable *_table;
};

#endif