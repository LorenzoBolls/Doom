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
    
    void placeScrollsRandomly();

    
    GameObject* findItems(int r, int c);

    void removeItems(int r, int c);

    bool isScroll(GameObject* item) const;
    
    void playerAttack();
    
    //DEBUGGING
    Monster* getMonster(int index) const;

    string getAllMonstersAttackStatus();
    void resetMonstersAttackingStatus();
    bool isAnyMonsterAttacking() const;

    bool generateRooms(int numRoom);
    void generateHallways();
    
    bool isAreaClear(int startRow, int startCol, int height, int width);
    void placeRoom(int startRow, int startCol, int height, int width);
    
    void generateUnblockedPosition(int& r, int& c);
    
    void addItem(GameObject* item);
    void clearPosition(int r, int c);
    void removeMonster(Monster* monster);
    
    bool isNextToPlayer(Monster* monster);
    
    void createTemple();
    
    void copyTemple(const char temple[TEMPLE_ROWS][TEMPLE_COLUMNS], char destination[TEMPLE_ROWS][TEMPLE_COLUMNS]);
    
    int pathExistsToPlayer(int grid[TEMPLE_ROWS][TEMPLE_COLUMNS], int goblinRow, int goblinCol, int playerRow, int playerCol, int steps);

    
    void placeGoldenIdol();
    
    int getVisitedGrid();
    
    
    Scroll* generateRandomScroll();
    char determineBestDirectionForGoblin(int goblinRow, int goblinCol, int playerRow, int playerCol, int steps);


    
    
private:
    int mGoblinSmellDistance;
    int mLevel;
    Player* mPlayer;
    std::vector<Monster*> mMonsters;
    std::vector<GameObject*> mItems;
    
    int mDescend_r;
    int mDescend_c;
    
    int mRoomNum;
    int roomRowCoord[TEMPLE_ROWS];
    int roomColCoord[TEMPLE_COLUMNS];
    char temple[TEMPLE_ROWS][TEMPLE_COLUMNS];
    
    int visitedGrid[TEMPLE_ROWS][TEMPLE_COLUMNS];

    

};




#endif /* Temple_h */
