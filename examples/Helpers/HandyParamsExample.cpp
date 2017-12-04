#include "Helpers/HandyParams.h"
#include <string>

//! [Handy Params snippet]
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

    Foo foo({{"a", 3}, {"c", 10.0f}, {"d", std::string("abcdefg")}});

    return 0;
}
//! [Handy Params snippet]