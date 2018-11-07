/** @file
 
    @brief An interface to easily access multidimensional data, having total compatibility 
           with STL algorithms and containers

    it is very generic and easy to use multidimensional container, which allows easy creation and access.

    It is aimed to be fast and easy to use.
    
    Also, there is a lot of ways to use it:

    @snippet Container/ContainerExample.cpp Container Snippet
*/

#ifndef HANDY_CONTAINER_H
#define HANDY_CONTAINER_H

#include <tuple>
#include <algorithm>
#include <cmath>
#include <numeric>

#include "Vector.h"
#include "Slice.h"



namespace handy
{

namespace impl
{

template <class>
struct Accessor;


/** @defgroup ContainerGroup Multidimensional Data Container
    @copydoc Container.h
*/
//@{

/** @brief Class to easily create and manipulate multidimensional data. Interacts easily with STL 
           algorithms and can be either statically or dinamically allocated.
  
    @tparam T The Container's type
    @tparam Is The compile time size of each dimension. The total size is the multiplication of these sizes. 
            See the handy::Vector class
*/
template <typename T, std::size_t... Is>
class Container : public Vector<T, cnt::multiply_v<Is...>>
{
public:


    /** @name
        @brief Some type definitions
    */
    //@{
    using Base = Vector<T, cnt::multiply_v<Is...>>;


    using value_type = typename Base::value_type;

    using reference = typename Base::reference;

    using const_reference = typename Base::const_reference;


    using Base::Size;
    //@}



    friend class Slice<Container>;           ///< Friend definition for the 'Slice' class
    friend class Slice<const Container>;     ///< Friend definition for the 'Slice' class




// --------------------------------- Constructors ---------------------------------------------- //


    /** @brief Constructor receiving variadic arguments, defined when inheriting from std::array
      
        Simulates std::array list initialization. The number of dimensions is given by @p Is.

        @params[in] args Variadic arguments. handy::Vector checks if they are of type @c T
    */
    template <typename... Args, std::size_t M = Size, cnt::EnableIfArray< M > = 0>
    Container (Args&&... args) : Base{std::forward<Args>(args)...}, numDimensions_(sizeof...(Is)), dimSize(Is...)
    {
        initWeights();
    }


    /** Constructor receiving variadic arguments, defined when inheriting from std::vector with #Base::Size 
        greater than the maximum stack allocation size. 
        
        In this case, we must resize to #Base::Size after intiallizing with @p args

        @params[in] args Variadic arguments. handy::Vector checks if they are of type @p T.
    */
    template <typename... Args, std::size_t M = Size, std::enable_if_t<( M >= cnt::maxSize ), int > = 0>
    Container (Args&&... args) : Base{std::forward<Args>(args)...}, numDimensions_(sizeof...(Is)), dimSize(Is...)
    {
        initWeights();

        Base::resize(Size);
    }


    /// Empty constructor for the case of <tt>Size == 0</tt> (no compile time size is given)
    template <std::size_t M = Size, cnt::EnableIfZero< M > = 0>
    Container () {}


    /** @brief Constructor for the case when #Size is 0 (inheriting from std::vector)
        
        This time the parameters are integral values that define the size of each dimension. So, <tt>3, 4, 7</tt> 
        would gives us a Container with thre dimensions with sizes <tt>3, 4 and 7</tt>, respectivelly.
      
        @param[in] args Variadic integral types defining the size of each dimension. Only integral types are accepted.
    */
    template <typename... Args, std::size_t M = Size, cnt::EnableIfZero< M > = 0,
              cnt::EnableIfIntegral< std::decay_t< Args >... > = 0 >
    Container (Args... args) : numDimensions_(sizeof...(args)), dimSize{std::size_t(args)...}, weights(sizeof...(args))
    {
        initWeights();

        // Total size is equal to this multiplication. See the initWeights() function.
        Base::resize(weights.front() * dimSize.front());
    }



