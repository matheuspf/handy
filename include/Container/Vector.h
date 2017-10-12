/** \file Vector
  *
  * Defines the base class for 'Container'
*/

#ifndef CNT_VECTOR_H
#define CNT_VECTOR_H


#include "Helpers.h"


namespace cnt
{

/** 'cnt::Vector' inherits from 'std::vector<T>' if it is not supplied with compile time size
  * or if the given compile time size is greater than the value defined at 'cnt::help::maxSize',
  * for maximum stack size allocation. Otherwise it inherits from 'std::array<T, N>'.
*/
template <typename T, std::size_t N = 0>
struct Vector : public help::SelectType<T, N>
{
    using Base = help::SelectType<T, N>;

    using Base::Base;   /// Inherits all constructors of std::vector. std::array has no constructor


    static constexpr std::size_t Size = N;  /// Compile time size. May be 0.


    static constexpr bool isArray  = help::isArray<N>;   /// Verify if the class inherits from a 'std::array'

    static constexpr bool isVector = help::isVector<N>;  /// Verify if the class inherits from a 'std::vector'


    /// If compile time size 'N' is greater than 'help::maxSize', initialize the 'std::vector' with this size
    template <std::size_t M = Size, help::EnableIfVector<M> = 0>
    Vector () : Base(Size) {}


    /// For std::array aggregate initialization
    template <typename... Args, std::size_t M = Size, help::EnableIfArray<M> = 0>
    Vector (Args&&... args) : Base{std::forward<Args>(args)...} {}


    /// Making the interface of 'std::array' a little more compatible with 'std::vector'
    template <std::size_t M = Size, help::EnableIfArray<M> = 0>
    Vector (int, const T& t = T())
    {
        std::fill(this->begin(), this->end(), t);
    }
};


} // namespace cnt


#endif  // CNT_VECTOR_H