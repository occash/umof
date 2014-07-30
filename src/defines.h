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

typedef Any(*InvokeFun)(int argc, const Any *);
typedef Any(*InvokeMem)(Object *, int argc, const Any *);
typedef Any(*ReadMem)(Object *);
typedef void(*WriteMem)(Object *, const Any&);

#define VA_NARGS_IMPL_(_1, _2, _3, _4, _5, _6, _7, _8, \
						_9, _10, _11, _12, _13, _14, _15, _16, \
						_17, _18, _19, _20, _21, _22, _23, _24, \
						_25, _26, _27, _28, _29, _30, _31, _32, \
					N, ...) N
#define VA_NARGS_IMPL(tuple) VA_NARGS_IMPL_ tuple
#define VA_NARGS(...) VA_NARGS_IMPL((__VA_ARGS__,  \
						32, 31, 30, 29, 28, 27, 26, 25, \
						24, 23, 22, 21, 20, 19, 18, 17, \
						16, 15, 14, 13, 12, 11, 10, 9, \
						8, 7, 6, 5, 4, 3, 2, 1))

#endif