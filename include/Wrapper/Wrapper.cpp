#include <bits/stdc++.h>
#include "Wrapper.h"

#include "../Helpers/HasMember.h"


using namespace std;


template <class> class Prt;



template <typename U>
std::ostream& operator << (std::ostream& out, const Wrapper<U>& w)
{
    return out << w.t;
}




template <typename T>
auto makeWrapper (T&& t)
{
    return Wrapper<T>(std::forward<T>(t));
}


template <typename T, typename U>
auto operator + (const Wrapper<T>& w1, const Wrapper<U>& w2)
{
    Wrapper<std::decay_t<decltype(std::declval<T>() + std::declval<U>())>> w(w1.t);

    w += w2;

    return w;

    //return makeWrapper(w1.t + w2.t);
}




HAS_FUNC(operator==, HasBracket)


template <typename T>
struct WRP : public Wrapper<T>
{
    USING_WRAPPER(Wrapper<T>);

    using Base::operator+=;


    //template <typename U = std::decay_t<T>, std::enable_if_t<HasBracket<U>::value, int> = 0>
    decltype(auto) operator [] (int p)
    {
        return this->t[p];
    }

    // template <typename U = T, std::enable_if_t<!HasBracket<U>::value, int> = 0>
    // decltype(auto) operator [] (int)
    // {
    //     return this->t;
    // }
    


    template <typename U>
    WRP& operator += (const WRP<U>& w)
    {
        this->t -= w.t;
        return *this;
    }

    template <typename V, typename U>
    friend auto operator+ (const WRP<V>&, const WRP<U>&);
};



template <typename T>
auto makeWRP (const T& t)
{
    return WRP<std::decay_t<T>>(t);
}


template <typename T, typename U>
auto operator + (const WRP<T>& w1, const WRP<U>& w2)
{
    return makeWRP(w1.t - w2.t);
}




struct Foo
{
    void a (int) {}
    void a (double) {}
    void a (int, char, double, string) {}

    void a () {}
};



// template<class>
// constexpr bool hasA (...) { return false; }

// template <class T>
// constexpr bool hasA (std::decay_t<decltype(std::declval<T>().a(int{}), void())>*) { return true; }

// template <class T>
// struct HasA : public std::integral_constant<bool, hasA<std::decay_t<T>>(nullptr)> {};


HAS_OVERLOADED_FUNC(a, HasA)


//#define FUNC_HELPER(T, func) static_cast<void(T::*)(int)>(&T::func)

int main ()
{
    int x = 10, y = 20;
    double z = 30.5;

    WRP<int&> a(x), b(y);
    WRP<double&> c(z);
    
    c += a;

    // WRP<vector<int>> wv(vector<int>{10, 20, 30, 40});

    // cout << wv[2] << "\n";



    cout << HasA<Foo, int>::value << "\n";


    return 0;
}