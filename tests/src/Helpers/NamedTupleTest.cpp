#include "Helpers/NamedTuple.h"
#include "gtest/gtest.h"

#include <string>


namespace
{

NAMED_TUPLE(Triple, first, second, third)

NAMED_GETTERS(first, second, third)



TEST(NamedTupleTest, AccessTest)
{
    Triple<int, double, std::string> triple(10, 20.0, " ");

    EXPECT_EQ(triple.first(),  first(triple));
    EXPECT_EQ(triple.second(), second(triple));
    EXPECT_EQ(triple.third(),  third(triple));

    EXPECT_EQ(triple.first(),  std::get<0>(triple));
    EXPECT_EQ(triple.second(), std::get<1>(triple));
    EXPECT_EQ(triple.third(),  std::get<2>(triple));


    triple.third() = "working!";

    EXPECT_EQ(triple.third(), std::string("working!"));
}


TEST(NamedTupleTest, InheritanceTest)
{
    Triple<float, std::string, char> triple(10.0f, " ", ' ');

    std::tuple<float, std::string, char>& tup = triple;

    EXPECT_EQ(first(tup),  std::get<0>(tup));
    EXPECT_EQ(second(tup), std::get<1>(tup));
    EXPECT_EQ(triple.third(),  std::get<2>(tup));
    


    std::get<1>(tup) = "working!";
    std::get<2>(tup) = 'w';

    EXPECT_EQ(second(tup), std::string("working!"));
    EXPECT_EQ(triple.third(), 'w');
}


}