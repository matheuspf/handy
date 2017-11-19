#include "Algorithms/Algorithms.h"

#include <vector>


int main ()
{
//! [Algorithms Snipet functions]
    std::vector<int> v, u;

    handy::sort(v, std::greater<int>());

    handy::copy(v, u);

    std::vector<int> transformed = handy::transform(v, u, [](int x){ return 2 * x });
//! [Algorithms Snipet functions]


//! [Algorithms Snipet pipeline]
    std::vector<int> sortedLeft = v & handy::sort();

    std::vector<int> sortedRight = handy::sort() & v;

    int sum = u & handy::reverse() & handy::accumulate(0);

    auto finder = handy::find_if([](auto x){ return x == 10; });

    bool foundV = v & finder;

    bool foundU = finder(u);
//! [Algorithms Snipet pipeline]


    return 0;
}