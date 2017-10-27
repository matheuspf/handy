#include <iostream>
#include <vector>

#include "STLContainer.h"


namespace std
{
    template <typename T>
    std::ostream& operator << (std::ostream& out, const std::vector<T>& v)
    {
        std::for_each(std::begin(v), std::end(v) - 1, [&](const auto& x)
        {
            out << x << " ";    
        });

        out << v.back() << "\n";
    }
}





int main ()
{
    std::vector<int> v{1, 4, 3, 7, 0, 2, 5, 6, 9, 8};

    std::cout << algc::all_of(v, [](int x){ return x > -1; }) << "\n";


    return 0;
}