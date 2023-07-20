#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


int Player_Avatar::getDieRoll() const{
	return this->die_roll;
}

int Player_Avatar::getCoins() const {
	return this->m_Coins;
}

int Player_Avatar::getStars() const{
	return this->m_Stars;
}

void Player_Avatar::addCoins() {
	this->m_Coins+=3;
	this->received = true;
	getWorld()->playSound(SOUND_GIVE_COIN);
}

void Player_Avatar::addStar() {
	if (this->m_Coins >= 20) {
		this->m_Stars += 1;
		this->m_Coins -= 20;
		getWorld()->playSound(SOUND_GIVE_STAR);
	}
}

void Player_Avatar::deductCoins() {
	this->m_Coins -= 3;
	this->received = true;
	getWorld()->playSound(SOUND_TAKE_COIN);
}

bool Player_Avatar::isWalking() const{
	return this->waiting;
}


void Player_Avatar::doSomething()
{
	

	if (waiting) { //waiting state
		switch (getWorld()->getAction(m_playerNum))
		{
		case ACTION_ROLL:
			if (this->die_roll == 0) {
				this->die_roll = randInt(1, 10); //roll the dice
				ticks_to_move = this->die_roll * 8;
				this->waiting = false;
				this->received = false;
				std::cerr << "rolled!" << std::endl;
			}
			break;

		case ACTION_NONE:
			// no key relevant to this player was hit ...
			break;
		case ACTION_LEFT:
			// change player's direction to left ...
			if (die_roll > 0 && canTurnL) {
				setDirection(left);
				canTurnL = false;
				movingLeft = true;
				waiting = false;
				needToTurn = false;
			}
			break;
		case ACTION_RIGHT:
			if (die_roll > 0 && canTurnR) {
				setDirection(right);
				canTurnR = false;
				movingRight = true;
				waiting = false;
				needToTurn = false;
			}
			break;
		case ACTION_UP:
			if (die_roll > 0 && canTurnU) {
				setDirection(right);
				canTurnU = false;
				movingUp = true;
				waiting = false;
				needToTurn = false;
			}
			break;
		case ACTION_DOWN:
			if (die_roll > 0 && canTurnD) {
				setDirection(right);
				canTurnD = false;
				movingDown = true;
				waiting = false;
				needToTurn = false;
			}
			break;
		case ACTION_FIRE:
			// add Vortex object in front of player...;
			break;
		default: //else if anythinge lse (because the user didn't press a key or pressed any other key):
			return; //return immediately 
		}
	}
	
	int p_x1 = getX();
	int p_y1 = getY();

	int front = (p_x1 + (1 * SPRITE_WIDTH));
	int behind = (p_x1 - (1 * SPRITE_WIDTH));
	int above = (p_y1 + (1 * SPRITE_HEIGHT));
	int below = (p_y1 - (1 * SPRITE_HEIGHT));


	if (!waiting) { //walking state
		
		//getWorld()->overlappingDirectionFirst(this);

		if (forcedTurn) {
			//std::cout << "Forcing" << std::endl;
			forcedTurn = false;
			needToTurn = false;

			
		}


		if (this->needToTurn == true) {
			//std::cout << "turn?" << std::endl;
			waiting = true;
			forcedTurn = false;
			movingDown = false;
			movingUp = false;
			movingRight = false;
			movingLeft = false;
			return;
		}


		if (movingRight) {
			//std::cerr << "moving right" << std::endl;
			this->setDirection(right);
			this->moveTo(getX() + 4, getY());
			ticks_to_move--;
			ticks_to_move--;


		}

		if (movingDown) {
			this->setDirection(right);
			this->moveAtAngle(down, 4);
			ticks_to_move--;
			ticks_to_move--;

		}

		if (movingUp) {
			this->setDirection(right);
			this->moveAtAngle(up, 4);
			ticks_to_move--;
			ticks_to_move--;


		}

		if (movingLeft) {
			this->setDirection(left);
			this->moveAtAngle(left, 4);
			ticks_to_move--;
			ticks_to_move--;


		}


			if (ticks_to_move % 8 == 0)
				this->die_roll--;

			if (ticks_to_move == 0 || die_roll == 0) {
				this->waiting = true;
				return;
			}

		
	}

	
	return;
}

