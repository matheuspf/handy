/** @file
 *  @brief Some compile time configurations
*/

#ifndef HANDY_HELPERS_CONFIG_H
#define HANDY_HELPERS_CONFIG_H


/** @name
 *  @brief Semantic versioning
*/
//@{
#define HANDY_MAJOR_VERSION 1
#define HANDY_MINOR_VERSION 0
#define HANDY_PATCH_VERSION 0
//@}



/** @name
 *  @brief Easy way to override assertions/throws
 *  
 *  @details By default, HANDY_ASSERTS is defined, which means that when @c handy_assert is called, we will do
 *           a simple assertion. If HANDY_ASSERTS_WITH_INFO is defined instead, we will call assert, but first
 *           the file and line of the assertion will be written to std::cerr. If HANDY_THROWS is defined, a 
 *           std::runtime_error will be thrown, containing the file and line of the error.
 * 
 *           If you define an @c handy_assert macro BEFORE including handy, the macro will be called instead,
 *           just like Eigen does.
 * 
 *           If NDEBUG is defined however, @c handy_assert will expand to nothing, even if haved you defined 
 *           it beforehand. 
*/
//@{
#define HANDY_ASSERTS
//#define HANDY_ASSERTS_WITH_INFO
//#define HANDY_THROWS


#ifdef NDEBUG
    #ifdef handy_assert
        #undef handy_assert
        #define handy_assert(x)
    #endif
#else
    #ifndef handy_assert
        #ifdef HANDY_ASSERTS
            #define handy_assert(x) std::assert(x)
        #elif defined HANDY_ASSERTS_WITH_INFO
            #define handy_assert(x) if(!(x)) { std::cerr << "Assertion at file: " << __FILE__ << ", line: "   \
                                                        << __LINE__ << std::endl; std::assert(x); }
        #elif defined HANDY_THROWS
            #define handy_assert(x) if (!(x)) { throw (std::runtime_error(std::string("Throwing at file: ") + \
                                                std::string(__FILE__) + std::string(", line: ") +             \
                                                std::string(__LINE__))); }
        #endif
    #endif
#endif
//@}



#endif // HANDY_HELPERS_CONFIG_H

