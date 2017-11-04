#include "NamedTuple.h"
#include <iostream>
#include <string>

using namespace std;


NAMED_TUPLE(Triad, first, second, third)
NAMED_GETTERS(first, second, third)


NAMED_TUPLE(Duple, first, second)


int main ()
{
    // Triad<int, char, std::string> t;

    // std::tuple<int, char, std::string>& tp = t;

    // first(t) = 10;

    // std::cout << t.first() << "\n";
    
    
    // Triad<int, double, std::string> triple(10, 20.0, "triple");
    

    std::tuple<int, std::string> tp;

    Duple<int, std::string> dp(tp);


    return 0;
}
