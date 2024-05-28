//
//  Temple.cpp
//  Project 3
//
//  Created by Lorenzo Bolls on 5/11/24.
//

#include "Temple.h"
#include "utilities.h"
#include <iostream>

using namespace std;

Temple::Temple(int goblinSmellDistance, int level)
    : mGoblinSmellDistance(goblinSmellDistance), mLevel(level), mPlayer(nullptr)
{
//    generateRooms();
//     Initialize the grid
    for(int r = 0; r < TEMPLE_ROWS; r++)
        for(int c = 0; c < TEMPLE_COLUMNS; c++){
            //draw wall
            if(r <= 0 || c <= 0 || r >= 17 || c >= 69)
                temple[r][c] = '#';
            else
                temple[r][c] = ' ';
        }
}

Temple::~Temple() {
     delete mPlayer;
    
    //might have to alter this logic
    for (Monster* monster : mMonsters)
    {
        delete monster;
    }
    
    for (GameObject* item : mItems)
    {
        delete item;
    }
}

Player* Temple::getPlayer() const
{
    return mPlayer;
}

int Temple::getLevel() const
{
    return mLevel;
}

void Temple::setLevel(int level)
{
    mLevel = level;
}

bool Temple::blockedPosition(int r, int c)
{
    //monsters block position, weapons don't
    //MAKE SURE THAT YOU CAN STILL ATTACK. MIGHT HAVE TO REMOVE BC IT WON'T MOVE TOWARDS DIRECTION
    //Currently there is a problem with calling on temple when you make a new one
    if (temple[r][c] == '#'|| temple[r][c] == 'G' || temple[r][c] == 'B' || temple[r][c] == 'S' || temple[r][c] == 'D')
    {
        return true;
    }
    else
    {
        return false;
    }

    //add all the monsters and weapons to this function
}

void Temple::generateUnblockedPosition(int& r, int& c)
{
    do {
        r = randInt(1, TEMPLE_ROWS - 1);
        c = randInt(1, TEMPLE_COLUMNS - 1);
    } while(blockedPosition(r, c));
}

bool Temple::isDescendPosition(int r, int c)
{
    return r == mDescend_r && c == mDescend_c;
}

void Temple::placePlayerRandomly() {
    int r, c;
    generateUnblockedPosition(r, c);

     if (mPlayer != nullptr) //player already exists
     {
          //clear the old position
          temple[mPlayer->getRow()][mPlayer->getCol()] = ' ';
         
          //update the player's position
          mPlayer->setPosition(r, c);
      } 
     else
     {
          mPlayer = new Player(this, r, c);
      }
     
    
//    mPlayer = new Player(this, r, c);
//    temple[r][c] = '@';
}

void Temple::placeDescendPoint()
{
    int r, c;
    generateUnblockedPosition(r, c);
    mDescend_r = r;
    mDescend_c = c;
}


bool Temple::isGameObjectAt(int r, int c)
{
    for (GameObject* item : mItems)
        if (r == item->getObjectRow() && c == item->getObjectCol())
        {
            return true;
        }
    return false;
}

void Temple::placeMultipleGameObjects()
{
    int mW = randInt(0,3);
    int mS = 0;
    
    mS = randInt(2-mW, 3-mW);
    if (mW == 3)
    {
        mS = 0;
    }
    for(int i = 0; i < mW; i++)
    {
        placeWeaponsRandomly();
    }
    for(int i = 0; i < mS; i++)
    {
        placeScrollsRandomly();
    }
}

//void Temple::placeMultipleScrolls()
//{
//    int n = randInt(1, 2);
//    for (int i = 0; i < n; i++)
//    {
//        placeScrollsRandomly();
//    }
//}

void Temple::placeScrollsRandomly()
{
    int scrollType = randInt(1, 5);
    
    int r = randInt(1, TEMPLE_ROWS - 1);
    int c = randInt(1, TEMPLE_COLUMNS - 1);
    
    while (blockedPosition(r, c) || isGameObjectAt(r, c) || isDescendPosition(r, c))
    {
        r = randInt(1, TEMPLE_ROWS - 1);
        c = randInt(1, TEMPLE_COLUMNS - 1);
    }
    
    Scroll* s = nullptr;
    switch (scrollType)
    {
        case 1:
        {
            s = new ScrollOfTeleportation;
            break;
        }
        case 2:
        {
            s = new ScrollOfImproveArmor;
            break;
        }
        case 3:
        {
            s = new ScrollOfEnhanceHealth;
            break;
        }
        case 4:
        {
            s = new ScrollOfRaiseStrength;
            break;
        }
        case 5:
        {
            s = new ScrollOfEnhanceDexterity;
            break;
        }
    }
    s->setPosition(c, r);
    mItems.push_back(s);
}

