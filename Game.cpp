#include "Game.h"
#include "Temple.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Constructor to initialize the game with a goblin smell distance
Game::Game(int goblinSmellDistance)
{
    mGoblinSmellDistance = goblinSmellDistance;
    mGameTemple = new Temple(goblinSmellDistance, 0);
}

// Destructor to clean up the dynamically allocated Temple object
Game::~Game()
{
    delete mGameTemple;
}

// Main game loop to handle player input and game actions
void Game::play()
{
    char ch = ' ';
    mGameTemple->display();
    Player* gamePlayer;
    
    // Inventory trackers
    int state = 0;
    bool inv = false;
    bool updatedStatus = false;
    bool pickedUpStatus = false;
    bool hasGoldenIdol = false;
    bool isAlive = true;
    string statusOutput = "";
    string pickUp = "";
    string scrollOutput = "";
    
    while ((ch) != 'q' && !hasGoldenIdol && isAlive) {
        ch = getCharacter();
        gamePlayer = mGameTemple->getPlayer();
        
        // Handle weapon equipping from inventory
        if (state == 1) {
            int index = gamePlayer->convertCharToInt(ch);
            if (index < gamePlayer->getInventory().size()) {
                updatedStatus = true;
                GameObject* invItem = gamePlayer->getInventory().at(index);
                if (!mGameTemple->isScroll(invItem)) {
                    Weapon* w = dynamic_cast<Weapon*>(invItem);
                    gamePlayer->equipWeapon(w);
                    statusOutput = "\nYou are wielding " + invItem->getName();
                } else {
                    statusOutput = "\nYou can't wield " + invItem->getName();
                }
                state = 0;
                inv = false;
            }
        }

        // Handle scroll reading from inventory
        if (state == 2) {
            int index = gamePlayer->convertCharToInt(ch);
            if (index < gamePlayer->getInventory().size()) {
                updatedStatus = true;
                GameObject* invItem = gamePlayer->getInventory().at(index);
                if (mGameTemple->isScroll(invItem)) {
                    Scroll* s = dynamic_cast<Scroll*>(invItem);
                    gamePlayer->readScroll(s);
                    scrollOutput = gamePlayer->getScrollStatus();
                    gamePlayer->removeFromInventory(s);
                    statusOutput = "\nYou read the " + invItem->getName();
                } else {
                    statusOutput = "\nYou can't read a " + invItem->getName();
                }
                state = 0;
                inv = false;
            }
        }

        // Cheat code to set player stats
        if (ch == 'c') {
            gamePlayer->setHitPoints(50);
            gamePlayer->setMaxHitPoints(50);
            gamePlayer->setStrength(9);
        }

        // Handle inventory view toggle
        if (ch == 'i' || ch == 'w' || ch == 'r') {
            if (!inv) { // Inventory is not opened
                gamePlayer->viewInventory();
                inv = true;
            } else { // Inventory is open
                inv = false;
            }
        } else {
            inv = false;
        }

        // Set state for weapon equipping or scroll reading
        if (ch == 'w') {
            state = 1;
        } else if (ch == 'r') {
            state = 2;
        }

        // Handle item pickup
        if (ch == 'g' && !mGameTemple->isDescendPosition(gamePlayer->getRow(), gamePlayer->getCol()) && mGameTemple->isGameObjectAt(gamePlayer->getRow(), gamePlayer->getCol())) {
            GameObject *toAdd = mGameTemple->findItems(gamePlayer->getRow(), gamePlayer->getCol());
            if (toAdd != nullptr && gamePlayer->getInventorySize() < 25) { // Player is on top of something
                mGameTemple->removeItems(gamePlayer->getRow(), gamePlayer->getCol());
                gamePlayer->addToInventory(toAdd);
                pickUp = "\nYou pick up " + toAdd->getName();
                pickedUpStatus = true;
            } else {
                pickUp = "Your knapsack is full; you can't pick that up.";
                pickedUpStatus = true;
            }
            if (toAdd != nullptr && toAdd->getName() == "the golden idol") {
                mGameTemple->removeItems(gamePlayer->getRow(), gamePlayer->getCol());
                gamePlayer->addToInventory(toAdd);
                pickUp = "\nYou pick up " + toAdd->getName();
                pickedUpStatus = true;
                hasGoldenIdol = true;
            }
        }

        // Handle level descent
        if (ch == '>' && mGameTemple->isDescendPosition(gamePlayer->getRow(), gamePlayer->getCol())) {
            descend();
        }

        // Handle player movement and interactions
        gamePlayer->move(ch);
        mGameTemple->playerAttack();
        mGameTemple->moveMonsters(gamePlayer);
        gamePlayer->regainHitPoint(gamePlayer->getMaxHitPoints());

        // Display game status and messages
        if (!inv) { // Inventory is not being shown
            mGameTemple->display();
            if (gamePlayer->isAsleep()) {
                cout << endl;
            }
            if (gamePlayer->isAttacking()) {
                cout << endl << gamePlayer->getName() << gamePlayer->getAttackStatus();
                gamePlayer->setAttacking(false);
            }
            if (mGameTemple->isAnyMonsterAttacking()) {
                cout << mGameTemple->getAllMonstersAttackStatus();
                mGameTemple->resetMonstersAttackingStatus();
            }
            if (pickedUpStatus) {
                cout << pickUp;
                pickedUpStatus = false;
            }
            if (hasGoldenIdol) {
                cout << "\nCongratulations, you won!" << endl;
            }
            if (updatedStatus) {
                cout << statusOutput;
                cout << scrollOutput;
                scrollOutput = "";
                updatedStatus = false;
            }
        }

        // Handle player death and end game
        if (gamePlayer->getHitPoints() <= 0) {
            isAlive = false;
        }
    }
}

// Handle level descent and player state transfer
void Game::descend()
{
    Player* oldPlayer = mGameTemple->getPlayer();
    int playerHitPoints = oldPlayer->getHitPoints();
    int playerMaxHitPoints = oldPlayer->getMaxHitPoints();
    int playerArmor = oldPlayer->getArmor();
    int playerStrength = oldPlayer->getStrength();
    int playerDexterity = oldPlayer->getDexterity();
    Weapon* playerWeapon = oldPlayer->getWeapon();
    vector<GameObject*> oldInventory = oldPlayer->getInventory();
    
    int nextLvl = mGameTemple->getLevel() + 1;
    delete mGameTemple;
    mGameTemple = new Temple(mGoblinSmellDistance, nextLvl);
    
    mGameTemple->placePlayerRandomly();
    
    Player* newPlayer = mGameTemple->getPlayer();
    newPlayer->setTemple(mGameTemple);
    newPlayer->setHitPoints(playerHitPoints);
    newPlayer->setMaxHitPoints(playerMaxHitPoints);
    newPlayer->setArmor(playerArmor);
    newPlayer->setStrength(playerStrength);
    newPlayer->setDexterity(playerDexterity);
    newPlayer->equipWeapon(playerWeapon);
    newPlayer->setInventory(oldInventory);
    
    if (mGameTemple->getLevel() < 4) {
        mGameTemple->placeDescendPoint();
    } else if (mGameTemple->getLevel() == 4) {
        mGameTemple->placeGoldenIdol();
    }
}
