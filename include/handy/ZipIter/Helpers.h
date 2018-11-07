/** 
 *  \file Helpers.h  
 *  \brief Some metaprogramming helpers
 */

#ifndef HANDY_ZIP_ITER_HELPERS_H
#define HANDY_ZIP_ITER_HELPERS_H

#include "../Helpers/Helpers.h"

#include <tuple>
#include <iterator>




/** @name
    
    @brief Utilities for calling handy::zipBegin() and handy::zipEnd() with variadic arguments
 
    When using stl functions which takes iterator parameters of the form (first, last), these macros 
    make it much easier. 
    
    Simply use ZIP_ALL(container) for a container class that is iterable (that is,has both std::begin 
    and std::end definitions or is a pointer).
   
    These macros simply call handy::impl::zip::begin and handy::impl::zip::end for each argument, delegating 
    the call to std::begin and std::end, while also supporting pointer types to be called.
*/
//@{
#define ZIP_BEGIN1(x, ...) ::handy::impl::zip::begin(x)
#define ZIP_BEGIN2(x, ...) ::handy::impl::zip::begin(x), ZIP_BEGIN1(__VA_ARGS__)
#define ZIP_BEGIN3(x, ...) ::handy::impl::zip::begin(x), ZIP_BEGIN2(__VA_ARGS__)
#define ZIP_BEGIN4(x, ...) ::handy::impl::zip::begin(x), ZIP_BEGIN3(__VA_ARGS__)
#define ZIP_BEGIN5(x, ...) ::handy::impl::zip::begin(x), ZIP_BEGIN4(__VA_ARGS__)
#define ZIP_BEGIN6(x, ...) ::handy::impl::zip::begin(x), ZIP_BEGIN5(__VA_ARGS__)
#define ZIP_BEGIN7(x, ...) ::handy::impl::zip::begin(x), ZIP_BEGIN6(__VA_ARGS__)
#define ZIP_BEGIN8(x, ...) ::handy::impl::zip::begin(x), ZIP_BEGIN7(__VA_ARGS__)

#define ZIP_BEGIN(...) APPLY_N(ZIP_BEGIN, __VA_ARGS__)


#define ZIP_END1(x, ...) ::handy::impl::zip::end(x)
#define ZIP_END2(x, ...) ::handy::impl::zip::end(x), ZIP_END1(__VA_ARGS__)
#define ZIP_END3(x, ...) ::handy::impl::zip::end(x), ZIP_END2(__VA_ARGS__)
#define ZIP_END4(x, ...) ::handy::impl::zip::end(x), ZIP_END3(__VA_ARGS__)
#define ZIP_END5(x, ...) ::handy::impl::zip::end(x), ZIP_END4(__VA_ARGS__)
#define ZIP_END6(x, ...) ::handy::impl::zip::end(x), ZIP_END5(__VA_ARGS__)
#define ZIP_END7(x, ...) ::handy::impl::zip::end(x), ZIP_END6(__VA_ARGS__)
#define ZIP_END8(x, ...) ::handy::impl::zip::end(x), ZIP_END7(__VA_ARGS__)

#define ZIP_END(...) APPLY_N(ZIP_END, __VA_ARGS__)


#define ZIP_ALL(...) EXPAND(::handy::zipIter(EXPAND(CONCAT(ZIP_BEGIN, NUM_ARGS(__VA_ARGS__)))(__VA_ARGS__)),  \
                            ::handy::zipIter(EXPAND(CONCAT(ZIP_END,   NUM_ARGS(__VA_ARGS__)))(__VA_ARGS__)))

//@}




namespace std
{
    /** This call converts a rvalue std::tuple to a lvalue and calls std::swap.
        
        This definition is necessary for some stl functions that use std::swap. 
        
        As long as I could see, there is no performance overhead
    */
    template <typename... Args>
    void swap (std::tuple<Args...>&& a, std::tuple<Args...>&& b) noexcept
    {
        swap(a, b);
    }
}



