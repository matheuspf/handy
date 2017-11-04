#include "Random.h"
#include <iostream>


using namespace std;



int main ()
{
    unsigned int seed = handy::rand(0, 1000);

    handy::RandDouble randDouble(seed), rd(seed);

    seed = handy::rand(0, 1000);

    randDouble.seed(seed), rd.seed(seed);

    for(int i = 0; i < 100; ++i)
        std::cout << randDouble() << "    " << rd(1.0) << "\n";



    return 0;
}
