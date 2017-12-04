/** @file
 
 	Some metaprogramming helpers for the handy::Container utility
*/

#ifndef HANDY_CONTAINER_HELPERS_H
#define HANDY_CONTAINER_HELPERS_H


#include "../Helpers/Helpers.h"
#include "../Helpers/HasMember.h"

#include <vector>
#include <array>
#include <initializer_list>




namespace handy
{

namespace impl
{

namespace cnt
{

/// A single stack allocation must not exceed this limit
constexpr std::size_t maxSize = 100000 * sizeof(char);


//---------------------------------------------------------------------------

/*
 


/** @name 
 *  
 * Multiply the integers to get the total size at compile time (returns 0 if the number of arguments is 0) If no argument
 * is given, the function returns 0
*/
//@{
template <std::size_t...>
struct multiply;

template <std::size_t I, std::size_t... Is>
struct multiply<I, Is...> : std::integral_constant<std::size_t, sizeof...(Is) ? 
												   I * multiply<Is...>::value : I> {};

template <>
struct multiply<> : std::integral_constant<std::size_t, 0> {};


template<std::size_t... Is>
constexpr std::size_t multiply_v = multiply<Is...>::value;
//@}





/** @name 
 *  @brief If the compile time size is greater than 0 and less than the defined maxSize, 
 *  the Vector is a std::array. Otherwise it is a std::vector
*/
//@{
template <std::size_t N>
constexpr bool isArray = ((N > 0) && (N < maxSize)) ? true : false;

template <std::size_t N>
constexpr bool isVector = !isArray< N >;
//@}


/// Selects either a std::array or a std::vector depending on the size @c N
template <typename T, std::size_t N>
using SelectType = std::conditional_t<isArray<N>, std::array<T, N>, std::vector<T>>;




HAS_EXTERN_FUNC(::std::begin, HasBegin)
HAS_EXTERN_FUNC(::std::end, HasEnd)

/// Tells if a type @p T is a container - that is, has a specialization for both std::begin and std::end
template <typename T>
struct IsIterable : std::integral_constant<bool, HasBegin<T>::value && HasEnd<T>::value> {};



HAS_VAR(value_type, HasValue)

/// Tells us if 'T' is an iterator of integrals
template <typename T>
struct IsIterator : std::integral_constant<bool, (HasValue<T>::value || std::is_pointer<std::decay_t<T>>::value)> {};




/** Some useful helpers for SFINAE */
//@{

/// Enable if @p N satisfies handy::isArray trait
template <std::size_t N>
using EnableIfArray = std::enable_if_t< isArray< N >, int >;

/// Enable if @p N satisfies handy::isVector trait
template <std::size_t N>
using EnableIfVector = std::enable_if_t< isVector< N >, int >;

/// Enable if @p N is not @c 0 trait
template <std::size_t N>
using EnableIfZero = std::enable_if_t< (N == 0), int >;


/// Enable if all @p Args are integrals
template <typename... Args>
using EnableIfIntegral = std::enable_if_t<And_v<std::is_integral_v<Args>...>, int>; 


/// Enable if all @p Args are iterables of integrals
template <typename... Args>
using EnableIfIterable = std::enable_if_t<And_v<IsIterable<Args>::value...>, int>;

/// Enable if all @p Args are iterators to integrals
template <typename... Args>
using EnableIfIterator = std::enable_if_t<And_v<IsIterator<Args>::value...>, int>;

/// Enable if all @p Args are either integrals or iterable types of integrals
template <typename... Args>
using EnableIfIntegralOrIterable = std::enable_if_t<And_v<(std::is_integral_v<Args> || IsIterable<Args>::value)...>, int>;




/** @name 
	@brief These are dummy classes that help to create functions to treat the type of parameters 
 		   of the accessors: integrals, iterables or iterators.
*/
//@{
struct IntegralType {};

struct IterableType {};

struct IteratorType {};
//@}

} // namespace cnt

} // namespace impl

} // namespace handy


#endif // HANDY_CONTAINER_HELPERS_H
