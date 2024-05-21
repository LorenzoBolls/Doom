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
Actor::Actor(Temple* temple, const std::string& name, int hitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int sleepTime, int row, int col)
    : mTemple(temple), mName(name), mHitPoints(hitPoints), mArmor(armor), mStrength(strength), mDexterity(dexterity), mSleepTime(sleepTime), currentWeapon(currWeapon), mRow(row), mCol(col)
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

void Actor::setPosition(int row, int col)
{
    mRow = row;
    mCol = col;
}

void Actor::setHitPoints(int points)
{
    mHitPoints = points;
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
        delete currentWeapon; // Delete the old weapon
        currentWeapon = weapon;
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
        if (currentWeapon->attackHits(mDexterity, target->getDexterity(), target->getArmor()))
        {
            int damage = currentWeapon->calculateDamageAmount(mStrength);
            target->takeDamage(damage);

            
            // Special case for magic fangs of sleep
            MagicFangsOfSleep* magicFangs = dynamic_cast<MagicFangsOfSleep*>(currentWeapon);
            if (magicFangs)
            {
                magicFangs->putToSleep(target->mSleepTime);
            }
        }
    }
    cout << "hello";
    int damage = currentWeapon->getWeaponDamageAmount();
    target->mHitPoints -= damage;
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
{}

