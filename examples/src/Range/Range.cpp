#include "Range/Range.h"
#include <iostream>


void halfClosedRangeExample ()
{
/// [HalfClosed Range Snippet]
    for(int x : handy::range(10))      // [0, 1, ..., 9]
        std::cout << x << "\n";

    for(int x : handy::range(0, 10))   // same as above
        std::cout << x << "\n";

    for(int x : handy::range(0, 10, 2))   // [0, 2, ..., 8] 
        std::cout << x << "\n";
    
    // type is double. [0, 0.1, ..., 9.9]  -> It is not precise for floating points
    for(double x : handy::range(0.0, 10.0, 0.1))
        std::cout << x << "\n";
/// [HalfClosed Range Snippet]
}


void closedRangeExample ()
{
/// [Closed Range Snippet]
    for(int x : handy::crange(10))      // [0, 1, ..., 10]
        std::cout << x << "\n";

    for(int x : handy::crange(0, 10))   // same as above
        std::cout << x << "\n";

    for(int x : handy::crange(0, 10, 2))   // [0, 2, ..., 10] 
        std::cout << x << "\n";
    
    // type is double. [0, 0.1, ..., 10.0]  -> Results may vary for floating points
    for(double x : handy::crange(0.0, 10.0, 0.1))
        std::cout << x << "\n";
/// [Closed Range Snippet]
}


void infiniteRangeExample ()
{
/// [Infinite Range Snippet]
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
/// [Infinite Range Snippet]
}


int main ()
{
    halfClosedRangeExample();
    closedRangeExample();
    infiniteRangeExample();

    return 0;
}