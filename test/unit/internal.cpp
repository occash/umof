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

void void_func_no_args()
{
    func1Called = true;
}

void void_func_with_args(int a, float b)
{
    func2Called = true;
}

double return_func_with_args(int a, float b)
{
    func3Called = true;
    return a + b;
}

std::string func_with_vararg(const char *format, ...)
{
    char buffer[64];
    va_list argptr;
    va_start(argptr, format);
    std::vsprintf(buffer, format, argptr);
    va_end(argptr);
    return buffer;
}

class Test
{
    float _value;
public:
    static void void_method_no_args()
    {
        staticFunc1Called = true;
    }

    static void void_method_with_args(int a, float b)
    {
        staticFunc2Called = true;
    }

    static double return_method_with_args(int a, float b)
    {
        staticFunc3Called = true;
        return a + b;
    }

    static std::string static_method_with_vararg(const char *format, ...) {}
    void method_with_vararg(const char *format, ...) {}

    void method() {}
    void method_const() const {}
    void method_volatile() volatile {}
    void method_const_volatile() const volatile {}

    float& method_lvalue() & { return _value;  } //MSVC workaround
    void method_lvalue_const() const & {}
    void method_lvalue_volatile() volatile & {}
    void method_lvalue_const_volatile() const volatile & {}

    void method_rvalue() && {}
    void method_rvalue_const() const && {}
    void method_rvalue_volatile() volatile && {}
    void method_rvalue_const_volatile() const volatile && {}
};

TEST_CASE("Method argument count", "[MethodProps]")
{
    //C functions
    REQUIRE(MethodArguments<decltype(&void_func_no_args)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::count() == 2);
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::count() == 2);

    //Static methods
    REQUIRE(MethodArguments<decltype(&Test::void_method_no_args)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::void_method_with_args)>::count() == 2);
    REQUIRE(MethodArguments<decltype(&Test::return_method_with_args)>::count() == 2);

    //Function with varargs
    REQUIRE(MethodArguments<decltype(&func_with_vararg)>::count() == 1);
    REQUIRE(MethodArguments<decltype(&Test::static_method_with_vararg)>::count() == 1);
    REQUIRE(MethodArguments<decltype(&Test::method_with_vararg)>::count() == 1);

    //Member functions
    REQUIRE(MethodArguments<decltype(&Test::method)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_const)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_volatile)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_const_volatile)>::count() == 0);

    //Member functions lvalue ref
    REQUIRE(MethodArguments<decltype(&Test::method_lvalue)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_lvalue_const)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_lvalue_volatile)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_lvalue_const_volatile)>::count() == 0);

    //Member functions rvalue ref
    REQUIRE(MethodArguments<decltype(&Test::method_rvalue)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_rvalue_const)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_rvalue_volatile)>::count() == 0);
    REQUIRE(MethodArguments<decltype(&Test::method_rvalue_const_volatile)>::count() == 0);

    //REQUIRE(MethodArguments<float>::count() == 0);
}

TEST_CASE("Method argument types", "[MethodProps]")
{
    REQUIRE(MethodArguments<decltype(&void_func_no_args)>::types()[0] == 0);

    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[0] == 0);
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[1] == Table<int>::get());
    REQUIRE(MethodArguments<decltype(&void_func_with_args)>::types()[2] == Table<float>::get());

    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[0] == Table<double>::get());
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[1] == Table<int>::get());
    REQUIRE(MethodArguments<decltype(&return_func_with_args)>::types()[2] == Table<float>::get());
}

TEST_CASE("Free function defs, no params, no return", "[internal]")
{
    //Check types
	REQUIRE(Invoker<decltype(&void_func_no_args)>::argCount() == 0);
	REQUIRE(Invoker<decltype(&void_func_no_args)>::types()[0] == 0);

    //Check function call
    REQUIRE_NOTHROW(Invoker<decltype(&void_func_no_args)>::template invoke<&void_func_no_args>(nullptr, 0, nullptr));
    REQUIRE(func1Called);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&void_func_no_args)>::template invoke<&void_func_no_args>(nullptr, 1, nullptr));
}

