#include <bits/stdc++.h>
#include "Wrapper.h"

#include "../Helpers/HasMember.h"


using namespace std;
using namespace handy;


template <class> class Prt;




// template <typename T>
// auto makeWrapper (T&& t)
// {
//     return Wrapper<T>(std::forward<T>(t));
// }


// template <typename T, typename U>
// auto operator + (const Wrapper<T>& w1, const Wrapper<U>& w2)
// {
//     Wrapper<std::decay_t<decltype(std::declval<T>() + std::declval<U>())>> w(w1.t);

//     w += w2;

//     return w;

//     //return makeWrapper(w1.t + w2.t);
// }




// HAS_FUNC(operator==, HasBracket)


// template <typename T>
// struct WRP : public Wrapper<T>
// {
//     USING_WRAPPER(Wrapper<T>);

//     using Base::operator+=;


//     //template <typename U = std::decay_t<T>, std::enable_if_t<HasBracket<U>::value, int> = 0>
//     decltype(auto) operator [] (int p)
//     {
//         return this->t[p];
//     }

//     // template <typename U = T, std::enable_if_t<!HasBracket<U>::value, int> = 0>
//     // decltype(auto) operator [] (int)
//     // {
//     //     return this->t;
//     // }
    


//     template <typename U>
//     WRP& operator += (const WRP<U>& w)
//     {
//         this->t -= w.t;
//         return *this;
//     }

//     template <typename V, typename U>
//     friend auto operator+ (const WRP<V>&, const WRP<U>&);
// };



// template <typename T>
// auto makeWRP (const T& t)
// {
//     return WRP<std::decay_t<T>>(t);
// }


// template <typename T, typename U>
// auto operator + (const WRP<T>& w1, const WRP<U>& w2)
// {
//     return makeWRP(w1.t - w2.t);
// }




// struct Foo
// {
//     void a (int) {}
//     // void a (double) {}
//     // void a (int, char, double, string) {}
//     void a (string) {}

//     void a () {}
// };


// HAS_OVERLOADED_FUNC(a, HasA)


// template <class T>
// struct Goo
// {
//     template <typename... Args, std::enable_if_t<HasA<T, Args...>::value, int> = 0>
//     decltype(auto) a (Args&&... args)
//     {
//         return t.a(std::forward<Args>(args)...);
//     }

//     T t;
// };



// HAS_EXTERN_FUNC(operator+, HasPlusEqual)

// void lol (int) {}



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
    Wrapper<const int&> b(y);
    Wrapper<const double&> c(z);

    SafeFloat<int> k(x);

    auto r = k + a;

    //auto r = (a + c - b + 5 + k) < 10;


    cout << r << "\n";



    //cout << HasPlusEqual<int>::value << "\n";


    // WRP<vector<int>> wv(vector<int>{10, 20, 30, 40});

    // cout << wv[2] << "\n";

    
    // Goo<Foo> g;

    // g.a(10);

    //cout << HasA<Foo, int>::value << "\n";


    return 0;
}