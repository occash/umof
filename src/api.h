#ifndef API_H
#define API_H

#include <map>

#include "defines.h"
#include "method.h"

struct Expose;

class Api
{
public:
	Api(const char *name, const Api *super, const Expose *exp);

	std::string name() const; //Class name
	const Api *super() const;
	//Any data(const char *) const; //Additional data connected to class
	//Method constructor(const char *) const; //find constructor by signature
	Method method(const char *) const; //find method by signature
	//Property property(const char *) const; //find property by name
	//Enum enum(const char *) const; //find enum by name
	
	//static Object *create(ArgPack args) const;
	static Any invoke(Object *obj, const char *name, ArgPack args);

private:
	typedef std::multimap<std::string, MethodTable *> MethodMap;
	std::string _name;
	const Api *_super;
	MethodMap methods;
	//properties goes here
};

#endif