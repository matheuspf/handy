#include <array>
#include <vector>
#include <list>
#include <set>

#include "gtest/gtest.h"
#include "Algorithms/Algorithms.h"


namespace
{
	struct AlgorithmsTest : public ::testing::Test
	{
		AlgorithmsTest () { }

		virtual ~AlgorithmsTest () { }


		virtual void SetUp ()
		{
            gen = std::mt19937(dvc());

            stdVec = std::vector<int>(n);

            std::generate(stdVec.begin(), stdVec.end(), [&]{ return rng(0, 100); });
            
            algVecFunc = stdVec;
            algVecOp = stdVec;
		}


		virtual void TearDown ()
		{
		}


		int rng (int l, int u)
		{
			return std::uniform_int_distribution<>(l, u - 1)(gen);
		}


        int n = 10;

        std::vector<int> stdVec;

        std::vector<int> algVecFunc;

        std::vector<int> algVecOp;


        std::mt19937 gen;

		std::random_device dvc;
	};



#define NO_RETURN_SINGLE_CONTAINER_TEST(ALGORITHM, ...)  \
TEST_F(AlgorithmsTest, ALGORITHM)    \
{   \
    ::std::ALGORITHM(stdVec.begin(), stdVec.end(), ## __VA_ARGS__);  \
    ::alg::ALGORITHM(algVecFunc, ## __VA_ARGS__);    \
    algVecOp & ::alg::ALGORITHM(__VA_ARGS__);   \
\
    EXPECT_EQ(stdVec, algVecFunc);  \
    EXPECT_EQ(stdVec, algVecOp);    \
}


NO_RETURN_SINGLE_CONTAINER_TEST(sort)
NO_RETURN_SINGLE_CONTAINER_TEST(reverse)
NO_RETURN_SINGLE_CONTAINER_TEST(random_shuffle)
//NO_RETURN_SINGLE_CONTAINER_TEST(shuffle, std::mt19937(0))



} // namespace

