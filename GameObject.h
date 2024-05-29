//
//  GameObjects.hpp
//  Project 3
//
//  Created by Lorenzo Bolls on 5/11/24.
//

#ifndef GAMEOBJECT_INCLUDED
#define GAMEOBJECT_INCLUDED

#include <stdio.h>
#include <string>

class Actor;

// Base class for all game objects
class GameObject
{
public:
    GameObject(const std::string& name);
    virtual ~GameObject();
    std::string getName() const;
    void setPosition(int x, int y);
    int getObjectCol() const;
    int getObjectRow() const;

private:
    std::string mName;
    int mObjectCol, mObjectRow;
};

// Base class for all weapons
class Weapon : public GameObject
{
public:
    Weapon(const std::string& name, const std::string& action, int dexterityBonus, int damageAmount);
    virtual ~Weapon();
    std::string getAction() const;
    int getDexterityBonus() const;
    int getWeaponDamageAmount() const;
    
    bool attackHits(int attackerDexterity, int defenderDexterity, int defenderArmor) const;
    int calculateDamageAmount(int attackerStrength) const;

private:
    std::string mAction;
    int mWeaponDexterityBonus;
    int mWeaponDamageAmount;
};

// Derived weapon classes
class Mace : public Weapon
{
public:
    Mace();
};

class ShortSword : public Weapon
{
public:
    ShortSword();
};

class LongSword : public Weapon
{
public:
    LongSword();
};


class MagicAxe : public Weapon
{
public:
    MagicAxe();
};


class MagicFangsOfSleep : public Weapon
{
public:
    MagicFangsOfSleep();
    bool putToSleep(int& defenderSleepTime) const;
};

// Base class for all scrolls
class Scroll : public GameObject
{
public:
    Scroll(const std::string& name);
    virtual ~Scroll();
};

// Special game object class
class GoldenIdol : public GameObject
{
public:
    GoldenIdol();
    virtual ~GoldenIdol();
};

// Derived scroll classes
class ScrollOfTeleportation : public Scroll
{
public:
    ScrollOfTeleportation();

};

class ScrollOfImproveArmor : public Scroll
{
public:
    ScrollOfImproveArmor();

};

class ScrollOfRaiseStrength : public Scroll
{
public:
    ScrollOfRaiseStrength();

};

class ScrollOfEnhanceHealth : public Scroll
{
public:
    ScrollOfEnhanceHealth();

};

class ScrollOfEnhanceDexterity : public Scroll
{
public:
    ScrollOfEnhanceDexterity();

};

#endif /* GAMEOBJECTS_INCLUDED */
