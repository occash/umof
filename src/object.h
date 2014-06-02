#ifndef OBJECT_H
#define OBJECT_H

#include "objectdefs.h"

#define OBJECT_CHECK
#define OBJECT(Class, Super) \
public: \
	static const Api *classApi() \
		{ \
		static const Api staticApi(Super::classApi()); \
		return &staticApi; \
		} \
	virtual const Api *api() const \
		{ \
		return Class::classApi(); \
		} \
private:

class Object
{
public:
	static const Api *classApi()
	{
		static const Api staticApi(nullptr);
		return &staticApi;
	}

	virtual const Api *api() const
	{
		return Object::classApi();
	}

	int op(int e, int f)
	{
		return e * f;
	}

	Expose<Object> methods{
			{
				declare(Object::op)
			}
	};
};

#endif