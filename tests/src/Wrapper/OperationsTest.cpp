#include "Wrapper/Wrapper.h"
#include "gtest/gtest.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <random>


namespace
{

struct OperationsTest : public ::testing::Test
{
    OperationsTest () : a(x), b(y), c(w), d(std::move(z)) {}

    virtual ~OperationsTest () {}


    virtual void SetUp () {}

    virtual void TearDown () {}


    int x = 10, y = 20;
    double w = 30.5, z = 41.1;
    
    handy::Wrapper<int> a;
    handy::Wrapper<int&> b;
    handy::Wrapper<const double&> c;
    handy::Wrapper<double&&> d;
};


TEST_F(OperationsTest, ArithmeticTest1)
{
    handy::Wrapper<double> op1 = a + b + c + d;

    handy::Wrapper<double> op2 = a + b - c - d;


    EXPECT_DOUBLE_EQ(op1, 101.6);
    EXPECT_DOUBLE_EQ(op2, -41.6);
}


TEST_F(OperationsTest, ArithmeticTest2)
{
    handy::Wrapper<int> op1 = std::llround(10 * c + a / 2);

    handy::Wrapper<double> op2 = c + d - 10 * a;


    EXPECT_EQ(op1, 310);
    EXPECT_DOUBLE_EQ(op2, -28.4);
}



TEST_F(OperationsTest, ComparisonTest1)
{
    EXPECT_TRUE((a < b));
    EXPECT_TRUE((b >= a));
    EXPECT_TRUE((c != a));
    EXPECT_TRUE((d > c));
    EXPECT_TRUE((c != b && c <= d));
    EXPECT_TRUE((c == b || c <= d));
}


TEST_F(OperationsTest, ComparisonTest2)
{
    std::vector<handy::Wrapper<double>> v(100);

    std::mt19937 gen(std::random_device{}());

    std::generate(v.begin(), v.end(), [&]{ return std::uniform_real_distribution<>(0.0, 100.0)(gen); });

    std::sort(v.begin(), v.end());


    for(int i = 0; i < v.size()-1; ++i)
        EXPECT_LE(v[i], v[i+1]);
}



    
}