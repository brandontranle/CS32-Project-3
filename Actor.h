#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"

/*===============================
HIERARCHY:
		ACTOR
			PLAYER_AVATAR
					YOSHI
					PEACH
			SQUARE
				BLUE COIN
				RED COIN
				STAR
				EVENT
				BANK
				DROPPING
			ENEMY
				BOWSER
				BOO
			PROJECTILE
				VORTEX
================================*/

class StudentWorld;
/*====================
	ACTOR CLASS 
======================*/
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* world)
		: GraphObject(imageID, startX, startY, startDirection, depth, size), m_world(world), alive(true), impacted(false) {};

	StudentWorld* getWorld() { return m_world; } //the most useful function
	//returns alive for actors
	 bool isAlive() {return alive;}
	void isAlive(bool i) { alive = i; }

	void add() { alive = true; }
	void remove() { alive = false;}


	//pure virtual since each avator has different "things to do" lol
	virtual void doSomething() = 0;


	//functions for Player Avatar ~ so other classes can have access and control the player when necessary
	virtual void setGoingRight() { return; }
	virtual void setGoingLeft() { return; }
	virtual void setGoingDown() { return; }
	virtual void setGoingUp() { return; }
	virtual bool getReceivedItemStatus() { return false; }
	virtual bool getBankEncounter() { return false; }
	virtual bool getEventEncounter() { return false; }

	//accessor and modifying functions for Player Avatar ~ so other classes can indirectly modify Player_Avatar values when "it" lands on a square.
	
	//COINS
	virtual void addCoins(int i) { return; }
	virtual int getCoins() const { return 0; }
	virtual void removeCoins(int i) { return; }
	virtual void setCoins(int i) { return; }
	virtual void b_removeCoins(int i) { return; }


	//STARS
	virtual void addStar() { return; }
	virtual int getStars() const { return 0;}
	virtual void setStars(int i) { return; }
	virtual void removeStars(int i) { return; }

	//PLAYER_AVATAR MOVEMENT RELATED FUNTIONS
	virtual int getDieRoll() const { return 0; }
	virtual int getMovingDirection() const { return 0; }
	virtual bool getStatus() const { return true; }
	virtual void setStatus(bool status) { return; }
	virtual int getTicks() { return 0; }
	virtual void setTicks(int num) { return; }
	virtual void setMovingDirection(int dir) { return; }

	//TELEPORTATION FOR EVENTS/VORTEXES FUNCTIONALITY
	virtual bool getTeleportStatus() const { return false; }
	virtual void setTeleportStatus(bool stat) { return; }
	virtual void setDieRoll(int i) { return;  }
	virtual int getVortexes() { return 0; }
	virtual void setVortexes(int i) { return; }
	virtual void setSwapStatus(bool stat) { return; }
	virtual bool getSwapStatus() { return false; }

	


	//VORTEX FUNCTIONS
	virtual bool can_be_hit_by_vortex() const = 0; //This function is pure virtual because not every object can be affected by a vortex
	virtual bool isActive() { return false; }
	virtual void setActive(bool i) { return; }
	virtual void increaseVortexes() { return; } //used to increase the number of vortexes a player is holding
	void setImpactStatus(bool i) { impacted = i; } //The two functions below are used to indicate when an object is HIT by a vortex
	bool getImpactStatus() { return impacted; }

	
	virtual bool is_a_square() const = 0; //Indicates whether an object is a square

	virtual bool is_a_dropping() const = 0;
	void setReceivedItemStatus(bool i) { return; }



private:
	StudentWorld* m_world; //the world.
	bool alive; //EVERY ACTOR NEEDS AN ALIVE STATUS ~ spawns in as true
	bool impacted; //FOR ENEMIES AND POSSIBLY PLAYERS IF EVER IMPEMENTED FOR FUTURE USE

};

/*=======================*/
/*       PLAYERS         */
/*=======================*/

class Player_Avatar : public Actor {

public:

	Player_Avatar(int imageID, int startX, int startY, int startDirection, StudentWorld* world, int num, int coins, int stars, bool wait)
		: Actor(imageID, startX, startY, startDirection, 0, 1.0, world), ticks_to_move(0), player_num(num), m_coins(coins), m_stars(stars), waiting(wait), die_roll(0), movingDirection(right), received(false), teleported(true), swapped(false), vortexes(0), deductedFromBank(false), fork(false), canTurnD(false), canTurnL(false), canTurnR(false), canTurnU(false){};
	
	virtual void doSomething();

	//useful functions
	int getCoins() const;
	int getStars() const;
	int getDieRoll() const;
	int getVortexes() const { return vortexes; }

