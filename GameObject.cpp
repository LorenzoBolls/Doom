//
//  GameObjects.cpp
//  Project 3
//
//  Created by Lorenzo Bolls on 5/11/24.
//

#include "GameObject.h"
#include <string>
#include "utilities.h"
#include <iostream>
using namespace std;

//GameObject class implementation
GameObject::GameObject(const string& name) : mName(name)
{}

GameObject::~GameObject()
{}

string GameObject::getName() const
{
    return mName;
}

void GameObject::setPosition(int x, int y)
{
    mObjectCol = x;
    mObjectRow = y;
}

int GameObject::getObjectCol() const
{
    return mObjectCol;
}

int GameObject::getObjectRow() const
{
    return mObjectRow;
}

//Weapon class implementation
Weapon::Weapon(const string& name, const string& action, int dexterityBonus, int damageAmount) : GameObject(name), mAction(action), mWeaponDexterityBonus(dexterityBonus), mWeaponDamageAmount(damageAmount)
{}
Weapon:: ~Weapon()
{}

string Weapon::getAction() const
{
    return mAction;
}

int Weapon::getWeaponDamageAmount() const
{
    return mWeaponDamageAmount;
}

int Weapon::getDexterityBonus() const
{
    return mWeaponDexterityBonus;
}

bool Weapon::attackHits(int attackerDexterity, int defenderDexterity, int defenderArmor) const
{
    int attackerPoints = attackerDexterity + mWeaponDexterityBonus;
    int defenderPoints = defenderDexterity + defenderArmor;
    return attackerPoints >= defenderPoints;
}

int Weapon::calculateDamageAmount(int attackerStrength) const
{
    return randInt(0, attackerStrength + mWeaponDamageAmount - 1);
}

//Weapon derived classes
Mace::Mace() : Weapon("mace", "swings", 0, 2)
{}

ShortSword::ShortSword() : Weapon("short sword", "slashes", 0, 2)
{}

LongSword::LongSword() : Weapon("long sword", "swings", 2, 4)
{}

MagicAxe::MagicAxe() : Weapon("magic axe", "chops", 5, 5)
{}

MagicFangsOfSleep::MagicFangsOfSleep() : Weapon("magic fangs of sleep", "strikes", 3, 2)
{}

bool MagicFangsOfSleep::putToSleep(int& defenderSleepTime) const
{
    if (randInt(1, 5) == 1)
    {
        int newSleepTime = randInt(2,6);
        
        //updates defender's sleep time to max of current sleep time and new sleep time
        defenderSleepTime = max(defenderSleepTime, newSleepTime);
        return true;
    }
    return false;
}


//Scroll class Implementation
Scroll::Scroll(const string& name) : GameObject(name)
{}

Scroll::~Scroll()
{}

ScrollOfTeleportation::ScrollOfTeleportation() : Scroll("scroll of teleportation")
{}

ScrollOfImproveArmor::ScrollOfImproveArmor() : Scroll("scroll of improved armor")
{}

ScrollOfEnhanceHealth::ScrollOfEnhanceHealth() : Scroll("scroll of enhance health")
{}

ScrollOfEnhanceDexterity::ScrollOfEnhanceDexterity() : Scroll("scroll of enhance dexterity")
{}

ScrollOfRaiseStrength::ScrollOfRaiseStrength() : Scroll("scroll of raise strength")
{}

//derived scroll classes
