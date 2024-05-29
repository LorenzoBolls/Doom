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

// Constructor
Temple::Temple(int goblinSmellDistance, int level)
    : mGoblinSmellDistance(goblinSmellDistance), mLevel(level), mPlayer(nullptr)
{
    // Create walls
    for (int r = 0; r < TEMPLE_ROWS; r++) {
        for (int c = 0; c < TEMPLE_COLUMNS; c++) {
            temple[r][c] = '#';
        }
    }

    // Generate rooms
    mRoomNum = randInt(3, 7);
    for (int i = 0; i < mRoomNum; ) {
        int roomNum = i;
        bool room = generateRooms(roomNum);
        if (!room) {
            continue;
        }
        i++;
    }

    // Generate hallways and place player
    generateHallways();
    placePlayerRandomly();
    
    if (mLevel >= 0 && mLevel < 4) {
        placeDescendPoint();
    }

    // Generate monsters and items
    generateMonsters();
    placeMultipleGameObjects();
}

// Destructor
Temple::~Temple() {
//    delete mPlayer;
//
    for (Monster* monster : mMonsters) {
        delete monster;
    }

    for (GameObject* item : mItems) {
        delete item;
    }
}

// Accessor methods
Player* Temple::getPlayer() const {
    return mPlayer;
}

int Temple::getLevel() const {
    return mLevel;
}

void Temple::setLevel(int level) {
    mLevel = level;
}

// Check if a position is blocked
bool Temple::blockedPosition(int r, int c) {
    if (temple[r][c] == '#') {
        return true;
    }

    for (Monster* monster : mMonsters) {
        if (monster->getRow() == r && monster->getCol() == c) {
            return true;
        }
    }

    return false;
}

// Generate an unblocked position
void Temple::generateUnblockedPosition(int& r, int& c) {
    r = randInt(1, TEMPLE_ROWS - 1);
    c = randInt(1, TEMPLE_COLUMNS - 1);
    
    while (blockedPosition(r, c) || isGameObjectAt(r, c) || isDescendPosition(r, c)) {
        r = randInt(1, TEMPLE_ROWS - 1);
        c = randInt(1, TEMPLE_COLUMNS - 1);
    }
}

// Check if a position is the descend point
bool Temple::isDescendPosition(int r, int c) {
    return r == mDescend_r && c == mDescend_c;
}

// Place the player at a random position
void Temple::placePlayerRandomly() {
    int r, c;
    generateUnblockedPosition(r, c);

    if (mPlayer != nullptr) { // Player already exists
        temple[mPlayer->getRow()][mPlayer->getCol()] = ' '; // Clear old position
        mPlayer->setPosition(r, c); // Update position
    } else {
        mPlayer = new Player(this, r, c); // Create new player
    }
}

// Place the descend point
void Temple::placeDescendPoint() {
    int r, c;
    generateUnblockedPosition(r, c);
    mDescend_r = r;
    mDescend_c = c;
}

// Check if a game object is at a position
bool Temple::isGameObjectAt(int r, int c) {
    for (GameObject* item : mItems) {
        if (r == item->getObjectRow() && c == item->getObjectCol()) {
            return true;
        }
    }
    return false;
}

// Place multiple game objects
void Temple::placeMultipleGameObjects() {
    int mW = randInt(0, 3);
    int mS = randInt(2 - mW, 3 - mW);
    if (mW == 3) {
        mS = 0;
    }
    for (int i = 0; i < mW; i++) {
        placeWeaponsRandomly();
    }
    for (int i = 0; i < mS; i++) {
        placeScrollsRandomly();
    }
}

// Generate a random scroll
Scroll* Temple::generateRandomScroll() {
    Scroll* newScroll = nullptr;
    int scrollType = randInt(1, 5);
    switch (scrollType) {
        case 1:
            newScroll = new ScrollOfTeleportation();
            break;
        case 2:
            newScroll = new ScrollOfImproveArmor();
            break;
        case 3:
            newScroll = new ScrollOfEnhanceHealth();
            break;
        case 4:
            newScroll = new ScrollOfRaiseStrength();
            break;
        case 5:
            newScroll = new ScrollOfEnhanceDexterity();
            break;
    }
    return newScroll;
}

// Place scrolls randomly
void Temple::placeScrollsRandomly() {
    Scroll* s = generateRandomScroll();
    if (s) {
        int r, c;
        generateUnblockedPosition(r, c);
        s->setPosition(c, r);
        mItems.push_back(s);
    }
}

