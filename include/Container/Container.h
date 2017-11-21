/** @file
    @brief An interface to easily access multidimensional data, having total 
           compatibility with STL algorithms and containers.
*/

#ifndef HANDY_CONTAINER_H
#define HANDY_CONTAINER_H

#include <tuple>
#include <algorithm>
#include <cmath>
#include <numeric>

#include "Vector.h"
#include "Slice.h"



// #include <iostream>
// #define DB(...) std::cout << __VA_ARGS__ << "\n" << std::flush


namespace handy
{

namespace impl
{

template <class>
struct Accessor;



/** Class to easily create and manipulate multidimensional data. Interacts easily
  * with STL algorithms and can be either statically or dinamically allocated.
  *
  * \tparam T The Containers type
  * \tparam Is The compile time size of each dimension. The total size is the 
  *         multiplication of these sizes. See the 'Vector' class.
*/
template <typename T, std::size_t... Is>
class Container : public Vector<T, cnt::multiply_v<Is...>>
{
public:


    /** Some type definitions */
    //@{
    using Base = Vector<T, cnt::multiply_v<Is...>>;


    using value_type = typename Base::value_type;

    using reference = typename Base::reference;

    using const_reference = typename Base::const_reference;


    using Base::Size;
    //@}



    friend class Slice<Container>;     /// Friend definition for the 'Slice' class
    friend class Slice<const Container>;     /// Friend definition for the 'Slice' class




// --------------------------------- Constructors ---------------------------------------------- //


    /** Constructor defined when inheriting from 'std::array'. Simulates 'std::array' list 
      * initialization. The number of dimensions is given by 'Is'.
      *
      * \params[in] args Variadic arguments. 'Vector' checks if they are of type 'T'.
    */
    template <typename... Args, std::size_t M = Size, cnt::EnableIfArray< M > = 0>
    Container (Args&&... args) : Base{ std::forward<Args>(args)... }, 
                                 numDimensions_(sizeof...(Is)),
                                 dimSize( Is... )
    {
        initWeights();
    }


    /** Same as above, but now for a 'Container' inheriting from 'std::vector' with 'Size'
      * greater than the maximum stack allocation size. In this case, we must resize to
      * 'Size' after intiallizing with 'args'.
      *
      * \params[in] args Variadic arguments. 'Vector' checks if they are of type 'T'.
    */
    template <typename... Args, std::size_t M = Size, std::enable_if_t<( M >= cnt::maxSize ), int > = 0>
    Container (Args&&... args) : Base{std::forward<Args>(args)...}, 
                                 numDimensions_(sizeof...(Is)),
                                 dimSize(Is...)
    {
        initWeights();

        Base::resize(Size);
    }


    /// Empty constructor for the case where 'Size' is 0 (no compile time size is given)
    template <std::size_t M = Size, cnt::EnableIfZero< M > = 0>
    Container () {}


    /** Constructor for the case when 'Size' is 0 (inheriting from 'std::vector').
      * This time the parameters are integral values that define the size of each
      * dimension. So, '3, 4, 7' would gives us a 'Container' with thre dimensions
      * with sizes 3, 4 and 7, respectivelly.
      *
      * \param[in] args Variadic integral types defining the size of each dimension.
                        Only integral types are accepted.
    */
    template <typename... Args, std::size_t M = Size, cnt::EnableIfZero< M > = 0,
              cnt::EnableIfIntegral< std::decay_t< Args >... > = 0 >
    Container (Args... args) : numDimensions_(sizeof...(args)), 
                               dimSize{std::size_t(args)...},
                               weights(sizeof...(args))
    {
        initWeights();


        /// Total size is equal to this multiplication. See the 'initWeights' function.
        Base::resize(weights.front() * dimSize.front());
    }



    /** Another constructor defined when 'Size' is 0. Each element is an iterable type
      * containing integral elements, that is, has both 'std::begin' and 'std::end' defined.
      * The number of dimensions is the sum of the sizes of the iterables. For example, if you pass 
      * 'vector<int>{2, 3}, list<long>{4, 5}', a 'Container' with 4 dimensions of sizes 2, 3, 4 and 
      * 5 will be created. Only iterables of integral types are accepted.
      *
      * \param[in] args Variadic iterable types of integrals
    */
    template <class... Args, std::size_t M = Size, cnt::EnableIfZero< M > = 0,
              cnt::EnableIfIterable< std::remove_reference_t< Args >... > = 0>
    Container (const Args&... args) : numDimensions_(0)
    {
    	/** For each iterable we increase the number of dimensions (sum of args.size() for each
    	  * iterable) and insert the dimensions at the end of 'dimSize' 'Vector'.
    	*/
        auto dummy = { (numDimensions_ += args.size(),
                         dimSize.insert(dimSize.end(), std::begin(args), std::end(args)))... };

        weights.resize(numDimensions_);

        initWeights();

        /// Total size is equal to this multiplication. See the 'initWeights' function.
        Base::resize(weights.front() * dimSize.front());
    }


