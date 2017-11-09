#ifndef HANDY_WRAPPER_MACROS_H
#define HANDY_WRAPPER_MACROS_H

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
template <typename U, typename V = std::decay_t<T>, impl::EnableIfHas<impl::HasPlusEqual, T, V> = nullptr>  \
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
    ret += t;   \
\
    return ret;    \
\
}









#endif // HANDY_WRAPPER_MACROS_H