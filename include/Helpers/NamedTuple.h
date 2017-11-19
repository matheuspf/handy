/** @file

    @brief Very simple macros for providing named access to std::tuple
    
    @details You can create a class that inherits from std::tuple, having access functions defined
             by you. There is no space (for each object) or runtime overhead. Ex:

    @snippet Helpers/NamedTupleExample.cpp Named Tuple Snippet

    @details Alternativelly, a less intrusive option is to defined named getters, to call std::get with
             a constant argument. Ex:

    @snippet Helpers/NamedTupleExample.cpp Named Getters Snippet        
**/

#ifndef HANDY_HELPERS_NAMED_TUPLE_H
#define HANDY_HELPERS_NAMED_TUPLE_H

#include "Helpers.h"
#include "HasMember.h"
#include <tuple>


/** @defgroup NamedTupleGroup Named Tuple
    @copydoc NamedTuple.h
*/
//@{

/// Auxiliary macro that calls std::get<I> when you call the @p NAME member function
#define MAKE_TUPLE_FUNC_AT(NAME, I) \
decltype(auto) NAME ()  \
{   \
    return std::get<I>(static_cast<Tuple&>(*this)); \
}

/** @name
    @brief Generates the code for the named member functions of the class created by calling NAMED_TUPLE()
    @note Up to 10 functions. Copy-pase to create more if you want
*/
//@{
#define MAKE_TUPLE_FUNC1(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 0)
#define MAKE_TUPLE_FUNC2(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 1)  MAKE_TUPLE_FUNC1(__VA_ARGS__)
#define MAKE_TUPLE_FUNC3(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 2)  MAKE_TUPLE_FUNC2(__VA_ARGS__)
#define MAKE_TUPLE_FUNC4(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 3)  MAKE_TUPLE_FUNC3(__VA_ARGS__)
#define MAKE_TUPLE_FUNC5(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 4)  MAKE_TUPLE_FUNC4(__VA_ARGS__)
#define MAKE_TUPLE_FUNC6(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 5)  MAKE_TUPLE_FUNC5(__VA_ARGS__)
#define MAKE_TUPLE_FUNC7(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 6)  MAKE_TUPLE_FUNC6(__VA_ARGS__)
#define MAKE_TUPLE_FUNC8(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 7)  MAKE_TUPLE_FUNC7(__VA_ARGS__)
#define MAKE_TUPLE_FUNC9(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 8)  MAKE_TUPLE_FUNC8(__VA_ARGS__)
#define MAKE_TUPLE_FUNC10(NAME, ...) MAKE_TUPLE_FUNC_AT(NAME, 9)  MAKE_TUPLE_FUNC9(__VA_ARGS__)
//@}


/** @brief This macro generates a class that inherits from 'std::tuple' and adds the function 
           names that you pass to call std::get in the given order of the parameters

    @note Of course, you will not play with pointers for this class, because it can cause leaks
**/
#define NAMED_TUPLE(TUPLE_NAME, ...)    \
template <typename... Args> \
struct TUPLE_NAME : public std::tuple<Args...>  \
{   \
    using Tuple = std::tuple<Args...>;  \
    using Tuple::Tuple; \
    using Tuple::operator=; \
\
    APPLY_N(MAKE_TUPLE_FUNC, EXPAND(REVERSE(__VA_ARGS__)))   \
};


/// Auxiliary macro to create the free functions with the given @p NAME to call std::get<I>
#define GET_TUPLE_AT(NAME, I) \
template <class T, std::enable_if_t<handy::impl::HasGet<T>::value>* = nullptr> \
inline decltype(auto) NAME (T&& t)  \
{   \
    return std::get<I>(std::forward<T>(t));    \
}



/** @name
    @brief Generates the code for the named free functions that call std::get with a fixed argument
    @note Up to 10 functions. Copy-pase to create more if you want
*/
//@{
#define MAKE_GETTERS1(NAME, ...)  GET_TUPLE_AT(NAME, 0)
#define MAKE_GETTERS2(NAME, ...)  GET_TUPLE_AT(NAME, 1)  MAKE_GETTERS1(__VA_ARGS__)
#define MAKE_GETTERS3(NAME, ...)  GET_TUPLE_AT(NAME, 2)  MAKE_GETTERS2(__VA_ARGS__)
#define MAKE_GETTERS4(NAME, ...)  GET_TUPLE_AT(NAME, 3)  MAKE_GETTERS3(__VA_ARGS__)
#define MAKE_GETTERS5(NAME, ...)  GET_TUPLE_AT(NAME, 4)  MAKE_GETTERS4(__VA_ARGS__)
#define MAKE_GETTERS6(NAME, ...)  GET_TUPLE_AT(NAME, 5)  MAKE_GETTERS5(__VA_ARGS__)
#define MAKE_GETTERS7(NAME, ...)  GET_TUPLE_AT(NAME, 6)  MAKE_GETTERS6(__VA_ARGS__)
#define MAKE_GETTERS8(NAME, ...)  GET_TUPLE_AT(NAME, 7)  MAKE_GETTERS7(__VA_ARGS__)
#define MAKE_GETTERS9(NAME, ...)  GET_TUPLE_AT(NAME, 8)  MAKE_GETTERS8(__VA_ARGS__)
#define MAKE_GETTERS10(NAME, ...) GET_TUPLE_AT(NAME, 9)  MAKE_GETTERS9(__VA_ARGS__)
//@}


/// Generates functions with the given variadic names, each calling std::get with argument corresponding to its position
#define NAMED_GETTERS(...) APPLY_N(MAKE_GETTERS, EXPAND(REVERSE(__VA_ARGS__)))


//@}


namespace handy
{
namespace impl
{
    /// Checks if 'std::get' is callable for a given argument
    HAS_EXTERN_FUNC(::std::get<0>, HasGet)
}
}



#endif // HANDY_HELPERS_NAMED_TUPLE_H