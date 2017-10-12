/** \file Helpers.h
  *
  * Some metaprogramming helpers
*/

#ifndef CNT_VECTOR_HELPERS_H
#define CNT_VECTOR_HELPERS_H

#include <type_traits>
#include <vector>
#include <array>
#include <initializer_list>



namespace std
{
	/** These are not available in C++14 */
	//@{
	template <typename T, typename U>
	constexpr bool is_same_v = std::is_same< T, U >::value;

	template <typename T>
	constexpr bool is_integral_v = std::is_integral<T>::value;
	//@}
}




namespace cnt
{

namespace help
{

/// A single stack allocation must not exceed this limit
constexpr std::size_t maxSize = 100000 * sizeof(char);


//---------------------------------------------------------------------------




/** Multiply the integers to get the total size at compile time. If no argument
  * is give, the function returns 0
  *
  * \param[in] Is The variadic arguments
  * \return The total size of multiplying Is or 0, if sizeof...(Is) == 0
*/
//@{
// template <std::size_t I, std::size_t... Is>
// constexpr inline std::size_t multiply ()
// {
// 	std::size_t res = I;

// 	const auto& dummy = { (res *= Is)... };

// 	return res;
// }

// constexpr inline std::size_t multiply ()
// {
// 	return 0;
// }


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



//-----------------------------------------------------------------------------------


/** Test truthness of variadic set of bool arguments. Only gives a true value
  * if all elements are true.
*/
//@{
template <bool...>
struct And;

template <bool B1, bool... Bs>
struct And< B1, Bs... > : And< Bs... > {};

template <bool... Bs>
struct And< false, Bs... > : std::false_type {};

template <>
struct And<true> : std::true_type {};

template <>
struct And<> : std::true_type {};

template <bool... Bs>
constexpr bool And_v = And< Bs... >::value;
//@}



/** If the compile time size is greater than 0 and less than the defined maxSize, 
  * the Vector is a std::array. Otherwise it is a std::vector
*/
//@{
template <std::size_t N>
constexpr bool isArray = ((N > 0) && (N < maxSize)) ? true : false;

template <std::size_t N>
constexpr bool isVector = !isArray< N >;
//@}


/// Selects either a 'std::array<T, N>' or a 'std::vector<T>' depending on the size 'N'
template <typename T, std::size_t N>
using SelectType = std::conditional_t<help::isArray<N>, std::array<T, N>, std::vector<T>>;




/** Tells us if 'T' is an iterable type (has definition for 'std::begin') of integrals */
//@{
template <class>
constexpr bool isIterableImpl (...) { return false; }

template <class T>
constexpr bool isIterableImpl (decltype(std::is_integral_v<decltype(*std::begin(std::declval<T>()))>, int{})*)
{
	return true;
}

template <class T>
constexpr bool isIterable () { return isIterableImpl<T>(nullptr); }
//@}



/** Tells us if 'T' is an iterator of integrals */
//@{
template <class>
constexpr bool isIteratorImpl (...) { return false; }

template <class T>
constexpr bool isIteratorImpl (decltype(std::is_integral_v<typename std::iterator_traits<T>::value_type>, int{})*)
{
	return true;
}

template <class T>
constexpr bool isIterator () { return isIteratorImpl<T>(nullptr); }
//@}




/** Some useful helpers for SFINAE */
//@{

/// Enable if 'N' satisfy 'isArray' trait
template <std::size_t N>
using EnableIfArray = std::enable_if_t< isArray< N >, int >;

/// Enable if 'N' satisfy 'isVector' trait
template <std::size_t N>
using EnableIfVector = std::enable_if_t< isVector< N >, int >;

/// Enable if 'N' is not '0' trait
template <std::size_t N>
using EnableIfZero = std::enable_if_t< (N == 0), int >;


/// Enable if all 'Args' are integrals
template <typename... Args>
using EnableIfIntegral = std::enable_if_t<And_v<std::is_integral_v<Args>...>, int>; 


/// Enable if all 'Args' are iterables of integrals
template <typename... Args>
using EnableIfIterable = std::enable_if_t<And_v<isIterable<Args>()...>, int>;

/// Enable if all 'Args' are iterators to integrals
template <typename... Args>
using EnableIfIterator = std::enable_if_t<And_v<isIterator<Args>()...>, int>;

/// Enable if all 'Args' are either integrals or iterable types of integrals
template <typename... Args>
using EnableIfIntegralOrIterable = std::enable_if_t<And_v<(std::is_integral_v<Args> || isIterable<Args>())...>, int>;




/** These are dummy classes that help to create functions to treat the type of parameters 
  * of the accessors: integrals, iterables or iterators.
*/
//@{
struct IntegralType {};

struct IterableType {};

struct IteratorType {};
//@}


}   // namespace help

}


#endif // CNT_VECTOR_HELPERS_H
