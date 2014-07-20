#ifndef API_H
#define API_H

#include <map>

#include "defines.h"
#include "method.h"

class Api
{
public:
	Api(const Api *super);

	//Api *super() const;
	//Any data(const char *) const; //Additional data connected to class
	//std::string name() const; //Class name
	//Method constructor(const char *) const; //find constructor by signature
	Method method(const char *) const; //find method by signature
	//Enum enum(const char *) const; //find enum by name
	//Property property(const char *) const; //find property by name
	//Object *create(ArgPack args) const;

	static Any invoke(Object *obj, const char *name, ArgPack args);

	struct
	{
		typedef std::multimap<std::string, Method> MethodMap;
		const Api *super;
		const char *name;
		MethodMap methods;
		//properties goes here
	} data;
};

#endif