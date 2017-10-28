#include <iostream>
#include <vector>

#include "Algorithms.h"

#include "../Helpers/Helpers.h"



using namespace alg;


void lel () {}

int main ()
{
    std::vector<int> v{1, 4, 3, 7, 0, 2, 5, 6, 9, 8};

    int x = v & stable_sort() & reverse() & transform(v, [](auto x){ return x*x; }) & random_shuffle() & accumulate(0);


    std::cout << v << "\n" << x << "\n";




    return 0;
}