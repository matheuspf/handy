#ifndef WRAPPER_MATRIX_H
#define WRAPPER_MATRIX_H

#include "Wrapper.h"
#include "Operations.h"


namespace wrp
{

// Defined as a variadic template, so it can be specialized in any necessary way

template <typename...>
struct Matrix;


namespace impl
{

template <typename>
struct IsMatrix : std::false_type {};

template <typename... Args>
struct IsMatrix<Matrix<Args...>> : std::true_type {};


template <typename... Args>
constexpr bool isMatrix = IsMatrix<Args...>::value;

}



// Main specialization. Allows for instantiations like 'wrp::Wrapper<Type>', where 'Type' may have any qualifier

template <typename T>
struct Matrix<T> : public Matrix<T, std::decay_t<T>>
{
    USING_WRAPPER(Matrix<T, std::decay_t<T>>)
};


//========================================================================================


// cv::Mat

//#define USING_OPENCV
#ifdef USING_OPENCV



template <class M, typename T>
struct Matrix<M, cv::Mat_<T>> : public Wrapper<M>
{
	USING_WRAPPER(Wrapper<M>)


    using ValueType = std::conditional_t<impl::isOperations<std::decay_t<T>>, T, Operations<T>>;


    //template <class U = M, typename = std::enable_if_t<!std::is_reference<U>::value>>
    Matrix (int r, int c) { t = cv::Mat_<T>(r, c); }


    int rows () const { return t.rows; }

    int cols () const { return t.cols; }


    template <typename U = T>
    decltype(auto) operator() (int i, int j, std::enable_if_t<!impl::isOperations<U>>* = nullptr)
    {
        return Operations<T&>(t.operator()(i, j));
    }

    template <typename U = T>
    decltype(auto) operator() (int i, int j, std::enable_if_t<impl::isOperations<U>>* = nullptr)
    {
        return t.operator()(i, j);
    }


    template <typename U = T>
    decltype(auto) operator() (int i, int j, std::enable_if_t<!impl::isOperations<U>>* = nullptr) const
    {
        return Operations<T>(t.operator()(i, j));
    }

    template <typename U = T>
    decltype(auto) operator() (int i, int j, std::enable_if_t<impl::isOperations<U>>* = nullptr) const
    {
        return t.operator()(i, j);
    }



    /*template <typename U>
    static auto create (U&&, int rows, int cols)
    {
        return Matrix<cv::Mat_<U>>(cv::Mat_<U>(rows, cols));
    }*/
};

#endif  // USING_OPENCV


}   // namespace wrp


#endif // WRAPPER_MATRIX_H