	//SINCE ONLY ONE ITEM CAN BE RECEIVED PER TURN
	virtual bool getReceivedItemStatus() { return received; }

	//mutator functions
	void addCoins(int i);
	void removeCoins(int i);
	void removeStars(int i) { m_stars-=1; }
	void addStar();

	//USED FOR SWAP
	void setCoins(int i) { m_coins = i; } 
	void setStars(int i) { m_stars = i; }
	void setDieRoll(int i) { die_roll = i; }


	//accessor functions
	void canTurnRight(bool i) { canTurnR = i; };
	void canTurnLeft(bool i) { canTurnL = i; };
	void canTurnUp(bool i) { canTurnU = i; };
	void canTurnDown(bool i) { canTurnD = i; };

	//force movement!
	 void setGoingRight() { movingDirection = right;  }
	void setGoingLeft() { movingDirection = left;  }
	 void setGoingDown() { movingDirection = down;  }
	 void setGoingUp() { movingDirection = up;  }


	 //EVENT SQUARE FUNCTIONS
	void setMovingDirection(int dir);
	int getMovingDirection() const { return movingDirection; }
	bool getStatus() const{ return waiting; }
	int getTicks() { return ticks_to_move; }
	void setStatus(bool status) { waiting = status; }
	void setTicks(int num) { ticks_to_move = num; }
	bool getTeleportStatus() const { return teleported; }
	void setTeleportStatus(bool stat) { teleported = stat; }
	void setSwapStatus(bool stat) { swapped = stat; }
	bool getSwapStatus() { return swapped; }

	//VORTEX FUNCTIONS
	void shootVortex();
	void increaseVortexes();
	void decreaseVortexes() { vortexes--; }
	bool can_be_hit_by_vortex() const { return false; }

	//ENCOUNTERS
	virtual bool is_a_square() const { return false; }
	bool is_a_dropping() const { return false;}
	
	//BANK
	bool getBankEncounter() { return deductedFromBank; }
	void setBankEncounter(bool i) { deductedFromBank = i; }

	void setReceivedItemStatus(bool i) { received = i; } //TURN
	int getVortexes() { return vortexes; }
	void setVortexes(int i) { vortexes = i; }


	//THIS IS USED IN THE CASE THAT A PLAYER PASSES A BANK SQUARE, 
	//THEY WILL STILL RECIEVE AN OUTCOME FOR THAT TURN IF THEY LAND 
	//ON SOMETHING OTHER THAN A DIRETIONAL SQUARE
	void b_removeCoins(int i) { m_coins -= i; } 
private:

	int m_coins;
	int m_stars;
	bool received; //SINCE ONLY ONE RESULT CAN BE RECEIVED PER TURN
	bool deductedFromBank;

	int player_num{}; //KEEP TRACK OF WHO'S WHO
	int ticks_to_move;
	int die_roll;
	bool waiting;

	int movingDirection; 

	//WHILE WAITING AT FORKS, DETERMINE WHETHER:
	bool canTurnR;
	bool canTurnL;
	bool canTurnU;
	bool canTurnD;

	//INTERSECTIONS
	bool fork;

	//PREVENT TELEPORTATION TWICE
	bool teleported;
	bool swapped;

	//PROJECTILE INVENTORY
	int vortexes;


};

//PLAYER TWO
class Yoshi : public Player_Avatar {
public:
	Yoshi(int m_x, int m_y, StudentWorld* world)
		: Player_Avatar(IID_YOSHI, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world, 2, 0, 0, true) {}
	virtual void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return false; }
	bool is_a_dropping() const { return false; }



};

//PLAYER ONE
class Peach : public Player_Avatar {
public:
	Peach(int m_x, int m_y, StudentWorld* world)
		: Player_Avatar(IID_PEACH, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world, 1, 0, 0, true) {}
	virtual void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return false; }
	bool is_a_dropping() const { return false; }




};


/*=======================*/
/*       SQUARES         */
/*=======================*/

class Square : public Actor {

public:
	Square(int imageID, int startX, int startY, int startDirection, StudentWorld* world)
		: Actor(imageID, startX, startY, startDirection, 1, 1.0, world),alive(true)  {}

	void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return true; }
	bool is_a_dropping() const { return false; }


private:
	bool alive;
};

class DirectionalSquare: public Square {
public:
	DirectionalSquare(int m_x, int m_y, StudentWorld* world, int direction)
		: Square(IID_DIR_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, direction, world) {}
	void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return true; }
	bool is_a_dropping() const { return false; }




};

