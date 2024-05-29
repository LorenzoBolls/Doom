// Actors.cpp
// Project 3
//
// Created by Lorenzo Bolls on 5/10/24.
//

#include "Actor.h"
#include "utilities.h"
#include "GameObject.h"
#include "Temple.h"
#include <string>
#include <iostream>

// Constructor
Actor::Actor(Temple* temple, const std::string& name, int hitPoints, int maxHitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int sleepTime, int row, int col)
    : mTemple(temple), mName(name), mHitPoints(hitPoints), mMaxHitPoints(maxHitPoints), mArmor(armor), mStrength(strength), mDexterity(dexterity), mSleepTime(sleepTime), currentWeapon(currWeapon), mRow(row), mCol(col), mIsAttacking(false)
{}

// Getters
int Actor::getRow() const { return mRow; }
int Actor::getCol() const { return mCol; }
int Actor::getHitPoints() const { return mHitPoints; }
int Actor::getMaxHitPoints() const { return mMaxHitPoints; }
int Actor::getArmor() const { return mArmor; }
int Actor::getStrength() const { return mStrength; }
int Actor::getDexterity() const { return mDexterity; }
int Actor::getSleepTime() const { return mSleepTime; }
std::string Actor::getName() const { return mName; }
Temple* Actor::getTemple() const { return mTemple; }
Weapon* Actor::getWeapon() const { return currentWeapon; }
bool Actor::isAttacking() const { return mIsAttacking; }
std::string Actor::getScrollStatus() const { return mScrollStatus; }
std::string Actor::getAttackStatus() const { return mAttackStatus; }

// Setters
void Actor::setAttacking(bool attacking) { mIsAttacking = attacking; }
void Actor::setPosition(int row, int col) { mRow = row; mCol = col; }
void Actor::setHitPoints(int points) { mHitPoints = points; }
void Actor::setMaxHitPoints(int maxHitPoints) { mMaxHitPoints = maxHitPoints; }
void Actor::setArmor(int armor) { mArmor = armor; }
void Actor::setStrength(int strength) { mStrength = strength; }
void Actor::setDexterity(int dexterity) { mDexterity = dexterity; }
void Actor::setSleepTime(int sleepTime) { mSleepTime = sleepTime; }
void Actor::setTemple(Temple* t) { mTemple = t; }

// Equip a new weapon
void Actor::equipWeapon(Weapon* weapon) {
    if (currentWeapon != weapon) {
        // Uncomment if you want to delete the old weapon
        // delete currentWeapon;
        currentWeapon = weapon;
    }
}

// Read a scroll and apply its effects
void Actor::readScroll(Scroll* scroll) {
    if (scroll->getName() == "scroll of teleportation") {
        mTemple->placePlayerRandomly();
        mScrollStatus = "\nYou feel your body wrenched in space and time.\n";
    }
    else if (scroll->getName() == "scroll of improve armor") {
        setArmor(mArmor + randInt(1, 3));
        mScrollStatus = "\nYour armor glows silver.\n";
    }
    else if (scroll->getName() == "scroll of raise strength") {
        setStrength(mStrength + randInt(1, 3));
        mScrollStatus = "\nYour muscles bulge.\n";
    }
    else if (scroll->getName() == "scroll of enhance health") {
        setMaxHitPoints(mMaxHitPoints + randInt(3, 8));
        mScrollStatus = "\nYou feel your heart beating stronger.\n";
    }
    else if (scroll->getName() == "scroll of enhance dexterity") {
        setDexterity(mDexterity + 1);
        mScrollStatus = "\nYou feel like less of a klutz.\n";
    }
}

// Attack another actor
void Actor::attack(Actor* target) {
    if (currentWeapon && target) {
        mIsAttacking = true;
        if (currentWeapon->attackHits(mDexterity, target->getDexterity(), target->getArmor())) {
            int damage = currentWeapon->calculateDamageAmount(mStrength);
            target->takeDamage(damage);

            // Check if currentWeapon is a MagicFangsOfSleep
            MagicFangsOfSleep* magicFangs = dynamic_cast<MagicFangsOfSleep*>(currentWeapon);
            if (magicFangs) {
                if (magicFangs->putToSleep(target->mSleepTime)) {
                    mAttackStatus = " " + getWeapon()->getAction() + " at " + target->getName() + " and hits, putting " + target->getName() + " to sleep.\n";
                } else {
                    mAttackStatus = " " + getWeapon()->getAction() + " at " + target->getName() + " and hits.\n";
                }
            } else {
                mAttackStatus = " " + getWeapon()->getAction() + " at " + target->getName() + " and hits.\n";
            }

            // Check if the target is dead
            if (target->getHitPoints() <= 0) {
                if (target->getName() == "Player") {
                    mAttackStatus = " " + getWeapon()->getAction() + " at " + target->getName() + " and hits, dealing a final blow.\n";
                } else {
                    mAttackStatus += "dealing a final blow.\n";
                }
            }
        } else {
            mAttackStatus = " " + getWeapon()->getAction() + " at " + target->getName() + " and misses.\n";
        }
    } else {
        mIsAttacking = false;
    }
}

// Regain hit points with a chance
void Actor::regainHitPoint(int maxHitPoints) {
    if (randInt(1, 10) == 1 && getHitPoints() < maxHitPoints) {
        setHitPoints(getHitPoints() + 1);
    }
}

// Move the actor in a direction
void Actor::move(char dir) {
    if (isAsleep()) {
        decreaseSleepTime();
        return;
    }
    char symbol = mName[0];
    int& row = mRow;
    int& col = mCol;
    if (mTemple->determineNewPosition(row, col, dir, symbol)) {
        setPosition(row, col);
    }
}

// Take damage and reduce hit points
void Actor::takeDamage(int amount) {
    mHitPoints -= amount;
}

// Decrease sleep time
void Actor::decreaseSleepTime() {
    mSleepTime--;
}

// Check if the actor is asleep
bool Actor::isAsleep() const {
    return mSleepTime > 0;
}

// Destructor
Actor::~Actor() {
//    delete currentWeapon;
    // Ensure the weapon is not double deleted in the inventory
}
