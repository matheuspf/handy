#ifndef RANGE_H
#define RANGE_H

#include <type_traits>
#include <assert.h>
#include <iterator>
#include <vector>
#include <cmath>
#include <iostream>


namespace rg
{

namespace impl
{

template <typename T, typename U>
struct TypePrec
{
    using Type = decltype(std::declval<std::decay_t<T>>() + std::declval<std::decay_t<U>>());
};

template <typename T, typename U>
using TypePrec_t = typename TypePrec<T, U>::Type;




struct HalfClosedInterval
{
    template <typename T>
    bool operator () (const T& first, const T& last, const T& step) const
    {
        return step * (last - first) > 0;
    }
};



struct ClosedInterval
{
    template <typename T>
    bool operator () (const T& first, const T& last, const T& step) const
    {
        return step * (last - first) >= 0;
    }
};



struct InfiniteInterval
{
    template <typename T>
    constexpr bool operator () (const T&, const T&, const T&) const
    {
        return true;
    }
};


}


template <typename T, typename ValidRange = impl::HalfClosedInterval>
class Range
{
public:


    class Iterator;

    friend Iterator;


    using Type = std::decay_t<T>;

    using value_type = Type;

    using iterator = Iterator;

    using iterator_category = typename iterator::iterator_category;

    /** You will never use 'const Iterator<t>' in pratice. But, if you do, the
    *   iterator cannot really be const, so we can increment the 'value' variable.
    **/
    using const_iterator = iterator;


    // static constexpr Type zero = Type{0};

    // static constexpr Type unit = Type{1};



    Range (const Type& last) : Range(Type{0}, last, last < Type{0} ? -Type{1} : Type{1}) {}

    Range (const Type& first, const Type& last) : Range(first, last, last < first ? -Type{1} : Type{1}) { }


    Range (const Type& first, const Type& last, const Type& step) : first(first), last(last), step(step)
    {
        assert(step && "Step size must be non zero.");


        /** To be a valid range, there must be a positive number 'c' such that:
         * 
         *  first + c * step = last   --->    c = (last - first) / step
         * 
         *  As we only care for the sign of 'c', we can change the division by a multiplication.
         *  The equal test is for the case where 'last == first'.
        **/
        assert(validRange(first, last, step) && "Invalid step for this range.");
        //assert(step * (last - first) >= 0 && "Invalid step for this range.");
    }



    class Iterator : public std::iterator<std::forward_iterator_tag, Type>
    {
    public:

        using Base = std::iterator<std::forward_iterator_tag, Type>;

        using value_type        = typename Base::reference;
        using reference         = typename Base::reference;
        using pointer           = typename Base::pointer;
        using difference_type   = typename Base::difference_type;
        using iterator_category = typename Base::iterator_category;


        Iterator (const Range& range, Type value) : range(range), value(value) {}


        /// Forward iterator
        reference operator* () { return value; }

        pointer operator-> () { return &value; }


        Iterator operator ++ () { value += range.step;  return *this;  }

        Iterator operator ++ (int) { Iterator ret(*this);  this->operator++();  return ret; }


        bool operator == (const Iterator& it) const { return this->value == it.value; }

        bool operator != (const Iterator &it) const { return range.validRange(value, it.value, range.step); }



    private:


        const Range& range;

        Type value;
    };




    iterator begin() { return iterator(*this, first); }

    iterator end() { return iterator(*this, last); }


    const_iterator begin() const { return const_iterator(*this, first); }

    const_iterator end() const { return const_iterator(*this, last); }




    std::vector<Type> eval ()
    {
        std::vector<Type> evaluated;

        evaluated.reserve(std::ceil((last - first) / step));

        eval(std::back_inserter(evaluated));

        return evaluated;
    }


    template <typename Iter>
    void eval (Iter iter)
    {
        std::copy(this->begin(), this->end(), iter);
    }




private:

    Type first;

    Type last;

    Type step;

    ValidRange validRange;
};

// template <typename T, typename ValidRange = impl::HalfClosedInterval>
// typename Range<T, ValidRange>::Type Type{0} = typename Range<T, ValidRange>::Type{0};

// template <typename T, typename ValidRange = impl::HalfClosedInterval>
// typename Range<T, ValidRange>::Type Type{1} = typename Range<T, ValidRange>::Type{1};




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


}	// namespace rg


#endif // RANGE_H
