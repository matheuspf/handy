/** \file Random.h
 * 
 *  Utilities for sampling random numbers.
 * 
 **/

#ifndef CPPL_RANDOM_H
#define CPPL_RANDOM_H

#include <type_traits>
#include <random>
#include <limits>


namespace handy
{

namespace impl
{
    /** The base class. It only has a generator and functions for setting the seed value.
     *  If you pass a '-1' (basically the same thing as 'std::string::max_size()'), it 
     *  will call 'std::random_device{}()', which will create the seed (pseudo-) randomly.
     **/
    struct RandBase
    {
        /// Default is '-1'
        RandBase (unsigned int seedVal = -1) : generator(pickSeed(seedVal)) {}


        /// Set a new seed value
        void seed (unsigned int seedVal = -1)
        {
            generator.seed(pickSeed(seedVal));
        }

        
    protected:

        /// If seedVal is '-1', choose the value given by 'std::random_device{}()'
        unsigned int pickSeed (unsigned int seedVal)
        {
            seedVal == (unsigned int)-1 ? std::random_device{}() : seedVal;
        }


        std::mt19937 generator;     /// The generator
    };
    
    

    /// For integer types only
    template <typename T>
    struct RandInt_ : public RandBase
    {
        using RandBase::RandBase;

        
        /// With no arguments, it will generate numbers between [0, std::numeric_limits<T>::max())
        inline T operator () ()
        {
            return operator()(T{0}, std::numeric_limits<T>::max());
        }

        /// With a single argument, it will generate numbers between [0, max)
        inline T operator () (T max)
        {
            return operator()(T{0}, max);
        }
        

        /**  The generating function, which takes a 'min' and 'max' values, and uses 'RandBase::generator'.
         *   Note that the interval is half closed [min, max). That is, the 'max' value will never be sampled,
         *   so never use 'min == max'. It will throw an error if you do this, so I don't need to check here anyway.
         **/
        inline T operator () (T min, T max)
        {
            return std::uniform_int_distribution<T>(min, max - 1)(generator);
        }
    };
    

    /// For floating points
    template <typename T>
    struct RandFloat_ : public RandBase
    {
        using RandBase::RandBase;


        /// With no arguments, it will generate numbers between [0.0, 1.0)
        inline T operator () ()
        {
            return operator()(T{0.0}, T{1.0});
        }

        /// With a single argument, it will generate numbers between [0.0, max)
        inline T operator () (T max)
        {
            return operator()(T{0.0}, max);
        }
        
        /// Same as 'RandInt_', but using a real uniform distribution now. It is also half closed [min, max).
        inline T operator () (T min, T max)
        {
            return std::uniform_real_distribution<T>(min, max)(generator);
        } 
    };

} // namespace impl


    /// Aliases
    using RandInt = impl::RandInt_<int>;

    using RandDouble = impl::RandFloat_<double>;


    /** You can also call this guy like this: 'Rand<float> rngFloat'. If you use anything else than
     *  integers or floats, it will give you an error.
    **/
    template <typename T>
    using Rand = std::conditional_t<std::is_integral<T>::value, impl::RandInt_<T>,
                 std::conditional_t<std::is_floating_point<T>::value, impl::RandFloat_<T>,
                 void>>;

    

    /**  If you just want to generate some random numbers, just call this one. Note that if you call
     *   'rand(0, 1)', it will use the 'RandInt' generator, and not the floating point, so you must
     *   call 'rand(0.0, 1.0)'. Also, you can pass a seed as third argument. Of course, you if call
     *   it twice with the same seed, the values generated will be the same.
    **/ 
    int rand (int min = 0, int max = std::numeric_limits<int>::max(), unsigned int seed = -1)
    {
        return RandInt(seed)(min, max);
    }

    double rand (double min = 0.0, double max = 1.0, unsigned int seed = -1)
    {
        return RandDouble(seed)(min, max);
    }

    
}   // namespace handy



#endif // CPPL_RANDOM_H
