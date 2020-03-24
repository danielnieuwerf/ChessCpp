#include "Position.h"


// constructors
Position::Position(int a, int b)
{
    x = a;
    y = b;
}
Position::Position(const Position& pos)
{
    x = pos.x;
    y = pos.y;
}

// overloaded operators
Position& Position::operator = (const Position& pos)
{
    if (&pos == this) {
        return *this;
    }

    x = pos.x;
    y = pos.y;

    return *this;
}

// getters
int Position::getX() const
{
    return x;
}
int Position::getY() const
{
    return y;
}
