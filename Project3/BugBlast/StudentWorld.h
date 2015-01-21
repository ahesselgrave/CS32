#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <list>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
    StudentWorld() : GameWorld(), m_ZumiCount(0) {}
    ~StudentWorld();
    //Accessors
    Level* getLevelPtr()     {return &m_level;}
    Player* getPlayerPtr()   {return playerPtr;}
    int getZumiCount()       {return m_ZumiCount;}
    Actor* findActorAt (int x, int y);

    //Mutators
    void addActor(Actor* ap) {actorList.push_back(ap);}
    void removeDeadActors();
    void addZumi()            {m_ZumiCount++;}
    void decZumi()            {m_ZumiCount--;}
	virtual int init();                    //Page 14
	virtual int move();
	virtual void cleanUp();



private:
    std::list<Actor*> actorList; 
    Player* playerPtr;
    Level m_level;
    int m_ZumiCount;
    int m_levelBonus;
};

#endif // STUDENTWORLD_H_
