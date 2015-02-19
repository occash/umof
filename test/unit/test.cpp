#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include <methoddefs.h>

#include <iostream>

void func1()
{
	std::cout << "func1" << std::endl;
}

void func2(int a, int b)
{
    std::cout << "func2" << a << b << std::endl;
}

int func3(int a, int b)
{
    std::cout << "func3" << std::endl;
    return a + b;
}

TEST_CASE("Internal is okey", "[internal]")
{
	REQUIRE(Invoker<decltype(&func1)>::argCount() == 0);
	REQUIRE(Invoker<decltype(&func1)>::types()[0] == 0);
    REQUIRE_NOTHROW(Invoker<decltype(&func1)>::template invoke<&func1>(nullptr, 0, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&func1)>::template invoke<&func1>(nullptr, 1, nullptr));
}