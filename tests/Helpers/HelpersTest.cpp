#include "Helpers/Helpers.h"
#include "gtest/gtest.h"


#include <fstream>
#include <sstream>

#include <vector>


namespace
{


TEST(HelpersTest, IsInheritedTest)
{
    struct A {};

    struct B : A {};

    struct C {};

    struct D : B {};
    

    EXPECT_EQ((handy::IsInherited<B, A>::value), true);
    EXPECT_EQ((handy::IsInherited<C, A>::value), false);
    EXPECT_EQ((handy::IsInherited<C, B>::value), false);
    EXPECT_EQ((handy::IsInherited<D, B>::value), true);
    EXPECT_EQ((handy::IsInherited<D, A>::value), true);

    EXPECT_EQ((handy::IsInherited<A, A>::value), true);
    EXPECT_EQ((handy::IsInherited<B, B>::value), true);
    EXPECT_EQ((handy::IsInherited<C, C>::value), true);
    EXPECT_EQ((handy::IsInherited<D, D>::value), true);
}




TEST(HelpersTest, GetArgTest)
{
    EXPECT_EQ((std::is_same<handy::GetArg_t<1, int, char, double>, char>::value), true);
    EXPECT_EQ((std::is_same<handy::GetArg_t<0, int, char, double>, char>::value), false);
    EXPECT_EQ((std::is_same<handy::GetArg_t<2, int, char, double>, double>::value), true);
}



}