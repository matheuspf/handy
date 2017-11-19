#include "Helpers/NamedTuple.h"
#include <string>


//! [Named Tuple Snippet]
// Define a struct called Triplet having functions first, second and third

NAMED_TUPLE(Triplet, first, second, third)


void namedTupleExample ()
{
    Triplet<int, double, std::string> triplet;

    std::get<0>(triplet) = 10;  
    
    triplet.first() = 10;       // Exactly the same as above
    triplet.second() = 20.0;
    triplet.third() = "abcdef";
}
//! [Named Tuple Snippet]


//! [Named Getters Snippet]
// Define functions called x, y, z, that call std::get with arguments 0, 1 and 2

NAMED_GETTERS(x, y, z)


void namedGettersExample ()
{
    std::tuple<double, double, double> point3d;

    x(point3d) = 10.0;
    y(point3d) = 20.0;
    z(point3d) = 30.0;
}
//! [Named Getters Snippet]



int main ()
{
    return 0;
}