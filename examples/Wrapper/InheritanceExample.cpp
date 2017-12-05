#include "../../include/Wrapper/Wrapper.h"

#include <vector>

#include "../../include/Helpers/HasMember.h"


///[Wrapper Inheritance Snippet]
// Using HasMember.h
HAS_OVERLOADED_FUNC(operator[], HasRandomAccess);


// For any type that does not have a operator[]
template <typename T>
struct RandomAccessScalar : public handy::Wrapper<T>
{
    USING_WRAPPER(handy::Wrapper<T>);

    T& operator [] (int) { return t; }

    int size () { return 1; }
};

// For any type that has a operator
template <typename T>
struct RandomAccessVector : public handy::Wrapper<T>
{
    USING_WRAPPER(handy::Wrapper<T>);

    decltype(auto) operator [] (int pos) { return t[pos]; }

    int size () { return t.size(); }
};

// Delegate to the right class using the HasRandomAccess checker
template <typename T>
using RandomAccess = std::conditional_t<HasRandomAccess<T, int>::value, RandomAccessVector<T>, RandomAccessScalar<T>>;



int main ()
{
    // A scalar
    RandomAccess<int> scalar(10);

    // A vector
    RandomAccess<std::vector<int>> vec{10, 20, 30};

    int res = 0;

    // Iterate for all elements of scalar (1)
    for(int i = 0; i < scalar.size(); ++i)
        res += scalar[i] * vec[i];


    return 0;
}
///[Wrapper Inheritance Snippet]