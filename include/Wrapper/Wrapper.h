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


    Wrapper() = default;    /// Empty constructor

    virtual ~Wrapper () {}   /// You will inherit from this cass




    template <typename... Args, std::enable_if_t<(sizeof...(Args) >= 2)>* = nullptr>
    Wrapper (Args&&... args) : t(std::forward<Args>(args)...) {}
    



    /// Constructor for different Wrapper types
    template <typename U>
    Wrapper (Wrapper<U>& w) : Wrapper(w.t) {}
    
    template <typename U>
    Wrapper (Wrapper<U>&& w) : Wrapper(std::move(w.t)) {}

    template <typename U>
    Wrapper (const Wrapper<U>& w) : Wrapper(w.t) {}




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


    /// Constructors for type 'T'
    Wrapper (const BaseType& b) : t(b) {}

    Wrapper (BaseType& b) : t(b) {}

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



    WRAPPER_ARITHMETIC_OPERATOR(+)
    WRAPPER_ARITHMETIC_OPERATOR(-)
    WRAPPER_ARITHMETIC_OPERATOR(*)
    WRAPPER_ARITHMETIC_OPERATOR(/)
    WRAPPER_ARITHMETIC_OPERATOR(%)
    WRAPPER_ARITHMETIC_OPERATOR(|)
    WRAPPER_ARITHMETIC_OPERATOR(&)
    WRAPPER_ARITHMETIC_OPERATOR(^)

    WRAPPER_COMPARISON_OPERATOR(==)
    WRAPPER_COMPARISON_OPERATOR(!=)
    WRAPPER_COMPARISON_OPERATOR(<)
    WRAPPER_COMPARISON_OPERATOR(>)
    WRAPPER_COMPARISON_OPERATOR(<=)
    WRAPPER_COMPARISON_OPERATOR(>=)
    WRAPPER_COMPARISON_OPERATOR(&&)
    WRAPPER_COMPARISON_OPERATOR(||)


    operator T& () { return t; }

    //operator const T& () const { return t; }
    

    


    T& operator * ()             { return t; }
    const T& operator * () const { return t; }

    decltype(auto) operator [] (int x)       { return t[x]; }
    decltype(auto) operator [] (int x) const { return t[x]; }

    decltype(auto) begin ()       { return t.begin(); }
    decltype(auto) begin () const { return t.begin(); }

    decltype(auto) end ()       { return t.begin(); }
    decltype(auto) end () const { return t.begin(); }
    





    friend std::ostream& operator << (std::ostream& out, const Wrapper& w)
    {
        out << w.t;

        return out;
    }



protected:


    Type t;        /// The only storage of the class

    template <typename> friend class Wrapper;     /// All wrappers are friends of each other (independently of the type)

};


template <typename T>
struct Wrapper<Wrapper<T>> : public Wrapper<T>
{
    USING_WRAPPER(Wrapper<T>)
};





} // namespace handy




#undef WRAPPER_DECLARATION_BOTH
#undef WRAPPER_DECLARATION_LEFT
#undef WRAPPER_DECLARATION_RIGHT
#undef WRAPPER_ARITHMETIC_OPERATOR_HELPER
#undef WRAPPER_ARITHMETIC_OPERATOR
#undef WRAPPER_COMPARISON_OPERATOR


#endif // HANDY_WRAPPER_H