TEST_CASE("Free function defs, has params, no return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&void_func_with_args)>::argCount() == 2);
    REQUIRE(Invoker<decltype(&void_func_with_args)>::types()[0] == 0);
    REQUIRE(Invoker<decltype(&void_func_with_args)>::types()[1] == Table<int>::get());
    REQUIRE(Invoker<decltype(&void_func_with_args)>::types()[2] == Table<float>::get());

    //Check function call
    Any params[] = { 1, 2.0f };
    REQUIRE_NOTHROW(Invoker<decltype(&void_func_with_args)>::template invoke<&void_func_with_args>(nullptr, 2, params));
    REQUIRE(func2Called);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&void_func_with_args)>::template invoke<&void_func_with_args>(nullptr, 1, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&void_func_with_args)>::template invoke<&void_func_with_args>(nullptr, 1, params));
}

TEST_CASE("Free function defs, has params, return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&return_func_with_args)>::argCount() == 2);
    REQUIRE(Invoker<decltype(&return_func_with_args)>::types()[0] == Table<double>::get());
    REQUIRE(Invoker<decltype(&return_func_with_args)>::types()[1] == Table<int>::get());
    REQUIRE(Invoker<decltype(&return_func_with_args)>::types()[2] == Table<float>::get());

    //Check function call
    Any params[] = { 1, 2.0f };
    REQUIRE_NOTHROW(Invoker<decltype(&return_func_with_args)>::template invoke<&return_func_with_args>(nullptr, 2, params));
    REQUIRE(func3Called);

    //Check return value
    Any res = Invoker<decltype(&return_func_with_args)>::template invoke<&return_func_with_args>(nullptr, 2, params);
    REQUIRE(any_cast<double>(res) == 3.0);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&return_func_with_args)>::template invoke<&return_func_with_args>(nullptr, 1, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&return_func_with_args)>::template invoke<&return_func_with_args>(nullptr, 1, params));
}

TEST_CASE("Static function defs, no params, no return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&Test::void_method_no_args)>::argCount() == 0);
    REQUIRE(Invoker<decltype(&Test::void_method_no_args)>::types()[0] == 0);

    //Check function call
    REQUIRE_NOTHROW(Invoker<decltype(&Test::void_method_no_args)>::template invoke<&Test::void_method_no_args>(nullptr, 0, nullptr));
    REQUIRE(func1Called);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&Test::void_method_no_args)>::template invoke<&Test::void_method_no_args>(nullptr, 1, nullptr));
}

TEST_CASE("Static function defs, has params, no return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&Test::void_method_with_args)>::argCount() == 2);
    REQUIRE(Invoker<decltype(&Test::void_method_with_args)>::types()[0] == 0);
    REQUIRE(Invoker<decltype(&Test::void_method_with_args)>::types()[1] == Table<int>::get());
    REQUIRE(Invoker<decltype(&Test::void_method_with_args)>::types()[2] == Table<float>::get());

    //Check function call
    Any params[] = { 1, 2.0f };
    REQUIRE_NOTHROW(Invoker<decltype(&Test::void_method_with_args)>::template invoke<&Test::void_method_with_args>(nullptr, 2, params));
    REQUIRE(func2Called);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&Test::void_method_with_args)>::template invoke<&Test::void_method_with_args>(nullptr, 1, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&Test::void_method_with_args)>::template invoke<&Test::void_method_with_args>(nullptr, 1, params));
}

TEST_CASE("Static function defs, has params, return", "[internal]")
{
    //Check types
    REQUIRE(Invoker<decltype(&Test::return_method_with_args)>::argCount() == 2);
    REQUIRE(Invoker<decltype(&Test::return_method_with_args)>::types()[0] == Table<double>::get());
    REQUIRE(Invoker<decltype(&Test::return_method_with_args)>::types()[1] == Table<int>::get());
    REQUIRE(Invoker<decltype(&Test::return_method_with_args)>::types()[2] == Table<float>::get());

    //Check function call
    Any params[] = { 1, 2.0f };
    REQUIRE_NOTHROW(Invoker<decltype(&Test::return_method_with_args)>::template invoke<&Test::return_method_with_args>(nullptr, 2, params));
    REQUIRE(func3Called);

    //Check return value
    Any res = Invoker<decltype(&Test::return_method_with_args)>::template invoke<&Test::return_method_with_args>(nullptr, 2, params);
    REQUIRE(any_cast<double>(res) == 3.0);

    //Wrong params check
    REQUIRE_THROWS(Invoker<decltype(&Test::return_method_with_args)>::template invoke<&Test::return_method_with_args>(nullptr, 1, nullptr));
    REQUIRE_THROWS(Invoker<decltype(&Test::return_method_with_args)>::template invoke<&Test::return_method_with_args>(nullptr, 1, params));
}