bool Temple::isScroll(GameObject* item) const
{
    return (item->getName().substr(0, 6) == "scroll");
}

//void Temple::placeMultipleWeapons()
//{
//    int n = randInt(1, 2);
//    for (int i = 0; i < n; i++)
//    {
//        placeWeaponsRandomly();
//    }
//}

void Temple::placeWeaponsRandomly()
{
    int weaponType = randInt(1,5);
    
    int r = randInt(1, TEMPLE_ROWS - 1);
    int c = randInt(1, TEMPLE_COLUMNS - 1);
    
    while (blockedPosition(r, c) || isGameObjectAt(r, c) || isDescendPosition(r, c))
    {
        r = randInt(1, TEMPLE_ROWS - 1);
        c = randInt(1, TEMPLE_COLUMNS - 1);
    }
    
    Weapon* w = nullptr;
    switch (weaponType)
    {
        case 1:
        {
            w = new ShortSword();
            break;
        }
        case 2:
        {
            w = new LongSword();
            break;
        }
        case 3:
        {
            w = new Mace();
            break;
        }
        case 4:
        {
            w = new MagicAxe();
            break;
        }
        case 5:
        {
            w = new MagicFangsOfSleep();
            break;
        }
    }
    
    w->setPosition(c, r);
    mItems.push_back(w);
    
}

void Temple::generateMonsters()
{
    int numMonsters = randInt(2, 5 * (mLevel + 1) + 1);
    for (int i = 0; i < numMonsters; ++i)
    {
        int r, c;
        generateUnblockedPosition(r, c);
        
        int monsterType = randInt(0, 2); // Example: 0 for Bogeymen, 1 for Snakewomen, 2 for Goblins
        
        switch (monsterType) {
            case 0:
                mMonsters.push_back(new Snakewomen(this, r, c));
            
                break;
            case 1:
                mMonsters.push_back(new Goblins(this, r, c));
                break;
            case 2:
                if (mLevel >= 2)
                {
                    mMonsters.push_back(new Bogeymen(this, r, c));
                }
                break;
                // Add other monster types here
            default:
                break;
        }
    }
}

void Temple::display() const {

    // Draw the grid
    clearScreen();
    for (int r = 0; r < TEMPLE_ROWS; r++) {
        for (int c = 0; c < TEMPLE_COLUMNS; c++) {
            bool printed = false;
            if (r == mPlayer->getRow() && c == mPlayer->getCol()) {
                cout << "@";
                printed = true; //now you only display the @ sign when on top of other objects
                continue;
            }
            for (Monster* m : mMonsters) {
                if (r == m->getRow() && c == m->getCol()) {
                    cout << m->getName()[0];
                    printed = true;
                }
            }
            if (printed == false && r == mDescend_r && c == mDescend_c) {
                cout << ">";
                printed = true;
                continue;
            }
            for (GameObject* item : mItems)
            {
                if (printed == false && c == item->getObjectCol() && r == item->getObjectRow() && !isScroll(item))
                {
                    
                    cout << ")";
                    printed = true;
                    continue;
                }
                else if (printed == false && c == item->getObjectCol() && r == item->getObjectRow()  && isScroll(item))
                {
                    cout << "?";
                    printed = true;
                    continue;
                }
                //else statement for & and exiting game
            }
            
                // loop through weapons & scroll objects
                // if position is a weapon location, print )
                // if pos is a scroll location, print ?
                // otherwise print the map .
            if (printed == false)
            {
                cout << temple[r][c];
            }
        }
        cout << endl;
    }
    cout << "Level: " << mLevel << ", Hit points: " << getPlayer()->getHitPoints() << ", Armor: " << getPlayer()->getArmor() << ", Strength: " << getPlayer()->getStrength() << ", Dexterity: " << getPlayer()->getDexterity();
}


