/** 
    @file
    
    @brief Simple facilities to iterate through multiple containers and iterators at the same time, 
           similar to Python's zip. Works easily with stl algorithms as well.
    
 */



#ifndef HANDY_ZIP_ITER_H
#define HANDY_ZIP_ITER_H

#include "Helpers.h"




namespace handy
{


/** @defgroup ZipIterGroup Iterator Zipper
    @copydoc ZipIter.h
*/

//@{
    
/** @brief Main iterator class
    
    @tparam T First iterator type. Separated from the variadic args to do some checks and to assure that
              at least 1 parameter is given as argument

    @tparam Iters The variadic iterator types
    
    
    Inherits from std::iterator, being of the most generic std::iterator_category from all of its arguments.
    
    The value type is a std::tuple of the @c value_type's of all the arguments.
    
    The basic iterator interface is implemented, while some functions are only alowed if all the iterator 
    parameters meet some requirements.

    For example, the @c + and @c - operators are defined only for random access operators.
*/
template <typename T, typename... Iters>
class ZipIter : public impl::zip::IteratorBase<T, std::remove_reference_t< Iters >...>
{
public:

        /// The base class of ZipIter. See impl::zip::IteratorBase
        using Base = impl::zip::IteratorBase<T, std::remove_reference_t< Iters >...>;


        /** @name
            @brief Some type definitions defined over the std::iterator base
        */
        //@{
        using iters_type = std::tuple<T, std::remove_reference_t< Iters >...>;

        using value_type      = typename Base::value_type;
        using reference       = typename Base::reference;
        using difference_type = typename Base::difference_type;

        using iterator_category = typename Base::iterator_category;
        //@}



        /// A single constructor taking the iterators by value, so no modification is done to  the real iterators
        ZipIter (T t, Iters... iterators) : iters ( t, iterators... ) {}




        /** @name
            
            @brief Function operators
            
            All the following operators simply apply a function to every member of the tuple of iterators. 
            Some of the methods are disabled if some iterator does not meet all the requirements.
        */
        //@{
        ZipIter& operator ++ ()
        {
            applyTuple(impl::zip::increment, iters); return *this;
        }

        ZipIter operator ++ (int)
        {
            ZipIter temp{*this};

            operator++();

            return temp;
        }


        template <class Tag = iterator_category, impl::zip::EnableIfMinimumTag< Tag, std::bidirectional_iterator_tag > = 0 >
        ZipIter& operator -- ()
        {
            applyTuple(impl::zip::decrement, iters); return *this;
        }

        template <class Tag = iterator_category, impl::zip::EnableIfMinimumTag< Tag, std::bidirectional_iterator_tag > = 0 >
        ZipIter operator -- (int)
        {
            ZipIter temp{ *this };

            operator--();

            return temp;
        }


        template <class Tag = iterator_category, impl::zip::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
        ZipIter& operator += (int inc)
        {
          applyTuple(impl::zip::add, iters, inc);

          return *this;
        }

        template <class Tag = iterator_category, impl::zip::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
        ZipIter& operator -= (int inc)
        {
            applyTuple(impl::zip::add, iters, -inc);

            return *this;
        }


        template <typename U, typename... Args>
		friend auto operator+ (ZipIter<U, Args...>, int);

		template <typename U, typename... Args>
		friend auto operator- (ZipIter<U, Args...>, int);

		template <typename U, typename... Args>
		friend auto operator+ (const ZipIter<U, Args...>&, const ZipIter<U, Args...>&);

		template <typename U, typename... Args>
		friend auto operator- (const ZipIter<U, Args...>&, const ZipIter<U, Args...>&);


		template <typename U, typename... Args>
		friend bool operator == (const ZipIter<U, Args...>&, const ZipIter<U, Args...>&);

		template <typename U, typename... Args>
		friend bool operator < (const ZipIter<U, Args...>&, const ZipIter<U, Args...>&);
        //@}




