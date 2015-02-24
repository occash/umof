#include <celero/Celero.h>

#include <object.h>

#include <iostream>

/*template<class T>
struct ApiHolder
{
	static const Api *api()
	{
		static const ApiTable apiTable
		{
			"Test",
			nullptr,
			expose_method<T>::exec().second,
			expose_props_method<T>::exec().second,
			expose_enums_method<T>::exec().second,
			expose_method<T>::exec().first,
			expose_props_method<T>::exec().first,
			expose_enums_method<T>::exec().first
		};
		static const Api staticApi(&apiTable);
		return &staticApi;
	}
};

#define U_DECLARE_API(Class) class Class ## Api : public ApiHolder<Class>
#define U_API(Class) Class ## Api::api()*/

static void my_fun(int i)
{
	std::cout << "free function" << i << std::endl;
}

class UTest : public Object
{
    U_OBJECT(UTest, Object)
	U_EXPOSE(
    U_OVERLOAD(UTest, UTest::func, int(UTest::*)(int, int)),
    U_OVERLOAD(UTest, UTest::func, float(UTest::*)(float, float)),
    U_METHOD(UTest::null),
    U_METHOD(UTest::print),
    U_METHOD(UTest::static_func),
		U_METHOD(my_fun)
	)
	U_PROPERTIES(
    U_PROPERTY(val, UTest::getVal, UTest::setVal)
	)
	U_ENUMERATORS(
		U_ENUMERATE(TestEnum,
        U_VALUE(UTest, Value1),
        U_VALUE(UTest, Value2)
		)
	)

public:
	enum TestEnum
	{
		Value1,
		Value2
	};

public:
	int func(int a, int b)
	{
		return a + b;
	}

	float func(float a, float b)
	{
		return a + b;
	}

	int null() const
	{
		return 0;
	}

	void print() const
	{
		std::cout << "Hello from native func" << std::endl;
	}

	static void static_func()
	{
		std::cout << "Hello from static func" << std::endl;
	}

	int getVal() const
	{
		return _val;
	}

	void setVal(int v)
	{
		_val = v;
	}

private:
	int _val;

};

/*U_DECLARE_API(Test)
{
	U_EXPOSE(
		U_OVERLOAD(Test, Test::func, int(Test::*)(int, int)),
		U_OVERLOAD(Test, Test::func, float(Test::*)(float, float)),
		U_METHOD(Test::null),
		U_METHOD(Test::print),
		U_METHOD(Test::static_func),
		U_METHOD(my_fun)
	)
	U_PROPERTIES(
		U_PROPERTY(val, Test::getVal, Test::setVal)
	)
	U_ENUMERATORS(
		U_ENUMERATE(TestEnum,
			U_VALUE(Test, Value1),
			U_VALUE(Test, Value2)
		)
	)
};*/

class UmofFixture : public celero::TestFixture
{
public:
    UmofFixture() : m(nullptr) {}

    virtual void setUp(int64_t experimentValue)
    {
        int idx = t.api()->indexOfMethod("func(int,int)");
        m = t.api()->method(idx);
    }

    virtual void tearDown()
    {
    }

public:
    UTest t;
    Method m;

};

BENCHMARK_F(FuncDefs, Umof, UmofFixture, 100, 10000)
{
    celero::DoNotOptimizeAway(any_cast<int>(m.invoke(&t, { 1, 2 })));
}
