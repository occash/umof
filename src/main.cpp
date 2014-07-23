#include "object.h"

#include <iostream>
#include <ctime>

class Test : public Object
{
	OBJECT(Test, Object)
	EXPOSE
	(Test, 
	OVERLOAD(lol, int(Test::*)(int, int)),
	OVERLOAD(lol, float(Test::*)(float, float)),

	METHOD(null),
	METHOD(test)
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
		for (int i = 0; i < 100000; ++i)
		{
			l0 += any_cast<int>(m.invoke(&t, {i, i})); 
			//m.invoke(&t, { i, i });
			//m.invoke(&t, args);
			//Invoker<int(Test::*)(int, int)>::invoke<&Test::lol>(&t, args);
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

	/*Any res = Api::invoke(&t, "lol", {5.0f, "6.0"});
	Api::invoke(&t, "test", {});
	Api::invoke(&t, "null", {});

	Any a{ 5 };*/
	//std::cout << res.type().name() << boost::spirit::any_cast<float>(res);

	/*Any ha(5.0);
	int i = boost::spirit::any_cast<int>(ha);*/

	return 0;
}