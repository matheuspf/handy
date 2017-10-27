#ifndef CPPL_STL_CONTAINER_H
#define CPPL_STL_CONTAINER_H

#include <algorithm>


#define SINGLE_CONTAINER_DECLARATION(NAME)                      \
                                                                \
template <class Container, typename... Args>                    \
decltype(auto) NAME (Container&& container, Args&&... args)


#define DOUBLE_CONTAINER_DECLARATION(NAME)                                              \
                                                                                        \
template <class Container1, class Container2, typename... Args>                         \
decltype(auto) NAME (Container1&& container1, Container2&& container2, Args&&... args)




#define SINGLE_CONTAINER_APPLY(NAME)                            \
                                                                \
::std::NAME(std::begin(std::forward<Container>(container)),     \
            std::end(std::forward<Container>(container)),       \
            std::forward<Args>(args)...);



#define DOUBLE_CONTAINER_APPLY_ONE(NAME)                            \
                                                                \
::std::NAME(std::begin(std::forward<Container1>(container1)),   \
            std::end(std::forward<Container1>(container1)),     \
            std::begin(std::forward<Container2>(container2)),    \
            std::forward<Args>(args)...);


#define DOUBLE_CONTAINER_APPLY_TWO(NAME)                            \
                                                                \
::std::NAME(std::begin(std::forward<Container1>(container1)),   \
            std::end(std::forward<Container1>(container1)),     \
            std::begin(std::forward<Container2>(container2)),    \
            std::end(std::forward<Container2>(container2)),    \
            std::forward<Args>(args)...);



#define NO_RETURN_SINGLE_CONTAINER(NAME)                            \
                                                                    \
SINGLE_CONTAINER_DECLARATION(NAME)                                  \
{                                                                   \
    SINGLE_CONTAINER_APPLY(NAME)                                    \
                                                                    \
    return std::forward<Container>(container);                      \
}


#define WITH_RETURN_SINGLE_CONTAINER(NAME)                          \
                                                                    \
SINGLE_CONTAINER_DECLARATION(NAME)                                  \
{                                                                   \
    return SINGLE_CONTAINER_APPLY(NAME)                             \
}






namespace algc
{

WITH_RETURN_SINGLE_CONTAINER(all_of)
WITH_RETURN_SINGLE_CONTAINER(any_of)
WITH_RETURN_SINGLE_CONTAINER(none_of)

NO_RETURN_SINGLE_CONTAINER(for_each)

WITH_RETURN_SINGLE_CONTAINER(count)
WITH_RETURN_SINGLE_CONTAINER(count_if)

WITH_RETURN_SINGLE_CONTAINER(mismatch)

WITH_RETURN_SINGLE_CONTAINER(equal)

WITH_RETURN_SINGLE_CONTAINER(find)
WITH_RETURN_SINGLE_CONTAINER(find_if)
WITH_RETURN_SINGLE_CONTAINER(find_if_not)


NO_RETURN_SINGLE_CONTAINER(sort)

}


#endif // CPPL_STL_CONTAINER_H