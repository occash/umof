#include "object.h"

#include <iostream>

class Test : public Object
{
	OBJECT(Test, Object)

public:
	static void expose()
	{
		static Expose<Test> m{
				{
					overload(Test::lol, int(Test::*)(int, int)),
					overload(Test::lol, float(Test::*)(float, float)),
					declare(Test::null),
					declare(Test::test)
				}
		};
	}

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
	
};

int my_fun(float f)
{
	return 5;
}

/*#define BOOST_SPIRIT_ANY_IMPLICIT_CASTING
#include <boost\spirit\home\support\detail\hold_any.hpp>*/
//#include <boost/timer/timer.hpp>

int main()
{
	Test t;
	t.expose();
	//t.lol(5.0f, 4);

	Method m = t.api()->method("lol(int,int)");
	//Invoker<int(Test::*)(int,int)>::invoke()

	std::clock_t c_start = std::clock();
	int l0 = 0;
	if (m.isValid())
	{
		ArgPack args{ 2, 45 };
		for (int i = 0; i < 1000000; ++i)
		{
			l0 += boost::any_cast<int>(m.invoke(&t, args));
			//Invoker<int(Test::*)(int, int)>::invoke<&Test::lol>(&t, args);
		}
	}
	std::clock_t c_end = std::clock();
	std::cout << "Meta system CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << l0 << std::endl;

	c_start = std::clock();
	int l1 = 0;
	for (int i = 0; i < 1000000; ++i)
	{
		l1 += t.lol(2, 45);
	}
	c_end = std::clock();
	std::cout << "Native CPU time used: "
		<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
		<< " ms\n" << l1 << std::endl;

	boost::any res = Api::invoke(&t, "lol", {5.0f, "6.0"});
	Api::invoke(&t, "test", {});
	Api::invoke(&t, "null", {});

	boost::any a{ 5 };
	//std::cout << res.type().name() << boost::spirit::any_cast<float>(res);

	/*boost::any ha(5.0);
	int i = boost::spirit::any_cast<int>(ha);*/

	return 0;
}