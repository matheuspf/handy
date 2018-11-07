/** @file
  
    @brief Definition for the base class for handy::Container

    The handy::Vector class inherits from std::vector if it is not supplied with compile time size
    or if the given compile time size is greater than the value defined at handy::impl::cnt::maxSize,
    for maximum stack size allocation. Otherwise it inherits from std::array.
*/

#ifndef HANDY_CONTAINER_VECTOR_H
#define HANDY_CONTAINER_VECTOR_H


#include "Helpers.h"


namespace handy
{

/** @copydoc Container/Vector.h

    @tparam T The base type of the class
    @tparam N The compile time initial size
*/
template <typename T, std::size_t N = 0>
struct Vector : public impl::cnt::SelectType<T, N>
{
    using Base = impl::cnt::SelectType<T, N>; ///< An alias for the base type

    using Base::Base;   ///< Inherits all constructors of std::vector -- std::array has no constructor


    static constexpr std::size_t Size = N;  ///< Compile time size -- May be 0.


    static constexpr bool isArray  = impl::cnt::isArray<N>;   ///< Verify if the class inherits from a std::array

    static constexpr bool isVector = impl::cnt::isVector<N>;  ///< Verify if the class inherits from a std::vector


    /// If compile time size @p N is greater than handy::impl::cnt::maxSize, initialize the std::vector with this size
    template <std::size_t M = Size, impl::cnt::EnableIfVector<M> = 0>
    Vector () : Base(Size) {}


    /// For std::array aggregate initialization
    template <typename... Args, std::size_t M = Size, impl::cnt::EnableIfArray<M> = 0>
    Vector (Args&&... args) : Base{std::forward<Args>(args)...} {}


    /// Making the interface of std::array a little more compatible with std::vector
    template <std::size_t M = Size, impl::cnt::EnableIfArray<M> = 0>
    Vector (int, const T& t = T())
    {
        std::fill(this->begin(), this->end(), t);
    }
};


} // namespace cnt


#endif  // HANDY_CONTAINER_VECTOR_H