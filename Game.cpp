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
    mGameTemple = new Temple(goblinSmellDistance, 2);
    mGameTemple->placePlayerRandomly();
    mGameTemple->placeDescendPoint();
    
    //CHECK THIS AGAIN MIGHT HAVE TO DELETE
    mGameTemple->generateMonsters();
    mGameTemple->placeMultipleWeapons();
    mGameTemple->placeMultipleScrolls();
    

}

Game::~Game()
{}

void Game::play()
{
    
    char ch = ' ';
    int maxHitPoints = 20;
    mGameTemple->display();
    Player* gamePlayer;
    
    while ((ch) != 'q') {
        ch = getCharacter();
        gamePlayer = mGameTemple->getPlayer();
        
        if(ch == 'c')
        {
            gamePlayer->setInventoryOpen(false);
            maxHitPoints = 50;
            gamePlayer->setHitPoints(50);
            gamePlayer->setStrength(9);
        }
        
        if (ch == 'i')
        {
            gamePlayer->viewInventory();
            
            //toggling, making sure its opposite of what it currently is
            gamePlayer->setInventoryOpen(!gamePlayer->getInventoryOpen());
        }
        
        if (ch == 'w')
        {
            gamePlayer->viewInventory();
            
            gamePlayer->setInventoryOpen(false);
        }
        
        //adding to inventory
        if (ch == 'g' && !mGameTemple->isDescendPosition(gamePlayer->getRow(), gamePlayer->getCol()))
        {
            GameObject *toAdd;
            bool isWeapon = false;
            gamePlayer->setInventoryOpen(false);
            
            toAdd = mGameTemple->findWeapon(gamePlayer->getRow(), gamePlayer->getCol());
            if (toAdd != nullptr) 
            {
                isWeapon = true;
            }
            
            //if GameObject is not a weapon, assumes its a scroll
            if (!isWeapon) {
                toAdd = mGameTemple->findScroll(gamePlayer->getRow(), gamePlayer->getCol());
            }
            
            if (isWeapon) 
            {
                mGameTemple->removeWeapon(gamePlayer->getRow(), gamePlayer->getCol());
            } 
            else
            {
                mGameTemple->removeScroll(gamePlayer->getRow(), gamePlayer->getCol());
            }
            gamePlayer->addToInventory(toAdd);
        
            vector<GameObject*> = gamePlayer->getInventory();
            for (int i = 0; i < gamePlayer->getInventory().size(); i++)
            {
                cout << gamePlayer->getInventory();
                
            }
            
            if (toAdd != nullptr)
            {
                cout << endl << endl << "You pick up " << toAdd->getName();
            }
            
        }
        
        if (ch == '>' && mGameTemple->isDescendPosition(gamePlayer->getRow(), gamePlayer->getCol()))
        {
            //anytime there is a character that isn't i, inventoryOpen is false
            gamePlayer->setInventoryOpen(false);
            descend();
            
        }

        gamePlayer->regainHitPoint(maxHitPoints);
        gamePlayer->move(ch);
        mGameTemple->moveMonsters(gamePlayer);

        
        mGameTemple->display();
        
    }
 
}

void Game::descend()
{
    Player* oldPlayer = mGameTemple->getPlayer();
    int playerHitPoints = oldPlayer->getHitPoints();
    int playerArmor = oldPlayer->getArmor();
    int playerStrength = oldPlayer->getStrength();
    int playerDexterity = oldPlayer->getDexterity();
    Weapon* playerWeapon = oldPlayer->getWeapon();
    
    int nextLvl = mGameTemple->getLevel() + 1;
    delete mGameTemple;
    mGameTemple = new Temple(mGoblinSmellDistance, nextLvl);
    
    mGameTemple->placePlayerRandomly();
    
    Player* newPlayer = mGameTemple->getPlayer();
    newPlayer->setTemple(mGameTemple);
    newPlayer->setHitPoints(playerHitPoints);
    newPlayer->setArmor(playerArmor);
    newPlayer->setStrength(playerStrength);
    newPlayer->setDexterity(playerDexterity);
    newPlayer->equipWeapon(playerWeapon);
    
    mGameTemple->placeDescendPoint();
    mGameTemple->generateMonsters();
    mGameTemple->placeMultipleWeapons();
    mGameTemple->placeMultipleScrolls();
}




// You will presumably add to this project other .h/.cpp files for the various
// classes of objects you need to play the game:  player, monsters, weapons, 
// etc.  You might have a separate .h/.cpp pair for each class (e.g., Player.h,
// Bogeyman.h, etc.), or you might put the class declarations for all actors in
// Actor.h, all game objects in GameObject.h, etc.
