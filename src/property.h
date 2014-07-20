#ifndef PROPERTY_H
#define PROPERTY_H

class Any;
struct Type;
class Object;

class Property
{
	const char *name() const;
	Type type() const;

	Any read(Object *) const;
	void write(Object *, const Any&);
};

#endif