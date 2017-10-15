#ifndef CPPL_BENCHMARK_H_INCLUDED
#define CPPL_BENCHMARK_H_INCLUDED

#include <type_traits>

#ifdef _WIN32

#include <windows.h>

#elif defined __unix__

#include <sys/time.h>

#endif



namespace cppl
{

struct Benchmark
{
    template <class F, typename... Args>
    double operator () (F&& f, Args&&... args)
    {
        start();

        f(std::forward<Args>(args)...);

        return end();
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


    double end()
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


#ifdef _WIN32

    LARGE_INTEGER freq, start_, end_;

#elif defined __unix__
    
    timespec start_, end_;

#endif

};



template <class F, class... Args>
double benchmark (F&& f, Args&&... args)
{
    return Benchmark{}(std::forward<F>(f), std::forward<Args>(args)...);
}

} // namespace cppl


#endif // CPPL_BENCHMARK_H
