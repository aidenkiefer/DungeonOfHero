//Aiden Kiefer
//UIC Fall 2023
//CS211 Program Design II

#ifndef _BOARDCELL_H
#define _BOARDCELL_H

using namespace std;

// First, the BoardCell abstract base class 

class BoardCell {
	
    public:  
		BoardCell() {} // default contstructor (do nothing)
        virtual ~BoardCell() {} // destructor (do nothing)
        
        virtual char display( ) = 0; // pure virtual function; this is an abstract base class

    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            
            //default stay in the same spot
            newR = getRow();
            newC = getCol();
            
        }
        
        virtual bool isHero( ) {return false;}
    	virtual bool isBaddie( ) {return false;}
    	virtual bool isSpace( ) {return false;}
        virtual bool isBarrier( ) {return false;}
        virtual bool isHole( ) {return false;}
    	virtual bool isExit( ) {return false;}
    	virtual bool isStatic( ) {return true;}

        virtual void setPower(int num) {}
        virtual int getPower() {return 1;}
        virtual void setNextMove(char inChar) {}
        
        void setMoved(bool m) {moved = m;}
        bool getMoved() {return moved;}
        void setRow(size_t r) {myRow = r;}
        size_t getRow() {return myRow;}
        void setCol(size_t c) {myCol = c;}
        size_t getCol() {return myCol;}
        void setPos(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }

    private:
        size_t myRow; // current row for this board cell in a 2D grid
        size_t myCol; // current column for this board cell in a 2D grid
        bool moved;   // true = this board cell already moved in the current round

}; // BoardCell (abstract base class)


// Then, all the derived classes

class Hero: public BoardCell {
	
    public:
    	Hero(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        
        virtual bool isHero( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return 'H';}
        
        virtual void setNextMove(char inChar ) {
            nextMove = inChar;
        }
    	
        virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            switch (nextMove) {
                case 'q':
                    newR = hRow - 1;
                    newC = hCol - 1;
                    break;
                case 'w':
                    newR = hRow - 1;
                    newC = hCol;
                    break;
                case 'e':
                    newR = hRow - 1;
                    newC = hCol + 1;
                    break;
                case 'a':
                    newR = hRow;
                    newC = hCol - 1;
                    break;
                case 's':
                    newR = hRow;
                    newC = hCol;
                    break;
                case 'd':
                    newR = hRow;
                    newC = hCol + 1;
                    break;
                case 'z':
                    newR = hRow + 1;
                    newC = hCol - 1;
                    break;
                case 'x':
                    newR = hRow + 1;
                    newC = hCol;
                    break;
                case 'c':
                    newR = hRow + 1;
                    newC = hCol + 1;
                    break;
                default:
                    //stay
                    newR = hRow;
                    newC = hCol;
                    break;
            }
        }

    private:
        char nextMove;

}; // Hero


class Monster: public BoardCell {
    
    public:
		Monster(size_t r, size_t c) {
            setRow(r);
            setCol(c);
            power = 1;
        }
        
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        
        virtual char display( ) {
            if (power == 2) {
                return 'M'; // Super Monster
            } else {
                return 'm'; // Monster
            } 
        }

        virtual void setPower(int num) {
            power = num;
        }

        virtual int getPower() {
            return power;
        }
        
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            int pwr = getPower();
            size_t c = getCol();
            size_t r = getRow();


            if (pwr == 1) {
                newC = c + (hCol < c ? -1 : (hCol > c ? 1 : 0));
                newR = r + (hRow < r ? -1 : (hRow > r ? 1 : 0));
            } else {
                newC = c + 2 * (hCol < c ? -1 : (hCol > c ? 1 : 0));
                newR = r + 2 * (hRow < r ? -1 : (hRow > r ? 1 : 0));
            }

            if (r < hRow) {
                cout << "Current position is above Hero" << endl;
                cout << "New attempted row: " << newR << endl;
            }
            else if (r > hRow) {
                cout << "Current position is below Hero" << endl;
                cout << "New attempted row: " << newR << endl;
            }
            else {
                cout << "Current position is on Hero's row" << endl;
            }

            if (c < hCol) {
                cout << "Current position is to the left of Hero" << endl;
                cout << "New attempted column: " << newC << endl;
            }
            else if (c > hCol) {
                cout << "Current position is to the right of Hero" << endl;
                cout << "New attempted column: " << newC << endl;
            }
            else {
                cout << "Current position is on Hero's column" << endl;
            }
        }
    
    private:
        int power; // power = 1 for Monster, power = 2 for SuperMonster

}; // Monster

class Bat: public BoardCell {
    public:
		Bat(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return '~';}
        
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            size_t r = getRow();
            size_t c = getCol();

            newC = hCol;
            newR = r;          
        }

}; // Bat


class Abyss: public BoardCell {
	public:
    	Abyss(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '#';}
        virtual bool isHole( ) {return true;}
}; // Abyss


class Wall: public BoardCell {
	public:
    	Wall(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '+';}
    	virtual bool isBarrier( ) {return true;}
}; // Wall


class Nothing: public BoardCell {
	public:
    	Nothing(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return ' ';}
    	virtual bool isSpace( ) {return true;}
}; // Nothing


class EscapeLadder: public BoardCell {
	public:
    	EscapeLadder(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '*';}
    	virtual bool isExit( ) {return true;}
}; // EscapeLadder


#endif //_BOARDCELL_H
