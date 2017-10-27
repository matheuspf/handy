#include <iostream>
#include <vector>

#include "STLContainer.h"


namespace std
{
    template <typename T>
    std::ostream& operator << (std::ostream& out, const std::vector<T>& v)
    {
        std::for_each(std::begin(v), std::begin(v) + v.size() - 1, [&](const auto& x)
        {
            out << x << " ";    
        });

        return out << v.back() << "\n";
    }
}



using namespace algc;


void lel () {}

int main ()
{
    std::vector<int> v{1, 4, 3, 7, 0, 2, 5, 6, 9, 8};

    v = (v & sort_(std::greater<int>{})) & sort_();

    //v = sort(v);

    std::cout << v << "\n";


    return 0;
}