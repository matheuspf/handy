#include <iostream>
#include <vector>

#include "Algorithms.h"


using namespace handy;




int main ()
{
    std::vector<int> v{1, 4, 3, 7, 0, 2, 5, 6, 9, 8};
    std::vector<int> u{1, 7, 3, 4, 0, 2, 5, 6, 9, 8};

    v = v & fill_n(5, 5) & fill(10) & transform(u, [](int x){ return 2 * x; }) & generate([]{ return -1; });

    //int x = v & stable_sort() & reverse() & transform(v, [](auto x){ return x*x; }) & random_shuffle() & accumulate(0) & equal(v);

    int x = (v & find_if([](int x){ return x == 5; })) - v.begin();

    std::cout << x << "\n";

    for(auto z : v)
        std::cout << z << " ";
    std::cout << "\n";



    return 0;
}