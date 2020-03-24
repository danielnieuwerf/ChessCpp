#include "Piece.h"
// constructors/destructor
Piece::Piece(const char& t){
    type = t;
}
Piece::Piece() {
    type = '.';    //default constructor creates empty square piece
}    
Piece::~Piece()
{
}

//methods
vector<string>Piece::getValidMoves(const Board& b, const Position& pos) const
{
    vector<string> legalMoves;
    return legalMoves;
}
char Piece::getType() const{
    return type;
}
bool Piece::isEmptySquare() const {
    return type == '.';
}
bool Piece::isBlack() const {
    return isupper(type);
}
bool Piece::isWhite() const{
    return islower(type);
}

// overloaded operators
Piece& Piece::operator = (const Piece& p)
{
    if (p.getType() == this->getType()) {
        return *this;
    }

    auto x = new Piece(p.getType());

    return *x;
}
ostream& operator<<(ostream& os, const Piece& piece)
{
    os << piece.getType();
    return os;
}
