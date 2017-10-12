#ifndef KERNEL_H
#define KERNEL_H
#include <bits/stdc++.h>
#include <boost/thread.hpp>

namespace rng
{





template <class Distribution, class Generator = std::mt19937>
class Random
{
public:


    using DistributionType = Distribution;
    using GeneratorType    = Generator;


    using ValueType = std::decay_t<decltype(std::declval<Generator>().operator()(std::declval<Distribution>()))>;


    Random (int maxSize = 1000, float loadFactor = 0.7, int numThreads = 1) :
            distribution(DistributionType()), generator(GeneratorType()), elements(std::vector<ValueType>(maxSize)),
            threads(std::vector<boost::thread>(numThreads)), position(0), maxSize(maxSize), loadFactor(loadFactor), numThreads(numThreads)
    {

    }






//private:


    DistributionType distribution;
    GeneratorType generator;


    std::vector<ValueType> elements;
    std::vector<boost::thread> threads;

    int position;
    int maxSize;
    float loadFactor;
    int numThreads;

};

}   // namespace rng



#endif
