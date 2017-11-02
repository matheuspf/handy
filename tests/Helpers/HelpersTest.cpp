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



TEST(HelpersTest, printTest)
{
    handy::print('I', "AM", "PRINTING");


    std::stringstream ss;

    handy::print(ss, "0", "1", 2, 3, "4", 5, "6", "7", 8, 9);

    for(int i = 0, x; i < 10; ++i)
    {
        ss >> x;

        EXPECT_EQ(i, x);
    }


    std::ofstream out("print_test.txt");

    handy::print(out, 0.0, 0.1, 0.2, 0.3, 0.4);

    out.close();


    std::ifstream in("print_test.txt");

    for(double i = 0, x; i <= 0.4; i += 0.1)
    {
        in >> x;

        EXPECT_DOUBLE_EQ(i, x);
    }


    EXPECT_EQ(std::cout.good(), true);
    EXPECT_EQ(ss.good(), true);
    EXPECT_EQ(out.good(), true);
}



TEST(HelpersTest, GetArgTest)
{
    EXPECT_EQ((std::is_same<handy::GetArg_t<1, int, char, double>, char>::value), true);
    EXPECT_EQ((std::is_same<handy::GetArg_t<0, int, char, double>, char>::value), false);
    EXPECT_EQ((std::is_same<handy::GetArg_t<2, int, char, double>, double>::value), true);
}



}