/** @file

    @brief Printing utilities

    @details By creating a handy::Print you can define how to print stuff. Also supports stl container printing. Ex:

    @snippet Helpers/PrintExample.cpp Print Class Snippet

    @details You can also use the direct function handy::print(). Ex:

    @snippet Helpers/PrintExample.cpp Print Function Snippet
*/

#include "Helpers.h"
#include "HasMember.h"

#include <ostream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>


namespace handy
{

HAS_EXTERN_FUNC(std::begin, ItHasBegin)
HAS_EXTERN_FUNC(std::end, ItHasEnd)

template <typename T>
struct ItIsContainer : public std::integral_constant<bool, ItHasBegin<T>::value && ItHasEnd<T>::value> {};


/** @defgroup PrintGroup Printing utilities
    @copydoc Helpers/Print.h
*/
//@{


/** @brief Class for defining how and where to print

    @details Create a handy::Print class by passing the std::ostream output reference. The default is std::cout.
             You can also define what will be in between arguments and what will come after them. Ex:

    @snippet Helpers/PrintExample.cpp Print Class Snippet
*/
struct Print
{
    /** @brief Constructor for handy::Print
     
        @param output A std::ostream reference. Defaulted to std::cout
        @param delimiter The delimiter between arguments
        @param endLine The string to print after all arguments
    */
    Print (std::ostream& output = std::cout, const std::string& delimiter = " ", const std::string& endLine = "\n") :
           output(output), delimiter(delimiter), endLine(endLine) {}

    /// @copydoc Print()
    Print (const std::string& delimiter, const std::string& endLine = "\n", std::ostream& output = std::cout) :
           output(output), delimiter(delimiter), endLine(endLine) {}


    /** @name
	@brief Easy printing
    */
    //@{
    /** @brief Prints a sequence of arguments separated by #delimiter to the std::ostream reference @c #output

        @param out A reference to a std::ostream object
        @param t The first template argument
        @param args Variadic number of arguments

        @return The reference @c out
    */
    template <typename T, typename... Args>
    std::ostream& operator () (std::ostream& out, const T& t, const Args& ...args)
    {
        print(out, t);

        auto dummie = { ((out << delimiter, print(out, args)), 0)..., 0 };

        return out << endLine;
    }

    /** @brief Delegate the call to handy::Print::operator()() with std::cout as the std::ostream argument.
        
        @note Only allowed if the first type @c T does not inherits from (or is) a std::stringstream or std::ostream classes
    */
    template <typename T, typename... Args,
              typename std::enable_if<(!IsInherited<T, std::stringstream>::value &&
                                       !IsInherited<T, std::ostream>::value)>::type* = nullptr>
    std::ostream& operator () (const T& t, const Args& ...args)
    {
        return operator()(output, t, args...);
    }


    /** @brief Print a single argument
        @details Simply print the argument @p t to the std::ostream reference @p out if @p T is not a container.
                 If @p T is a container, print every element of the container separated by #delimiter
    */
    template <typename T, std::enable_if_t<!ItIsContainer<T>::value>* = nullptr>
    std::ostream& print (std::ostream& out, const T& t)
    {
        return out << t;
    }

    /// @copydoc Print::print()
    template <typename T, std::enable_if_t<ItIsContainer<T>::value>* = nullptr>
    std::ostream& print (std::ostream& out, const T& t)
    {
        auto it = std::begin(t);

        print(out, *it);

        std::advance(it, 1);

        std::for_each(it, std::end(t), [&](const auto& x)
        {
            out << delimiter;
            this->print(out, x);
        });

        return out;
    }
    //@}


    std::ostream& output;   ///< The output std::ostream reference

    std::string delimiter;  ///< What to print between arguments

    std::string endLine;    ///< What to print after arguments
};



/** @brief Create a handy::Print object and delegate the call to handy::Print::operator()()
    @copydetails handy::Print::operator()()
*/
template <typename T, typename... Args>
std::ostream& print (std::ostream& out, const T& t, const Args& ...args)
{
    Print prt(out);

    return prt(t, args...);
}

/** @brief Delegate the call to handy::print() with std::cout as output
    @copydetails handy::print()
*/
template <typename T, typename... Args,
        typename std::enable_if<(!IsInherited<T, std::stringstream>::value &&
                                !IsInherited<T, std::ostream>::value)>::type* = nullptr>
std::ostream& print (const T& t, const Args& ...args)
{
    return print(std::cout, t, args...);
}









} // namespace handy