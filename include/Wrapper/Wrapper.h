#ifndef HANDY_WRAPPER_H
#define HANDY_WRAPPER_H

#include "Helpers.h"




namespace handy
{

template <typename T>
struct Wrapper
{
    using Type = T;     /// Type may have any qualifier

    using BaseType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;     /// Striped type


    Wrapper () {}   /// Empty constructor


    virtual ~Wrapper () {}   /// You will inherit from this cass



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


    template <typename U = T, std::enable_if_t<!std::is_lvalue_reference<U>::value>* = nullptr>
    Wrapper (const BaseType& b) : t(b) {}

    template <typename U = T, std::enable_if_t<std::is_lvalue_reference<U>::value>* = nullptr>
    Wrapper (const BaseType& b) : t(const_cast<BaseType&>(b)) {}


    Wrapper (BaseType& b) : t(b) {}

    template <typename U = T, std::enable_if_t<!std::is_lvalue_reference<U>::value>* = nullptr>
    Wrapper (BaseType&& b) : t(std::move(b)) {}



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



    WRAPPER_ARITHMETIC_OPERATOR_IN(+=)
    WRAPPER_ARITHMETIC_OPERATOR_IN(-=)
    WRAPPER_ARITHMETIC_OPERATOR_IN(*=)
    WRAPPER_ARITHMETIC_OPERATOR_IN(/=)
    WRAPPER_ARITHMETIC_OPERATOR_IN(%=)
    WRAPPER_ARITHMETIC_OPERATOR_IN(|=)
    WRAPPER_ARITHMETIC_OPERATOR_IN(&=)
    WRAPPER_ARITHMETIC_OPERATOR_IN(^=)
    // WRAPPER_ARITHMETIC_OPERATOR_IN(<)
    // WRAPPER_ARITHMETIC_OPERATOR_IN(==)



    template <typename U>
    friend std::ostream& operator << (std::ostream&, const Wrapper<U>&);




//protected:

    Type t;        /// The only storage of the class


    template <class> friend class Wrapper;     /// All wrappers are friends of each other (independently of the type)

};



WRAPPER_ARITHMETIC_OPERATOR_OUT(+)
WRAPPER_ARITHMETIC_OPERATOR_OUT(-)
WRAPPER_ARITHMETIC_OPERATOR_OUT(*)
WRAPPER_ARITHMETIC_OPERATOR_OUT(/)
WRAPPER_ARITHMETIC_OPERATOR_OUT(%)
WRAPPER_ARITHMETIC_OPERATOR_OUT(|)
WRAPPER_ARITHMETIC_OPERATOR_OUT(&)
WRAPPER_ARITHMETIC_OPERATOR_OUT(^)






} // namespace handy


#endif // HANDY_WRAPPER_H
