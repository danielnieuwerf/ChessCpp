#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

//function prototypes in alphabetical order
void initialBoard(char[8][8]);
void printBoard(char[8][8],bool);
void makeMove(char[8][8],string);
bool validString(string);
bool validMove(char[8][8], string, bool);
string getDiagUp(char[8][8], int, int);
string getDiagDown(char[8][8], int, int);
string getFile(char[8][8], int);
string getRank(char[8][8], int);
vector<int> verticalBounds(char[8][8], int, int);
vector<int> horizontalBounds(char[8][8], int, int);
bool whiteKingIsInCheck(char[8][8], int, int);
bool blackKingIsInCheck(char[8][8], int, int);
bool stillInCheck(char[8][8],string);
bool pawnPromotionNeeded(char[8][8]);
char pawnPromotion(char[8][8], bool);
vector<int> findWhiteKing(char[8][8]);
vector<int> findBlackKing(char[8][8]);
void whiteCastle(char[8][8], bool, string, string);
void blackCastle(char[8][8], bool, string, string);
bool isCheckmate(char[8][8], bool);
bool isStalemate(char[8][8], bool);

int main()
{
    bool gameover = false, checkmate=false, stalemate=false;
    bool whiteCanCastle=true, blackCanCastle = true;
    bool whiteTurn = true;
    char board[8][8];
    int whiteKingRow=7;         // position of kings initially
    int whiteKingCol=4;         // keep track of these 
    int blackKingRow=0;         // throughout the game
    int blackKingCol=4;

    // initialise board and display it
    initialBoard(board);
    printBoard(board,whiteTurn);
    string s{};                 // move inputs stored in string

    //play game until gameover
    while (!gameover) {
        cin >> s;
        while(!validString(s)){ cin >> s; }

        if (stillInCheck(board, s)) { cout << "invalid move"; }
        // special moves castling and en passant
        else if((s=="7476"||s=="7472" )&& whiteCanCastle){ 
            //move is valid so castle
            whiteCastle(board, whiteCanCastle, s, getRank(board, 7));
            //white can no longer castle
            whiteCanCastle = false;
            // flip whiteTurn
            whiteTurn = false;
            //clear screen and display board again
            system("CLS");
            printBoard(board, whiteTurn);
        
        }
        else if ((s == "0406" || s == "0402") && blackCanCastle) {
            blackCastle(board, blackCanCastle, s, getRank(board, 0));
            //black can no longer castle
            blackCanCastle = false;
            // flip whiteTurn
            whiteTurn = true;
            //clear screen and display board again
            system("CLS");
            printBoard(board, whiteTurn);
        }
        else if (validMove(board, s, whiteTurn)) {
            //if move is valid and no longer in check, make the move
            makeMove(board, s);
            // update king position and castlability
            int r = (int)s[2] - 48, c = (int)s[3] - 48;
            char temp = board[r][c];
            if (temp == 'k') {
                whiteKingRow = r;
                whiteKingCol = c;
                whiteCanCastle = false;
            }
            else if (temp == 'K') {
                blackKingRow = r;
                blackKingCol = c;
                blackCanCastle = false;
            }

            // check for pawn promotion
            bool pawnPromo=pawnPromotionNeeded(board);
            if (pawnPromo) {
                board[r][c] = pawnPromotion(board, whiteTurn);
            }
            // flip whiteTurn
            whiteTurn = !whiteTurn;
            //clear screen and display board again
            system("CLS");
            printBoard(board,whiteTurn);
            //if check display check
            if (whiteKingIsInCheck(board, whiteKingRow, whiteKingCol) ||
                blackKingIsInCheck(board, blackKingRow, blackKingCol)) {
                cout << "\nCheck!";
            }
             }
        else { cout << "invalid move"; }
        // check for checkmate or stalemate
        if (isStalemate(board,whiteTurn)) { stalemate = true; }
        else if (isCheckmate(board,whiteTurn)) { checkmate = true; }
        if(checkmate || stalemate){ gameover=true; }
    }
    //display final board 
    // print game result
    //and prompt user 
    // s: save game, q:quit , n: new game
}