        /** @name
            @brief Derreferencing operator
            
            Delegating. I dont implement 'operator->' because the return of dereferencing is a temporary, 
            and because it is almost not used (not by any stl function I now).
        */
        //@{
        decltype(auto) operator * ()
        {
        	return dereference( std::make_index_sequence< sizeof... (Iters) + 1 >() );
        }

        decltype(auto) operator * () const
        {
        	return dereference( std::make_index_sequence< sizeof... (Iters) + 1 >() );
        }
        //@}



private:



    /** @name
        @brief Here the tuple of value is returned as a temporary to avoid any extra extorage or access
    */
    //@{
    template <std::size_t... Is>
    auto dereference (std::index_sequence<Is...>)
    {
        return std::forward_as_tuple( *std::get< Is >( iters )... );
    }

    template <std::size_t... Is>
    auto dereference (std::index_sequence<Is...>) const
    {
        return std::forward_as_tuple( *std::get< Is >( iters )... );
    }
    //@}



    iters_type iters; ///< The tuple of iterators

};


/** @name
    @brief Definition of some operators

    @note The distance and comparison between operators are based on the first argument only
*/
//@{

template <typename T, typename... Iters>
inline auto operator+ (ZipIter<T, Iters...> iter, int inc)
{
	iter += inc;

	return iter;
}

template <typename T, typename... Iters>
inline auto operator- (ZipIter<T, Iters...> iter, int inc)
{
	iter -= inc;

	return iter;
}


template <typename T, typename... Iters>
inline auto operator+ (const ZipIter<T, Iters...>& iter1, const ZipIter<T, Iters...>& iter2)
{
	return std::get<0>(iter1.iters) + std::get<0>(iter2.iters);
}

template <typename T, typename... Iters>
inline auto operator- (const ZipIter<T, Iters...>& iter1, const ZipIter<T, Iters...>& iter2)
{
	return std::get<0>(iter1.iters) - std::get<0>(iter2.iters);
}


/// Comparisons. Only 'operator==' and 'operator<' are defined
template <typename T, typename... Iters>
inline bool operator== (const ZipIter<T, Iters...>& iter1, const ZipIter<T, Iters...>& iter2)
{
	return std::get<0>(iter1.iters) == std::get<0>(iter2.iters);
}

template <typename T, typename... Iters>
inline bool operator!= (const ZipIter<T, Iters...>& iter1, const ZipIter<T, Iters...>& iter2)
{
	return !operator==(iter1, iter2);
}


template <typename T, typename... Iters>
inline bool operator< (const ZipIter<T, Iters...>& iter1, const ZipIter<T, Iters...>& iter2)
{
	return std::get<0>(iter1.iters) < std::get<0>(iter2.iters);
}

template <typename T, typename... Iters>
inline bool operator> (const ZipIter<T, Iters...>& iter1, const ZipIter<T, Iters...>& iter2)
{
	return operator<(iter2, iter1);
}

template <typename T, typename... Iters>
inline bool operator<= (const ZipIter<T, Iters...>& iter1, const ZipIter<T, Iters...>& iter2)
{
	return !operator>(iter2, iter1);
}

template <typename T, typename... Iters>
inline bool operator>= (const ZipIter<T, Iters...>& iter1, const ZipIter<T, Iters...>& iter2)
{
	return !operator<(iter2, iter1);
}
//@}








/** @brief Zip class
    
    @tparam Containers The variadic container types
    
    This class servers mainly as a wrapper for iterating in the for range loop.
    
    It is composed of a tuple of references to containers that are iterable, and exposes a #begin and #end# 
    methods, returning a ZipIter with the iterators of each container passed as argument.
*/
template <typename... Containers>
class Zip
{
public:


    /** @name 
        @brief Some type definitions
    */
    //@{
    using value_type = std::tuple < Containers... >;

    using iterator = ZipIter < typename impl::zip::Iterable<std::remove_reference_t<Containers>>::iterator... >;

    using iterator_category = typename iterator::iterator_category;
    //@}


