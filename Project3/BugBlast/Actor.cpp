#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"
#include <iostream>
#include <cmath>
#include <queue>
#include <list>
#include <cassert>
using namespace std;
//this should recompile
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
class Coord
{
public:
	Coord(int x, int y): m_x(x), m_y(y){}
	~Coord()                           {}
	int getX() const {return m_x;}
	int getY() const {return m_y;}
	void assignParent(Coord* c) {parent = c;}
	Coord*  getParent() const   {return parent;}
private:
	int m_x, m_y;
	Coord* parent;
};

int randInt(int lowest, int highest)
{
	if (highest < lowest)
		swap(highest, lowest);
	return lowest + (rand() % (highest - lowest + 1));
}

void Player::doSomething()
{
	if(isDead())
		return;
	else if (!canWalkThroughDestroyableBricks() && dynamic_cast<Brick*>(getWorld()->findActorAt(getX(), getY())) != nullptr)
	{
		setDead();
		return;
	}

	if (canWalkThroughDestroyableBricks())
		ghostModeTicksLeft--;
	if (ghostModeTicksLeft == 0 && canWalkThroughDestroyableBricks())
		toggleGhostMode();

	if (hasExtraBombs)
		extraBombTicksLeft--;
	if (extraBombTicksLeft == 0 && hasExtraBombs)
	{
		toggleExtraBombs();
		decreaseBombsBy(getWorld()->getLevelPtr()->getOptionValue(optionMaxBoostedSprayers) - 2);
	}

	int ch;
	if (getWorld()->getKey(ch))
	{
		int x = getX();
		int y = getY();
		Brick* br;
		switch(ch)
		{
			case KEY_PRESS_UP:      //Trogdor is burninating here
			{
				 br = dynamic_cast<Brick*>(getWorld()->findActorAt(x, y + 1));
				 if (br != nullptr && br->isDestroyable() && canWalkThroughDestroyableBricks())  //Player has walk through walls goodie
				 	moveTo(x, y + 1);
				 else if (br == nullptr)    //br does NOT point to a brick
					moveTo(x, y + 1);
				break;
			}
			case KEY_PRESS_LEFT:
			{				
				 br = dynamic_cast<Brick*>(getWorld()->findActorAt(x - 1, y));
				 if (br != nullptr && br->isDestroyable() && canWalkThroughDestroyableBricks())
				 	moveTo(x - 1, y);
				 else if (br == nullptr)
					moveTo(x - 1, y);
				break;
			}
			case KEY_PRESS_DOWN:
			{	
				 br = dynamic_cast<Brick*>(getWorld()->findActorAt(x, y - 1));
				 if (br != nullptr && br->isDestroyable() && canWalkThroughDestroyableBricks())
				 	moveTo(x, y - 1);
				 else if (br == nullptr)
					moveTo(x, y - 1);
				break;
			}
			case KEY_PRESS_RIGHT:
			{	
				 br = dynamic_cast<Brick*>(getWorld()->findActorAt(x + 1, y));
				 if (br != nullptr && br->isDestroyable() && canWalkThroughDestroyableBricks())
				 	moveTo(x + 1, y);
				 else if (br == nullptr)
					moveTo(x + 1, y);
				break;
			}
			case KEY_PRESS_SPACE:
			{
				if (m_bombCount > 0)
				{
					Actor* emptySpace = getWorld()->findActorAt(x, y); 
					if (emptySpace == nullptr)                  //No (non Player) actor on current space
					{
						getWorld()->addActor(new Bomb(x, y, getWorld()));
						decreaseBombsBy(1);
					}
				}
				break;
			}
			default:
				break;
		}
	}
}

