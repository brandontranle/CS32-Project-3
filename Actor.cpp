#include "Actor.h"
#include "StudentWorld.h"
#include <string>


void Player_Avatar::doSomething() {
	//if player is waiting
	if (waiting) {


		switch (getWorld()->getAction(player_num)) {

			//if dice is rolled
		case ACTION_ROLL:

			//set back to false when dice is rolled (while die roll is still decrementing)
			received = false;

			//if first instantiated
			if (die_roll == 0) {

				//roll dice (0-10)
				die_roll = randInt(1, 10);
				ticks_to_move = die_roll * 8;

				//change avatar state to walking state
				waiting = false;
				teleported = false;
				swapped = false;

			}
			break;

			//no key was hit
		case ACTION_NONE:

			break;
		case ACTION_LEFT:   //all action directions changes the moving direction
			if (die_roll > 0 && canTurnL) {
				setMovingDirection(left);
				movingDirection = left;
				waiting = false;
				teleported = false;
				swapped = false;
			}

			break;

		case ACTION_RIGHT:
			if (die_roll > 0 && canTurnR) {
				setMovingDirection(right);
				movingDirection = right;
				waiting = false;
				teleported = false;
				swapped = false;
			}
			break;

		case ACTION_UP:
			if (die_roll > 0 && canTurnU) {
				setMovingDirection(up);
				movingDirection = up;
				waiting = false;
				teleported = false;
				swapped = false;
			}
			break;

		case ACTION_DOWN:
			if (die_roll > 0 && canTurnD) {
				setMovingDirection(down);
				movingDirection = down;
				waiting = false;
				teleported = false;
				swapped = false;
			}
			break;

		case ACTION_FIRE:
			// add Vortex object in front of player...;
			if (vortexes > 0) {
				shootVortex();
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			//update so no more Vortex..
			break;

		default: //no key pressed; return immediately
			return;

		} // end switch



	}

	// The player is in the walking state at this point!
	if (!waiting) {


		//SWITCH-CASES FOR MOVEMENT DIRECTIONS
		switch (movingDirection) {
/*==================================
		MOVING RIGHT
====================================*/
		case right:
			
			//DEFAULT FACING DIRECTION: RIGHT
			setDirection(right);

			if (getX() % 16 == 0 && getY() % 16 == 0) {

				//A FORK ENCOUNTER WHILE GOING RIGHT IS INDICATED HERE: 
				if (getWorld()->forkEncounter(this) == 2 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
					this->canTurnRight(false);
					this->canTurnLeft(false);
					this->canTurnUp(true);
					this->canTurnDown(true);
					this->waiting = true;
					this->fork = true;
					//cerr << "fork 2 encountered while I was moving right!" << endl;
					return;
				}
				
				//CHECK WHETHER OR NOT RIGHT POSITION IS VALID
				if (getWorld()->isValidPosition(right, this)) {


					if (getWorld()->forkEncounter(this) == 1 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(true);
						this->canTurnLeft(false);
						this->canTurnUp(false);
						this->canTurnDown(true);
						this->waiting = true;
						this->fork = true;
						//cerr << "fork 1 encountered while going right!" << endl;
						return;
					}

					if (getWorld()->forkEncounter(this) == 3 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(true);
						this->canTurnLeft(false);
						this->canTurnUp(true);
						this->canTurnDown(true);
						this->waiting = true;
						this->fork = true;
						//cerr << "fork 3 encountered while going right!" << endl;
						return;
					}

					if (getWorld()->forkEncounter(this) == 4 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(true);
						this->canTurnLeft(false);
						this->canTurnUp(true);
						this->canTurnDown(false);
						this->waiting = true;
						this->fork = true;
						//cerr << "fork 4 encountered while going right!" << endl;
						return;
					}


					moveTo(getX() + 2, getY());
					ticks_to_move--;
					fork = false;
					break;

				}
				else {
				  /*================
						EDGE CASES
					================*/

					//At this point, prioritize up over down if both are available!
					if (getWorld()->isValidPosition(up, this)) {  
						movingDirection = up;
						return;
					}
					if (getWorld()->isValidPosition(down, this)) {
						movingDirection = down;
						return;
					}

					break;
				}
			}

			//Move to the right by two pixels and decrease the tick. There is no fork encounter at this point.
			moveTo(getX() + 2, getY());
			ticks_to_move--; 
			fork = false;
			break; //end the moving right case here
			/*==================================
					 MOVING LEFT
			====================================*/
		case left:

			//DEFAULT FACING DIR: LEFT
			setDirection(left);

			//CHECK IF PLAYER IS ACTUALLY ON A SQUARE FULLY
			if (getX() % 16 == 0 && getY() % 16 == 0) {

				//This is a case where a player encounters a fork while 
				if (getWorld()->forkEncounter(this) == 3 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
					this->canTurnRight(false);
					this->canTurnLeft(false);
					this->canTurnUp(true);
					this->canTurnDown(true);
					this->waiting = true;
					this->fork = true;
					//cerr << "encountered fork 3 while moving left, cannot turn left!" << endl;
					return;
				}

				//check if moving left resutls in a valid position (pass in current location and player)
				if (getWorld()->isValidPosition(left, this)) {

					if (getWorld()->forkEncounter(this) == 4 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(false);
						this->canTurnLeft(true);
						this->canTurnUp(true);
						this->canTurnDown(false);
						this->waiting = true;
						this->fork = true;
						//cerr << "encountered fork 4 while moving left, can turn left!" << endl;
						return;
					}

					if (getWorld()->forkEncounter(this) == 2 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(false);
						this->canTurnLeft(false);
						this->canTurnUp(true);
						this->canTurnDown(true);
						this->waiting = true;
						this->fork = true;
						//cerr << "encountered fork 2 while moving left, can turn left!" << endl;
						return;
					}

					if (getWorld()->forkEncounter(this) == 1 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(false);
						this->canTurnLeft(true);
						this->canTurnUp(false);
						this->canTurnDown(true);
						this->waiting = true;
						this->fork = true;
						//cerr << "encountered fork 1 while moving left, can turn left!" << endl;
						return;
					}


					moveTo(getX() - 2, getY());
					ticks_to_move--;
					fork = false;

					break;
				}
				else {
					//if can move up (higher precendence), else can go down, then move accordingly
					if (getWorld()->isValidPosition(up, this)) {  //returns false if not empty

						movingDirection = up;
						return;
					}
					if (getWorld()->isValidPosition(down, this)) {

						movingDirection = down;
						return;
					}

				}
			}

			//move to the left, decrease ticks
			moveTo(getX() - 2, getY());
			ticks_to_move--;
			fork = false;

			break;
/*==================
	 MOVING UP
====================*/

		case up:
			
			//set facing direction
			setDirection(right);

			if (getX() % 16 == 0 && getY() % 16 == 0) {

				if (getWorld()->forkEncounter(this) == 1 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
					this->canTurnRight(true);
					this->canTurnLeft(true);
					this->canTurnUp(false);
					this->canTurnDown(false);
					this->waiting = true;
					this->fork = true;
				//	cerr << "fork 1 ecountered while going up" << endl;
					return;
				}

				//check if moving up resutls in a valid position (pass in current location and player)
				if (getWorld()->isValidPosition(up, this)) {
					if (getWorld()->forkEncounter(this) == 3 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(true);
						this->canTurnLeft(false);
						this->canTurnUp(true);
						this->canTurnDown(false);
						this->waiting = true;
						this->fork = true;
					//	cerr << "fork 3 ecountered while going up" << endl;
						return;
					}

					if (getWorld()->forkEncounter(this) == 2 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(false);
						this->canTurnLeft(true);
						this->canTurnUp(true);
						this->canTurnDown(false);
						this->waiting = true;
						this->fork = true;
					//	cerr << "fork 2 ecountered while going up" << endl;
						return;
					}

					if (getWorld()->forkEncounter(this) == 4 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(true);
						this->canTurnLeft(true);
						this->canTurnUp(true);
						this->canTurnDown(false);
						this->waiting = true;
						this->fork = true;
						//	cerr << "fork 4 ecountered while going up" << endl;
						return;
					}

					moveTo(getX(), getY() + 2);
					ticks_to_move--;
					fork = false;

					break;
				}
				else {

					//if can move right (higher precendence), else can go left, move accordingly 

					if (getWorld()->isValidPosition(right, this)) {
						movingDirection = right;
						return;
					}

					if (getWorld()->isValidPosition(left, this)) {
						movingDirection = left;
						return;
					}


				}
			}

			moveTo(getX(), getY() + 2);
			ticks_to_move--;
			fork = false;

			break;

/*==================
	 MOVING DOWN
====================*/

		case down:

			setDirection(right);

			if (getX() % 16 == 0 && getY() % 16 == 0) {

				if (getWorld()->forkEncounter(this) == 4 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
					this->canTurnRight(true);
					this->canTurnLeft(true);
					this->canTurnUp(false);
					this->canTurnDown(false);
					this->waiting = true;
					this->fork = true;
					//cerr << "fork 4 ecountered while going down, cannot go down!" << endl;

					return;
				}

				//check if moving down resutls in a valid position (pass in current location and player)
				if (getWorld()->isValidPosition(down, this)) {

					if (getWorld()->forkEncounter(this) == 3 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(true);
						this->canTurnLeft(false);
						this->canTurnUp(false);
						this->canTurnDown(true);
						this->waiting = true;
						this->fork = true;
						//cerr << "fork 3 ecountered while going down" << endl;

						return;
					}
					if (getWorld()->forkEncounter(this) == 2 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(false);
						this->canTurnLeft(true);
						this->canTurnUp(false);
						this->canTurnDown(true);
						this->waiting = true;
						this->fork = true;
						//cerr << "fork 2 ecountered while going down" << endl;

						return;
					}

					if (getWorld()->forkEncounter(this) == 1 && fork == false && ((!(getWorld()->onDirSquare(this))) || (getWorld()->getSquare(this)->is_a_dropping()) || getWorld()->overlappingTwoObjects(this))) {
						this->canTurnRight(true);
						this->canTurnLeft(true);
						this->canTurnUp(false);
						this->canTurnDown(true);
						this->waiting = true;
						this->fork = true;
						//cerr << "fork 1 ecountered while going down" << endl;
						return;
					}

					moveTo(getX(), getY() - 2);
					ticks_to_move--;
					fork = false;

					break;
				}
				else {

					//if can move right (higher precendence), else can go left, move accordingly 

					if (getWorld()->isValidPosition(right, this)) {
						movingDirection = right;
						return;
					}

					if (getWorld()->isValidPosition(left, this)) {
						movingDirection = left;
						return;
					}

				}
			}

			//move to the down, decrease ticks
			moveTo(getX(), getY() - 2);
			ticks_to_move--;
			fork = false;

			break;


		default:
			break;
		} //end switch



		//DECREMENT DIE ROLLS EVERY 8 TICKS
		if (ticks_to_move % 8 == 0) {
			die_roll--;
		}

		//RESET TO WAITING STATE
		if (ticks_to_move == 0 || die_roll == 0) {
			waiting = true;
			teleported = false;
		}





	}

} //END OF PLAYER DOSOEMTHING

//YOSHI AND PEACH WILL DO THE COMMON THING
void Yoshi::doSomething() {
	Player_Avatar::doSomething();
}

void Peach::doSomething() {
	Player_Avatar::doSomething();
}

/*==================
	   ENEMIES
====================*/

//ENEMY COMMON THING
void Enemy::doSomething() {

	setTouchStatusP1(false);
	setTouchStatusP2(false);

	if (getX() % 16 == 0 && getY() % 16 == 0) {  //if bowser is on a square 

		if (getWorld()->forkEncounter(this) == 1) {
			int move = randInt(1, 3);
			if (move == 1) {
				setMovingDir(right);
			}
			else if (move == 2) {
				setMovingDir(left);

			}
			else setMovingDir(down);
		}

		if (getWorld()->forkEncounter(this) == 2) {
			int move = randInt(1, 3);
			if (move == 1) {
				setMovingDir(down);
			}
			else if (move == 2) {
				setMovingDir(left);

			}
			else setMovingDir(up);
		}
		if (getWorld()->forkEncounter(this) == 3) {
			int move = randInt(1, 3);
			if (move == 1) {
				setMovingDir(right);
			}
			else if (move == 2) {
				setMovingDir(up);

			}
			else setMovingDir(down);
		}

		if (getWorld()->forkEncounter(this) == 4) {
			int move = randInt(1, 3);
			if (move == 1) {
				setMovingDir(up);
			}
			else if (move == 2) {
				setMovingDir(left);

			}
			else setMovingDir(right);
		}

	}

	switch (getMovingDir()) {
	case right:
		if (getX() % 16 == 0 && getY() % 16 == 0) {  //if bowser is on a square 
			if (getWorld()->isValidPosition(right, this)) { //right valid
				setDirection(right);
				moveTo(getX() + 2, getY());
				decrementTicks();
				break;
			} //end of right valid
			else { //start of invalid right
				//cerr << "Bowser can't move right!" << endl;

				if (getWorld()->isValidPosition(up, this)) {
					setDirection(right);
					setMovingDir(up);
					return;
				}

				if (getWorld()->isValidPosition(down, this)) {
					setDirection(right);
					setMovingDir(down);
					return;
				}
				break;
			} //end of valid right
		} //end of square right


		moveTo(getX() + 2, getY());
		decrementTicks();
		break;

	case left:
		if (getX() % 16 == 0 && getY() % 16 == 0) {  //if bowser is on a square 
			if (getWorld()->isValidPosition(left, this)) { //left valid
				setDirection(left);
				moveTo(getX() - 2, getY());
				decrementTicks();
				break;
			} //end of left valid
			else { //start of invalid left
			//	cerr << "Bowser can't move left!" << endl;

				if (getWorld()->isValidPosition(up, this)) {
					setDirection(right);
					setMovingDir(up);
					return;
				}

				if (getWorld()->isValidPosition(down, this)) {
					setDirection(right);
					setMovingDir(down);
					return;
				}
				break;
			} //end of valid right
		} //end of square right


		moveTo(getX() - 2, getY());
		decrementTicks();
		break;
	case up:
		if (getX() % 16 == 0 && getY() % 16 == 0) {  //if bowser is on a square 
			if (getWorld()->isValidPosition(up, this)) { //up valid
				setDirection(right);
				moveTo(getX(), getY() + 2);
				decrementTicks();
				break;
			} //end of up valid
			else { //start of invalid up
				//cerr << "Bowser can't move up!" << endl;

				if (getWorld()->isValidPosition(right, this)) {
					setDirection(right);
					setMovingDir(right);
					return;
				}

				if (getWorld()->isValidPosition(left, this)) {
					setDirection(left);
					setMovingDir(left);
					return;
				}
				break;
			} //end of valid up
		} //end of square up
		moveTo(getX(), getY() + 2);
		decrementTicks();
		break; //end of up case
	case down:
		if (getX() % 16 == 0 && getY() % 16 == 0) {  //if bowser is on a square 
			if (getWorld()->isValidPosition(down, this)) { //down valid
				setDirection(right);
				moveTo(getX(), getY() - 2);
				decrementTicks();
				break;
			} //end of down valid
			else { //start of invalid down
				//cerr << "Bowser can't move down!" << endl;

				if (getWorld()->isValidPosition(right, this)) {
					setDirection(right);
					setMovingDir(right);
					return;
				}

				if (getWorld()->isValidPosition(left, this)) {
					setDirection(left);
					setMovingDir(left);
					return;
				}
				break;
			} //end of valid down
		} //end of square down


		moveTo(getX(), getY() - 2);
		decrementTicks();
		break;
	} //end of switch statement
	

}

void Boo::doSomething() {

	//STRUCK BY A VORTEX
	if (getImpactStatus()) {
		getWorld()->teleport(this);
		setPauseStatus(true);
		setPauseCounter(180);
		setDirection(right);
		setMovingDir(right);
		setImpactStatus(false);
		return;
	}


	if (getPauseStatus()) { //start ~ if its paused

			
		if ((getWorld()->getPlayer1()->getDieRoll() == 0 && getWorld()->overlappingPlayer1(this) && (!(getTouchStatusP1()))) || (getWorld()->getPlayer2()->getDieRoll() == 0 && getWorld()->overlappingPlayer2(this) && (!(getTouchStatusP2())))) {


			if (getWorld()->getPlayer1()->getDieRoll() == 0 && getWorld()->overlappingPlayer1(this) && (!(getTouchStatusP1()))) {
				getWorld()->swapStats(getWorld()->getPlayer1(), getWorld()->getPlayer2());
				setTouchStatusP1(true);
			}

			if (getWorld()->getPlayer2()->getDieRoll() == 0 && getWorld()->overlappingPlayer2(this) && (!(getTouchStatusP2()))) {
				getWorld()->swapStats(getWorld()->getPlayer1(), getWorld()->getPlayer2());
				setTouchStatusP2(true);
			}

			getWorld()->playSound(SOUND_BOO_ACTIVATE);
		}
		

		decerementPauseCounter();

		if (getPauseCounter() == 0) {
			int boo_moves = randInt(1, 3);
			setSquaresToMove(boo_moves);

			int ticks = boo_moves * 8;
			setTicksToMove(ticks);

			if (getX() % 16 == 0 && getY() % 16 == 0) { //check if boo is on a square
				if (getWorld()->isValidPosition(right, this)) { //check right!

					if (getWorld()->isValidPosition(left, this)) { //check left!
						int choose = randInt(1, 2); //choose randomly
						if (choose == 1) {
							setMovingDir(right);
						}
						else {
							setMovingDir(left);
							setPauseStatus(false);
							//std::cerr << "im gonna move left" << std::endl;
							setDirection(left);
						}
					}
					setMovingDir(right); //otherwise just go right
					setDirection(right);
					//std::cerr << "im gonna move right" << std::endl;
					setPauseStatus(false);
					return;

				}


				if (getWorld()->isValidPosition(up, this)) { //check up!

					if (getWorld()->isValidPosition(down, this)) { //check down.
						int choose = randInt(1, 2); //choose randomly
						if (choose == 1) {
							setMovingDir(up);
						}
						else {
							setMovingDir(down);
							setPauseStatus(false);
							//std::cerr << "im gonna move down" << std::endl;
							setDirection(right);
							return;
						}
					}
					setMovingDir(up); //otherwise just go up.
					setDirection(right);
					setPauseStatus(false);
					//std::cerr << "im gonna move up" << std::endl;
					return;
				}
				if (getWorld()->isValidPosition(down, this)) { //check down

					if (getWorld()->isValidPosition(up, this)) { //check up
						int choose = randInt(1, 2); //choose randomly
						if (choose == 1) {
							setMovingDir(down);
						}
						else {
							setMovingDir(up);
							setPauseStatus(false);
							//std::cerr << "im gonna move up" << std::endl;
							setDirection(right);
							return;
						}
					}
					setMovingDir(down); //just go down
					setDirection(right);
					setPauseStatus(false);
					//std::cerr << "im gonna move down" << std::endl;
					return;
				}

				if (getWorld()->isValidPosition(left, this)) { //check left

					if (getWorld()->isValidPosition(right, this)) { //check right
						int choose = randInt(1, 2);
						if (choose == 1) {
							setMovingDir(left);
						}
						else {
							setMovingDir(right);
							setPauseStatus(false);
							//std::cerr << "im gonna move right" << std::endl;
							setDirection(right);
							return;
						}
					}
					setMovingDir(left); //otherwise just go left
					setDirection(right);
					setPauseStatus(false);
					//std::cerr << "im gonna move left" << std::endl;
					return;
				}
				return;
			} //ending of checking whether boo is on a square
		} //end of zero 
	} //pause ending


	if ((!(getPauseStatus()))) { //start
		//DO THE COMMON THING
		Enemy::doSomething();

		//RESET TO PAUSE STATE
		if (getEnemyTicks() == 0) {
			setPauseStatus(true);
			setPauseCounter(180);
		}

	}

	return;
} //boo is done

void Bowser::doSomething() {

	//STRUCK BY A VORTEX
	if (getImpactStatus()) {
		getWorld()->teleport(this);
		setDirection(right);
		setMovingDir(right);
		setPauseStatus(true);
		setPauseCounter(180);
		setImpactStatus(false);
		return;
	}


	if (getPauseStatus()) { //start

			
			if (getWorld()->overlappingPlayer1(this) && (!getTouchStatusP1()) && getWorld()->getPlayer1()->getDieRoll() == 0) {
				int chance = randInt(1, 2);
				if (chance == 1) {
					getWorld()->getPlayer1()->setCoins(0);
					setTouchStatusP1(true);
					getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				}
			}

			if (getWorld()->overlappingPlayer2(this) && (!getTouchStatusP2()) && getWorld()->getPlayer2()->getDieRoll() == 0) {
				int chance = randInt(1, 2);
				if (chance == 1) {
					getWorld()->getPlayer2()->setCoins(0);
					setTouchStatusP2(true);
					getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				}
			}
			


		decerementPauseCounter();

		if (getPauseCounter() == 0) {

			int move = randInt(1, 10);
			setSquaresToMove(move);

			int ticks = move * 8;
			setTicksToMove(ticks);

			
			if (getX() % 16 == 0 && getY() % 16 == 0) {
				if (getWorld()->isValidPosition(right, this)) {

					if (getWorld()->isValidPosition(left, this)) {
						int choose = randInt(1, 2);
						if (choose == 1) {
							setMovingDir(right);
							}
						else {
							setMovingDir(left);
							setPauseStatus(false);
							//std::cerr << "im gonna move left" << std::endl;
							setDirection(left);
							}
						}
					setMovingDir(right);
					setDirection(right);
					//std::cerr << "im gonna move right" << std::endl;
					setPauseStatus(false);
					return;

				}


				if (getWorld()->isValidPosition(up, this)) {

					if (getWorld()->isValidPosition(down, this)) {
						int choose = randInt(1, 2);
						if (choose == 1) {
							setMovingDir(up);
						}
						else {
							setMovingDir(down);
							setPauseStatus(false);
							//std::cerr << "im gonna move down" << std::endl;
							setDirection(right);
							return;
						}
					}
					setMovingDir(up);
					setDirection(right);
					setPauseStatus(false);
					//std::cerr << "im gonna move up" << std::endl;
				}

				if (getWorld()->isValidPosition(down, this)) {

					if (getWorld()->isValidPosition(up, this)) {
						int choose = randInt(1, 2);
						if (choose == 1) {
							setMovingDir(down);
						}
						else {
							setMovingDir(up);
							setPauseStatus(false);
							//std::cerr << "im gonna move up" << std::endl;
							setDirection(right);
							return;
						}
					}
					setMovingDir(down);
					setDirection(right);
					setPauseStatus(false);
					//std::cerr << "im gonna move down" << std::endl;
					return;
				}

				if (getWorld()->isValidPosition(left, this)) {

					if (getWorld()->isValidPosition(right, this)) {
						int choose = randInt(1, 2);
						if (choose == 1) {
							setMovingDir(left);
						}
						else {
							setMovingDir(right);
							setPauseStatus(false);
							//std::cerr << "im gonna move right" << std::endl;
							setDirection(right);
							return;
						}
					}
					setMovingDir(left);
					setDirection(right);
					setPauseStatus(false);
					//std::cerr << "im gonna move left" << std::endl;
					return;
				}

				return;

			}

		} //end of pause counter being zero

	} //end of pause

	if ((!(getPauseStatus()))) { //start

			//do common thing
			Enemy::doSomething();

			if (getEnemyTicks() == 0) {
				setPauseStatus(true);
				setPauseCounter(180);
				int chance = randInt(1, 4); //25% chance
				if (chance == 1) {
					//find the x and y values of the square / bowser
					int x = getX();
					int y = getY();

						//remove the original square
						//add the dropping square
					if (getWorld()->getSquare(this)->is_a_square()) {
						getWorld()->getSquare(this)->remove();
						getWorld()->addDroppingSquare(x / SPRITE_WIDTH, y / SPRITE_HEIGHT, this);
					}
					return;
				}

			}
			return;

	} //end of pause status not being true 

} //end of bowser doSomething()

void Square::doSomething() {} //squares will not do the common thing

/*========================
	 DIRECTIONAL SQUARE
==========================*/
void DirectionalSquare::doSomething() {

	//IF PLAYER 1 ENCOUNTERS A DIRECTIONAL SQUARE
	if (getWorld()->overlappingPlayer1(this)) {  
		//FORCE DIRECTIONS ONTO PLAYER 1 WHEN ON TOP OF A DIRECTIONAL SQUARE
		switch (this->getDirection()) {
		case right:
			getWorld()->getPlayer1()->setGoingRight();
			break;
		case left:
			getWorld()->getPlayer1()->setGoingLeft();
			break;
		case up:
			getWorld()->getPlayer1()->setGoingUp();
			break;
		case down:
			getWorld()->getPlayer1()->setGoingDown();
			break;

		default:
			return;
		}
		return;
	}

	//IF PLAYER 2 ENCOUNTERS A DIRECTIONAL SQUARE
	if (getWorld()->overlappingPlayer2(this)) {
		//FORCE DIRECTIONS ONTO PLAYER 2 WHEN ON TOP OF A DIRECTIONAL SQUARE
		switch (this->getDirection()) {
		case right:
			getWorld()->getPlayer2()->setGoingRight();
			break;
		case left:
			getWorld()->getPlayer2()->setGoingLeft();
			break;
		case up:
			getWorld()->getPlayer2()->setGoingUp();
			break;
		case down:
			getWorld()->getPlayer2()->setGoingDown();
			break;
		default:
			return;
		}
		
		return;
	}

}

/*========================
	 BLUE COIN SQUARE
==========================*/

void BlueCoinSquare::doSomething() {

	//IF PLAYER1 || PLAYER2 LAND ON A BLUE COIN SQUARE, INCREASE COINS BY THREE.

	//PLAYER1
	if (getWorld()->overlappingPlayer1(this)) {
		//if player has not received item AND die roll ended/began
		if (!(getWorld()->getPlayer1()->getReceivedItemStatus()) && getWorld()->getPlayer1()->getDieRoll() == 0) {
			getWorld()->getPlayer1()->addCoins(3);
			getWorld()->playSound(SOUND_GIVE_COIN);
		}
	}

	//PLAYER2
	if (getWorld()->overlappingPlayer2(this)) {
		//if player has not received item AND die roll ended/began
		if (!(getWorld()->getPlayer2()->getReceivedItemStatus()) && getWorld()->getPlayer2()->getDieRoll() == 0) {
			getWorld()->getPlayer2()->addCoins(3);
			getWorld()->playSound(SOUND_GIVE_COIN);
		}
	}

}

/*========================
	 RED COIN SQUARE
==========================*/

void RedCoinSquare::doSomething() {

	//IF PLAYER1 || PLAYER2 LAND ON A RED COIN SQUARE, DECREASE COINS BY THREE.

	//PLAYER1
	if (getWorld()->overlappingPlayer1(this)) {
		//if player has not received item AND die roll ended/began
		if (!(getWorld()->getPlayer1()->getReceivedItemStatus()) && getWorld()->getPlayer1()->getDieRoll() == 0) {
			getWorld()->getPlayer1()->removeCoins(3);
			getWorld()->playSound(SOUND_TAKE_COIN);
		}
	}

	//PLAYER2
	if (getWorld()->overlappingPlayer2(this)) {
		//if player has not received item AND die roll ended/began
		if (!(getWorld()->getPlayer2()->getReceivedItemStatus()) && getWorld()->getPlayer2()->getDieRoll() == 0) {
			getWorld()->getPlayer2()->removeCoins(3);
			getWorld()->playSound(SOUND_TAKE_COIN);

		}
	}
}
/*========================
	 EVENT SQUARE
==========================*/
void EventSquare::doSomething() {

	//easier functionality
	Actor* a = getWorld()->getPlayer1();
	Actor* b = getWorld()->getPlayer2();

	//IF PLAYER1 LANDS ON AN EVENT SQUARE
	if (getWorld()->overlappingPlayer1(this) && a->getDieRoll() == 0 && (!(a->getReceivedItemStatus())) && (!(a->getTeleportStatus()))) {
		int option = randInt(1,3);

		if (option == 1) { //NORMAL TELEPORTATION
			if ((!(a->getTeleportStatus()))) {
				getWorld()->teleport(a);
				a->setTeleportStatus(true);
				//cerr << "teleported p1!" << endl;
				if (a->getWorld()->onEventSquare(a)) {
					option = randInt(1, 3);
					if (option == 1) {
						getWorld()->teleport(a);
					}
					else if (option == 2) {
						getWorld()->swap(a, b);
						b->setSwapStatus(true); //prevents infinite loop
						//a->setSwapStatus(true);
						b->setTeleportStatus(true);
						//cerr << "swapped p1 with p2 after a teleport!" << endl;
					}
					else {
						a->setReceivedItemStatus(true);
						a->setTeleportStatus(true);

						a->increaseVortexes();
						//cerr << "gave vortex to p1 after a teleport!" << endl;

						getWorld()->playSound(SOUND_GIVE_VORTEX);

						//one vortex at a time!
						if (a->getVortexes() > 1)
							a->setVortexes(1);

					}

				}
			}
			return;
		}
		else if (option == 2) { //SWAP THE STATUS, MOVES, TICKS, AND POSITION
			if ((!(a->getSwapStatus()))) {
				getWorld()->swap(a, b);
				b->setSwapStatus(true); //prevents infinite loop
				//a->setSwapStatus(true);
				b->setTeleportStatus(true);
				//cerr << "swapped p1 with p2!" << endl;
			}
			return;
		}
		else if (option == 3) { //VORTEX ADDED! ~ best one
			if ((!(a->getReceivedItemStatus()))) {
				a->setReceivedItemStatus(true);
				a->setTeleportStatus(true);

				a->increaseVortexes();
				//cerr << "gave vortex to p1!" << endl;

				getWorld()->playSound(SOUND_GIVE_VORTEX);

				//one vortex at a time!
				if (a->getVortexes() > 1)
					a->setVortexes(1);
			}
			return;
		}
	} //END OF PLAYER 1 SCENARIO

	//IF PLAYER TWO LANDS ON A EVENT SQUARE
	if (getWorld()->overlappingPlayer2(this) && getWorld()->getPlayer2()->getDieRoll() == 0 && (!(b->getReceivedItemStatus())) && (!(b->getTeleportStatus()))) {
		int option = randInt(1, 3);

		if (option == 1) { //NORMAL TELEPORTATION
			if ((!(b->getTeleportStatus()))) {
				getWorld()->teleport(b);
				b->setTeleportStatus(true);
				//cerr << "teleported p2!" << endl;

				if (b->getWorld()->onEventSquare(b)) {
					option = randInt(1, 3);
					if (option == 1) {
						getWorld()->teleport(b);
					}
					else if (option == 2) {
						getWorld()->swap(a, b);
						//b->setSwapStatus(true); //prevents infinite loop
						a->setSwapStatus(true);
						a->setTeleportStatus(true);
						//cerr << "swapped p2 with p1 after a teleport!" << endl;
					}
					else {
						b->setReceivedItemStatus(true);
						b->setTeleportStatus(true);

						b->increaseVortexes();
						//cerr << "gave vortex to p2 after a teleport!" << endl;

						getWorld()->playSound(SOUND_GIVE_VORTEX);

						//one vortex at a time!
						if (b->getVortexes() > 1)
							b->setVortexes(1);
					}
				}
				return;
			}
		}
		else if (option == 2) { //SWAP THE STATUS, MOVES, TICKS, AND POSITION
			if ((!(getWorld()->getPlayer2()->getSwapStatus()))) {
				getWorld()->swap(b, a);
				//b->setSwapStatus(true);
				a->setSwapStatus(true);
				a->setTeleportStatus(true);
				//cerr << "swapped p2 with p1!" << endl;
			}
			return;
		}
		else if (option == 3) { //VORTEX ADDED! ~ best one
			if ((!(b->getReceivedItemStatus()))) {
				b->setReceivedItemStatus(true);
				b->setTeleportStatus(true);
				b->increaseVortexes();
				//cerr << "gave vortex to p2!" << endl;
				getWorld()->playSound(SOUND_GIVE_VORTEX);

				if (b->getVortexes() > 1)
					b->setVortexes(1);

			}
			
			//otherwise increase the vortexes
			return;
		}


	} //end of if statement

} 

/*========================
	 PROJECTILE(S)
==========================*/

void Projectile::doSomething() {

	switch (getProjectileMoveDirection()) {
	case right:
		moveAtAngle(right, 2);
		break;
	case left:
		moveAtAngle(left, 2);
		break;
	case up:
		moveAtAngle(up, 2);
		break;
	case down:
		moveAtAngle(down, 2);
		break;
	}
}

/*========================
		VORTEX
==========================*/

void Vortex::doSomething() {
	if (!(isAlive())) 
		return;

	Projectile::doSomething();

	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT) {
		remove();
		setActive(false);
	}

	if (getWorld()->overlappingObject(this)) {
		remove();
		setActive(false);
		getWorld()->playSound(SOUND_HIT_BY_VORTEX);

	}
	return;
}

