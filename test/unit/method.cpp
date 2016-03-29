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

U_DECALRE_METHODS(MTest)
{
    U_METHOD(func1)
};
U_DECLARE_API(MTest)

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