void Player::setDead()
{
	Actor::setDead();
	getWorld()->decLives();
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Exit::doSomething()
{
	if (getWorld()->getZumiCount() == 0 && !isVisible)
	{
		isVisible = true;
		setVisible(isVisible);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
	}

	Player* player = getWorld()->getPlayerPtr();
	if (isVisible && (player->getX() == getX() && player->getY() == getY()))
		getWorld()->getPlayerPtr()->changeLevelCompletedStatus();  
}

void Bomb::doSomething()
{
	if (isDead())
		return;

	decTicks();
	if (getTicks() == 0)
		setDead();
}

void Bomb::setDead()
{
	makeTicksZero();
	getWorld()->getPlayerPtr()->increaseBombsBy(1);   //Allow the player to use another bomb after this one BugSprayes
	getWorld()->playSound(SOUND_SPRAY);
	Actor::setDead();
	createBugSprays();
	
}

void Bomb::createBugSprays()
{
	int x = getX();
	int y = getY();
	getWorld()->addActor(new BugSpray(x, y, getWorld()));    //Can always create an explosion at bomb's current point;
	for(int yi = y + 1; yi <= y + 2; yi++)
	{
		Brick* br = dynamic_cast<Brick*>(getWorld()->findActorAt(x, yi));
		if (br != nullptr) 												//A brick was found
		{
			br->setDead();
			if (br->isDestroyable())
				getWorld()->addActor(new BugSpray(x, yi, getWorld()));
			break;														//Kill the brick if possible, and stop expansion in this direction;
		}
		if (br == nullptr)
		{
			getWorld()->addActor(new BugSpray(x, yi, getWorld()));
		}
	}
	for(int yi = y - 1; yi >= y - 2; yi--)								//Rinse and repeat for all the other directions
	{
		Brick* br = dynamic_cast<Brick*>(getWorld()->findActorAt(x, yi));
		if (br != nullptr) 												
		{
			br->setDead();
			if (br->isDestroyable())
				getWorld()->addActor(new BugSpray(x, yi, getWorld()));
			break;														 
		}
		if (br == nullptr)
		{
			getWorld()->addActor(new BugSpray(x, yi, getWorld()));
		}
	}

	for(int xi = x + 1; xi <= x + 2; xi++)
	{
		Brick* br = dynamic_cast<Brick*>(getWorld()->findActorAt(xi, y));
		if (br != nullptr) 												
		{
			br->setDead();
			if (br->isDestroyable())
				getWorld()->addActor(new BugSpray(xi, y, getWorld()));
			break;														 
		}
		if (br == nullptr)
		{
			getWorld()->addActor(new BugSpray(xi, y, getWorld()));
		}
	}
	for(int xi = x - 1; xi >= x - 2; xi--)				
	{
		Brick* br = dynamic_cast<Brick*>(getWorld()->findActorAt(xi, y));
		if (br != nullptr) 												
		{
			br->setDead();
			if (br->isDestroyable())
				getWorld()->addActor(new BugSpray(xi, y, getWorld()));
			break;														 
		}
		if (br == nullptr)
		{
			getWorld()->addActor(new BugSpray(xi, y, getWorld()));
		}
	}
}

void BugSpray::doSomething()
{
	if (isDead())
		return;

	decTicks();
	if (getTicks() == 0)
		setDead();

	if (getWorld()->getPlayerPtr()->getX() == getX() && getWorld()->getPlayerPtr()->getY() == getY())
		getWorld()->getPlayerPtr()->setDead();

	Zumi* zp = dynamic_cast<Zumi*>(getWorld()->findActorAt(getX(), getY()));
	if (zp != nullptr)
		zp->setDead();

	Bomb* bp = dynamic_cast<Bomb*>(getWorld()->findActorAt(getX(), getY()));
	if (bp != nullptr)	
		bp->makeTicksZero();

}

Zumi::Zumi(int imageID, int startX, int startY, StudentWorld* world, int TPA): Actor(imageID, startX, startY, world), ticksPerAction(TPA), tickCounter(0)
{
	getWorld()->addZumi();
	currentDirection = randInt(NORTH, EAST);
	probabilityOfGoodieOverall = getWorld()->getLevelPtr()->getOptionValue(optionProbOfGoodieOverall);
	probabilityOfWalkThruWalls = getWorld()->getLevelPtr()->getOptionValue(optionProbOfWalkThruGoodie);
	probabilityOfExtraLife     = getWorld()->getLevelPtr()->getOptionValue(optionProbOfExtraLifeGoodie);
	probabilityOfExtraBombs    = getWorld()->getLevelPtr()->getOptionValue(optionProbOfMoreSprayersGoodie);
}

Zumi::~Zumi()
{
	getWorld()->decZumi();
	int willGoodieSpawn = randInt(0, probabilityOfGoodieOverall);
	if (willGoodieSpawn <= probabilityOfGoodieOverall)          //A goodie will spawn
	{
		if(willGoodieSpawn > 0 && willGoodieSpawn <= probabilityOfExtraLife) //Spawn Extra Life
			getWorld()->addActor(new ExtraLife(getX(), getY(), getWorld()));
		else if(willGoodieSpawn > probabilityOfExtraLife && willGoodieSpawn <= probabilityOfExtraLife + probabilityOfWalkThruWalls) //Spawn Walk Through Walls
			getWorld()->addActor(new GhostMode(getX(), getY(), getWorld()));
		else if(willGoodieSpawn > probabilityOfExtraLife + probabilityOfWalkThruWalls && willGoodieSpawn <= 100) //Spawn Extra Bombs
			getWorld()->addActor(new BombBag(getX(), getY(), getWorld()));
			
	}
}

bool Zumi::isOnPlayer() const
{
	return (getX() == getWorld()->getPlayerPtr()->getX() && getY() == getWorld()->getPlayerPtr()->getY());
}

void Zumi::SimpleZumiMove()
{
	Brick* br;
	Bomb*  bp;
	int x = getX(), y = getY();
	switch(getCurrentDirection())
	{
		case NORTH:
		{
			br = dynamic_cast<Brick*>(getWorld()->findActorAt(x, y + 1));
			bp = dynamic_cast<Bomb*> (getWorld()->findActorAt(x, y + 1));
			if (br == nullptr && bp == nullptr)    //br does NOT point to a brick
				moveTo(x, y + 1);
			else
				changeCurrentDirection();
			break;
		}
		case WEST:
		{				
			br = dynamic_cast<Brick*>(getWorld()->findActorAt(x - 1, y));
			bp = dynamic_cast<Bomb*> (getWorld()->findActorAt(x - 1, y));
			if (br == nullptr && bp == nullptr)
				moveTo(x - 1, y);
			else
				changeCurrentDirection();
			break;
		}
		case SOUTH:
		{	
			br = dynamic_cast<Brick*>(getWorld()->findActorAt(x, y - 1));
			bp = dynamic_cast<Bomb*> (getWorld()->findActorAt(x, y - 1));
			if (br == nullptr && bp == nullptr)
				moveTo(x, y - 1);
			else
				changeCurrentDirection();
			break;
		}
		case EAST:
		{	
			br = dynamic_cast<Brick*>(getWorld()->findActorAt(x + 1, y));
			bp = dynamic_cast<Bomb*> (getWorld()->findActorAt(x + 1, y));
			if (br == nullptr && bp == nullptr)
				moveTo(x + 1, y);
			else
				changeCurrentDirection();
			break;
		}
	}
}

void SimpleZumi::setDead()
{
	Actor::setDead();
	getWorld()->increaseScore(100);
	getWorld()->playSound(SOUND_ENEMY_DIE);
}

void SimpleZumi::doSomething()
{
	if (isDead())
		return;
	addToTickCounter();
	if(getTickCounter() == getTPA())
	{
		resetTickCounter();
		SimpleZumiMove();
	}
	if (isOnPlayer())
		getWorld()->getPlayerPtr()->setDead();
}

ComplexZumi::ComplexZumi(int startX, int startY, StudentWorld* world, int TPA): Zumi(IID_COMPLEX_ZUMI, startX, startY, world, TPA)
{
	searchDistance = getWorld()->getLevelPtr()->getOptionValue(optionComplexZumiSearchDistance);
}

void ComplexZumi::setDead()
{
	Actor::setDead();
	getWorld()->increaseScore(500);
	getWorld()->playSound(SOUND_ENEMY_DIE);
}

int ComplexZumi::horizDist() const
{
	return abs(getWorld()->getPlayerPtr()->getX() - getX());
}

int ComplexZumi::vertDist() const
{
	return abs(getWorld()->getPlayerPtr()->getY() - getY());
}

bool ComplexZumi::canSmellPlayer() const
{
	return ((horizDist() <= searchDistance && vertDist() <= searchDistance) ? true : false);
}

bool isThereCoordAt(int x, int y, list<Coord*>& cList)
{
	list<Coord*>::iterator itr = cList.begin();
	while(itr != cList.end())
	{
		if ((*itr)->getX() == x && (*itr)->getY() == y)
			return true;
		itr++;
	}
	return false;
}

void deleteAllCoords(list<Coord*>& cList)
{
	list<Coord*>::iterator itr = cList.begin();
	int count = 1;
	while(itr != cList.end())
	{
		delete *itr;
		itr = cList.erase(itr);
		// printf("\033[38;2;255;0;0mCoord number \033[38;2;0;255;255m%d\033[0;00m destructed\033[0;00m\n", count);
		count++;
	}
	assert (cList.size() == 0);
}

int ComplexZumi::findBestDirToPlayer()
{
	int newCoordCount = 1;
	int totalPushedOntoList;
	int totalPushedOntoQueue = totalPushedOntoList = 0;
	queue<Coord*> cQueue;
	list<Coord*>  cList;
	Coord* start = new Coord(getX(), getY());
	// printf("\033[38;2;0;255;0mCoord number \033[38;2;255;0;255m%d\033[0;00m constructed\033[0;00m\n", newCoordCount);
	start->assignParent(nullptr);
	int goalX = getWorld()->getPlayerPtr()->getX();
	int goalY = getWorld()->getPlayerPtr()->getY();
	cQueue.push(start);
	totalPushedOntoQueue++;
	while(!cQueue.empty())
	{
		Coord* currentCoord = cQueue.front();
		cQueue.pop();
		int x = currentCoord->getX();
		int y = currentCoord->getY();

		/////////////////////////////////   If the player is found
		if (x == goalX && y == goalY)
		{
			// printf("Created %d total coords, pushed %d coords onto the queue and %d coords onto the list\n", newCoordCount, totalPushedOntoQueue, totalPushedOntoList);
			// cout << "\033[48;2;0;0;255mFound player!!!!!\033[0;00m" << endl;
			Coord* path = currentCoord;
			while (path->getParent() != start)
				path = path->getParent();

			int pathX = path->getX();
			int pathY = path->getY();
			deleteAllCoords(cList);
			delete start;
			if (pathX - getX() != 0)
				return (pathX - getX() == 1) ? EAST : WEST;
			else
				return (pathY - getY() == 1) ? NORTH : SOUTH;
		}
		//////////////////////////////////

		for (int i = NORTH; i <= EAST; i++)
		{
			switch(i)
			{
				case NORTH:
				{
					if(y + 1 < VIEW_HEIGHT && !isThereCoordAt(x, y + 1, cList) && dynamic_cast<Brick*>(getWorld()->findActorAt(x, y + 1)) == nullptr
					   && dynamic_cast<Bomb*>(getWorld()->findActorAt(x, y + 1)) == nullptr)
					{
						Coord* next = new Coord(x, y + 1);
						newCoordCount++;
						next->assignParent(currentCoord);
						cQueue.push(next);
						totalPushedOntoQueue++;
						cList.push_back(next);
						totalPushedOntoList++;
					}
					break;

					// printf("\033[38;2;0;255;0mCoord number \033[38;2;255;0;255m%d\033[0;00m constructed, number %d in queue, %d in list\033[0;00m\n",
					//        newCoordCount, totalPushedOntoQueue, totalPushedOntoList);
				}
				case WEST:
					if(x - 1 > 0 && !isThereCoordAt(x - 1, y, cList) && dynamic_cast<Brick*>(getWorld()->findActorAt(x - 1, y)) == nullptr
					   && dynamic_cast<Bomb*>(getWorld()->findActorAt(x - 1, y)) == nullptr)
					{
						Coord* next = new Coord(x - 1, y);
						newCoordCount++;
						next->assignParent(currentCoord);

						cQueue.push(next);
						totalPushedOntoQueue++;

						cList.push_back(next);
						totalPushedOntoList++;

						// printf("\033[38;2;0;255;0mCoord number \033[38;2;255;0;255m%d\033[0;00m constructed, number %d in queue, %d in list\033[0;00m\n",
						//        newCoordCount, totalPushedOntoQueue, totalPushedOntoList);
					}
					break;
				case SOUTH:
					if(y - 1 > 0 && !isThereCoordAt(x, y - 1, cList) && dynamic_cast<Brick*>(getWorld()->findActorAt(x, y - 1)) == nullptr
					   && dynamic_cast<Bomb*>(getWorld()->findActorAt(x, y - 1)) == nullptr)
					{
						Coord* next = new Coord(x, y - 1);
						newCoordCount++;
						next->assignParent(currentCoord);

						cQueue.push(next);
						totalPushedOntoQueue++;

						cList.push_back(next);
						totalPushedOntoList++;

						// printf("\033[38;2;0;255;0mCoord number \033[38;2;255;0;255m%d\033[0;00m constructed, number %d in queue, %d in list\033[0;00m\n",
						//        newCoordCount, totalPushedOntoQueue, totalPushedOntoList);
					}
					break;
				case EAST:
					if(x + 1 > 0 && !isThereCoordAt(x + 1, y, cList) && dynamic_cast<Brick*>(getWorld()->findActorAt(x + 1, y)) == nullptr
					   && dynamic_cast<Bomb*>(getWorld()->findActorAt(x + 1, y)) == nullptr)
					{
						Coord* next = new Coord(x + 1, y);
						newCoordCount++;
						next->assignParent(currentCoord);

						cQueue.push(next);
						totalPushedOntoQueue++;

						cList.push_back(next);
						totalPushedOntoList++;

						// printf("\033[38;2;0;255;0mCoord number \033[38;2;255;0;255m%d\033[0;00m constructed, number %d in queue, %d in list\033[0;00m\n",
						//        newCoordCount, totalPushedOntoQueue, totalPushedOntoList);
					}
					break;
				default:
					break;
			}
		}
	}
	// cout << "I couldn't find you :(" << endl;
	// printf("Created %d total coords, pushed %d coords onto the queue and %d coords onto the list\n", newCoordCount, totalPushedOntoQueue, totalPushedOntoList);
	deleteAllCoords(cList);
	delete start;
	return NODIR;
}

void ComplexZumi::doSomething()
{
	if (isDead())
		return;
	addToTickCounter();
	if(getTickCounter() == getTPA())
	{
		resetTickCounter();
		if (!canSmellPlayer())  
		{            
			SimpleZumiMove();
		}
		else
		{
			int dir = findBestDirToPlayer();
			// printf("\033[48;2;255;255;255m\033[38;2;0;0;0mDid not segfault: Direction is: %d\033[0;00m\033[0;00m\n", dir);
			Brick* br;
			Bomb*  bp;
			int x = getX(), y = getY();
			switch(dir)
			{		
				case NORTH:
				{
					br = dynamic_cast<Brick*>(getWorld()->findActorAt(x, y + 1));
					bp = dynamic_cast<Bomb*> (getWorld()->findActorAt(x, y + 1));
					if (br == nullptr && bp == nullptr)    //br does NOT point to a brick
						moveTo(x, y + 1);
					break;
				}
				case WEST:
				{				
					br = dynamic_cast<Brick*>(getWorld()->findActorAt(x - 1, y));
					bp = dynamic_cast<Bomb*> (getWorld()->findActorAt(x - 1, y));
					if (br == nullptr && bp == nullptr)
						moveTo(x - 1, y);
					break;
				}
				case SOUTH:
				{	
					br = dynamic_cast<Brick*>(getWorld()->findActorAt(x, y - 1));
					bp = dynamic_cast<Bomb*> (getWorld()->findActorAt(x, y - 1));
					if (br == nullptr && bp == nullptr)
						moveTo(x, y - 1);
					break;
				}
				case EAST:
				{	
					br = dynamic_cast<Brick*>(getWorld()->findActorAt(x + 1, y));
					bp = dynamic_cast<Bomb*> (getWorld()->findActorAt(x + 1, y));
					if (br == nullptr && bp == nullptr)
						moveTo(x + 1, y);
					break;
				}
				case NODIR:					//If a direction is not found, act like a simple Zumi
					SimpleZumiMove();
					break;
			}
		}
	}
	if (isOnPlayer())
		getWorld()->getPlayerPtr()->setDead();
	// cout << "End of ComplexZumi::doSomething()" << endl;
}

Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* world): TimedLifeActor(imageID, startX, startY, world)
{
	setTicks(getWorld()->getLevelPtr()->getOptionValue(optionGoodieLifetimeInTicks));
};

