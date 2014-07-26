#ifndef PROPERTY_H
#define PROPERTY_H

#include "defines.h"
#include "any.h"
#include "type.h"

class Object;

struct PropertyTable
{
	Type type;
	ReadMem reader;
	WriteMem writer;
};

class Property
{
public:
	Property(const char *name, PropertyTable *table);

	std::string name() const;
	Type type() const;

	Any read(Object *obj) const;
	void write(Object *obj, const Any& value) const;

private:
	std::string _name;
	PropertyTable *_table;

};

#endif