//MUST HAVE THE SAME doSomething() functionality as Player_Avatar.
void Yoshi::doSomething(){
	Player_Avatar::doSomething();
}

void Peach::doSomething(){
	Player_Avatar::doSomething();
}



void Monster::doSomething() {
	//every couple ticks do something

}

void Square::remove() {
//
}

void BlueCoinSquare::remove() {
	Square::remove();
	//change to dropping square

}

void Square::doSomething() {
	//check for turns for ALL squares

	int p_x1 = getWorld()->getFirstPlayer()->getX();
	int p_y1 = getWorld()->getFirstPlayer()->getY();

	int p_x2 = getWorld()->getSecondPlayer()->getX();
	int p_y2 = getWorld()->getSecondPlayer()->getY();

	int front = (p_x1 + (1*SPRITE_WIDTH));
	int behind = (p_x1 - (1*SPRITE_WIDTH));
	int above = (p_y1 + (1 * SPRITE_HEIGHT));
	int below = (p_y1 - (1*SPRITE_HEIGHT));


	int front2 = (p_x2 + (1 * SPRITE_WIDTH));
	int behind2 = (p_x2 - (1 * SPRITE_WIDTH));
	int above2 = (p_y2 + (1 * SPRITE_HEIGHT));
	int below2 = (p_y2 - (1 * SPRITE_HEIGHT));
	
	


	
	if (getWorld()->getFirstPlayer()->getDieRoll() > 0 || (getWorld()->getFirstPlayer()->isWalking())) { //FIRST PLAYER TURNING SYSTEM
			/*===============================================
			SCENARIOS WITH A SQUARE || ACTOR BELOW THE PLAYER
			=================================================*/
		
		if (getWorld()->detectAheadFirstPlayer(p_x1, below)) { //START OF IF THERS SOMETHING BELOW SCENARIOS
		
			/*================
					   |
				-------|
					   |

			================*/
			//SIDE-WAYS FORK 1
			
				if (getWorld()->detectAheadFirstPlayer(p_x1, above) && (!(getWorld()->detectAheadFirstPlayer(front, p_y1))) && getWorld()->detectAheadFirstPlayer(behind, p_y1) && getWorld()->detectAheadFirstPlayer(p_x1, below)) {//if there isnt anything ahead
					//std::cerr << "sideways fork 1" << std::endl;
					if (getWorld()->getFirstPlayer()->isMovingRight()) {
						getWorld()->getFirstPlayer()->canTurnRight(false);
						getWorld()->getFirstPlayer()->canTurnLeft(false);
						getWorld()->getFirstPlayer()->canTurnUp(true);
						getWorld()->getFirstPlayer()->canTurnDown(true);
					}

					if (getWorld()->getFirstPlayer()->isMovingUp()) {
						getWorld()->getFirstPlayer()->canTurnRight(false);
						getWorld()->getFirstPlayer()->canTurnLeft(true);
						getWorld()->getFirstPlayer()->canTurnUp(true);
						getWorld()->getFirstPlayer()->canTurnDown(false);
					}
					if (getWorld()->getFirstPlayer()->isMovingDown()) {
						getWorld()->getFirstPlayer()->canTurnRight(false);
						getWorld()->getFirstPlayer()->canTurnLeft(true);
						getWorld()->getFirstPlayer()->canTurnUp(false);
						getWorld()->getFirstPlayer()->canTurnDown(true);
					}

					getWorld()->getFirstPlayer()->needsToTurn(true);

				} //END
			

			/*==================
			  90 DEGREE ANGLE C
				_______________
				| 
				|
				|
			====================*/
			//IF THE PLAYER IS RISING TO A 90 DEGREE ANGLE C
			//if theres something below the player, and there is nothing behind it, but theres something in front of it
			if (getWorld()->detectAheadFirstPlayer(front, p_y1) && (!(getWorld()->detectAheadFirstPlayer(behind, p_y1))) && (!(getWorld()->detectAheadFirstPlayer(p_x1, above))) && getWorld()->detectAheadFirstPlayer(p_x1,below)) { //BEGIN OF 90 DEGREE ANGLE C SCENARIO
				if (getWorld()->getFirstPlayer()->isMovingUp()) {
					//std::cerr << "rising here!" << std::endl;
					getWorld()->getFirstPlayer()->forceRight(true);
				}

			//IF THE PLAYER IS DESCENDING A 90 DEGREE ANGLE C
				if (getWorld()->getFirstPlayer()->isMovingLeft()) {
					//std::cerr << "desension started" << std::endl;
					getWorld()->getFirstPlayer()->forceDown(true);
				}

			} //end of 90 DEGREE ANGLE C SCENARIO  
				
			

			/*====================
			  90 DEGREE ANGLE D
			    ___________			
					      |
				          |
					      |
			======================*/

			if ((!(getWorld()->detectAheadFirstPlayer(front, p_y1))) && getWorld()->detectAheadFirstPlayer(behind, p_y1) && (!(getWorld()->detectAheadFirstPlayer(p_x1, above))) && getWorld()->detectAheadFirstPlayer(p_x1, below)) { //START OF 90 DGEREE ANGLE D SCENARIO
				if (getWorld()->getFirstPlayer()->isMovingUp()) {
					//std::cerr << "moving left from an angle D" << std::endl;
					getWorld()->getFirstPlayer()->forceLeft(true);
				}

				if (getWorld()->getFirstPlayer()->isMovingRight()) {
					//std::cerr << "moving down from an angle D" << std::endl;
					getWorld()->getFirstPlayer()->forceDown(true);
				}
			} //END OF 90 DEGREE ANGLE D SCENARIO */ //DELETE

		/*===========================
			UPWARD T - SHAPED FORK
				  _______
				     |
			         |
			         |
		  ==========================*/
			
			//force the player to choose right or down or left based on how they're moving
			if (getWorld()->detectAheadFirstPlayer(front, p_y1) && getWorld()->detectAheadFirstPlayer(behind, p_y1) && getWorld()->detectAheadFirstPlayer(p_x1, below) && (!(getWorld()->detectAheadFirstPlayer(p_x1, above)))) { //UPWARD FORK SCENARIO BEGIN
				if (getWorld()->getFirstPlayer()->isMovingLeft()) {
					getWorld()->getFirstPlayer()->canTurnRight(false);
					getWorld()->getFirstPlayer()->canTurnLeft(true);
					getWorld()->getFirstPlayer()->canTurnUp(false);
					getWorld()->getFirstPlayer()->canTurnDown(true);
				}

				if (getWorld()->getFirstPlayer()->isMovingRight()) {
					getWorld()->getFirstPlayer()->canTurnRight(true);
					getWorld()->getFirstPlayer()->canTurnLeft(false);
					getWorld()->getFirstPlayer()->canTurnUp(false);
					getWorld()->getFirstPlayer()->canTurnDown(true);
				}

				if (getWorld()->getFirstPlayer()->isMovingUp()) {
					getWorld()->getFirstPlayer()->canTurnRight(true);
					getWorld()->getFirstPlayer()->canTurnLeft(true);
					getWorld()->getFirstPlayer()->canTurnUp(false);
					getWorld()->getFirstPlayer()->canTurnDown(false);
				}
				//FORCE THE PLAYER TO TAKE A TURN ~ waiting state
				getWorld()->getFirstPlayer()->needsToTurn(true);
			}//END OF UPWARD FORK SCENARIO
		} //END OF SCENARIOS WHEN THERS AN ACTOR BELOW THE PLAYER*/ //DELETE


			/*===============================================
			SCENARIOS WITH A SQUARE || ACTOR ABOVE THE PLAYER
			=================================================*/
		
		if (getWorld()->detectAheadFirstPlayer(p_x1, above)) { //if theres something above the player
			if (!(getWorld()->detectAheadFirstPlayer(p_x1, below))) { //if theres NOTHING below the player
				/*===================
				 90 DEGREE ANGLE A
					|
					|
					|______
				===================*/
				if (getWorld()->detectAheadFirstPlayer(front, p_y1) && (!(getWorld()->detectAheadFirstPlayer(behind, p_y1)))) { //START OF 90 DEGREE ANGLE A
					//std::cerr << "forcing up" << std::endl;
					if (getWorld()->getFirstPlayer()->isMovingLeft()) {
						getWorld()->getFirstPlayer()->forceUp(true);
					}

					//std::cerr << "forcing right " << std::endl;
					if (getWorld()->getFirstPlayer()->isMovingDown()) {
						getWorld()->getFirstPlayer()->forceRight(true);
					}
				
				} //end of if theres something in front and nothing behind while theres nothing below, and something above (90 degree angle A) */ //DELETE

				
				/*==================
				 90 DEGREE ANGLE B
					     |
						 |
					_____|
				===================*/
				if (!(getWorld()->detectAheadFirstPlayer(front, p_y1)) && (getWorld()->detectAheadFirstPlayer(behind, p_y1))) { //START OF 90 DEGREE ANGLE B
				//	std::cerr << "forcing up" << std::endl;
					if (getWorld()->getFirstPlayer()->isMovingRight()) {
						//std::cerr << "forcing up line 394" << std::endl;
						getWorld()->getFirstPlayer()->forceUp(true);
					}

					if (getWorld()->getFirstPlayer()->isMovingDown()) {
						//std::cerr << "forcing left line 399" << std::endl;
						getWorld()->getFirstPlayer()->forceLeft(true);
					}
				}//end of if theres nothing in front and something behind, while there is something above and nothing below (90 degree angle B)
			} //end if theres nothing below the player while something above the player */ //DELETE

			/*==================
			   SIDEWAYS FORK 2
				|
				|----------
				|
			==================*/
			//if theres something below the player and theres something in front (right) of the player while theres something above it (SIDEWAYS FORK 2)
			if ((getWorld()->detectAheadFirstPlayer(front, p_y1)) && !(getWorld()->detectAheadFirstPlayer(behind, p_y1)) && (getWorld()->detectAheadFirstPlayer(p_x1, above)) && (getWorld()->detectAheadFirstPlayer(p_x1, below))) {//START OF SIDEWAYS FORK 2
				//std::cerr << "sideways fork 2" << std::endl;
				if (getWorld()->getFirstPlayer()->isMovingUp()) {
					getWorld()->getFirstPlayer()->canTurnRight(true);
					getWorld()->getFirstPlayer()->canTurnLeft(false);
					getWorld()->getFirstPlayer()->canTurnUp(true);
					getWorld()->getFirstPlayer()->canTurnDown(false);
				}

				if (getWorld()->getFirstPlayer()->isMovingDown()) {
					getWorld()->getFirstPlayer()->canTurnRight(true);
					getWorld()->getFirstPlayer()->canTurnLeft(false);
					getWorld()->getFirstPlayer()->canTurnUp(false);
					getWorld()->getFirstPlayer()->canTurnDown(true);
				}

				//if player is moving left, "front" is logically defined as behind of it 
				if (getWorld()->getFirstPlayer()->isMovingLeft()) {
					getWorld()->getFirstPlayer()->canTurnRight(false);
					getWorld()->getFirstPlayer()->canTurnLeft(false);
					getWorld()->getFirstPlayer()->canTurnUp(true);
					getWorld()->getFirstPlayer()->canTurnDown(true);

				}

					getWorld()->getFirstPlayer()->needsToTurn(true);
			} //END OF SIDEWAYS FORK 2 SCENARIO 



			/*===============
			UPSIDE DOWN FORK
				  |
				  |
				__|__
			 ==============*/
			//if theres a square in front, behind, and above the player while theres nothing below (UPSIDE-DOWN FORK)
			if (getWorld()->detectAheadFirstPlayer(front, p_y1) && getWorld()->detectAheadFirstPlayer(behind, p_y1)&& getWorld()->detectAheadFirstPlayer(p_x1, above) && (!(getWorld()->detectAheadFirstPlayer(p_x1,below)))) { //START OF UPSIDE DOWN FORK SCENARIO

				if (getWorld()->getFirstPlayer()->isMovingDown()) {
					getWorld()->getFirstPlayer()->canTurnRight(true);
					getWorld()->getFirstPlayer()->canTurnLeft(true);
					getWorld()->getFirstPlayer()->canTurnUp(false);
					getWorld()->getFirstPlayer()->canTurnDown(false);
				}

				if (getWorld()->getFirstPlayer()->isMovingRight()) {
					getWorld()->getFirstPlayer()->canTurnRight(true);
					getWorld()->getFirstPlayer()->canTurnLeft(false);
					getWorld()->getFirstPlayer()->canTurnUp(true);
					getWorld()->getFirstPlayer()->canTurnDown(false);
				}

				if (getWorld()->getFirstPlayer()->isMovingLeft()) {
					getWorld()->getFirstPlayer()->canTurnRight(false);
					getWorld()->getFirstPlayer()->canTurnLeft(true);
					getWorld()->getFirstPlayer()->canTurnUp(true);
					getWorld()->getFirstPlayer()->canTurnDown(false);
				}

				//std::cerr << "upside-down fork" << std::endl;
				getWorld()->getFirstPlayer()->needsToTurn(true);
			} //END OF UPSIDE-DOWN FORK SCENARIO

		} //end of scenarios if theres something above the player 
	} //END OF ALL TURN SCENARIOS 
	//=====================================================================





	if (getWorld()->getSecondPlayer()->getDieRoll() > 0 || getWorld()->getSecondPlayer()->isWalking()) { //SECOND PLAYER TURNING SYSTEM
		/*===============================================
		SCENARIOS WITH A SQUARE || ACTOR BELOW THE PLAYER
		=================================================*/

		if (getWorld()->detectAheadSecondPlayer(p_x2, below2)) { //START OF IF THERS SOMETHING BELOW SCENARIOS

			/*================
					   |
				-------|
					   |

			================*/
			//SIDE-WAYS FORK 1

			if (getWorld()->detectAheadSecondPlayer(p_x2, above2) && (!(getWorld()->detectAheadSecondPlayer(front2, p_y2))) && getWorld()->detectAheadSecondPlayer(behind2, p_y2) && getWorld()->detectAheadSecondPlayer(p_x2, below2)) {//if there isnt anything ahead
				//std::cerr << "sideways fork 1" << std::endl;
				if (getWorld()->getSecondPlayer()->isMovingRight()) {
					getWorld()->getSecondPlayer()->canTurnRight(false);
					getWorld()->getSecondPlayer()->canTurnLeft(false);
					getWorld()->getSecondPlayer()->canTurnUp(true);
					getWorld()->getSecondPlayer()->canTurnDown(true);
				}

				if (getWorld()->getSecondPlayer()->isMovingUp()) {
					getWorld()->getSecondPlayer()->canTurnRight(false);
					getWorld()->getSecondPlayer()->canTurnLeft(true);
					getWorld()->getSecondPlayer()->canTurnUp(true);
					getWorld()->getSecondPlayer()->canTurnDown(false);
				}
				if (getWorld()->getSecondPlayer()->isMovingDown()) {
					getWorld()->getSecondPlayer()->canTurnRight(false);
					getWorld()->getSecondPlayer()->canTurnLeft(true);
					getWorld()->getSecondPlayer()->canTurnUp(false);
					getWorld()->getSecondPlayer()->canTurnDown(true);
				}

				getWorld()->getSecondPlayer()->needsToTurn(true);
				return;

			}


			/*==================
			  90 DEGREE ANGLE C
				_______________
				|
				|
				|
			====================*/
			//IF THE PLAYER IS RISING TO A 90 DEGREE ANGLE C
			//if theres something below the player, and there is nothing behind it, but theres something in front of it
			if (getWorld()->detectAheadSecondPlayer(front2, p_y2) && (!(getWorld()->detectAheadSecondPlayer(behind2, p_y2))) && (!(getWorld()->detectAheadSecondPlayer(p_x2, above2))) && getWorld()->detectAheadSecondPlayer(p_x2, below2)) { //BEGIN OF 90 DEGREE ANGLE C SCENARIO
				if (getWorld()->getSecondPlayer()->isMovingUp()) {
					//std::cerr << "rising here!" << std::endl;
					getWorld()->getSecondPlayer()->forceRight(true);
					return;
				}

				//IF THE PLAYER IS DESCENDING A 90 DEGREE ANGLE C
				if (getWorld()->getSecondPlayer()->isMovingLeft()) {
					//std::cerr << "desension started" << std::endl;
					getWorld()->getSecondPlayer()->forceDown(true);
					return;
				}

			} //end of 90 DEGREE ANGLE C SCENARIO



			/*====================
			  90 DEGREE ANGLE D
				___________
						  |
						  |
						  |
			======================*/

			if ((!(getWorld()->detectAheadSecondPlayer(front2, p_y2))) && getWorld()->detectAheadSecondPlayer(behind2, p_y2) && (!(getWorld()->detectAheadSecondPlayer(p_x2, above2))) && getWorld()->detectAheadSecondPlayer(p_x2, below2)) { //START OF 90 DGEREE ANGLE D SCENARIO
				if (getWorld()->getSecondPlayer()->isMovingUp()) {
					//std::cerr << "moving left from an angle D" << std::endl;
					getWorld()->getSecondPlayer()->forceLeft(true);
					return;
				}

				if (getWorld()->getSecondPlayer()->isMovingRight()) {
					//std::cerr << "moving down from an angle D" << std::endl;
					getWorld()->getSecondPlayer()->forceDown(true);
					return;
				}
			} //END OF 90 DEGREE ANGLE D SCENARIO 

		/*===========================
			UPWARD T - SHAPED FORK
				  _______
					 |
					 |
					 |
		  ==========================*/

		  //force the player to choose right or down or left based on how they're moving
			if (getWorld()->detectAheadSecondPlayer(front2, p_y2) && getWorld()->detectAheadSecondPlayer(behind2, p_y2) && getWorld()->detectAheadSecondPlayer(p_x2, below2) && (!(getWorld()->detectAheadSecondPlayer(p_x2, above2)))) { //UPWARD FORK SCENARIO BEGIN
				if (getWorld()->getSecondPlayer()->isMovingLeft()) {
					getWorld()->getSecondPlayer()->canTurnRight(false);
					getWorld()->getSecondPlayer()->canTurnLeft(true);
					getWorld()->getSecondPlayer()->canTurnUp(false);
					getWorld()->getSecondPlayer()->canTurnDown(true);
				}

				if (getWorld()->getSecondPlayer()->isMovingRight()) {
					getWorld()->getSecondPlayer()->canTurnRight(true);
					getWorld()->getSecondPlayer()->canTurnLeft(false);
					getWorld()->getSecondPlayer()->canTurnUp(false);
					getWorld()->getSecondPlayer()->canTurnDown(true);
				}

				if (getWorld()->getSecondPlayer()->isMovingUp()) {
					getWorld()->getSecondPlayer()->canTurnRight(true);
					getWorld()->getSecondPlayer()->canTurnLeft(true);
					getWorld()->getSecondPlayer()->canTurnUp(false);
					getWorld()->getSecondPlayer()->canTurnDown(false);
				}
				//FORCE THE PLAYER TO TAKE A TURN ~ waiting state
				getWorld()->getSecondPlayer()->needsToTurn(true);
				return;
			}//END OF UPWARD FORK SCENARIO
		} //END OF SCENARIOS WHEN THERS AN ACTOR BELOW THE PLAYER


			/*===============================================
			SCENARIOS WITH A SQUARE || ACTOR ABOVE THE PLAYER
			=================================================*/

		if (getWorld()->detectAheadSecondPlayer(p_x2, above2)) { //if theres something above the player
			if (!(getWorld()->detectAheadSecondPlayer(p_x2, below2))) { //if theres NOTHING below the player
				/*===================
				 90 DEGREE ANGLE A
					|
					|
					|______
				===================*/
				if (getWorld()->detectAheadSecondPlayer(front2, p_y2) && (!(getWorld()->detectAheadSecondPlayer(behind2, p_y2)))) { //START OF 90 DEGREE ANGLE A
					//std::cerr << "forcing up" << std::endl;
					if (getWorld()->getSecondPlayer()->isMovingLeft()) {
						getWorld()->getSecondPlayer()->forceUp(true);
						return;
					}

					//std::cerr << "forcing right " << std::endl;
					if (getWorld()->getSecondPlayer()->isMovingDown()) {
						getWorld()->getSecondPlayer()->forceRight(true);
						return;
					}

				} //end of if theres something in front and nothing behind while theres nothing below, and something above (90 degree angle A)


				/*==================
				 90 DEGREE ANGLE B
						 |
						 |
					_____|
				===================*/
				if (!(getWorld()->detectAheadSecondPlayer(front2, p_y2)) && (getWorld()->detectAheadSecondPlayer(behind2, p_y2))) { //START OF 90 DEGREE ANGLE B
					//	std::cerr << "forcing up" << std::endl;
					if (getWorld()->getSecondPlayer()->isMovingRight()) {
						//std::cerr << "forcing up line 394" << std::endl;
						getWorld()->getSecondPlayer()->forceUp(true);
						return;
					}

					if (getWorld()->getSecondPlayer()->isMovingDown()) {
						//std::cerr << "forcing left line 399" << std::endl;
						getWorld()->getSecondPlayer()->forceLeft(true);
						return;
					}
				}//end of if theres nothing in front and something behind, while there is something above and nothing below (90 degree angle B)
			} //end if theres nothing below the player while something above the player

			/*==================
			   SIDEWAYS FORK 2
				|
				|----------
				|
			==================*/
			//if theres something below the player and theres something in front (right) of the player while theres something above it (SIDEWAYS FORK 2)
			if ((getWorld()->detectAheadSecondPlayer(front2, p_y2)) && !(getWorld()->detectAheadSecondPlayer(behind2, p_y2)) && (getWorld()->detectAheadSecondPlayer(p_x2, above2)) && (getWorld()->detectAheadSecondPlayer(p_x2, below2))) {//START OF SIDEWAYS FORK 2
				//std::cerr << "sideways fork 2" << std::endl;
				if (getWorld()->getSecondPlayer()->isMovingUp()) {
					getWorld()->getSecondPlayer()->canTurnRight(true);
					getWorld()->getSecondPlayer()->canTurnLeft(false);
					getWorld()->getSecondPlayer()->canTurnUp(true);
					getWorld()->getSecondPlayer()->canTurnDown(false);
				}

				if (getWorld()->getSecondPlayer()->isMovingDown()) {
					getWorld()->getSecondPlayer()->canTurnRight(true);
					getWorld()->getSecondPlayer()->canTurnLeft(false);
					getWorld()->getSecondPlayer()->canTurnUp(false);
					getWorld()->getSecondPlayer()->canTurnDown(true);
				}

				//if player is moving left, "front" is logically defined as behind of it 
				if (getWorld()->getSecondPlayer()->isMovingLeft()) {
					getWorld()->getSecondPlayer()->canTurnRight(false);
					getWorld()->getSecondPlayer()->canTurnLeft(false);
					getWorld()->getSecondPlayer()->canTurnUp(true);
					getWorld()->getSecondPlayer()->canTurnDown(true);

				}

				getWorld()->getSecondPlayer()->needsToTurn(true);
				return;
			} //END OF SIDEWAYS FORK 2 SCENARIO



			/*===============
			UPSIDE DOWN FORK
				  |
				  |
				__|__
			 ==============*/
			 //if theres a square in front, behind, and above the player while theres nothing below (UPSIDE-DOWN FORK)
			if (getWorld()->detectAheadSecondPlayer(front2, p_y2) && getWorld()->detectAheadSecondPlayer(behind2, p_y2) && getWorld()->detectAheadSecondPlayer(p_x2, above2) && (!(getWorld()->detectAheadSecondPlayer(p_x2, below2)))) { //START OF UPSIDE DOWN FORK SCENARIO

				if (getWorld()->getSecondPlayer()->isMovingDown()) {
					getWorld()->getSecondPlayer()->canTurnRight(true);
					getWorld()->getSecondPlayer()->canTurnLeft(true);
					getWorld()->getSecondPlayer()->canTurnUp(false);
					getWorld()->getSecondPlayer()->canTurnDown(false);
				}

				if (getWorld()->getSecondPlayer()->isMovingRight()) {
					getWorld()->getFirstPlayer()->canTurnRight(true);
					getWorld()->getFirstPlayer()->canTurnLeft(false);
					getWorld()->getFirstPlayer()->canTurnUp(true);
					getWorld()->getFirstPlayer()->canTurnDown(false);
				}

				if (getWorld()->getSecondPlayer()->isMovingLeft()) {
					getWorld()->getSecondPlayer()->canTurnRight(false);
					getWorld()->getSecondPlayer()->canTurnLeft(true);
					getWorld()->getSecondPlayer()->canTurnUp(true);
					getWorld()->getSecondPlayer()->canTurnDown(false);
				}

				//std::cerr << "upside-down fork" << std::endl;
				getWorld()->getSecondPlayer()->needsToTurn(true);
				return;
			} //END OF UPSIDE-DOWN FORK SCENARIO

		} //end of scenarios if theres something above the player



	} //END OF ALL SCENARIOS FOR PLAYER 2






	return;

} //END OF SQUARE::DOSOMETHING()

