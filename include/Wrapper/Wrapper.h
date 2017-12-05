/** @file
    
    @brief Helper class for defining uniform interfaces

    The handy::Wrapper helps you to define classes with uniform interface by constructing a thin, zero runtime 
    overhead layer to access the classe's methods

    You can create for example a handy::Wrapper<int> that has a specific @c .size() and @ operator[] functions,
    so you can iterate in a loop:

    @snippet Wrapper/InheritanceExample.cpp Wrapper Inheritance Snippet

    Another use is to easily define specialized operators for a given type:

    @snippet Wrapper/OperationsExample.cpp Wrapper Operations Snippet
*/

#ifndef HANDY_WRAPPER_H
#define HANDY_WRAPPER_H

#include "Helpers.h"

#include <iostream>



namespace handy
{

/** @defgroup WrapperGroup Interface helper
    @copydoc Wrapper.h
*/
//@{

template <typename T>
struct Wrapper
{
    using Type = T;     ///< Type may have any qualifier

    using BaseType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;     ///< Striped type


    Wrapper() = default;    ///< Empty constructor

    virtual ~Wrapper () {}   ///< You will inherit from this class



    /// For multiple arguments, the constructor of Type is used
    template <typename... Args, std::enable_if_t<(sizeof...(Args) >= 2)>* = nullptr>
    Wrapper (Args&&... args) : t(std::forward<Args>(args)...) {}
    



    /** @name
        @brief Constructors for a Wrapper of any type
    */
    //@{
    /// Only allow rvalues if Type is not a lvalue references
    template <typename U, typename V = Type, std::enable_if_t<!std::is_lvalue_reference<V>::value>* = nullptr>
    Wrapper (Wrapper<U>&& w) : Wrapper(std::move(w.t)) {}

    template <typename U>
    Wrapper (Wrapper<U>& w) : Wrapper(w.t) {}

    template <typename U>
    Wrapper (const Wrapper<U>& w) : Wrapper(w.t) {}
    //@}



    /** @name
        @brief Assignment operators for a Wrapper of any type
    */
    //@{
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
    //@}


    /** @name
        @brief Constructors for Type
    */
    //@{
    Wrapper (const BaseType& b) : t(b) {}

    /// If Type is a nonconst reference, it must be initialized with another nonconst reference
    Wrapper (BaseType& b) : t(b) {}
    
    /// Only allow rvalues if Type is not a lvalue references
    template <typename V = Type, std::enable_if_t<!std::is_lvalue_reference<V>::value>* = nullptr>
    Wrapper (BaseType&& b) : t(std::move(b)) {}
    
    /// std::initializer_list of any type is delegated to Type
    template <typename U>
    Wrapper (std::initializer_list<U> il) : t(il) {}
    //@}



    /** @name
        @brief Constructors for Type
    */
    //@{
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
    //@}



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


    /** @name
        @brief Conversion to Type
    */
    //@{
    operator Type&       ()       { return t; }

    operator const Type& () const { return t; }
    //@}

    

    /** @brief Delegating some operators
        
        Even if Type does not have definitions for these functions, they are not even compiled, because
        they can't even be used  (a compile time error will happen if you try to use something that is
        not there)
    */
    //@{
    Type& operator * ()             { return t; }
    const Type& operator * () const { return t; }

    decltype(auto) operator [] (int x)       { return t[x]; }
    decltype(auto) operator [] (int x) const { return t[x]; }

    decltype(auto) begin ()       { return t.begin(); }
    decltype(auto) begin () const { return t.begin(); }

    decltype(auto) end ()       { return t.begin(); }
    decltype(auto) end () const { return t.begin(); }
    //@}




    /// Delegate std::ostream& call to Type
    friend std::ostream& operator << (std::ostream& out, const Wrapper& w)
    {
        out << w.t;

        return out;
    }



protected:


    Type t;        ///< The only storage of the class

    /// All wrappers are friends of each other (independently of the type)
    template <typename> friend class Wrapper;     

};


/// A Wrapper of Wrapper is not necessary, and leads to some problems
template <typename T>
struct Wrapper<Wrapper<T>> : public Wrapper<T>
{
    USING_WRAPPER(Wrapper<T>)
};

//@}




} // namespace handy


#undef WRAPPER_DECLARATION_BOTH
#undef WRAPPER_DECLARATION_LEFT
#undef WRAPPER_DECLARATION_RIGHT
#undef WRAPPER_ARITHMETIC_OPERATOR_HELPER
#undef WRAPPER_ARITHMETIC_OPERATOR
#undef WRAPPER_COMPARISON_OPERATOR


#endif // HANDY_WRAPPER_H
