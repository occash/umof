#include "../catch.hpp"
#include "test.h"

using namespace umof;

TEST_CASE("Method interface", "[method]")
{
    const Api api = Api::from<MTest>();
    Method method = api.method(0);

    REQUIRE(method.valid());
    REQUIRE(method.name() == "func1");
    REQUIRE(method.parameterCount() == 2);
    REQUIRE(method.parmaeterType(0) == Type(detail::Type<int>::table()));
    REQUIRE(method.parmaeterType(1) == Type(detail::Type<float>::table()));
    REQUIRE(method.returnType() == Type(detail::Type<double>::table()));
    REQUIRE(method.signature() == "func1(int,float)");

    MTest test;
    double ret;
    REQUIRE(method.invoke(&test, ret, { 1, 2.0f }));
    REQUIRE(ret == 3.0);
}