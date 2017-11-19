/** @file

    @brief Very simple C++11 helpers to verify at compile time if a class has a variable or a function. 

    @details Given a call to HAS_VAR(), HAS_FUNC(), HAS_EXTERN_FUNC() or HAS_OVERLOADED_FUNC(), two functions 
             and one class are generated. By using SFINAE, we can determine if a given class has or has not a 
             member variable or function at compile time. Ex:

    @snippet Helpers/HasMemberExample.cpp HasMember snippet

    @note Very useful for creating <a href="http://en.cppreference.com/w/cpp/language/sfinae">SFINAE</a>
          friendly classes or functions
*/

#ifndef HANDY_HELPERS_HAS_MEMBER_H
#define HANDY_HELPERS_HAS_MEMBER_H

#include "Helpers.h"



namespace handy
{

/** @defgroup HasMemberGroup Member checking
    @copydoc HasMember.h
*/

//@{
/** @name
    @brief Macro helpers for checking the existence of variables inside a class
*/
//@{
/// Macro helper for checking variables
#define VAR_HELPER(HM_T, var, ...) std::declval<HM_T>().var

/// Calls HAS_MEMBER() with the VAR_HELPER() helper
#define HAS_VAR_IMPL(STATIC, ...) EXPAND(HAS_MEMBER(VAR_HELPER, STATIC, __VA_ARGS__, CONCAT(Has, __VA_ARGS__)))

/// Delegates the call to HAS_VAR_IMPL() with @p STATIC set to nothing (call outside a class)
#define HAS_VAR(...) HAS_VAR_IMPL(, __VA_ARGS__)

/// Delegates the call to HAS_VAR_IMPL() with @p STATIC set to @c static (call inside a class)
#define HAS_VAR_STATIC(...) HAS_VAR_IMPL(static, __VA_ARGS__)
//@}


/** @name
    @brief Macro helpers for checking the existence of functions inside a class
    @note For overloaded functions, use HAS_OVERLOADED_FUNC()
*/
//@{
/// Macro helper for checking regular functions
#define FUNC_HELPER(HM_T, func, ...) &HM_T::func

/// Calls HAS_MEMBER() with the FUNC_HELPER() helper
#define HAS_FUNC_IMPL(STATIC, ...) EXPAND(HAS_MEMBER(FUNC_HELPER, STATIC, __VA_ARGS__, CONCAT(Has, __VA_ARGS__)))

/// Delegates the call to HAS_FUNC_IMPL() with @p STATIC set to nothing (call outside a class)
#define HAS_FUNC(...) HAS_FUNC_IMPL(, __VA_ARGS__)

/// Delegates the call to HAS_FUNC_IMPL() with @p STATIC set to @c static (call inside a class)
#define HAS_FUNC_STATIC(...) HAS_FUNC_IMPL(static, __VA_ARGS__)
//@}


/** @name
    @brief Macro helpers for checking the existence of overloaded functions inside a class
    @note Does not check for strict overloads
*/
//@{
/// Macro helper for checking overloaded functions
#define OVERLOADED_FUNC_HELPER(HM_T, func, Args, ...) std::declval<HM_T>().func(std::declval<HM_Args>()...)

/// Calls HAS_MEMBER() with the OVERLOADED_FUNC_HELPER() helper
#define HAS_OVERLOADED_FUNC_IMPL(STATIC, ...) EXPAND(HAS_MEMBER(OVERLOADED_FUNC_HELPER, STATIC, __VA_ARGS__, CONCAT(has_, __VA_ARGS__)))

/// Delegates the call to HAS_OVERLOADED_FUNC_IMPL() with @p STATIC set to nothing (call outside a class)
#define HAS_OVERLOADED_FUNC(...) HAS_OVERLOADED_FUNC_IMPL(, __VA_ARGS__)

/// Delegates the call to HAS_OVERLOADED_FUNC_IMPL() with @p STATIC set to @c static (call inside a class)
#define HAS_OVERLOADED_FUNC_STATIC(...) HAS_OVERLOADED_FUNC_IMPL(static, __VA_ARGS__)
//@}


/** @name
    @brief Macro helpers for checking the existence of functions that takes a given class as parameter
*/
//@{
/// Macro helper for checking for functions
#define EXTERN_FUNC_HELPER(HM_T, ext, Args, ...) ext(std::declval<HM_T>(), std::declval<HM_Args>()...)

/// Calls HAS_MEMBER() with the EXTERN_FUNC_HELPER() helper
#define HAS_EXTERN_FUNC_IMPL(STATIC, ...) EXPAND(HAS_MEMBER(EXTERN_FUNC_HELPER, STATIC, __VA_ARGS__, CONCAT(Has, __VA_ARGS__)))

/// Delegates the call to HAS_EXTERN_FUNC_IMPL() with @p STATIC set to nothing (call outside a class)
#define HAS_EXTERN_FUNC(...) HAS_EXTERN_FUNC_IMPL(, __VA_ARGS__)

/// Delegates the call to HAS_EXTERN_FUNC_IMPL() with @p STATIC set to @c static (call inside a class)
#define HAS_EXTERN_FUNC_STATIC(...) HAS_EXTERN_FUNC_IMPL(static, __VA_ARGS__)
//@}



/** @brief Uses SFINAE to determine the existence of a certain member given a class

    @note I am using @p HM_T and @p HM_Args  to avoid any naming conflict if you 
          call this guy inside a class template, for example.
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

//@}

}



#endif //HANDY_HELPERS_HAS_MEMBER_H