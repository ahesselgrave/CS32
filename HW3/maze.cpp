#include <string>
#include <iostream>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;                      
    maze[sr][sc] = '*'; //Visited already


    //North Case
    if (sr - 1 >= 0 && maze[sr - 1][sc] == '.')
        if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
            return true;
    //West Case
    if (sc - 1 >= 0 && maze[sr][sc -1] == '.')  
        if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
            return true;
    
    //South Case
    if (sr + 1 < nRows && maze[sr + 1][sc] == '.')
        if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
            return true;
    //East Case
    if (sc + 1 < nCols && maze[sr][sc + 1] == '.')
        if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
            return true;
    return false;
}

int main()
{
    string maze[10] = {
      "XXXXXXXXXX",
      "X........X",
      "XX.X.XXXXX",
      "X..X.X...X",
      "X..X...X.X",
      "XXXX.XXX.X",
      "X..X...XXX",
      "X..XX.XX.X",
      "X...X....X",
      "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 6,4, 1,1))
       cout << "Solvable!" << endl;
    else
       cout << "Out of luck!" << endl;
} 