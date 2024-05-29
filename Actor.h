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

class Scroll;

class Actor
{
public:
    Actor(Temple* temple, const std::string& name, int hitPoints, int maxHitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int sleepTime, int row, int col);

    
    //accessors
    int getRow() const;
    int getCol() const;
    int getHitPoints() const;
    int getMaxHitPoints() const;
    int getArmor() const;
    int getStrength() const;
    int getDexterity() const;
    int getSleepTime() const;
    string getName() const;
    Weapon* getWeapon() const;
    Temple* getTemple() const;
    string getScrollStatus() const;
    string getAttackStatus() const;

    //mutators/modifiers
    void setPosition(int row, int col);
    void setHitPoints(int points);
    void setMaxHitPoints(int maxHitPoints);
    void setArmor(int armor);
    void setStrength(int strength);
    void setDexterity(int dexterity);
    void setSleepTime(int sleepTime);
    void equipWeapon(Weapon* weapon);
    void setTemple(Temple* t);
    void readScroll(Scroll* scroll);
    void setAttacking(bool attacking);
    
    //actions
    virtual void attack(Actor* target);
    void takeDamage(int amount);
    void decreaseSleepTime();
    void move(char dir);
    
    bool isAttacking() const;
    bool isAsleep() const;
    
    void regainHitPoint(int maxHitPoints);
    
    virtual ~Actor();
    
private:
    string mName;
    string mScrollStatus, mAttackStatus;
    bool mIsAttacking;
    int mRow, mCol;
    int mHitPoints, mMaxHitPoints, mArmor, mStrength, mDexterity, mSleepTime;
    Weapon* currentWeapon;
    Temple* mTemple;
};

class Player : public Actor
{
public:
    Player(Temple* temple, int row, int col);
    virtual ~Player();
    
    
    //void attack(Actor* target);
    
    
    //TODO
    void grabObject();
    void viewInventory();
    void descendStairs();
    void quitGame();
    void cheat();
    
    void addToInventory(GameObject* add);
    
    int getInventorySize() const;
    
    void move(char dir);
    
    int convertCharToInt(char val);
    
    const vector<GameObject*>& getInventory();
    void setInventory(vector<GameObject*> inv);
    void removeFromInventory(GameObject* remove);
    
    char getLastDirection() const;

private:
    vector<GameObject*> Inventory;
    char lastDirection;
};

class Monster : public Actor
{
public:
    Monster(Temple* currTemple, const string& name, int hitPoints, int maxHitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int row, int col, int mSmellDistance);
    
    virtual void takeTurn(Player* player);
    virtual ~Monster();
    void setSmellDistance(int distance);
    int getSmellDistance() const;
    void die();
    void goblinTakeTurn(Player* player);
    
private:
    int mSmellDistance;
};

class Bogeymen : public Monster
{
public:
    Bogeymen(Temple* currBogeymenTemple, int row, int col);

};

class Snakewomen : public Monster
{
public:
    Snakewomen(Temple* currSnakewomenTemple, int row, int col);
    
};

class Dragon : public Monster
{
public:
    Dragon(Temple* currDragonTemple, int row, int col);
};

class Goblins : public Monster
{
public:
    Goblins(Temple* currGoblinTemple, int row, int col);
};

#endif /* ACTOR_INCLUDED */
