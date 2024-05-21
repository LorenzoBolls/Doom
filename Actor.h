//
//  Actors.hpp
//  Project 3
//
//  Created by Lorenzo Bolls on 5/10/24.
//

#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include <stdio.h>
#include <string>
#include <vector>
#include "utilities.h"

using namespace std;

class Temple;

class GameObject;

class Weapon;

class Actor
{
public:
    Actor(Temple* temple, const std::string& name, int hitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int sleepTime, int row, int col);

    
    //accessors
    int getRow() const;
    int getCol() const;
    int getHitPoints() const;
    int getArmor() const;
    int getStrength() const;
    int getDexterity() const;
    int getSleepTime() const;
    string getName() const;
    Weapon* getWeapon() const;
    Temple* getTemple() const;
    
    //mutators/modifiers
    void setPosition(int row, int col);
    void setHitPoints(int points);
    void setArmor(int armor);
    void setStrength(int strength);
    void setDexterity(int dexterity);
    void setSleepTime(int sleepTime);
    void equipWeapon(Weapon* weapon);
    void setTemple(Temple* t);
    
    //actions
    virtual void attack(Actor* target);
    void takeDamage(int amount);
    void decreaseSleepTime();
    void move(char dir);
    
    virtual ~Actor();
    
private:
    string mName;
    int mRow, mCol;
    int mHitPoints, mArmor, mStrength, mDexterity, mSleepTime;
    Weapon* currentWeapon;
    Temple* mTemple;
};

class Player : public Actor
{
public:
    Player(Temple* temple, int row, int col);
    virtual ~Player();
    
    void regainHitPoint(int maxHitPoints);
    
    
    void attack(Actor* target);
    
    
    //TODO
    void grabObject();
    void wieldWeapon();
    void readScroll();
    void viewInventory();
    void descendStairs();
    void quitGame();
    void cheat();
    
    void addToInventory(GameObject* add);
    
    void move(char dir);
    
    bool getInventoryOpen();

    void setInventoryOpen(bool inv);
    
    
    vector<GameObject*> getInventory();
    

    
    //POSITION

private:
    vector<GameObject*> Inventory;
    bool inventoryOpen;
};

class Monster : public Actor
{
public:
    Monster(Temple* currTemple, const string& name, int hitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int row, int col);
    
    virtual void takeTurn(Player* player) = 0;
    virtual ~Monster();
};

class Bogeymen : public Monster
{
public:
    Bogeymen(Temple* currBogeymenTemple, int row, int col);
    virtual void takeTurn(Player* player);
};

class Snakewomen : public Monster
{};

class Dragon : public Monster
{};

class Goblins : public Monster
{};

#endif /* ACTOR_INCLUDED */
