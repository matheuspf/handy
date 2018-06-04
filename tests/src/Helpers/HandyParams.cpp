#include "Helpers/HandyParams.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>


namespace
{

struct HandyParams
{
    HandyParams () {}


    int intParam= 10;

    float floatParam = 20.0f;

    double doubleParam = 30.0;

    std::string stringParam = "not initialized";

    std::vector<int> vectorParam;


    HANDY_PARAMS(HandyParams, intParam, floatParam, doubleParam, stringParam, vectorParam);
};


TEST(HandyParamsTest, Test_1)
{
    HandyParams hp({ {"intParam", 100}, {"floatParam", 200.0f}, {"doubleParam", 300.0} });

    EXPECT_EQ(hp.intParam, 100);
    EXPECT_EQ(hp.floatParam, 200.0f);
    EXPECT_EQ(hp.doubleParam, 300.0);
    EXPECT_EQ(hp.stringParam, "not initialized");
    EXPECT_EQ(hp.vectorParam, std::vector<int>{});
}


TEST(HandyParamsTest, Test_2)
{
    HandyParams hp({ {"intParam", -100}, {"floatParam", -200.0f}, {"doubleParam", -300.0},
                     {"stringParam", std::string("initialized")}, {"vectorParam", std::vector<int>{0, 1, 2, 3, 4}} });

    EXPECT_EQ(hp.intParam, -100);
    EXPECT_EQ(hp.floatParam, -200.0f);
    EXPECT_EQ(hp.doubleParam, -300.0);
    EXPECT_EQ(hp.stringParam, "initialized");
    EXPECT_EQ(hp.vectorParam, (std::vector<int>{0, 1, 2, 3, 4}));
}


}