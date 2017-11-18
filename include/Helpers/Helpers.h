/** @file
 * 	
 *  Almost every other file includes this one. It has many definitions that
 *  are used by other algorithms.
 * 
**/

#ifndef HANDY_HELPERS_HELPERS_H
#define HANDY_HELPERS_HELPERS_H

#include <type_traits>
#include <utility>

#include <assert.h>

#include <ostream>
#include <iostream>


#include <iterator>


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



namespace handy
{

/// Empty template base
template <typename> struct Empty {};


namespace impl
{

/** @defgroup InheritanceGroup Inheritance Check
	
	Utilities to check for inheritance
*/

/** @ingroup InheritanceGroup

	@brief Tells if @c T inherits from @c U, or from a template @c Template<W>.
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

  
/** @ingroup InheritanceGroup
    @brief Delegate the call to handy::impl::IsInherited with the class argument
*/
template<class T, class U = std::nullptr_t>
using IsInherited = impl::IsInherited<T, U>;

/** @ingroup InheritanceGroup
	@brief Delegate the call to handy::impl::IsInherited with the template argument
*/
template<class T, template <typename> class Template>
using IsInheritedTemplate = impl::IsInherited<T, std::nullptr_t, Template>;




/** @name
	@brief Easy printing
*/
//@{
/** @brief Prints a sequence of arguments separated by comma to the std::ostream reference @c out

	@param out A reference to a std::ostream object
	@param t The first template argument
	@param args Variadic number of arguments

	@return The reference @c out
*/
template <typename T, typename... Args>
inline std::ostream& print (std::ostream& out, const T& t, const Args& ...args)
{
	out << t;

	auto dummie = { ((out << ' ' << args), 0)..., 0 };

	return out << '\n' << std::flush;
}

/** @brief Delegate the call to handy#print() with std::cout as the std::ostream argument.
    
	@note Only allowed if the first type @c T does not inherits (or is) from a 
		  std::stringstream or from a std::ostream classes
*/
template <typename T, typename... Args,
		  typename std::enable_if<(!IsInherited<T, std::stringstream>::value &&
								   !IsInherited<T, std::ostream>::value)>::type* = nullptr>
inline std::ostream& print (const T& t, const Args& ...args)
{
	return print(std::cout, t, args...);
}
//@}



/** @defgroup GetArgGroup Type Picker
	@brief Utilities for picking a type given a variadic types
	@{
*/
/// Base definition
template <std::size_t, typename...>
struct GetArg;

/// If @c I is not 0, take the type of handy::GetArg<I-1, Args...>
template <std::size_t I, typename T, typename... Args>
struct GetArg<I, T, Args...>
{
	using type = typename GetArg<I-1, Args...>::type;
};

/// If @c I is 0, take type @c T
template <typename T, typename... Args>
struct GetArg<0, T, Args...>
{
	using type = T;
};

/// If the initial @c I is greater than the number of arguments or is negative, throw an error
template <std::size_t I>
struct GetArg<I>
{
	static_assert((I & !I), "Position specified exceeds the maximum number of variadic arguments.");
};

/// Helper alias
template <std::size_t I, typename... Args>
using GetArg_t = typename GetArg<I, Args...>::type;
/** @} */



/** @defgroup AndGroup Variadic And
	@brief Perform the @c and operator on a variadic number of bool constants
	@{
*/
/// Base definition
template <bool...>				struct And;

/// If @c B1 is true, check for the rest
template <bool B1, bool... Bs> struct And<B1, Bs...> : And<Bs...> {};

/// If the current argument (@c B1) is @c false, so the return can only be @c false
template <bool... Bs> 		   struct And<false, Bs...> : std::false_ty

/// If none of the arguments are @c false, so the result is @c true
template <> 				   struct And<> : std::true_type {};

/// Helper alias
template <bool... Bs>
constexpr bool And_v = And< Bs... >::value;
/// @}



/** @defgroup IsSpecializationGroup Specialization check
    @brief Check for template specialization
	@note Taken from https://bitbucket.org/martinhofernandes/wheels/src/default/include/wheels/meta/type_traits.h%2B%2B?fileviewer=file-view-default#cl-161
	@{
*/
/// If class @c T is not a specialization of template @c Template, inherit from std::false_type
template <typename T, template <typename...> class Template>
struct IsSpecialization : std::false_type {};

/// If class @c T is a specialization of template @c Template, inherit from std::true_type
template <template <typename...> class Template, typename... Args>
struct IsSpecialization<Template<Args...>, Template> : std::true_type {};
//@}


/// Verify if type @c T is a tuple using handy::IsSpecialization
template <class T>
using IsTuple = IsSpecialization<std::decay_t<T>, std::tuple>;


/** @name
    @brief Apply a function to every element of a tuple
*/
//@{
/** @param apply The function to be applied to tuple @c tup
	@param tup A reference to a std::tuple
	@param funcArgs Arguments to the @c apply function
	@brief Apply a function to every element of a tuple, passing "funcArgs" as argument. In C++17 it is a lot easier.
*/
template <class Apply, typename... Args, std::size_t... Is, typename... FuncArgs>
void applyTuple (Apply apply, std::tuple<Args...>& tup, std::index_sequence<Is...>, FuncArgs&&... funcArgs)
{
    const auto& dummie = { ( apply( std::get<Is>(tup), std::forward<FuncArgs>(funcArgs)... ), int{} ) ... };
}

/** @copydetails applyTuple
	@brief Delegate the call to applyTuple() generating a @link std::integer_sequence std::index_sequence @endlink
		   to expand the std::tuple
*/
template <class Apply, typename... Args, typename... FuncArgs>
void applyTuple (Apply apply, std::tuple<Args...>& tup, FuncArgs&&... funcArgs)
{
    return applyTuple(apply, tup, std::make_index_sequence<sizeof...(Args)>(), std::forward<FuncArgs>(funcArgs)...);
}
//@}

} // namespace handy


#endif // HANDY_HELPERS_HELPERS_H