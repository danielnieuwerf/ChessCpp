// Class used to store position (x,y) on the game board
class Position
{
public:
    // Constructors
    Position(int x, int y);
    Position(const Position& pos);      //copy constructor

    //Overloaded operators
    Position& operator = (const Position& pos);	

    //Getters and setters
    int getX() const;	
    int getY() const;	
    void setX(int a)
    {
        x = a;
    }
    void setY(int a)
    {
        y = a;
    }


private:
	// x,y co-ordinates in board
	int x;
	int y;
};

