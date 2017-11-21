#include <list>
#include <set>
#include <random>

#include "gtest/gtest.h"
#include "Container/Container.h"


namespace
{
	TEST(SliceTest, Creation)
	{
		handy::Container<int> v(7, 10, 4, 9);


		auto a = v.slice(3);
		auto b = v.slice(4, 5);
		auto c = v.slice(std::initializer_list<int>{0, 2, 3});
		auto d = v.slice(std::vector<int>{6, 9, 3});
		auto e = v.slice(0, 0, 0, 0);
		auto f = v.slice();


		EXPECT_EQ(a.size(), 10*4*9);
		EXPECT_EQ(b.size(), 4*9);
		EXPECT_EQ(c.size(), 9);
		EXPECT_EQ(d.size(), 9);
		EXPECT_EQ(e.size(), 1);
		EXPECT_EQ(f.size(), 7*10*4*9);
	}



	TEST(SliceTest, Access)
	{
		handy::Container<int> v(6, 5, 1, 7, 5);

		int x = v(3, 4, 0, 5, 2) = -10;


		auto a = v.slice();
		auto b = v.slice(3);
		auto c = v.slice(3, 4);
		auto d = v.slice(3, 4, 0);
		auto e = v.slice(3, 4, 0, 5);
		auto f = v.slice(3, 4, 0, 5, 2);
		

		EXPECT_EQ(a(std::vector<long>{3, 4, 0}, 5, std::list<int>{2}), x);
		EXPECT_EQ(b({4, 0, 5, 2}), x);
		EXPECT_EQ(c(0, 5, 2), x);
		EXPECT_EQ(d(std::set<int, std::greater<int>>{5, 2}), x);
		EXPECT_EQ(e(2), x);
		EXPECT_EQ(f(0), x);
	}



	TEST(SliceTest, Looping)
	{
		std::mt19937 gen(std::random_device{}());

		handy::Container<int> v(15, 10, 3, 17, 6);

		std::generate(v.begin(), v.end(), [&]{ return std::uniform_int_distribution<>(0, 100)(gen); });


		auto slc = v.slice(11);


		for(int i = 0, j = 11*10*3*17*6; i < slc.size(); ++i, ++j)
			EXPECT_EQ(slc[i], v[j]);
	}



}