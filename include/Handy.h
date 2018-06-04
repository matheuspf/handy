/** @file
    
    @brief Main header file

    Includes all other files. Also, it checks for compiler compatibility
*/

#pragma once


// C++14
#if __cplusplus >= 201402L


#include "Helpers/Benchmark.h"
#include "Helpers/HasMember.h"
#include "Helpers/NamedTuple.h"
#include "Helpers/Print.h"
#include "Helpers/Random.h"


#include "Algorithms/Algorithms.h"

#include "Container/Container.h"

#include "Range/Range.h"

#include "Wrapper/Wrapper.h"

#include "ZipIter/ZipIter.h"


// C++17
#if __cplusplus >= 201703L

#include "Helpers/HandyParams.h"

#endif // C++17


#else

#error You need at least C++14 to use Handy

#endif // C++14
