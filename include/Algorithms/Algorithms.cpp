#include <iostream>
#include <vector>

#include "Algorithms.h"


using namespace handy;


#define TEST(a, b, ...) b



int main ()
{
    // std::vector<int> v{1, 4, 3, 7, 0, 2, 5, 6, 9, 8};

    // int x = v & stable_sort() & reverse() & transform(v, [](auto x){ return x*x; }) & random_shuffle() & accumulate(0);


    // std::cout << v << "\n" << x << "\n";

    int a = 10, b = 20, c = 30;

    std::cout << TEST(a, 20, 10) << "\n";




    return 0;
}