    /** It does not make sense to create a const Zip, so I define this guy like this.
        The containers, on the other hand, can be const, so the #const_iterator are call instead by impl::zip::Iterable
    */
    using const_iterator = iterator;
    
    /// Number of containers
    static constexpr std::size_t containersSize = sizeof... (Containers);


    /** @brief A single constructor taking parameters by value
        
        @note Notice here that the @p Containers are all references, so is #value_type a std::tuple
              of references
    */
    Zip (Containers... containers) : containers( containers... ) {}



    /** @name
        @brief begin and end methods
    */
    //@{
    iterator begin () { return begin( std::make_index_sequence<containersSize>() ); }

    const_iterator begin () const { return begin( std::make_index_sequence<containersSize>() ); }


    iterator end () { return end( std::make_index_sequence<containersSize>() ); }

    const_iterator end () const { return end( std::make_index_sequence<containersSize>() ); }
    //@}


    /// This is simply a facility for acessing random access containers, returning a tuple of values
    template <class Tag = iterator_category, impl::zip::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
    auto operator [] (std::size_t pos)
    {
        return at( pos, std::make_index_sequence<containersSize>() );
    }

    /// @copydoc #operator[]()
    template <class Tag = iterator_category, impl::zip::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
    auto operator [] (std::size_t pos) const
    {
        return at( pos, std::make_index_sequence<containersSize>() );
    }


    /// The size of the first element defines the range
    std::size_t size () const { return std::get<0>( containers ).size(); }  



private:


    /** @name 
        @brief The actual implementations
    */
    //@{
    template <std::size_t... Is>
    iterator begin (std::index_sequence<Is...>)
    {
        return iterator( impl::zip::begin( std::get<Is>( containers ) )... );
    }

    template <std::size_t... Is>
    const_iterator begin (std::index_sequence<Is...>) const
    {
        return const_iterator( impl::zip::begin( std::get<Is>( containers ) )... );
    }

    template <std::size_t... Is>
    iterator end (std::index_sequence<Is...>)
    {
        return iterator( impl::zip::end( std::get<Is>( containers ) )... );
    }

    template <std::size_t... Is>
    const_iterator end (std::index_sequence<Is...>) const
    {
        return const_iterator( impl::zip::end( std::get<Is>( containers ) )... );
    }

    template <std::size_t... Is,
              class Tag = iterator_category,impl::zip::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
    auto at (std::size_t pos, std::index_sequence<Is...>)
    {
        return std::forward_as_tuple( containers[ Is ]... );
    }

    template <std::size_t... Is,
              class Tag = iterator_category, impl::zip::EnableIfMinimumTag< Tag, std::random_access_iterator_tag > = 0 >
    auto at (std::size_t pos, std::index_sequence<Is...>) const
    {
        return std::forward_as_tuple( containers[ Is ]... );
    }
    //@}