    /** @brief Another constructor defined when #Size is 0. Each element is an iterable type containing 
               integral elements, that is, has both std::begin() and std::end() defined
    
        The number of dimensions is the sum of the sizes of the iterables. For example, if you pass 
        <tt>vector<int>{2, 3}, list<long>{4, 5}</tt>, a Container with 4 dimensions of sizes <tt>2, 3, 4</tt> and 
        5 will be created. Only iterables of integral types are accepted.
      
        @param[in] args Variadic iterable types of integrals
    */
    template <class... Args, std::size_t M = Size, cnt::EnableIfZero< M > = 0,
              cnt::EnableIfIterable< std::remove_reference_t< Args >... > = 0>
    Container (const Args&... args) : numDimensions_(0)
    {
    	/* For each iterable we increase the number of dimensions (sum of args.size() for each iterable) 
           and insert the dimensions at the end of 'dimSize' 'Vector'. */
        auto dummy = { (numDimensions_ += args.size(),
                         dimSize.insert(dimSize.end(), std::begin(args), std::end(args)))... };

        weights.resize(numDimensions_);

        initWeights();

        // Total size is equal to this multiplication. See the initWeights function.
        Base::resize(weights.front() * dimSize.front());
    }


    /** @brief One more constructor defined when #Size is 0. In this case, the argument is the starting
               and ending positions of a iterator. You can also use pointers. 
        
        If you have for example <tt>int v[3] = {4, 1, 7}</tt>, and pass it like: <tt>Container<double> c(v, v+3)</tt>,
        a Container with dimensions of sizes <tt>4, 1 and 7</tt> will be created

        @param[in] begin Initial position of the iterator/pointer of integral types
        @param[in] end Final position of the iterator/pointer of integral types
    */
    template <typename U, typename V, std::size_t M = Size, cnt::EnableIfZero< M > = 0,
              cnt::EnableIfIterator< std::decay_t< U >, std::decay_t< V > > = 0>
    Container (const U& begin, const V& end) : numDimensions_(std::distance(begin, end)), 
                                               dimSize(begin, end),
                                               weights(std::distance(begin, end))
    {
        initWeights();

        // Total size is equal to this multiplication. See the initWeights() function.
        Base::resize(weights.front() * dimSize.front());
    }


    /** @brief A constructor taking a std::initializer_list
        
        You can also construct a container with a single dimension, like that: <tt>Container<int> c{1, 2, 3}</tt>. 
        The Container in this case will have a single dimension with three elements.
      
        @param[in] il Initializer list of type @p T (same as Container)
    */
    template<typename U, std::size_t M = Size, cnt::EnableIfZero< M > = 0,
      		 cnt::EnableIfIntegral<std::decay_t<U>> = 0>
    Container (std::initializer_list<U> il) : Container(il.begin(), il.end()) {}


    /** @brief Initialize the necessary structures of the Container
      
        This function is called from all constructors. It will initialize the #weights to access a given 
        position in the continuous array created either by std::vector or std::array by performing an inner 
        product, given the size of each dimension
    */
  	void initWeights ()
    {
    	weights.back() = 1;

    	std::partial_sum(dimSize.rbegin() , dimSize.rend() - 1,
    					 weights.rbegin() + 1, std::multiplies<std::size_t>());
    }




// ------------------------------- Access - operator() --------------------------------------------- //


    
    /**  @brief Helpers for access operators 

        Get either a integral type or a iterable of integrals and multiply each element with the iterator 
        @p iter, given by a position in the variable #weights. The iterator is incremented, and the value
        of the multiplication is returned
      
        @param[in] u Either a integral type or a iterable of integrals
        @param[in] iter A reference to a iterator. 
        @return Result after multiplication(s).
    */
    template <typename U, typename Iter, cnt::EnableIfIntegral<std::decay_t<U>> = 0>
    static std::size_t increment (U u, Iter& iter)
    {
    	return *iter++ * u;
    }

    /// @copydoc increment()
    template <typename U, typename Iter, cnt::EnableIfIterable<std::decay_t<U>> = 0>
    static std::size_t increment (const U& u, Iter& iter)
    {
    	std::size_t res = 0;

    	for(auto x : u)
    		res += *iter++ * x;

    	return res;
    }



    /** @brief Access operator for variadic iterables or integrals 
        
        This access operator lets you pass variadic arguments being either integral types or iterables of 
        integral types. The order of the arguments determines the position in each dimension. For example: 

        @code{.cpp}
        Container<int> c(4, 1, 3);

        c(vector<long>{1, 0}, 2);
        @endcode
        
        will give you the positions <tt>1, 0 and 2</tt> in the first, second and third dimension, respectivelly.
      
        @note The @c Dummie template stuff is a trick to only allow the call if he arguments are either integral 
              or iterable of integrals types.
      
        @param[in] args Either integral types or a iterables of integrals
    */
    template <typename... Args>
    const_reference operator () (cnt::IntegralType, const Args&... args) const
    {
        std::size_t pos = 0;

        auto iter = weights.begin();

        const auto& dummy = { (pos += increment(args, iter), int{})... };

        return this->operator[](pos);
    }