void ExtraLife::doSomething()
{
	if (isDead())
		return;
	decTicks();
	if (getTicks() == 0)
		setDead();
	if (getWorld()->getPlayerPtr()->getX() == getX() && getWorld()->getPlayerPtr()->getY() == getY())
	{
		getWorld()->incLives();
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

void GhostMode::doSomething()
{
	if (isDead())
		return;
	decTicks();
	if (getTicks() == 0)
		setDead();
	if (getWorld()->getPlayerPtr()->getX() == getX() && getWorld()->getPlayerPtr()->getY() == getY())
	{
		getWorld()->getPlayerPtr()->toggleGhostMode();
		getWorld()->getPlayerPtr()->setGhostModeDuration(getWorld()->getLevelPtr()->getOptionValue(optionWalkThruLifetimeTicks));
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

void BombBag::doSomething()
{
	if (isDead())
		return;
	decTicks();
	if (getTicks() == 0)
		setDead();
	if (getWorld()->getPlayerPtr()->getX() == getX() && getWorld()->getPlayerPtr()->getY() == getY())
	{
		getWorld()->getPlayerPtr()->increaseBombsBy(getWorld()->getLevelPtr()->getOptionValue(optionMaxBoostedSprayers) - 2);
		getWorld()->getPlayerPtr()->toggleExtraBombs();
		getWorld()->getPlayerPtr()->setExtraBombDuration(getWorld()->getLevelPtr()->getOptionValue(optionBoostedSprayerLifetimeTicks));

		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