void DirectionalSquare::doSomething() {
	/*========================================================================*/
	//CHECK IF THE FIRST PLAYER IS BEING FORCED TO TURN BY A DIRECTIONAL ARROW
	/*========================================================================*/

	if (getWorld()->overlappingFirstPlayer(this)) {
		getWorld()->getFirstPlayer()->forceTurn(true);

			int de = this->getDirection();

			switch (de) {
			case right: 
				getWorld()->getFirstPlayer()->forceRight(true);
				break;
			case up: 
				getWorld()->getFirstPlayer()->forceUp(true);
			
				break;
			case left: 
				getWorld()->getFirstPlayer()->forceLeft(true);
				break;
			case down:
				getWorld()->getFirstPlayer()->forceDown(true);
				break;
			default: break;
			}
			

			return;
		}

	/*========================================================================*/
	//CHECK IF THE SECOND PLAYER IS BEING FORCED TO TURN BY A DIRECTIONAL ARROW
	/*========================================================================*/

	if (getWorld()->overlappingSecondPlayer(this)) {
		getWorld()->getSecondPlayer()->forceTurn(true);

		int de = this->getDirection();

		switch (de) {
		case 0:
			getWorld()->getSecondPlayer()->forceRight(true);
			
				break;
		case 90:
			getWorld()->getSecondPlayer()->forceUp(true);
			
			break;
		case 180:
			getWorld()->getSecondPlayer()->forceLeft(true);
			
			break;
		case 270:
			getWorld()->getSecondPlayer()->forceDown(true);
			

			break;
		default: break;
		}
		return;
	}

	return;
}

