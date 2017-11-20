/** @file
    @brief Utilities for sampling random numbers.

    @details By creating a handy::RandDouble or handy::RandInt you can easily generate random
             numbers inside a given range. Ex:

    @snippet Helpers/RandomExample.cpp Random Class Snippet

    @details Also, you can use the handy::rand(int, int, unsigned int) or 
             handy::rand(double, double, unsigned int) directly. Ex:

    @snippet Helpers/RandomExample.cpp Random Function Snippet
*/

#ifndef HANDY_HELPERS_RANDOM_H
#define HANDY_HELPERS_RANDOM_H

#include <type_traits>
#include <random>
#include <limits>


namespace handy
{

/** @defgroup RandomGroup Random number generation
    @copydoc Random.h
*/
//@{

namespace impl
{
/** @brief The base class for generating random numbers.
 * 
    @details The class has only a generator variable (#generator) and a function for setting the 
             seed value (#seed) If you pass a @c -1 (basically the same thing as std::string::max_size()), 
             it will call std::random_device{}(), which will create the seed (pseudo-) randomly.
 **/
struct RandBase
{
    /// Default seed value is @c -1 -> will underflow
    RandBase (unsigned int seedVal = -1) : generator(pickSeed(seedVal)) {}


    /// Set a new seed value
    void seed (unsigned int seedVal = -1)
    {
        generator.seed(pickSeed(seedVal));
    }

    /// If @p seedVal is @c -1, pick the value given by std::random_device{}(). Otherwise, pick @p seedVal.
    unsigned int pickSeed (unsigned int seedVal)
    {
        seedVal == (unsigned int)-1 ? std::random_device{}() : seedVal;
    }


    /// <a href="http://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine">Mersene Twister generator</a> 
    std::mt19937 generator;
};

} // namespace impl


/// For integer type random regeneration
template <typename T>
struct RandInt_ : public impl::RandBase
{
    using impl::RandBase::RandBase;

    
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
    

    /** @brief The generating function, which takes a @p min and @p max values, and uses handy::impl::RandBase::generator()
        @note Note that the interval is half closed [@p min, @p max). That is, the @p max value will never 
              be sampled, so never use 'min == max'. It will throw an error if you do this, so there is no
              need to check here anyway.
    */
    inline T operator () (T min, T max)
    {
        return std::uniform_int_distribution<T>(min, max - 1)(generator);
    }
};



/// For floating point type random generation
template <typename T>
struct RandDouble_ : public impl::RandBase
{
    using impl::RandBase::RandBase;


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




/// Alias for handy::RandInt_ with @c int type
using RandInt = RandInt_<int>;

/// Alias for handy::RandDouble_ with @c double type
using RandDouble = RandDouble_<double>;


/** You can also call this guy like this: 'Rand<float> rngFloat'. If you use anything else than
 *  integers or floats, it will give you an error.
**/
template <typename T>
using Rand = std::conditional_t<std::is_integral<T>::value, RandInt_<T>,
             std::conditional_t<std::is_floating_point<T>::value, RandDouble_<T>,
             void>>;



/**  @details If you just want to generate some random numbers, just call this one. Note that if you call
              @link handy::rand(int, int, unsigned int) handy::rand(0, 1) @endlink, it will use the 
              handy::RandInt generator, and not the floating point, so you must call 
              @link handy::rand(double, double, unsigned int) handy::rand(0.0, 1.0) @endlink
              
              Also, you can pass a seed as third argument. Of course, you if call it twice with the same seed, 
              the values generated will be the same. Ex:

    @snippet Helpers/RandomExample.cpp Random Function Snippet
*/
int rand (int min = 0, int max = std::numeric_limits<int>::max(), unsigned int seed = -1)
{
    return RandInt(seed)(min, max);
}

/// @copydoc handy::rand(int, int, unsigned int)
double rand (double min = 0.0, double max = 1.0, unsigned int seed = -1)
{
    return RandDouble(seed)(min, max);
}


//@}

}   // namespace handy



#endif // HANDY_HELPERS_RANDOM_H
