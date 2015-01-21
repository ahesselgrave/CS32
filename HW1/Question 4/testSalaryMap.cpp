#include "SalaryMap.h"
#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	SalaryMap sMap;
	sMap.add("Joe", 65000);
	assert(!sMap.add("Joe", 65000));
	sMap.add("Kongpoy", 123474);
	assert(sMap.salary("Kongpoy") == 123474 && sMap.salary("Joe") == 65000);
	assert(sMap.raise("Kongpoy", -100) && !sMap.raise("Joe", -101));
	assert(sMap.size() == 2);
	sMap.print();

	cout << "Passed all tests" << endl;
}