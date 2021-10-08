#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <list>
#include <iterator>
#include <string>
#include <iostream>
#include <stdlib.h> 
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    m_actors.clear();
    m_ghostRacer = new GhostRacer(this);
    m_actors.push_back(m_ghostRacer);
    for (int i = 0; i <= VIEW_HEIGHT / SPRITE_HEIGHT; i++) { //for loops to place all starting border lines
        m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2, i * SPRITE_HEIGHT));
        m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2, i * SPRITE_HEIGHT));
    }
    for (int i = 0; i <= VIEW_HEIGHT / (4 * SPRITE_HEIGHT); i++) {
        m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 6, i * 4 * SPRITE_HEIGHT));
        m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 6, i * 4 * SPRITE_HEIGHT));
        setLastWhiteY(256);
    }
    m_soulsToSave = 2 * getLevel() + 5;
    m_bonus = 5000;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    list <Actor*> ::iterator it;
    getGhostRacer()->doSomething();
    for (it = m_actors.begin(); it != m_actors.end(); it++)
        if ((*(it))->alive()) {
            (*(it))->doSomething();
        }
    if (!getGhostRacer()->alive()) {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        for (it = m_actors.begin(); it != m_actors.end(); )
            it = m_actors.erase(it);
        return GWSTATUS_PLAYER_DIED;
    }
    if (m_soulsToSave <= 0) {
        increaseScore(m_bonus);
        playSound(SOUND_FINISHED_LEVEL);
        for (it = m_actors.begin(); it != m_actors.end(); )
            it = m_actors.erase(it);
        return GWSTATUS_FINISHED_LEVEL;
    }
    setLastWhiteY(getLastWhiteY()-4-getGhostRacer()->getSpeedY());
    for (it = m_actors.begin(); it != m_actors.end(); )
        if (!(*(it))->alive() || ((*(it))->isColliding() && (*(it))->getHealth() <= 0)) {
            (*it)->dead();
            delete* it;
            it = m_actors.erase(it);
        }
        else
            it++;
    if (VIEW_HEIGHT - SPRITE_HEIGHT - getLastWhiteY() >= SPRITE_HEIGHT) {
        m_actors.push_front(new BorderLine(this, IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2, VIEW_HEIGHT - SPRITE_HEIGHT));
        m_actors.push_front(new BorderLine(this, IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2, VIEW_HEIGHT - SPRITE_HEIGHT));
    }
    if (VIEW_HEIGHT - SPRITE_HEIGHT - getLastWhiteY() >= 4 * SPRITE_HEIGHT) {
        m_actors.push_front(new BorderLine(this, IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 6, VIEW_HEIGHT - SPRITE_HEIGHT));
        m_actors.push_front(new BorderLine(this, IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 6, VIEW_HEIGHT - SPRITE_HEIGHT));
        setLastWhiteY(256);
    }
    if (randInt(0, max(200 - getLevel() * 10, 30) - 1) == 0)
        m_actors.push_front(new HumanPedestrian(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
    if (randInt(0, max(100 - getLevel() * 10, 20) - 1) == 0)
        m_actors.push_front(new ZombiePedestrian(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
    if (randInt(0, max(150 - getLevel() * 10, 40) - 1) == 0)
        m_actors.push_front(new OilSlick(this, randInt(ROAD_CENTER - ROAD_WIDTH / 2, ROAD_CENTER + ROAD_WIDTH / 2), VIEW_HEIGHT));
    if (randInt(0, 100 + 10 * getLevel() - 1) == 0)
        m_actors.push_front(new RefillGoodie(this, randInt(ROAD_CENTER - ROAD_WIDTH / 2, ROAD_CENTER + ROAD_WIDTH / 2), VIEW_HEIGHT));
    if (randInt(0, 100 - 1) == 0)
        m_actors.push_front(new LostSoulGoodie(this, randInt(ROAD_CENTER - ROAD_WIDTH / 2, ROAD_CENTER + ROAD_WIDTH / 2), VIEW_HEIGHT));
    if (randInt(0, max(100 - getLevel() * 10, 20) - 1) == 0) {
        int x = randInt(0, 2); //which lane to examine first
        Actor* y;
        bool z;
        if (randInt(0, 1) == 0) //randomizes direction to loop through lanes, so it is purely random
            z = true;
        else
            z = false;
        for (int i = 0; i < 3; i++) {
            if (x % 3 == 0) //creates new ZombieCab in the given lane
                y = new ZombieCab(this, ROAD_CENTER - ROAD_WIDTH / 3, 0, 0);
            else if (x % 3 == 1)
                y = new ZombieCab(this, ROAD_CENTER, 0, 0);
            else
                y = new ZombieCab(this, ROAD_CENTER + ROAD_WIDTH / 3, 0, 0);
            if (closestCollisionWorthyActor(y, true, 0) == nullptr || closestCollisionWorthyActor(y, true, 0)->getY() > VIEW_HEIGHT / 3) { //if there isn't a close collidable actor near the bottom of current lane
                y->moveTo(y->getX(), SPRITE_HEIGHT / 2);
                y->setSpeedY(getGhostRacer()->getSpeedY() + randInt(2, 4));
                i = 3;
            }
            else if (closestCollisionWorthyActor(y, false, VIEW_HEIGHT) == nullptr || closestCollisionWorthyActor(y, false, VIEW_HEIGHT)->getY() < VIEW_HEIGHT * 2 / 3) { //if there isn't a close collidable actor near the top of current lane
                y->moveTo(y->getX(), VIEW_HEIGHT - SPRITE_HEIGHT / 2);
                y->setSpeedY(getGhostRacer()->getSpeedY() - randInt(2, 4));
                i = 3;
            }
            if (z)
                x++;
            else
                x--;
            if (i != 3) {
                delete y;
                y = nullptr;
            }
        }
        if (y != nullptr) {
            m_actors.push_front(y); 
        }
    }
    setGameStatText("Score: " + to_string(getScore()) + "  Lvl: " + to_string(getLevel()) + "  Souls2Save: " + to_string(m_soulsToSave) + "  Lives: " + to_string(getLives()) + "  Health: " + to_string(getGhostRacer()->getHealth()) + "  Sprays: " + to_string(getGhostRacer()->getSprays()) + "  Bonus: " + to_string(m_bonus));
    m_bonus--;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_ghostRacer;
    m_actors.clear();
}

GhostRacer* StudentWorld::getGhostRacer()
{
    return m_ghostRacer;
}

double StudentWorld::getLastWhiteY()
{
    return lastWhiteY;
}

void StudentWorld::setLastWhiteY(double y)
{
    lastWhiteY = y;
}

Actor* StudentWorld::closestCollisionWorthyActor(Actor* a, bool up, double y)
{
    list<Actor*> ::iterator it;
    double maxDiffY = VIEW_HEIGHT;
    Actor* x = nullptr;
    if (getGhostRacer()->getLane() == a->getLane() && ((up && getGhostRacer()->getY() - y > 0) || (!up && getGhostRacer()->getY() - y < 0)) && abs(getGhostRacer()->getY() - y) < maxDiffY) {
        maxDiffY = abs(getGhostRacer()->getY() - a->getY());
        x = getGhostRacer();
    }
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        if ((*it)->getLane() == a->getLane() && *it != a && (*it)->isColliding() && ((up && (*it)->getY() - y > 0) || (!up && (*it)->getY() - y < 0)) && abs((*it)->getY() - y) < maxDiffY) { //if there is a collidable actor in the right direction and closer than the last chosen collidable actor
            maxDiffY = abs((*it)->getY() - a->getY());
            x = *it;
        }
    }
    return x;
}

bool StudentWorld::spray(Actor* a)
{
    list<Actor*> ::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
        if ((*it)->isProjectileAffected() && a->isOverlapping(*it)) {
            if ((*it)->isColliding())
                (*(it))->sprayed();
            else
                (*it)->dead();
            return true;
        }
    return false;
}

void StudentWorld::addActor(Actor* a)
{
    m_actors.push_front(a);
}

void StudentWorld::changeScore(int score)
{
    m_score += score;
}

void StudentWorld::soulSaved()
{
    m_soulsToSave--;
}