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

Monster::Monster(Temple* currTemple, const string& name, int hitPoints, Weapon* currWeapon, int armor, int strength, int dexterity, int row, int col) : Actor(currTemple, name, hitPoints, currWeapon, armor, strength, dexterity, 0, row, col)
{}

Monster::~Monster()
{}

Bogeymen::Bogeymen(Temple* currBogeymenTemple, int row, int col) : Monster(currBogeymenTemple, "Bogeyman", randInt(5,10), new ShortSword(), 2, randInt(2,3), randInt(2,3), row, col)
{}

void Bogeymen::takeTurn(Player* player) 
{
    int playerRow = player->getRow();
    int playerCol = player->getCol();

    //if adjacent to player, attack
    if (abs(playerRow - getRow()) + abs(playerCol - getCol()) == 1) 
    {
        attack(player);
        return;
    }

    //move closer to player if within smell range
    if (abs(playerRow - getRow()) <= 5 && abs(playerCol - getCol()) <= 5) 
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
