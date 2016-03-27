#include "../catch.hpp"

#include <umof.h>

using namespace umof;

class MTest
{
public:
	double func1(int a, float b)
	{
		return a + b;
	}

};

U_DECLARE_API(MTest, METHODS);

U_DECLARE_METHODS(MTest)
{
    U_METHOD(func1)
};

TEST_CASE("Method interface", "[method]")
{
    const Api *api = U_API(MTest);
	Method method = api->method(0);

	REQUIRE(method.valid());
	REQUIRE(method.name() == "func1");
	REQUIRE(method.parameterCount() == 2);
	REQUIRE(method.parmaeterType(0) == Type(detail::Table<int>::get()));
	REQUIRE(method.parmaeterType(1) == Type(detail::Table<float>::get()));
	REQUIRE(method.returnType() == Type(detail::Table<double>::get()));
	REQUIRE(method.signature() == "func1(int,float)");

    MTest test;
    double ret;
    REQUIRE(method.invoke(&test, ret, { 1, 2.0f }));
	REQUIRE(ret == 3.0);
}