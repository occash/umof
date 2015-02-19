#include <object.h>

#include <iostream>
#include <ctime>

template<class T>
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
#define U_API(Class) Class ## Api::api()

void my_fun(int i)
{
	std::cout << "free function" << i << std::endl;
}

class Test : public Object
{
	U_OBJECT(Test, Object)
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

U_DECLARE_API(Test)
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
};

int main()
{
	int idx = U_API(Test)->indexOfMethod("func(int,int)");

	Test t;

	Method m = t.api()->method(t.api()->indexOfMethod("func(int,int)"));
	if (!m.valid())
		return 1;

	std::clock_t c_start = std::clock();
	int result = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		result += any_cast<int>(m.invoke(&t, { i, i }));
	}
	std::clock_t c_end = std::clock();
	std::cout << "uMOF CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << result << std::endl;

	return 0;
}