#ifndef CPPL_STL_CONTAINER_H
#define CPPL_STL_CONTAINER_H

#include <algorithm>


#define SINGLE_CONTAINER_DECLARATION(NAME)                      \
template <class Container1, typename... Args>                    \
decltype(auto) NAME (Container1&& container1, Args&&... args)



#define DOUBLE_CONTAINER_DECLARATION(NAME)                                              \
template <class Container1, class Container2, typename... Args>                         \
decltype(auto) NAME (Container1&& container1, Container2&& container2, Args&&... args)




#define SINGLE_CONTAINER_APPLY(NAME)                            \
::std::NAME(std::begin(std::forward<Container1>(container1)),     \
            std::end(std::forward<Container1>(container1)),       \
            std::forward<Args>(args)...);



#define DOUBLE_CONTAINER_APPLY_SINGLE_ITERATOR(NAME)            \
::std::NAME(std::begin(std::forward<Container1>(container1)),   \
            std::end(std::forward<Container1>(container1)),     \
            std::begin(std::forward<Container2>(container2)),   \
            std::forward<Args>(args)...);



#define DOUBLE_CONTAINER_APPLY_DOUBLE_ITERATOR(NAME)            \
::std::NAME(std::begin(std::forward<Container1>(container1)),   \
            std::end(std::forward<Container1>(container1)),     \
            std::begin(std::forward<Container2>(container2)),   \
            std::end(std::forward<Container2>(container2)),     \
            std::forward<Args>(args)...);





#define WITH_RETURN(DECLARATION, APPLY)      \
DECLARATION                                  \
{                                            \
    return APPLY                             \
}

#define NO_RETURN(DECLARATION, APPLY, CONTAINER)    \
DECLARATION                                         \
{                                                   \
    APPLY                                           \
                                                    \
    return CONTAINER;                               \
} 


#define RETURN_CONTAINER1   std::forward<Container1>(container1)

#define RETURN_CONTAINER2   std::forward<Container2>(container2)



#define WITH_RETURN_SINGLE_CONTAINER(NAME)  \
WITH_RETURN(SINGLE_CONTAINER_DECLARATION(NAME), SINGLE_CONTAINER_APPLY(NAME))

#define NO_RETURN_SINGLE_CONTAINER(NAME)   \
NO_RETURN(SINGLE_CONTAINER_DECLARATION(NAME), SINGLE_CONTAINER_APPLY(NAME), RETURN_CONTAINER1)



#define WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(NAME) \
WITH_RETURN(DOUBLE_CONTAINER_DECLARATION(NAME), DOUBLE_CONTAINER_APPLY_SINGLE_ITERATOR(NAME))

#define WITH_RETURN_DOUBLE_CONTAINER_DOUBLE_ITERATOR(NAME) \
WITH_RETURN(DOUBLE_CONTAINER_DECLARATION(NAME), DOUBLE_CONTAINER_APPLY_DOUBLE_ITERATOR(NAME))


#define NO_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(NAME) \
NO_RETURN(DOUBLE_CONTAINER_DECLARATION(NAME), DOUBLE_CONTAINER_APPLY_SINGLE_ITERATOR(NAME)'')

#define NO_RETURN_DOUBLE_CONTAINER_DOUBLE_ITERATOR(NAME) \
NO_RETURN(DOUBLE_CONTAINER_DECLARATION(NAME), DOUBLE_CONTAINER_APPLY_DOUBLE_ITERATOR(NAME))




namespace algc
{

WITH_RETURN_SINGLE_CONTAINER(all_of)
WITH_RETURN_SINGLE_CONTAINER(any_of)
WITH_RETURN_SINGLE_CONTAINER(none_of)

NO_RETURN_SINGLE_CONTAINER(for_each)

WITH_RETURN_SINGLE_CONTAINER(count)
WITH_RETURN_SINGLE_CONTAINER(count_if)

WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(mismatch)

WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(equal)

WITH_RETURN_SINGLE_CONTAINER(find)
WITH_RETURN_SINGLE_CONTAINER(find_if)
WITH_RETURN_SINGLE_CONTAINER(find_if_not)

WITH_RETURN_DOUBLE_CONTAINER_DOUBLE_ITERATOR(find_first_of)

WITH_RETURN_SINGLE_CONTAINER(adjacent_find)

WITH_RETURN_DOUBLE_CONTAINER_DOUBLE_ITERATOR(search)

WITH_RETURN_SINGLE_CONTAINER(search_n)

WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(copy)
WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(copy_if)
WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(copy_backward)

WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(move)
WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(move_backward)

NO_RETURN_SINGLE_CONTAINER(fill)

NO_RETURN_SINGLE_CONTAINER(generate)

WITH_RETURN_SINGLE_CONTAINER(remove)
WITH_RETURN_SINGLE_CONTAINER(remove_if)

WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(remove_copy)
WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(remove_copy_if)

NO_RETURN_SINGLE_CONTAINER(replace)
NO_RETURN_SINGLE_CONTAINER(replace_if)

WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(replace_copy)
WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(replace_copy_if)

WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(swap_ranges)

NO_RETURN_SINGLE_CONTAINER(reverse)

WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(reverse_copy)

//NO_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(rotate)
WITH_RETURN_DOUBLE_CONTAINER_SINGLE_ITERATOR(rotate)



NO_RETURN_SINGLE_CONTAINER(sort)




#define MAKE_FUNC(FUNC) \
template <typename... Args>                                                 \
decltype(auto) FUNC##_ (Args&&... args)                                     \
{                                                                           \
    return [&](auto&& container)                                            \
            {                                                               \
                return FUNC(std::forward<decltype(container)>(container),   \
                            std::forward<Args>(args)...);                   \
            };                                                              \
}


MAKE_FUNC(sort)


template <class Container, class F>
decltype(auto) operator & (Container&& container, F f)
{
    f(std::forward<Container>(container));

    return container;
}



}

// SPECIAL_CASES:  copy_n, fill_n, generate_n, transform, swap, iter_swap


#endif // CPPL_STL_CONTAINER_H