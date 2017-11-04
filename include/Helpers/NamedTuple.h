/** \file NamedTuple.h
 * 
 *  Ridiculously simple macros for generating functions that basically call
 *  'std::get' with a defined constant position.
 *
**/

#ifndef CPPL_NAMED_TUPLE_H
#define CPPL_NAMED_TUPLE_H

#include "Helpers.h"
#include "HasMember.h"
#include <tuple>


/// 12 lines just to reverse the variadic arguments. I could not find another way to do that =/
#define REVERSE1(X, ...)  X
#define REVERSE2(X, ...)  REVERSE1(__VA_ARGS__), X
#define REVERSE3(X, ...)  REVERSE2(__VA_ARGS__), X
#define REVERSE4(X, ...)  REVERSE3(__VA_ARGS__), X
#define REVERSE5(X, ...)  REVERSE4(__VA_ARGS__), X
#define REVERSE6(X, ...)  REVERSE5(__VA_ARGS__), X
#define REVERSE7(X, ...)  REVERSE6(__VA_ARGS__), X
#define REVERSE8(X, ...)  REVERSE7(__VA_ARGS__), X
#define REVERSE9(X, ...)  REVERSE8(__VA_ARGS__), X
#define REVERSE10(X, ...) REVERSE9(__VA_ARGS__), X

#define REVERSE(...) APPLY_N(REVERSE, __VA_ARGS__)



/// Auxiliary macro to call 'std::get<I>' when you call the 'NAME' function
#define MAKE_TUPLE_FUNC_AT(NAME, I) \
decltype(auto) NAME ()  \
{   \
    return std::get<I>(static_cast<Tuple&>(*this)); \
}


/// Up to 10 functions. Easy to create more if you want
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



/** This class inherits from 'std::tuple' and adds the functions that you pass to
 *  call 'std::get' in the order of the parameters. Of course, you will not play with
 *  pointers to this guy because that can cause leaks.
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



/// Auxiliary macro to create the functions with the given 'NAME' to call 'std::get<I>'
#define GET_TUPLE_AT(NAME, I) \
template <class T, std::enable_if_t<handy::impl::HasGet<T>::value>* = nullptr> \
inline decltype(auto) NAME (T&& t)  \
{   \
    return std::get<I>(std::forward<T>(t));    \
}


/// Up to 10 functions. Easy to create more if you want
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


#define NAMED_GETTERS(...) APPLY_N(MAKE_GETTERS, EXPAND(REVERSE(__VA_ARGS__)))




namespace handy
{
namespace impl
{
    /// Checks if 'std::get' is callable for a given argument
    HAS_EXTERN_FUNC(::std::get<0>, HasGet)
}
}




//#undef GET_TUPLE_AT
//#undef MAKE_TUPLE_FUNC_AT



#endif // CPPL_NAMED_TUPLE_H