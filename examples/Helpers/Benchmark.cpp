#include "Helpers/Benchmark.h"


void benchmarkClassExample ()
{
//! [Benchmark class snippet]
    handy::Benchmark bm;

    // Do something

    double timeElapsed = bm.finish();

    bm.start();

    // Do something more

    double moreTimeElapsed = bm.finish();
//! [Benchmark class snippet]
}


void benchmarkFunctionExample ()
{
    //! [Benchmark function snippet]
    handy::Benchmark bm;

    auto func = []{ /* Do something */ };

    double timeElapsed = bm(func);

    double moreTimeElapsed = handy::benchmark(func);
//! [Benchmark function snippet]
}


int main ()
{
    benchmarkClassExample();
    benchmarkFunctionExample();

    return 0;
}