// Check if an item is a scroll
bool Temple::isScroll(GameObject* item) const {
    return (item->getName().substr(0, 6) == "scroll");
}

// Place the golden idol
void Temple::placeGoldenIdol() {
    int r, c;
    generateUnblockedPosition(r, c);
    GoldenIdol* idol = new GoldenIdol();
    idol->setPosition(c, r);
    mItems.push_back(idol);
}

// Place weapons randomly
void Temple::placeWeaponsRandomly() {
    int weaponType = randInt(1, 5);
    int r, c;
    generateUnblockedPosition(r, c);

    Weapon* w = nullptr;
    switch (weaponType) {
        case 1:
            w = new ShortSword();
            break;
        case 2:
            w = new LongSword();
            break;
        case 3:
            w = new Mace();
            break;
        case 4:
            w = new MagicAxe();
            break;
        case 5:
            w = new MagicFangsOfSleep();
            break;
    }

    w->setPosition(c, r);
    mItems.push_back(w);
}

// Generate monsters
void Temple::generateMonsters() {
    int numMonsters = randInt(2, 5 * (mLevel + 1) + 1);

    for (int i = 0; i < numMonsters; ++i) {
        int r, c;
        generateUnblockedPosition(r, c);
        int monsterType = randInt(0, 3);

        switch (monsterType) {
            case 0:
                mMonsters.push_back(new Snakewomen(this, r, c));
                break;
            case 1:
                mMonsters.push_back(new Goblins(this, r, c));
                break;
            case 2:
                if (mLevel >= 2) {
                    mMonsters.push_back(new Bogeymen(this, r, c));
                }
                break;
            case 3:
                if (mLevel >= 3) {
                    mMonsters.push_back(new Dragon(this, r, c));
                }
                break;
            default:
                break;
        }
    }

    // Ensure all monsters are generated
    while (mMonsters.size() < numMonsters) {
        int r, c;
        generateUnblockedPosition(r, c);
        int monsterType = randInt(0, 3);

        switch (monsterType) {
            case 0:
                mMonsters.push_back(new Snakewomen(this, r, c));
                break;
            case 1:
                mMonsters.push_back(new Goblins(this, r, c));
                break;
            case 2:
                if (mLevel >= 2) {
                    mMonsters.push_back(new Bogeymen(this, r, c));
                }
                break;
            case 3:
                if (mLevel >= 3) {
                    mMonsters.push_back(new Dragon(this, r, c));
                }
                break;
            default:
                break;
        }
    }
}

// Display the temple
void Temple::display() const {
    clearScreen();
    for (int r = 0; r < TEMPLE_ROWS; r++) {
        for (int c = 0; c < TEMPLE_COLUMNS; c++) {
            bool printed = false;

            // Check if the player is at this position
            if (r == mPlayer->getRow() && c == mPlayer->getCol()) {
                cout << "@";
                printed = true;
            }

            // Check if a monster is at this position
            if (!printed) {
                for (Monster* m : mMonsters) {
                    if (r == m->getRow() && c == m->getCol()) {
                        cout << m->getName()[0];
                        printed = true;
                        break;
                    }
                }
            }

            // Check if the descending point is at this position
            if (!printed && r == mDescend_r && c == mDescend_c && mLevel < 4) {
                cout << ">";
                printed = true;
            }

            // Check if an item is at this position
            if (!printed) {
                for (GameObject* item : mItems) {
                    if (c == item->getObjectCol() && r == item->getObjectRow()) {
                        if (item->getName() == "the golden idol") {
                            cout << "&";
                        } else if (isScroll(item)) {
                            cout << "?";
                        } else {
                            cout << ")";
                        }
                        printed = true;
                        break;
                    }
                }
            }

            // If no object was printed, print the map tile
            if (!printed) {
                cout << temple[r][c];
            }
        }
        cout << endl;
    }
    cout << "Level: " << mLevel << ", Hit points: " << getPlayer()->getHitPoints() << ", Armor: " << getPlayer()->getArmor() << ", Strength: " << getPlayer()->getStrength() << ", Dexterity: " << getPlayer()->getDexterity() << endl;
}

// For debugging: get a monster by index
Monster* Temple::getMonster(int index) const {
    if (index >= 0 && index < mMonsters.size()) {
        return mMonsters[index];
    }
    return nullptr;
}

