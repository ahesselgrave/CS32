#include "StudentWorld.h"
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::~StudentWorld()
{
    std::list<Actor*>::iterator itr = actorList.begin();
    while(itr != actorList.end())
    {
        delete (*itr);
        itr = actorList.erase(itr);
    }
    delete playerPtr;
}

Actor* StudentWorld::findActorAt(int x, int y)
{
	std::list<Actor*>::iterator itr = actorList.begin();
	while(itr != actorList.end())
	{
		if ((*itr)->getX() == x && (*itr)->getY() == y)
			return *itr;
		itr++;
	}
	return nullptr;
}

void StudentWorld::removeDeadActors()
{
    std::list<Actor*>::iterator itr = actorList.begin();
    while(itr != actorList.end())
    {	
    	if ((*itr)->isDead())
    	{
    		delete (*itr);
    		itr = actorList.erase(itr);
    	}
		else itr++;
    }

}

int StudentWorld::init()
{
    std::string curLevel = "level00.dat";  // Step 2
    int l_num = getLevel();
    if (l_num < 10) 
        curLevel[6] = l_num + '0';
    else                              //2 digit number 
    {   
        curLevel[5] = (l_num / 10) + '0';  //Tens digit
        curLevel[6] = (l_num % 10) + '0';  //Ones digit
    }
    Level::LoadResult result = m_level.loadLevel(curLevel);

    if (l_num == 0 && result == Level::load_fail_file_not_found) //First level cannot be found
        return GWSTATUS_NO_FIRST_LEVEL; 
    if (l_num != 0 && result == Level::load_fail_file_not_found) //Player won
        return GWSTATUS_PLAYER_WON;
    if (result == Level::load_fail_bad_format)                   //Improper format level
        return GWSTATUS_LEVEL_ERROR;

    m_levelBonus = m_level.getOptionValue(optionLevelBonus);
// enum MazeEntry {
//     empty, exit, player, simple_zumi, complex_zumi, perma_brick, destroyable_brick
// };
    Level::MazeEntry item;
    for (int x = 0; x < VIEW_WIDTH; x++)
    {
        for (int y = 0; y < VIEW_HEIGHT; y++)
        {
            item = m_level.getContentsOf(x,y);
            switch(item)
            {
	            case Level::perma_brick:
	            {
	                actorList.push_back(new PermaBrick(x,y,this));         
	                break;
	        	}
	            case Level::destroyable_brick:
	         	{
	 	  			actorList.push_back(new DestroyableBrick(x,y,this));
	 	            break;
	 	        }
	            case Level::player:
	            {  
	            	playerPtr = new Player(x,y, this);
		            break;
		        }
	            case Level::exit:
	            {
	            	actorList.push_back(new Exit(x,y,this));
	        	    break;
	        	}
	        	case Level::simple_zumi:
	        	{
	        		actorList.push_back(new SimpleZumi(x,y,this,getLevelPtr()->getOptionValue(optionTicksPerSimpleZumiMove)));
	        		break;
	        	}
	        	case Level::complex_zumi:
	        	{
	        		actorList.push_back(new ComplexZumi(x,y,this,getLevelPtr()->getOptionValue(optionTicksPerComplexZumiMove)));
	        		break;
	        	}
	        	default:
	        		break;
	        }

        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    playerPtr->doSomething();
    for (std::list<Actor*>::iterator itr = actorList.begin(); itr != actorList.end(); itr++)
    {
        if (!((*itr)->isDead()))
            (*itr)->doSomething();
        if (playerPtr->isDead())
            return GWSTATUS_PLAYER_DIED;
        if (playerPtr->completedLevel())
        {
            increaseScore(m_levelBonus);
            playSound(SOUND_FINISHED_LEVEL);
        	return GWSTATUS_FINISHED_LEVEL;
        }
    }
    removeDeadActors();
    if (m_levelBonus > 0)
    m_levelBonus--;

    ostringstream oss;
    oss << "Score: " << setw(8);
    oss.fill('0');
    oss << getScore() << "  Level: " << setw(2);
    oss.fill('0');
    oss << getLevel() << "  Lives: " << setw(3);
    oss.fill('0');
    oss << getLives() << "  Bonus: " << setw(6);
    oss.fill(' ');
    oss << m_levelBonus;
    setGameStatText(oss.str());

    if (playerPtr->isDead())
        return GWSTATUS_PLAYER_DIED;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    std::list<Actor*>::iterator itr = actorList.begin();
    while(itr != actorList.end())
    {
        delete *itr;
        itr = actorList.erase(itr);
    }
    delete playerPtr;
}
