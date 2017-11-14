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


/// Count number of variadic arguments
#define NUM_ARGS_(_1, _2 ,_3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

/// This guy will call 'MACRON', where 'N' is the number of variadic arguments
#define APPLY_N(MACRO, ...) EXPAND(CONCAT(MACRO, NUM_ARGS(__VA_ARGS__)))(__VA_ARGS__)



namespace handy
{

template <typename> struct Empty {};


namespace impl
{

template <class T, class U = std::nullptr_t, template <typename> class Template = Empty>
class IsInherited
{
protected:

	template <typename W>
	static constexpr std::true_type isInherited (Template<W>);

	static constexpr std::true_type isInherited (std::decay_t<U>);
	
	static constexpr std::false_type isInherited (...);

public:
	
	enum{ value = decltype(isInherited(std::declval<std::decay_t<T>>()))::value };

};

}

template<class T, class U = std::nullptr_t>
using IsInherited = impl::IsInherited<T, U>;

template<class T, template <typename> class Template>
using IsInheritedTemplate = impl::IsInherited<T, std::nullptr_t, Template>;






/// Easy printing
template <typename T, typename... Args>
inline std::ostream& print (std::ostream& out, const T& t, const Args& ...args)
{
	out << t;

	auto dummie = { ((out << ' ' << args), 0)..., 0 };

	return out << '\n' << std::flush;
}

template <typename T, typename... Args,
		  typename std::enable_if<(!IsInherited<T, std::stringstream>::value &&
								   !IsInherited<T, std::ostream>::value)>::type* = nullptr>
inline std::ostream& print (const T& t, const Args& ...args)
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



template <typename T>
struct ToLvalue
{
	ToLvalue () {}
	ToLvalue (T t) : t(t) {}

	T t;
};





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


/// Verify if 'T' is a tuple
template <class T>
using IsTuple = IsSpecialization<std::decay_t<T>, std::tuple>;



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