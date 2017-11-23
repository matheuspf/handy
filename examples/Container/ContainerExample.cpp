/** \flie ContainerExample.cpp
  *
  * Containts some examples of how to use the 'Container' class
*/

#include "Container/Container.h"

#include <iostream>
#include <list>
#include <set>


using namespace std;



int main ()
{
	// There are a lot of ways to create a 'Container' class. You just need to 
	// define the type and the size of each dimension. Some examples follow:

	/** Type int and three dimensions with sizes 10, 20 and 30 respetivelly.
	  * In this case (compile time size is given), 'a' inherits from 'std::array'.
	*/
	handy::Container<int, 10, 20, 30> a;

	/// The same as above, but now 'b' inherits from 'std::vector' (runtime size is given).
	handy::Container<int> b(10, 20, 30);


	/** You can also create a 'Container' giving the sizes via an itearable type
	  * (any type that has both 'std::begin' and 'std::end' defined). They have
	  * to contain any integral type. Seriously, dont even try to use a floating
	  * pointer container here. It is like 150 lines of compile time errors.
	*/
	handy::Container<double> c(vector<int>{10, 20, 30});


	/// Or using more than one iterable (different types, but all of them are integral)
	handy::Container<char> d(list<char>{10}, set<long>{20}, array<int, 1>{30});


	/// You can also use a range defined by iterators (or pointers)
	int szs[] = {10, 20, 30, 10000000000};

	handy::Container<long> e(&szs[0], szs + 3);


	/// Or even a initializer list of integrals
	handy::Container<float> f({10, 20, 30});



#define PRINT_SIZE(x) cout << "Size of:   " << #x << "  =  " << 

	/// Print Sizes
	PRINT_SIZE(a);
	PRINT_SIZE(b);
	PRINT_SIZE(c);
	PRINT_SIZE(d);
	PRINT_SIZE(e);
	PRINT_SIZE(f);


	//@}






	/** These are some ways to access the containers via 'operator()' */
	//@{

	/// With variadic integrals
	a(0, 1, 2) = 10;


	/// With iterables
	b(vector<int>{3}, list<char>{4, 5}) = 20;


	/// With integrals or iterables (only for access, not allowed for creation)
	c(9, set<int>{6, 7}) = 0.1;


	/// With list initialization
	d({9, 19, 29}) = 'a';


	/// With a tuple of integrals/iterables (also only for access)
	e.back() = -10;

	cout << e(make_tuple(vector<int>{9, 19}, 29)) << "\n";


	/// And you can access it normally with 'operator[]'
	f[50] = -10.0;


	/// You can also use any STL function. It is either a 'std::vector' or a 'std::array', after all
	sort(a.begin(), a.end());


	//@}


	

	return 0;
}