Handy
=====

Handy is a compilation of very useful header only utilities for modern C++.

All you need is a compiler supporting C++11/14/17.


- [Handy](#handy)
    - [Helpers](#helpers)
        - [Benchmark (C++11)](#benchmark-c11)
        - [HandyParams (C++17)](#handyparams-c17)
        - [HasMember (C++11)](#hasmember-c11)



Helpers
-------


### Benchmark (C++11)

Precise time measurement for both Windows and Linux (up to nanoseconds). The time return is in **seconds**.

I did this one because the standard library for C++ kinda lacks precision. Also, this one is much easier to use:

``` C++
    handy::Benchmark bm;

    // Do something

    double timeElapsed = bm.finish();

    bm.start();

    // Do something else

    double moreTimeElapsed = bm.finish();  // Time until last call to start()
```

Also works with a function call:


``` C++
    handy::Benchmark bm;

    auto func = []{ /* Do something */ };

    double timeElapsed = bm(func);

    double moreTimeElapsed = handy::benchmark(func); // Same as above
```


### HandyParams (C++17)

This is a very simple header that allows the creation of classes with (almost) python-like initialization

It is only supported in C++17, with the aid of the [std::any](http://en.cppreference.com/w/cpp/utility/any) class. Also, it incurs a small runtime overhead, and may be prone to type casting rrors, but the result is simply beautiful:

Note: the typ


``` C++
struct Foo
{
    int a;
    double b;
    float c;
    std::string d;


    // Put this at a public section of your class with the class
    // name and all variables you want to be able to initialize

    HANDY_PARAMS(Foo, a, b, c, d);
};


int main ()
{
    // A std::map<std::string, std::any> is passed in the constructor.
    // Notice that you just need to inialize the variables you want.
    // Also, the type must be exact (10.0f instead of 10.0 for floats)

    Foo foo({{"a", 3}, {"c", 10.0f}, {"d", "abcdefg"s}});

    return 0;
}
```


### HasMember (C++11)

Simple utility to verify at compile time if a class has a variable or a function. 

``` C++
struct Foo
{
    void foo (int) {}

    void bar (int) {}
    void bar (std::string) {}

    int var;
};

struct Goo {};


void foo (int) {}

void foo (Foo, std::string, Goo = Goo{}) {}


HAS_VAR(var);   // Check the existence of the variable 'var'. Automatically named 'Hasvar'

HAS_FUNC(foo, HasFoo);   // Check the existence of the regular member function 'foo'. Manually named 'Hasfoo'

HAS_EXTERN_FUNC(foo, HasExternFoo);     // Check the existence of the free function 'foo'

HAS_OVERLOADED_FUNC(bar, HasOverloadedBar);     // Check the existence of the overloaded member function 'bar'


int main ()
{
    Hasvar<int>::value;     // false
    Hasvar<Foo>::value;     // true

    HasFoo<Foo>::value;     // true
    HasFoo<Goo>::value;     // false

    HasExternFoo<Foo>::value;   // false
    HasExternFoo<Foo, std::string>::value;   // true
    HasExternFoo<Foo, std::string, Goo>::value;   // true

    HasOverloadedBar<Foo, std::string>::value;   // true
    HasOverloadedBar<Foo, double>::value;   // true -> implicit call to bar(int)

    return 0;
}
```