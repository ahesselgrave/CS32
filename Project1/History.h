#ifndef HISTORY
#define HISTORY
 
#include "globals.h"
class History
{
	public:
		History(int nRows, int nCols);
		bool record(int r, int c);
		void  display() const;
	private:
		int m_nRows;
		int m_nCols;
		char m_historyGrid[MAXROWS][MAXCOLS];
};

#endif