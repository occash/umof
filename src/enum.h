#ifndef ENUM_H
#define ENUM_H

#include "defines.h"

class Enumerator
{
	const char *name() const;
	StringList keys() const;
	int value(const char *) const;
	const char *key(int) const;
};

#endif