//FOR DEBUGGING
Monster* Temple::getMonster(int index) const 
{
    if (index >= 0 && index < mMonsters.size()) 
    {
        return mMonsters[index];
    }
    return nullptr; // Return nullptr if the index is out of bounds
}

string Temple::getAllMonstersAttackStatus()
{
    string allStatus;
    for (Monster* monster : mMonsters)
    {
        if (monster->getAttackStatus() != "" && isNextToPlayer(monster))
        {
            allStatus += "The " + monster->getName() + monster->getAttackStatus();
        }
    }
    return allStatus;
}

bool Temple::isNextToPlayer(Monster* monster)
{
    int playerRow = mPlayer->getRow();
    int playerCol = mPlayer->getCol();

    //if adjacent to player, attack
    if (abs(playerRow - monster->getRow()) + abs(playerCol - monster->getCol()) == 1)
    {
        //FOR DEBUGGING
        //cout << endl << "bogeymen calculating whether to attack: " << endl;
        return true;
    }
    return false;
}

bool Temple::isAnyMonsterAttacking() const
{
    for (const Monster* monster : mMonsters)
    {
        if (monster->isAttacking())
        {
            return true;
        }
    }
    return false;
}

void Temple::resetMonstersAttackingStatus()
{
    for (Monster* monster : mMonsters)
    {
        monster->setAttacking(false);
    }
}