/*========================
	DROPPING SQUARE
==========================*/
void DroppingSquare::doSomething() {

	this->isAlive(true);

	Actor* p1 = getWorld()->getPlayer1();

	if (p1->getDieRoll() == 0 && getWorld()->overlappingPlayer1(this) && !(p1->getReceivedItemStatus())) {

		int choice = randInt(1, 2);

		//FIRST OPTION: REMOVE TEN COINS 
		if (choice == 1) {
			//if the player has less than ten coins, remove the rest of them since coins cannot be negative.
			if (p1->getCoins() < 10) {
				int total = p1->getCoins();
				p1->removeCoins(total);
			}
			else p1->removeCoins(10); //otherwise just remove ten coins	
		} //first outcome

		//OPTION 2 : REMOVE A STAR! >:)
		if (choice == 2) {
			if (p1->getStars() >= 1)
				p1->removeStars(1);
		}

		//play sound
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
		return;
	}

	Actor* p2 = getWorld()->getPlayer2();


	if (p2->getDieRoll() == 0 && getWorld()->overlappingPlayer2(this) && !(p2->getReceivedItemStatus())) {

		//randomly choose two options
		int choice = randInt(1, 2);

		//first outcome
		if (choice == 1) {
			//if the player has less than ten coins, remove the rest of them since coins cannot be negative.
			if (p2->getCoins() < 10) {
				int total = p2->getCoins();
				p2->removeCoins(total);
			} else p2->removeCoins(10); //otherwise just remove ten coins	
		} //second outcome

		//OPTION 2 : REMOVE A STAR! >:)
		if (choice == 2) {
			if (p2->getStars() >= 1)
				p2->removeStars(1);
		}

		//play sound
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
		return;
	}

}

