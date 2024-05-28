//  Actors.cpp
//  Project 3
//
//  Created by Lorenzo Bolls on 5/10/24.
//

#include "Actor.h"
#include "utilities.h"
#include "GameObject.h"
#include <string>
#include "Temple.h"
#include <iostream>

//This section of code is for developing the base class Actor
Actor::Actor(Temple* temple, const std::string& name, int hitPoints, int maxHitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int sleepTime, int row, int col)
    : mTemple(temple), mName(name), mHitPoints(hitPoints), mMaxHitPoints(maxHitPoints), mArmor(armor), mStrength(strength), mDexterity(dexterity), mSleepTime(sleepTime), currentWeapon(currWeapon), mRow(row), mCol(col), mIsAttacking(false)
{}

int Actor::getRow() const
{
    return mRow;
}

int Actor::getCol() const
{
    return mCol;
}

int Actor::getHitPoints() const
{
    return mHitPoints;
}

int Actor::getMaxHitPoints() const
{
    return mMaxHitPoints;
}

int Actor::getArmor() const
{
    return mArmor;
}

int Actor::getStrength() const
{
    return mStrength;
}

int Actor::getDexterity() const
{
    return mDexterity;
}

int Actor::getSleepTime() const
{
    return mSleepTime;
}

std::string Actor::getName() const
{
    return mName;
}

Temple* Actor::getTemple() const
{
    return mTemple;
}

Weapon* Actor::getWeapon() const
{
    return currentWeapon;
}

bool Actor::isAttacking() const
{
    return mIsAttacking;
}

void Actor::setAttacking(bool attacking)
{
    mIsAttacking = attacking;
}

void Actor::setPosition(int row, int col)
{
    mRow = row;
    mCol = col;
}

void Actor::setHitPoints(int points)
{
    mHitPoints = points;
}

void Actor::setMaxHitPoints(int maxHitPoints) 
{
    mMaxHitPoints = maxHitPoints;
}

void Actor::setArmor(int armor)
{
    mArmor = armor;
}

void Actor::setStrength(int strength)
{
    mStrength = strength;
}

void Actor::setDexterity(int dexterity)
{
    mDexterity = dexterity;
}

void Actor::setSleepTime(int sleepTime)
{
    this->mSleepTime = sleepTime;
}

void Actor::equipWeapon(Weapon* weapon)
{
    if (currentWeapon != weapon) {
        /*delete currentWeapon;*/ // Delete the old weapon
        currentWeapon = weapon;
    }
}

string Actor::getScrollStatus() const
{
    return mScrollStatus;
}

string Actor::getAttackStatus() const
{
    return mAttackStatus;
}

//TODO FINISH IMPLEMENTING
void Actor::readScroll(Scroll* scroll)
{
    if (scroll->getName() == "scroll of teleportation")
    {
        mTemple->placePlayerRandomly();
        mScrollStatus = "\nYou feel your body wrenched in space and time.\n";
    }
    if (scroll->getName() == "scroll of improve armor")
    {
        /*
         CHECK IF IT CHANGES CHANCE TO HIT PLAYER
         CHANGECHANCE FOR HITTING PLAYER??
         
         */
        this->setArmor(mArmor + randInt(1, 3));
        mScrollStatus = "\nYour armor glows sliver.\n";
    }
    if (scroll->getName() == "scroll of raise strength")
    {
        this->setStrength(mStrength + randInt(1, 3));
        
        mScrollStatus = "\nYour muscles bulge.\n";
    }
    if (scroll->getName() == "scroll of enhance health")
    {
        this-> setMaxHitPoints(mMaxHitPoints + randInt(3, 8));
        mScrollStatus = "\nYou feel your heart beating stronger.\n";
    }
    if (scroll->getName() == "scroll of enhance dexterity")
    {
        this->setDexterity(mDexterity + 1);
        mScrollStatus = "\nYou feel like less of a klutz.\n";
    }
}

void Actor::setTemple(Temple* t)
{
    mTemple = t;
}

void Actor::attack(Actor* target)
{
    if (currentWeapon && target) //makes sure there is a valid weapon and a valid target
    {
        mIsAttacking = true;
        if (currentWeapon->attackHits(mDexterity, target->getDexterity(), target->getArmor()))
        {
            //DEBUGGING COUT
            //cout << endl << "ENTERING DAMAGE NOW" << endl;
            
            
            int damage = currentWeapon->calculateDamageAmount(mStrength);
            target->takeDamage(damage);
            
            //DEBUGGING COUT
            //cout << endl << "damaged applied";
            
            //special case for magic fangs of sleep
            MagicFangsOfSleep* magicFangs = dynamic_cast<MagicFangsOfSleep*>(currentWeapon);
            if (magicFangs)
            {
                magicFangs->putToSleep(target->mSleepTime);
            }
            
            mAttackStatus = " " + this->getWeapon()->getAction() + " at " + target->getName() + " and hits.\n";
        }
        else
        {
            mAttackStatus = " " + this->getWeapon()->getAction() + " at " + target->getName() + " and misses.\n";
        }
    }
    else
    {
        mIsAttacking = false;
    }
}



void Actor::regainHitPoint(int maxHitPoints)
{
    if(randInt(1, 10) == 1)
    {
        if(getHitPoints() < maxHitPoints)
        {
            setHitPoints(getHitPoints() + 1);
        }
    }
}

void Actor::move(char dir)
{
    char symbol = mName[0];
    int &row = mRow;
    int &col = mCol;
    if (mTemple->determineNewPosition(row, col, dir, symbol)) {
        setPosition(row, col);
    }
}

void Actor::takeDamage(int amount)
{
    mHitPoints -= amount;
}

void Actor::decreaseSleepTime()
{
    mSleepTime--;
}

Actor::~Actor()
{
}

