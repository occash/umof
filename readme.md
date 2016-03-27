The uMOF library is a cross platform C++ meta system. It can be used for reflection and script binding. It's written in C++11 and require modern compiler.

## Build
[![Build Status](https://semaphoreci.com/api/v1/occash/umof/branches/master/badge.svg)](https://semaphoreci.com/occash/umof)

The library uses [premake4](http://industriousone.com/premake-quick-start) as the build system. To build uMOF, in command line navigate into uMOF folder, then:

	premake4 action

See premake4 documentation for full list of supported actions. The project files will be created in ```uMOF/build``` folder. After compiling the project, the library
file will be in ```uMOF/lib```.

To build static library use ```--static``` option.

## Usage
To use meta system the class should inherit from class Object. Use OBJECT macro to define metaclass specific api. 

```
class Test : public Object
{
	U_OBJECT(Test, Object)
	U_EXPOSE(Test, 
		U_OVERLOAD(func, Test, int, int, int),
		U_OVERLOAD(func, Test, float, float, float),
		U_METHOD(null),
		U_METHOD(test)
	)
	U_PROPERTIES(
		U_PROPERTY(val, Test::getVal, Test::setVal)
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

## Benchmark
To build benchmark use ```--benchmark``` option.

Configuration: Windows, Visual Studio 2013, Release x86

Framework | Compile/Link time, ms | Executable size, KB | Call time spent*, ms
-----|------|-----|-----
Native | 371/63 | 12 | 2 (45**)
uMOF | 406/78 | 18 | 349
Camp | 4492/116 | 66 | 6889
Qt | 1040/80 (+129***) | 15 | 498
cpgf | 2514/166 | 71 | 1184

\* 10000000 calls

\*\* Force noinline

\*\*\* Meta object compiler