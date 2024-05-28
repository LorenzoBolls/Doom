//
//  Player.cpp
//  Project 3
//
//  Created by Lorenzo Bolls on 5/19/24.
//

#include "Actor.h"
#include "utilities.h"
#include "GameObject.h"
#include <string>
#include "Temple.h"
#include <iostream>
using namespace std;

//at runtime it will know actor's current position
//IMPORTANT row and col (last two parameters) ARE RANDOM INTS (3, 3), CHANGE LATER
//IMPORTANT change "short sword" to weapon after implementing weapon object
// starting conditions "Player", 20, "short sword", 2, 2, 2, 0, 3, 3
//CHANGED (3, 3) to row and col from the constructor parameters, BE CAREFUL AND CHANGE IT BACK IF IT DOESN'T WORK
Player::Player(Temple* playerTemple, int row, int col) : Actor(playerTemple, "Player", 20, 20, new ShortSword(), 2, 2, 2, 0, row, col)
{
    addToInventory(getWeapon());
}

Player::~Player()
{
}



//void Player::attack(Actor* attacker, Actor* target)
//{
//    int curr
//}


char Player::getLastDirection() const
{
    return lastDirection;
}



//void Player::attack(Actor* target)
//{
////    cout << "Player attacking target" << endl;
////    
////    if (getWeapon()->attackHits(getDexterity(), target->getDexterity(), target->getArmor()))
////    {
////        int damage = currentWeapon->calculateDamageAmount(mStrength);
////        target->takeDamage(damage);
////
////        
////        //special case for magic fangs of sleep
////        MagicFangsOfSleep* magicFangs = dynamic_cast<MagicFangsOfSleep*>(getWeapon());
////        if (magicFangs)
////        {
////            magicFangs->putToSleep(target->getSleepTime());
////        }
////    }
//}


void Player::move(char dir)
{
    char symbol = '@';
    int row = getRow();
    int col = getCol();
    lastDirection = dir;
    if (getTemple()->determineNewPosition(row, col, dir, symbol)) 
    {
        setPosition(row, col);
    }
}

/*
 when you click w, viewInventory(). Based on what user presses, find which pointer that character is referring to using a for loop. Then use dynamic cast to check if the pointer is a weapon or not:

ex

 dynamic_cast<Weapon* w>
 if (w == nullptr) it is not a weapon , else is a weapon and you equip it
 
 dynamic_cast<Scroll* s>
 if (s == nullptr) it is not a scroll, else is a scroll and you equip it
 */

void Player::addToInventory(GameObject* add)
{
    Inventory.push_back(add);
}


//HAVE TO CHECK IF LOGIC IS CORRECT
void Player::removeFromInventory(GameObject* remove)
{
    for (auto it = Inventory.begin(); it != Inventory.end(); )
    {
        if (*it == remove)
        {
            it = Inventory.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Player::viewInventory()
{
    clearScreen();
    cout << "Inventory: " << endl;
    char ind = 'a';
    for (auto i : Inventory) {
        if(ind <= 'z')
        {
            cout << (char)(ind) << ". " << i->getName() << endl;
            ind++;
        }
        
    }
}

//converts a to index 0, b to index 1 and so on
int Player::convertCharToInt(char val)
{
    return (int)(val) - (int)('a');
}

const vector<GameObject*>& Player::getInventory()
{
    return Inventory;
}

void Player::setInventory(vector<GameObject*> inv)
{
    Inventory = inv;
}


