#include <queue>
#include <iostream>
#include <string>
using namespace std;

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;

class Coord
{
	public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
	private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> cQueue;
	Coord start(sr, sc);
	Coord  goal(er, ec);
	maze[sr][sc] = 'C';    //C to indicate closed set
	cQueue.push(start);
	while (!cQueue.empty())
	{
		Coord next = cQueue.front();
		cQueue.pop();
		if (next.r() == goal.r() && next.c() == goal.c()) //If current coordinate is the goal coordinate
			return true;
		else
		{
			for(int dir = NORTH; dir <= WEST; dir++) //NORTH->EAST->SOUTH->WEST , CW expansion
			{
				switch(dir)
				{
					case NORTH:     //r - 1
						if(next.r() - 1 >= 0 && maze[next.r()-1][next.c()] != 'X' && maze[next.r()-1][next.c()] != 'C')  //If its still in the maze array, and not a wall or discovered
						{
							maze[next.r()-1][next.c()] = 'C';
							cQueue.push( Coord(next.r()-1, next.c()) );							
						}	
						break;
					case WEST:      //c - 1
						if(next.c() - 1 >= 0 && maze[next.r()][next.c()-1] != 'X' && maze[next.r()][next.c()-1] != 'C')
						{
							maze[next.r()][next.c()-1] = 'C';						
							cQueue.push( Coord(next.r(), next.c()-1) );							
						}
						break;
					case SOUTH:    //r + 1
						if(next.r() + 1 <= nRows - 1 && maze[next.r()+1][next.c()] != 'X' && maze[next.r()+1][next.c()] != 'C')
						{
							maze[next.r()+1][next.c()] = 'C';
							cQueue.push( Coord(next.r()+1, next.c()) );							
						}
						break;
					case EAST:    //c + 1
						if(next.c() + 1 <= nCols - 1 && maze[next.r()][next.c()+1] != 'X' && maze[next.r()][next.c()+1] != 'C')
						{
							maze[next.r()][next.c()+1] = 'C';
							cQueue.push( Coord(next.r(), next.c()+1) );							
						}
						break;
				}
			}
		}
	}

	return false;
}
