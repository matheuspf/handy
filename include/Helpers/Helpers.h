/** \file Helpers.h
 * 	
 *  Almost every other file includes this one. It has many definitions that
 *  are used by other algorithms.
 * 
**/

#ifndef CPPL_HELPERS_H
#define CPPL_HELPERS_H

#include <type_traits>
#include <utility>

#include <assert.h>

#include <ostream>
#include <iostream>

#include <iterator>


/// Expands variadic arguments
#define EXPAND(...) __VA_ARGS__

/// Concatenate two tokens
#define CONCAT(x, y) CONCAT_(x, y)
#define CONCAT_(x, y) EXPAND(x ## y)



namespace handy
{

template <class T, class U>
class IsInherited
{
private:

	static constexpr std::true_type isInherited (const std::decay_t<U>&);
	
	static constexpr std::false_type isInherited (...);

public:
	
	enum{ value = decltype(isInherited(std::declval<std::decay_t<T>>()))::value };

};



/// Easy printing
std::ostream& print (std::ostream& out)
{
	out << '\n' << std::flush;

	return out;
}

template <typename T, typename... Args>
std::ostream& print (std::ostream& out, const T& t, const Args& ...args)
{
	out << t;

	if(sizeof...(Args))
		out << ' ';

	return print(out, args...);
}

template <typename T, typename... Args,
		  std::enable_if_t<(!IsInherited<T, std::stringstream>::value &&
							!IsInherited<T, std::ostream>::value), int> = 0>
std::ostream& print (const T& t, const Args& ...args)
{
	return print(std::cout, t, args...);
}





/// Get argument type at position 'I'
template <std::size_t, typename...>
struct GetArg;

template <std::size_t I, typename T, typename... Args>
struct GetArg<I, T, Args...>
{
	using type = typename GetArg<I-1, Args...>::type;
};

template <typename T, typename... Args>
struct GetArg<0, T, Args...>
{
	using type = T;
};

template <std::size_t I>
struct GetArg<I>
{
	static_assert((I & !I), "Position specified exceeds the maximum number of variadic arguments.");
};


/// Helper
template <std::size_t I, typename... Args>
using GetArg_t = typename GetArg<I, Args...>::type;





/// Because 'operator <<' supports only two arguments, we have to pass a pair of iterators
template <class Iterator>
std::ostream& operator << (std::ostream& out, const std::pair<Iterator, Iterator>& range)
{
	std::copy(range.first, range.second, std::ostream_iterator<Iterator>());

	return out;
}



/// A compile time 'and' for variadic bool.
template <bool...>				struct And;

template <bool B1, bool... Bs> struct And< B1, Bs... > : And< Bs... > {};

template <bool... Bs> 		   struct And< false, Bs... > : std::false_type {};

template <> 				   struct And<true> : std::true_type {};

template <> 				   struct And<> : std::true_type {};

/// Helper
template <bool... Bs>
constexpr bool And_v = And< Bs... >::value;





/// Taken from https://bitbucket.org/martinhofernandes/wheels/src/default/include/wheels/meta/type_traits.h%2B%2B?fileviewer=file-view-default#cl-161
template <typename T, template <typename...> class Template>
struct IsSpecialization : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct IsSpecialization<Template<Args...>, Template> : std::true_type {};



// /// Simple way to test if a class is a container -> check if 'std::begin' and 'std::end' are defined
// HAS_EXTERN_FUNC(std::begin, HasBegin)
// HAS_EXTERN_FUNC(std::end, HasEnd)

// template <class T>
// struct IsContainer : std::numerical_constant<bool, HasBegin<T>::value && HasEnd<T>::value> {};

// template <class T>
// constexpr bool isContainer_v = IsContainer<T>::value;



/// Apply a function to every element of a tuple, passing "funcArgs" as argument. In C++17 it is a lot easier.
template <class Apply, typename... Args, std::size_t... Is, typename... FuncArgs>
void applyTuple (Apply apply, std::tuple<Args...>& tup, std::index_sequence<Is...>, FuncArgs&&... funcArgs)
{
    const auto& dummie = { ( apply( std::get<Is>(tup), std::forward<FuncArgs>(funcArgs)... ), int{} ) ... };
}

template <class Apply, typename... Args, typename... FuncArgs>
void applyTuple (Apply apply, std::tuple<Args...>& tup, FuncArgs&&... funcArgs)
{
    return applyTuple(apply, tup, std::make_index_sequence<sizeof...(Args)>(), std::forward<FuncArgs>(funcArgs)...);
}


} // namespace handy


#endif // CPPL_HELPERS_H