bool Temple::determineNewPosition(int& r, int& c, char dir, char symbol)
{
    switch (dir) {
        case 'k':
            if(!blockedPosition(r-1, c))
            {
                temple[r][c] = ' ';
                r--;
                temple[r][c] = symbol;
                return true;
            }
            break;
        case 'j':
            if(!blockedPosition(r+1, c))
            {
                temple[r][c] = ' ';
                r++;
                temple[r][c] = symbol;
                return true;
            }
            break;
        case 'h':
            if(!blockedPosition(r, c-1))
            {
                temple[r][c] = ' ';
                c--;
                temple[r][c] = symbol;
                return true;
            }
            break;
        case 'l':
            if(!blockedPosition(r, c+1))
            {
                temple[r][c] = ' ';
                c++;
                temple[r][c] = symbol;
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

void Temple::moveMonsters(Player* player)
{
    for (Monster* monster : mMonsters)
    {
        
        //write if statements later
        monster->takeTurn(player);
        if(monster->getName() == "Goblin")
        {
            Goblins* goblin = dynamic_cast<Goblins*>(monster);
            char goblinTemple[TEMPLE_ROWS][TEMPLE_COLUMNS];
            copyTemple(this->temple, goblinTemple);
            
            cout << goblin->pathExistsToPlayer(goblinTemple, goblin->getRow(), goblin->getCol(), getPlayer()->getRow(), getPlayer()->getCol(), 15);
        }
    }
}

void Temple::copyTemple(const char temple[TEMPLE_ROWS][TEMPLE_COLUMNS], char destination[TEMPLE_ROWS][TEMPLE_COLUMNS])
{
    for (int i = 0; i < TEMPLE_ROWS; i++)
    {
        for (int j = 0; j < TEMPLE_COLUMNS; j++)
        {
            destination[i][j] = temple[i][j];
        }
    }
}

void Temple::playerAttack()
{
    //makes sure player holds a weapon before
    if (!mPlayer->getWeapon())
        return;
    
    int targetRow = mPlayer->getRow();
    int targetCol = mPlayer->getCol();


    
    switch (mPlayer->getLastDirection()) {
        case ARROW_LEFT:
            targetCol--;
            break;
        case ARROW_RIGHT:
            targetCol++;
            break;
        case ARROW_UP:
            targetRow--;
            break;
        case ARROW_DOWN:
            targetRow++;
    }
    
    for (Monster* m : mMonsters)
    {
        if (m->getRow() == targetRow && m->getCol() == targetCol) {
            //DEBUGGING COUT
            //cout << endl << "Player calculating whether to attack:" << endl;
            mPlayer->attack(m);
            return;
        }
    }
}



GameObject* Temple::findItems(int r, int c) {
    for (auto i : mItems) {
        if (i->getObjectCol() == c && i->getObjectRow() == r) return i;
    }
    return nullptr;
}

void Temple::removeItems(int r, int c) {
    for (vector<GameObject*>::iterator i = mItems.begin(); i != mItems.end(); i++) {
        if ((*i)->getObjectCol() == c && (*i)->getObjectRow() == r) {
            mItems.erase(i);
            return;
        }
    }
}

void Temple::addItem(GameObject* item)
{
    mItems.push_back(item);
}

void Temple::clearPosition(int r, int c)
{
    temple[r][c] = ' ';
}

void Temple::removeMonster(Monster* monster)
{
    auto it = std::find(mMonsters.begin(), mMonsters.end(), monster);
    if (it != mMonsters.end()) {
        mMonsters.erase(it);
    }
    delete monster;
}


void Temple::generateRooms()
{
    //fill the entire grid with walls
    for (int r = 0; r < TEMPLE_ROWS; ++r)
    {
        for (int c = 0; c < TEMPLE_COLUMNS; ++c)
        {
            temple[r][c] = '#';
        }
    }

    //etermine the number of rooms
    int numRooms = randInt(3, 7);

    for (int i = 0; i < numRooms; ++i)
    {
        int roomWidth = randInt(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH);
        int roomHeight = randInt(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT);
        int startRow, startCol;
        int roomAttemptCounter = 0;

        cout << "room width: " << roomWidth << "room height: " << roomHeight;
        // Ensure the room doesn't overlap with existing rooms
        do
        {
            
            startRow = randInt(0, TEMPLE_ROWS - roomHeight - 1);
            startCol = randInt(0, TEMPLE_COLUMNS - roomWidth - 1);
            roomAttemptCounter++;
            

            cout << "start row: " << startRow << endl;
            cout << "start col: " << startCol << endl;
            
        } while (!isAreaClear(startRow, startCol, roomHeight, roomWidth) && roomAttemptCounter != 10);
        
        if (roomAttemptCounter == 10)
        {
            cout << "breaked out\n";
            break;
        }
        
        int endRow = startRow + roomHeight;
        int endCol = startCol + roomWidth;
        
        vector<int> coords = {startRow, startCol, endRow, endCol};
        mRooms.push_back(coords);

        // Place the room in the grid
        placeRoom(startRow, startCol, roomHeight, roomWidth);
        cout << numRooms << endl;
        cout << "ROOMS PRINTED SUCCESSFULLY" << endl;
    }
}

bool Temple::isAreaClear(int startRow, int startCol, int height, int width)
{
    //makes sure a new room isn't placed within the coordinates of another room
    for (int r = startRow; r <= startRow + height; ++r)
    {
        for (int c = startCol; c <= startCol + width; ++c)
        {
            for(vector<int> coords : mRooms)
            {
                if (r > coords[0] && r < coords[2] && c > coords[1] && c < coords[3])
                {
                    cout << "returning false: ";
                    cout << "r: " << r << " c: " << c << " coords 0: " << coords[0] << " " << coords[1] << "coords 2: " << coords[2] << " " << coords[3] << endl;
                    return false;
                }
            }
        }
    }
    cout << "returning true: ";
    cout << "startRow: " << startRow << "startCol: " << startCol << " end row: " << startRow+height << " end col: " << startCol+width << endl;
    return true;
}

void Temple::placeRoom(int startRow, int startCol, int height, int width)
{
    for (int r = startRow + 1; r < startRow + height - 1; ++r)
    {
        for (int c = startCol + 1; c < startCol + width -1; ++c)
        {
            temple[r][c] = ' ';
        }
    }
}
//




/*
bool Temple::isPlayerAt(int r, int c) const {
    return mPlayer != nullptr && mPlayer->getRow() == r && mPlayer->getCol() == c;
}


void Temple::printGrid()
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for(int x = 0; x < GRID_WIDTH; x++)
        {
            cout << grid[y][x];
        }
        cout << endl;
    }
}

void Temple::testRoom()
{
    const int totalColumns = 70;
    const int rows = 18;

    // Print the top wall
    for (int i = 0; i < totalColumns; ++i) {
        cout << '#';
    }
    cout << endl;

    // Print the grid with side walls
    for (int i = 0; i < rows; ++i) {
        cout << '#';  // Left wall
        for (int j = 0; j < totalColumns - 2; ++j) {  // Inner spaces
            cout << ' ';
        }
        cout << '#';  // Right wall
        cout << endl;
    }

    // Print the bottom wall
    for (int i = 0; i < totalColumns; ++i) {
        cout << '#';
    }
    cout << endl;
}
*/

//*IMPORTANT* PROBABLY PUT THIS IN GAME.CPP UNDER GENERATELEVEL FUNCTION


//PSEUDOCODE
//fill in entire grid
//choose 5 locations to put holes (end goal: keep putting holes until path to finish. Maybe random amount of locations)
//for each location:
//          fill in each whitespace of certain width

///*
// code for making room
// for making rooms
// for (int i = 0; i < 5; i++)
// {
//    int height = randInt(4, 9) //arb
//    int width = randInt(4, 9)
//    int x = randInt()
// }
//
//
// make a room, randomly generate hallway in any of the four directions, and then make another room based on where you placed hallway. can have two halsl that make a right angle
//
// */
//
///*
// start_point = (random)
// for (int i = 0; i < 5; i++)
// {
//    choose a place for a room
//    choose a size of a room
//    choose a direction and length for a hall (can have two halls that connect to same room)
//    try to make a room at the end of the hall, if you can't try a new hall
//    start_point = end_point;
//
// }
// */
//
////edit and put this in .h file
//
//
//
//const int MIN_ROOM_WIDTH = 3;
//const int MIN_ROOM_HEIGHT = 3;
//const int MAX_ROOM_WIDTH = 9;
//const int MAX_ROOM_HEIGHT = 9;
//
//const int MIN_HALL_LENGTH = 1;
//const int MAX_HALL_LENGTH = 10;
//
//const int GRID_WIDTH = 70;
//const int GRID_HEIGHT = 18;
//
////fill in the entire grid
//
//char grid[GRID_HEIGHT][GRID_WIDTH];
//
//for(int i = 0; i < GRID_HEIGHT; i++)
//{
//    for (int j = 0; j < GRID_WIDTH; j++)
//    {
//        grid[i][j] = '#';
//    }
//}
//
//int start_x = randInt(1, (GRID_WIDTH - 1) - MAX_ROOM_WIDTH - 1);
//int start_y = randInt(1, (GRID_HEIGHT - 1) - MAX_ROOM_HEIGHT - 1);

/*
********** IMPORTANT CODE TO GENERATE GRID
void Temple::generateGrid()
{
    for(int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            grid[i][j] = '#';
        }
    }

    //choosing a randomized starting position for the first room
    int room_X = randInt(1, (GRID_WIDTH - 1) - MAX_ROOM_WIDTH - 1);
    int room_Y = randInt(1, (GRID_HEIGHT - 1) - MAX_ROOM_HEIGHT - 1);
    int roomHeight = randInt(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT);
    int roomWidth = randInt(MIN_ROOM_WIDTH, MAX_ROOM_HEIGHT);
    
    //creates the room
    for(int y = room_Y; y < room_Y + roomHeight; y++)
    {
        for(int x = room_X; x < room_X + roomWidth; x++)
        {
            grid[y][x] = ' ';
        }
    }
    
    //creating hall
    int direction = randInt(1, 4);
    int hall_x;
    int end_hall_x;
    
    int hall_y;
    int end_hall_y;
    
    //int hall_length = randInt(MIN_HALL_LENGTH, MAX_HALL_LENGTH);


    while(true)
    {
        
        switch (direction){
            case 1: // NORTH
                hall_x = randInt(room_X, room_X + roomWidth - 1);
                hall_y = room_Y;
                
                
                end_hall_x = hall_x;
                end_hall_y = hall_y - hall_length;
                
                //check if you can make a valid hallway
                if (end_hall_y >= 1)
                {
                    //
                }
                break;
            case 2: // EAST
                hall_x = room_X + roomWidth - 1;
                hall_y = randInt(room_Y, room_Y + roomHeight - 1);
                end_hall_x = hall_x + hall_length;
                end_hall_y = hall_y;
                break;
            case 3: // SOUTH
                hall_x = randInt(room_X, room_X + roomWidth - 1);
                hall_y = room_Y + roomHeight - 1;
                end_hall_x = hall_x;
                end_hall_y = hall_y + hall_length;
                break;
            case 4: // WEST
                hall_x = room_X;
                hall_y = randInt(room_Y, room_Y + roomHeight - 1);
                end_hall_x = hall_x - hall_length;
                end_hall_y = hall_y;
                break;
        }
    }
    
 
    
        //first choose random direction and then random location to start hallway on that direction
     
}
*/
