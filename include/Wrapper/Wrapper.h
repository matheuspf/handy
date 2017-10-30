#ifndef WRAPPER_H
#define WRAPPER_H

#include <type_traits>
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



namespace wrp
{

namespace impl
{
    HAS_OVERLOADED_FUNC(operator+=, HasPlusEqual)
    HAS_EXTERN_FUNC(operator+, HasPlus)



    template <template <typename...> class Has, typename... Args>
    using EnableIfHas = typename std::enable_if<Has<Args...>::value, void>::type;
}




template <typename>
struct Wrapper;


template <class>
struct IsWrapper;


template <class U>
auto makeWrapper (U&& u)
{
    return Wrapper<std::decay_t<U>>(std::forward<U>(u));
}




template <typename T>
struct Wrapper
{
    using Type = T;     /// Type may have any qualifier

    using BaseType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;     /// Striped type


    Wrapper () {}   /// Empty constructor


    virtual ~Wrapper () {}   /// Virtual destructor for safe inheritance



    /// Constructor for different Wrapper types
    template <typename U>
    Wrapper (Wrapper<U>& w) : Wrapper(w.t) {}
    
    template <typename U>
    Wrapper (Wrapper<U>&& w) : Wrapper(std::move(w.t)) {}

    template <typename U>
    Wrapper (const Wrapper<U>& w) : Wrapper(w.t) {}



    /// Assignment operators for different Wrapper types
    template <typename U>
    Wrapper& operator = (Wrapper<U>& w)
    {
        t = w.t;

        return *this;
    }

    /// Assignment operators for different Wrapper types
    template <typename U>
    Wrapper& operator = (Wrapper<U>&& w)
    {
        t = std::move(w.t);

        return *this;
    }

    template <typename U>
    Wrapper& operator = (const Wrapper<U>& w)
    {
        t = w.t;
        
        return *this;
    }



    Wrapper (const BaseType& b) : t(b) {}
    Wrapper (BaseType& b) : t(b) {}
    Wrapper (BaseType&& b) : t(std::move(b)) {}


    // /// Universal constructor for 't' only allowed if the first argument is not a Wrapper
    // template <typename U, typename... Args, std::enable_if_t<!IsWrapper<U>::value, int> = 0>
    // Wrapper (U&& u, Args&&... args) : t(std::forward<U>(u), std::forward<Args>(args)...) {}




    /// Assignment operators for t
    Wrapper& operator = (const BaseType& b)
    {
        t = b;

        return *this;
    }

    Wrapper& operator = (BaseType&& b)
    {
        t = std::move(b);

        return *this;
    }




    template <typename U, typename V = std::decay_t<T>,
              std::enable_if_t<std::is_fundamental<V>::value || impl::HasPlusEqual<BaseType, U>::value, int> = 0>
    Wrapper& operator += (const Wrapper<U>& w)
    {
        this->t += w.t;
        return *this;
    }


    template <typename U, typename V = std::decay_t<T>,
              std::enable_if_t<std::is_fundamental<V>::value || impl::HasPlus<BaseType, U>::value, int> = 0>
    friend auto operator+ (const Wrapper& w1, const Wrapper<U>& w2)
    {
        return makeWrapper(w1.t + w2.t);
    }


    





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


 

template <typename U>
std::ostream& operator << (std::ostream& out, const Wrapper<U>& w)
{
    return out << w.t;
}



} // namespace wrp


#endif // WRAPPER_H
