#include "newMap.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int main()
{     
	Map a(1000);   // a can hold at most 1000 key/value pairs
	Map b(5);      // b can hold at most 5 key/value pairs
	Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
	KeyType k[6] = {"a", "b", "c", "d", "e", "f"};
	ValueType v  = 123.456;

	// No failures inserting pairs with 5 distinct keys into b
	for (int n = 0; n < 5; n++)
	{
		assert(b.insert(k[n], v));
		cout << "Passed assert " << n << endl;
	}

	// Failure if we try to insert a pair with a sixth distinct key into b
	cout << "Before assert failure" << endl;
	assert(!b.insert(k[5], v));
	cout << "After assert failure" << endl;
	// When two Maps' contents are swapped, their capacities are swapped
	// as well:

	a.swap(b);
	cerr << "Swap success!\n";
	assert(!a.insert(k[5], v)  &&  b.insert(k[5],v));
	cout << "Passed all tests!\n";
 }