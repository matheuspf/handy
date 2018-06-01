#include "Helpers/HasMember.h"
#include "gtest/gtest.h"

#include <string>


namespace
{

struct A
{
    int a;
};

struct B
{
    void b () {}
};

struct C
{
    void c (int) {}
    void c (double) {}
};

void f (int) {}



HAS_VAR(a, HasMemberA)
HAS_VAR(b, HasMemberB)

HAS_FUNC(a, HasFuncA)
HAS_FUNC(b, HasFuncB)

HAS_OVERLOADED_FUNC(c, HasFuncC)

HAS_EXTERN_FUNC(f, HasFuncF)


TEST(HasMemberTest, SingleTest)
{
    EXPECT_TRUE((HasMemberA<A>::value));
    EXPECT_TRUE((HasFuncB<B>::value));
    EXPECT_TRUE((HasFuncC<C, int>::value));
    EXPECT_TRUE((HasFuncC<C, double>::value));
    EXPECT_TRUE((HasFuncF<int>::value));

    EXPECT_FALSE((HasMemberA<B>::value));
    EXPECT_FALSE((HasMemberA<int>::value));
    EXPECT_FALSE((HasFuncB<A>::value));
    EXPECT_FALSE((HasFuncC<C, std::string>::value));
    EXPECT_FALSE((HasFuncF<std::string>::value));
}


}