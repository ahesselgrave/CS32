#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_nRows = nRows;
	m_nCols = nCols;
	for (int r = 1; r <= nRows; r++)
	{
		for (int c = 1; c <= nCols; c++)
		{
			m_historyGrid[r-1][c-1] = '.';
		}
	}
}

bool History::record(int r, int c)
{
	if (r > MAXROWS || c > MAXCOLS)
		return false;
	else
	{
		if (m_historyGrid[r-1][c-1] == '.')
			m_historyGrid[r-1][c-1] = 'A';

		else if (m_historyGrid[r-1][c-1] >= 'A' && m_historyGrid[r-1][c-1] < 'Y')
			m_historyGrid[r-1][c-1] = m_historyGrid[r-1][c-1] + 1;

		else if (m_historyGrid[r-1][c-1] >= 'Y' && m_historyGrid[r-1][c-1] != '.')
			m_historyGrid[r-1][c-1] = 'Z';
	}
	return true;
}

void History::display() const
{
	clearScreen();
	for (int r = 1; r <= m_nRows; r++)
	{
		for (int c = 1; c <= m_nCols; c++)
		{
			cout << m_historyGrid[r-1][c-1];
		}
		cout << endl;
	}
	cout << endl;
}