    /** One more constructor defined when 'Size' is 0. In this case, the argument is the starting
      * and ending positions of a iterator. You can also use pointers. If you have for example
      * int v[3] = {4, 1, 7}, and pass it like: 'Container<double> c(v, v+3)', a 'Container' with
      * dimensions of sizes 4, 1 and 7 will be created.
      *
      * \param[in] begin Initial position of the iterator/pointer of integral types
      * \param[in] end Final position of the iterator/pointer of integral types
    */
    template <typename U, typename V, std::size_t M = Size, cnt::EnableIfZero< M > = 0,
              cnt::EnableIfIterator< std::decay_t< U >, std::decay_t< V > > = 0>
    Container (const U& begin, const V& end) : numDimensions_(std::distance(begin, end)), 
                                               dimSize(begin, end),
                                               weights(std::distance(begin, end))
    {
        initWeights();

        /// Total size is equal to this multiplication. See the 'initWeights' function.
        Base::resize(weights.front() * dimSize.front());
    }


    /** A constructor taking a 'std::initializer_list', so you can also construct a
      * container with a single dimension, like that: 'Container<int> c{1, 2, 3}'. The
      * 'Container' in this case will have a single dimension with three elements.
      *
      * \param[in] il Initializer list of type 'T' (same as 'Container')
    */
    template<typename U, std::size_t M = Size, cnt::EnableIfZero< M > = 0,
      		 cnt::EnableIfIntegral<std::decay_t<U>> = 0>
    Container (std::initializer_list<U> il) : Container(il.begin(), il.end()) {}


    /** This function is called from all constructors. It will initialize the 'weights' to 
      * access a given position in the continuous array created either by 'std::vector' or
      * 'std::array' by performing an inner product, given the size of each dimension.
    */
  	void initWeights ()
    {
    	weights.back() = 1;

    	std::partial_sum(dimSize.rbegin() , dimSize.rend() - 1,
    					 weights.rbegin() + 1, std::multiplies<std::size_t>());
    }




// ------------------------------- Access - operator() --------------------------------------------- //



    /** These functions get either a integral type or a iterable of integrals
      * and multiply each element with the iterator 'iter', given by a position
      * in the variable 'weights'. The iterator is incremented, and the value
      * of the multiplication is returned.
      *
      * \param[in] u Either a integral type or a iterable of integrals
      * \param[in] iter A reference to a iterator. 
      * \return Result after multiplication(s).
    */
    //@
    template <typename U, typename Iter, cnt::EnableIfIntegral<std::decay_t<U>> = 0>
    static std::size_t increment (U u, Iter& iter)
    {
    	return *iter++ * u;
    }


    template <typename U, typename Iter, cnt::EnableIfIterable<std::decay_t<U>> = 0>
    static std::size_t increment (const U& u, Iter& iter)
    {
    	std::size_t res = 0;

    	for(auto x : u)
    		res += *iter++ * x;

    	return res;
    }
    //@}



    /** This access operator lets you pass variadic arguments being either integral
      * types or iterables of integral types. The order of the arguments determines
      * the position in each dimension. For example: 'Container<int> c(4, 1, 3);
      * c(vector<long>{1, 0}, 2);' will give you the positions 1, 0 and 2 in the
      * first, second and third dimension, respectivelly.
      *
      * \note The 'Dummie' template stuff is a trick to only allow the call if
      * the arguments are either integral or iterable of integrals types.
      *
      * \param[in] args Either integral types or a iterables of integrals
    */
    //@{
    template <typename... Args>
    const_reference operator () (cnt::IntegralType, const Args&... args) const
    {
        std::size_t pos = 0;

        auto iter = weights.begin();

        const auto& dummy = { (pos += increment(args, iter), int{})... };

        return this->operator[](pos);
    }



    /** Access operator for an iterator defined by the starting position 'begin'.
      * The dimensions to access are defined by the order of the integral elements
      * of the iterator.
      *
      * \param[in] begin Initial position of the iterator/pointer of integral types
    */
    //@{
    template <typename U>
    const_reference operator () (cnt::IteratorType, const U& begin) const
    {
        return this->operator[](std::inner_product(weights.begin(), weights.end(), begin, 0));
    }
    //@}


    /** Access for a 'std::initializer_list' of integral type. You can then access a
      * 'Container' as easily as: 'Container<int, 2, 3, 4> c;  c({1, 2, 3}) = 10'.
      *
      * \param[in] il Initializer list defining the position to access
    */
    //@{
    template <typename U>
    const_reference operator () (std::initializer_list<U> il) const
    {
        return this->operator[](std::inner_product(weights.begin(), weights.end(), il.begin(), 0));
    }
    //@}




