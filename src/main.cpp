#include "object.h"

#include <iostream>
#include <ctime>

/*void my_fun(Object *obj)
{
	std::cout << "free function" << std::endl;
}*/

class Test : public Object
{
	OBJECT(Test, Object)
	EXPOSE(
		OVERLOAD(lol, Test, int, int, int),
		OVERLOAD(lol, Test, float, float, float),
		METHOD(null),
		METHOD(test),
		METHOD(sfunc)
		//PROPERTY(val, getVal, setVal)
	)

public:
	Test() = default;

	int lol(int a, int b)
	{
		return a + b;
	}

	float lol(float a, float b)
	{
		return a + b;
	}

	int null()
	{
		return 0;
	}

	void test()
	{
		std::cout << "loool" << std::endl;
	}

	static void sfunc()
	{
		std::cout << "static func" << std::endl;
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
	int size = VA_NARGS(1, 2, 3, 4);

	Property p = t.api()->property("val");
	if (p.valid())
	{
		p.write(&t, 25);
		int i = any_cast<int>(p.read(&t));
	}

	Method sm = t.api()->method("sfunc()");
	sm.invoke(0, {});
	Method m = t.api()->method("lol(int,int)");

	std::clock_t c_start = std::clock();
	int l0 = 0;
	if (m.valid())
	{
		for (int i = 0; i < 100000; ++i)
		{
			l0 += any_cast<int>(m.invoke(&t, {i, i})); 
		}
	}
	std::clock_t c_end = std::clock();
	std::cout << "Meta system CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << l0 << std::endl;

	c_start = std::clock();
	int l1 = 0;
	for (int i = 0; i < 100000; ++i)
	{
		l1 += t.lol(i, i);
	}
	c_end = std::clock();
	std::cout << "Native CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << l1 << std::endl;

	return 0;
}