#include <iostream>
using namespace std;
/*bool includes(const double a1[], int n1, const double a2[], int n2)
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
    double a2Last = a2[n2 - 1]
    if (a1Last == a2Last)
    	return includes(a1, n1 - 1, a2, n2 - 1);
    else if (a1Last != a2Last)
    	return includes(a1, n1 - 1, a2, n2);
    else if (a1Last != a2Last)
    	return false;

}*/

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then for this value of a2     the function must return
//    10 20 40                          1
//    10 40 30                          2
//    20 10 40                          0
//    50 40 30                          3

int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if (n1 <= 0 && n2 > 0)
        return 0;
    else if (n2 <= 0)
        return 1;
    else if (n2 == 1 && a1[n1 - 1] == a2[n2 - 1])
        return 1;
    else if (n2 == 1 && a1[n1 - 1] != a2[n2 - 1])
        return countIncludes(a1, n1 - 1, a2, n2);
    //Create new branch only if things match

    double a1Last = a1[n1 - 1];
    double a2Last = a2[n2 - 1];
    if (a1Last == a2Last)           //If they match, begin branching
        return countIncludes(a1, n1 - 1, a2, n2 - 1) + countIncludes(a1, n1 - 1, a2, n2);
    else if (a1Last != a2Last)
        return countIncludes(a1, n1 - 1, a2, n2);
}



  // Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

  // Rearrange the elements of the array so that all the elements
  // whose value is > splitter come before all the other elements,
  // and all the elements whose value is < splitter come after all
      // the other elements.  Upon return, firstNotGreater is set to the
  // index of the first element in the rearranged array that is
  // <= splitter, or n if there is no such element, and firstLess is
  // set to the index of the first element that is < splitter, or n
  // if there is no such element.
  // In other words, upon return from the function, the array is a
  // permutation of its original value such that
  //   * for 0 <= i < firstNotGreater, a[i] > splitter
  //   * for firstNotGreater <= i < firstLess, a[i] == splitter
  //   * for firstLess <= i < n, a[i] < splitter
  // All the elements > splitter end up in no particular order.
  // All the elements < splitter end up in no particular order.
void split(double a[], int n, double splitter,
                                    int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;

      // It will always be the case that just before evaluating the loop
      // condition:
      //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
      //  Every element earlier than position firstNotGreater is > splitter
      //  Every element from position firstNotGreater to firstUnknown-1 is
      //    == splitter
      //  Every element from firstUnknown to firstLess-1 is not known yet
      //  Every element at position firstLess or later is < splitter

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
            if (a[firstUnknown] < splitter)
            {
                firstLess--;
                exchange(a[firstUnknown], a[firstLess]);
            }
            else
            {
                if (a[firstUnknown] > splitter)
                {
                    exchange(a[firstNotGreater], a[firstUnknown]);
                    firstNotGreater++;
                }
                firstUnknown++;
            }
    }
}

  // Rearrange the elements of the array so that
  // a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
  // If n <= 1, do nothing.
void order(double a[], int n)                       //YER A WIZARD HARRY
{
    if (n <= 1)
        return;
    int j, k;
    split(a, n, a[n - n/2 - 1], j, k);
    order(a, j);
    order(a + k, n - k);
}

int fact(int b)
{
    int a = 1;
    while (b > 1)
    {
        a *= b;
        b--;
    }
    return a;
}
int main()
{
    cout << "\033[2J\033[1;1H";

    int n1 = 7, n2 = 2;
    int numIter = fact(n1+1)/(fact(n2) *fact(n1+1-n2));
    double a1[7] = {10,50,40,20,50,40,30};
    double a2[3] = {10,40};
    cout << "No more than " << numIter << " iterations" << endl;
    cout << countIncludes(a1, n1, a2, n2) << endl;
}
