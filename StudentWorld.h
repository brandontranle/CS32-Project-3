#ifndef STUDENTWORLD_H
#define STUDENTWORLD_H

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:


    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

    //destructor
    ~StudentWorld();

    //check if object overlapping players
    bool overlappingPlayer1(Actor* a);
    bool overlappingPlayer2(Actor* a);


    //returning the player(s)
    Actor* getPlayer1() { return players.front(); }
    Actor* getPlayer2() { return players.back(); }


    //FOR PLAYER MOVEMENT
    bool isValidPosition(int dir, Actor* a);
    bool onDirSquare(Actor* a);

    //FOR EVENT SQUARE
    void teleport(Actor* a);
    void swap(Actor* a, Actor* b);
    bool StudentWorld::onEventSquare(Actor* a);


    //FOR BOO
    void swapStats(Actor* a, Actor* b);

    //ADD NEW OBJECTS
    void addVortex(Actor* a);
    void addDroppingSquare(int x, int y, Actor* a);

    Actor* getSquare(Actor* a);
    int forkEncounter(Actor* a);

    //BANK FUNCTIONALITY
    void addToBank(int i);
    int getBank() const { return bank; }
    void resetBank();

  
    //OVERLAP DETECTION
    bool overlappingObject(Actor* a);
    bool overlapping(Actor* a, Actor* b);
    bool overlappingPixel(int x1, int y1, int x2, int y2);
    bool checkSquares(Actor* a, Actor* b);
    bool overlappingTwoObjects(Actor* a);




private:

    std::vector<Actor*> actors;
    std::vector<Player_Avatar*> players;

    Board tempBoard;
    int bank;


};

#endif