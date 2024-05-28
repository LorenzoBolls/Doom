//
//  Temple.h
//  Project 3
//
//  Created by Lorenzo Bolls on 5/11/24.
//

#ifndef TEMPLE_INCLUDED
#define TEMPLE_INCLUDED

#include <stdio.h>
#include "Actor.h"
#include "GameObject.h"
#include <vector>
using namespace std;

const int MIN_ROOM_WIDTH = 7;
const int MIN_ROOM_HEIGHT = 7;
const int MAX_ROOM_WIDTH = 16;
const int MAX_ROOM_HEIGHT = 10;

const int MIN_HALL_LENGTH = 1;
const int MAX_HALL_LENGTH = 10;

const int TEMPLE_COLUMNS = 70;
const int TEMPLE_ROWS = 18;

class Player;

class Temple
{
public:
    Temple(int goblinSmellDistance, int level);
    ~Temple();

    void display() const;
    bool determineNewPosition(int& r, int& c, char dir, char symbol);
    
    
    Player* getPlayer() const;
    int getLevel() const;
    void setLevel(int level);
    
    void placePlayerRandomly();
    void placeDescendPoint();
    
    void generateMonsters();
    
    bool blockedPosition(int r, int c);
    bool isDescendPosition(int r, int c);
    bool isGameObjectAt(int r, int c);
    
    void moveMonsters(Player* player);
    
    
    void placeMultipleGameObjects();
    
    void placeWeaponsRandomly();
//    void placeMultipleWeapons();
    
    void placeScrollsRandomly();
//    void placeMultipleScrolls();
    
    GameObject* findItems(int r, int c);

    void removeItems(int r, int c);

    bool isScroll(GameObject* item) const;
    
    void playerAttack();
    
    //DEBUGGING
    Monster* getMonster(int index) const;

    string getAllMonstersAttackStatus();
    void resetMonstersAttackingStatus();
    bool isAnyMonsterAttacking() const;

    void generateRooms();
    bool isAreaClear(int startRow, int startCol, int height, int width);
    void placeRoom(int startRow, int startCol, int height, int width);
    
    void generateUnblockedPosition(int& r, int& c);
    
    void addItem(GameObject* item);
    void clearPosition(int r, int c);
    void removeMonster(Monster* monster);
    
    bool isNextToPlayer(Monster* monster);
    
    void copyTemple(const char temple[TEMPLE_ROWS][TEMPLE_COLUMNS], char destination[TEMPLE_ROWS][TEMPLE_COLUMNS]);
    
    
    /*
    void testRoom();
    void printGrid();
    void generateGrid();
    int generate_room_width();
    int generate_room_height();
    char grid[GRID_HEIGHT][GRID_WIDTH];
    */
    
private:
    int mGoblinSmellDistance;
    int mLevel;
    Player* mPlayer;
    std::vector<Monster*> mMonsters;
    std::vector<GameObject*> mItems;
    
    std::vector<std::vector<int>> mRooms;
    
    int mDescend_r;
    int mDescend_c;
    char temple[TEMPLE_ROWS][TEMPLE_COLUMNS];
    
    //for determining min/max of room size
};

//*IMPORTANT* PROBABLY PUT THIS IN GAME.CPP UNDER GENERATELEVEL FUNCTION


//PSEUDOCODE
//fill in entire grid
//choose 5 locations to put holes (end goal: keep putting holes until path to finish. Maybe random amount of locations)
//for each location:
//          fill in each whitespace of certain width

/*
 code for making room
 for making rooms
 for (int i = 0; i < 5; i++)
 {
    int height = randInt(4, 9) //arb
    int width = randInt(4, 9)
    int x = randInt()
 }
 
 
 make a room, randomly generate hallway in any of the four directions, and then make another room based on where you placed hallway. can have two halsl that make a right angle
 
 */

/*
 start_point = (random)
 for (int i = 0; i < 5; i++)
 {
    choose a place for a room
    choose a size of a room
    choose a direction and length for a hall (can have two halls that connect to same room)
    try to make a room at the end of the hall, if you can't try a new hall
    start_point = end_point;
 
 }
 */

//edit and put this in .h file





//fill in the entire grid


#endif /* Temple_h */
