#include"Board.h"
#include"Position.h"
#include<vector>
#include<string>
#include <iostream>

using namespace std;

class Piece
{
public:
    // constructor and destructor
    Piece();
    Piece(const char& t);	
    virtual ~Piece();

    // methods
    char getType() const;
    bool isBlack() const;
    bool isWhite() const;
    bool isEmptySquare() const;
    virtual vector<string> getValidMoves(const Board& b, const Position& pos) const;
    
    //overloaded operators
    Piece& operator = (const Piece& p);
    friend ostream& operator << (ostream& os, const Piece& piece);

private:
	char type;     
    /* 'k'-white king, 'K'-black king
       'n'-white knight, 'N'-black knight
       'r'-white rook, 'R'-black rook
       'p'-white pawn, 'P'-black pawn
       'b'-white bishop, 'B'-black bishop
       'q'-white queen, 'Q'-black queen
       '.'-empty square
     */
};

