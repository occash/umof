The uMOF library is meta system for C++.

## Usage
To use meta system the class should enherit from class Object. Use OBJECT macro to define metaclass specific api. 
```
class Test : public Object
{
	OBJECT(Test, Object)
	EXPOSE(Test, 
		OVERLOAD(lol, Test, int, int, int),
		OVERLOAD(lol, Test, float, float, float),
		METHOD(null),
		METHOD(test)
	)

public:
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
int i = any_cast<int>(m.invoke(&t, args));

//This function will try to cast type if possible
//else will throw
Any res = Api::invoke(&t, "func", {5.0f, "6.0"});

```