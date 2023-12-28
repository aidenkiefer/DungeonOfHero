//Aiden Kiefer
//UIC Fall 2023
//CS211 Program Design II


#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        size_t HeroRow; // Hero's position row
	    size_t HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }
        
        //---------------------------------------------------------------------------------
        // void getHeroPosition(size_t& row, size_t& col)
        //
        // getter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // note: row and col are passed-by-reference
        //---------------------------------------------------------------------------------
        void getHeroPosition(size_t& row, size_t& col) {
            row = HeroRow;  
            col = HeroCol;  
        }

        //---------------------------------------------------------------------------------
        // void setHeroPosition(size_t row, size_t col)
        //
        // setter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        //---------------------------------------------------------------------------------
        void setHeroPosition(size_t row, size_t col) {
            
            HeroRow = row;
            HeroCol = col;
        
        }

        //---------------------------------------------------------------------------------
        // findHero()
        //
        // updater for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // this function should find Hero in board and update
        //      HeroRow and HeroCol with the Hero's updated position;
        // if Hero cannot be found in board, then set Hero's position to (-1,-1)
        //---------------------------------------------------------------------------------
        void findHero() {
            
            for (size_t r = 0; r < numRows; ++r) {
                for (size_t c = 0; c < numCols; ++c) {
                    if (board(r, c)->isHero()) {
                        //hero found, update position
                        setHeroPosition(r, c);
                        return;
                    }
                }
            }

            //hero not found, set position to null
            setHeroPosition(-1, -1);
        
        }

        void swapCells(size_t HeroRow, size_t HeroCol, size_t newRow, size_t newCol) {
            //create pointers for each of the cells swapped
            Hero* hero = new Hero(HeroRow, HeroCol);
            Nothing* nothing = new Nothing(newRow, newCol);

            //free cells that are being swapped
            freeCell(newRow, newCol);
            freeCell(HeroRow, HeroCol);

            //set swapped cells 
            setCell(nothing, HeroRow, HeroCol);
            setCell(hero, newRow, newCol);

            //set new hero position (updates HeroRow and Col)
            //and moved bool
            setHeroPosition(newRow, newCol);
            hero->setMoved(true);

            nothing->setPos(HeroRow, HeroCol);
            //display result
            //display();
        }

        void removeHero(size_t HeroRow, size_t HeroCol) {
            freeCell(HeroRow, HeroCol);
            Nothing* nothing = new Nothing(HeroRow, HeroCol);
            setCell(nothing, HeroRow, HeroCol);
            nothing->setPos(HeroRow, HeroCol);
            findHero();
            
            //display();
        }

        void baddieSwap(size_t row, size_t col, size_t newRow, size_t newCol) {
            cout << "Swapping (" << row << ", " << col << ") with (" << newRow << ", " << newCol << ")" << endl;
            if (board(row, col)->display() == '~') {
                //Bat being swapped
                Bat* bat = new Bat(row, col);
                Nothing* nothing = new Nothing(newRow, newCol);

                //free cells that are being swapped
                freeCell(newRow, newCol);
                freeCell(row, col);

                //set swapped cells
                setCell(nothing, row, col);
                setCell(bat, newRow, newCol);

                bat->setPos(newRow, newCol);
                bat->setMoved(true);
                nothing->setPos(row, col);

                //display();
            }
            else {
                //Monster being swapped
                int pwr = board(row,col)->getPower();
                
                Monster* monster = new Monster(row, col);
                Nothing* nothing = new Nothing(newRow, newCol);

                //free cells that are being swapped
                freeCell(newRow, newCol);
                freeCell(row, col);

                //set swapped cells
                setCell(nothing, row, col);
                setCell(monster, newRow, newCol);

                
                monster->setPos(newRow, newCol);
                board(newRow, newCol)->setPower(pwr);
                board(newRow, newCol)->setMoved(true);
                nothing->setPos(row, col);

                //display();
            }
        }

        void removeBaddie(size_t row, size_t col) {
            freeCell(row, col);
            Nothing* nothing = new Nothing(row, col);
            setCell(nothing, row, col);
            nothing->setPos(row, col);
            //display();
        }


        void adjustPosition(size_t row, size_t col, size_t& newR, size_t& newC) {
            //  this helper function takes a current non-static cell and 
            //an invalid attempted position and updates newR and newC s.t.
            //the destination cell is valid and correct based on logic.

            //3 cases of change: row only, column only, both changed
            //Piece moving out of bounds
            //Piece running into wall
            //Baddie running into exit ladder
            cout << "Current coordinates: " << row << ", " << col << endl;
            cout << "Attempted position: " << newR << ", " << newC << endl;
            cout << "Adjustment initiated..." << endl;
            if (row != newR && col != newC) {
                //diagonal movement
                if (newR >= numRows && newC < numCols - 1) {
                    //attempted position has invalid row, but valid column (OOB)
                    newR = row;
                }
                else if (newC >= numCols && newR < numRows - 1) {
                    //attempted position has valid row and invalid column (OOB)
                    newC = col;
                }
                else if ((newC >= numCols) && (newR >= numRows)) {
                    //attempted position has invalid row and column
                    newC = col;
                    newR = row;
                }
                else {
                    //attempted position is in bounds, but is invalid (Object collision)
                    if (board(newR, col)->isSpace()) {
                        //no static object above or below, must be left or right
                        //invalid column
                        newC = col;
                    }
                    else if (board(row, newC)->isSpace()) {
                        //no static object to the left or right, must be above or below
                        //invalid row
                        newR = row;
                    }
                    else {
                        //static objects exist in both directions
                        newC = col;
                        newR = row;
                    }
                }
            }
            else {
                newC = col;
                newR = row;
            }
            cout << "Adjustment set. New coordinates: " << newR << ", " << newC << endl;
        }


        void moveBaddie(size_t row, size_t col, size_t HeroRow, size_t HeroCol, bool& onBoard) {
            //row and col represent current Baddie position
            //depending on HeroRow and HeroCol, move Baddie
            //handle edge cases appropriately

            //findHero(); //?
            size_t newBRow; //new Baddie row
            size_t newBCol; //new Baddie col

            bool removed = false;
            bool captured = false;

            board(row, col)->attemptMoveTo(newBRow, newBCol, HeroRow, HeroCol);

            cout << "Attempting to move Baddie from " << row << ", " << col << " to " << newBRow << ", " << newBCol << endl;

            //implement try/catch logic from makeMoves(), correctly changing the format
            //to work with the Baddie guidelines. this should handle the following edge cases in order:
            //
            //1. newBRow is out of bounds (newBRow >= numRows)
            //2. newBCol is out of bounds (newBCol >= numCols)
            //3. board(newBRow, newBCol) is not a space (object already in cell)
            //  a. if it's a wall or exit ladder, either return to previous square or adjacent square if move is diagonal
            //  b. if it's an abyss, remove the Baddie from the board using removeBaddie()
            //  c. if it's the Hero, remove the Hero and replace with empty Nothing cell
            //  d. if it's another Baddie, ignore attempted move, no position change
            //
            //once these edge cases are taken care of, board(newBRow, newBCol) should be empty
            //swap the Baddie cell (board(row, col)) with the empty cell (board(newBRow, newBCol))
            //done

            try {
                if (newBRow >= numRows) {
                    // Baddie attempts to move out-of-bounds in rows
                    throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Adjusting Baddie position to stay in-bounds.." << endl;
                adjustPosition(row, col, newBRow, newBCol);
            }

            try {
                if (newBCol >= numCols) {
                    // Baddie attempts to move out-of-bounds in columns
                    throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Adjusting Baddie position to stay in-bounds.." << endl;
                adjustPosition(row, col, newBRow, newBCol);
            }

            try {
                if (board(newBRow, newBCol)->isBarrier() || board(newBRow, newBCol)->isExit()) {
                    // Baddie ran into a wall or exit ladder
                    throw runtime_error("Baddie trying to move into a Wall or Exit cell");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Adjusting Baddie position to stay in-bounds.." << endl;
                adjustPosition(row, col, newBRow, newBCol);
            }

            try {
                if (board(newBRow, newBCol)->isHole()) {
                    // Baddie falls into a hole 
                    // Remove Baddie from the board
                    throw runtime_error("Baddie fell into a hole!!");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Removing Baddie position off the board" << endl;
                board(row, col)->setMoved(true);
                removeBaddie(row, col);
                removed = true;
                
            }

            try {
                if ((board(newBRow, newBCol)->isBaddie()) && !(newBRow == row && newBCol == col)) {
                    // Baddie runs into another Baddie 
                    // Baddie stays put
                    throw runtime_error("Baddie ran into another Baddie! BONK!");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Baddie gets up and dusts themselves off" << endl;
                // Stay put
                newBRow = row;
                newBCol = col;
                board(row, col)->setMoved(true);
            }

            try {
                if (board(newBRow, newBCol)->isHero()) {
                    // Baddie attempts to move onto the cell with the Hero
                    throw runtime_error("A Baddie caught the Hero! Game Over!");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                // remove Hero from the board + swap Baddie and Nothing cells 
                //removeHero(HeroRow, HeroCol);
                //baddieSwap(row, col, newBRow, newBCol);
                
                cout << "Waiting to remove Hero position off the board" << endl;
                onBoard = false;
                captured = true;
                //the issue is this, when the hero is removed, the monsters start heading the wrong way
                
            }

            cout << "Baddie edge cases checked" << endl;

            if (captured) {
                cout << "Monster caught the Hero... That's gotta hurt..." << endl;
                captured = false;
            }
            else if (removed) {
                cout << "Baddie has already been removed from board" << endl;
            }
            else if ((board(row, col)->getMoved() == false) && !(newBRow == row && newBCol == col)) {
                baddieSwap(row, col, newBRow, newBCol);
                cout << "Baddie swapped" << endl;
            }
            else if ((newBRow == row && newBCol == col)) {
                cout << "Baddie position remains the same" << endl;
            }
            else {
                cout << "Baddie getMoved() is currently set to: " << board(row, col)->getMoved() << endl;
            }
        }

        //---------------------------------------------------------------------------------
        bool makeMoves(char HeroNextMove) { //_________________________________________|
            //                                                                          \/
            // this function takes a character input representing the direction of      ||
            //the Hero's movement and updates the board accordingly, handling edge      ||
            //cases such as collision or invalid movement. it also determines the       ||
            //environmental response to the player's input and updates the status       //
            //of the game board. ___________  _______  _______  ________________________))
            //

            //resets HeroRow and Col for function
            findHero();

            //return condition, whether or not Hero still exists on the board
            bool onBoard = true;

            cout << "Hero position : (" << HeroRow << ", " << HeroCol << ")" << endl;

            //check for non-static pieces and reset their getMoved() bool
            for (size_t r = 0; r < numRows; ++r) {
                for (size_t c = 0; c < numCols; ++c) {
                    if (board(r, c)->isBaddie()) {
                        cout << "Baddie found at (" << board(r, c)->getRow() << ", " << board(r, c)->getCol() << "). Refreshing energy levels..." << endl;
                        board(r, c)->setMoved(false);
                        cout << "Baddie movement refreshed!" << endl;
                    }
                }
            }

            // determine where Hero proposes to move to
            size_t newR, newC;
            board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);

            //---------------------------------------------------------------------------------
            //  Try/Catch Section -- __________________________________________________________|                                                           
            // this section explores the edge cases, and executes logic accordingly.            \/
            //it's in here that the board data is updated along with the data surrounding       ||
            //the Hero and Baddie pieces. if the game is over, onBoard will be returned false   ||
            //otherwise, the loop continues. _______  _______  _______  ________________________/
            

            try {
                // Hero attempts to move out-of-bounds in rows
                if (newR >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                } 
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Adjusting Hero position to stay in-bounds.." << endl;
                adjustPosition(HeroRow, HeroCol, newR, newC);
                onBoard = true;
            }


            try {
                // Hero attempts to move out-of-bounds in columns
                if (newC >= numCols) {
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid column");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Adjusting Hero position to stay in-bounds.." << endl;
                adjustPosition(HeroRow, HeroCol, newR, newC);
                onBoard = true;
            }

            try {
                // Hero ran into a wall 
                if (board(newR, newC)->isBarrier()) {
                    throw runtime_error("Hero trying to move into a Wall cell");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Adjusting Hero position to avoid wall.." << endl;
                adjustPosition(HeroRow, HeroCol, newR, newC);
                onBoard = true;
            }

            try {
                // Hero finds the exit!
                if (board(newR, newC)->isExit()) {
                    throw runtime_error("Hero reached the exit! You win!");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                wonGame = true;
                cout << "Hero takes the exit ladder and escapes..." << endl;
                removeHero(HeroRow, HeroCol);
                onBoard = false;
            }

            try {
                if (board(newR, newC)->isHole()) {
                    // Hero falls into a hole (losing condition)
                    // remove Hero from board
                    throw runtime_error("Hero fell into a hole! You lose!");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Removing Hero position off the board" << endl;
                removeHero(HeroRow, HeroCol);
                onBoard = false;
            }

            try {
                if (board(newR, newC)->isBaddie()) {
                    // Hero encounters a Baddie (losing condition)
                    // remove Hero from board
                    throw runtime_error("Hero encountered a Baddie! You lose!");
                }
            } 
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                cout << "Removing Hero position off the board" << endl;
                removeHero(HeroRow, HeroCol);
                onBoard = false;
            }

            
            //by this point we've checked that the new position is:
            // -in bounds
            // -not wall
            // -not exit
            // -not abyss
            // -not baddie
            //so now we just need to swap cells if necessary, and
            //move the Baddies
            
            if (onBoard) {
                if (newR == HeroRow && newC == HeroCol) {
                    //Hero is not moving or no adjustments have been made
                    cout << "Hero decided to stay put for now..." << endl;
                    cout << "..." << endl << "Hero wakes up feeling well rested!" << endl;
                }
                else {
                    //Hero is on board, and adjustments have been made
                    //swapCells function call
                    swapCells(HeroRow, HeroCol, newR, newC);
                    cout << "Hero moved to ("<< newR << ", " << newC << ")!" << endl;
                    display();
                }

                size_t bRow, bCol;

                //check each cell of board for Baddies who have not moved yet this turn
                //for each, call moveBaddie to move them and update the board
                //also updates the onBoard bool in case of collision with Hero
                for (size_t r = 0; r < numRows; ++r) {
                    for (size_t c = 0; c < numCols; ++c) {
                        if (board(r, c)->isBaddie() && board(r, c)->getMoved() == false) {  
                            cout << "Baddie found at " << board(r, c)->getRow() << ", " << board(r, c)->getCol() << " and has not moved yet this turn" << endl;
                            if (onBoard) {
                                moveBaddie(r, c, HeroRow, HeroCol, onBoard);
                                if (!onBoard) {
                                    bRow = r;
                                    bCol = c;
                                }
                            }
                            else {
                                moveBaddie(r, c, HeroRow, HeroCol, onBoard);
                            }
                            display();
                        }
                    }
                }

                if (!onBoard) {
                    cout << "Removing Hero now..." << endl;
                    size_t tRow = HeroRow;
                    size_t tCol = HeroCol;
                    cout << "Old Hero coords were " << tRow << ", " << tCol << endl;
                    int oPwr = board(bRow, bCol)->getPower();
                    removeHero(HeroRow, HeroCol);
                    cout << "Hero removed. Swapping baddie to correct position." << endl;
                    baddieSwap(bRow, bCol, tRow, tCol);
                    board(tRow, tCol)->setPower(oPwr);
                    cout << "Board updated." << endl;
                    display();
                }
                
            }
            else {
                if (wonGame) {
                    cout << "Congratulations Hero! You won!" << endl;
                }
                else {
                    cout << "You lost! RIP Hero..." << endl;
                }
            }
            
            return onBoard;
        }

    
};

#endif //_GAMEBOARD_H