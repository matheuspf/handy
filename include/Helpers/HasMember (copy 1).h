/** \file HasMember.h
  *
  * Very simple C++11 helpers to verify at compile time if a class has
  * a variable or a function. Very useful for creating SFINAE friendly
  * classes or functions
*/

#ifndef HAS_MEMBER_H
#define HAS_MEMBER_H

#include <type_traits>


/// C++11 does not have these definitions
// namespace std
// {
// 	template <typename T>
// 	using decay_t = typename std::decay<T>::type;

// 	template <bool B, typename T, typename U>
// 	using conditional_t = typename std::conditional<B, T, U>::type;
// }


/// Expands variadic arguments
#define EXPAND(...) __VA_ARGS__

/// Concatenate two tokens
#define CONCAT(x, y) CONCAT_(x, y)
#define CONCAT_(x, y) EXPAND(x ## y)

/// Helper for variables
#define VAR_HELPER(T, var) std::declval<T>().var

/// Helper for functions
#define FUNC_HELPER(T, func) &T::func


#define OVERLOADED_FUNC_HELPER(...) static_cast<__VA_ARGS__> FUNC_HELPER

//ReturnT(ClassT::*)(ArgTs);


/// Extern function helper
#define EXTERN_FUNC_HELPER(T, ext) ext(std::declval<T>()) 


/// If the class has a variable
#define HAS_VAR(...) EXPAND(HAS_MEMBER(VAR_HELPER, __VA_ARGS__, CONCAT(has_, __VA_ARGS__)))

/// If the class has a function
#define HAS_FUNC(...) EXPAND(HAS_MEMBER(FUNC_HELPER, __VA_ARGS__, CONCAT(has_, __VA_ARGS__)))

/// Extern function
#define HAS_EXTERN_FUNC(...) EXPAND(HAS_MEMBER(EXTERN_FUNC_HELPER, __VA_ARGS__, CONCAT(has_, __VA_ARGS__)))

/// Overloaded function
#define HAS_OVERLOADED_FUNC(...) EXPAND(HAS_MEMBER(OVERLOADED_FUNC_HELPER, __VA_ARGS__, CONCAT(has_, __VA_ARGS__)))


/** Here we create functions returning compile time values that tells us
  * if a class has or not a member of the given name. See the examples below.
*/
#define HAS_MEMBER(HELPER, member, Name, ...) \
\
template <class> \
constexpr bool CONCAT(Name, Impl) (...) { return false; } \
\
template <class T> \
constexpr bool CONCAT(Name, Impl) (std::decay_t<decltype(EXPAND(HELPER(T, member)), void())>*) { return true; } \
\
template <class T>  \
struct Name : public std::integral_constant<bool, CONCAT(Name, Impl) <std::decay_t<T>>(nullptr)> {};

// template <class T> \
// constexpr bool Name () { return CONCAT(Name, Impl) <std::decay_t<T>>(nullptr); }


#endif //HAS_MEMBER_H