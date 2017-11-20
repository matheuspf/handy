#include "Helpers/Print.h"
#include "gtest/gtest.h"


#include <fstream>
#include <sstream>

#include <vector>



TEST(PrintTest, functionTest)
{
    handy::print("I", "AM", "PRINTING");


    std::stringstream ss1, ss2;

    handy::print(ss1, "0", "1", 2, 3, "4", 5, "6", "7", 8, 9);
    
    handy::print(ss2, std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    for(int i = 0, x, y; i < 10; ++i)
    {
        ss1 >> x;
        ss2 >> y;

        EXPECT_EQ(i, x);
        EXPECT_EQ(i, y);
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
    EXPECT_EQ(ss1.good(), true);
    EXPECT_EQ(ss2.good(), true);
    EXPECT_EQ(out.good(), true);
}


TEST(PrintTest, classTest)
{
    handy::Print printer(", ", "");

    std::stringstream ss1, ss2;

    printer(ss1, 10, 20, 30);

    printer(ss2, std::vector<int>{10, 20, 30});


    std::string res("10, 20, 30");

    EXPECT_EQ(ss1.str(), res);
    EXPECT_EQ(ss2.str(), res);
}