#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <list>

class Any;
class Type;
class Object;

typedef std::vector<Any> ArgPack;
typedef std::list<std::string> StringList;
typedef std::list<Type> TypeList;

typedef Any(*InvokeFun)(std::vector<Any>);
typedef Any(*InvokeMem)(Object *, const std::vector<Any>&);

#endif