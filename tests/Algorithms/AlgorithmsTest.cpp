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
		
		funcVec = stdVec;
		opVec = stdVec;
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

	std::vector<int> funcVec;

	std::vector<int> opVec;


	std::mt19937 gen;

	std::random_device dvc;
};



TEST_F(AlgorithmsTest, PipeTest)
{
	auto trnsFunc = [](auto x){ return 2 * x; };

	auto forFunc = [](auto& x){ x += 10; };

	std::sort(stdVec.begin(), stdVec.end());
	std::shuffle(stdVec.begin(), stdVec.end(), std::mt19937{0});
	std::reverse(stdVec.begin(), stdVec.end());
	std::transform(stdVec.begin(), stdVec.end(), stdVec.begin(), trnsFunc);
	std::for_each(stdVec.begin(), stdVec.end(), forFunc);

	funcVec = handy::reverse(handy::shuffle(handy::sort(funcVec), std::mt19937{0}));
	funcVec = handy::for_each(handy::transform(funcVec, funcVec, trnsFunc), forFunc);

	opVec = opVec & handy::sort() & handy::shuffle(std::mt19937{0}) & handy::reverse();				
	opVec = opVec & handy::transform(opVec, trnsFunc) & handy::for_each(forFunc);


	EXPECT_EQ(stdVec, funcVec);
	EXPECT_EQ(stdVec, opVec);
}





#define NO_RETURN_SINGLE_CONTAINER_TEST(ALGORITHM, ...)  \
TEST_F(AlgorithmsTest, ALGORITHM)    \
{   \
	::std::ALGORITHM(stdVec.begin(), stdVec.end(), ## __VA_ARGS__);  \
\
	::handy::ALGORITHM(funcVec, ## __VA_ARGS__);    \
\
    opVec & ::handy::ALGORITHM(__VA_ARGS__);   \
\
    EXPECT_EQ(stdVec, funcVec);  \
    EXPECT_EQ(stdVec, opVec);    \
}

#define WITH_RETURN_SINGLE_CONTAINER_TEST(ALGORITHM, ...)  \
TEST_F(AlgorithmsTest, ALGORITHM)    \
{   \
	auto stdRet = ::std::ALGORITHM(stdVec.begin(), stdVec.end(), ## __VA_ARGS__);  \
\
	auto funcRet = ::handy::ALGORITHM(funcVec, ## __VA_ARGS__);    \
\
    auto opRet = opVec & ::handy::ALGORITHM(__VA_ARGS__);   \
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

