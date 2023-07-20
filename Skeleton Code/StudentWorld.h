#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  StudentWorld::~StudentWorld();

  //USEFUL FUNCTIONS
  bool needToTurn(Player_Avatar* p);
  bool overlappingFirstPlayer(Actor* a);
  bool overlappingSecondPlayer(Actor* a);

  
  int getMax_X();//get the max x coordinate of the board
  int getMax_Y(); //get the max y coordinate of the board

  int getMin_X();
  int getMin_Y();


  bool detectAheadFirstPlayer(int x, int y);
  bool detectAheadSecondPlayer(int x, int y);


  bool overlappingDirectionFirst(Player_Avatar* a);

  Actor* getFirstPlayer() { return players.front();}
  Actor* getSecondPlayer() { return players.back(); }




private:

	std::vector<Player_Avatar*> players;
	std::vector<Actor*> actors;

};

#endif // STUDENTWORLD_H_
