#ifndef WRAPPER_H
#define WRAPPER_H

#include <type_traits>
#include <functional>

#define USING_WRAPPER(...)  using Base = __VA_ARGS__;  \
\
                            using Base::Base;    \
                            using Base::operator =;  \
\
                            using Base::t;  \
\
                            using Type     = typename Base::Type;  \
                            using BaseType = typename Base::BaseType;    \



namespace wrp
{

// Base class for inheritance


template <typename T>
struct Wrapper
{
    using Type = T;     // Type may have any qualifier

    using BaseType = std::decay_t<T>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // Default constructor only allowed if Type is not a reference

    template <typename U = T, typename = std::enable_if_t<!std::is_reference<U>::value>>
    constexpr Wrapper () : t{} {}


    // Delegating constructor only if Type is not a reference

	//template <typename... Args, typename U = T, typename = std::enable_if_t<!std::is_reference<U>::value>>
	//constexpr Wrapper (Args&&... args) : t{std::forward<Args>(args)...} {}


    constexpr Wrapper (BaseType& b) : t{b} {}

    constexpr Wrapper (BaseType&& b) : t{std::move(b)} {}

    constexpr Wrapper (const BaseType& b) : t{b} {}



    template <typename U>
    constexpr Wrapper (Wrapper<U>& w) : Wrapper(w.t) {}

    template <typename U>
    constexpr Wrapper (Wrapper<U>&& w) : Wrapper(std::move(w.t)) {}

    template <typename U>
    constexpr Wrapper (const Wrapper<U>& w) : Wrapper(w.t) {}



    /*constexpr operator BaseType& () { return t; }

    constexpr operator BaseType&& () { return t; }

    constexpr operator const BaseType& () { return t; }*/

    constexpr operator Type () { return t; }



    template <typename U>
    explicit constexpr operator U () { return t; }




    decltype(auto) operator = (BaseType& b) { t = b; return *this; }

    decltype(auto) operator = (BaseType&& b) { t = std::move(b); return *this; }

    decltype(auto) operator = (const BaseType& b) { t = b; return *this; }


    template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>&& w) { t = std::move(w.t); return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }


    //template <typename U>
    //decltype(auto) operator == (const U& u) { return t == u; }

    //template <typename U>
    //decltype(auto) operator == (const Wrapper<U>& w) { return t == w.t; }


    friend decltype(auto) operator << (std::ostream& out, const Wrapper& w) { return out << w.t; }


    template <typename U>
    decltype(auto) operator [] (U&& u) { return t[std::forward<U>(u)]; }

    template <typename U>
    decltype(auto) operator [] (U&& u) const { return t[std::forward<U>(u)]; }

};

}


#endif // WRAPPER_H
