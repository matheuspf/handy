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
                            using Type     = typename Base::Type;   \
                            using BaseType = typename Base::BaseType;



// namespace wrp
// {

template <class>
struct IsWrapper;


template <typename T>
struct Wrapper
{
    using Type = T;     /// Type may have any qualifier

    using BaseType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;     /// Striped type


    virtual ~Wrapper () {}   /// Virtual destructor for safe inheritance



    /// Constructor for different Wrapper types
    template <typename U>
    constexpr Wrapper (Wrapper<U>& w) : Wrapper(w.t) {}

    template <typename U>
    constexpr Wrapper (Wrapper<U>&& w) : Wrapper(std::move(w.t)) {}

    template <typename U>
    constexpr Wrapper (const Wrapper<U>& w) : Wrapper(w.t) {}



    /// Assignment operators for different Wrapper types
    template <typename U>
    Wrapper& operator = (Wrapper<U>& w) { t = w.t; return *this; }

    template <typename U>
    Wrapper& operator = (Wrapper<U>&& w) { t = std::move(w.t); return *this; }

    template <typename U>
    Wrapper& operator = (const Wrapper<U>& w) { t = w.t; return *this; }



    /// Universal constructor for 't'
    template <typename... Args>
    constexpr Wrapper (Args&&... args) : t(std::forward<Args>(args)...) {}


    /// Universal assignment operator for 't'
    // template <typename U, std::enable_if_t<!IsWrapper<std::decay_t<U>>::value, int> = 0>
    // Wrapper& operator = (U&& u) { t = std::forward<U>(u); return *this; }


    Wrapper& operator = (const BaseType& b) { t = b; }

    Wrapper& operator = (BaseType&& b) { t = std::move(b); }




    template <typename U>
    Wrapper& operator += (const Wrapper<U>& w)
    {
        this->t += w.t;
        return *this;
    }


    template <typename U, typename V>
    friend auto operator+ (const Wrapper<U>&, const Wrapper<V>&);





    template <typename U>
    friend std::ostream& operator << (std::ostream&, const Wrapper<U>&);
    


//protected:

    Type t;        /// The only storage of the class

    template <class> friend class Wrapper;     /// All wrappers are friends of each other (independently of the type)

};



template <class T>
struct IsWrapper : std::false_type {};

template <class T>
struct IsWrapper<Wrapper<T>> : std::true_type {};



/// If 'T' is a 'Wrapper', take its type. Otherwise, simply take 'T'.
// template <class T>
// struct StripWrapper
// {
//     using type = T;
// };

// template <class T>
// struct StripWrapper<Wrapper<T>>
// {
//     using type = T;
// };


/// This is necessary so we nave have a 'Wrapper<Wrapper<T>>'
// template <typename T>
// using Wrapper = impl::Wrapper<typename impl::StripWrapper<T>::type>;

 


//} // namespace wrp


#endif // WRAPPER_H