    /// Size of each dimension
    constexpr std::size_t size (int p) const { return dimSize[p]; }

    /// Total size
    constexpr std::size_t size ()      const { return Base::size(); }

    constexpr auto sizes ()      	   const { return dimSize; }

    constexpr std::size_t numDimensions () const { return numDimensions_; }






//---------------------------------- Slice ---------------------------------------------- //
    


    /** As the name says, it takes a 'Slice' of the container. If you use for example:
      * 'Container<int, 2, 3, 4> c;   auto slc = c.slice(1);', the variable 'slc' will
      * a proxy to access the container 'c', having two dimensions and starting from
      * position 1 from the first dimension. For more, see the examples.
      *
      * \param[in] args Variadic integral arguments defining the dimensions to 'take a slice'.
    */
    //@{
    template <typename... Args>
    auto slice (const Args&... args) const
    {
        return Accessor<Slice<const Container>>(*this, args...);
    }

    template <typename... Args>
    auto slice (const Args&... args)
    {
        return Accessor<Slice<Container>>(*this, args...);
    }
    //@}



private:


	/// Number of dimensions
   	std::size_t numDimensions_;


    /// The size of each dimension
    Vector<std::size_t, bool(Size) * sizeof...(Is)> dimSize;


    /// The weights to access given the position and sizes of the dimensions
    Vector<std::size_t, bool(Size) * sizeof...(Is)> weights;

};




// ----------------------------------- Accessors -------------------------- //


/** The only purpose of this class is to delegate calls to the accessors of 'Container' 
  * or 'Slice', so we dont have duplication of code and the classes can be written more clearly.
  *
*/
template <class BaseType>
struct Accessor : public BaseType	/// We inherit from either 'Container' or 'Slice'
{
	/** Some type definitions */
    //@{
	using Base = BaseType;

	using Base::Base;


    using value_type = typename Base::value_type;

    using reference = typename Base::reference;

    using const_reference = typename Base::const_reference;
    //@}


    /** These functions simply delegate the access to either 'Container' or 'Slice', which
      * have the same interface for access. They are also responsible to handle the SFINAE
      * to treat all different types of access.
    */
    //@{

    /// Integral or Iterable types
    template <typename... Args, cnt::EnableIfIntegralOrIterable<Args...> = 0>
    const_reference operator () (const Args&... args) const
    {
    	return Base::operator()(cnt::IntegralType{}, args...);
    }

    template <typename... Args, cnt::EnableIfIntegralOrIterable<Args...> = 0>
    reference operator () (const Args&... args)
    {
    	return const_cast<reference>(static_cast<const Accessor&>(*this)(args...));
    }



    /// Iterators
    template <typename U, cnt::EnableIfIterator<std::decay_t<U>> = 0>
    const_reference operator () (const U& begin) const
    {
        return Base::operator()(cnt::IteratorType{}, begin);
    }

    template <typename U, cnt::EnableIfIterator<std::decay_t<U>> = 0>
    reference operator () (const U& begin)
    {
        return const_cast<reference>(static_cast<const Accessor&>(*this)(begin));
    }



    /// Specific for 'std::initializer_list'
    template <typename U, cnt::EnableIfIntegral<std::decay_t<U>> = 0>
    const_reference operator () (std::initializer_list<U> il) const
    {
        return Base::operator()(il);
    }

    template <typename U, cnt::EnableIfIntegral< std::decay_t< U > > = 0 >
    reference operator () (std::initializer_list<U> il)
    {
        return const_cast<reference>(static_cast<const Accessor&>(*this)(il));
    }



    /// For tuples containing integrals or iterables
    template <typename... Args, cnt::EnableIfIntegral<std::decay_t<Args>...> = 0>
    constexpr const_reference operator () (const std::tuple<Args...>& tup) const
    {
        return this->operator()(tup, std::make_index_sequence<sizeof...(Args)>());
    }

    template <typename... Args, cnt::EnableIfIntegral< std::decay_t< Args>...> = 0 >
    constexpr reference operator () (const std::tuple<Args...>& tup)
    {
        return const_cast<reference>(static_cast<const Accessor&>(*this)(tup));
    }

    template <typename... Args, std::size_t... Js>
    constexpr const_reference operator () (const std::tuple<Args...>& tup, std::index_sequence<Js...>) const
    {
        return this->operator()(std::get<Js>(tup)...);
    }
    //@}
};


} // namespace impl



/** These are the classes you will use: the 'Accessor' class over a 'Container' or a 'Slice' */
//@{
template <typename T, std::size_t... Is>
using Container = handy::impl::Accessor<handy::impl::Container<T, Is...>>;

template <typename T, std::size_t... Is>
using Slice = handy::impl::Accessor<handy::impl::Container<T, Is...>>;
//@}




} // namespace handy

#endif  // HANDY_CONTAINER_H
