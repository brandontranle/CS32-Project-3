#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), bank(0)
{
    actors.clear();
    players.clear();
}

int StudentWorld::init()
{
    //determine board file
    Board game;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";

    //load board file
    Board::LoadResult result = game.loadBoard(board_file);

    //determines if successfully uploaded
    if (result == Board::load_fail_file_not_found) {
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format) {
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success)
        cerr << "Successfully loaded level" << endl;



    //iterate through the whole board 16 x 16
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {


            //switch statement determines what actors belong on the grid
            Board::GridEntry ge = game.getContentsOf(x, y);

            switch (ge) {
            case Board::empty:
                //cout << "Location " << x << "," << y << " is empty" << endl;
                break;

                //FIGURES
            case Board::boo:
                actors.push_back(new Boo(x, y, this));
                actors.push_back(new BlueCoinSquare(x, y, this));
               // cout << "Location " << x << "," << y << " has a Boo and a blue coin square\n";
                break;
            case Board::bowser:
                actors.push_back(new Bowser(x, y, this));
                actors.push_back(new BlueCoinSquare(x, y, this));
               // cout << "Location " << x << "," << y << " has a Bowser and a blue coin square\n";
                break;

            case Board::player:
                //cout << "Location " << x << "," << y << " has Peach & Yoshi and a blue coin square\n";
                players.push_back(new Peach(x, y, this));
                players.push_back(new Yoshi(x, y, this));
                actors.push_back(new BlueCoinSquare(x, y, this));

                break;

                //squares
            case Board::red_coin_square:
                //cout << "Location " << x << "," << y << " has a red coin square\n";
                actors.push_back(new RedCoinSquare(x, y, this));
                break;
            case Board::blue_coin_square:
               // cout << "Location " << x << "," << y << " has a blue coin square\n";
                actors.push_back(new BlueCoinSquare(x, y, this));
                break;
            case Board::up_dir_square:
               // cout << "Location " << x << "," << y << " has a up square\n";
                actors.push_back(new DirectionalSquare(x, y, this, 90));
                break;
            case Board::down_dir_square:
                //cout << "Location " << x << "," << y << " has a down direction square\n";
                actors.push_back(new DirectionalSquare(x, y, this, 270));

                break;
            case Board::left_dir_square:
                //cout << "Location " << x << "," << y << " has a left direction square\n";
                actors.push_back(new DirectionalSquare(x, y, this, 180));

                break;
            case Board::right_dir_square:
               // cout << "Location " << x << "," << y << " has a right direction square\n";
                actors.push_back(new DirectionalSquare(x, y, this, 0));

                break;
            case Board::event_square:
                //cout << "Location " << x << "," << y << " has a event square\n";
                actors.push_back(new EventSquare(x, y, this));

                break;
            case Board::bank_square:
                //cout << "Location " << x << "," << y << " has a bank square\n";
                actors.push_back(new BankSquare(x, y, this));

                break;
            case Board::star_square:
               // cout << "Location " << x << "," << y << " has a star square\n";
                actors.push_back(new StarSquare(x, y, this));
                break;
                // etc…
            } // end of switch



        } //end of inner for loop

    } // end of outer for loop

    startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;



} //end of iniit function

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.




    //iterate through each player in the vector
    for (auto it = players.begin(); it != players.end(); it++) {



        //calls doSomething for each player
        (*it)->doSomething();

    }


    //iterate through each actor in the vector
    for (auto it = actors.begin(); it != actors.end(); it++) {

        //if actor is alive, call doSomething
        if ((*it)->isAlive()) {
            (*it)->doSomething();
        }


    }


    //iterate through each actor in the vector
    for (auto it = actors.begin(); it != actors.end(); it++) {


        if (!((*it)->isAlive())) {
            //delete an actor if its dead
            delete (*it);
            actors.erase(it);
            //cerr << "deleted!" << endl;
            it = actors.begin(); //don't lose track!
        }
    }


    ostringstream oss;
    Player_Avatar* p1 = players.front();
    Player_Avatar* p2 = players.back();


    oss.fill('0');
    oss << "P1 Roll: ";
    oss << p1->getDieRoll();
    oss << " Stars: " << p1->getStars() << " ";
    oss << "$$: " << p1->getCoins();
        if (p1->getVortexes() > 0) {
            oss << " VOR";
        }

    oss << " |";
    oss << " Time: " << timeRemaining() << " |";
    oss << " Bank: " << bank << " |";
    oss << " P2 Roll: " << p2->getDieRoll() << " ";
    oss << "Stars: " << p2->getStars() << " ";
    oss << "$$: " << p2->getCoins();
    if (p2->getVortexes() > 0) {
        oss << " VOR";
    }

    setGameStatText(oss.str());



    if (timeRemaining() <= 0) { //CHOOES A WINNER!!
        playSound(SOUND_GAME_FINISHED);

        //STARS
        if (p1->getStars() > p2->getStars()) {
            setFinalScore(p1->getStars(), p1->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (p1->getStars() < p2->getStars()) {
            setFinalScore(p2->getStars(), p2->getCoins());
            return GWSTATUS_YOSHI_WON;
        }

        //COINS TIE BREAKER
        if (p1->getCoins() > p2->getCoins()) {
            setFinalScore(p1->getStars(), p1->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (p1->getCoins() < p2->getCoins()) {
            setFinalScore(p2->getStars(), p2->getCoins());
            return GWSTATUS_YOSHI_WON;
        }

        if (p1->getStars() == p2->getStars() && p1->getCoins() == p2->getCoins()) {
            int winner = randInt(1, 2);

            if (winner == 1)
                return GWSTATUS_PEACH_WON;
            else 
                return GWSTATUS_YOSHI_WON;

        }

    } //time remaining ends! A winner is chosen.

    //end of the game
    return GWSTATUS_CONTINUE_GAME;
}



void StudentWorld::cleanUp()
{

    for (Actor* a : actors)
        delete a;

    actors.clear();

    for (Player_Avatar* p : players)
        delete p;

    players.clear();


}

StudentWorld::~StudentWorld() {
    cleanUp();
}


//USEFUL FUNCTIONS

bool StudentWorld::overlappingPlayer1(Actor* a) {

    if (players.front()->getX() == a->getX() && players.front()->getY() == a->getY()) {
        return true;
    }
    return false;

}

bool StudentWorld::overlappingPlayer2(Actor* a) {

    if (players.back()->getX() == a->getX() && players.back()->getY() == a->getY()) {
        return true;
    }
    return false;


}

bool StudentWorld::isValidPosition(int dir, Actor* a) {

    int newX;
    int newY;


    //store updated x,y in variables if move in the direction
    a->getPositionInThisDirection(dir, 16, newX, newY);

    //create temp board of the current board
    string temp_board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    tempBoard.loadBoard(temp_board_file);


    //if the future position is not valid
    if (tempBoard.getContentsOf(newX / SPRITE_WIDTH, newY / SPRITE_HEIGHT) == Board::empty) {
        return false;
    }

    //if it is a valid position
    return true;

}

int StudentWorld::forkEncounter(Actor* a) {
    /*=================================
    right, up, left, and down, 
    0, 90, 180, and 270, respectively.
    ==================================*/

    //FORK 1 - STRAIGHT T FORK
    if (isValidPosition(270, a) && isValidPosition(0, a) && isValidPosition(180, a) && (!(isValidPosition(90, a)))) {
        return 1;
    }
    //FORK 2 ~ SIDEWAYS FORK - INVALID RIGHT
    if (isValidPosition(270, a) && isValidPosition(90, a) && isValidPosition(180, a) && (!(isValidPosition(0, a)))) {
        return 2;
    }

    //FORK 3 ~ SIDEWAYS FORK - INVALID LEFT
    if (isValidPosition(270, a) && isValidPosition(0, a) && isValidPosition(90, a) && (!(isValidPosition(180, a)))) {
        return 3;
    }

    //FORK 4 ~ UPSIDE-DOWN FORK
    if (isValidPosition(180, a) && isValidPosition(90, a) && isValidPosition(0, a) && (!(isValidPosition(270, a)))) {
        return 4;
    }

    return 0;
}


//BANK FUNCTIONALITIES
void StudentWorld::addToBank(int i) {
    bank += i;
}

void StudentWorld::resetBank() {
    bank = 0;
}


bool StudentWorld::onDirSquare(Actor* a) {

    //create temp board of the current board
    string temp_board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    tempBoard.loadBoard(temp_board_file);

    if (tempBoard.getContentsOf(a->getX() / SPRITE_WIDTH, a->getY() / SPRITE_HEIGHT) == Board::down_dir_square || tempBoard.getContentsOf(a->getX() / SPRITE_WIDTH, a->getY() / SPRITE_HEIGHT) == Board::up_dir_square ||
        tempBoard.getContentsOf(a->getX() / SPRITE_WIDTH, a->getY() / SPRITE_HEIGHT) == Board::left_dir_square || tempBoard.getContentsOf(a->getX() / SPRITE_WIDTH, a->getY() / SPRITE_HEIGHT) == Board::right_dir_square) {

        //std::cerr << "directional detection" << std::endl;
        return true;


    }

    return false;

}

bool StudentWorld::onEventSquare(Actor* a) {

    //create temp board of the current board
    string temp_board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    tempBoard.loadBoard(temp_board_file);

    if (tempBoard.getContentsOf(a->getX() / SPRITE_WIDTH, a->getY() / SPRITE_HEIGHT) == Board::event_square) {

        return true;


    }

    return false;

}


void StudentWorld::teleport(Actor* a) {

    string temp_board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    tempBoard.loadBoard(temp_board_file);

    int x, y;

    bool valid = false;
    while ((!(valid))) {
         x = randInt(0, 15);
         y = randInt(0, 15);

        if (tempBoard.getContentsOf(x, y) != Board::empty) { //CHANGE 
                a->moveTo(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                //std::cerr << x << y << std::endl;

                if (a == getPlayer1() || a == getPlayer2()) {
                    playSound(SOUND_PLAYER_TELEPORT);
                    if ((isValidPosition(270, a) && isValidPosition(90, a))) {
                        a->setGoingUp();
                    }
                    else if ((isValidPosition(0, a) && isValidPosition(180, a))) {
                        a->setGoingRight();
                    }
                }
                valid = true;
                break;
            }

        }
    return;

    }

void StudentWorld::swap(Actor* a, Actor* b) {
    //SWAP THE POSITION
    int tempX = a->getX();
    int tempY = a->getY();

    //SWAP THE DIRECTION
    int tempDirection = a->getDirection();
    //SWAP THE MOVING DIRECTION
    int tempMovingDirection = a->getMovingDirection();
    //SWAP THE WAITING STATUS
    bool tempStatus = a->getStatus();
    //SWAP THE TICKS TO MOVE
    int tempTicks = a->getTicks();
    int tempDieRoll = a->getDieRoll();



    a->moveTo(b->getX(), b->getY());
    b->moveTo(tempX, tempY);
    playSound(SOUND_PLAYER_TELEPORT);

    a->setTicks(b->getTicks());
    b->setTicks(tempTicks);

    a->setStatus(b->getStatus());
    b->setStatus(tempStatus);

    a->setDieRoll(b->getDieRoll());
    b->setDieRoll(tempDieRoll);

    a->setDirection(b->getDirection());
    b->setDirection(tempDirection);

    a->setMovingDirection(b->getMovingDirection());
    b->setMovingDirection(tempMovingDirection);

   

    return;

}


void StudentWorld::addVortex(Actor* a) {
    switch (a->getMovingDirection()) {
    case 90: //up
        actors.push_back(new Vortex(a->getX() / SPRITE_WIDTH, a->getY()/SPRITE_HEIGHT + 1, a->getMovingDirection(), this));
        break;
    case 270: //down
        actors.push_back(new Vortex(a->getX() / SPRITE_WIDTH, a->getY() / SPRITE_HEIGHT - 1, a->getMovingDirection(), this));
        break;
    case 0: //right
        actors.push_back(new Vortex(a->getX() / SPRITE_WIDTH + 1, a->getY() / SPRITE_HEIGHT, a->getMovingDirection(), this));
        break;
    case 180: 
        actors.push_back(new Vortex(a->getX() / SPRITE_WIDTH - 1, a->getY() / SPRITE_HEIGHT, a->getMovingDirection(), this));
        break;
    }

    return;
}

void StudentWorld::addDroppingSquare(int x, int y, Actor *a) {
    
    //cout << actors.size() << endl;
    

    if (getSquare(a)->is_a_square()) {
        getSquare(a)->remove();
    }  

   // cerr << "dropping square added" << endl;

    actors.push_back(new DroppingSquare(x, y, this));
  //  cout << actors.size() << endl;


    playSound(SOUND_DROPPING_SQUARE_CREATED);
    return;

}


Actor* StudentWorld::getSquare(Actor* a) {

    for (auto i = actors.begin(); i != actors.end(); i++) {
        if ((*i) != a)
            if (a->getX() == (*i)->getX() && a->getY() == (*i)->getY())
                return (*i);
    }

    return a;

}


void StudentWorld::swapStats(Actor* a, Actor* b) {
    int tempCoins = a->getCoins();
    int tempStars = a->getStars();

    int choice = randInt(1, 2);

    if (choice == 1) {
        a->setCoins(b->getCoins());
        b->setCoins(tempCoins);
    }
    else if (choice == 2) {
        a->setStars(b->getStars());
        b->setStars(tempStars);
    }
    return;

}


bool StudentWorld::overlappingObject(Actor* a) {

    for (auto b = actors.begin(); b != actors.end(); b++) {
        if ((*b) != a)
            if ((*b)->isAlive() && (*b)->can_be_hit_by_vortex() && overlapping(a, *b)) {
                //cerr << "Shot!" << endl;
                (*b)->setImpactStatus(true);
                return true;
            }
    }
    return false;

}

bool StudentWorld::overlapping(Actor* a, Actor* b) {
    return overlappingPixel(a->getX(), a->getY(), b->getX(), b->getY());
}

bool StudentWorld::overlappingPixel(int x1, int y1, int x2, int y2) { 
    if (((x1 + SPRITE_WIDTH) > x2) && (x1 < (x2 + SPRITE_WIDTH)))
        if (((y1 + SPRITE_HEIGHT) > y2) && (y1 < (y2 + SPRITE_HEIGHT)))
            return true;
    return false;
}



bool StudentWorld::overlappingTwoObjects(Actor* a) {

    int count = 0;
    for (auto b = actors.begin(); b != actors.end(); b++) {
        if ((*b) != a) {
            if (((*b)->isAlive() && (*b)->can_be_hit_by_vortex()) || (*b)->is_a_dropping()) {
                if (checkSquares(a, *b)) {
                    count++;
                }
            }
        }
    }
    if (count >= 2)
        return true;

    return false;

}



bool StudentWorld::checkSquares(Actor* a, Actor* b) {
    if (a->getX() == b->getX() && a->getY() == b->getY()) {
        return true;
    }

    return false;
}



