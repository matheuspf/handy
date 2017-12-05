#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "../../include/ZipIter/ZipIter.h"  // The header file



int main ()
{
/// [ZipIter Snippet]
	std::vector<int> v = {1, 2, 3, 4, 5};
	std::array<double, 5> u = {5, 4, 3, 2, 1};



	// Iterating through both containers using std::for_each
	for_each(handy::zipBegin(v, u), handy::zipEnd(v, u), [](auto tup){
		std::cout << std::get<0>(tup) << "     " << std::get<1>(tup) << "\n";
	});



	// Using the handy::unZip to unpack those values.
	// They can be taken as references too
	for_each(handy::zipBegin(v, u), handy::zipEnd(v, u), handy::unZip([](int x, double y){
		std::cout << x << "     " << y << "\n";
	}));



	// Using for range -- The return is a tuple containing references
	for(auto tup : handy::zip(v, u)) handy::unZip(tup, [](int x, double y){
		std::cout << x << "     " << y << "\n";
	});



	// Or using a function that encapsulates the above.
	// The lambda comes after the variadic arguments 
	handy::forEach(v, u, [](int x, double y){
		std::cout << x << "     " << y << "\n";
	});


    // If you have C++17, you can even use this one
    for(auto [x, y] : handy::zip(v, u))
        std::cout << x << "     " << y << "\n";



	// Sorting both containers using the std::tuple operator <
	sort(handy::zipIter(v.begin(), u.begin()), handy::zipIter(v.end(), u.end()));


	// or
	sort(handy::zipBegin(v, u), handy::zipEnd(v, u));


	// or even using a macro that does exactly the same as above
	sort(ZIP_ALL(v, u));


	// using a custom comparison
	sort(ZIP_ALL(v, u), [](auto tup1, auto tup2){
		return std::get<0>(tup1) + std::get<1>(tup1) < std::get<0>(tup2) + std::get<1>(tup2);
	});


	// or using the handy::unZip to magically unpack those tuples
	sort(ZIP_ALL(v, u), handy::unZip([](int v1, double u1, int v2, double u2){
		return v1 + u1 < v2 + u2;
	}));



	// It is really that easy
	transform(ZIP_ALL(v, u), handy::zipBegin(v, u), handy::unZip([](int x, double y){
		return std::make_tuple(0, 0.0);
	}));


	reverse(ZIP_ALL(v, u));


	accumulate(ZIP_ALL(v, u), 0.0, handy::unZip([](double sum, int x, double y){
		return sum + x + y;
	}));




  return 0;
/// [ZipIter Snippet]
}