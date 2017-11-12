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

    Wrapper<int> a(x);
    Wrapper<const int&> b(y);
    Wrapper<const double&> c(z);

    SafeFloat<int> k(x);

    auto r = a + k;


    cout << r << "\n";



    return 0;
}