#include "Benchmark.h"
#include <iostream>


using namespace std;



int main ()
{
    handy::Benchmark bench1;
    handy::impl::Benchmark<false> bench2;

    bench2.start();

    double bench3 = handy::benchmark([]
    {
        for(int i = 0; i < 1e6; ++i);
    });

    std::cout << bench1.finish() << "    " << bench2.finish() << "    " << bench3 << "\n\n";


    return 0;
}
