#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include <methoddefs.h>

#include <iostream>

void func1()
{
	std::cout << "func1" << std::endl;
}

TEST_CASE("Internal is okey", "[internal]")
{
	REQUIRE(Invoker<decltype(&func1)>::argCount() == 0);
	REQUIRE(Invoker<decltype(&func1)>::types()[0] == nullptr);
}