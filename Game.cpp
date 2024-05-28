// Game.cpp

#include "Game.h"
#include "Temple.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
{
    
    mGoblinSmellDistance = goblinSmellDistance;
    
    //change (goblinSmellDistance, #) back to 0
    mGameTemple = new Temple(goblinSmellDistance, 0);
    mGameTemple->placePlayerRandomly();
    mGameTemple->placeDescendPoint();
    
    //CHECK THIS AGAIN MIGHT HAVE TO DELETE
//    mNumMonsters = randInt(2, 5 * (mGameTemple->getLevel() + 1) + 1);
//    for (int i = 0; i < mNumMonsters; i++)
//    {
        mGameTemple->generateMonsters();
//    }
    
    mGameTemple->placeMultipleGameObjects();
//    mGameTemple->placeMultipleWeapons();
//    mGameTemple->placeMultipleScrolls();
    

}

Game::~Game()
{
    delete mGameTemple;
}

void Game::play()
{
    char ch = ' ';
    mGameTemple->display();
    Player* gamePlayer;
    
    //inventory trackers
    int state = 0;
    bool inv = false;
    bool updatedStatus = false;
    bool pickedUpStatus = false;
    string statusOutput = "";
    string pickUp = "";
    string scrollOutput = "";
    
    while ((ch) != 'q') {
        ch = getCharacter();
        gamePlayer = mGameTemple->getPlayer();
        
        if (state == 1) {
            int index = gamePlayer->convertCharToInt(ch);
            if (index < gamePlayer->getInventory().size())
            {
                updatedStatus = true;
                GameObject* invItem = gamePlayer->getInventory().at(index);
                if (!mGameTemple->isScroll(invItem))
                {
                    Weapon* w = dynamic_cast<Weapon*>(invItem);
                    gamePlayer->equipWeapon(w);
                    statusOutput = "\nYou are wielding " + invItem->getName();
                }
                else
                {
                    statusOutput = "\nYou can't wield " + invItem->getName();
                }
                state = 0;
                inv = false;
            }
        }
        if (state == 2) {
            int index = gamePlayer->convertCharToInt(ch);
            if (index < gamePlayer->getInventory().size())
            {
                updatedStatus = true;
                GameObject* invItem = gamePlayer->getInventory().at(index);
                if (mGameTemple->isScroll(invItem))
                {
                    Scroll* s = dynamic_cast<Scroll*>(invItem);
                    
                    gamePlayer->readScroll(s);
                    scrollOutput = gamePlayer->getScrollStatus();
                    gamePlayer->removeFromInventory(s);
                    
                    statusOutput = "\nYou read the " + invItem->getName();
                }
                else
                {
                    statusOutput = "\nYou can't read a " + invItem->getName();
                }
                state = 0;
                inv = false;
            }
        }
        
        if(ch == 'c')
        {
            gamePlayer->setHitPoints(50);
            gamePlayer->setMaxHitPoints(50);
            gamePlayer->setStrength(9);
        }
        
        if (ch == 'i' || ch == 'w' || ch == 'r')
        {
            if (inv == false) //inventory is not opened
            {
                gamePlayer->viewInventory();
                inv = true;
            }
            else if (inv == true) //inventory is open
            {
                inv = false;
            }

        }
        else
        {
            inv = false;
        }
        
        if (ch == 'w')
        {
            state = 1;
        }
        else if (ch == 'r')
        {
            state = 2;
        }
        
        //makes sure only adds gameobjects to inventory
        if (ch == 'g' && !mGameTemple->isDescendPosition(gamePlayer->getRow(), gamePlayer->getCol()) && mGameTemple->isGameObjectAt(gamePlayer->getRow(), gamePlayer->getCol()))
        {
            
            GameObject *toAdd = mGameTemple->findItems(gamePlayer->getRow(), gamePlayer->getCol());
            
            
            if (toAdd != nullptr) //you are on top of something
            {
                mGameTemple->removeItems(gamePlayer->getRow(), gamePlayer->getCol());
                gamePlayer->addToInventory(toAdd);
                pickUp = "\nYou pick up " + toAdd->getName();
                pickedUpStatus = true;
            }
        
            
        }
        
        if (ch == '>' && mGameTemple->isDescendPosition(gamePlayer->getRow(), gamePlayer->getCol()))
        {
            descend();
        }

        gamePlayer->regainHitPoint(gamePlayer->getMaxHitPoints());
        gamePlayer->move(ch);
        mGameTemple->moveMonsters(gamePlayer);
        mGameTemple->playerAttack();
        
        
        //DEBUGGING COUT
        //cout << "mosnter health: " << mGameTemple->getMonster(0)->getHitPoints();
        
        
        if (inv != true) //inventory is not being showed
        {
            mGameTemple->display();
            cout << endl;
            //couting states underneath display
            if (gamePlayer->isAttacking())
            {
                cout << endl << gamePlayer->getName() << gamePlayer->getAttackStatus();
                gamePlayer->setAttacking(false);
            }
            if (mGameTemple->isAnyMonsterAttacking())
            {
                cout << mGameTemple->getAllMonstersAttackStatus();
                mGameTemple->resetMonstersAttackingStatus();
            }
            if (pickedUpStatus)
            {
                cout << pickUp;
                pickedUpStatus = false;
            }
            if (updatedStatus)
            {
                cout << statusOutput;
                cout << scrollOutput;
                scrollOutput = "";
                updatedStatus = false;
            }
        }
        
    }
 
}

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
    
    mGameTemple->placeDescendPoint();
    
    
//    mNumMonsters = randInt(2, 5 * (mGameTemple->getLevel() + 1) + 1);
//    for (int i = 0; i < mNumMonsters; i++)
//    {
        mGameTemple->generateMonsters();
//    }
    
    mGameTemple->placeMultipleGameObjects();
    
//    mGameTemple->placeMultipleWeapons();
//    mGameTemple->placeMultipleScrolls();
}




// You will presumably add to this project other .h/.cpp files for the various
// classes of objects you need to play the game:  player, monsters, weapons, 
// etc.  You might have a separate .h/.cpp pair for each class (e.g., Player.h,
// Bogeyman.h, etc.), or you might put the class declarations for all actors in
// Actor.h, all game objects in GameObject.h, etc.
