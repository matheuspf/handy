#include "Wrapper/Wrapper.h"
#include "gtest/gtest.h"

#include <random>
#include <vector>


namespace
{

template <typename T>
struct SafeFloat : public handy::Wrapper<T>
{
    USING_WRAPPER(handy::Wrapper<T>);

    friend bool operator == (const SafeFloat<T>& s1, const T& t)
    {
        return std::abs(*s1 - t) < 1e-8;
    }
};


TEST(InheritanceTest, SafeFloatTest)
{
    handy::Wrapper<double> wr = 10.0 / 3;

    SafeFloat<double> sf = 10.0 / 3;

    EXPECT_NE(wr, (1.0 / 3) * 10);
    EXPECT_EQ(sf, (1.0 / 3) * 10);
}



HAS_OVERLOADED_FUNC(operator[], HasRandomAccess);

template <typename T>
struct RandomAccessScalar : public handy::Wrapper<T>
{
    USING_WRAPPER(handy::Wrapper<T>);

    T& operator [] (int) { return t; }

    int size () { return 1; }
};


template <typename T>
struct RandomAccessVector : public handy::Wrapper<T>
{
    USING_WRAPPER(handy::Wrapper<T>);

    decltype(auto) operator [] (int pos) { return t[pos]; }

    int size () { return t.size(); }
};


template <typename T>
using RandomAccess = std::conditional_t<HasRandomAccess<T, int>::value, RandomAccessVector<T>, RandomAccessScalar<T>>;


template <class> class Prtt;

TEST(InheritanceTest, HasRandomAccessTest)
{
    RandomAccess<int> ra1(10);
    RandomAccess<std::vector<int>> ra2(10, 10);

    int s1 = 0, s2 = 0;

    for(int i = 0; i < ra2.size(); ++i)
    {
        s1 += ra1[i];
        s2 += ra2[i];
    }

    EXPECT_EQ(s1, 100);
    EXPECT_EQ(s2, 100);
}


    
}