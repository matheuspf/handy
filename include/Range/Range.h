#ifndef RANGE_H
#define RANGE_H

#include <type_traits>
#include <iterator>


namespace rg
{

namespace impl
{

template <typename T, typename U>
struct TypePrec { using Type = decltype(std::declval<T>() + std::declval<U>()); };

template <typename T, typename U>
using TypePrec_t = typename TypePrec<T, U>::Type;

}


template <typename T, typename U>
class Range
{
public:


	using Type = impl::TypePrec_t<T, U>;

	using Step = U;



	Range (const Type& begin, const Type& end, const Step& step) : begin_(begin), end_(end), step_(step) {}

	Range (const Type& begin, const Type& end) : begin_(begin), end_(end), step_(Step{1}) {}

	Range (const Type& end) : begin_(Type{}), end_(end), step_(Step{1}) {}



	class Iterator : public std::iterator<std::forward_iterator_tag, Type>
	{
    public:

    friend class Range;


    	Iterator (const Type& index = Type{}, const Step& step = Step{1}) : index(index), step(step) {}


		inline Type& operator * () { return index; }

		inline const Type& operator * () const { return index; }


		inline const Iterator& operator ++ () { index += step; return *this; }

		inline Iterator operator ++ (int) { Iterator ret(*this); this->operator++(); return ret; }



		inline bool operator < (const Iterator& it) const { return step > 0 ? index < it.index : index > it.index; }

		///// ---- GAMB  ----   /////

		inline bool operator == (const Iterator& it) const { return !(*this < it); }

		inline bool operator != (const Iterator &it) const { return !(*this == it); }

		///// ---- GAMB  ----   /////


	private:

		Type index;

		Step step;

   };




   inline Iterator begin() const { return Iterator(begin_, step_); }


   inline Iterator end() const { return Iterator(end_, step_); }



//private:

	Type begin_;

	Type end_;

	Step step_;


};


template <typename T, typename U, typename Step>
inline decltype(auto) range (const T& first, const U& last, const Step& step)
{
	return Range<impl::TypePrec_t<T, U>, Step>(first, last, step);
}

template <typename T, typename U>
inline decltype(auto) range (const T& first, const U& last)
{
	return Range<impl::TypePrec_t<T, U>, impl::TypePrec_t<T, U>>(first, last);
}

template <typename T>
inline decltype(auto) range (const T& last)
{
	return Range<T, T>(last);
}


//================================================================================================


template <class C, typename T>
class Connect
{
public:

    using Container = C;

    using Type = T;


    template <class C_, typename T_>
    Connect (C_&& container, T_&& value) : container(std::forward<C_>(container)), value(std::forward<T_>(value)) {}



    class Iterator : public std::forward_iterator_tag //std::iterator<std::forward_iterator_tag, Type>
	{
    public:

    friend class Connect;

        using It = decltype(std::declval<Container>().begin());


    	Iterator (Connect& connect, It it) : connect(connect), it(it) {}


		inline decltype(auto) operator * () { return *it + connect.value; }

		inline decltype(auto) operator * () const { return *it + connect.value; }


		inline const Iterator& operator ++ () { ++it; return *this; }

		inline Iterator operator ++ (int) { Iterator ret(*this); this->operator++(); return ret; }



		inline bool operator < (const Iterator& iter) const { return it < iter.it; }

		inline bool operator == (const Iterator& iter) const { return it == iter.it; }

		inline bool operator != (const Iterator &iter) const { return !operator==(iter); }


	private:

        Connect& connect;

        It it;

   };

    using iterator = Iterator;
    using const_iterator = const iterator;


    inline Iterator begin() { return Iterator(*this, container.begin()); }

    inline Iterator end() { return Iterator(*this, container.end()); }



//private:

    Container container;

    Type value;
};



template <class C, typename T>
inline decltype(auto) connect (C&& c, T&& t)
{
    return Connect<C, T>(std::forward<C>(c), std::forward<T>(t));
}


}	// namespace rg


#endif // RANGE_H
