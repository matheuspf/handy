/** @file

    @brief A class to take a "slice" of a cnt::Container class.
*/

#ifndef HANDY_CONTAINER_SLICE_H
#define HANDY_CONTAINER_SLICE_H

#include "Helpers.h"

#include <tuple>
#include <algorithm>
#include <cmath>
#include <numeric>


namespace handy
{

namespace impl
{

/** @brief This class is just a proxy to access chosen dimensions of a container. The only storage is 
           the positions to access and a reference to the container that created this slice. Some examples:

    @snippet Container/SliceExample.cpp Container Slice Snippet
*/
template <class Cnt>
class Slice
{
public:


    /** Some type definition
    */
    //@{
    using Base = std::decay_t<Cnt>;


    using value_type = typename Base::value_type;

    using reference = typename Base::reference;

    using const_reference = typename Base::const_reference;
    //@}



// --------------------------------- Constructors ---------------------------------------------- //


    /** These are the same constructor types defined for the 'Container' class, but now
      * with a reference to the 'Container' class that created this slice. Also, the
      * slice defines the range that it can access.
    */
    //@{

    /// For integrals
    template <typename... Args, handy::impl::cnt::EnableIfIntegral< std::decay_t< Args >... > = 0 >
    Slice (Cnt& c, const Args&... args) : c(c), dims(sizeof...(Args)), first(0)
    {
        auto iter = c.weights.begin();

        const auto& dummy = { (first += *iter++ * args, int{})..., int{} };

        last = first + (sizeof...(Args) ? c.weights[dims-1] : c.size());
    }


    /// For iterables
    template <typename... Args, std::enable_if_t<sizeof...(Args), int> = 0,
              handy::impl::cnt::EnableIfIterable< std::remove_reference_t< Args >... > = 0 >
    Slice (Cnt& c, const Args&... args) : c(c), dims(0), first(0)
    {
        auto iter = c.weights.begin();

        const auto& dummy = { (first += std::inner_product(args.begin(), args.end(), iter, 0), 
                                                            iter += args.size())... };
        dims = iter - c.weights.begin();

        last = first + c.weights[dims-1];
    }


    /// For a range defined by iterators
    template <typename U, typename V, handy::impl::cnt::EnableIfIterator< std::decay_t< U >, std::decay_t< V > > = 0>
    Slice (Cnt& c, const U& begin, const V& end) : c(c), dims(std::distance(begin, end)), first(0)
    {
        first = std::inner_product(begin, end, c.weights.begin(), 0);

        last = first + c.weights[dims-1];
    }


    /// For list initialization
    template <typename U, handy::impl::cnt::EnableIfIntegral<std::decay_t<U>> = 0> 
    Slice (Cnt& c, std::initializer_list<U> il) : Slice(c, il.begin(), il.end()) {}
    //@}




// ------------------------------- Access - operator() --------------------------------------------- //


    /** Again, these are almost identical to 'Container' accessors. Of course, they
      * can only access the defined range, so the operations are a bit different,
      * but the interface is the same.
    */
    //@{

    /// For integral or iterable types
    template <typename... Args>
    const_reference operator () (handy::impl::cnt::IntegralType, const Args&... args) const
    {
        std::size_t pos = first;

        auto iter = c.weights.begin() + dims;

        const auto& dummy = { (pos += Base::increment(args, iter), int{})... };

        return c[pos];
    }


    /// For iterators
    template <typename U, handy::impl::cnt::EnableIfIterator< std::decay_t< U >> = 0>
    const_reference operator () (handy::impl::cnt::IteratorType, const U& begin) const
    {
        return this->operator[](std::inner_product(c.weights.begin() + dims, c.weights.end(), begin(), first));
    }


    /// For 'std::intializer_lists'
    template <typename U>
    const_reference operator () (std::initializer_list<U> il) const
    {
        return this->operator[](std::inner_product(c.weights.begin() + dims, c.weights.end(), il.begin(), 0));
    }




    /** Overloading the access via 'operator[]' */
    //@{
    const_reference operator [] (int p) const
    {
        return c[first + p];
    }

    reference operator [] (int p)
    {
        return const_cast<reference>(static_cast<const Slice&>(*this)[p]);
    }
    //@}



    /// Size of each dimension
    auto size (int p) const { return c.size(dims + p); }

    /// Total size of the slice
    auto size () const      { return last - first; }


    /** Begin and end */
    //@{
    decltype(auto) begin ()        { return c.begin() + first; }

    decltype(auto) cbegin () const { return c.cbegin() + first; }

    decltype(auto) end ()          { return c.begin() + last; }

    decltype(auto) cend ()   const { return c.cbegin() + last; }
    //@}


private:

    Cnt& c;             /// Reference to the creator container

    int dims;           /// Number of dimensions BEFORE the slice
    int first;          /// First element on the contiguous array
    int last;           /// Last element on the contiguous array

};


} // namespace impl

} // namespace handy



#endif // HANDY_CONTAINER_SLICE_H