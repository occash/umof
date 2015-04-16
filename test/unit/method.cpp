#include "../catch.hpp"

#include <methoddefs.h>
#include <method.h>
#include <object.h>

#include <iostream>
#include <tuple>

class MTest : public Object
{
public:
	double func1(int a, float b)
	{
		return a + b;
	}

};

struct table_func1
{
    using UC = MTest;
};

TEST_CASE("Method interface", "[method]")
{
    MethodTable table
    {
        ConstString("func1"),
        MethodCall<decltype(&MTest::func1), &MTest::func1>::call,
        MethodArguments<decltype(&MTest::func1)>::count,
        MethodArguments<decltype(&MTest::func1)>::types()
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

    double ret;
    REQUIRE(method.invoke(&test, ret, { 1, 2.0f }));
	REQUIRE(ret == 3.0);
}