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

Player::Player(Temple* playerTemple, int row, int col) : Actor(playerTemple, "Player", 20, 20, new ShortSword(), 2, 2, 2, 0, row, col)
{
    addToInventory(getWeapon());
}

Player::~Player()
{
}

char Player::getLastDirection() const
{
    return lastDirection;
}



void Player::move(char dir)
{
    if (isAsleep()) {
        decreaseSleepTime();
        return;
    }
    char symbol = '@';
    int row = getRow();
    int col = getCol();
    lastDirection = dir;
    if (getTemple()->determineNewPosition(row, col, dir, symbol)) 
    {
        setPosition(row, col);
    }
}



void Player::addToInventory(GameObject* add)
{
    int scrollsAndWeaponsCount = 0;
    for (auto item : Inventory)
    {
        if (dynamic_cast<Scroll*>(item) || dynamic_cast<Weapon*>(item))
        {
            scrollsAndWeaponsCount++;
        }
    }

    // Check if we can add scrolls or weapons
    if (scrollsAndWeaponsCount < 25 || (add->getName() == "the golden idol" && Inventory.size() < 26))
    {
        Inventory.push_back(add);
    }
}

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

int Player::getInventorySize() const
{
    return (int)Inventory.size();
}


