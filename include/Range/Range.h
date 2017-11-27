/** @file

    @brief Range utility similar to python's range

    A set of utilities for defining ranges, mainly used for loop iterations.

    The ranges can have any kind of type. The only requirement is that the type must have @c operator== defined.

    Also, you can defined half-closed intervals, closed intervals, and even infinite intervals.

    An example of defining a half-closed interval:

    @snippet Range/RangeExample.cpp HalfClosed Range Snippet
    
    For closed intervals the syntax is almost the same:

    @snippet Range/RangeExample.cpp Closed Range Snippet
    
    For infinite intervals you need, of course, stop the loop at some point:

    @snippet Range/RangeExample.cpp Infinite Range Snippet
*/

#ifndef HANDY_RANGE_H
#define HANDY_RANGE_H

#include "../Helpers/Helpers.h"

#include <iterator>
#include <vector>
#include <cmath>


/** @defgroup RangeGroup Range utilities
    @copydoc Range.h
*/
//@{

namespace handy
{

namespace impl
{

/** @brief Defines a half-closed interval
    
    To be a valid range, there must be a positive number @c c such that:
          
    @f$ first + c * step = last @f$
    @f$ c = (last - first) / step @f$
          
    As we only care for the sign of @c c, we can change the division by a multiplication.
    
    The equality test is for closed range intervals.
*/
struct HalfClosedInterval
{
    /** @brief Checks if the half-closed interval defined by @p first, @p last and @p step is valid

        @param first The first element in the interval
        @param last The last element in the interval
        @param step The step between points of the interval
    */
    template <typename T>
    bool operator () (const T& first, const T& last, const T& step) const
    {
        return step * (last - first) > 0;
    }
};

/** @brief Defines a closed interval
    @copydetails HalfClosedInterval
*/
struct ClosedInterval
{
    /** @brief Checks if the closed interval defined by @p first, @p last and @p step is valid
        @copydetails HalfClosedInterval::operator()()
    */
    template <typename T>
    bool operator () (const T& first, const T& last, const T& step) const
    {
        return step * (last - first) >= 0;
    }
};

/// Defines an infinite interval
struct InfiniteInterval
{
    /// Simply returns true for any argument -- any interval is valid
    template <typename T>
    constexpr bool operator () (const T&, const T&, const T&) const
    {
        return true;
    }
};

} // namespace impl


/** @brief Defines the procedures for generating ranges
    
    @tparam T The type to generate the ranges. Must have @c operator== defined
    @tparam ValidRange The class for validating ranges

    It is basically an iterable class which holds the values defining the range, exposing the
    #begin and #end methods.

    It does lazy evaluation, so no operation is done at creation.
*/
template <typename T, typename ValidRange = impl::HalfClosedInterval>
class Range
{
public:

    /** @name
        @brief Some type definitions
    */
    //@{
    class Iterator;

    friend Iterator;


    using Type = std::decay_t<T>;

    using value_type = Type;

    using iterator = Iterator;

    using iterator_category = typename iterator::iterator_category;

    /** You will never use @c const Iterator<t> in pratice. But, if you do, the iterator cannot 
        really be const, so we can increment the @c value variable.
    */
    using const_iterator = iterator;
    //@}



    /** @brief Defines only the last position of the range
        
        If a single argument is given, the first position is set to @c Type{0}, and the step is
        set to @c -Type{1} if <t>last < Type{0}<!t>, or @c Type{1} otherwise
        
        @param last The last element of the range 
    */
    Range (const Type& last) : Range(Type{0}, last, last < Type{0} ? -Type{1} : Type{1}) {}

    
    /** @brief Defines the first and last positions of the range
        
        If the step is not given, it is set to @c -Type{1} if <t>last < Type{0}<!t>, or @c Type{1} otherwise

        @param first The first element of the range
        @param last The last element of the range
    */
    Range (const Type& first, const Type& last) : Range(first, last, last < first ? -Type{1} : Type{1}) { }


    /** @brief Defines the first and last positions of the range along with the step between points
        
        @param first The first element of the range
        @param last The last element of the range
        @param step The step to take between points of the range
    */
    Range (const Type& first, const Type& last, const Type& step) : first(first), last(last), step(step)
    {
        // assert(step && "Step size must be non zero.");
        // assert(validRange(first, last, step) && "Invalid step for this range.");
    }