/*========================
		BANK SQUARE
==========================*/

void BankSquare::doSomething() {

	Actor* p1 = getWorld()->getPlayer1();
	Actor* p2 = getWorld()->getPlayer2();


	/*=============================================================*/
	//SCENARIOS IN WHICH THE PLAYER PASSES THE BANK SQUARE INSTEAD!
	/*=============================================================*/

	//IF PLAYER 1 PASSES OVER SQUARE
	if (getWorld()->overlappingPlayer1(this) && p1->getDieRoll() != 0) {

		
		if (p1->getCoins() < 5) {

			int lowBalance = p1->getCoins();
			getWorld()->addToBank(lowBalance); //deposit your low balance to the bank.
			p1->b_removeCoins(lowBalance); //remove your low balance for an even lower balance

			getWorld()->playSound(SOUND_DEPOSIT_BANK);
		}
		else {
			//otherwise, deposit five coins to the bank.
			p1->b_removeCoins(5);
			getWorld()->addToBank(5);

			getWorld()->playSound(SOUND_DEPOSIT_BANK);
		}
	}

	//IF PLAYER 2 PASSES OVER SQUARE ~ check the same conditions as player one
	if (p2->getDieRoll() != 0 && getWorld()->overlappingPlayer2(this)) {

		//IF PLAYER BALANCE LESS THAN 5
		if (p2->getCoins() < 5) {
			int lowBalance = p2->getCoins();
			getWorld()->addToBank(lowBalance);
			p2->b_removeCoins(lowBalance);

			getWorld()->playSound(SOUND_DEPOSIT_BANK);
		}
		else {
			p2->b_removeCoins(5);
			getWorld()->addToBank(5);
			getWorld()->playSound(SOUND_DEPOSIT_BANK);
		}
	}


	/*=============================================================*/
	//  SCENARIOS IN WHICH THE PLAYER LANDS ON THE BANK SQUARE	   //
	/*=============================================================*/

	//IF PLAYER 1 LANDED ON SQUARE
	if (getWorld()->overlappingPlayer1(this) && p1->getDieRoll() == 0 && !(p1->getReceivedItemStatus())) {

		//withdraw from the bank, reset central bank, give balance to player1
		int b_Coins = getWorld()->getBank();

		p1->addCoins(b_Coins);  //add the coins! this can only happen one time PER turn.
		getWorld()->resetBank();//reset the bank to zero.
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}

	//IF PLAYER 2 LANDED ON SQUARE
	if (getWorld()->overlappingPlayer2(this) && p2->getDieRoll() == 0 && !(p2->getReceivedItemStatus())) {

		//withdraw from the bank, reset central bank, give balance to player1
		int b_Coins = getWorld()->getBank();
		p2->addCoins(b_Coins);  //add the coins! this can only happen one time PER turn.
		getWorld()->resetBank();  //reset the bank to zero.
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}


	return;
}


