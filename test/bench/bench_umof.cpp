#include <object.h>

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

void my_fun(int i)
{
	std::cout << "free function" << i << std::endl;
}

class Test : public Object
{
	OBJECT(Test, Object)
	EXPOSE(
		OVERLOAD(Test, func, int, int, int),
		OVERLOAD(Test, func, float, float, float),
		METHOD(null),
		METHOD(print),
		METHOD(static_func),
		METHOD(my_fun)
	)
	PROPERTIES(
		PROPERTY(val, getVal, setVal)
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

NOINLINE int callFunc(Test *t, std::initializer_list<int> a)
{
	int res;
	void *args[] = { &res, t, (void *)a.begin(), (void *)(a.begin() + 1) };
	Invoker<int(Test::*)(int, int)>::invoke<&Test::func>(args);
	return res;
}

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
		/*void *args[10];
		//Any ret = 0;
		//args[0] = ret.pointer();
		int ret;
		args[0] = &ret;
		args[1] = &t;
		for (int i = 0; i < 2; ++i)
			args[i + 2] = (void *)i;
		m.invoke(args);
		Any a(ret);
		result += any_cast<int>(a);
		//result += ret;*/
		result += any_cast<int>(m.invoke(&t, { i, i }));
	}
	std::clock_t c_end = std::clock();
	std::cout << "uMOF CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << result << std::endl;

	return 0;
}