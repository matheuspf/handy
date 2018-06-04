#include "Helpers/Random.h"
#include "gtest/gtest.h"


namespace
{
    TEST(RandomTest, TestDouble)
    {
        unsigned int seed = handy::rand(0, 1000);
        
        handy::RandDouble rngDouble1(seed), rngDouble2(seed);

        for(int i = 0; i < 10; ++i)
        {
            double x1 = rngDouble1(), x2 = rngDouble2(0.0, 1.0);
            
            EXPECT_DOUBLE_EQ(x1, x2);
        }
        

        seed = handy::rand(0, 1000, -1);

        rngDouble1.seed(seed), rngDouble2.seed(seed);

        for(int i = 0; i < 10; ++i)
            EXPECT_DOUBLE_EQ(rngDouble1(1.0), rngDouble2());
    }


    TEST(RandomTest, TestInt)
    {
        unsigned int seed = handy::rand(0, 1000);
        
        handy::RandInt rngInt1(seed), rngInt2(seed);
        
        for(int i = 0; i < 10; ++i)
            EXPECT_EQ(rngInt1(0, 100), rngInt2(100));


        seed = handy::rand(0, 1000, -1);
            
        rngInt1.seed(seed), rngInt2.seed(seed);

        for(int i = 0; i < 10; ++i)
            EXPECT_EQ(rngInt1(-100, 100), rngInt2(-100, 100));
    }
}