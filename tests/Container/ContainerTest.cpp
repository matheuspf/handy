#include <list>
#include <set>
#include <random>

#include "gtest/gtest.h"
#include "Container/Container.h"


namespace
{
	TEST(ContainerTest, Creation)
	{
		std::array<int, 4> sizes = {2, 3, 4, 5};

		int tSize = std::accumulate(sizes.begin(), sizes.end(), 1, std::multiplies<int>());


		handy::Container<int, 2, 3, 4, 5> a;

		handy::Container<double> b(2, 3, 4, 5);

		handy::Container<char> c({2, 3, 4, 5});

		handy::Container<float> d(std::vector<int>{2, 3}, std::set<long>{4}, std::list<char>{5});

		handy::Container<std::vector<int>> e(sizes.begin(), sizes.end());



		for(int i = 0; i < sizes.size(); ++i)
		{
			EXPECT_EQ(a.size(i), sizes[i]);
			EXPECT_EQ(b.size(i), sizes[i]);
			EXPECT_EQ(c.size(i), sizes[i]);
			EXPECT_EQ(d.size(i), sizes[i]);
			EXPECT_EQ(e.size(i), sizes[i]);
		}

		EXPECT_EQ(a.size(), tSize);
		EXPECT_EQ(b.size(), tSize);
		EXPECT_EQ(c.size(), tSize);
		EXPECT_EQ(d.size(), tSize);
		EXPECT_EQ(e .size(), tSize);
	}



	TEST(ContainerTest, Access)
	{
		handy::Container<int> c(7, 3, 6, 2);

		int x = c[213] = 91;

		int arr[] = {5, 2, 4, 1};


		EXPECT_EQ(c(5, 2, 4, 1), x);
		EXPECT_EQ(c({5, 2, 4, 1}), x);
		EXPECT_EQ(c(std::vector<int>{5, 2, 4, 1}), x);
		EXPECT_EQ(c(std::set<int>{5}, 2, 4, std::list<int>{1}), x);
		EXPECT_EQ(c(std::make_tuple(5, 2, 4, 1)), x);
		EXPECT_EQ(c(&arr[0]), x);
	}
	


	TEST(ContainerTest, Looping)
	{
		std::mt19937 gen(std::random_device{}());

		std::vector<int> v(200);

		std::generate(v.begin(), v.end(), [&]{ return std::uniform_int_distribution<>(0, 100)(gen); });



		handy::Container<int> a(10, 20);
		handy::Container<int> b(5, 5, 8);
		handy::Container<int, 50, 4> c;
		handy::Container<int> d(2, 2, 2, 25);
		handy::Container<int, 200> e;


		

		std::copy(v.begin(), v.end(), a.begin());
		std::copy(v.begin(), v.end(), b.begin());
		std::copy(v.begin(), v.end(), c.begin());
		std::copy(v.begin(), v.end(), d.begin());
		std::copy(v.begin(), v.end(), e.begin());


		for(int i = 0; i < v.size(); ++i)
		{
			EXPECT_EQ(a[i], v[i]);
			EXPECT_EQ(b[i], v[i]);
			EXPECT_EQ(c[i], v[i]);
			EXPECT_EQ(d[i], v[i]);
			EXPECT_EQ(e[i], v[i]);
		}
	}


} // namespace

