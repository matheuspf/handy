#include "Container/Container.h"

#include <iostream>
#include <list>
#include <set>



using namespace std;




int main ()
{
///[Container Snippet]
	// Type int and three dimensions with sizes 10, 20 and 30 respetivelly.
	// In this case (compile time size is given), 'a' inherits from 'std::array'.
	handy::Container<int, 10, 20, 30> a;

	/// The same as above, but now 'b' inherits from 'std::vector' (runtime size is given).
	handy::Container<int> b(10, 20, 30);

	// You can also create a Container giving the sizes via an itearable type
	// (any type that has both std::begin and std::end defined). They have
	// to contain any integral type.
	handy::Container<double> c(vector<int>{10, 20, 30});

	// Or using more than one iterable (different types, but all of them are integral)
	handy::Container<char> d(list<char>{10}, set<long>{20}, array<int, 1>{30});

	// You can also use a range defined by iterators (or pointers)
	int szs[] = {10, 20, 30, 10000000};

	handy::Container<long> e(&szs[0], szs + 3);

	// Or even a initializer list of integrals
	handy::Container<float> f({10, 20, 30});


	// These are some ways to access the containers via 'operator()'

	// With variadic integrals
	a(0, 1, 2) = 10;

	// With iterables
	b(vector<int>{3}, list<char>{4, 5}) = 20;

	// With integrals or iterables (only for access, not allowed for creation)
	c(9, set<int>{6, 7}) = 0.1;

	// With list initialization
	d({9, 19, 29}) = 'a';

	// With a tuple of integrals/iterables (also only for access)
	e(make_tuple(9, 19, 29)) = 20;

	// And you can access it normally with 'operator[]'
	f[50] = -10.0;

	// You can also use any STL function. It is either a std::vector or a std::array, after all
	sort(a.begin(), a.end());
///[Container Snippet]

	

	return 0;
}