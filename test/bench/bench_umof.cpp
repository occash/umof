#include <celero/Celero.h>

#include <object.h>

class UmofTest : public Object
{
	U_OBJECT(UmofTest, Object)
	U_EXPOSE(
		U_METHOD(UmofTest::func),
	)

public:
	int func(int a, int b)
	{
		return a + b;
	}
};

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
    UmofTest t;
    Method m;

};

BENCHMARK_F(FuncDefs, Umof, UmofFixture, 100, 100000)
{
    /*int i1 = 1;
    int i2 = 2;

    void *data[10] = { &i1, &i2 };

    int res;

    int argc = MethodArguments<decltype(&UmofTest::func)>::count;
    const TypeTable **types = MethodArguments<decltype(&UmofTest::func)>::types();

    for (int i = 0; i < argc; ++i)
        if (types[i] != Table<int>::get())
            return;

    MethodCall<decltype(&UmofTest::func), &UmofTest::func>::call(&t, &res, data);
    celero::DoNotOptimizeAway(res);*/

    celero::DoNotOptimizeAway(any_cast<int>(m.invoke(&t, { 1, 2 })));
}