    value_type containers; ///< std::tuple of references to containers

};





/** @brief Delegating function to handy::ZipIter

    These are the functions that will actually be called instead of initializing the classes with 
    cumbersome types.
    
    I used the first type separatelly because it is easier to defined constraints (the first element 
    of a container cannot be a pointer), and because it forces the call with at least 1 element
*/
template <typename T, typename... Iterators>
auto zipIter (T&& t, Iterators&&... iterators)
{
    return ZipIter<T, Iterators...>(std::forward<T>(t), std::forward<Iterators>(iterators)...);
}

/** @brief Delegating function to handy::Zip
    @copydetails zipIter()
*/
template <typename T, typename... Containers, std::enable_if_t< !std::is_pointer< T >::value, int > = 0>
auto zip (T&& t, Containers&&... containers)
{
    return Zip<T, Containers...>(std::forward<T>(t), std::forward<Containers>(containers)...);
}




/** @brief Call handy::zipIter() with the begin of each container argument
    
    These are facilities for calling handy::zipIter() more easily. 
    
    You can simply pass a container (or a pointer) with a defined std::begin or std::end and it will
    call the proper function.
*/
template <typename T, typename... Containers, std::enable_if_t< !std::is_pointer< T >::value, int > = 0>
auto zipBegin (T&& t, Containers&&... containers)
{
    return zipIter(impl::zip::begin(std::forward<T>(t)), impl::zip::begin(std::forward<Containers>(containers))...);
}

/** @brief Call handy::zipIter() with the end of each container argument
    @copydetails zipBegin()
*/
template <typename T, typename... Containers, std::enable_if_t< !std::is_pointer< T >::value, int > = 0>
auto zipEnd (T&& t, Containers&&... containers)
{
    return zipIter(impl::zip::end(std::forward<T>(t)), impl::zip::end(std::forward<Containers>(containers))...);
}

/** @brief Call handy::zipIter() with the begin and end of each container argument, returning a std::pair of handy::zipIter()
    @copydetails zipBegin()
*/
template <typename T, typename... Containers, std::enable_if_t< !std::is_pointer< T >::value, int > = 0>
auto zipAll (T&& t, Containers&&... containers)
{
    return std::make_pair(zipBegin(std::forward<T>(t), std::forward<Containers>(containers)...),
                          zipEnd  (std::forward<T>(t), std::forward<Containers>(containers)...));
}






/** @brief Utility for @c unzipping arguments
    
    @tparam Apply The user defined function to apply to the expanded arguments

    This class has a single method that takes variadic arguments including tuples and expand all of them, 
    passing to a function.
  
    This way you can handle the elements of a tuple separatelly. It is very useful while iterating in the 
    for range loops and in stl functions.
*/
template <class Apply>
struct UnZip
{
    /// A single constructor taking a function as argument
    UnZip (const Apply& apply = Apply()) : apply(apply) {}



    /// These functions expand the arguments and aplly the function, using some helpers
    template <typename... Args>
    decltype(auto) operator () (Args&&... args)
    {
        return operator()( std::make_index_sequence< impl::zip::CountElements< std::decay_t< Args >... >::value >(),
                           std::forward< Args >( args )...);
    }
    
    /// @copybrief #operator()()
    template <std::size_t... Is, typename... Args>
    decltype(auto) operator () (std::index_sequence< Is... >, Args&&... args)
    {
        return apply( std::get< Is >( impl::zip::packArgs( std::forward< Args >( args )... ) )... );
    }


    Apply apply;  ///< The function to apply to the expanded arguments
};




/** @brief Delegating function to handy::UnZip
   
    As in the 'zipIter' and 'zip' cases, this function is much easier than to call than to instantiate 
    the class. 
    
    The first function gets only a function as parameter, and is intended to use in stl functions.
    
    The other two are for the for range loop, and gets a tuple as parameter as well.
*/
//@{
template <class F>
auto unZip (F f)
{
    return UnZip<F>(f);
}

template <class Tuple, class Function, std::size_t... Is>
decltype(auto) unZip (Tuple&& tup, Function function, std::index_sequence<Is...>)
{
    return function( std::get< Is >( std::forward<Tuple>(tup) )... );
}

template <class Tuple, class Function>
decltype(auto) unZip (Tuple&& tup, Function function)
{
    return unZip(std::forward<Tuple>(tup), function, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>());
}
//@}





/** @brief For range loop 
    
    This function makes a call to the for range loop unpacking the parameters with the handy::unZip() 
    function.
    
    A thing to notice is that the function is actually the first parameter of the variadic arguments.
    
    The order is changed with the handy::reverseArgs() function, with zero runtime overhead (with at least -O2)
  */
template <typename... Args>
void forEach (Args&&... args)
{
    reverseArgs<sizeof...(Args)-1>([](auto apply, auto&&... elems)
    {
        for(auto&& tup : zip(std::forward<decltype(elems)>(elems)...))
        {
            unZip(std::forward<decltype(tup)>(tup), apply);
        }

    }, std::forward<Args>(args)...);
}


} // namespace handy

//@}



#endif	// HANDY_ZIP_ITER_H
