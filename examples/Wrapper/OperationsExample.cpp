#include "../../include/Wrapper/Wrapper.h"


#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>


///[Wrapper Operations Snippet]
// For any type other than double, simply execute the regular operators
template <typename T>
struct SafeDouble : public handy::Wrapper<T> 
{
    // Inherit constructors and assignment operators
    USING_WRAPPER(handy::Wrapper<T>);
};


// For double, use a custom comparison
template <>
struct SafeDouble<double> : public handy::Wrapper<double>
{
    USING_WRAPPER(handy::Wrapper<double>);

    // All cases for comparison are treated
    template <typename U, std::enable_if_t<!handy::impl::IsWrapperBase<U>::value>* = nullptr>
    friend bool operator == (const SafeDouble& s1, const U& u)
    {
        return std::abs(s1.t - u) < 1e-8;
    }

    template <typename U, std::enable_if_t<!handy::impl::IsWrapperBase<U>::value>* = nullptr>
    friend bool operator == (const U& u, const SafeDouble& s1)
    {
        return operator==(s1, u);
    }

    friend bool operator == (const SafeDouble& s1, const SafeDouble& s2)
    {
        return operator==(s1, s2.t);
    }
};




int main ()
{
    // These should be the same thing
    double x = 10.0 / 3;
    double y = (1.0 / 3) * 10;

    // But they are not because of the limited floating point precision
    assert(x != y);

    // This is a safe way to compare
    assert(SafeDouble<double>(x) == SafeDouble<double>(y));


    return 0;
}
///[Wrapper Operations Snippet]