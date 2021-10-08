#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class BorderLine;
class GhostRacer;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    GhostRacer* getGhostRacer();
    double getLastWhiteY();
    void setLastWhiteY(double y);
    Actor* closestCollisionWorthyActor(Actor* a, bool up, double y); //is y necessary?? (probably not)
    bool spray(Actor* a);
    void addActor(Actor* a);
    void changeScore(int score);
    void soulSaved();
private:
    std::list <Actor*> m_actors;
    std::list <Actor*> m_actorscopy;
    GhostRacer* m_ghostRacer;
    double lastWhiteY;
    int m_score = 0;
    int m_soulsToSave;
    int m_bonus = 5000;
};

#endif // STUDENTWORLD_H_