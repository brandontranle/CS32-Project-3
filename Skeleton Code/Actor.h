#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public: 
	Actor(int imageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* world): GraphObject(imageID,startX,startY,startDirection,depth,size), m_world(world), alive(true){};
	StudentWorld* getWorld() { return m_world; };
	bool isAlive() { return alive; };
	virtual void remove() { alive = false; };
	virtual void doSomething() = 0;
	virtual bool isDirectional() { return false; };
	virtual void needsToTurn(bool i) { return; }
	virtual int getDieRoll() const { return 0; }
	virtual bool isWalking() const { return false; }


	virtual void addCoins() { return; }
	virtual void deductCoins() { return; }
	virtual bool receivedAward() { return false; }
	virtual void addStar() { return; }
	int getCoins() const { return 0; }






	virtual void forceTurn(bool i) { return; }
	virtual void forceRight(bool i) { return; }
	virtual void forceLeft(bool i) { return; }
	virtual void forceUp(bool i) { return; }
	virtual void forceDown(bool i) { return; }
	

	virtual void canTurnRight(bool i) { return; };
	virtual void canTurnLeft(bool i) { return; };
	virtual void canTurnUp(bool i) { return; };
	virtual void canTurnDown(bool i) { return; };

	virtual bool isMovingRight() { return false; }
	virtual bool isMovingUp() { return false; }
	virtual bool isMovingDown() { return false; }
	virtual bool isMovingLeft() { return false; }



private: 
	StudentWorld* m_world;
	bool alive;


};





//======================================

//PLAYER(S)
class Player_Avatar : public Actor {
public:
	Player_Avatar(int imageID, int startX, int startY, int startDirection, StudentWorld* world, int p_num, int coins, int stars, bool waiting)
		:Actor(imageID, startX, startY, startDirection, 0, 1.0, world), ticks_to_move(0), m_playerNum(p_num), die_roll(0), m_Coins(coins), m_Stars(stars), waiting(true), needToTurn(false)
		, movingDown(false), movingUp(false), movingRight(true), movingLeft(false), forcedTurn(false), received(false) {};
	virtual void doSomething();

	
	//USEFUL FUNCTIONS
	int getDieRoll() const;
	int getStars() const;
	int getCoins() const;
	void addCoins();
	void deductCoins();
	void addStar();
	void needsToTurn(bool i) { needToTurn = i; }
	bool isWalking() const;


	//DIRECTIONAL SQUARE ECOUNTERS || CORNER PIECES 
	void forceTurn(bool i) { forcedTurn = i; }
	void forceRight(bool i) { movingRight = i; movingDown = false; movingLeft = false; movingUp = false; }
	void forceLeft(bool i) { movingLeft = i; movingRight = false; movingUp = false; movingDown = false; }
	void forceUp(bool i) { movingUp = i; movingRight = false; movingDown = false; movingLeft = false; }
	void forceDown(bool i) { movingDown = i; movingRight = false; movingUp = false; movingLeft = false; }

	bool isMovingRight() { return movingRight; }
	bool isMovingUp() { return movingUp; }
	bool isMovingDown() { return movingDown; }
	bool isMovingLeft() { return movingLeft; }



	void canTurnRight(bool i) { canTurnR = i;};
	void canTurnLeft(bool i) {  canTurnL = i;  };
	void canTurnUp(bool i) {canTurnU = i; };
	void canTurnDown(bool i) { canTurnD = i; };

	bool receivedAward() { return received; }

	
private:
	int m_Coins;
	int m_Stars;
	int m_playerNum{};
	int ticks_to_move;
	int die_roll;
	bool waiting;


	//FOR TURNS
	bool needToTurn;

	//CHECK FOR MOVEMENTS
	bool movingDown;
	bool movingUp;
	bool movingRight;
	bool movingLeft;

	bool canTurnR;
	bool canTurnL;
	bool canTurnU;
	bool canTurnD;

	//FOR DIRECTIONAL
	bool forcedTurn;

	bool received;

};


//PLAYER 1
class Yoshi : public Player_Avatar {
public:
	Yoshi(int m_x, int m_y, StudentWorld* world):
		Player_Avatar(IID_YOSHI, SPRITE_WIDTH * m_x, SPRITE_HEIGHT * m_y, right, world, 2, 0, 0, true) {};
	void doSomething();

};

//PLAYER 2
class Peach : public Player_Avatar {
public:
	Peach(int m_x, int m_y, StudentWorld* world):
		Player_Avatar(IID_PEACH, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world, 1, 0, 0, true){};
	void doSomething();



};


//======================================

//ENEMIES
class Monster : public Actor {
public:
	Monster(int imageID, int startX, int startY, int startDirection, StudentWorld* world)
		 : Actor(imageID, startX, startY, startDirection, 0, 1.0, world) {};
	void doSomething();

};
class Bowser: public Monster {
public:
	Bowser(int m_x, int m_y, StudentWorld* world)
		: Monster(IID_BOWSER, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {};
};

class Boo : public Monster {
public:
	Boo(int m_x, int m_y, StudentWorld* world) 
		: Monster(IID_BOO, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {};



};

class Projectile : public Actor {


};

class Vortex : public Projectile {


};


//======================================

//LAND-ONS
class Square: public Actor {

public: 
	Square(int imageID, int startX, int startY, int startDirection, StudentWorld* world)
		:Actor(imageID, startX, startY, startDirection, 1, 1.0, world) {};
	virtual void doSomething() = 0;
	virtual bool isDirectional() { return false; };

protected:
	virtual void remove();

private:

};

class BlueCoinSquare: public Square {

public:
	BlueCoinSquare(int m_x, int m_y, StudentWorld* world):
		Square(IID_BLUE_COIN_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {};
	void doSomething();

private:
	virtual void remove(); //changes to dropping square
};


class RedCoinSquare : public Square {

public:
	RedCoinSquare(int m_x, int m_y, StudentWorld* world) :
		Square(IID_RED_COIN_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {};
	void doSomething();


};

class StarSquare:public Square {
public:
	StarSquare(int m_x, int m_y, StudentWorld* world) :
		Square(IID_STAR_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {};
	void doSomething();

};

class DirectionalSquare : public Square {
public:
	DirectionalSquare(int m_x, int m_y, StudentWorld* world, int direction) :
		Square(IID_DIR_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, direction, world) {};
	void doSomething();
	virtual bool isDirectional() { return true; }
};




class EventSquare : public Square {

public:

	EventSquare(int m_x, int m_y, StudentWorld* world) :
		Square(IID_EVENT_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {};
	void doSomething();

};

class BankSquare: public Square {
public:
	BankSquare(int m_x, int m_y, StudentWorld* world) :
		Square(IID_BANK_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {};
	void doSomething();

};

class DroppingSquare : public Square {
public:
	DroppingSquare(int m_x, int m_y, StudentWorld* world) :
		Square(IID_DROPPING_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {};
	void doSomething();

};





#endif // ACTOR_H_
