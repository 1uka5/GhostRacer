#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp



//class StudentWorld;
Actor::~Actor()
{
}

bool Actor::alive()
{
	return m_alive;
}

void Actor::dead()
{
	m_alive = false;
}

double Actor::getSpeedX()
{
	return m_speedX;
}

double Actor::getSpeedY()
{
	return m_speedY;
}

void Actor::setSpeedX(double speedX)
{
	m_speedX = speedX;
}

void Actor::setSpeedY(double speedY)
{
	m_speedY = speedY;
}

StudentWorld* Actor::getWorld()
{
	return m_studentWorld;
}

bool Actor::isColliding()
{
	return false;
}

int Actor::getLane()
{
	if (getX() >= ROAD_CENTER - ROAD_WIDTH / 2 && getX() < ROAD_CENTER - ROAD_WIDTH / 6) //0 is left most lane, 1 is middle lane, 2 is right most lane
		return 0;
	else if (getX() >= ROAD_CENTER - ROAD_WIDTH / 6 && getX() < ROAD_CENTER + ROAD_WIDTH / 6)
		return 1;
	else if (getX() >= ROAD_CENTER + ROAD_WIDTH / 6 && getX() < ROAD_CENTER + ROAD_WIDTH / 2)
		return 2;
	else
		return 3;
}

bool Actor::isOverlapping(Actor* a)
{
	return abs(getX() - a->getX()) < (getRadius() + a->getRadius()) * 0.25 && abs(getY() - a->getY()) < (getRadius() + a->getRadius()) * 0.6 && this != a;
}

bool Actor::outOfBounds()
{
	return (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT);
}



bool Actor::isProjectileAffected()
{
	return false;
}

void Actor::sprayed()
{
	dead();
}

int Actor::getHealth()
{
	return 1;
}

BorderLine::~BorderLine()
{
}

void BorderLine::doSomething()
{
	moveTo(getX() + getSpeedX(), getY() + getSpeedY() - getWorld()->getGhostRacer()->getSpeedY());
	if (outOfBounds())
		dead();
}

GhostRacer::~GhostRacer()
{
}

void GhostRacer::doSomething()
{
	if (alive()) {
		int x;
		if (getX() <= ROAD_CENTER - ROAD_WIDTH / 2) { //if wall is crashed into
			if (getDirection() > 90)
				changeHealth(-10);
			setDirection(82);
			getWorld()->playSound(SOUND_VEHICLE_CRASH);
		}
		else if (getX() >= ROAD_CENTER + ROAD_WIDTH / 2) {
			if (getDirection() < 90)
				changeHealth(-10);
			setDirection(98);
			getWorld()->playSound(SOUND_VEHICLE_CRASH);
		}
		else if (getWorld()->getKey(x)) //if key is pressed
			if (x == KEY_PRESS_LEFT && getDirection() < 114)
				setDirection(getDirection() + 8);
			else if (x == KEY_PRESS_RIGHT && getDirection() > 66)
				setDirection(getDirection() - 8);
			else if (x == KEY_PRESS_UP && getSpeedY() < 5)
				setSpeedY(getSpeedY() + 1);
			else if (x == KEY_PRESS_DOWN && getSpeedY() > -1)
				setSpeedY(getSpeedY() - 1);
			else if (x == KEY_PRESS_SPACE && getSprays() > 0) {
				getWorld()->addActor(new Spray(getWorld(), getX() + SPRITE_HEIGHT * cos(getDirection() * 3.14159 / 180), getY() + SPRITE_HEIGHT * sin(getDirection() * 3.14159 / 180), getDirection()));
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
				changeSprays(-1);
			}
		moveTo(getX() + cos(getDirection() * 3.14159 / 180) * 4, getY());
	}
	else
		return;
}

void GhostRacer::spin()
{
	if (randInt(0, 1) == 0) { //randomly changes direction
		setDirection(getDirection() - randInt(5, 20));
		if (getDirection() < 60)
			setDirection(60);
	}
	else {
		setDirection(getDirection() + randInt(5, 20));
		if (getDirection() > 120)
			setDirection(120);
	}
}

bool GhostRacer::isProjectileAffected()
{
	return false;
}

int CollidingActor::getHealth()
{
	return m_health;
}

void CollidingActor::setHealth(int health)
{
	m_health = health;
}

int GhostRacer::getSprays()
{
	return m_sprays;
}

void GhostRacer::changeSprays(int sprays)
{
	m_sprays += sprays;
}

CollidingActor::~CollidingActor()
{
}

bool CollidingActor::isColliding()
{
	return true;
}

