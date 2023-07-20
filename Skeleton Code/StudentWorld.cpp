#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Actor.h"
#include "Board.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath): GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    /*Your StudentWorld’s init() member function must :
    1. Initialize the data structures used to keep track of your game’s world.
        2. Allocate and insert Peach and Yoshi objects into the game world.Every time a
        game begins, Peachand Yoshi start out initialized in their starting location as
        specified by the current board data file.
        3. Allocate and insert all of the other objects(e.g., squares, baddies, etc.) into the
        game world as described below.
        4. Start the countdown timer for the 99 - second game*/
    

    //add the world
    Board game;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = game.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found) {
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format) {
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success)
        cerr << "Successfully loaded level" << endl;


    //add the characters
    for (int x = 0; x < 16; x++) { //x axis
        for (int y = 0; y < 16; y++) { //y axis

            Board::GridEntry ge = game.getContentsOf(x, y); // x=5, y=10
            switch (ge) {
            case Board::empty:
                cout << "Location "<<x<<","<<y<<" is empty\n";
                break;
                //===============
                //FIGURES
            case Board::boo:
                actors.push_back(new Boo(x, y, this));
                cout << "Location "<<x<<","<<y<<" has a Boo and a blue coin square\n";
                actors.push_back(new BlueCoinSquare(x, y, this));
                break;
            case Board::bowser:
                    actors.push_back(new Bowser(x, y, this));   
                    cout << "Location " << x << "," << y << " has a Bowser and a blue coin square\n";
                    actors.push_back(new BlueCoinSquare(x, y, this));
                break;
            case Board::player:
                cout << "Location " << x << "," << y << " has Peach & Yoshi and a blue coin square\n";
                players.push_back(new Peach(x, y, this));
                players.push_back(new Yoshi(x, y, this));
                actors.push_back(new BlueCoinSquare(x, y, this));

                //(SPRITE_WIDTH * x, SPRITE_HEIGHT * y)
                break;
            //==================
            //squares
            case Board::red_coin_square:
                cout << "Location " << x << "," << y << " has a red coin square\n";
                actors.push_back(new RedCoinSquare(x,y,this));
                break;
            case Board::blue_coin_square:
                cout << "Location " << x << "," << y << " has a blue coin square\n";
                actors.push_back(new BlueCoinSquare(x, y, this));

                break;
                //FORCE THE DIRECTIONS
            case Board::up_dir_square:
                cout << "Location " << x << "," << y << " has a up square\n";
                actors.push_back(new DirectionalSquare(x, y, this, 90));

                break;
            case Board::down_dir_square:
                cout << "Location " << x << "," << y << " has a down direction square\n";
                actors.push_back(new DirectionalSquare(x, y, this, 270));

                break;
            case Board::left_dir_square:
                cout << "Location " << x << "," << y << " has a left direction square\n";
                actors.push_back(new DirectionalSquare(x, y, this, 180));

                break;
            case Board::right_dir_square:
                cout << "Location " << x << "," << y << " has a right direction square\n";
                actors.push_back(new DirectionalSquare(x, y, this, 0));

                break;
            case Board::event_square:
                cout << "Location " << x << "," << y << " has a event square\n";
                actors.push_back(new EventSquare(x, y, this));

                break;
            case Board::bank_square:
                cout << "Location " << x << "," << y << " has a bank square\n";
                actors.push_back(new BankSquare(x, y, this));

                break;
            case Board::star_square:
                cout << "Location " << x << "," << y << " has a star square\n";
                actors.push_back(new StarSquare(x, y, this));
                break;
                // etc…
            }
        }
        }

    //start countdown timer
	startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds ~ change later to 99

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    for (auto it = players.begin(); it != players.end(); it++) {
            (*it)->doSomething();
    }

    for (auto it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();

         }
    }

    for (auto it = actors.begin(); it != actors.end(); it++) {
        if (!(*it)->isAlive()) {
            delete* it;
            actors.erase(it);
            it = actors.begin();
        }
    }

    /*=============================================
                DISPLAY GAME STATISTICS 
      =============================================*/
    ostringstream oss;
    Player_Avatar* p1 = players.front();
    Player_Avatar* p2 = players.back();


    oss.fill('0');
    oss << "P1 Roll: ";
    oss << p1->getDieRoll() << "  ";
    oss << "Stars: " << p1->getStars() << " ";
    oss << "$$: " << p1->getCoins() << "  |";
    oss << " Time: " << timeRemaining() << "  |";
    oss << " Bank: " << "0" << " |";
    oss << " P2 Roll: " << p2->getDieRoll() << " ";
    oss << "Stars: " << p2->getStars() << " ";
    oss << "$$: " << p2->getCoins();

    setGameStatText(oss.str());
        

    
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);

        if (p1->getStars() >= p2->getStars()) {
            setFinalScore(p1->getStars(), p1->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else {
            setFinalScore(p2->getStars(), p2->getCoins());
                return GWSTATUS_YOSHI_WON;
        }
    }

    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* a : actors)
        delete a;
    actors.clear();

    for (Player_Avatar* a : players)
        delete a;
    players.clear();
    

}