    /** @brief The iterator for the Range class

        It is a forward iterator, defining deferrencing, increment and comparison operators
    */
    class Iterator : public std::iterator<std::forward_iterator_tag, Type>
    {
    public:

        /** @name
            @brief Some type definitions
        */
        //@{
        using Base = std::iterator<std::forward_iterator_tag, Type>;

        using value_type        = typename Base::reference;
        using reference         = typename Base::reference;
        using pointer           = typename Base::pointer;
        using difference_type   = typename Base::difference_type;
        using iterator_category = typename Base::iterator_category;
        //@}

        
        /** @brief Constructor for Range::Iterator
            
            @param range A constant reference to a Range class
            @param value The intial value in the range
        */
        Iterator (const Range& range, Type value) : range(range), value(value) {}


        /** @name
            @brief The operators for a forward iterator
        */
        //@{
        reference operator* () { return value; }

        pointer operator-> () { return &value; }


        Iterator operator ++ () { value += range.step;  return *this;  }

        Iterator operator ++ (int) { Iterator ret(*this);  this->operator++();  return ret; }


        bool operator == (const Iterator& it) const { return this->value == it.value; }

        bool operator != (const Iterator &it) const { return range.validRange(value, it.value, range.step); }
        //@}


    private:


        const Range& range;  ///< A constant reference to a Range class

        Type value;     ///< The initial value of the range
    };



    /** @name
        @brief #begin() and #end() definitions
    */
    //@{
    iterator begin() { return iterator(*this, first); }

    iterator end() { return iterator(*this, last); }


    const_iterator begin() const { return const_iterator(*this, first); }

    const_iterator end() const { return const_iterator(*this, last); }
    //@}


    
    /** @brief Evaluates a range into a std::vector

        @return A std::vector with the evaluated range   
    */
    std::vector<Type> eval ()
    {
        std::vector<Type> evaluated;

        evaluated.reserve(std::ceil((last - first) / step));

        eval(std::back_inserter(evaluated));

        return evaluated;
    }

    /** @brief Evaluates a range into the given iterator

        @param iter The iterator to copy the range
    */
    template <typename Iter>
    void eval (Iter iter)
    {
        std::copy(this->begin(), this->end(), iter);
    }




private:

    Type first;     ///< First element of the range

    Type last;      ///< Last element of the range

    Type step;      ///< Step between elements of the range

    ValidRange validRange;  ///< Range validator
};





template <typename First, typename Last, typename Step>
decltype(auto) range (const First& first, const Last& last, const Step& step)
{
    using Type = impl::TypePrec_t<impl::TypePrec_t<First, Last>, Step>;

	return Range<Type, impl::HalfClosedInterval>(first, last, step);
}

template <typename First, typename Last>
decltype(auto) range (const First& first, const Last& last)
{
    using Type = impl::TypePrec_t<First, Last>;

	return Range<Type, impl::HalfClosedInterval>(first, last);
}

template <typename Last>
decltype(auto) range (const Last& last)
{
	return Range<Last, impl::HalfClosedInterval>(last);
}



template <typename First, typename Last, typename Step>
decltype(auto) crange (const First& first, const Last& last, const Step& step)
{
    using Type = impl::TypePrec_t<impl::TypePrec_t<First, Last>, Step>;

	return Range<Type, impl::ClosedInterval>(first, last, step);
}

template <typename First, typename Last>
decltype(auto) crange (const First& first, const Last& last)
{
    using Type = impl::TypePrec_t<First, Last>;

	return Range<Type, impl::ClosedInterval>(first, last);
}

template <typename Last>
decltype(auto) crange (const Last& last)
{
	return Range<Last, impl::ClosedInterval>(last);
}




template <typename First, typename Step>
decltype(auto) irange (const First& first, const Step& step)
{
    using Type = std::decay_t<impl::TypePrec_t<First, Step>>;

	return Range<Type, impl::InfiniteInterval>(first, Type{0}, step);
}

template <typename First>
decltype(auto) irange (const First& first)
{
    using Type = std::decay_t<First>;

	return Range<First, impl::InfiniteInterval>(first, Type{0}, Type{1});
}

decltype(auto) irange ()
{
	return Range<int, impl::InfiniteInterval>(0, 0, 1);
}
//@}

}	// namespace handy


#endif // HANDY_RANGE_H