void CollidingActor::changeHealth(int health)
{
	m_health += health;
}

void CollidingActor::sprayed()
{
	changeHealth(-1);
}

bool CollidingActor::isProjectileAffected()
{
	return true;
}

Pedestrian::~Pedestrian()
{
}

int Pedestrian::getMovementPlanDistance()
{
	return m_movementPlanDistance;
}

void Pedestrian::setMovementPlanDistance(int movementDistancePlan)
{
	m_movementPlanDistance = movementDistancePlan;
}

void Pedestrian::doSomething()
{
	moveTo(getX() + getSpeedX(), getY() + getSpeedY() - getWorld()->getGhostRacer()->getSpeedY());
	if (outOfBounds()) {
		dead();
		return;
	}
	setMovementPlanDistance(getMovementPlanDistance() - 1);
	if (getMovementPlanDistance() <= 0) {
		if (randInt(1, 2) == 1) {
			setSpeedX(randInt(-3, -1));
			setDirection(180);
		}
		else {
			setSpeedX(randInt(1, 3));
			setDirection(0);
		}
		setMovementPlanDistance(randInt(4, 32));
	}
}

HumanPedestrian::~HumanPedestrian()
{
}

void HumanPedestrian::doSomething()
{
	if (!alive())
		return;
	if (isOverlapping(getWorld()->getGhostRacer())) {
		dead();
		return;
	}
	Pedestrian::doSomething();
}

void HumanPedestrian::sprayed()
{
	setSpeedX(getSpeedX() * -1);
	if (getDirection() == 180)
		setDirection(0);
	else
		setDirection(180);
	getWorld()->playSound(SOUND_PED_HURT);
}

ZombiePedestrian::~ZombiePedestrian()
{
}

void ZombiePedestrian::doSomething()
{
	if (!alive())
		return;
	if (isOverlapping(getWorld()->getGhostRacer())) {
		getWorld()->getGhostRacer()->setHealth(getWorld()->getGhostRacer()->getHealth() - 5);
		dead();
		getWorld()->playSound(SOUND_PED_DIE);
		getWorld()->increaseScore(150);
		return;
	}
	if (abs(getX() - getWorld()->getGhostRacer()->getX()) < 30 && getY() > getWorld()->getGhostRacer()->getY()) { // if in right position relative to ghost racer
		setDirection(270);
		if (getX() < getWorld()->getGhostRacer()->getX())
			setSpeedX(1);
		else if (getX() > getWorld()->getGhostRacer()->getX())
			setSpeedX(-1);
		else
			setSpeedX(0);
		setTicksUntilNextGrunt(getTicksUntilNextGrunt() - 1);
		if (getTicksUntilNextGrunt() <= 0) {
			getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
			setTicksUntilNextGrunt(20);
		}
	}
	Pedestrian::doSomething();
}

int ZombiePedestrian::getTicksUntilNextGrunt()
{
	return m_ticksUntilNextGrunt;
}
void ZombiePedestrian::setTicksUntilNextGrunt(int ticksUntilNextGrunt)
{
	m_ticksUntilNextGrunt = ticksUntilNextGrunt;
}

void ZombiePedestrian::sprayed()
{
	CollidingActor::sprayed();
	if (getHealth() <= 0) {
		getWorld()->playSound(SOUND_PED_DIE);
		if (randInt(0, 4) == 0)
			getWorld()->addActor(new HealingGoodie(getWorld(), getX(), getY()));
		getWorld()->increaseScore(150);
	} else
		getWorld()->playSound(SOUND_PED_HURT);
}

void ZombiePedestrian::dead()
{
	Actor::dead();
}

OilSlick::~OilSlick()
{
}

void OilSlick::doSomething()
{
	moveTo(getX() + getSpeedX(), getY() + getSpeedY() - getWorld()->getGhostRacer()->getSpeedY());
	if (outOfBounds()) {
		dead();
		return;
	}
	if (isOverlapping(getWorld()->getGhostRacer())) {
		getWorld()->playSound(SOUND_OIL_SLICK);
		getWorld()->getGhostRacer()->spin();
	}
}

HealingGoodie::~HealingGoodie()
{
}

void HealingGoodie::doSomething()
{
	moveTo(getX() + getSpeedX(), getY() + getSpeedY() - getWorld()->getGhostRacer()->getSpeedY());
	if (outOfBounds()) {
		dead();
		return;
	}
	if (isOverlapping(getWorld()->getGhostRacer())) {
		getWorld()->getGhostRacer()->changeHealth(10);
		dead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(150);
	}
}

