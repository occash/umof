#include <object.h>

#include <iostream>
#include <ctime>

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

int main()
{
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