#ifndef CPPL_HELPERS_H
#define CPPL_HELPERS_H

#include <type_traits>
#include <assert.h>






void db ()
{
	std::cout << '\n' << std::flush;
}

template <typename T, typename... Args>
void db (const T& t, const Args& ...args)
{
	std::cout << t;

	if(sizeof...(Args))
		std::cout << ' ';

	db(args...);
}






template <class Iterator>
std::ostream& operator << (std::ostream& out, std::pair<Iterator, Iterator> range)
{
	std::copy(range.first, range.second, std::ostream_iterator<Iterator>());

	return out;
}




template <bool...>
struct And;

template <bool B1, bool... Bs>
struct And< B1, Bs... > : And< Bs... > {};

template <bool... Bs>
struct And< false, Bs... > : std::false_type {};

template <>
struct And<true> : std::true_type {};

template <>
struct And<> : std::true_type {};


struct <bool...>
struct Or;


template <bool B1, bool B2>
struct Or : std::


template <bool B1, bool... Bs>


template <bool... Bs>
ctx bool And_v = And< Bs... >::value;





/// Taken from https://bitbucket.org/martinhofernandes/wheels/src/default/include/wheels/meta/type_traits.h%2B%2B?fileviewer=file-view-default#cl-161
template <typename T, template <typename...> class Template>
struct IsSpecialization : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct IsSpecialization<Template<Args...>, Template> : std::true_type {};




HAS_EXTERN_FUNC(std::begin, hasBegin)

template <class T>
constexpr bool isContainer_v = hasBegin<T>();




template <class Apply, typename... Args, std::size_t... Is, typename... FuncArgs>
void applyTuple (Apply apply, std::tuple<Args...>& tup, std::index_sequence<Is...>, FuncArgs&&... funcArgs)
{
    const auto& dummie = { ( apply( std::get<Is>(tup), std::forward<FuncArgs>(funcArgs)... ), int{} ) ... };
}

template <class Apply, typename... Args, typename... FuncArgs>
void applyTuple (Apply apply, std::tuple<Args...>& tup, FuncArgs&&... funcArgs)
{
    return applyTuple(apply, tup, std::make_index_sequence<sizeof...(Args)>(), std::forward<FuncArgs>(funcArgs)...);
}




#endif // CPPL_HELPERS_H