//
//  Monster.cpp
//  Project 3
//
//  Created by Lorenzo Bolls on 5/19/24.
//

#include "Actor.h"
#include "utilities.h"
#include "GameObject.h"
#include <string>
#include "Temple.h"
#include <cmath>

#include<iostream>

Monster::Monster(Temple* currTemple, const string& name, int hitPoints, int maxHitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int row, int col, int smellDistance) : Actor(currTemple, name, hitPoints, maxHitPoints, currWeapon, armor, strength, dexterity, 0, row, col), mSmellDistance(smellDistance)
{
    
}

Monster::~Monster()
{
}

int Monster::getSmellDistance() const
{
    return mSmellDistance;
}


void Monster::takeTurn(Player* player)
{
    if (getHitPoints() <= 0) 
    {
        die();

        return;
    }
    if (isAsleep()) {
        decreaseSleepTime();
        return;
    }
    
    int playerRow = player->getRow();
    int playerCol = player->getCol();

    //if adjacent to player, attack
    if (abs(playerRow - getRow()) + abs(playerCol - getCol()) == 1)
    {
        //FOR DEBUGGING
        //cout << endl << "bogeymen calculating whether to attack: " << endl;
        attack(player);
        return;
    }

    //move closer to player if within smell range
    if (abs(playerRow - getRow()) <= getSmellDistance() && abs(playerCol - getCol()) <= getSmellDistance())
    {
        if (playerRow > getRow() && !getTemple()->blockedPosition(getRow() + 1, getCol()))
        {
            move('j');
        }
        else if (playerRow < getRow() && !getTemple()->blockedPosition(getRow() - 1, getCol()))
        {
            move('k');
        }
        else if (playerCol > getCol() && !getTemple()->blockedPosition(getRow(), getCol() + 1))
        {
            move('l');
        }
        else if (playerCol < getCol() && !getTemple()->blockedPosition(getRow(), getCol() - 1))
        {
            move('h');
        }
    }
}

void Monster::die() {
    int row = getRow();
    int col = getCol();

    // Check for item drop
    if (!getTemple()->isGameObjectAt(row, col)) 
    {
        if (dynamic_cast<Bogeymen*>(this) && randInt(1, 10) == 1) 
        {
            Weapon* magicAxe = new MagicAxe();
            magicAxe->setPosition(col, row);
            getTemple()->addItem(magicAxe);
        } 
        else if (dynamic_cast<Snakewomen*>(this) && randInt(1, 3) == 1)
        {
            Weapon* magicFangs = new MagicFangsOfSleep();
            magicFangs->setPosition(col, row);
            getTemple()->addItem(magicFangs);
        } 
        else if (dynamic_cast<Goblins*>(this) && randInt(1, 3) == 1) 
        {
            Weapon* weaponDrop = nullptr;
            if (randInt(1, 2) == 1) 
            {
                weaponDrop = new MagicAxe();
            } 
            else
            {
                weaponDrop = new MagicFangsOfSleep();
            }
            weaponDrop->setPosition(col, row);
            getTemple()->addItem(weaponDrop);
        }
        else if (dynamic_cast<Dragon*>(this)) {
            // Always drop a scroll for a dragon
            Scroll* newScroll = getTemple()->generateRandomScroll();
            if (newScroll) 
            {
                newScroll->setPosition(col, row);
                getTemple()->addItem(newScroll);
            }
        }
    }

    // Clear the position on the grid
    getTemple()->clearPosition(row, col);

    // Deallocate the weapon
    delete getWeapon();
    
    // Remove from the temple's monster vector
    getTemple()->removeMonster(this);

}
Bogeymen::Bogeymen(Temple* currBogeymenTemple, int row, int col) : Monster(currBogeymenTemple, "Bogeyman", randInt(5,10), 10, new ShortSword(), 2, randInt(2,3), randInt(2,3), row, col, 5)
{
    
}


Snakewomen::Snakewomen(Temple* currSnakewomenTemple, int row, int col) : Monster(currSnakewomenTemple, "Snakewoman", randInt(3, 6), 6, new MagicFangsOfSleep, 3, 2, 3, row, col, 3)
{
    
}

// set 15 to 0 or 0 to 15
Goblins::Goblins(Temple* currGoblinTemple, int row, int col) : Monster(currGoblinTemple, "Goblin", randInt(15, 20), 20, new ShortSword(), 1, 3, 1, row, col, 15)
{
}

void Monster::goblinTakeTurn(Player* player) {
    if (getHitPoints() <= 0) {
        die();
        return;
    }

    int playerRow = player->getRow();
    int playerCol = player->getCol();

    // If adjacent to player, attack
    if (abs(playerRow - getRow()) + abs(playerCol - getCol()) == 1) {
        attack(player);
        return;
    }

    // Move closer to player if within smell range
    if (abs(playerRow - getRow()) <= getSmellDistance() && abs(playerCol - getCol()) <= getSmellDistance()) {
        char bestDirection = getTemple()->determineBestDirectionForGoblin(getRow(), getCol(), playerRow, playerCol, getSmellDistance());
        if (bestDirection != ' ') {
            move(bestDirection);
        }
    }
}

Dragon::Dragon(Temple* currDragonTemple, int row, int col)
    : Monster(currDragonTemple, "Dragon", randInt(20,25), 25, new LongSword(), 4, 4, 4, row, col, 0)
{
}

