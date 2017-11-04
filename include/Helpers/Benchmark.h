/** \file Benchmark.h
 * 
 *  Precise time measurement for both Windows and Linux systems.
**/


#ifndef CPPL_BENCHMARK_H_INCLUDED
#define CPPL_BENCHMARK_H_INCLUDED

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

namespace impl  /// The main class goes here
{

/** If 'StartOnCreation' is true,  this class initializes the clock on creation. You can also reset it 
 *  by calling the 'start' function. The 'finish' function returns the time elapsed (in seconds, 
 *  up to nanoseconds of precision) elapsed since the call to the 'start' function.
**/
template <bool StartOnCreation = true>
class Benchmark
{
public:

    /// Call the start function on creation if 'StartOnCreation' is true
    template <bool B = StartOnCreation, typename std::enable_if<B>::type* = nullptr>
    Benchmark ()
    {
        start();
    }

    /// Do nothing if 'StartOnCreation' is false
    template <bool B = StartOnCreation, typename std::enable_if<!B>::type* = nullptr>
    Benchmark () {}

    

    /// You can also use 'operator()' and pass a function along with its arguments. 
    template <class F, typename... Args>
    double operator () (F&& f, Args&&... args)
    {
        start();

        f(std::forward<Args>(args)...);

        return finish();
    }


    
    void start ()
    {
    #ifdef _WIN32
    
        QueryPerformanceFrequency(&freq_);
        QueryPerformanceCounter(&start_);
    
    #elif defined __unix__
    
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_);
        
    #endif
    }



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

    LARGE_INTEGER freq, start_, end_;

#elif defined __unix__
    
    timespec start_, end_;

#endif

};

} // namespace impl



/// By default, we start on creation
using Benchmark = impl::Benchmark<true>;


/// This is the function you will actually call if you only want to call 'operator()'. No need to start on creation here.
template <class F, class... Args>
double benchmark (F&& f, Args&&... args)
{
    return impl::Benchmark<false>{}(std::forward<F>(f), std::forward<Args>(args)...);
}


} // namespace handy


#endif // CPPL_BENCHMARK_H
