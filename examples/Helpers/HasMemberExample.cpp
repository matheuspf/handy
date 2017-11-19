#include "Helpers/HasMember.h"
#include <string>

//! [HasMember snippet]
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

    HasFoo<int>::value;     // false
    HasFoo<Foo>::value;     // true
    HasFoo<Goo>::value;     // false

    HasExternFoo<int>::value;   // true
    HasExternFoo<Foo>::value;   // false
    HasExternFoo<Foo, std::string>::value;   // true
    HasExternFoo<Foo, std::string, Goo>::value;   // true

    HasOverloadedBar<Foo, int>::value;   // true
    HasOverloadedBar<Foo, std::string>::value;   // true
    HasOverloadedBar<Foo, double>::value;   // true -> does not work for explicit overloads


    return 0;
}
//! [HasMember snippet]