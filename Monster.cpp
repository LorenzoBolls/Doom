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

void Monster::die()
{
    int row = getRow();
    int col = getCol();

    // Check for item drop
    if (!getTemple()->isGameObjectAt(row, col)) {
        if (dynamic_cast<Bogeymen*>(this) && randInt(1, 10) == 1) {
            Weapon* magicAxe = new MagicAxe();
            magicAxe->setPosition(col, row);
            getTemple()->addItem(magicAxe);
        } else if (dynamic_cast<Snakewomen*>(this) && randInt(1, 3) == 1) {
            Weapon* magicFangs = new MagicFangsOfSleep();
            magicFangs->setPosition(col, row);
            getTemple()->addItem(magicFangs);
        }
    }

    // Clear the position on the grid
    getTemple()->clearPosition(row, col);

    // Remove from the temple's monster vector
    getTemple()->removeMonster(this);

    // Deallocate the weapon
    delete getWeapon();
}

Bogeymen::Bogeymen(Temple* currBogeymenTemple, int row, int col) : Monster(currBogeymenTemple, "Bogeyman", randInt(5,10), 10, new ShortSword(), 2, randInt(2,3), randInt(2,3), row, col, 5)
{
    
}

//
//void Bogeymen::takeTurn(Player* player)
//{
//    int playerRow = player->getRow();
//    int playerCol = player->getCol();
//
//    //if adjacent to player, attack
//    if (abs(playerRow - getRow()) + abs(playerCol - getCol()) == 1) 
//    {
//        //FOR DEBUGGING
//        //cout << endl << "bogeymen calculating whether to attack: " << endl;
//        attack(player);
//        return;
//    }
//
//    //move closer to player if within smell range
//    if (abs(playerRow - getRow()) <= 5 && abs(playerCol - getCol()) <= 5) 
//    {
//        if (playerRow > getRow() && !getTemple()->blockedPosition(getRow() + 1, getCol()))
//        {
//            move('j');
//        }
//        else if (playerRow < getRow() && !getTemple()->blockedPosition(getRow() - 1, getCol())) 
//        {
//            move('k');
//        }
//        else if (playerCol > getCol() && !getTemple()->blockedPosition(getRow(), getCol() + 1))
//        {
//            move('l');
//        }
//        else if (playerCol < getCol() && !getTemple()->blockedPosition(getRow(), getCol() - 1))
//        {
//            move('h');
//        }
//    }
//}

Snakewomen::Snakewomen(Temple* currSnakewomenTemple, int row, int col) : Monster(currSnakewomenTemple, "Snakewoman", randInt(3, 6), 6, new MagicFangsOfSleep, 3, 2, 3, row, col, 3)
{
    
}


Goblins::Goblins(Temple* currGoblinTemple, int row, int col) : Monster(currGoblinTemple, "Goblin", randInt(15, 20), 20, new ShortSword(), 1, 3, 1, row, col, 0)
{
}

Dragon::Dragon(Temple* currDragonTemple, int row, int col)
    : Monster(currDragonTemple, "Dragon", randInt(20,25), 25, new LongSword(), 4, 4, 4, row, col, 0)
{
}


bool Goblins::pathExistsToPlayer(char currTemple[18][70], int sR, int sC, int playerRow, int playerCol, int stepsTaken)
{
//    for (int r = 0; r < 18; r++)
//    {
//        for (int c = 0; c < 70; c++)
//        {
//            cout <<currTemple[r][c];
//        }
//        cout << endl;
//    }
    
    currTemple[playerRow][playerCol] = '!';
    
    bool east = false, south = false, west = false, north = false;
    
    if(stepsTaken > 0  && getTemple()->isNextToPlayer(this))
    {
        return true;
    }
//    if(stepsTaken >= 1 && getTemple()->isNextToPlayer(this))
    //has to be next to
    if(stepsTaken == 0 && !getTemple()->isNextToPlayer(this))
    {
        return false;
    }
//    if(sR == playerRow && sC == playerCol)
//    {
//        return true;
//    }
//    if (sR == playerRow && sC == playerCol)
//    {
//        return true;
//    }
    //EAST
    if (currTemple[playerRow][playerCol+1] == ' ')
    {
        east = pathExistsToPlayer(currTemple, sR, sC, playerRow, playerCol+1, stepsTaken-1);
//        if (pathExistsToPlayer(currTemple, sR, sC+1, playerRow, playerCol, stepsTaken-1))
//        {
//            return true;
//        }
    }
    //SOUTH
    if (currTemple[playerRow+1][playerCol] == ' ')
    {
        south =pathExistsToPlayer(currTemple, sR, sC, playerRow+1, playerCol, stepsTaken-1);
//        if (pathExistsToPlayer(currTemple, sR+1, sC, playerRow, playerCol, stepsTaken-1))
//        {
//            return true;
//        }
    }
    //WEST
    if (currTemple[playerRow][playerCol-1] == ' ')
    {
        west = pathExistsToPlayer(currTemple, sR, sC, playerRow, playerCol-1, stepsTaken-1);
//        if (pathExistsToPlayer(currTemple, sR, sC-1, playerRow, playerCol, stepsTaken-1))
//        {
//            return true;
//        }
    }
    //NORTH
    if (currTemple[playerRow-1][playerCol] == ' ')
    {
        north = pathExistsToPlayer(currTemple, sR, sC, playerRow-1, playerCol, stepsTaken-1);
//        if (pathExistsToPlayer(currTemple, sR-1, sC, playerRow, playerCol, stepsTaken-1))
//        {
//            return true;
//        }
    }
    return east || south || west || north;
}
//int Goblins::distanceToPlayer(Player* p, int row, int col)
//{
//    if (sr == er && sc == ec)
//    {
//        return true;
//    }
//    maze[sr][sc] = '@'; //indicates value has been visited
//    
//    Coord current = Coord(sr, sc);
//        
//    int r = current.r();
//    int c = current.c();
//    
//    //EAST
//    if (maze[r][c+1] == '.')
//    {
//        if (pathExists(maze, r, c+1, er, ec))
//        {
//            return true;
//        }
//    }
//    //SOUTH
//    if (maze[r+1][c] == '.')
//    {
//        if (pathExists(maze, r+1, c, er, ec))
//        {
//            return true;
//        }
//    }
//    //WEST
//    if (maze[r][c-1] == '.')
//    {
//        if (pathExists(maze, r, c-1, er, ec))
//        {
//            return true;
//        }
//    }
//    //NORTH
//    if (maze[r-1][c] == '.')
//    {
//        if (pathExists(maze, r-1, c, er, ec))
//        {
//            return true;
//        }
//    }
//    return false;
//}
//
//int Goblins::pathExistsToPlayer(char temple[18][70], int sr, int sc, int er, int ec, int moveTracker)
//{
//    //CALL HELPER FUNCTION IN EACH OF THE FOUR DIRECTIONS
//    
//    
//    
//}
