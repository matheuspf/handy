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
    Wrapper<int> b(y);
    Wrapper<double> c(z);

    SafeFloat<int> k(x);

    Wrapper<vector<int>> vx;


    k = 10 + k;

    k = a + b + x - c * x / a;
    k += x;
    k += c;

    k |= a;

    cout << k << "\n";



    return 0;
}