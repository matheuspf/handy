#include "Helpers/Benchmark.h"
#include "gtest/gtest.h"


namespace
{

TEST(BenchMarkTest, SingleTest)
{
    handy::Benchmark bench1;
    handy::impl::Benchmark<false> bench2;

    bench2.start();


    double b3 = handy::benchmark([]
    {
        for(int i = 0; i < 1e6; ++i);
    });


    double b1 = bench1.finish();
    double b2 = bench2.finish();


    EXPECT_NEAR(b1, b2, 1e-4);
    EXPECT_NEAR(b1, b3, 1e-4);
    EXPECT_NEAR(b2, b3, 1e-4);
}


}