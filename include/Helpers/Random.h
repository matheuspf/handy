#ifndef CPPL_RANDOM_H
#define CPPL_RANDOM_H

#include <type_traits>
#include <random>


namespace handy
{
    struct RandBase
    {
        RandBase (unsigned int seed = -1) : generator(seed == (unsigned int)-1 ? std::random_device{}() : seed) {}
    
        std::mt19937 generator;
    };
    
    
    template <typename T>
    struct RandInt_ : public RandBase
    {
        using RandBase::RandBase;
    
        inline T operator () (T min, T max)
        {
            return std::uniform_int_distribution<T>(min, max - 1)(generator);
        }
    };

    using RandInt = RandInt_<int>;
    

    template <typename T>
    struct RandDouble_ : public RandBase
    {
        using RandBase::RandBase;
        
        inline T operator () (T min, T max)
        {
            return std::uniform_real_distribution<T>(min, max)(generator);
        } 
    };

    using RandDouble = RandDouble_<double>;


    struct RandWrongType;

    template <typename T>
    using Rand = std::conditional_t<std::is_integral<T>::value, RandInt_<T>,
                 std::conditional_t<std::is_floating_point<T>::value, RandDouble_<T>,
                 RandWrongType>;

    

    template <typename T>
    T rand (T min, T max, unsigned int seed = -1)
    {
        return Rand<T>(seed)(min, max);
    }
    
}   // namespace handy



#endif // CPPL_RANDOM_H