//function definitions in alphabetical order
void initialBoard(char b[8][8]){
    b[0][0] = b[0][7] = 'R';
    b[0][1] = b[0][6] = 'N';
    b[0][2] = b[0][5] = 'B';
    b[0][3] = 'Q';
    b[0][4] = 'K';
    b[7][0] = b[7][7] = 'r';
    b[7][1] = b[7][6] = 'n';
    b[7][2] = b[7][5] = 'b';
    b[7][3] = 'q';
    b[7][4] = 'k';
    for (int i = 0; i < 8; ++i) {
        b[1][i] = 'P';
        b[6][i] = 'p';
    }
    for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) { b[i][j] = '.'; }
    }
}
void printBoard(char b[8][8],bool turn) {
    turn ? cout << " White's turn (lower case)"<<endl : cout << " Black's turn (upper case)"<<endl;
    cout << "  0  1  2  3  4  5  6  7 " << endl;
    cout << "  -----------------------" << endl;
    for (int i = 0; i < 8; ++i) {
        cout << i;
        for (int j = 0; j < 8; ++j) {
            cout<<"|"<<b[i][j]<<" ";
        }
        cout <<"|"<< endl;
    }
    cout << "  -----------------------"<<endl;
    //cout << "  A  B  C  D  E  F  G  H " << endl;
}
void makeMove(char b[8][8],string s) {
    int itemp1 = (int)s[0]-48, itemp2=(int)s[1]-48;
    int itemp3 = (int)s[2]-48, itemp4=(int)s[3]-48;
    // move piece to new pos and set old square to empty
    char temp = b[itemp1][itemp2];
    b[itemp3][itemp4] = temp;
    b[itemp1][itemp2] = '.';   
}
bool validString(string s) {
    if (s.length() != 4) { return false; }
    if (!isdigit(s[0])) { return false; }
    if (!isdigit(s[1])) { return false; }
    if (!isdigit(s[2])) { return false; }
    if (!isdigit(s[3])) { return false; }
    if (s[0] == '9' || s[1] == '9' || s[2] == '9' || s[3] == '9') { return false; }
    if (s[0] == s[2] && s[1] == s[3]) { return false; } //can't move to self
    return true;
}
bool validMove(char b[8][8], string s,bool turn) {
    int row1 = (int)s[0] - 48, col1 = (int)s[1] - 48;
    int row2 = (int)s[2] - 48, col2 = (int)s[3] - 48;
    char temp = b[row1][col1];              //initial position
    char temp2 = b[row2][col2];             //final position

    // if white moves during black move or vice versa return false
    if (isupper(temp) && turn) { return false; }
    if (islower(temp) && !turn) { return false; }

    // if try move empty square return false
    if (temp == '.') { return false; }

    switch (temp) {
    case 'p': {
        if (row1 == row2 + 1 && col1 == col2 && temp2 == '.') { return true; }
        if (row1 == row2 + 2 && row1 == 6 && col1 == col2 && temp2 == '.' && b[row2 + 1][col1] == '.') { return true; }
        if (row1 == row2 + 1 && col1 == col2 + 1 && isupper(temp2)) { return true; }
        if (row1 == row2 + 1 && col1 == col2 - 1 && isupper(temp2)) { return true; }
        return false; }
    case 'P': {
        if (row2 == row1 + 1 && col1 == col2 && temp2 == '.') { return true; }
        if (row2 == row1 + 2 && row1 == 1 && col1 == col2 && temp2 == '.' && b[2][col1] == '.') { return true; }
        if (row2 == row1 + 1 && col1 == col2 + 1 && islower(temp2)) { return true; }
        if (row2 == row1 + 1 && col1 == col2 - 1 && islower(temp2)) { return true; }
        return false;
    }
    case 'n': {
        if (row1 + 1 == row2 && col1 + 2 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 + 1 == row2 && col1 - 2 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 - 1 == row2 && col1 + 2 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 - 1 == row2 && col1 - 2 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }         if (row1 + 2 == row2 && col1 + 1 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 + 2 == row2 && col1 - 1 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 - 2 == row2 && col1 + 1 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 - 2 == row2 && col1 - 1 == col2 && (temp2 == '.' || isupper(temp2))) {return true;}
        return false;
        }
    case 'N': {
        if (row1 + 1 == row2 && col1 + 2 == col2 && (temp2 == '.' || islower(temp2))) { return true; }
        if (row1 + 1 == row2 && col1 - 2 == col2 && (temp2 == '.' || islower(temp2))) { return true; }
        if (row1 - 1 == row2 && col1 + 2 == col2 && (temp2 == '.' || islower(temp2))) { return true; }
        if (row1 - 1 == row2 && col1 - 2 == col2 && (temp2 == '.' || islower(temp2))) { return true; }
        if (row1 + 2 == row2 && col1 + 1 == col2 && (temp2 == '.' || islower(temp2))) { return true; }
        if (row1 + 2 == row2 && col1 - 1 == col2 && (temp2 == '.' || islower(temp2))) { return true; }
        if (row1 - 2 == row2 && col1 + 1 == col2 && (temp2 == '.' || islower(temp2))) { return true; }
        if (row1 - 2 == row2 && col1 - 1 == col2 && (temp2 == '.' || islower(temp2))) { return true; }
        return false; }
    case 'b':
    case 'B': {
        //vector<int> diag = diagonalBoundsUp(b, row1, col1);
        //int l = diag[0];
        //int h = diag[1];
    
    }
    case 'r' || 'R': {
        vector<int> vert = verticalBounds(b, row1, col1);
        int l = vert[0];
        int h = vert[1];
        if(l>-1){
            if (l <= row2 && row2 <= h && col1 == col2) { return true; }       
        }
        
        auto horizon = horizontalBounds(b, row1, col1);
        int lh = horizon[0];
        int hh = horizon[1];
        if (lh > -1) {
            if (lh <= col2 && col2 <= hh && row1 == row2) { return true; }
        }

        return false; 
    }     
    case 'q':
    case 'Q':
    case 'k': {
        if (abs(row1 - row2) <= 1 && abs(col1 - col2) <= 1 && (temp2 == '.' || isupper(temp2))) { return true; }
        return false; 
    }
    case 'K': {
        if (abs(row1 - row2) <= 1 && abs(col1 - col2) <= 1 && (temp2 == '.' || islower(temp2))) { return true; }
        return false; 
    }
    }

    // shouldn't reach here
    return true;
}
vector<int> verticalBounds(char b[8][8], int r, int c) {
    // returns {-1,-1} if no vertical moves
    //else returns range low to high of possible vertical moves
    int low{}, high{};
    // check if piece is white or black
    char temp = b[r][c];
    bool isWhite = islower(temp);
    int row = r;        //initial row
    if (temp == '.') { return { -2,-2 }; }      //trying to move an empty square

    // no moves if surrounded by same colour pieces or edges return {-1,-1}
    {if (r == 0 && ((isWhite && islower(b[1][c])) || (!isWhite && isupper(b[1][c])))) {
        return { -1,-1 };
    }
    if (r == 7 && ((isWhite && islower(b[6][c])) || (!isWhite && isupper(b[6][c])))) {
        return { -1,-1 };
    }
    if (0 < r && r < 7 && (isWhite && islower(b[r - 1][c]) && islower(b[r + 1][c]))) {
        return { -1,-1 };
    }
    if (0 < r && r < 7 && (!isWhite && isupper(b[r - 1][c]) && isupper(b[r + 1][c]))) {
        return { -1,-1 };
    }}      
    // if we reach here at least one move is possible
    // if cant go down set low to row+1 
    bool done = false;
    if (row == 0) {
        low = 1;
        done = true;
    }
    else if (((isWhite && islower(b[r - 1][c])) || (!isWhite && isupper(b[r - 1][c])))) {
        low = r + 1;
        done = true;
    }
    while (!done) {
        //keep going down until cant go down anymore
        if (((isWhite && isupper(b[r - 1][c])) || (!isWhite && islower(b[r - 1][c])))) {
            low = r - 1;
            done = true;
        }
        else if (b[r - 1][c] == '.') {
            if (r == 1) {
                low = 0;
                done = true;
            }
            if (r > 1) { r--; }
        }
        else {//blocked by same colour
            low = r;
            done = true;    
        }
    }

    // reset r to row and done to false to find high
    r = row;
    done = false;
    // if can't go up and at least one move exists set high to row-1
    if (row == 7) {
        high = 6;
        done = true;
    }
    else if(((isWhite && islower(b[r + 1][c])) || (!isWhite && isupper(b[r + 1][c])))) {
        high = r - 1;
        done = true;
    }
    //keep going up until cant go up anymore
    while (!done) {
        if (((isWhite && isupper(b[r + 1][c])) || (!isWhite && islower(b[r + 1][c])))) {
            high = r + 1;
            done = true;
        }
        else if (b[r + 1][c] == '.') {
            if (r == 6) {
                high = 7;
                done = true;
            }
            if (r < 6) { r++; }
        }
        else {//blocked by same colour
            high = r;
            done = true;
        }
    }
    return { low,high };
}
vector<int> horizontalBounds(char b[8][8], int r, int c) {
    // rotate copy of board 90 degrees then apply verticalBounds to copy
    char copy[8][8];
    char temp[8][8];
    for(int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            copy[i][j] = temp[i][j]=b[i][j];
        }
    }

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            copy[j][7-i] = temp[i][j];
        }
    }
    return verticalBounds(copy,c,7-r);
} 
char pawnPromotion(char b[8][8], bool turn) {
    char p;
    while (true) {
        cout << "Promote to piece: queen(q), rook(r), bishop(b), knight(n) ";
        cin >> p;
        if (turn) {
            if (p == 'q') { return 'q'; }
            else if (p == 'r') { return 'r'; }
            else if (p == 'b') { return 'b'; }
            else if (p == 'n') { return 'n'; }
        }
        else {
            if (p == 'q') { return 'Q'; }
            else if (p == 'r') { return 'R'; }
            else if (p == 'b') { return 'B'; }
            else if (p == 'n') { return 'N'; }
        }
    }
    return '.';
}
bool pawnPromotionNeeded(char b[8][8]) {
    for (int i = 0; i < 8; ++i) {
        if (b[7][i] == 'P') { return true; }
    }
    for (int i = 0; i < 8; ++i) {
        if (b[0][i] == 'p') { return true; }
    }
    return false;
}
bool whiteKingIsInCheck(char b[8][8], int r, int c) {
    // r/c = row/column of white king

    //check by black pawn
    if (r >= 1 && c >= 1 && b[r - 1][c - 1] == 'P') { return true; }
    if (r >= 1 && c <= 6 && b[r - 1][c + 1] == 'P') { return true; }
    //check by black knight
    if (r >= 2 && c >= 1 && b[r - 2][c - 1] == 'N') { return true; }
    if (r >= 2 && c <= 6 && b[r - 2][c + 1] == 'N') { return true; }
    if (r >= 1 && c >= 2 && b[r - 1][c - 2] == 'N') { return true; }
    if (r >= 1 && c <= 5 && b[r - 1][c + 2] == 'N') { return true; }
    if (r <= 5 && c >= 1 && b[r + 2][c - 1] == 'N') { return true; }
    if (r <= 5 && c <= 6 && b[r + 2][c + 1] == 'N') { return true; }
    if (r <= 6 && c >= 2 && b[r + 1][c - 2] == 'N') { return true; }
    if (r <= 6 && c <= 5 && b[r + 1][c + 2] == 'N') { return true; }

    //check by black king (effectively)
    if (r >= 1 && c >= 1 && b[r - 1][c - 1] == 'K') { return true; }
    if (r >= 1  && b[r - 1][c] == 'K') { return true; }
    if (r >= 1 && c <= 6 && b[r - 1][c + 1] == 'K') { return true; }
    if (c <= 6 && b[r][c + 1] == 'K') { return true; }
    if (c >= 1 && b[r][c - 1] == 'K') { return true; }
    if (r <= 6 && c <= 6 && b[r + 1][c + 1] == 'K') { return true; }
    if (r <= 6 && c>=1 && b[r + 1][c - 1] == 'K') { return true; }
    if (r <= 6 && b[r + 1][c] == 'K') { return true; }

    //start at king position and look out in every direction
    // Diagonals 
    string diag1 = getDiagDown(b, r, c);
    string diag2 = getDiagUp(b, r, c);
    string d1{}, d2{};
    //erase empty spaces and set bishops and queens to 'X'
    for (auto c : diag1) {
        if(c!='.'){
            if (c == 'B' || c == 'Q') { 
                d1 += 'X'; 
            }
            else {
                d1 += c;
            }
        }
    }
    for (auto c : diag2) {
        if (c != '.') {
            if (c == 'B' || c == 'Q') {
                d2 += 'X';
            }
            else {
                d2 += c;
            }
        }
    }
    // if kX or Xk in d1 or d2 return true
    auto found = d1.find("Xk");
    if (found != string::npos) { return true; }
    auto found2 = d1.find("kX");
    if (found2 != string::npos) { return true; }
    auto found3 = d2.find("Xk");
    if (found3 != string::npos) { return true; }
    auto found4 = d2.find("kX");
    if (found4 != string::npos) { return true; }

    //horizontal and vertical
    string file = getFile(b, c);
    string rank = getRank(b, r);
    string f{}, ran{};

    //erase empty spaces and set rooks and queens to 'X'
    for (auto c : file) {
        if (c != '.') {
            if (c == 'R' || c == 'Q') {
                f += 'X';
            }
            else {
                f += c;
            }
        }
    }
    for (auto c : rank) {
        if (c != '.') {
            if (c == 'R' || c == 'Q') {
                ran += 'X';
            }
            else {
                ran += c;
            }
        }
    }
    // if kX or Xk in f or ran return true
    auto found5 = f.find("Xk");
    if (found5 != string::npos) { return true; }
    auto found6 = f.find("kX");
    if (found6 != string::npos) { return true; }
    auto found7 = ran.find("Xk");
    if (found7 != string::npos) { return true; }
    auto found8 = ran.find("kX");
    if (found8 != string::npos) { return true; }

    return false;
};
bool blackKingIsInCheck(char b[8][8], int r, int c) {
    // r/c = row/column of black king

    //check by white pawn
    if (r <= 6 && c >= 1 && b[r + 1][c - 1] == 'p') { return true; }
    if (r <= 6 && c <= 6 && b[r + 1][c + 1] == 'p') { return true; }

    //check by white knight 
    if (r >= 2 && c >= 1 && b[r - 2][c - 1] == 'n') { return true; }
    if (r >= 2 && c <= 6 && b[r - 2][c + 1] == 'n') { return true; }
    if (r >= 1 && c >= 2 && b[r - 1][c - 2] == 'n') { return true; }
    if (r >= 1 && c <= 5 && b[r - 1][c + 2] == 'n') { return true; }
    if (r <= 5 && c >= 1 && b[r + 2][c - 1] == 'n') { return true; }
    if (r <= 5 && c <= 6 && b[r + 2][c + 1] == 'n') { return true; }
    if (r <= 6 && c >= 2 && b[r + 1][c - 2] == 'n') { return true; }
    if (r <= 6 && c <= 5 && b[r + 1][c + 2] == 'n') { return true; }

    //check by white king (effectively)
    if (r >= 1 && c >= 1 && b[r - 1][c - 1] == 'k') { return true; }
    if (r >= 1 && b[r - 1][c] == 'k') { return true; }
    if (r >= 1 && c <= 6 && b[r - 1][c + 1] == 'k') { return true; }
    if (c <= 6 && b[r][c + 1] == 'k') { return true; }
    if (c >= 1 && b[r][c - 1] == 'k') { return true; }
    if (r <= 6 && c <= 6 && b[r + 1][c + 1] == 'k') { return true; }
    if (r <= 6 && c >= 1 && b[r + 1][c - 1] == 'k') { return true; }
    if (r <= 6 && b[r + 1][c] == 'k') { return true; }

    // Diagonals 
    string diag1 = getDiagDown(b, r, c);
    string diag2 = getDiagUp(b, r, c);
    string d1{}, d2{};
    //erase empty spaces and set bishops and queens to 'x'
    for (auto c : diag1) {
        if (c != '.') {
            if (c == 'b' || c == 'q') {
                d1 += 'x';
            }
            else {
                d1 += c;
            }
        }
    }
    for (auto c : diag2) {
        if (c != '.') {
            if (c == 'b' || c == 'q') {
                d2 += 'x';
            }
            else {
                d2 += c;
            }
        }
    }
    // if Kx or xK in d1 or d2 return true
    auto found = d1.find("xK");
    if (found != string::npos) { return true; }
    auto found2 = d1.find("Kx");
    if (found2 != string::npos) { return true; }
    auto found3 = d2.find("xK");
    if (found3 != string::npos) { return true; }
    auto found4 = d2.find("Kx");
    if (found4 != string::npos) { return true; }

    //horizontal and vertical
    string file = getFile(b, c);
    string rank = getRank(b, r);
    string f{}, ran{};

    //erase empty spaces and set rooks and queens to 'x'
    for (auto c : file) {
        if (c != '.') {
            if (c == 'r' || c == 'q') {
                f += 'x';
            }
            else {
                f += c;
            }
        }
    }
    for (auto c : rank) {
        if (c != '.') {
            if (c == 'r' || c == 'q') {
                ran += 'x';
            }
            else {
                ran += c;
            }
        }
    }
    // if xK or Kx in f or ran return true
    auto found5 = f.find("xK");
    if (found5 != string::npos) { return true; }
    auto found6 = f.find("Kx");
    if (found6 != string::npos) { return true; }
    auto found7 = ran.find("Kx");
    if (found7 != string::npos) { return true; }
    auto found8 = ran.find("xK");
    if (found8 != string::npos) { return true; }

    return false;
};
bool stillInCheck(char b[8][8], string str) {
    //copy board and make move on the copy board
    char copy[8][8];
    for (int i = 0; i < 8; ++i) { 
        for (int j = 0; j < 8; ++j) { 
            copy[i][j] = b[i][j]; 
        }
    }
    makeMove(copy,str);
    //update king position
    //if last move was a king move we know the kings position
    int r = (int)str[2] - 48, c = (int)str[3] - 48;
    char temp = copy[r][c];
    int wkr{}, wkc{}, bkr{}, bkc{};
    if (temp == 'k') {
        wkr= r;
        wkc = c;
        if (whiteKingIsInCheck(copy, wkr, wkc)) { return true; }
    }
    else if (temp == 'K') {
        bkr = r;
        bkc = c;
        if (blackKingIsInCheck(copy, bkr, bkc)) { return true; }
    }
    
    // find location of kings whose turn it is
    // then check if the king is still in check
    if (islower(temp)) {
        vector<int> wkvec = findWhiteKing(copy);
        wkr = wkvec[0];
        wkc = wkvec[1];
        if (whiteKingIsInCheck(copy, wkr, wkc)) {
            return true;
        }
    }
    if (isupper(temp)) {
        vector<int> bkvec = findBlackKing(copy);
        bkr = bkvec[0];
        bkc = bkvec[1];
        if (blackKingIsInCheck(copy, bkr, bkc)) { return true; }
    }
    
    return false;
}
vector<int> findWhiteKing(char b[8][8]) {
    /* Returns vector of size 2
     vec[0] is the kings row
     and vec[1] is the kings column.
     we search for the king around it's 
     starting position  (7,4) */
    if (b[7][4] == 'k') { 
        return { 7,4 }; 
    }
    for (int i = 7; i > -1; --i) {
        for (int j = 0; j < 8; ++j) {
            if (b[i][j] == 'k') {
                return { i,j };
            }
        } 
    }
    return { -1,-1 };
};
vector<int> findBlackKing(char b[8][8]) {
    /* Returns vector of size 2
 vec[0] is the kings row
 and vec[1] is the kings column.
 we search for the king around it's
 starting position  (0,4) */
    if (b[0][4] == 'K') {
        return { 0,4 };
    }
    for (int i = 0; i <8 ; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (b[i][j] == 'K') {
                return { i,j };
            }
        }
    }
    return { -1,-1 };
};
string getFile(char b[8][8], int c) {
    string ans{};
    for (int i = 0; i < 8; ++i) {
        ans += b[i][c]; 
    }
    return ans;
}
string getRank(char b[8][8], int r) {
    string ans{};
    for (int i = 0; i < 8; ++i) {
        ans += b[r][i];
    }
    return ans;
}
void whiteCastle(char b[8][8], bool wcc, string move, string rank){
    if(wcc){
        if (move == "7476") {
            auto found = rank.find("k..r");
            if (found != string::npos) {
                b[7][6] = 'k';
                b[7][5] = 'r';
                b[7][4] = '.';
                b[7][7] = '.';       
            }
        }
        else if (move == "7472") {
            auto found = rank.find("r...k");
            if (found != string::npos) {
                b[7][2] = 'k';
                b[7][3] = 'r';
                b[7][0] = '.';
                b[7][4] = '.';
            }
        }
        wcc = false;
    }
   
}
void blackCastle(char b[8][8], bool bcc, string move, string rank) {
    if (bcc) {
        if (move == "0406") {
            auto found = rank.find("K..R");
            if (found != string::npos) {
                b[0][6] = 'K';
                b[0][5] = 'R';
                b[0][4] = '.';
                b[0][7] = '.';
            }
        }
        else if (move == "0402") {
            auto found = rank.find("R...K");
            if (found != string::npos) {
                b[0][2] = 'K';
                b[0][3] = 'R';
                b[0][0] = '.';
                b[0][4] = '.';
            }
        }
        bcc = false;    
    }
}
string getDiagUp(char b[8][8], int r, int c) {
    /*
    Return diagonal containing (r,c)
    
    */
    string ans{};
    int temp = min(r,c);
    r -= temp;
    c -= temp;
    ans += b[r][c];
    r++;
    c++;
    while (r <= 7 && c <= 7) {
        ans += b[r][c];
        r++;
        c++;
    }

    return ans;
}
string getDiagDown(char b[8][8], int r, int c) {
    /*
    Return diagonal containing (r,c)

    */
    string ans{};
    int temp = min(r, 7-c);
    r -= temp;
    c += temp;
    ans += b[r][c];
    r++;
    c--;
    while (r <= 7 && c >= 0) {
        ans += b[r][c];
        r++;
        c--;
    }

    return ans;
}
bool isStalemate(char b[8][8], bool turn) {
    return false;
}
bool isCheckmate(char b[8][8], bool turn) {
    return false;
}
bool whiteCastleMoveIsValid(char b[8][8], string move ) {}
bool blackCastleMoveIsValid(char b[8][8], string move) {}
/*
CODE REVIEW

catsling should return true if castle was done successfully so we can set bcc wcc to false
test castling functions to see if they need to b fixed

castling should only be done if king doesn travel through under attack areas
move the king to each place on the journey and calculate isincheck for each


add en passant to validmove? no cannot
must check en passant valid doesnt reveal check
en passant
create string t to hold last successful move
pass in t,s,board


bishops queens for validmove and redo it for rooks?


need test checking by r b q


define CHECKMATE n stalemate



use tolower and toupper merge blackisincheck and whiteisincheck
call whiteisincheck on blackisincheck costs efficiency tho
*/