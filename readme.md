The uMOF library is meta system for C++.

## Usage
To use meta system the class should enherit from class Object. Use OBJECT macro to define metaclass specific api. 
```
class Test : public Object
{
	OBJECT(Test, Object)

public:
	static void expose()
	{
		static Expose<Test> m{
				{
					overload(Test::func, int(Test::*)(int, int)),
					overload(Test::func, float(Test::*)(float, float)),
					declare(Test::null),
					declare(Test::test)
				}
		};
	}

	Test() = default;

	int func(int a, int b)
	{
		return a + b;
	}

	float func(float a, float b)
	{
		return a + b;
	}

	int null()
	{
		return 0;
	}

	void test()
	{
		std::cout << "test" << std::endl;
	}
};
```
Now you can use exposed api of the class.
```
Test t;

//Api for exact signature
//Will throw if types are not consistent
Method m = t.api()->method("func(int,int)");
int i = boost::any_cast<int>(m.invoke(&t, args));

//This function will try to cast type if possible
//else will throw
boost::any res = Api::invoke(&t, "lol", {5.0f, "6.0"});

```