#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
const int NODIR = -1;
const int NORTH = 0;
const int WEST  = 1;
const int SOUTH = 2;
const int EAST  = 3;

class StudentWorld;

int randInt(int lowest, int highest);


class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world)
		: GraphObject(imageID, startX, startY), m_world(world), m_dead(false) {setVisible(true);}
	virtual void doSomething()  = 0;
	virtual void setDead()      {m_dead = true; setVisible(false);}
	virtual bool isDead() const {return m_dead;}
	StudentWorld* getWorld() const {return m_world;}
private:
	bool m_dead;
	StudentWorld* m_world;
};

class Exit: public Actor
{
public:
	Exit(int startX, int startY, StudentWorld* world): Actor(IID_EXIT, startX, startY, world), isVisible(false) {setVisible(isVisible);}
	virtual ~Exit(){}
	virtual void doSomething();
private:
	bool isVisible;
};

class Player : public Actor
{
public:
	Player(int startX, int startY, StudentWorld* world)
		: Actor(IID_PLAYER, startX, startY, world), canWalkThroughDBricks(false), completedCurrentLevel(false), hasExtraBombs(false), m_bombCount(2) {}
	virtual ~Player(){}

	//Accessors
	bool completedLevel() const {return completedCurrentLevel;}
	bool canWalkThroughDestroyableBricks() const {return canWalkThroughDBricks;}
	//Mutators
	virtual void doSomething();
	virtual void setDead();
	void toggleGhostMode()            {canWalkThroughDBricks = !canWalkThroughDBricks;}
	void setGhostModeDuration(int n)  {ghostModeTicksLeft = n;}
	void toggleExtraBombs()           {hasExtraBombs = !hasExtraBombs;}
	void setExtraBombDuration(int n)  {extraBombTicksLeft = n;}
	void changeLevelCompletedStatus() {if (!completedCurrentLevel) completedCurrentLevel = true;}
	void changeBrickWalkStatus()      {canWalkThroughDBricks = (!canWalkThroughDBricks);}
	void increaseBombsBy(int n)       {m_bombCount += n;}
	void decreaseBombsBy(int n)       {m_bombCount -= n;}


private:
	int  m_bombCount, ghostModeTicksLeft, extraBombTicksLeft;
	bool canWalkThroughDBricks, completedCurrentLevel, hasExtraBombs;

};

class Zumi : public Actor
{
public:
	Zumi(int imageID, int startX, int startY, StudentWorld* world, int TPA);
	virtual ~Zumi();
	//Accessors
	int getCurrentDirection() const    			{return currentDirection;}
	int getTickCounter() const 					{return tickCounter;}
	int getTPA() const 							{return ticksPerAction;}
	bool isOnPlayer() const;
	//Mutators	
	void resetTickCounter()            			{tickCounter = 0;}
	void addToTickCounter()			            {tickCounter++;}
	void changeCurrentDirection()       		{currentDirection = randInt(NORTH, EAST);}
	void SimpleZumiMove();
private:
	int currentDirection,
		ticksPerAction,
		tickCounter,
		probabilityOfGoodieOverall,
		probabilityOfWalkThruWalls,
		probabilityOfExtraLife,
		probabilityOfExtraBombs;
};

class SimpleZumi : public Zumi
{
public:
	SimpleZumi(int startX, int startY, StudentWorld* world, int TPA): Zumi(IID_SIMPLE_ZUMI, startX, startY, world, TPA){}
	virtual ~SimpleZumi(){}
	virtual void doSomething();
	virtual void setDead();

};

class ComplexZumi : public Zumi
{
public:
	ComplexZumi(int startX, int startY, StudentWorld* world, int TPA);
	virtual ~ComplexZumi(){}
	//Accessors
	int horizDist() const;
	int vertDist()  const;
	bool canSmellPlayer() const;
	//Mutators
	virtual void doSomething();
	virtual void setDead();
	//Calculations
	int findBestDirToPlayer();

private:
	int searchDistance;

};

class TimedLifeActor : public Actor
{
public:
	TimedLifeActor(int imageID, int startX, int startY, StudentWorld* world): Actor(imageID, startX, startY, world){}
	//Accessors
	int getTicks() const {return ticksLeft;}
	//Mutators
	void setTicks(int t) {ticksLeft = t;}
	void decTicks()      {if (ticksLeft > 0) ticksLeft--;}
	void makeTicksZero() {ticksLeft = 0;}
private:
	int ticksLeft;
};

class Bomb : public TimedLifeActor
{
public:
	Bomb(int startX, int startY, StudentWorld* world): TimedLifeActor(IID_BUGSPRAYER, startX, startY, world){setTicks(40);}
	virtual ~Bomb(){}
	virtual void doSomething();
	virtual void setDead();

	void createBugSprays();


};

class BugSpray : public TimedLifeActor
{
public:
	BugSpray(int startX, int startY, StudentWorld* world): TimedLifeActor(IID_BUGSPRAY, startX, startY, world){setTicks(3);}
	virtual ~BugSpray(){}
	virtual void doSomething();
};

class Goodie : public TimedLifeActor
{
public:
	Goodie(int imageID, int startX, int startY, StudentWorld* world);

};

class ExtraLife : public Goodie
{
public:
	ExtraLife(int startX, int startY, StudentWorld* world): Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, world){}
	virtual ~ExtraLife(){}
	virtual void doSomething();
};

class GhostMode : public Goodie
{
public:
	GhostMode(int startX, int startY, StudentWorld* world): Goodie(IID_WALK_THRU_GOODIE, startX, startY, world){}
	virtual ~GhostMode(){}
	virtual void doSomething();
};

class BombBag : public Goodie
{
public:
	BombBag(int startX, int startY, StudentWorld* world): Goodie(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, startX, startY, world){}
	virtual ~BombBag(){}
	virtual void doSomething();
};

class Brick : public Actor
{
public:
	Brick(int imageID, int startX, int startY, StudentWorld* world, bool isDestroyable = false): Actor(imageID, startX, startY, world), m_isDestroyable(isDestroyable){}
	bool isDestroyable(){return m_isDestroyable;}
private:
	bool m_isDestroyable;
};

class PermaBrick : public Brick
{
public:
	PermaBrick(int startX, int startY, StudentWorld* world): Brick(IID_PERMA_BRICK, startX, startY, world) {}
	virtual ~PermaBrick(){}
	virtual void doSomething(){}
	virtual void setDead(){}                     //Cannot kill permabrick
};

class DestroyableBrick : public Brick
{
public:
	DestroyableBrick(int startX, int startY, StudentWorld* world): Brick(IID_DESTROYABLE_BRICK, startX, startY, world, true){}
	virtual ~DestroyableBrick(){}
	virtual void doSomething(){}
};


#endif // ACTOR_H_
