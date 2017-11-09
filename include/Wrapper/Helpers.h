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




#define WRAPPER_ARITHMETIC_OPERATOR_IN(OP, CONST)    \
\
template <typename U, typename V = std::decay_t<T>, impl::EnableIfHas<impl::HasPlusEqual, T, V> = nullptr>  \
Wrapper& operator OP (const Wrapper<U>& w) CONST  \
{   \
    this->t += w.t; \
\
    return *this;   \
}   \
\
template <typename U, typename V = std::decay_t<T>, impl::EnableIfHas<impl::HasPlusEqual, T, V> = nullptr,  \
          std::enable_if_t<::handy::IsWrapperBase<U>::value>* = nullptr>  \
Wrapper& operator OP (const U& u) CONST  \
{   \
    this->t += u; \
\
    return *this;   \
}



#define WRAPPER_ARITHMETIC_OPERATOR_OUT(OP)    \
\
template <typename T, typename U,  impl::EnableIfHas<impl::HasPlus, T, U> = nullptr>   \
auto operator OP (const Wrapper<T>& w1, const Wrapper<U>& w2)   \
{   \
    Wrapper<std::decay_t<decltype(std::declval<std::decay_t<T>>() OP std::declval<std::decay_t<U>>())>> ret(w1);    \
\
    ret += w2;  \
\
    return ret;    \
}   \
\
template <typename T, typename U, impl::EnableIfHas<impl::HasPlus, T, U> = nullptr>   \
auto operator OP (const Wrapper<T>& w1, const U& u)   \
{   \
    Wrapper<std::decay_t<decltype(std::declval<std::decay_t<T>>() OP std::declval<std::decay_t<U>>())>> ret(w1);    \
\
    ret.t += u; \
\
    return ret;    \
}   \
\
template <typename T, typename U, impl::EnableIfHas<impl::HasPlus, T, U> = nullptr>   \
auto operator OP (const T& t, const Wrapper<U>& w2)   \
{   \
    Wrapper<std::decay_t<decltype(std::declval<std::decay_t<T>>() OP std::declval<std::decay_t<U>>())>> ret(w2);    \
\
    ret.t += t;   \
\
    return ret;    \
\
}



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




namespace impl
{
    HAS_OVERLOADED_FUNC(operator+=, HasPlusEqual)
    HAS_EXTERN_FUNC(operator+, HasPlus)



    template <template <typename...> class Has, typename T, typename... Args>
    using EnableIfHas = typename std::enable_if<std::is_fundamental<std::decay_t<T>>::value ||
                                                Has<T, Args...>::value, void>::type*;

}


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