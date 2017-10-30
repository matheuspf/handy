#ifndef RANGE_H
#define RANGE_H

#include <type_traits>
#include <iterator>


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

}


template <typename T>
class Range
{
public:

class Iterator;


    using Type = std::decay_t<T>;

    using value_type = Type;
    
    using iterator = Iterator;

    using iterator_category = typename iterator::iterator_category;

    using const_iterator = iterator;



    static constexpr Type zero = Type{0};

    static constexpr Type unit = Type{1};



    Range (const Type& last) : Range(zero, last, unit) {}

    Range (const Type& first, const Type& last) : Range(first, last, unit) {}

    
    Range (const Type& first, const Type& last, const Type& step) : first(first), last(last), step(step)
    {
        assert(step && "Step size must be non zero.");

        assert
    }




	class Iterator : public std::iterator<std::random_access_iterator_tag, Type>
	{
    public:

        using Base = std::iterator<std::random_access_iterator_tag, Type>;
    
        using value_type      = typename Base::reference;
        using reference       = typename Base::reference;
        using difference_type = typename Base::difference_type;

        using iterator_category = typename Base::iterator_category;



        Iterator (const Range& range, Type value) : range(range), value(value) {}


        Type& operator * ()
        {
            return value;
        }

        const Type& operator * () const
        {
            return value;
        }


        const Iterator& operator ++ ()
        {
            index += step;
            
            return *this;
        }

        Iterator operator ++ (int)
        {
            Iterator ret(*this);
            
            this->operator++();
            
            return ret;
        }



		bool operator < (const Iterator& it) const { return step > 0 ? index < it.index : index > it.index; }

		///// ---- GAMB  ----   /////

		bool operator == (const Iterator& it) const { return !(*this < it); }

		bool operator != (const Iterator &it) const { return !(*this == it); }

		///// ---- GAMB  ----   /////


    //private:
    
        friend class Range;


        Range& range;
        
        Type value;
   };




    Iterator begin() const
    {
        return Iterator(*this, first);
    }

    Iterator end() const
    {
        return Iterator(*this, last);
    }


    const Iterator cbegin() const
    {
        return Iterator(*this, first);
    }

    const Iterator cend() const
    {
        return Iterator(*this, last);
    }




//private:

   Type first;

   Type last;

   Type step;

};



template <typename First, typename Last, typename Step>
decltype(auto) range (const First& first, const Last& last, const Step& step)
{
	return Range<impl::TypePrec_t<impl::TypePrec_t<First, Last>, Step>>(first, last, step);
}

template <typename First, typename Last>
decltype(auto) range (const First& first, const Last& last)
{
	return Range<impl::TypePrec_t<First, Last>>(first, last);
}

template <typename Last>
decltype(auto) range (const Last& last)
{
	return Range<Last>(last);
}




}	// namespace rg


#endif // RANGE_H
