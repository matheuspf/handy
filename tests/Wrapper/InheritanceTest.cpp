#include "Wrapper/Wrapper.h"
#include "gtest/gtest.h"

#include <random>


namespace
{

template <typename T>
struct SafeFloat : public ::handy::Wrapper<T>
{
    USING_WRAPPER(::handy::Wrapper<T>);

    template <typename U>
    friend bool operator == (const SafeFloat<T>& s1, const SafeFloat<U>& s2)
    {
        return std::abs(*s1 - *s2) < 1e-8;
    }
};


TEST(InheritanceTest, SafeFloatTest)
{
    ::handy::Wrapper<double> w1(1.0), w2(1.0);

    SafeFloat<double> s1(1.0), s2(1.0);

    std::mt19937 gen(std::random_device{}());


    for(int i = 0; i < 1000; ++i)
    {
        double x = std::uniform_real_distribution<>(-100.0, 100.0)(gen);
        w1 *= x, w2 *= x;
        s1 *= x, s2 *= x;
    }

    for(int i = 0; i < 1000; ++i)
    {
        double x = std::uniform_real_distribution<>(-100.0, 100.0)(gen);
        w1 /= x, w2 /= x;
        s1 /= x, s2 /= x;
    }


    EXPECT_TRUE(w1 == w2);
    EXPECT_TRUE(s1 == s2);
}


    
}