namespace handy
{
namespace impl
{
namespace zip
{



/// Simple trait for defining iterable types
template <typename T>                struct Iterable             { using iterator = typename T::iterator; };
template <typename T>                struct Iterable<const T>    { using iterator = typename T::const_iterator; };
template <typename T, std::size_t N> struct Iterable<T[N]>       { using iterator = T*; };
template <typename T, std::size_t N> struct Iterable<const T[N]> { using iterator = const T*; };
template <typename T>                struct Iterable<T*>         { using iterator = T*; };
template <typename T>                struct Iterable<const T*>   { using iterator = const T*; };



/// This is the order of the iterator types. The smaller is the more generic.
constexpr int iterTagOrder ( std::forward_iterator_tag )       { return 0; }
constexpr int iterTagOrder ( std::bidirectional_iterator_tag ) { return 1; }
constexpr int iterTagOrder ( std::random_access_iterator_tag ) { return 2; }



/// Selects the smallest (the more generic) iterator type from an variadic argument list
template <class...>   struct SelectIterTag;

template <class Iter> struct SelectIterTag < Iter > { using type = Iter; };

template <class Iter1, class Iter2>
struct SelectIterTag < Iter1, Iter2 >
{
    using type = std::conditional_t< (iterTagOrder(Iter1{}) < iterTagOrder(Iter2{})), Iter1, Iter2 >;
};

template <class Iter, class... Iters>
struct SelectIterTag < Iter, Iters... > : SelectIterTag< Iter, typename SelectIterTag< Iters... >::type > {};

template <class... Iter> using SelectIterTag_t = typename SelectIterTag< Iter... >::type;




/// Avoids some boilerplate in the definition of the 'ZipIter' class
template <typename... Iters>
using IteratorBase = std::iterator < impl::zip::SelectIterTag_t< typename std::iterator_traits< Iters >::iterator_category... >, 
                                     std::tuple< typename std::iterator_traits< Iters >::value_type... > >;



/// Only let functions to be called if the iterator tag satisfies a minimum tag
template <typename Tag, typename MinimumTag>
using EnableIfMinimumTag = std::enable_if_t< iterTagOrder( Tag{} ) >= iterTagOrder( MinimumTag {} ), int >; 








/// Simple functions to be used in the handy::applyTuple() function, defined as lambdas for simplicity.
auto increment = [](auto&& x) { return ++x; };

auto decrement = [](auto&& x) { return --x; };

auto add       = [](auto&& x, int inc) { return x = x + inc; };




/** The sole reason these functions were defined is to allow pointers 
  * to be called in 'zip' as if they were iterable types, having a starting and
  * ending points. As the first argument defines the range, there is no
  * problem in passing a pointer to be iterated, as long as its range
  * is smaller than the range of the first argument of the 'zip' function.
*/
template <typename T, std::enable_if_t< !std::is_pointer< std::decay_t<T> >::value, int > = 0>
decltype(auto) begin (T&& t) noexcept
{
    return std::begin(std::forward<T>(t)); /// Simply delegating the call for any other type
}

template <typename T, std::enable_if_t< std::is_pointer< std::decay_t<T> >::value, int > = 0>
decltype(auto) begin (T t) noexcept
{
    return t; /// The pointer itself is the starting point
}


template <typename T, std::enable_if_t< !std::is_pointer< std::decay_t<T> >::value, int > = 0>
decltype(auto) end (T&& t) noexcept
{
    return std::end(std::forward<T>(t)); /// Simply delegating the call for any other type
}

template <typename T, std::enable_if_t< std::is_pointer< std::decay_t<T> >::value, int > = 0>
decltype(auto) end (T t) noexcept
{
    return t + std::size_t(1e13); /// It is simply a unreachable point
}




/** This utility takes variadic arguments and packs them in a single tuple.
  * If any of the arguments itself is a tuple, it is concatenated (via std::tuple_cat)
  * with the rest of the arguments, always resulting in a single tuple.
*/
template <typename... ArgsTup>
decltype(auto) packArgs (std::tuple< ArgsTup... > tup)
{
    return std::move( tup );
}

template <typename T>
decltype(auto) packArgs (T&& t)
{
    return std::forward_as_tuple( std::forward< T >( t ) );
}

template <typename... ArgsTup, typename... Args>
decltype(auto) packArgs (std::tuple<ArgsTup...> tup, Args&&... args)
{
    return std::tuple_cat( std::move( tup ), std::move( packArgs(std::forward<Args>(args)...) ) );
}

template <typename T, typename... Args>
decltype(auto) packArgs (T&& t, Args&&... args)
{
    return std::tuple_cat( std::forward_as_tuple( std::forward< T >( t ) ),
                           std::move( packArgs(std::forward<Args>(args)...) ) );
}






/// Counts the number of arguments. If an argument is a tuple, sums its total size.
template <typename...> struct CountElements;

template <> struct CountElements <> : std::integral_constant<std::size_t, 0> { };

template <typename T, typename... Args>
struct CountElements<T, Args...> : std::integral_constant<std::size_t, CountElements<Args...>::value + 1> {};

template <typename... TupArgs, typename... Args>
struct CountElements<std::tuple<TupArgs...>, Args...> : std::integral_constant<std::size_t, sizeof...(TupArgs) + 
                                                                               CountElements< Args... >::value> {};


} // namespace zip

} // namespace impl

} // namespace handy


#endif // HANDY_ZIP_ITER_HELPERS_H