/** \file HasMember.h
  *
  * Very simple C++11 helpers to verify at compile time if a class has
  * a variable or a function. Very useful for creating SFINAE friendly
  * classes or functions
*/

#ifndef HAS_MEMBER_H
#define HAS_MEMBER_H

#include "Helpers.h"



namespace handy
{

/// Helper for variables
#define VAR_HELPER(HM_T, var, ...) std::declval<HM_T>().var

/// Helper for functions
#define FUNC_HELPER(HM_T, func, ...) &HM_T::func


/// Helper for overloaded functions
#define OVERLOADED_FUNC_HELPER(HM_T, func, Args, ...) std::declval<HM_T>().func(std::declval<HM_Args>()...)

/// Extern function helper
#define EXTERN_FUNC_HELPER(HM_T, ext, Args, ...) ext(std::declval<HM_T>(), std::declval<HM_Args>()...)



/// If the class has a variable
#define HAS_VAR_IMPL(STATIC, ...) EXPAND(HAS_MEMBER(VAR_HELPER, STATIC, __VA_ARGS__, CONCAT(Has, __VA_ARGS__)))

/// If the class has a function
#define HAS_FUNC_IMPL(STATIC, ...) EXPAND(HAS_MEMBER(FUNC_HELPER, STATIC, __VA_ARGS__, CONCAT(Has, __VA_ARGS__)))

/// Extern function
#define HAS_EXTERN_FUNC_IMPL(STATIC, ...) EXPAND(HAS_MEMBER(EXTERN_FUNC_HELPER, STATIC, __VA_ARGS__, CONCAT(Has, __VA_ARGS__)))

/// Overloaded function. WARNING: it does not verify explicit overloads
#define HAS_OVERLOADED_FUNC_IMPL(STATIC, ...) EXPAND(HAS_MEMBER(OVERLOADED_FUNC_HELPER, STATIC, __VA_ARGS__, CONCAT(has_, __VA_ARGS__)))

// /// Overloaded function
// #define HAS_STRICTLY_OVERLOADED_FUNC(...) EXPAND(HAS_MEMBER(STRICTLY_OVERLOADED_FUNC_HELPER, __VA_ARGS__, CONCAT(has_, __VA_ARGS__)))




/// These delegate the call to the 'IMPL' version with 'STATIC' set to nothing (call outside a function)
#define HAS_VAR(...) HAS_VAR_IMPL(, __VA_ARGS__)

#define HAS_FUNC(...) HAS_FUNC_IMPL(, __VA_ARGS__)

#define HAS_EXTERN_FUNC(...) HAS_EXTERN_FUNC_IMPL(, __VA_ARGS__)

#define HAS_OVERLOADED_FUNC(...) HAS_OVERLOADED_FUNC_IMPL(, __VA_ARGS__)


/// These delegate the call to the 'IMPL' version with 'STATIC' set to 'static' (call inside a function)
#define HAS_VAR_STATIC(...) HAS_VAR_IMPL(static, __VA_ARGS__)

#define HAS_FUNC_STATIC(...) HAS_FUNC_IMPL(static, __VA_ARGS__)

#define HAS_EXTERN_FUNC_STATIC(...) HAS_EXTERN_FUNC_IMPL(static, __VA_ARGS__)

#define HAS_OVERLOADED_FUNC_STATIC(...) HAS_OVERLOADED_FUNC_IMPL(static, __VA_ARGS__)



/** Here we create functions returning compile time values that tells us
  * if a class has or not a member of the given name. See the examples below.
  *
  * \note I am using 'HM_T' and 'HM_Args' to avoid any naming conflict if you 
  *       call this guy inside a class, for example.
*/
#define HAS_MEMBER(HELPER, STATIC, member, Name, ...) \
\
template <typename...> \
STATIC constexpr bool CONCAT(Name, Impl) (...) { return false; } \
\
template <typename HM_T, typename... HM_Args> \
STATIC constexpr bool CONCAT(Name, Impl) (std::decay_t<decltype(EXPAND(HELPER(HM_T, member, HM_Args)), void())>*) { return true; } \
\
template <typename HM_T, typename... HM_Args>  \
struct Name : public std::integral_constant<bool, CONCAT(Name, Impl) <std::decay_t<HM_T>, HM_Args...>(nullptr)> {};

} // namespace handy


#endif //HAS_MEMBER_H