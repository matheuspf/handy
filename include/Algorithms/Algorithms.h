#ifndef CPPL_ALGORITHMS_H
#define CPPL_ALGORITHMS_H

#include <algorithm>
#include <numeric>
#include "../Helpers/Helpers.h"
#include "../Helpers/HasMember.h"




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


template <class Container, class F>
decltype(auto) operator & (Container&& container, F f)
{
    return f(std::forward<Container>(container));
}




#define SINGLE_CONTAINER_DECLARATION(NAME)                       \
template <class Container1, typename... Args, std::enable_if_t<IsContainer<Container1>::value, int> = 0>     \
decltype(auto) NAME (Container1&& container1, Args&&... args)   \



#define DOUBLE_CONTAINER_DECLARATION(NAME)                                              \
template <class Container1, class Container2, typename... Args, std::enable_if_t<IsContainer<Container2>::value, int> = 0>                         \
decltype(auto) NAME (Container1&& container1, Container2&& container2, Args&&... args)  \




#define CONTAINER_APPLY(NAME, ...)   \
std::NAME( __VA_ARGS__ , std::forward<Args>(args)...)


#define SINGLE_CONTAINER_ITERATORS    \
::std::begin(std::forward<Container1>(container1)),     \
::std::end(std::forward<Container1>(container1))


#define SINGLE_CONTAINER_APPLY(NAME)    \
CONTAINER_APPLY(NAME, SINGLE_CONTAINER_ITERATORS)


#define DOUBLE_CONTAINER_APPLY_1(NAME)            \
CONTAINER_APPLY(NAME, SINGLE_CONTAINER_APPLY(NAME), ::std::begin(std::forward<Container2>(container2)))


#define DOUBLE_CONTAINER_APPLY_2(NAME)     \
CONTAINER_APPLY(NAME, DOUBLE_CONTAINER_APPLY_1(NAME), ::std::end(std::forward<Container2>(container2)))





#define WITH_RETURN(NAME, DECLARATION, APPLY)      \
DECLARATION                                  \
{                                            \
    return APPLY;                             \
}   \
\
MAKE_FUNCTION(NAME)


#define NO_RETURN(NAME, DECLARATION, APPLY, CONTAINER)    \
DECLARATION                                         \
{                                                   \
    APPLY;                                           \
                                                  \
    return CONTAINER;                               \
} \
\
MAKE_FUNCTION(NAME)



#define CONTAINER_1   std::forward<Container1>(container1)

#define CONTAINER_2   std::forward<Container2>(container2)



#define WITH_RETURN_SINGLE_CONTAINER(NAME)  \
WITH_RETURN(NAME, SINGLE_CONTAINER_DECLARATION(NAME), SINGLE_CONTAINER_APPLY(NAME))

#define NO_RETURN_SINGLE_CONTAINER(NAME)   \
NO_RETURN(NAME, SINGLE_CONTAINER_DECLARATION(NAME), SINGLE_CONTAINER_APPLY(NAME), CONTAINER_1)


#define WITH_RETURN_DOUBLE_CONTAINER_1(NAME) \
WITH_RETURN(NAME, DOUBLE_CONTAINER_DECLARATION(NAME), DOUBLE_CONTAINER_APPLY_1(NAME))

#define WITH_RETURN_DOUBLE_CONTAINER_2(NAME) \
WITH_RETURN(NAME, DOUBLE_CONTAINER_DECLARATION(NAME), DOUBLE_CONTAINER_APPLY_2(NAME))



#define NO_RETURN_DOUBLE_CONTAINER_1(NAME) \
NO_RETURN(NAME, DOUBLE_CONTAINER_DECLARATION(NAME), DOUBLE_CONTAINER_APPLY_1(NAME), CONTAINER_1)

#define NO_RETURN_DOUBLE_CONTAINER_2(NAME) \
NO_RETURN(NAME, DOUBLE_CONTAINER_DECLARATION(NAME), DOUBLE_CONTAINER_APPLY_1(NAME), CONTAINER_2)