// Get all monsters' attack status
string Temple::getAllMonstersAttackStatus() {
    string allStatus;
    for (Monster* monster : mMonsters) {
        if (monster->getAttackStatus() != "" && isNextToPlayer(monster)) {
            allStatus += "The " + monster->getName() + monster->getAttackStatus();
        }
    }
    return allStatus;
}

// Check if a monster is next to the player
bool Temple::isNextToPlayer(Monster* monster) {
    int playerRow = mPlayer->getRow();
    int playerCol = mPlayer->getCol();

    if (abs(playerRow - monster->getRow()) + abs(playerCol - monster->getCol()) == 1) {
        return true;
    }
    return false;
}

// Check if any monster is attacking
bool Temple::isAnyMonsterAttacking() const {
    for (const Monster* monster : mMonsters) {
        if (monster->isAttacking()) {
            return true;
        }
    }
    return false;
}

// Reset monsters' attacking status
void Temple::resetMonstersAttackingStatus() {
    for (Monster* monster : mMonsters) {
        monster->setAttacking(false);
    }
}

// Determine new position based on direction
bool Temple::determineNewPosition(int& r, int& c, char dir, char symbol) {
    switch (dir) {
        case 'k':
            if (!blockedPosition(r - 1, c)) {
                temple[r][c] = ' ';
                r--;
                temple[r][c] = symbol;
                return true;
            }
            break;
        case 'j':
            if (!blockedPosition(r + 1, c)) {
                temple[r][c] = ' ';
                r++;
                temple[r][c] = symbol;
                return true;
            }
            break;
        case 'h':
            if (!blockedPosition(r, c - 1)) {
                temple[r][c] = ' ';
                c--;
                temple[r][c] = symbol;
                return true;
            }
            break;
        case 'l':
            if (!blockedPosition(r, c + 1)) {
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

// Determine best direction for goblin
char Temple::determineBestDirectionForGoblin(int goblinRow, int goblinCol, int playerRow, int playerCol, int steps) {
    // Initialize visited grid
    for (int r = 0; r < TEMPLE_ROWS; ++r) {
        for (int c = 0; c < TEMPLE_COLUMNS; ++c) {
            if (temple[r][c] == ' ') {
                visitedGrid[r][c] = 1;
            }
        }
    }

    int minSteps = 1000;
    char bestDirection = ' ';

    // Check all directions
    int eastSteps = pathExistsToPlayer(visitedGrid, goblinRow, goblinCol, playerRow, playerCol + 1, steps - 1);
    if (eastSteps < minSteps && (steps >= 0 && steps <= mGoblinSmellDistance)) {
        minSteps = eastSteps;
        bestDirection = 'l'; // move right
    }

    int southSteps = pathExistsToPlayer(visitedGrid, goblinRow, goblinCol, playerRow + 1, playerCol, steps - 1);
    if (southSteps < minSteps && (steps >= 0 && steps <= mGoblinSmellDistance)) {
        minSteps = southSteps;
        bestDirection = 'j'; // move down
    }

    int westSteps = pathExistsToPlayer(visitedGrid, goblinRow, goblinCol, playerRow, playerCol - 1, steps - 1);
    if (westSteps < minSteps && (steps >= 0 && steps <= mGoblinSmellDistance)) {
        minSteps = westSteps;
        bestDirection = 'h'; // move left
    }

    int northSteps = pathExistsToPlayer(visitedGrid, goblinRow, goblinCol, playerRow - 1, playerCol, steps - 1);
    if (northSteps < minSteps && (steps >= 0 && steps <= mGoblinSmellDistance)) {
        minSteps = northSteps;
        bestDirection = 'k'; // move up
    }

    return bestDirection;
}

// Move monsters
void Temple::moveMonsters(Player* player) {
    for (Monster* monster : mMonsters) {
        if (monster->getName() == "Dragon") {
            monster->regainHitPoint(monster->getMaxHitPoints());
        }
        if (monster->getName() == "Goblin") {
            // Uncomment this line to enable goblin actions
            // monster->goblinTakeTurn(player);
        } else {
            monster->takeTurn(player);
        }
    }
}

// Player attack
void Temple::playerAttack() {
    if (mPlayer->isAsleep()) {
        return;
    }
    if (!mPlayer->getWeapon()) {
        return;
    }

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

    for (Monster* m : mMonsters) {
        if (m->getRow() == targetRow && m->getCol() == targetCol) {
            mPlayer->attack(m);
            return;
        }
    }
}

// Find items at a position
GameObject* Temple::findItems(int r, int c) {
    for (auto i : mItems) {
        if (i->getObjectCol() == c && i->getObjectRow() == r) return i;
    }
    return nullptr;
}

// Remove items from a position
void Temple::removeItems(int r, int c) {
    for (vector<GameObject*>::iterator i = mItems.begin(); i != mItems.end(); i++) {
        if ((*i)->getObjectCol() == c && (*i)->getObjectRow() == r) {
            mItems.erase(i);
            return;
        }
    }
}

// Add an item to the temple
void Temple::addItem(GameObject* item) {
    mItems.push_back(item);
}

// Clear a position
void Temple::clearPosition(int r, int c) {
    temple[r][c] = ' ';
}

// Remove a monster from the temple
void Temple::removeMonster(Monster* monster) {
    for (auto it = mMonsters.begin(); it != mMonsters.end(); ++it) {
        if (*it == monster) {
            mMonsters.erase(it);
            delete monster;
            return;
        }
    }
}

// Generate rooms
bool Temple::generateRooms(int numRoom) {
    int topRow = randInt(2, 66);
    int topCol = randInt(2, 15);
    int length = randInt(7, 13);
    int height = randInt(3, 11);

    if (topRow + length > 68 || topCol + height > 15) {
        return false;
    }

    for (int r = topRow - 1; r < topRow + length + 1; r++) {
        for (int c = topCol - 1; c < topCol + height + 1; c++) {
            if (temple[c][r] != '#') {
                return false;
            }
        }
    }

    for (int r = topRow; r < topRow + length; r++) {
        for (int c = topCol; c < topCol + height; c++) {
            temple[c][r] = ' ';
        }
    }

    int rowCoord = (topRow + (length / 2));
    int colCoord = (topCol + (height / 2));

    roomRowCoord[numRoom] = rowCoord;
    roomColCoord[numRoom] = colCoord;

    return true;
}

// Generate hallways
void Temple::generateHallways() {
    for (int i = 0; i < mRoomNum - 1; i++) {
        if (roomColCoord[i] > roomColCoord[i + 1]) {
            for (int j = roomColCoord[i]; j != roomColCoord[i + 1]; j--) {
                temple[j][roomRowCoord[i]] = ' ';
            }
        } else {
            for (int j = roomColCoord[i]; j != roomColCoord[i + 1]; j++) {
                temple[j][roomRowCoord[i]] = ' ';
            }
        }

        if (roomRowCoord[i] < roomRowCoord[i + 1]) {
            for (int j = roomRowCoord[i]; j != roomRowCoord[i + 1]; j++) {
                temple[roomColCoord[i + 1]][j] = ' ';
            }
        } else {
            for (int j = roomRowCoord[i]; j != roomRowCoord[i + 1]; j--) {
                temple[roomColCoord[i + 1]][j] = ' ';
            }
        }
    }
}

// Check if a path exists to the player
int Temple::pathExistsToPlayer(int grid[TEMPLE_ROWS][TEMPLE_COLUMNS], int goblinRow, int goblinCol, int playerRow, int playerCol, int steps) {
    if (steps <= 0) {
        return 10001;
    }
    
    if (grid[playerRow][playerCol] == 0 && temple[playerRow][playerCol] != ' ') {
        return 10001;
    }
    if (playerRow == goblinRow && playerCol == goblinCol) {
        return 0;
    }

    grid[playerRow][playerCol] = 0;

    int east = 1 + pathExistsToPlayer(grid, goblinRow, goblinCol + 1, playerRow, playerCol, steps - 1);
    int south = 1 + pathExistsToPlayer(grid, goblinRow + 1, goblinCol, playerRow, playerCol, steps - 1);
    int west = 1 + pathExistsToPlayer(grid, goblinRow, goblinCol - 1, playerRow, playerCol, steps - 1);
    int north = 1 + pathExistsToPlayer(grid, goblinRow - 1, goblinCol, playerRow, playerCol, steps - 1);

    grid[playerRow][playerCol] = 1;

    int minSteps = east;
    if (south < minSteps) {
        minSteps = south;
    }
    if (west < minSteps) {
        minSteps = west;
    }
    if (north < minSteps) {
        minSteps = north;
    }

    return minSteps;
}
