#include <bits/stdc++.h>
#include "Wrapper.h"

#include "../Helpers/HasMember.h"


using namespace std;
using namespace handy;


template <class> class Prt;



template <typename T>
struct SafeFloat : public Wrapper<T>
{
    USING_WRAPPER(Wrapper<T>);
};



int main ()
{
    int x = 10, y = 20;
    double z = 30.5;

    Wrapper<int&> a(x);
    Wrapper<int&&> b(move(y));
    Wrapper<const double&> c(z);

    SafeFloat<int> k(x);

    Wrapper<vector<int>> vx;


    k = 10 + k;

    k = a + *b / k + x - c * x / a;
    k += x;
    k += c;

    k |= a;

    cout << k << "\n";

    cout << (k >= b) << "\n";



    return 0;
}