#ifndef DISTANCES_H
#define DISTANCES_H
#include <type_traits>
#include <array>
#include <algorithm>
#include <iostream>



namespace dist
{

struct Euclidean;

namespace impl
{

template <class Dist>
class Distances
{
public:

    constexpr Distances (const Dist& dist = Dist()) : dist(dist) {}


    template <typename T, typename U>
    inline constexpr auto operator () (const T& t, const U& u)
    {
        return dist(t, u);
    }


    template <typename T, typename U, std::size_t N>
    inline auto operator () (const std::array<T, N>& v, const std::array<U, N>& u)
    {
        return this->operator()(v.begin(), v.end(), u.begin());
    }

    template <typename T, std::size_t N, typename U>
    inline auto operator () (const std::array<T, N>& v, const std::vector<U>& u)
    {
        return this->operator()(v.begin(), v.end(), u.begin());
    }


    template <typename T, typename U>
    inline auto operator () (const std::vector<T>& v, const std::vector<U>& u)
    {
        return this->operator()(v.begin(), v.end(), u.begin());
    }

    template <typename T, typename U, std::size_t N>
    inline auto operator () (const std::vector<T>& v, const std::array<U, N>& u)
    {
        return this->operator()(v.begin(), v.end(), u.begin());
    }



    template <typename Iter1, typename Iter2>
    inline auto operator () (Iter1 first1, Iter1 last1, Iter2 first2)
    {
        using Type = decltype(*first1 * *first2);

        return std::inner_product(first1, last1, first2, Type{}, std::plus<Type>(),
                                  [this](const auto& x, const auto& y){ return this->operator()(x, y); });
    }


private:

    Dist dist;

};



//---------------------------------------------------------------------------

struct Euclidean
{
    template <typename T, typename U>
    inline constexpr auto operator () (const T& t, const U& u)
    {
        auto diff = t - u;

        return diff * diff;
    }
};


//---------------------------------------------------------------------------

struct EuclideanSquared : public Euclidean
{
    template <typename T, typename U>
    inline auto operator () (const T& t, const U& u)
    {
    	return std::sqrt(Euclidean::operator()(t, u));
    }
};


//---------------------------------------------------------------------------

struct Manhattan
{
    template <typename T, typename U>
    inline constexpr auto operator () (const T& t, const U& u)
    {
        return std::abs(t - u);
    }
};


//---------------------------------------------------------------------------


template <typename Type, std::size_t N>
class LookUpDistance
{
public:

    //template <typename T, typename U>
    inline constexpr auto operator () (int t, int u)
    {
        return v[t - u];
    }


protected:

    std::array<Type, N> lookUp;

    Type* v;
};



struct EuclideanRGB : public LookUpDistance<unsigned int, 511>
{
    EuclideanRGB ()
    {
        v = &lookUp[255];

        for(int i = 0; i < 255; ++i)
            lookUp[i] = (255 - i) * (255 - i);

        for(int i = 255, j = 0; i < 511; ++i, ++j)
            lookUp[i] = j * j;
    }
};


struct ManhattanRGB : public LookUpDistance<unsigned int, 511>
{
    ManhattanRGB ()
    {
        v = &lookUp[255];

        for(int i = 0; i < 255; ++i)
            lookUp[i] = 255 - i;

        for(int i = 255, j = 0; i < 511; ++i, ++j)
            lookUp[i] = j;
    }
};



}   // namespace impl


//=============================================================================================



struct Euclidean : impl::Distances<impl::Euclidean> {};

struct EuclideanSquared : impl::Distances<impl::EuclideanSquared> {};

struct Manhattan : impl::Distances<impl::Manhattan> {};


struct EuclideanRGB : impl::Distances<impl::EuclideanRGB> {};

struct ManhattanRGB : impl::Distances<impl::ManhattanRGB> {};




template <class F>
inline auto makeDistance (F f)
{
    return impl::Distances<F>(f);
}



}   // namespace dist


#endif // DISTANCES_H
