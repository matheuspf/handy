/** @file

    @brief Define STL container algorithms and pipeline operators

    @details By including this class, for almost any stl algorithm, a function taking a container
             instead of iterators is defined.
             
             This function simply delegate the call to the actual stl function by calling std::begin 
             and std::end on the container passed as parameter.
            
             The exact form of the function, as well as the number of arguments depends on the stl 
             algorithm. For each begin/end pair there is one container. For single iterators, such as
             the third argument to std::copy, there is also one container. Any other paramater of the
             stl algorithm can also be passed as parameter to the @ref handy version. Example:

    @snippet Algorithms/AlgorithmsExample.cpp Algorithms Snipet functions

    @details Notice that the return is also modified for some functions. For stl functions that do not
             return, a reference to the container that is being modified is returned. For functions that
             return an iterator that does not imply position (std::transform, for example), the return is
             also the container that is being modified. Any other case has the same return as the stl algorithm.

             Another feature is the function pipeline, which can be controled by changing the ALGORITHM_OP_SYMBOL().
             
             By passing only the arguments to the @ref handy algorithm function, a lambda is returned,
             taking the missing container argument.

             It is easy then to construct a pipeline of operations (starting at the left). Also, the order
             of the parameters is irrelevant, as long as a single container is given. Example:

    @snippet Algorithms/AlgorithmsExample.cpp Algorithms Snipet pipeline

    
    @note For many functions the overloads are not ambiguous to the original stl functions, so there
          is no need to use the @ref handy namespace to disambiguate the call.
*/

#ifndef HANDY_ALGORITHMS_H
#define HANDY_ALGORITHMS_H

#include "../Helpers/Helpers.h"
#include "../Helpers/HasMember.h"

#include <algorithm>
#include <numeric>


/** @defgroup AlgorithmsGroup STL container algorithms
    @copydoc Algorithms.h
*/

//@{
/** @brief Symbol that defines the operation pipeline
    @note You can change it to any binary operator you want.
*/
#define ALGORITHM_OP_SYMBOL &


/** @brief Generates functions that operate on the function pipeline
    @details Takes a stl algorithm (@c FUNCTION) and generates a function returning a lambda.
             This lambda takes as parameter a container and apply @c FUNCTION along with
             @c args...
             
*/
#define MAKE_FUNCTION(FUNCTION)                                                 \
template <typename... Args>                                                      \
decltype(auto) FUNCTION (Args&&... args)                                         \
{                                                                               \
    return [&](auto&& container) -> decltype(auto)                               \
            {                                                                    \
                return FUNCTION (std::forward<decltype(container)>(container),   \
                            std::forward<Args>(args)...);                        \
            };                                                                   \
}


/** @name
    @brief Macros used for generating code. Internal usage.
*/
//@{
#define SINGLE_CONTAINER_DECLARATION(NAME)                       \
template <class Container1, typename... Args, std::enable_if_t<IsContainer<Container1>::value>* = nullptr>     \
decltype(auto) NAME (Container1&& container1, Args&&... args)   \


#define DOUBLE_CONTAINER_DECLARATION(NAME)                                              \
template <class Container1, class Container2, typename... Args, std::enable_if_t<IsContainer<Container2>::value>* = nullptr>                         \
decltype(auto) NAME (Container1&& container1, Container2&& container2, Args&&... args)  \


#define SINGLE_CONTAINER_DECLARATION_N(NAME)    \
template <class Container1, typename... Args, std::enable_if_t<IsContainer<Container1>::value>* = nullptr>     \
decltype(auto) NAME (Container1&& container1, int n, Args&&... args)   \


#define DOUBLE_CONTAINER_DECLARATION_N(NAME)                                              \
template <class Container1, class Container2, typename... Args, std::enable_if_t<IsContainer<Container2>::value>* = nullptr>                         \
decltype(auto) NAME (Container1&& container1, int n, Container2&& container2, Args&&... args)  \



#define CONTAINER_1   std::forward<Container1>(container1)
#define CONTAINER_2   std::forward<Container2>(container2)

#define FUNC_VAR_ARGS std::forward<Args>(args)...





#define CONTAINER_APPLY(NAME, ...)   \
::std::NAME( __VA_ARGS__ , FUNC_VAR_ARGS)


#define SINGLE_CONTAINER_APPLY(NAME)    \
CONTAINER_APPLY(NAME, ::std::begin(CONTAINER_1), ::std::end(CONTAINER_1))


#define DOUBLE_CONTAINER_APPLY_SINGLE(NAME)    \
CONTAINER_APPLY(NAME, ::std::begin(CONTAINER_1), ::std::end(CONTAINER_1), ::std::begin(CONTAINER_2))


