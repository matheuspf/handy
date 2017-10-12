#ifndef WRAPPER_OPERATIONS_H
#define WRAPPER_OPERATIONS_H

#include "Wrapper.h"



namespace wrp
{

// Defined as a variadic template, so it can be specialized in any necessary way

template <typename...>
struct Operations;


namespace impl
{

template <typename>
struct IsOperations : std::false_type {};

template <typename... Args>
struct IsOperations<Operations<Args...>> : std::true_type {};


template <typename... Args>
constexpr bool isOperations = IsOperations<Args...>::value;

}




// Main specialization. Allows for instantiations like 'wrp::Wrapper<Type>', where 'Type' may have any qualifier

template <typename T>
struct Operations<T> : public Operations<T, std::decay_t<T>>
{
    USING_WRAPPER(Operations<T, std::decay_t<T>>)
};


//========================================================================================


// Specialization for fundamental types. The 'std::enable_if' is only necessary here

template <typename T>
struct Operations<T, std::enable_if_t<std::is_fundamental<std::decay_t<T>>::value, std::decay_t<T>>> : public Wrapper<T>
{
    USING_WRAPPER(Wrapper<T>)
};




//========================================================================================


// std::array

template <typename T, typename V, std::size_t N>
struct Operations<T, std::array<V, N>> : public Wrapper<T>
{
    USING_WRAPPER(Wrapper<T>)


    template <typename U>
	constexpr Operations (const std::array<U, N>& u) { std::copy(u.begin(), u.end(), t.begin()); }

    constexpr Operations (std::initializer_list<V> in) { std::copy(in.begin(), in.end(), t.begin()); }




    template <typename>
    struct IsArray : std::false_type {};

    template <typename U, std::size_t M>
    struct IsArray<std::array<U, M>> : std::true_type {};

    template <class U>
    static constexpr bool isArray = IsArray<U>::value;



    template <typename U>
    struct OpType { using Type = Operations<std::array<decltype(std::declval<std::decay_t<V>>() * std::declval<std::decay_t<U>>()), N>>; };

    template <typename U>
    struct OpType<Operations<U>> { using Type = typename OpType<std::decay_t<U>>::Type; };

    template <typename U>
    struct OpType<std::array<U, N>> { using Type = typename OpType<std::decay_t<U>>::Type; };

    template <typename U>
    using OpType_t = typename OpType<U>::Type;



#define DEFAULT_OP(OP)  \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const U& u) { for(int i = 0; i < N; ++i) t[i] OP ## = u; return *this; }    \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const std::array<U, N>& v) { for(int i = 0; i < N; ++i) t[i] OP ## = v[i]; return *this; }   \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const Operations<U>& w) { return *this OP ## = w.t; }     \
\
    template <typename U>   \
    decltype(auto) operator OP (const U& u) const { OpType_t<U> r(t); r OP ## = u; return r; }    \
\
    template <typename U, typename = std::enable_if_t<!impl::isOperations<std::decay_t<U>>>>   \
    friend decltype(auto) operator OP (const U& u, const Operations& w) { return w * u; }


DEFAULT_OP(+);
DEFAULT_OP(-);
DEFAULT_OP(*);
DEFAULT_OP(/);


#undef OP_TYPE
#undef DEFAULT_OP


    template <typename U>
    double operator * (const std::array<U, N>& u) const
    {
        decltype(std::decay_t<V>() * std::decay_t<U>()) r{};

        for(int i = 0; i < N; ++i)
            r += t[i] * u[i];

        return r;
    }

    template <typename U, typename = std::enable_if_t<isArray<std::decay_t<U>>>>
    double operator * (const Operations<U>& w) const { return *this * w.t; }

};




//========================================================================================

// cv::Vec

//#define USING_OPENCV
//#ifdef USING_OPENCV


template <typename T, typename V, int N>
struct Operations<T, cv::Vec<V, N>> : public Wrapper<T>
{
    USING_WRAPPER(Wrapper<T>)


    Operations () : Wrapper<T>() {}

    Operations (std::initializer_list<V> il)
    {
        std::copy(il.begin(), il.begin() + 3, &this->t[0]);
    }


    //template <typename U>
    bool operator == (const Operations& u) const { return this->t == u.t; }


    template <typename>
    struct IsVec : std::false_type {};

    template <typename U, int M>
    struct IsVec<cv::Vec<U, M>> : std::true_type {};

    template <class U>
    static constexpr bool isVec = IsVec<U>::value;



    template <typename U>
    struct OpType { using Type = Operations<cv::Vec<decltype(std::declval<std::decay_t<V>>() * std::declval<std::decay_t<U>>()), N>>; };

    template <typename U>
    struct OpType<Operations<U>> { using Type = typename OpType<std::decay_t<U>>::Type; };

    template <typename U>
    struct OpType<cv::Vec<U, N>> { using Type = typename OpType<std::decay_t<U>>::Type; };

    template <typename U>
    using OpType_t = typename OpType<U>::Type;



#define DEFAULT_OP(OP)  \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const U& u) { for(int i = 0; i < N; ++i) t[i] OP ## = u; return *this; }    \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const cv::Vec<U, N>& v) { for(int i = 0; i < N; ++i) t[i] OP ## = v[i]; return *this; }   \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const Operations<U>& w) { return *this OP ## = w.t; }     \
\
    template <typename U>   \
    decltype(auto) operator OP (const U& u) const { OpType_t<U> r(t); r OP ## = u; return r; }    \
\
    template <typename U, typename = std::enable_if_t<!impl::isOperations<std::decay_t<U>>>>   \
    friend decltype(auto) operator OP (const U& u, const Operations& w) { return w * u; }


DEFAULT_OP(+);
DEFAULT_OP(-);
DEFAULT_OP(*);
DEFAULT_OP(/);


#undef OP_TYPE
#undef DEFAULT_OP


    template <typename U>
    double operator * (const cv::Vec<U, N>& u) const
    {
        decltype(std::decay_t<V>() * std::decay_t<U>()) r{};

        for(int i = 0; i < N; ++i)
            r += t[i] * u[i];

        return r;
    }

    template <typename U, typename = std::enable_if_t<isVec<std::decay_t<U>>>>
    double operator * (const Operations<U>& w) const { return *this * w.t; }

};

//#endif  // USING_OPENCV




}   // namespace wrp


namespace std
{

//#ifdef USING_OPENCV

template <typename T, int N>
struct hash<cv::Vec<T, N>>
{
    size_t operator () (const cv::Vec<T, N>& v) const
    {
        size_t r = 0;

        for(int i = 0; i < N; ++i)
            r += hash<T>(v[i]);

        return r;
    }
};


// Faster hash for rgb

template <>
struct hash<cv::Vec3b>
{
    static constexpr int mult[3] = {1, 11, 101};

    size_t operator () (const cv::Vec3b& v) const
    {
        return v[0] * mult[0] + v[1] * mult[1] + v[2] * mult[2];
    }
};

constexpr int hash<cv::Vec3b>::mult[3];


//#endif // USING_OPENCV


template <typename T>
struct hash<wrp::Operations<T>>
{
    inline size_t operator () (const wrp::Operations<T>& op) const
    {
        return hash<std::decay_t<T>>()(op.t);
    }
};

}



#endif // WRAPPER_OPERATIONS_H
