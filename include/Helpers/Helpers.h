/** @file
 * 	
 *  @brief Almost every other file includes this one. It has many definitions that
 *  	   are used by other algorithms.
 * 
**/

#ifndef HANDY_HELPERS_HELPERS_H
#define HANDY_HELPERS_HELPERS_H

#include <type_traits>
#include <utility>
#include <tuple>

#include <assert.h>


/// Expands variadic arguments
#define EXPAND(...) __VA_ARGS__


//@{
/// Concatenate two tokens
#define CONCAT(x, y) CONCAT_(x, y)

/// @copybrief CONCAT
#define CONCAT_(x, y) EXPAND(x ## y)
//@}



//@{
/** @brief Count number of variadic arguments
*/
#define NUM_ARGS_(_1, _2 ,_3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

/// @copybrief NUM_ARGS_
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
//@}


/// This guy will call MACRON, where @c N is the number of variadic arguments
#define APPLY_N(MACRO, ...) EXPAND(CONCAT(MACRO, NUM_ARGS(__VA_ARGS__)))(__VA_ARGS__)


/** @name
    @brief Reverse the order of variadic macro arguments
    @note 12 lines just to reverse the variadic arguments. I could not find another way to do that =/
*/
//@{
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
//@}



namespace handy
{

/// Empty template base
template <typename> struct Empty {};


namespace impl
{
/** @brief Tells if @c T inherits from @c U, or from a template @c Template<W>.
		   In the case where @c T is the same as @c U, @link IsInherited::value value @endlink is @c true

	@tparam T Derived class to check
	@tparam U Possible base class
	@tparam Template Possible Template base class
*/
template <class T, class U = std::nullptr_t, template <typename> class Template = Empty>
struct IsInherited
{
	/// Returns @c true if @c T inherits from the template @c Template<W>
	template <typename W>
	static constexpr std::true_type isInherited (Template<W>);

	/// Returns @c true if @c T inherits from @c U
	static constexpr std::true_type isInherited (const std::decay_t<U>&);
	
	/// Returns @c false otherwise
	static constexpr std::false_type isInherited (...);

