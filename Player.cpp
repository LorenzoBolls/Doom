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
Player::Player(Temple* playerTemple, int row, int col) : Actor(playerTemple, "Player", 20, new ShortSword(), 2, 2, 2, 0, row, col)
{
    addToInventory(getWeapon());
    inventoryOpen = false;
}

Player::~Player()
{}

void Player::regainHitPoint(int maxHitPoints)
{
    if(randInt(1, 10) == 1)
    {
        if(getHitPoints() < maxHitPoints)
        {
            setHitPoints(getHitPoints() + 1);
        }
    }
}

//void Player::attack(Actor* attacker, Actor* target)
//{
//    int curr
//}

void Player::attack(Actor* target)
{
    int currRow = getRow();
    int currCol = getCol();
    
    Weapon* currWeapon = getWeapon();
    
    //makes sure player holds a weapon before
    if (!currWeapon)
        return;
    
    int damage = currWeapon->getWeaponDamageAmount();
    
    if (target->getRow() == currRow-- && ARROW_LEFT)
    {
        //subtracts targets HitPoints by Damage
        target->setHitPoints(target->getHitPoints() - damage);
    }
    
    if (target->getRow() == currRow++ && ARROW_RIGHT)
    {
        target->setHitPoints(target->getHitPoints() - damage);
    }
    
    if (target->getCol() == currCol++ && ARROW_UP)
    {
        target->setHitPoints(target->getHitPoints() - damage);
    }
    
    if (target->getCol() == currCol-- && ARROW_DOWN)
    {
        target->setHitPoints(target->getHitPoints() - damage);
    }
}

void Player::move(char dir)
{
    char symbol = '@';
    int row = getRow();
    int col = getCol();
    setInventoryOpen(false);
    if (getTemple()->determineNewPosition(row, col, dir, symbol)) {
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

vector<GameObject*> Player::getInventory()
{
    return Inventory;
}

bool Player::getInventoryOpen()
{
    return inventoryOpen;
}
void Player::setInventoryOpen(bool inv)
{
    inventoryOpen = inv;
}
