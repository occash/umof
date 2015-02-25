#include "../catch.hpp"

#include <methoddefs.h>
#include <method.h>
#include <object.h>

#include <iostream>

class MTest : public Object
{
public:
	static double func1(int a, float b)
	{
		return a + b;
	}

};

TEST_CASE("Method interface", "[method]")
{
	MethodTable table
	{
		ConstString("func1"),
		&Invoker<decltype(&MTest::func1)>::invoke <&MTest::func1>,
		Invoker<decltype(&MTest::func1)>::argCount(),
		Invoker<decltype(&MTest::func1)>::types()
	};
	Method method(&table);
	MTest test;

	REQUIRE(method.valid());
	REQUIRE(method.name() == "func1");
	REQUIRE(method.parameterCount() == 2);
	REQUIRE(method.parmaeterType(0) == Type(Table<int>::get()));
	REQUIRE(method.parmaeterType(1) == Type(Table<float>::get()));
	REQUIRE(method.returnType() == Type(Table<double>::get()));
	REQUIRE(method.signature() == "func1(int,float)");
	REQUIRE(any_cast<double>(method.invoke(&test, {1, 2.0f})) == 3.0);
}