#define DOUBLE_CONTAINER_APPLY_DOUBLE(NAME)    \
CONTAINER_APPLY(NAME, ::std::begin(CONTAINER_1), ::std::end(CONTAINER_1), ::std::begin(CONTAINER_2), ::std::end(CONTAINER_2))




#define APPLY_CONTAINER_FUNCTION(NAME, DECLARATION, ...)  \
DECLARATION(NAME) { __VA_ARGS__ } MAKE_FUNCTION(NAME)


#define WITH_RETURN(NAME, DECLARATION, APPLY)   \
APPLY_CONTAINER_FUNCTION(NAME, DECLARATION, return APPLY(NAME);)

#define NO_RETURN(NAME, DECLARATION, APPLY, CONTAINER)   \
APPLY_CONTAINER_FUNCTION(NAME, DECLARATION, APPLY(NAME); return CONTAINER;)
//@}



namespace handy
{

HAS_EXTERN_FUNC(::std::begin, HasBegin)
HAS_EXTERN_FUNC(::std::end, HasEnd)

/// Tells if a type @p T is a container - that is, has a specialization for both std::begin and std::end
template <typename T>
struct IsContainer : std::integral_constant<bool, HasBegin<T>::value && HasEnd<T>::value> {};



/** @class AlgOPDoc
    @brief Applies a function @p f to the @c container class
 
    @param container A universal reference to a container that will be operand of @p f
    @param f A function that takes a container as its first argument
    @return The result of @p f(Container)
*/

/// @copydoc AlgOPDoc
template <class Container, class F, std::enable_if_t<IsContainer<Container>::value>* = nullptr>
decltype(auto) operator ALGORITHM_OP_SYMBOL (Container&& container, F f)
{
    return f(std::forward<Container>(container));
}

/// @copydoc AlgOPDoc
template <class Container, class F, std::enable_if_t<IsContainer<Container>::value>* = nullptr>
decltype(auto) operator ALGORITHM_OP_SYMBOL (F f, Container&& container)
{
    return f(std::forward<Container>(container));
}
//@}
    


WITH_RETURN(all_of, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(any_of, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(none_of, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

NO_RETURN(for_each, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

WITH_RETURN(count, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(count_if, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(mismatch, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

WITH_RETURN(equal, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

WITH_RETURN(find, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(find_if, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(find_if_not, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(find_end, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_DOUBLE)

WITH_RETURN(find_first_of, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_DOUBLE)

WITH_RETURN(adjacent_find, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(search, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_DOUBLE)

WITH_RETURN(search_n, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

NO_RETURN(copy, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE, CONTAINER_2)
NO_RETURN(copy_if, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE, CONTAINER_2)
NO_RETURN(copy_backward, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE, CONTAINER_2)

APPLY_CONTAINER_FUNCTION(copy_n, DOUBLE_CONTAINER_DECLARATION_N,
                         ::std::copy_n(::std::begin(CONTAINER_1), n, ::std::begin(CONTAINER_2)); return CONTAINER_2;)


NO_RETURN(move, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE, CONTAINER_2)
NO_RETURN(move_backward, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE, CONTAINER_2)

NO_RETURN(fill, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

APPLY_CONTAINER_FUNCTION(fill_n, SINGLE_CONTAINER_DECLARATION_N,
                         ::std::fill_n(std::begin(CONTAINER_1), n, FUNC_VAR_ARGS); return CONTAINER_1;)


NO_RETURN(transform, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE, CONTAINER_2)

NO_RETURN(generate, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

APPLY_CONTAINER_FUNCTION(generate_n, SINGLE_CONTAINER_DECLARATION_N,
                         ::std::generate_n(std::begin(CONTAINER_1), n, FUNC_VAR_ARGS); return CONTAINER_1;)

WITH_RETURN(remove, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(remove_if, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(remove_copy, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)
WITH_RETURN(remove_copy_if, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

NO_RETURN(replace, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)
NO_RETURN(replace_if, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

WITH_RETURN(replace_copy, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)
WITH_RETURN(replace_copy_if, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

NO_RETURN(swap_ranges, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE, CONTAINER_2)

NO_RETURN(reverse, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

NO_RETURN(reverse_copy, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE, CONTAINER_2)

APPLY_CONTAINER_FUNCTION(rotate, SINGLE_CONTAINER_DECLARATION_N,
          ::std::rotate(::std::begin(CONTAINER_1), ::std::begin(CONTAINER_1) + n, ::std::end(CONTAINER_1)); return CONTAINER_1;)

APPLY_CONTAINER_FUNCTION(rotate_copy, DOUBLE_CONTAINER_DECLARATION_N,
          ::std::rotate(::std::begin(CONTAINER_1), ::std::begin(CONTAINER_1) + n,
                        ::std::end(CONTAINER_1), ::std::begin(CONTAINER_2)); return CONTAINER_2;)

NO_RETURN(random_shuffle, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)
NO_RETURN(shuffle, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

WITH_RETURN(unique, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(unique_copy, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

WITH_RETURN(is_partitioned, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(partition, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(stable_partition, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(partition_point, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(is_sorted, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(is_sorted_until, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

NO_RETURN(sort, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

APPLY_CONTAINER_FUNCTION(partial_sort, SINGLE_CONTAINER_DECLARATION_N,
          ::std::partial_sort(::std::begin(CONTAINER_1), ::std::begin(CONTAINER_1) + n, ::std::end(CONTAINER_1));
          return CONTAINER_1;)

APPLY_CONTAINER_FUNCTION(partial_sort_copy, DOUBLE_CONTAINER_DECLARATION_N,
          ::std::partial_sort_copy(::std::begin(CONTAINER_1), ::std::begin(CONTAINER_1),
                                   ::std::begin(CONTAINER_2), ::std::end(CONTAINER_2), FUNC_VAR_ARGS);
          return CONTAINER_1;)

NO_RETURN(stable_sort, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

APPLY_CONTAINER_FUNCTION(nth_element, SINGLE_CONTAINER_DECLARATION_N,
          ::std::nth_element(::std::begin(CONTAINER_1), ::std::begin(CONTAINER_1) + n, ::std::end(CONTAINER_1));
          return CONTAINER_1;)

WITH_RETURN(lower_bound, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(upper_bound, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(binary_search, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(equal_range, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

APPLY_CONTAINER_FUNCTION(inplace_merge, SINGLE_CONTAINER_DECLARATION_N,
          ::std::inplace_merge(::std::begin(CONTAINER_1), ::std::begin(CONTAINER_1) + n, ::std::end(CONTAINER_1));
          return CONTAINER_1;)

WITH_RETURN(includes, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_DOUBLE)

WITH_RETURN(is_heap, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(is_heap_until, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

NO_RETURN(make_heap, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)
NO_RETURN(push_heap, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)
NO_RETURN(pop_heap, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)
NO_RETURN(sort_heap, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

WITH_RETURN(max_element, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(min_element, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(minmax_element, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(lexicographical_compare, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_DOUBLE)

WITH_RETURN(is_permutation, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(next_permutation, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)
WITH_RETURN(prev_permutation, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

NO_RETURN(iota, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY, CONTAINER_1)

WITH_RETURN(accumulate, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

WITH_RETURN(inner_product, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

WITH_RETURN(adjacent_difference, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

WITH_RETURN(partial_sum, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

// WITH_RETURN(reduce, SINGLE_CONTAINER_DECLARATION, SINGLE_CONTAINER_APPLY)

// WITH_RETURN(exclusive_scan, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)
// WITH_RETURN(inclusive_scan, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

// WITH_RETURN(transform_reduce, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

// WITH_RETURN(transform_exclusive_scan, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)
// WITH_RETURN(transform_inclusive_scan, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)

// WITH_RETURN(uninitialized_copy, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)
// WITH_RETURN(uninitialized_copy, DOUBLE_CONTAINER_DECLARATION, DOUBLE_CONTAINER_APPLY_SINGLE)











}



#undef ALGORITHM_OP_SYMBOL

#undef MAKE_FUNCTION

#undef SINGLE_CONTAINER_DECLARATION

#undef DOUBLE_CONTAINER_DECLARATION

#undef SINGLE_CONTAINER_DECLARATION_N

#undef DOUBLE_CONTAINER_DECLARATION_N

#undef CONTAINER_1

#undef CONTAINER_2

#undef FUNC_VAR_ARGS

#undef CONTAINER_APPLY

#undef SINGLE_CONTAINER_APPLY

#undef DOUBLE_CONTAINER_APPLY_SINGLE

#undef DOUBLE_CONTAINER_APPLY_DOUBLE

#undef APPLY_CONTAINER_FUNCTION

#undef WITH_RETURN

#undef NO_RETURN







// SPECIAL_CASES: transform* -> 3 containers specific overload
//                sample -> C++17
//                parition_copy, merge, set_difference, set_intersection, set_symmetric_difference, set_union -> 3 containers 



#endif // HANDY_ALGORITHMS_H