bool HealingGoodie::isProjectileAffected()
{
	return true;
}

RefillGoodie::~RefillGoodie()
{
}

void RefillGoodie::doSomething()
{
	moveTo(getX() + getSpeedX(), getY() + getSpeedY() - getWorld()->getGhostRacer()->getSpeedY());
	if (outOfBounds()) {
		dead();
		return;
	}
	if (isOverlapping(getWorld()->getGhostRacer())) {
		getWorld()->getGhostRacer()->changeSprays(10);
		dead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(50);
	}
}

bool RefillGoodie::isProjectileAffected()
{
	return true;
}

ZombieCab::~ZombieCab()
{
}

void ZombieCab::doSomething()
{
	if (alive()) {
		if (isOverlapping(getWorld()->getGhostRacer())) {
			getWorld()->playSound(SOUND_VEHICLE_CRASH);
			getWorld()->getGhostRacer()->changeHealth(-20);
			if (getX() <= getWorld()->getGhostRacer()->getX()) {
				setSpeedX(-5);
				setDirection(120 + randInt(0, 20));
			}
			else {
				setSpeedX(5);
				setDirection(60 - randInt(0, 20));
			}
			damaged();
		}
		moveTo(getX() + getSpeedX(), getY() + getSpeedY() - getWorld()->getGhostRacer()->getSpeedY());
		if (outOfBounds()) {
			dead();
			return;
		}
		if (getSpeedY() > getWorld()->getGhostRacer()->getSpeedY() && getWorld()->closestCollisionWorthyActor(this, true, getY()) != nullptr && abs(getWorld()->closestCollisionWorthyActor(this, true, getY())->getY() - getY()) < 96) { // if cab is moving up and there is a collision actor nearby in front
			setSpeedY(getSpeedY() - 0.5);
			return;
		}
		if (getSpeedY() < getWorld()->getGhostRacer()->getSpeedY() && getWorld()->closestCollisionWorthyActor(this, false, getY()) != nullptr && abs(getWorld()->closestCollisionWorthyActor(this, false, getY())->getY() - getY()) < 96) { // if cab is moving down and there is a collision actor nearby in front
			setSpeedY(getSpeedY() + 0.5);
			return;
		}
		setPlanLength(getPlanLength() - 1);
		if (getPlanLength() > 0)
			return;
		setPlanLength(randInt(4, 32));
		setSpeedY(getSpeedY() + randInt(-2, 2));
	}
}

int ZombieCab::getPlanLength()
{
	return m_planLength;
}

void ZombieCab::setPlanLength(int planLength)
{
	m_planLength = planLength;
}

bool ZombieCab::hasDamaged()
{
	return m_hasDamaged;
}

void ZombieCab::damaged()
{
	m_hasDamaged = true;
}

void ZombieCab::sprayed()
{
	CollidingActor::sprayed();
	if (getHealth() <= 0) {
		dead();
		getWorld()->playSound(SOUND_VEHICLE_DIE);
		if (randInt(0, 4) == 0)
			getWorld()->addActor(new OilSlick(getWorld(), getX(), getY()));
		getWorld()->increaseScore(200);
	} else
		getWorld()->playSound(SOUND_VEHICLE_HURT);
}

LostSoulGoodie::~LostSoulGoodie()
{
}

void LostSoulGoodie::doSomething()
{
	moveTo(getX() + getSpeedX(), getY() + getSpeedY() - getWorld()->getGhostRacer()->getSpeedY());
	if (outOfBounds()) {
		dead();
		return;
	}
	if (isOverlapping(getWorld()->getGhostRacer())) {
		getWorld()->soulSaved();
		dead();
		getWorld()->playSound(SOUND_GOT_SOUL);
		getWorld()->increaseScore(100);
	}
	setDirection(getDirection() - 10);
}

Spray::~Spray()
{
}

void Spray::doSomething()
{
	if (alive()) {
		if (getWorld()->spray(this)) {
			dead();
			return;
		}
		moveForward(SPRITE_HEIGHT);
		setPixelsTraveled(getPixelsTraveled() + SPRITE_HEIGHT);
		if (outOfBounds() || getPixelsTraveled() >= 160) {
			dead();
			return;
		}
	}
}

bool Spray::isActivated()
{
	return m_activated;
}

void Spray::activate()
{
	m_activated = true;
}

int Spray::getPixelsTraveled()
{
	return m_pixelsTraveled;
}

void Spray::setPixelsTraveled(int x)
{
	m_pixelsTraveled = x;
}

NonCollidingActor::~NonCollidingActor()
{
}
