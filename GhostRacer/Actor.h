#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/*
Ghost Racer
Human pedestrians
Zombie pedestrians
Zombie cabs
Oil slicks
Holy water projectiles (fired by Aisha from Ghost Racer)
Border lines (yellow and white)
Lost souls
Healing goodies
Holy water bottle (refill) goodies
*/

/*
//hw projectile
//soul goodie
//oil slick
//borderline
//ghost racer
spray
*/

class StudentWorld;
class Actor : public GraphObject {
public:
	Actor(StudentWorld* studentWorld, int imageID, double startX, double startY, double speedX, double speedY, int dir = 0, double size = 1.0, unsigned int depth = 0) : GraphObject(imageID, startX, startY, dir, size, depth), m_speedX(speedX), m_speedY(speedY) { m_studentWorld = studentWorld; }
	virtual ~Actor();
	virtual bool alive();
	void dead();
	virtual double getSpeedX();
	virtual double getSpeedY();
	virtual void setSpeedX(double speedX);
	virtual void setSpeedY(double speedY);
	virtual void doSomething() = 0;
	virtual StudentWorld* getWorld();
	virtual bool isColliding();
	virtual int getLane();
	virtual bool isOverlapping(Actor* a);
	virtual bool outOfBounds();
	virtual bool isProjectileAffected();
	virtual void sprayed();
	virtual int getHealth();
private:
	StudentWorld* m_studentWorld;
	double m_speedX;
	double m_speedY;
	bool m_alive = true;
};

class BorderLine : public Actor {
public:
	BorderLine(StudentWorld* studentWorld, int imageID, int startX, int startY) : Actor(studentWorld, imageID, startX, startY, 0, -4, 0, 2, 2) {}
	virtual ~BorderLine();
	virtual void doSomething();
};

class CollidingActor : public Actor {
public:
	CollidingActor(StudentWorld* studentWorld, int imageID, double startX, double startY, double speedX, double speedY, int health, int dir, double size, unsigned int depth) : Actor(studentWorld, imageID, startX, startY, speedX, speedY, dir, size, depth), m_health(health) {}
	virtual ~CollidingActor();
	virtual bool isColliding();
	virtual void setHealth(int health);
	virtual bool isProjectileAffected();
	virtual int getHealth();
	virtual void changeHealth(int health);
	virtual void sprayed();
private:
	int m_health;
};

class GhostRacer : public CollidingActor {
public:
	GhostRacer(StudentWorld* studentWorld) : CollidingActor(studentWorld, IID_GHOST_RACER, 128, 32, 0, 0, 100, 90, 4, 0) {}
	virtual ~GhostRacer();
	virtual void doSomething();
	virtual void spin();
	virtual bool isProjectileAffected();
	virtual int getSprays();
	virtual void changeSprays(int sprays);
private:
	int m_sprays = 10;
};

class Pedestrian : public CollidingActor {
public:
	Pedestrian(StudentWorld* studentWorld, int imageID, double startX, double startY, double size) : CollidingActor(studentWorld, imageID, startX, startY, 0, -4, 2, 0, size, 0) {}
	virtual ~Pedestrian();
	virtual int getMovementPlanDistance();
	virtual void setMovementPlanDistance(int movementDistancePlan);
	virtual void doSomething();
private:
	int m_movementPlanDistance = 0;
};

class HumanPedestrian : public Pedestrian {
public:
	HumanPedestrian(StudentWorld* studentWorld, double startX, double startY) : Pedestrian(studentWorld, IID_HUMAN_PED, startX, startY, 2) {}
	virtual ~HumanPedestrian();
	virtual void doSomething();
	virtual void sprayed();
};

class ZombiePedestrian : public Pedestrian {
public:
	ZombiePedestrian(StudentWorld* studentWorld, double startX, double startY) : Pedestrian(studentWorld, IID_ZOMBIE_PED, startX, startY, 3) {}
	virtual ~ZombiePedestrian();
	virtual void doSomething();
	virtual int getTicksUntilNextGrunt();
	virtual void setTicksUntilNextGrunt(int ticksUntilNextGrunt);
	virtual void sprayed();
	virtual void dead();
private:
	int m_ticksUntilNextGrunt = 0;
};

class ZombieCab : public CollidingActor {
public:
	ZombieCab(StudentWorld* studentWorld, double startX, double startY, double speedY) : CollidingActor(studentWorld, IID_ZOMBIE_CAB, startX, startY, 0, speedY, 3, 90, 4, 0) {}
	virtual ~ZombieCab();
	virtual void doSomething();
	virtual int getPlanLength();
	virtual void setPlanLength(int planLength);
	virtual bool hasDamaged();
	virtual void damaged();
	virtual void sprayed();
private:
	int m_planLength = 0;
	bool m_hasDamaged = false;
};

class NonCollidingActor : public Actor {
public:
	NonCollidingActor(StudentWorld* studentWorld, int imageID, double startX, double startY, double speedX, double speedY, int dir, double size, unsigned int depth) : Actor(studentWorld, imageID, startX, startY, speedX, speedY, dir, size, depth) {}
	virtual ~NonCollidingActor();
};

class OilSlick : public NonCollidingActor {
public:
	OilSlick(StudentWorld* studentWorld, double startX, double startY) : NonCollidingActor(studentWorld, IID_OIL_SLICK, startX, startY, 0, -4, 0, randInt(2, 5), 2) {}
	virtual ~OilSlick();
	virtual void doSomething();
};

class HealingGoodie : public NonCollidingActor {
public:
	HealingGoodie(StudentWorld* studentWorld, double startX, double startY) : NonCollidingActor(studentWorld, IID_HEAL_GOODIE, startX, startY, 0, -4, 0, 1, 2) {}
	virtual ~HealingGoodie();
	virtual void doSomething();
	virtual bool isProjectileAffected();
};

class RefillGoodie : public NonCollidingActor {
public:
	RefillGoodie(StudentWorld* studentWorld, double startX, double startY) : NonCollidingActor(studentWorld, IID_HOLY_WATER_GOODIE, startX, startY, 0, -4, 90, 2, 2) {}
	virtual ~RefillGoodie();
	virtual void doSomething();
	virtual bool isProjectileAffected();
};

class LostSoulGoodie : public NonCollidingActor {
public:
	LostSoulGoodie(StudentWorld* studentWorld, double startX, double startY) : NonCollidingActor(studentWorld, IID_SOUL_GOODIE, startX, startY, 0, -4, 0, 4, 2) {}
	virtual ~LostSoulGoodie();
	virtual void doSomething();
};

class Spray : public Actor {
public:
	Spray(StudentWorld* studentWorld, double startX, double startY, int dir) : Actor(studentWorld, IID_HOLY_WATER_PROJECTILE, startX, startY, 0, 0, dir, 1, 1) {}
	virtual ~Spray();
	virtual void doSomething();
	virtual bool isActivated();
	virtual void activate();
	virtual int getPixelsTraveled();
	virtual void setPixelsTraveled(int x);
private:
	bool m_activated = false;
	int m_pixelsTraveled = 0;
};
#endif // ACTOR_H_