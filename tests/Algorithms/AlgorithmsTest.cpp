#include <array>
#include <vector>
#include <list>
#include <set>
#include <random>

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
\
	::handy::ALGORITHM(algVecFunc, ## __VA_ARGS__);    \
\
    algVecOp & ::handy::ALGORITHM(__VA_ARGS__);   \
\
    EXPECT_EQ(stdVec, algVecFunc);  \
    EXPECT_EQ(stdVec, algVecOp);    \
}

#define WITH_RETURN_SINGLE_CONTAINER_TEST(ALGORITHM, ...)  \
TEST_F(AlgorithmsTest, ALGORITHM)    \
{   \
	auto stdRet = ::std::ALGORITHM(stdVec.begin(), stdVec.end(), ## __VA_ARGS__);  \
\
	auto funcRet = ::handy::ALGORITHM(algVecFunc, ## __VA_ARGS__);    \
\
    auto opRet = algVecOp & ::handy::ALGORITHM(__VA_ARGS__);   \
\
    EXPECT_EQ(stdRet, funcRet);  \
    EXPECT_EQ(stdRet, opRet);    \
}


WITH_RETURN_SINGLE_CONTAINER_TEST(all_of, [](int x){ return x < 10; })
WITH_RETURN_SINGLE_CONTAINER_TEST(any_of, [](int x){ return x > 10; })
WITH_RETURN_SINGLE_CONTAINER_TEST(none_of, [](int x){ return x == 10; })

NO_RETURN_SINGLE_CONTAINER_TEST(for_each, [](int& x){ x = 0; })

WITH_RETURN_SINGLE_CONTAINER_TEST(count, 10)
WITH_RETURN_SINGLE_CONTAINER_TEST(count_if, [](int x){ return x == 10; })


NO_RETURN_SINGLE_CONTAINER_TEST(sort)
NO_RETURN_SINGLE_CONTAINER_TEST(reverse)
NO_RETURN_SINGLE_CONTAINER_TEST(shuffle, std::mt19937(0))



} // namespace

