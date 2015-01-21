#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include "Zombie.h"
#include "Player.h"
#include "Arena.h"
#include "globals.h"
#include "History.h"
#include "Game.h"
using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    Game g(10, 12, 20);

    g.play();
}
