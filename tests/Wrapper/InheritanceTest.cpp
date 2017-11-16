#include "Wrapper/Wrapper.h"
#include "gtest/gtest.h"

#include <random>


namespace
{

template <typename T>
struct SafeFloat : public ::handy::Wrapper<T>
{
    USING_WRAPPER(::handy::Wrapper<T>);

    // operator T& () { return t; }

    // operator T& () { return t; }
};


template <typename T>
bool operator == (const SafeFloat<T>& s1, const T& t)
{
    return std::abs(*s1 - t) < 1e-8;
}


TEST(InheritanceTest, SafeFloatTest)
{
    ::handy::Wrapper<double> wr = 10.0 / 3;

    SafeFloat<double> sf = 10.0 / 3;

    EXPECT_NE(wr, (1.0 / 3) * 10);
    EXPECT_EQ(sf, (1.0 / 3) * 10);
}


    
}