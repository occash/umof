#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <list>

class Any;
struct Type;
class Object;

typedef std::vector<Any> ArgPack;
typedef std::list<std::string> StringList;
typedef std::list<Type> TypeList;

typedef Any(*InvokeFun)(std::vector<Any>);
typedef Any(*InvokeMem)(Object *, int argc, const Any *);

#endif