    /** @brief Acess operator for iterators
        
        Access operator for an iterator defined by the starting position @p begin
        
        The dimensions to access are defined by the order of the integral elements of the iterator

        @code{.cpp}
        Container<int, 2, 3, 4> c;

        std::vector<int> v = {1, 2, 3};

        c(v.begin()) = 10;
        @endcode
      
        @param[in] begin Initial position of the iterator/pointer of integral types
    */
    template <typename U>
    const_reference operator () (cnt::IteratorType, const U& begin) const
    {
        return this->operator[](std::inner_product(weights.begin(), weights.end(), begin, 0));
    }


    /** @brief Acess operator for std::initializer_list of integral type

        You can access a Container as easily as: 
        
        @code{.cpp}
        Container<int, 2, 3, 4> c;  
        
        c({1, 2, 3}) = 10'.
        @endcode
      
        @param[in] il Initializer list defining the position to access
    */
    template <typename U>
    const_reference operator () (std::initializer_list<U> il) const
    {
        return this->operator[](std::inner_product(weights.begin(), weights.end(), il.begin(), 0));
    }




    /// Size of each dimension
    constexpr std::size_t size (int p) const { return dimSize[p]; }

    /// Total size
    constexpr std::size_t size ()      const { return Base::size(); }

    /// Sizes of each dimension
    constexpr auto sizes ()      	   const { return dimSize; }

    /// Number of dimensions
    constexpr std::size_t numDimensions () const { return numDimensions_; }






//---------------------------------- Slice ---------------------------------------------- //
    


    /** @brief As the name says, it takes a 'Slice' of the container

        If you use for example:
        
        @code{.cpp}
        Container<int, 2, 3, 4> c;
        
        auto slc = c.slice(1);
        @endcode
        
        the variable @c slc will be a proxy to access the container @c c, having two dimensions and 
        starting from position 1 from the first dimension. For more, see the examples.
      
        @param[in] args Variadic integral arguments defining the dimensions to 'take a slice'.
    */
    template <typename... Args>
    auto slice (const Args&... args) const
    {
        return Accessor<Slice<const Container>>(*this, args...);
    }

    /// @copydoc slice()
    template <typename... Args>
    auto slice (const Args&... args)
    {
        return Accessor<Slice<Container>>(*this, args...);
    }


protected:


	/// Number of dimensions
   	std::size_t numDimensions_;


    /// The size of each dimension
    Vector<std::size_t, bool(Size) * sizeof...(Is)> dimSize;


    /// The weights to access given the position and sizes of the dimensions
    Vector<std::size_t, bool(Size) * sizeof...(Is)> weights;

};




// ----------------------------------- Accessors -------------------------- //


/** @brief Delegate the call to the accessors of Container or Slice
    
    The only purpose of this class is to delegate calls to the accessors of Container or Slice, 
    so we dont have duplication of code and the classes can be written more clearly

    @tparam BaseType Either a Container or a Slice class
*/
template <class BaseType>
struct Accessor : public BaseType
{
	/** @name
    @brief Some type definitions
    */
    //@{
	using Base = BaseType;

	using Base::Base;


    using value_type = typename Base::value_type;

    using reference = typename Base::reference;

    using const_reference = typename Base::const_reference;
    //@}


    /** @name
        @brief Delegate the call to the right access function

        These functions simply delegate the access to either Container or Slice, which have the same 
        interface for access 
        
        They are also responsible to handle SFINAE to treat all different types of access.
    */
    //@{
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



    /** @name
        @brief Access for tuples

        These are special accessors defined for std::tuple.

        The tuples are unpacked and given as argument to the other delegating operators
    */
    //@{
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
    //@}
};


} // namespace impl



/** @name
    @brief These are the classes you will use: the Accessor class over a Containe' or a Slice
*/
//@{
/// An alias defining an accessor to Container
template <typename T, std::size_t... Is>
using Container = handy::impl::Accessor<handy::impl::Container<T, Is...>>;

/// An alias defining an accessor to Slice
template <typename T, std::size_t... Is>
using Slice = handy::impl::Accessor<handy::impl::Container<T, Is...>>;
//@}

//@}



} // namespace handy

#endif  // HANDY_CONTAINER_H
