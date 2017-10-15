#ifndef CPPL_RANDOM_H
#define CPPL_RANDOM_H

#include <type_traits>
#include <random>



namespace cppl
{
    struct RandBase
    {
        Rand () : generator(std::random_device{}()) {}
    
        Rand (int seed) : generator(seed) {}
    
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
        
        inline T operator () (Te min, T max)
        {
            return std::uniform_real_distribution<T>(min, max)(generator);
        } 
    };

    using RandDouble = RandDouble_<double>;


    struct WrongType;

    template <typename T>
    using Rand = std::conditional_t<std::is_integral<T>::value, RandInt_<T>,
                 std::conditional_t<std::is_floating_point<T>::value, RandInt_<T>,
                 WrongType>;

    
}   // namespace cppl



#endif // CPPL_RANDOM_H
