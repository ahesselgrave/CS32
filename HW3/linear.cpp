#include <cassert>
#include <iostream>
using namespace std;

// Return true if any of the array elements is negative, false
  // otherwise.
bool anyNegative(const double a[], int n)
{
	if (n <= 0)
		return false;
	if (a[n - 1] < 0)
		return true;
	else
		return anyNegative(a, n - 1);

}

  // Return the number of negative elements in the array.
int countNegatives(const double a[], int n)
{
	if (n <= 0)
		return 0;
	if (a[n - 1] < 0)
		return countNegatives(a, n - 1) + 1;
	else
		return countNegatives(a, n - 1);
}

  // Return the subscript of the first negative element in the array.
  // If no element is negative, return -1.
int firstNegative(const double a[], int n)                  		//	Dragon slayed: Yer a wizard, Harry
{
	if (n <= 0)
		return -1;
	if (n == 1)
		return a[n - 1] < 0 ? 0 : -1;

	if (a[n - 1] < 0)
	{
		int i = firstNegative(a, n - 1);
		if (i == -1)
			return n - 1;
		else
			return i;
	}
	return firstNegative(a, n - 1);
}

  // Return the subscript of the smallest element in the array.  If
  // more than one element has the same smallest value, return the
  // smallest subscript of such an element.  If the array has no
  // elements to examine, return -1.
int indexOfMin(const double a[], int n)								//Dragon: slayed
{
	if (n <= 0)
		return -1;
	if (n == 1)
		return 0;
	int i = indexOfMin(a, n - 1);
	return a[i] <= a[n - 1] ? i : n - 1;
}

  // If all n2 elements of a2 appear in the n1 element array a1, in
  // the same order (though not necessarily consecutively), then
  // return true; otherwise (i.e., if the array a1 does not include
  // a2 as a not-necessarily-contiguous subsequence), return false.
  // (Of course, if a2 is empty (i.e., n2 is 0), return true.)
  // For example, if a1 is the 7 element array
  //    10 50 40 20 50 40 30
  // then the function should return true if a2 is
  //    50 20 30
  // or
  //    50 40 40
  // and it should return false if a2 is
  //    50 30 20
  // or
  //    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
    	return true;
    else if (n1 <= 0)
    	return false;
	else if (n2 == 1 && a1[n1 - 1] == a2[n2 - 1])
			return true;
	else if (n2 == 1 && a1[n1 - 1] != a2[n2 - 1])
			return includes(a1, n1 - 1, a2, n2);

    double a1Last = a1[n1 - 1];
    double a2Last = a2[n2 - 1];
    if (a1Last == a2Last)
    	return includes(a1, n1 - 1, a2, n2 - 1);
    else if (a1Last != a2Last)
    	return includes(a1, n1 - 1, a2, n2);
    else if (a1Last != a2Last)
    	return false;

}

int main()
{
	double arr[5] = {10.5, 0, 15, -3, -1};
	double a1[7]  = {10, 50, 40, 20, 50, 40, 30};
	double a2[4]  = {50, 20, 30};
	assert(anyNegative(arr, 5));
	assert(!anyNegative(arr, -1));
	assert(!anyNegative(arr, 3));
	assert(countNegatives(arr, 5) == 2 && countNegatives(arr, 0) == 0 && countNegatives(arr, 1) == 0);
	assert(firstNegative(arr, 5) == 3 && firstNegative(arr, -1) == -1 && firstNegative(arr, 1) == -1);
	assert(indexOfMin(arr, 5) == 3 && indexOfMin(arr, -1) == -1 && indexOfMin(arr, 1) == 0);
	cout << boolalpha << includes(a1, 7, a2, 3) << endl;
}