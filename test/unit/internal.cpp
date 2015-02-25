#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include <methoddefs.h>

#include <iostream>

bool func1Called = false;
bool func2Called = false;
bool func3Called = false;

bool staticFunc1Called = false;
bool staticFunc2Called = false;
bool staticFunc3Called = false;

void func1()
{
    func1Called = true;
}

void func2(int a, int b)
{
    func2Called = true;
}

int func3(int a, int b)
{
    func3Called = true;
    return a + b;
}

class Test
{
public:
    static void func1()
    {
        staticFunc1Called = true;
    }

    static void func2(int a, int b)
    {
        staticFunc2Called = true;
    }

    static int func3(int a, int b)
    {
        staticFunc3Called = true;
        return a + b;
    }
};

TEST_CASE("Free function defs, no params, no return", "[internal]")
{
    //Check types
	REQUIRE(Invoker<decltype(&func1)>::argCount() == 0);
	REQUIRE(Invoker<decltype(&func1)>::types()[0] == 0);

    //Check function call
    REQUIRE_NOTHROW(Invoker<decltype(&func1)>::template invoke<&func1>(nullptr, 0, nullptr));
    REQUIRE(func1Called);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&func1)>::template invoke<&func1>(nullptr, 1, nullptr));
}

TEST_CASE("Free function defs, has params, no return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&func2)>::argCount() == 2);
    REQUIRE(Invoker<decltype(&func2)>::types()[0] == 0);
    REQUIRE(Invoker<decltype(&func2)>::types()[1] == Table<int>::get());
    REQUIRE(Invoker<decltype(&func2)>::types()[2] == Table<int>::get());

    //Check function call
    Any params[] = { 1, 2 };
    REQUIRE_NOTHROW(Invoker<decltype(&func2)>::template invoke<&func2>(nullptr, 2, params));
    REQUIRE(func2Called);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&func2)>::template invoke<&func2>(nullptr, 1, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&func2)>::template invoke<&func2>(nullptr, 1, params));
}

TEST_CASE("Free function defs, has params, return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&func3)>::argCount() == 2);
    REQUIRE(Invoker<decltype(&func3)>::types()[0] == Table<int>::get());
    REQUIRE(Invoker<decltype(&func3)>::types()[1] == Table<int>::get());
    REQUIRE(Invoker<decltype(&func3)>::types()[2] == Table<int>::get());

    //Check function call
    Any params[] = { 1, 2 };
    REQUIRE_NOTHROW(Invoker<decltype(&func3)>::template invoke<&func3>(nullptr, 2, params));
    REQUIRE(func3Called);

    //Check return value
    Any res = Invoker<decltype(&func3)>::template invoke<&func3>(nullptr, 2, params);
    REQUIRE(any_cast<int>(res) == 3);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&func3)>::template invoke<&func3>(nullptr, 1, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&func3)>::template invoke<&func3>(nullptr, 1, params));
}

TEST_CASE("Static function defs, no params, no return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&Test::func1)>::argCount() == 0);
    REQUIRE(Invoker<decltype(&Test::func1)>::types()[0] == 0);

    //Check function call
    REQUIRE_NOTHROW(Invoker<decltype(&Test::func1)>::template invoke<&Test::func1>(nullptr, 0, nullptr));
    REQUIRE(func1Called);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&Test::func1)>::template invoke<&Test::func1>(nullptr, 1, nullptr));
}

TEST_CASE("Static function defs, has params, no return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&Test::func2)>::argCount() == 2);
    REQUIRE(Invoker<decltype(&Test::func2)>::types()[0] == 0);
    REQUIRE(Invoker<decltype(&Test::func2)>::types()[1] == Table<int>::get());
    REQUIRE(Invoker<decltype(&Test::func2)>::types()[2] == Table<int>::get());

    //Check function call
    Any params[] = { 1, 2 };
    REQUIRE_NOTHROW(Invoker<decltype(&Test::func2)>::template invoke<&Test::func2>(nullptr, 2, params));
    REQUIRE(func2Called);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&Test::func2)>::template invoke<&Test::func2>(nullptr, 1, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&Test::func2)>::template invoke<&Test::func2>(nullptr, 1, params));
}

TEST_CASE("Static function defs, has params, return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&Test::func3)>::argCount() == 2);
    REQUIRE(Invoker<decltype(&Test::func3)>::types()[0] == Table<int>::get());
    REQUIRE(Invoker<decltype(&Test::func3)>::types()[1] == Table<int>::get());
    REQUIRE(Invoker<decltype(&Test::func3)>::types()[2] == Table<int>::get());

    //Check function call
    Any params[] = { 1, 2 };
    REQUIRE_NOTHROW(Invoker<decltype(&Test::func3)>::template invoke<&Test::func3>(nullptr, 2, params));
    REQUIRE(func3Called);

    //Check return value
    Any res = Invoker<decltype(&Test::func3)>::template invoke<&Test::func3>(nullptr, 2, params);
    REQUIRE(any_cast<int>(res) == 3);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&Test::func3)>::template invoke<&Test::func3>(nullptr, 1, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&Test::func3)>::template invoke<&Test::func3>(nullptr, 1, params));
}