	enum
	{
		/// Saves the value returned by isInherited()
		value = decltype(isInherited(std::declval<std::decay_t<T>>()))::value
	};
};

} // namespace impl

  
/// Delegate the call to handy::impl::IsInherited with the class argument
template<class T, class U = std::nullptr_t>
using IsInherited = impl::IsInherited<T, U>;

/// Delegate the call to handy::impl::IsInherited with the template argument
template<class T, template <typename> class Template>
using IsInheritedTemplate = impl::IsInherited<T, std::nullptr_t, Template>;




/** @brief Utilities for picking a type given variadic types
	
	@tparam I Position to pick the argument you want
	@tparam Args Variadic arguments to pick
*/
template <std::size_t I, typename... Args>
struct GetArg;

/** @copydoc GetArg
	@details If @c I is not 0, take the type of @link handy::GetArg<I-1, T, Args...> handy::GetArg<I-1, Args...> @endlink
*/
template <std::size_t I, typename T, typename... Args>
struct GetArg<I, T, Args...>
{
	using type = typename GetArg<I-1, Args...>::type;
};

/** @copydoc GetArg
	@details If @p I is 0, take type @p T
*/
template <typename T, typename... Args>
struct GetArg<0, T, Args...>
{
	using type = T;
};

/** @copydoc GetArg
	@details If the initial @p I is greater than the number of arguments or is negative, throw an error
*/
template <std::size_t I>
struct GetArg<I>
{
	static_assert((I & !I), "Position specified exceeds the maximum number of variadic arguments.");
};

/** @copybrief GetArg
	@details Alias for handy::GetArg
*/
template <std::size_t I, typename... Args>
using GetArg_t = typename GetArg<I, Args...>::type;



/**
	@brief Perform the @c and operator on a variadic number of bool constants
	@details Base definition
*/
template <bool...>				struct And;

/** @copybrief handy::And
	@details If @c B1 is true, check for the rest
*/
template <bool B1, bool... Bs> struct And<B1, Bs...> : And<Bs...> {};

/** @copybrief handy::And
	@details If the current argument (@c B1) is @c false, so the return can only be @c false
*/
template <bool... Bs> 		   struct And<false, Bs...> : std::false_type {};

/** @copybrief handy::And
	@details If none of the arguments are @c false, so the result is @c true
*/
template <> 				   struct And<> : std::true_type {};

/** @copybrief handy::And
	@details Alias for handy::And
*/
template <bool... Bs>
constexpr bool And_v = And< Bs... >::value;



/** @class IsSpecializationDoc
 	@brief Check for template specialization
	@tparam T Type to check
	@tparam Template Template type to check
	@note Taken from https://bitbucket.org/martinhofernandes/wheels/src/default/include/wheels/meta/type_traits.h%2B%2B?fileviewer=file-view-default#cl-161
*/

/** @copydoc IsSpecializationDoc
	@details If class @c T is not a specialization of template @c Template, inherit from std::false_type
**/
template <typename T, template <typename...> class Template>
struct IsSpecialization : std::false_type {};

/** @copydoc IsSpecializationDoc
	@details If class @c T is a specialization of template @c Template, inherit from std::true_type
**/
template <template <typename...> class Template, typename... Args>
struct IsSpecialization<Template<Args...>, Template> : std::true_type {};



/// Verify if type @c T is a tuple using handy::IsSpecialization
template <class T>
using IsTuple = IsSpecialization<std::decay_t<T>, std::tuple>;




/** @brief Apply a function to every element of a tuple
	
 	@param apply The function to be applied to tuple @c tup
	@param tup A reference to a std::tuple
	@param funcArgs Arguments to the @c apply function
	@brief Apply a function to every element of a tuple, passing "funcArgs" as argument. In C++17 it is a lot easier.
*/
template <class Apply, typename... Args, std::size_t... Is, typename... FuncArgs>
void applyTuple (Apply apply, std::tuple<Args...>& tup, std::index_sequence<Is...>, FuncArgs&&... funcArgs)
{
    const auto& dummie = { ( apply( std::get<Is>(tup), std::forward<FuncArgs>(funcArgs)... ), int{} ) ... };
}

/** @brief Delegate the call to applyTuple() generating a @link std::integer_sequence std::index_sequence @endlink
		   to expand the std::tuple
	@copydetails applyTuple
*/
template <class Apply, typename... Args, typename... FuncArgs>
void applyTuple (Apply apply, std::tuple<Args...>& tup, FuncArgs&&... funcArgs)
{
    return applyTuple(apply, tup, std::make_index_sequence<sizeof...(Args)>(), std::forward<FuncArgs>(funcArgs)...);
}



/** @copydoc reverseArgs()
	
	@tparam Is Index arguments -- 0, 1, ..., P
	@tparam Js Index arguments -- 0, 1, ..., sizeof(Args) - P
*/
template <std::size_t P, class Apply, class... Args, std::size_t... Is, std::size_t... Js>
decltype(auto) reverseArgs (Apply apply, std::tuple<Args...>&& tup, std::index_sequence<Is...>, std::index_sequence<Js...>)
{
    return apply(std::get<Js+P>(tup)..., std::get<Is>(tup)...);
}

/** @brief Reverses the order of variadic arguments given an index
 	
	@tparam P The starting index to reverse
	@param apply The function to apply after the reversing
	@param tup The std::tuple that holds the arguments

	Given an index P, this function reverses the order of the arguments from @f$ [0, 1, ..., P, ..., N] @f$ to
	@f$ [P, ..., N, 0, 1, ..., P-1] @f$
	
	After that, a function is applied to every element in the new order.
*/
template <std::size_t P, class Apply, class... Args>
decltype(auto) reverseArgs (Apply apply, Args&&... args)
{
    return reverseArgs<P>(apply, std::forward_as_tuple(std::forward<Args>(args)...), 
                          	 	 std::make_index_sequence<P>{}, 
                          		 std::make_index_sequence<sizeof...(Args)-P>{});
}



} // namespace handy


namespace std
{
	/** @name
	 	@brief Some aliases for C++14
	*/
	//@{
	template <typename T, typename U>
	constexpr bool is_same_v = std::is_same< T, U >::value;

	template <typename T>
	constexpr bool is_integral_v = std::is_integral<T>::value;
	//@}
}	// namespace std




#endif // HANDY_HELPERS_HELPERS_H