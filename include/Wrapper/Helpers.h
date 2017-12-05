#ifndef HANDY_WRAPPER_MACROS_H
#define HANDY_WRAPPER_MACROS_H

#include <type_traits>
#include <initializer_list>
#include <ostream>

#include "../Helpers/HasMember.h"


/// Useful for defining operations inherited from Wrapper
#define USING_WRAPPER(...)  using Base = __VA_ARGS__;  \
\
                            using Base::Base;    \
                            using Base::operator =;  \
\
                            using Base::t;  \
\
                            using Type     = typename Base::Type;   \
                            using BaseType = typename Base::BaseType;   \
\
                            using Base::operator Type&;




/** @name
    @brief Macros that help to define Wrapper operators
*/
//@{
#define WRAPPER_DECLARATION_BOTH(OP)    \
template <typename U>   \
friend auto operator OP (const Wrapper<T>& w1, const Wrapper<U>& w2)

#define WRAPPER_DECLARATION_LEFT(OP)    \
template <typename U, std::enable_if_t<!impl::IsWrapperBase<U>::value>* = nullptr>  \
friend auto operator OP (const Wrapper<T>& w1, const U& u)

#define WRAPPER_DECLARATION_RIGHT(OP)   \
template <typename U, std::enable_if_t<!impl::IsWrapperBase<U>::value>* = nullptr>  \
friend auto operator OP (const U& u, const Wrapper<T>& w1)



#define WRAPPER_ARITHMETIC_OPERATOR_HELPER(OP, ...)    \
Wrapper<std::decay_t<decltype(std::declval<std::decay_t<T>>() OP std::declval<std::decay_t<U>>())>> ret(w1);    \
\
__VA_ARGS__;    \
\
return ret;





#define WRAPPER_ARITHMETIC_OPERATOR(OP)    \
\
template <typename U>  \
Wrapper& operator CONCAT(OP, =) (const Wrapper<U>& w)  \
{   \
    this->t CONCAT(OP, =) w.t; \
\
    return *this;   \
}   \
\
template <typename U, std::enable_if_t<!impl::IsWrapperBase<U>::value>* = nullptr>  \
Wrapper& operator CONCAT(OP, =) (const U& u)  \
{   \
    this->t CONCAT(OP, =) u; \
\
    return *this;   \
}\
\
\
WRAPPER_DECLARATION_BOTH(OP)    \
{   \
    WRAPPER_ARITHMETIC_OPERATOR_HELPER(OP, ret CONCAT(OP, =) w2 )  \
}   \
\
WRAPPER_DECLARATION_LEFT(OP)    \
{   \
    WRAPPER_ARITHMETIC_OPERATOR_HELPER(OP, ret CONCAT(OP, =) u )  \
}   \
\
WRAPPER_DECLARATION_RIGHT(OP)   \
{   \
    WRAPPER_ARITHMETIC_OPERATOR_HELPER(OP, ret CONCAT(OP, =) u )  \
}




#define WRAPPER_COMPARISON_OPERATOR(OP) \
WRAPPER_DECLARATION_BOTH(OP)    \
{   \
    return *w1 OP *w2;    \
}   \
\
WRAPPER_DECLARATION_LEFT(OP)    \
{   \
    return *w1 OP u;   \
}   \
\
WRAPPER_DECLARATION_RIGHT(OP)   \
{   \
    return u OP *w1; \
}
//@}








namespace handy
{

template <typename>
struct Wrapper;


namespace impl
{

/// Check if a given type @p T is a Wrapper
template <typename T>
struct IsWrapper : std::false_type {};

/// @copydoc IsWrapper
template <typename T>
struct IsWrapper<Wrapper<T>> : std::true_type {};


/// Check if a type inherits from a Wrapper
template <typename T>
struct IsWrapperBase : public IsInheritedTemplate<T, Wrapper> {};

} // namespace impl


/// Create a wrapper of class @p T and return it
template <class T>
auto makeWrapper (T&& t)
{
    return Wrapper<std::decay_t<T>>(std::forward<T>(t));
}



} // namespace handy






#endif // HANDY_WRAPPER_MACROS_H