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


    handy::RandDouble_<float> randFloat;     // You can also use the '_' version, which is a template

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