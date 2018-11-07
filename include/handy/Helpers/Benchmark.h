/** @file
    
    @brief Precise time measurement for both Windows and Linux systems

    @details By creating a handy::Benchmark (which is an alias to @link handy::impl::Benchmark 
             handy::impl::Benchmark<true> @endlink) object, you start the clock, whichs is precise 
             up to nanoseconds. The clock can also be created by calling handy::impl::Benchmark::start().

             The handy::impl::Benchmark::finish() returns the time elapsed in seconds since the
             last call to handy::impl::Benchmark::start().

             By default, the constructor will start the call, but you can also create a @link 
             handy::impl::Benchmark handy::impl::Benchmark<false> @endlink object, which will not start 
             the clock at creation. Ex:

    @snippet Helpers/BenchmarkExample.cpp Benchmark class snippet

    @details You can also use handy::impl::Benchmark::operator()(), which takes a function its 
             arguments, returning the time it takes to execute. The handy::benchmark() function
             does the exact same thing, but without the need to explicitly create an object. Ex:

    @snippet Helpers/BenchmarkExample.cpp Benchmark function snippet    

**/


#ifndef HANDY_HELPERS_BENCHMARK_H
#define HANDY_HELPERS_BENCHMARK_H

#include <type_traits>
#include <utility>
#include <time.h>

#ifdef _WIN32

#include <windows.h>

#elif defined __unix__

#include <sys/time.h>

#endif



namespace handy
{

namespace impl
{

/** @defgroup BenchmarkGroup Benchmarking
    @copydoc Benchmark.h
*/

//@{
/** @brief A class to do precise benchmarking on both Windows and Linux

    @tparam StartOnCreation @c bool template constant telling if the clock initialization must be done at creation

    @details If @p StartOnCreation is @c true, this class initializes the clock on creation. You can also reset it 
             by calling the start() function. The finish() function returns the time (in seconds, up to nanoseconds 
             of precision) elapsed since the call to the start() function.
**/
template <bool StartOnCreation = true>
class Benchmark
{
public:

    /// Call the start function on creation if @p StartOnCreation is @c true
    template <bool B = StartOnCreation, typename std::enable_if<B>::type* = nullptr>
    Benchmark ()
    {
        start();
    }

    /// Do nothing if @p StartOnCreation is @c false
    template <bool B = StartOnCreation, typename std::enable_if<!B>::type* = nullptr>
    Benchmark () {}

    

    /// You can also use this operator and pass a function along with its arguments. 
    template <class F, typename... Args>
    double operator () (F&& f, Args&&... args)
    {
        start();

        f(std::forward<Args>(args)...);

        return finish();
    }


    /// Start clock
    void start ()
    {
    #ifdef _WIN32
    
        QueryPerformanceFrequency(&freq_);
        QueryPerformanceCounter(&start_);
    
    #elif defined __unix__
    
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_);
        
    #endif
    }


    /// Stop clock and return time elapsed
    double finish()
    {
    #ifdef _WIN32
    
        QueryPerformanceCounter(&end_);
    
        double secondsElapsed = (end_.QuadPart - start_.QuadPart) / freq.QuadPart;
    
    #elif defined __unix__
    
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_);
    
        double secondsElapsed = (end_.tv_sec - start_.tv_sec) + (end_.tv_nsec - start_.tv_nsec) / 1e9;
    
    #endif 

        return secondsElapsed;
    }



private:


#ifdef _WIN32

    LARGE_INTEGER freq, start_, end_;   ///< Variables for windows

#elif defined __unix__
    
    timespec start_, end_;  ///< Variables for Linux

#endif

};

} // namespace impl



/// A alias calling handy::impl::Benchmark with @p StartOnCreation set to @c true
using Benchmark = impl::Benchmark<true>;


/** @brief This is the function you will actually call if you only want to call the handy::impl::Benchmark::operator()()
    @note No need to start on creation here
*/
template <class F, class... Args>
double benchmark (F&& f, Args&&... args)
{
    return impl::Benchmark<false>{}(std::forward<F>(f), std::forward<Args>(args)...);
}


} // namespace handy


#endif // HANDY_HELPERS_BENCHMARK_H
