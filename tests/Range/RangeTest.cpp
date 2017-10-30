#include <vector>
#include <algorithm>
#include <random>

#include "Range/Range.h"
#include "gtest/gtest.h"


namespace
{
    struct RangeTest : public ::testing::Test
    {
        virtual void SetUp ()
        {
            gen = std::mt19937(std::random_device{});

            v = std::vector<int>(n);

            std::generate(v.begin(), v.end(), [&]{ return std::uniform_int_distribution<>(0, 100)(gen);  });
        }

        virtual void TearDown () {}

        virtual ~RangeTest () {}



        int n = 10;

        std::vector<int> v;

		std::mt19937 gen;
    };


    TEST_F(RangeTest, ForRange)
    {
        for(int i = 0; i < n; ++i)
            v.push_back(i);

        for(auto i : rg::range(n))
            u.push_back(i);

        EXPECT_EQ(v, u);

        u.clear();


        for(auto i : rg::range(0, n))
            u.push_back(i);

        EXPECT_EQ(v, u);
        
        u.clear();


        for(auto i : rg::range(0, n, 1))
            u.push_back(i);

        EXPECT_EQ(v, u);

        u.clear();
    }
}