#include <celero/Celero.h>

#include <iostream>
#include <ctime>

//Force noinline
#if defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define NOINLINE __attribute__ ((noinline))
#else
#define NOINLINE
#endif

static void my_fun(int i)
{
	std::cout << "free function" << i << std::endl;
}

class Object {};

class Test : public Object
{
public:
	NOINLINE int func(int a, int b)
	{
		return a + b;
	}

	NOINLINE float func(float a, float b)
	{
		return a + b;
	}

	NOINLINE int null() const
	{
		return 0;
	}

	NOINLINE void print() const
	{
		std::cout << "Hello from native func" << std::endl;
	}

	NOINLINE static void static_func()
	{
		std::cout << "Hello from static func" << std::endl;
	}
	
	NOINLINE int getVal() const
	{
		return _val;
	}

	NOINLINE void setVal(int v)
	{
		_val = v;
	}

private:
	int _val;

};

class NativeFixture : public celero::TestFixture
{
public:
    virtual void setUp(int64_t experimentValue)
    {
    }

    virtual void tearDown()
    {
    }

public:
    Test t;
};

BASELINE_F(FuncDefs, Native, NativeFixture, 100, 10000)
{
    celero::DoNotOptimizeAway(t.func(1, 2));
}
