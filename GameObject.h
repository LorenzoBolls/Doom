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

//derived classes of weapons
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

//only dropped when killing a monster
class MagicAxe : public Weapon
{
public:
    MagicAxe();
};

//only dropped when killing Sankewomen
class MagicFangsOfSleep : public Weapon
{
public:
    MagicFangsOfSleep();
    bool putToSleep(int& defenderSleepTime) const;
};

//Base scroll class
class Scroll : public GameObject
{
public:
    Scroll(const std::string& name);
    virtual ~Scroll();

    // Pure virtual function for the derived classes to implement specific use behavior
    //virtual void use(Actor* user) = 0;
};

// Derived classes of scrolls
//TO DO IMPLEMENT DERIVED SCROLLS
//Only drops when killed by a monster
class ScrollOfTeleportation : public Scroll
{
public:
    ScrollOfTeleportation();
    //void use(Actor* user) override;
};

class ScrollOfImproveArmor : public Scroll
{
public:
    ScrollOfImproveArmor();
   // void use(Actor* user) override;
};

class ScrollOfRaiseStrength : public Scroll
{
public:
    ScrollOfRaiseStrength();
    //void use(Actor* user) override;
};

class ScrollOfEnhanceHealth : public Scroll
{
public:
    ScrollOfEnhanceHealth();
   // void use(Actor* user) override;
};

class ScrollOfEnhanceDexterity : public Scroll
{
public:
    ScrollOfEnhanceDexterity();
  //  void use(Actor* user) override;
};


#endif /* GAMEOBJECTS_INCLUDED */
