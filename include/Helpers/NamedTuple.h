#ifndef CPPL_NAMED_TUPLE_H
#define CPPL_NAMED_TUPLE_H

#include <tuple>


#define EXPAND(...) __VA_ARGS__

#define CONCAT_(x, y) x ## y
#define CONCAT(x, y) CONCAT_(x, y)

// http://efesx.com/2010/07/17/variadic-macro-to-count-number-of-arguments/
#define NUM_ARGS_(_1, _2 ,_3, _4, _5, _6, _7, _8, _9, N, ...) N
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define APPLY_N(MACRO, ...) EXPAND(CONCAT(MACRO, NUM_ARGS(__VA_ARGS__)))(__VA_ARGS__)



#define MAKE_GETTERS(...) APPLY_N(MAKE_GETTERS, __VA_ARGS__)


#define MAKE_GETTERS1(NAME)                                     \
template <typename T, T... Cs>                                  \
Get<0, Cs...> constexpr operator ""_NAME () { return {}; }      \







template <typename T, template <typename...> class Template>
struct IsSpecialization : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct IsSpecialization<Template<Args...>, Template> : std::true_type {};



template <std::size_t I, char... Cs>
struct Get
{
    template <class Tuple, std::enable_if_t<IsSpecialization<std::decay_t<Tuple>, std::tuple>::value, int> = 0>
    decltype(auto) operator () (Tuple&& tup) const
    {
        return std::get<I>(std::forward<Tuple>(tup));
    }
};


template <typename T, T... Cs>
Get<0, Cs...> constexpr operator ""_first () { return {}; }

template <typename T, T... Cs>
Get<1, Cs...> constexpr operator ""_second () { return {}; }


#define first ""_first
#define second ""_second








template <typename... Args>
struct NamedTuple : public std::tuple<Args...>
{
    using Tuple = std::tuple<Args...>;

    using Tuple::Tuple;
    using Tuple::operator=;
    using Tuple::swap;



    decltype(auto) first () { return std::get<0>(static_cast<Tuple&>(*this)); }

    decltype(auto) second () { return std::get<1>(static_cast<Tuple&>(*this)); }

    decltype(auto) third () { return std::get<2>(static_cast<Tuple&>(*this)); }
};






#endif // CPPL_NAMED_TUPLE_H