class StarSquare : public Square {
public:
	StarSquare(int m_x, int m_y, StudentWorld* world)
		: Square(IID_STAR_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {}
	void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return true; }
	bool is_a_dropping() const { return false; }



};

class BlueCoinSquare : public Square {
public:
	BlueCoinSquare(int m_x, int m_y, StudentWorld* world)
		: Square(IID_BLUE_COIN_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {}
	void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return true; }
	bool is_a_dropping() const { return false; }



};

class RedCoinSquare : public Square {
public:
	RedCoinSquare(int m_x, int m_y, StudentWorld* world)
		: Square(IID_RED_COIN_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {}
	void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return true; }
	bool is_a_dropping() const { return false; }



};

class EventSquare : public Square {
public:
	EventSquare(int m_x, int m_y, StudentWorld* world)
		: Square(IID_EVENT_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {}
	void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return true; }
	bool is_a_dropping() const { return false; }



};

class DroppingSquare : public Square {
public:
	DroppingSquare(int m_x, int m_y, StudentWorld* world)
		: Square(IID_DROPPING_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {}
	void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return false; }
	bool is_a_dropping() const { return true; }



};

class BankSquare : public Square {
public:
	BankSquare(int m_x, int m_y, StudentWorld* world)
		: Square(IID_BANK_SQUARE, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world) {}
	void doSomething();
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return true; }
	bool is_a_dropping() const { return false; }
};

/*=======================*/
/*       ENEMIES         */
/*=======================*/
class Enemy : public Actor {

public:
	Enemy(int imageID, int startX, int startY, int startDirection, StudentWorld* world, int count)
		: Actor(imageID, startX, startY, startDirection, 0, 1.0, world), paused(true), movingDir(right), pauseCounter(count), ticks_to_move(0), touchedP1(false), touchedP2(false){};
	void doSomething();
	bool can_be_hit_by_vortex() const { return true; }
	
	void setMovingDir(int dir) { movingDir = dir; }
	int getMovingDir() { return movingDir; }

	void setSquaresToMove(int num) { squares_to_move = num; }
	void setTicksToMove(int num) { ticks_to_move = num; }
	int getEnemyTicks() const { return ticks_to_move; }
	void decrementTicks() { ticks_to_move--; }

	int getPauseCounter() const { return pauseCounter; }
	void setPauseStatus(bool i) { paused = i; }
	void decerementPauseCounter() { pauseCounter--; }
	void setPauseCounter(int i) { pauseCounter = i; }
	bool getPauseStatus() const { return paused; }

	void setTouchStatusP1(bool i) { touchedP1 = i; }
	void setTouchStatusP2(bool i) { touchedP2 = i; }

	bool getTouchStatusP1() { return touchedP1; }
	bool getTouchStatusP2() { return touchedP2; }

	bool is_a_dropping() const { return false; } 
private:
	int pauseCounter;
	int squares_to_move;
	int ticks_to_move;
	int movingDir;
	bool paused;
	bool touchedP1;
	bool touchedP2;
};



class Bowser: public Enemy {
public:
	Bowser(int m_x, int m_y, StudentWorld* world)
		: Enemy(IID_BOWSER, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world, 180) {};
	void doSomething();
	bool can_be_hit_by_vortex() const { return true; }
	bool is_a_square() const { return false; }
	bool is_a_dropping() const { return false; }
};

class Boo: public Enemy {
public:
	Boo(int m_x, int m_y, StudentWorld* world)
		: Enemy(IID_BOO, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world, 180) {};
	void doSomething();
	bool can_be_hit_by_vortex() const { return true; }
	bool is_a_square() const { return false; }
	bool is_a_dropping() const { return false; }
};



/*=======================*/
/*      PROJECTILE(S)    */
/*=======================*/

class Projectile : public Actor {
public:
	Projectile(int imageID, int startX, int startY, int startDirection, StudentWorld* world, int move)
		: Actor(imageID, startX, startY, startDirection, 0, 1.0, world), active(true), movingDirection(move) {}
	void doSomething();
	bool is_a_dropping() const { return false; }


	bool isActive() { return active; }
	void setActive(bool i) { active = i; }
	bool can_be_hit_by_vortex() const { return false; }
	bool is_a_square() const { return false; }
	int getProjectileMoveDirection() { return movingDirection; }
private:
	bool active;
	int movingDirection;

};

class Vortex : public Projectile { //player is at a fork, a vortex cannot be fired!
public:
	Vortex(int m_x, int m_y, int move,StudentWorld* world)
		: Projectile(IID_VORTEX, SPRITE_WIDTH* m_x, SPRITE_HEIGHT* m_y, right, world, move) {}
	 void doSomething();
};

//the end.

#endif // ACTOR_H_