# Handy

[![Build Status](https://travis-ci.org/matheuspf/Handy.svg?branch=master)](https://travis-ci.org/matheuspf/Handy) [![Coverage Status](https://coveralls.io/repos/github/matheuspf/Handy/badge.svg?branch=master)](https://coveralls.io/github/matheuspf/Handy?branch=master)

Handy is a compilation of very useful header only utilities for modern C++.

No need to compile or install anything, simply include and use.

All you need is a compiler supporting C++11/14/17 (tested on g++ 7.2.0 and clang 5.0.1).


## Google Test

To run the tests, simply do:

```
cd tests
mkdir build
cd build

cmake ..
cmake --build .

./HandyTest
```

Google test will be downloaded automatically if you don't have it installed.

<br>


## Examples

There are a bunch of examples for everything in the folder `examples`. If you want to see them running, simply do:


```
cd examples
mkdir build
cd build

cmake ..
cmake --build .

./HandyExample
```

<br>


## Documentation

The documentation is also abundant and very readable from the code. If you want to generate the documentation, all you need is [Doxygen](http://www.stack.nl/~dimitri/doxygen/):

```
cd doc
doxygen Doxyfile
```

The documentation is generated using 
[doxygen-bootstrapped](https://github.com/Velron/doxygen-bootstrapped).


<br>


## Table of Contents


- [Handy](#handy)
    - [Google Test](#google-test)
    - [Examples](#examples)
    - [Documentation](#documentation)
    - [Table of Contents](#table-of-contents)
- [Headers](#headers)
    - [Algorithms](#algorithms)
    - [Container](#container)
    - [Range](#range)
    - [Wrapper](#wrapper)
    - [ZipIter](#zipiter)
    - [Helpers](#helpers)
        - [Benchmark](#benchmark)
        - [HandyParams](#handyparams)
        - [HasMember](#hasmember)
        - [NamedTuple](#namedtuple)
        - [Print](#print)
        - [Random](#random)



<br><br>


# Headers


## Algorithms

Define STL container algorithms and pipeline operators.

By including this class, for almost any stl algorithm, a function taking a container instead of iterators is defined.
             
This function simply delegate the call to the actual stl function by calling std::begin and std::end on the container passed as parameter.
            
The exact form of the function, as well as the number of arguments depends on the stl algorithm. For each begin/end pair there is one container. For single iterators, such as the third argument to std::copy, there is also one container. Any other paramater of the stl algorithm can also be passed as parameter to the handy version.

An example of use:

``` C++

std::vector<int> v, u;

handy::sort(v, std::greater<int>());

handy::copy(v, u);

std::vector<int> transformed = handy::transform(v, u, [](int x){ return 2 * x; });
```

Notice that the return is also modified for some functions. For stl functions that do not return, a reference to the container that is being modified is returned. 

For functions that return an iterator that does not imply position (std::transform, for example), the return is also the container that is being modified. Any other case has the same return as the stl algorithm.

Another feature is the function pipeline, which can be controled by changing the **ALGORITHM\_OP\_SYMBOL** macro (the default is **&**).
             
By passing only the arguments to the handy algorithm function, a lambda is returned, taking the missing container argument.

It is easy then to construct a pipeline of operations (starting at the left). Also, the order of the parameters is irrelevant, as long as a single container is given. Example:

``` C++
std::vector<int> sortedLeft = v & handy::sort();

std::vector<int> sortedRight = handy::sort() & v;

int sum = u & handy::reverse() & handy::accumulate(0);

auto finder = handy::find_if([](auto x){ return x == 10; });

auto posV = (v & finder) - v.begin();

bool posU = finder(u) - u.begin();
```

For many functions the overloads are not ambiguous to the original stl functions, so there is no need to use the handy namespace to disambiguate the call.

<br>

## Container

An interface to easily access multidimensional data, having total compatibility with STL algorithms and containers

it is a very generic and easy to use multidimensional container, which allows easy creation and access.

It is aimed to be fast and easy to use.

Also, there is a lot of ways to use it:


``` C++
// Type int and three dimensions with sizes 10, 20 and 30 respetivelly.
// In this case (compile time size is given), 'a' inherits from 'std::array'.
handy::Container<int, 10, 20, 30> a;

/// The same as above, but now 'b' inherits from 'std::vector' (runtime size is given).
handy::Container<int> b(10, 20, 30);

// You can also create a Container giving the sizes via an itearable type
// (any type that has both std::begin and std::end defined). They have
// to contain any integral type.
handy::Container<double> c(vector<int>{10, 20, 30});

// Or using more than one iterable (different types, but all of them are integral)
handy::Container<char> d(list<char>{10}, set<long>{20}, array<int, 1>{30});

// You can also use a range defined by iterators (or pointers)
int szs[] = {10, 20, 30, 10000000};

handy::Container<long> e(&szs[0], szs + 3);

// Or even a initializer list of integrals
handy::Container<float> f({10, 20, 30});


// These are some ways to access the containers via 'operator()'

// With variadic integrals
a(0, 1, 2) = 10;

// With iterables
b(vector<int>{3}, list<char>{4, 5}) = 20;

// With integrals or iterables (only for access, not allowed for creation)
c(9, set<int>{6, 7}) = 0.1;

// With list initialization
d({9, 19, 29}) = 'a';

// With a tuple of integrals/iterables (also only for access)
e(make_tuple(9, 19, 29)) = 20;

// And you can access it normally with 'operator[]'
f[50] = -10.0;

// You can also use any STL function. It is either a std::vector or a std::array, after all
sort(a.begin(), a.end());
```

You can also take a **slice** of a container like this:


``` C++
handy::Container<int> cnt(2, 3, 4);

std::iota(cnt.begin(), cnt.end(), 0);   // [0, 1, ..., 23]

auto slice1 = cnt.slice(1);  // [12, 13, ..., 23]

auto slice2 = cnt.slice(1, 2); // [20, 21, 22, 23]

auto slice3 = cnt.slice(0);  // [0, 1, ..., 11]

auto slice4 = cnt.slice(0, 1);  // [4, 5, 6, 7]

auto slice5 = cnt.slice(1, 2, 3);   // [23]

// The access is exactly the same as in the Container, but now in the new dimensions
auto x = slice1(0, 1);
```

<br>

## Range

Range utility similar to python's range

The range can have any kind of type. The only requirement is that the type must have an equality operator defined.

Also, you can defined half-closed intervals, closed intervals, and even infinite intervals.

An example showing how to define a half-closed interval:


``` C++
for(int x : handy::range(10))      // [0, 1, ..., 9]
    std::cout << x << "\n";

for(int x : handy::range(0, 10))   // same as above
    std::cout << x << "\n";

for(int x : handy::range(0, 10, 2))   // [0, 2, ..., 8] 
    std::cout << x << "\n";

// type is double. [0, 0.1, ..., 9.9]  -> It is not precise for floating points
for(double x : handy::range(0.0, 10.0, 0.1))
    std::cout << x << "\n";
```

For closed intervals the syntax is almost the same:

``` C++
for(int x : handy::crange(10))      // [0, 1, ..., 10]
    std::cout << x << "\n";

for(int x : handy::crange(0, 10))   // same as above
    std::cout << x << "\n";

for(int x : handy::crange(0, 10, 2))   // [0, 2, ..., 10] 
    std::cout << x << "\n";

// type is double. [0, 0.1, ..., 10.0]  -> Results may vary for floating points
for(double x : handy::crange(0.0, 10.0, 0.1))
    std::cout << x << "\n";
```

For infinite intervals you need, of course, stop the loop at some point:

``` C++
for(int x : handy::irange(10))      // [10, 11, ...]
{
    if(x == 100)
        break;

    std::cout << x << "\n";
}

for(int x : handy::irange())      // [0, 1, ...]
{
    if(x == 100)
        break;

    std::cout << x << "\n";
}
```

<br>

## Wrapper

Helper class for defining uniform interfaces.

The `handy::Wrapper` helps you to define classes with uniform interface by constructing a thin, zero runtime  overhead layer to access the methods of a given class.

You can create for example a `handy::Wrapper<int>` that has a specific `.size()` and `operator[]` functions, so you can iterate in a loop:


``` C++
// Using HasMember.h
HAS_OVERLOADED_FUNC(operator[], HasRandomAccess);


// For any type that does not have a operator[]
template <typename T>
struct RandomAccessScalar : public handy::Wrapper<T>
{
    USING_WRAPPER(handy::Wrapper<T>);

    T& operator [] (int) { return t; }

    int size () { return 1; }
};

// For any type that has a operator
template <typename T>
struct RandomAccessVector : public handy::Wrapper<T>
{
    USING_WRAPPER(handy::Wrapper<T>);

    decltype(auto) operator [] (int pos) { return t[pos]; }

    int size () { return t.size(); }
};

// Delegate to the right class using the HasRandomAccess checker
template <typename T>
using RandomAccess = std::conditional_t<HasRandomAccess<T, int>::value, RandomAccessVector<T>, RandomAccessScalar<T>>;



int main ()
{
    // A scalar
    RandomAccess<int> scalar(10);

    // A vector
    RandomAccess<std::vector<int>> vec{10, 20, 30};

    int res = 0;

    // Iterate for all elements of scalar (1)
    for(int i = 0; i < scalar.size(); ++i)
        res += scalar[i] * vec[i];


    return 0;
}
```


Another use is to easily define specialized operators for a given type:


``` C++
// For any type other than double, simply execute the regular operators
template <typename T>
struct SafeDouble : public handy::Wrapper<T> 
{
    // Inherit constructors and assignment operators
    USING_WRAPPER(handy::Wrapper<T>);
};


// For double, use a custom comparison
template <>
struct SafeDouble<double> : public handy::Wrapper<double>
{
    USING_WRAPPER(handy::Wrapper<double>);

    // All cases for comparison are treated
    template <typename U, std::enable_if_t<!handy::impl::IsWrapperBase<U>::value>* = nullptr>
    friend bool operator == (const SafeDouble& s1, const U& u)
    {
        return std::abs(s1.t - u) < 1e-8;
    }

    template <typename U, std::enable_if_t<!handy::impl::IsWrapperBase<U>::value>* = nullptr>
    friend bool operator == (const U& u, const SafeDouble& s1)
    {
        return operator==(s1, u);
    }

    friend bool operator == (const SafeDouble& s1, const SafeDouble& s2)
    {
        return operator==(s1, s2.t);
    }
};




int main ()
{
    // These should be the same thing
    double x = 10.0 / 3;
    double y = (1.0 / 3) * 10;

    // But they are not because of the limited floating point precision
    assert(x != y);

    // This is a safe way to compare
    assert(SafeDouble<double>(x) == SafeDouble<double>(y));


    return 0;
}
```

<br>

## ZipIter

Iterator zipper similar to Python

Simple facilities to iterate through multiple containers and iterators at the same time, similar to Python's `zip`. Works easily with stl algorithms as well.

It has zero runtime overhead when compiled with optimizations (at least -O2).

Examples of use:

``` C++
std::vector<int> v = {1, 2, 3, 4, 5};
std::array<double, 5> u = {5, 4, 3, 2, 1};



// Iterating through both containers using std::for_each
std::for_each(handy::zipBegin(v, u), handy::zipEnd(v, u), [](auto tup){
    std::cout << std::get<0>(tup) << "     " << std::get<1>(tup) << "\n";
});



// Using the handy::unZip to unpack those values.
// They can be taken as references too
std::for_each(handy::zipBegin(v, u), handy::zipEnd(v, u), handy::unZip([](int x, double y){
    std::cout << x << "     " << y << "\n";
}));



// Using for range -- The return is a tuple containing references
for(auto tup : handy::zip(v, u)) handy::unZip(tup, [](int x, double y){
    std::cout << x << "     " << y << "\n";
});



// Or using a function that encapsulates the above.
// The lambda comes after the variadic arguments 
handy::forEach(v, u, [](int x, double y){
    std::cout << x << "     " << y << "\n";
});


// If you have C++17, you can even use this one
for(auto [x, y] : handy::zip(v, u))
    std::cout << x << "     " << y << "\n";



// Sorting both containers using the std::tuple operator <
std::sort(handy::zipIter(v.begin(), u.begin()), handy::zipIter(v.end(), u.end()));


// or
std::sort(handy::zipBegin(v, u), handy::zipEnd(v, u));


// or even using a macro that does exactly the same as above
std::sort(ZIP_ALL(v, u));


// using a custom comparison
std::sort(ZIP_ALL(v, u), [](auto tup1, auto tup2){
    return std::get<0>(tup1) + std::get<1>(tup1) < std::get<0>(tup2) + std::get<1>(tup2);
});


// or using the handy::unZip to easily unpack these tuples
std::sort(ZIP_ALL(v, u), handy::unZip([](int v1, double u1, int v2, double u2){
    return v1 + u1 < v2 + u2;
}));



// It is really that easy
transform(ZIP_ALL(v, u), handy::zipBegin(v, u), handy::unZip([](int x, double y){
    return std::make_tuple(0, 0.0);
}));


std::reverse(ZIP_ALL(v, u));


accumulate(ZIP_ALL(v, u), 0.0, handy::unZip([](double sum, int x, double y){
    return sum + x + y;
}));
```


<br>

## Helpers

These are some helpers that are needed by the other modules.

I chose to put them in different files so it is way more readable and easy to extend. But you have to carry the folder `Helpers` whenever you are using other `handy` utilities.


### Benchmark

Precise time measurement for both Windows and Linux (up to nanoseconds). The time returned is in **seconds**.

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

<br>


### HandyParams

This is a very simple header that allows the creation of classes with (almost) python-like initialization

It is only supported in C++17, with the aid of the [std::any](http://en.cppreference.com/w/cpp/utility/any) class. Also, it incurs a small runtime overhead, and may be prone to type casting rrors, but the result is simply beautiful:


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

<br>


### HasMember

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

<br>




### NamedTuple

Very simple macros for providing named access to `std::tuple`.
    
You can create a class that inherits from `std::tuple`, having access functions defined by you. There is no space (for each object) or runtime overhead. Ex:

``` C++
// Define a struct called Triplet having functions first, second and third
NAMED_TUPLE(Triplet, first, second, third)

int main ()
{
    Triplet<int, double, std::string> triplet;

    std::get<0>(triplet) = 10;  
    
    triplet.first() = 10;       // Exactly the same as above
    triplet.second() = 20.0;
    triplet.third() = "abcdef";

    return 0;
}
```

Alternativelly, a less intrusive option is to defined named getters, to call `std::get` with a defined argument:


``` C++
// Define functions called x, y, z, that call std::get with arguments 0, 1 and 2
NAMED_GETTERS(x, y, z)

int main ()
{
    std::tuple<double, double, double> point3d;

    x(point3d) = 10.0;
    y(point3d) = 20.0;
    z(point3d) = 30.0;

    return 0;
}
```

<br>

### Print

Some useful printing utilities.

By creating a `handy::Print` you can define how to print stuff. Also supports stl container printing:

``` C++
handy::Print printer(", ", "");   // Output to std::cout, separated by ", " and without line break ("")

printer(10, 20, 30);  // "10, 20, 30"

printer(std::vector<int>{10, 20, 30});  // Same as bove


std::ofstream myFile("myFile.txt");

handy::Print filePrinter(myFile, ", ", "");   // Output set to 'myFile'

filePrinter(10, 20, 30);    // Print to 'myFile'

filePrinter(std::cout, 10, 20, 30);  // Here the output is to std::cout, not to 'myFile'

filePrinter(10, 20, 30);    // Print to 'myFile' again
```


You can also use the direct function `handy::print()`:

``` C++
handy::print(10, 20, 30);   // Exactly the same as handy::Print(std::cout, " ", "\n").operator()

std::ofstream myFile("myFile.txt");

handy::print(myFile, 10, 20, 30);   // Print to 'myFile' -> "10, 20, 30\n"

handy::print(myFile, std::vector<int>{10, 20, 30}); // Same as above
```

<br>


### Random

Utilities for sampling random numbers.

By creating a `handy::RandDouble` or `handy::RandInt` you can easily generate random numbers inside a given range:

``` C++
handy::RandInt randInt(0);  // If the seed is not given, a random seed is picked

int i1 = randInt(0, 10);
int i2 = randInt(10);     // Same as above


handy::RandDouble randDouble;

double d1 = randDouble();           // Random double in [0, 1]
double d2 = randDouble(0.0, 10.0);
double d3 = randDouble(10.0);       // Same as above


// You can also use the handy::Rand, which is a template
// For floating point types it is the same as handy::RandDouble and
// for integers handy::RandInt. Anything else will give you an error
handy::Rand<float> randFloat;

float f1 = randFloat(0.0, 10.0);
```


Also, you can use the `handy::rand(int, int, unsigned int)` or  `handy::rand(double, double, unsigned int)` directly:

``` C++
int i1 = handy::rand(0, 10);    // Argument is int -> call handy::RandInt

double d1 = handy::rand(0.0, 10.0);    // Argument is double -> call handy::RandDouble

double d2 = handy::rand(0.0, 10.0, 0);  // Seed is given. If called twice, it will give the same result
```