void BlueCoinSquare::doSomething() {
	Square::doSomething();

	//if the player has not received an award an their roll has ended.
	//add three coins to the overlapping player's turn.

	if (getWorld()->overlappingFirstPlayer(this))
		if (!(getWorld()->getFirstPlayer()->receivedAward()) && getWorld()->getFirstPlayer()->getDieRoll() == 0) 
			getWorld()->getFirstPlayer()->addCoins(); 

	if (getWorld()->overlappingSecondPlayer(this))
		if (!(getWorld()->getSecondPlayer()->receivedAward()) && getWorld()->getSecondPlayer()->getDieRoll() == 0)
			getWorld()->getSecondPlayer()->addCoins();

	

}

void EventSquare::doSomething() {
	Square::doSomething();

}

void RedCoinSquare::doSomething() {

	if (getWorld()->overlappingFirstPlayer(this))
		if (!(getWorld()->getFirstPlayer()->receivedAward()) && getWorld()->getFirstPlayer()->getDieRoll() == 0)
			getWorld()->getFirstPlayer()->deductCoins();

	if (getWorld()->overlappingSecondPlayer(this))
		if (!(getWorld()->getSecondPlayer()->receivedAward()) && getWorld()->getSecondPlayer()->getDieRoll() == 0)
			getWorld()->getSecondPlayer()->deductCoins();
}

void StarSquare::doSomething() {
	Square::doSomething();
	if (getWorld()->overlappingFirstPlayer(this))
			getWorld()->getFirstPlayer()->addStar();

	if (getWorld()->overlappingSecondPlayer(this))
			getWorld()->getSecondPlayer()->addStar();


}

void BankSquare::doSomething() {
	Square::doSomething();


}



