#ifndef HANDY_WRAPPER_MACROS_H
#define HANDY_WRAPPER_MACROS_H

#include <type_traits>

#include <ostream>

#include "../Helpers/HasMember.h"



#define USING_WRAPPER(...)  using Base = __VA_ARGS__;  \
\
                            using Base::Base;    \
                            using Base::operator =;  \
\
                            using Base::t;  \
\
                            using Type     = typename Base::Type;   \
                            using BaseType = typename Base::BaseType;




#define WRAPPER_DECLARATION_BOTH(OP)    \
template <typename U>   \
friend auto operator OP (const Wrapper<T>& w1, const Wrapper<U>& w2)

#define WRAPPER_DECLARATION_LEFT(OP)    \
template <typename U, std::enable_if_t<!::handy::isWrapperBase(std::decay_t<U>())>* = nullptr>  \
friend auto operator OP (const Wrapper<T>& w1, const U& u)

#define WRAPPER_DECLARATION_RIGHT(OP)   \
template <typename U, std::enable_if_t<!::handy::isWrapperBase(std::decay_t<U>())>* = nullptr>  \
friend auto operator OP (const T& t, const Wrapper<U>& w1)



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
template <typename U, std::enable_if_t<!::handy::isWrapperBase(std::decay_t<U>())>* = nullptr>  \
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
    WRAPPER_ARITHMETIC_OPERATOR_HELPER(OP, ret.t CONCAT(OP, =) u )  \
}   \
\
WRAPPER_DECLARATION_RIGHT(OP)   \
{   \
    WRAPPER_ARITHMETIC_OPERATOR_HELPER(OP, ret.t CONCAT(OP, =) t )  \
}



// #define WRAPPER_COMPARISON_OPERATOR(OP) \
// bool operator OP (const )







namespace handy
{

template <typename>
struct Wrapper;



template <typename T>
struct IsWrapper : std::false_type {};

template <typename T>
struct IsWrapper<Wrapper<T>> : std::true_type {};



template <typename T>
struct IsWrapperBase : public IsInherited<T>
{
    template <typename U>
	static constexpr std::true_type isInherited (const Wrapper<U>&);
};



template <typename T>
constexpr bool isWrapperBase (Wrapper<T>){ return true; }

constexpr bool isWrapperBase (...){ return false; }



template <class U>
auto makeWrapper (U&& u)
{
    return Wrapper<std::decay_t<U>>(std::forward<U>(u));
}


template <typename T>
std::ostream& operator << (std::ostream& out, const Wrapper<T>& w)
{
    return out << w.t;
}



} // namespace handy






#endif // HANDY_WRAPPER_MACROS_H