/*========================
		STAR SQUARE
==========================*/
void StarSquare::doSomething() {
		
		if (getWorld()->overlappingPlayer1(this))
			getWorld()->getPlayer1()->addStar(); //subtract TWENTY coins if the player has enough, and add a star!

		if (getWorld()->overlappingPlayer2(this)) {
			getWorld()->getPlayer2()->addStar();
		}
}
//====================================
//END OF DOSOETHING() IMPLEMENTATIONS
//====================================

void Player_Avatar::increaseVortexes() { 
		//cerr << "vortex added!!" << endl;
		vortexes++;
		getWorld()->playSound(SOUND_GIVE_VORTEX);
		received = true;
	
	return;
}




void Player_Avatar::shootVortex() {
	vortexes--;
	getWorld()->addVortex(this);
	return;

}



void Player_Avatar::addCoins(int i) {
	received = true;
	m_coins += i;
	return;
}

void Player_Avatar::removeCoins(int i) {
	received = true;
	m_coins -= i;
	if (m_coins < 0) {
		m_coins = 0;
	}
	return;
}


int Player_Avatar::getCoins() const {
	return m_coins;
}

int Player_Avatar::getStars() const {
	return m_stars;
}


void Player_Avatar::addStar() {

	if (m_coins >= 20) {
		m_coins -= 20;
		m_stars++;
		getWorld()->playSound(SOUND_GIVE_STAR);
	}

	return;
}

int Player_Avatar::getDieRoll() const {
	return die_roll;
}

void Player_Avatar::setMovingDirection(int dir) {
	this->movingDirection = dir;
}