StudentWorld::~StudentWorld() {
    cleanUp();
}

bool StudentWorld::needToTurn(Player_Avatar* p) {

    int x = p->getX();
    int y = p->getY();


    if (p->getDirection() == 0) { //if the player is facing right
        //check if there are any turns the player needs to make.
        for (auto a : actors) {
            if ((a->getY() == (p->getY() - 1)) || (a->getY() == (p->getY() + 1))) //if there is a square below or above the player while the player
                return true;
        }// is facing right.
    }


    if (p->getDirection() == 90) { //if the player is facing up
        //check if there are any turns the player needs to make.
        for (auto a : actors)
            if ((a->getX() == (p->getX() - 1)) || (a->getX() == (p->getX() + 1))) //if there is a square left or right the player while the player
                return true;                                                     // is facing up.
    }

    if (p->getDirection() == 180) { //if the player is left
        //check if there are any turns the player needs to make.
        for (auto a : actors)
            if ((a->getY() == (p->getY() - 1)) || (a->getY() == (p->getY() + 1))) //if there is a square up or down the player while the player
                return true;                                                     // is facing left.
    }

    if (p->getDirection() == 270) { //if the player is facing down
        //check if there are any turns the player needs to make.
        for (auto a : actors)
            if ((a->getX() == (p->getX() - 1)) || (a->getX() == (p->getX() + 1))) //if there is a square left or right the player while the player
                return true;                                                     // is facing down.
    }



    return false;
}

bool StudentWorld::overlappingFirstPlayer(Actor* a) {

    //if there is overlap between player one with an actor or player two with an actor
    if (a->getX() == players.front()->getX() && a->getY() == players.front()->getY()) 
        return true;

    return false;

}

bool StudentWorld::overlappingSecondPlayer(Actor* a) {
    if (a->getX() == players.back()->getX() && a->getY() == players.back()->getY()) 
        return true;
    
    return false;
}



/*
int StudentWorld::move()
{
// The term "actors" refers to all actors, e.g., Peach, Yoshi,
// baddies, squares, vortexes, etc.
// Give each actor a chance to do something, incl. Peach and Yoshi
for each of the actors in the game world
{
 if (actor[i] is still active/alive)
 {
 // tell that actor to do something
 actor[i]->doSomething();
 }
}
// Remove newly-inactive actors after each tick
remove inactive/dead game objects
// Update the Game Status Line
update display text // update the coins/stars stats text at screen top
 if (time has expired for the game)
{
 play end of game sound;
 if (yoshi won)
 {
 setFinalScore(yoshi_stars, yoshi_coins);
 return GWSTATUS_YOSHI_WON;
 }
 else // peach won
 {
 setFinalScore(peach_stars, peach_coins);
 return GWSTATUS_PEACH_WON;
 }
 }
// the game isn't over yet so continue playing
return GWSTATUS_CONTINUE_GAME;
 }

    */


bool StudentWorld::overlappingDirectionFirst(Player_Avatar* a) {
    for (auto d : actors)
        if (d->isDirectional()) { //if the player is overlapping a directional square
            if (a->getX() == d->getX() && a->getY() == d->getY()) {
                a->setDirection(d->getDirection());
                return true;
            }
        }
    return false;
}

bool StudentWorld::detectAheadFirstPlayer(int x, int y) {
    for (auto a = actors.begin(); a != actors.end(); a++) {
        //cout << a->getX() << " , " << a->getY() << endl;
        if ((*a)->getX() == x && (*a)->getY() == y) { //if there is an actor in a specific location in front of the player.
            return true;
        }
    }

    //std::cerr << actorPresent << std::endl;
    return false;

}

bool StudentWorld::detectAheadSecondPlayer(int x, int y) {
    for (auto a = actors.begin(); a != actors.end(); a++) {
        //cout << a->getX() << " , " << a->getY() << endl;
        if ((*a)->getX() == x && (*a)->getY() == y) { //if there is an actor in a specific location in front of the player.
            return true;
        }
    }

    //std::cerr << actorPresent << std::endl;
    return false;

}

