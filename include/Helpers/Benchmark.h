#ifndef BENCHMARK_H_INCLUDED
#define BENCHMARK_H_INCLUDED
#include <ratio>
#include <iostream>
#include <ctime>

#ifdef _WIN32
#include <windows.h>

#elif defined __unix__
#include <sys/time.h>

#endif


template <class F, class... Args>
inline auto benchmark (F&& f, Args&&... args)
{

#ifdef _WIN32

    LARGE_INTEGER freq, start, end;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);


#elif defined __unix__

    timespec start, end;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

#endif


    std::forward<F>(f)(std::forward<Args>(args)...);


#ifdef _WIN32

    QueryPerformanceCounter(&end);

    return (end.QuadPart - start.QuadPart) / freq.QuadPart;


#elif defined __unix__

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    //return Timer::den * static_cast<long double>(end.tv_nsec- start.tv_nsec);

#endif

}


#endif // BENCHMARK_H
