#include "Helpers/Random.h"


void RandomClassExample ()
{
//! [Random Class Snippet]
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

    flaot f1 = randFloat(0.0, 10.0);
//! [Random Class Snippet]
}


void RandomFunctionExample ()
{
//! [Random Function Snippet]
    int i1 = handy::rand(0, 10);    // Argument is int -> call handy::RandInt

    double d1 = handy::rand(0.0, 10.0);    // Argument is double -> call handy::RandDouble

    double d2 = handy::rand(0.0, 10.0, 0);  // Specific seed. If called twice, it will give the same result
//! [Random Function Snippet]
}


int main ()
{
    RandomClassExample();    
    RandomFunctionExample();    

    
    return 0;
}