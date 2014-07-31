#ifndef TYPE_H
#define TYPE_H

#include "type_traits.h"

class Any;

class Type
{
public:
	//typedef Any(*ConvertFun)(const Any&);
	//typedef bool(*CheckFun)(const Any&);

	Type(TypeTable *table);

	bool valid() const;
	std::type_info const& id() const;
	int size() const;
	void *construct(void *where, void *const copy = nullptr) const;
	void *create(void *const copy = nullptr) const;
	void destroy(void *data) const;
	void destruct(void *data) const;

private:
	TypeTable *_table;
	//std::type_index id = typeid(void);
	//CheckFun checker;
	//ConvertFun converter;
};

#endif