#include "../../include/Container/Container.h"

#include <algorithm>
#include <iostream>



int main ()
{
///[Slice Snippet]
    handy::Container<int> cnt(2, 3, 4);

    std::iota(cnt.begin(), cnt.end(), 0);   // [0, 1, ..., 23]

    auto slice1 = cnt.slice(1);  // [12, 13, ..., 23]

    auto slice2 = cnt.slice(1, 2); // [20, 21, 22, 23]

    auto slice3 = cnt.slice(0);  // [0, 1, ..., 11]

    auto slice4 = cnt.slice(0, 1);  // [4, 5, 6, 7]

    auto slice5 = cnt.slice(1, 2, 3);   // [23]

    // The access is exactly the same as in the Container, but now in the new dimensions
    auto x = slice1(0, 1);
///[Slice Snippet]

    return 0;
}