namespace handy
{

HAS_EXTERN_FUNC(::std::begin, HasBegin)
HAS_EXTERN_FUNC(::std::end, HasEnd)

template <typename T>
struct IsContainer : std::integral_constant<bool, HasBegin<T>::value && HasEnd<T>::value> {};
    


WITH_RETURN_SINGLE_CONTAINER(all_of)
WITH_RETURN_SINGLE_CONTAINER(any_of)
WITH_RETURN_SINGLE_CONTAINER(none_of)

NO_RETURN_SINGLE_CONTAINER(for_each)

WITH_RETURN_SINGLE_CONTAINER(count)
WITH_RETURN_SINGLE_CONTAINER(count_if)

//WITH_RETURN_DOUBLE_CONTAINER_1(mismatch)

WITH_RETURN_DOUBLE_CONTAINER_1(equal)

WITH_RETURN_SINGLE_CONTAINER(find)
WITH_RETURN_SINGLE_CONTAINER(find_if)
WITH_RETURN_SINGLE_CONTAINER(find_if_not)

//WITH_RETURN_SINGLE_CONTAINER(adjacent_find)

//WITH_RETURN_DOUBLE_CONTAINER_1(search)
//WITH_RETURN_SINGLE_CONTAINER(search_n)

NO_RETURN_DOUBLE_CONTAINER_2(copy)
NO_RETURN_DOUBLE_CONTAINER_2(copy_if)
NO_RETURN_DOUBLE_CONTAINER_2(copy_backward)

NO_RETURN_DOUBLE_CONTAINER_2(move)
NO_RETURN_DOUBLE_CONTAINER_2(move_backward)

NO_RETURN_SINGLE_CONTAINER(fill)

NO_RETURN_DOUBLE_CONTAINER_1(transform)

NO_RETURN_SINGLE_CONTAINER(generate)

WITH_RETURN_SINGLE_CONTAINER(remove)
WITH_RETURN_SINGLE_CONTAINER(remove_if)

WITH_RETURN_DOUBLE_CONTAINER_1(remove_copy)
WITH_RETURN_DOUBLE_CONTAINER_1(remove_copy_if)

NO_RETURN_SINGLE_CONTAINER(replace)
NO_RETURN_SINGLE_CONTAINER(replace_if)

WITH_RETURN_DOUBLE_CONTAINER_1(replace_copy)
WITH_RETURN_DOUBLE_CONTAINER_1(replace_copy_if)

NO_RETURN_DOUBLE_CONTAINER_2(swap_ranges)

NO_RETURN_SINGLE_CONTAINER(reverse)

NO_RETURN_DOUBLE_CONTAINER_2(reverse_copy)

NO_RETURN_DOUBLE_CONTAINER_1(rotate)

NO_RETURN_SINGLE_CONTAINER(shuffle)
NO_RETURN_SINGLE_CONTAINER(random_shuffle)

NO_RETURN_SINGLE_CONTAINER(unique)

NO_RETURN_DOUBLE_CONTAINER_2(unique_copy)

WITH_RETURN_SINGLE_CONTAINER(is_partitioned)
WITH_RETURN_SINGLE_CONTAINER(partition)
WITH_RETURN_SINGLE_CONTAINER(stable_partition)
WITH_RETURN_SINGLE_CONTAINER(partition_point)

WITH_RETURN_SINGLE_CONTAINER(is_sorted)
WITH_RETURN_SINGLE_CONTAINER(is_sorted_until)

NO_RETURN_SINGLE_CONTAINER(sort)
NO_RETURN_SINGLE_CONTAINER(stable_sort)

WITH_RETURN_SINGLE_CONTAINER(lower_bound)
WITH_RETURN_SINGLE_CONTAINER(upper_bound)
WITH_RETURN_SINGLE_CONTAINER(binary_search)
WITH_RETURN_SINGLE_CONTAINER(equal_range)

WITH_RETURN_SINGLE_CONTAINER(max_element)
WITH_RETURN_SINGLE_CONTAINER(min_element)
WITH_RETURN_SINGLE_CONTAINER(minmax_element)

WITH_RETURN_DOUBLE_CONTAINER_2(lexicographical_compare)

NO_RETURN_SINGLE_CONTAINER(iota)

WITH_RETURN_SINGLE_CONTAINER(accumulate)



}

// SPECIAL_CASES:  copy_n, fill_n, generate_n, sample, partition_copy, partial_sort_copy, partial_sort
//                 transform*, swap, iter_swap, find_first_of, search, search_n, nth_element
//                 ""Set operations"", ""Heap Operations""


#endif // CPPL_ALGORITHMS_H