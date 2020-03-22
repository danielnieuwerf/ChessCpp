#include <algorithm>
#include <cstddef> 
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

// overloaded operator prototypes
template < class T >
inline std::ostream& operator << (std::ostream&, const std::vector<T>&);

//function prototypes in alphabetical order
void addGameToSavedGames(vector<string>);
void blackCastle(char[8][8], bool, string);
bool blackCastleMoveIsValid(char[8][8], bool, string);
bool blackKingIsInCheck(char[8][8], int, int);
bool blackKingIsInCheckmate(char[8][8], int, int);
string convertString(string);
bool enPassantAttempt(char[8][8], string, string);
vector<int> findBlackKing(char[8][8]);
vector<int> findWhiteKing(char[8][8]);
void gameReplay(vector<string>);
void gameResult(bool, bool, bool, vector<string>);
string getEast(char[8][8], int, int);
string getFile(char[8][8], int);
string getNorth(char[8][8], int, int);
string getNorthEast(char[8][8], int, int);
string getNorthWest(char[8][8], int, int);
string getRank(char[8][8], int);
string getSouth(char[8][8], int, int);
string getSouthEast(char[8][8], int, int);
string getSouthWest(char[8][8], int, int);
string getWest(char[8][8], int, int);
void howToPlay();
void initialBoard(char[8][8]);
bool isCheckmate(char[8][8], bool, int, int, int, int);
bool isDrawByLackOfMaterial(char[8][8]);
bool isStalemate(char[8][8], bool, int, int, int, int);
bool makeEnPassentMove(char[8][8], string, string);
void makeMove(char[8][8], string);
void menu();
void newGame();
char pawnPromotion(char[8][8], bool);
bool pawnPromotionNeeded(char[8][8]);
void printBoard(char[8][8]);
void printBoard(char[8][8], bool);
void rules();
void savedGames();
bool stillInCheck(char[8][8], string);
bool validMove(char[8][8], string, bool);
bool validString(string);
void whiteCastle(char[8][8], bool, string);
bool whiteCastleMoveIsValid(char[8][8], bool, string);
bool whiteKingIsInCheck(char[8][8], int, int);
bool whiteKingIsInCheckmate(char[8][8], int, int);

int main(){
    menu();
}

//function definitions in alphabetical order
void addGameToSavedGames(vector<string> m) {
    // 

}
void blackCastle(char b[8][8], bool bcc, string move) {
    if (move == "0406") {
        b[0][6] = 'K';
        b[0][5] = 'R';
        b[0][4] = '.';
        b[0][7] = '.';
    }
    else if (move == "0402") {
        b[0][2] = 'K';
        b[0][3] = 'R';
        b[0][0] = '.';
        b[0][4] = '.';
    }
}
bool blackCastleMoveIsValid(char b[8][8], bool bcc, string move) {
    /* return true if black castle move is valid:
    This means there are no pieces on the kings path or enemy
    pieces attacking the path during the move. Also the king
    cannot be in check while making the move */
    if (!bcc) { return false; }
    if (blackKingIsInCheck(b, 0, 4)) {
        return false;
    }
    // check queen side castling and king side castling
    if (move == "0406") {
        //return false if there are pieces in the king's castling path
        string rank = getRank(b, 0);
        auto found = rank.find("K..R");
        if (found == string::npos) {
            return false;
        }
        // create copy of board and place king in each square on castling path and test for check
        char copy[8][8];
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                copy[i][j] = b[i][j];
            }
        }
        makeMove(copy, "0405");
        if (blackKingIsInCheck(copy, 0, 5)) { return false; }
    }
    else if (move == "0402") {
        //return false if there are pieces in the king's castling path
        string rank = getRank(b, 0);
        auto found = rank.find("R...K");
        if (found == string::npos) {
            return false;
        }
        // create copy of board and place king in each square on castling path and test for check
        char copy[8][8];
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                copy[i][j] = b[i][j];
            }
        }
        makeMove(copy, "0403");
        if (blackKingIsInCheck(copy, 0, 3)) { return false; }
    }
    return true;
}
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
    string diag1 = 'K' + getNorthEast(b, r, c);
    string diag2 = 'K' + getNorthWest(b, r, c);
    string diag3 = 'K' + getSouthEast(b, r, c);
    string diag4 = 'K' + getSouthWest(b, r, c);
    string d1{}, d2{}, d3{}, d4{};
    //erase empty spaces and set bishops and queens to 'X'
    // if kX is in the leftover string return true
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
    auto found = d1.find("Kx");
    if (found != string::npos) {
        return true;
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
    auto found2 = d2.find("Kx");
    if (found2 != string::npos) {
        return true;
    }

    for (auto c : diag3) {
        if (c != '.') {
            if (c == 'b' || c == 'q') {
                d3 += 'x';
            }
            else {
                d3 += c;
            }
        }
    }
    auto found3 = d3.find("Kx");
    if (found3 != string::npos) {
        return true;
    }

    for (auto c : diag4) {
        if (c != '.') {
            if (c == 'b' || c == 'q') {
                d4 += 'x';
            }
            else {
                d4 += c;
            }
        }
    }
    auto found4 = d4.find("Kx");
    if (found4 != string::npos) {
        return true;
    }

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
bool blackKingIsInCheckmate(char b[8][8], int bkr, int bkc) {
    return false;
}
string convertString(string t) {
    // convert string 6444 to e4e2 etc
    if (t == "r" || t == "R") {
        return t;
    }
    if(t.size()!=4){
        //return invalid move
        return "9999";
    }
    string s{};
    return s;

}
bool enPassantAttempt(char b[8][8], string m, string pm) {
    /*  returns true if an attempt was made to move a pawn
        diagonally forward onto an empty square from row 3 or 4
        depending on colour
        else returns false
    */
    int row1 = (int)m[0] - 48, col1 = (int)m[1] - 48;
    int row2 = (int)m[2] - 48, col2 = (int)m[3] - 48;
    char temp = b[row1][col1];             //initial position of m
    char temp2 = b[row2][col2];            //final position of m
    int rowpm1 = (int)pm[0] - 48, colpm1 = (int)pm[1] - 48;
    int rowpm2 = (int)pm[2] - 48, colpm2 = (int)pm[3] - 48;

    char temp3 = b[rowpm2][colpm2];            //final position of pm

    // requires previous move to be pawn move forward 2 squares
    if (abs(rowpm2 - rowpm1) != 2) { return false; }
    if (tolower(temp3) != 'p') { return false; }

    if (tolower(temp) == 'p' && temp2 == '.') {
        if (temp == 'p' && row1 == 3 && row2 == 2 && abs(col1 - col2) == 1) { return true; }
        if (temp == 'P' && row1 == 4 && row2 == 5 && abs(col1 - col2) == 1) { return true; }
    }

    return false;
}
vector<int> findBlackKing(char b[8][8]) {
    /* Returns vector of size 2
 vec[0] is the kings row
 and vec[1] is the kings column.
 we search for the king around it's
 starting position  (0,4) */
    if (b[0][4] == 'K') {
        return { 0,4 };
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (b[i][j] == 'K') {
                return { i,j };
            }
        }
    }
    return { -1,-1 };
};
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
void gameReplay(vector<string> m) {
    // input moves vector to watch the game being played with
    // one move made per second
    int row1{}, row2{}, col1{}, col2{};
    char temp, temp2;
    cout << "Game replay!" << endl;
    char b[8][8];           //board
    initialBoard(b);
    printBoard(b);
    // for each move in m, make the move then wait 1500ms
    // then print board
    for (auto move : m) {
        if (move != "r" && move != "R") {
            row1 = (int)move[0] - 48;
            col1 = (int)move[1] - 48;
            row2 = (int)move[2] - 48;
            col2 = (int)move[3] - 48;
            temp = b[row1][col1];              //initial position
            temp2 = b[row2][col2];             //final position

            // special cases 
            // en passant
            if (temp == 'p' && col2!=col1 && temp2 == '.') {
                b[row1][col1] = '.';
                b[row2][col2] = 'p';
                b[3][col2] = '.';
            }
            else if (temp == 'P' && col2 != col1 && temp2 == '.') {
                b[row1][col1] = '.';
                b[row2][col2] = 'P';
                b[5][col2] = '.';
            }
            // pawn promotion
            else if ((temp == 'p' && row2 == 0) || (temp == 'P' && row2 == 7)) {
                b[row1][col1] = '.';
                islower(temp) ? b[row2][col2] = 'q' : b[row2][col2] = 'Q';
                // this needs to be fixed since not all pawn promotions promote to queen
            }
            // castling 
            else if (temp == 'k' && abs(col2 - col1) > 1) {
                b[7][4] = '.';
                b[7][col2] = 'k';
                b[7][(4 + col2) / 2] = 'r';
                if (col2 == 2) {
                    b[7][0] = '.';
                }
                else {
                    b[7][7] = '.';
                }
            }
            else if (temp == 'K' && abs(col2 - col1) > 1) {
                b[0][4] = '.';
                b[0][col2] = 'K';
                b[0][(4 + col2) / 2] = 'R';
                if (col2 == 2) {
                    b[0][0] = '.';
                }
                else {
                    b[0][7] = '.';
                }
            }
            //makemove
            else {
                b[row2][col2] = temp;
                b[row1][col1] = '.';
            }
        }
        else {
            break;      
        }
        Sleep(1500);
        system("cls");
        cout << "Game replay!" << endl;
        printBoard(b);  
    }
    cout << "\nMoves:" << endl;
    cout << m << endl;
}
void gameResult(bool ww, bool bw, bool draw, vector<string> m) {
    if (draw) {
        cout << "Game over! The game was a draw." << endl;
    }
    else if (ww) {
        cout << "Game over! White won." << endl;
    }
    else if (bw) {
        cout << "Game over! Black won." << endl;
    }
    char x;
    cout << "Enter 's' to save game, 'q' to quit, 'r' to replay game or 'n' for new game: ";
    cin >> x;
    if (x == 'r') {
        system("cls");
        gameReplay(m);
        cout << "Enter q to quit to menu, s to save game or n for new game: ";
        cin >> x;
    }
    if (x == 's') {
        //save game
        system("cls");
        cout << "Game saved!" << endl;
        cout << "Enter q to quit to menu, or n for new game: ";
        cin >> x;
    }
    if (x == 'q') {
        std::system("CLS");
        menu();
    }
    else if (x == 'n') {
        std::system("CLS");
        newGame();
    }
    
}
string getEast(char b[8][8], int r, int c) {
    // Starting from (r,c) return squares in the direction d
    // in get(d). This excludes (r,c) itself.

    string ans{};
    if (c == 7) { return ans; }
    for (int j = c + 1; j < 8; ++j) {
        ans += b[r][j];
    }
    return ans;
}
string getFile(char b[8][8], int c) {
    string ans{};
    for (int i = 0; i < 8; ++i) {
        ans += b[i][c];
    }
    return ans;
}
string getNorth(char b[8][8], int r, int c) {
    // Starting from (r,c) return squares in the direction d
    // in get(d). This excludes (r,c) itself.

    string ans{};
    if (r == 0) { return ans; }
    for (int i = r - 1; i > -1; --i) {
        ans += b[i][c];
    }
    return ans;
}
string getNorthEast(char b[8][8], int r, int c) {
    // Starting from (r,c) return squares in the direction d
    // in get(d). This excludes (r,c) itself.

    string ans{};
    if (r == 0 || c == 7) { return ans; }
    // r goes down by 1 and c goes up by 1
    for (struct { int i; int j; } v = { r - 1, c + 1 }; v.i > -1 && v.j < 8; v.i--, v.j++) {
        ans += b[v.i][v.j];
    }
    return ans;
}
string getNorthWest(char b[8][8], int r, int c) {
    // Starting from (r,c) return squares in the direction d
    // in get(d). This excludes (r,c) itself.

    string ans{};
    if (r == 0 || c == 0) { return ans; }
    // r goes down by 1 and c goes down by 1

    for (struct { int i; int j; } v = { r - 1, c - 1 }; v.i > -1 && v.j > -1; v.i--, v.j--) {
        ans += b[v.i][v.j];
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
string getSouth(char b[8][8], int r, int c) {
    // Starting from (r,c) return squares in the direction d
    // in get(d). This excludes (r,c) itself.

    string ans{};
    if (r == 7) { return ans; }
    for (int i = r + 1; i < 8; ++i) {
        ans += b[i][c];
    }
    return ans;
}
string getSouthEast(char b[8][8], int r, int c) {
    // Starting from (r,c) return squares in the direction d
    // in get(d). This excludes (r,c) itself.

    string ans{};
    if (r == 7 || c == 7) { return ans; }
    // r goes up by 1 and c goes up by 1

    for (struct { int i; int j; } v = { r + 1, c + 1 }; v.i < 8 && v.j < 8; v.i++, v.j++) {
        ans += b[v.i][v.j];
    }
    return ans;
}
string getSouthWest(char b[8][8], int r, int c) {
    // Starting from (r,c) return squares in the direction d
    // in get(d). This excludes (r,c) itself.

    string ans{};
    if (r == 7 || c == 0) { return ans; }
    // r goes up by 1 and c goes down by 1

    for (struct { int i; int j; } v = { r + 1, c - 1 }; v.i < 8 && v.j >-1; v.i++, v.j--) {
        ans += b[v.i][v.j];
    }
    return ans;
}
string getWest(char b[8][8], int r, int c) {
    // Starting from (r,c) return squares in the direction d
    // in get(d). This excludes (r,c) itself.

    string ans{};
    if (c == 0) { return ans; }
    for (int j = c - 1; j > -1; --j) {
        ans += b[r][j];
    }
    return ans;
}
void howToPlay() {
    system("cls");
    cout << "How to play:" << endl;
    cout << "\nPieces move according to chess rules. To "<<endl;
    cout << "make a move: enter 4 digits between 0 and 7 "<<endl;
    cout << "where the first digit is a piece's row, the "<<endl;
    cout << "second digit is the piece's column and the "<<endl; 
    cout << "third and fourth digits are the row and " << endl;
    cout << "column of the destination of your move."<<endl;

    cout << "\n\nYour move must be a legal chess move " << endl;
    cout << "otherwise the message \"invalid move\" " << endl;
    cout << "will appear and you'll be prompted to " << endl;
    cout << "choose another move." << endl;
    cout << "\n\nIf you wish to resign make the move \"R\"." << endl;
    cout << "\n\nEnter b to go back to menu: ";

    string s{};
    cin >> s;
    system("cls");
    menu();


}
void initialBoard(char b[8][8]) {
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
bool isCheckmate(char b[8][8], bool turn, int wkr, int wkc, int bkr, int bkc) {
    //wkr- white king row, bkr-black king row etc...
    /* if king is in check and has no legal moves
       find pieces attacking the king if there's 2 or
       more return true isCheckmate
       if there is one piece attacking the king
       see if there's a legal move that captures it
       if not return true else return false
    */

    // check if white(black) is in check on white's(black's) turn
    // then check to see if the king has any legal moves
    // then try capture checking piece with every piece
    if (turn) {
        if (!whiteKingIsInCheck(b, wkr, wkc)) { 
            return false; 
        }  
        if (whiteKingIsInCheckmate(b, wkr, wkc)) {
            return true;
        }
    }
    else {
        if (!blackKingIsInCheck(b, bkr, bkc)) {
            return false;
        }
        if (blackKingIsInCheckmate(b, bkr, bkc)) {
            return true;
        }

    }
    return false;
}
bool isDrawByLackOfMaterial(char b[8][8]) {
    // loop through whole board and store pieces in string
    string pieces{};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (b[i][j] != '.') {
                pieces += b[i][j];
            }
        }
    }

    if (pieces == "kK" || pieces == "Kk") {
        return true;
    }

    string temp{};
    for (auto c : pieces) {
        temp += tolower(c);
    }

    sort(temp.begin(), temp.end());
    if (temp == "bkk" || temp == "kkn") {
        return true;
    }

    return false;
}
bool isStalemate(char b[8][8], bool turn, int wkr, int wkc, int bkr, int bkc) {
    //wkr- white king row, bkr-black king row etc...
    /* if in check return false
    if king has no legal moves
    loop through all pieces of that
    colour. once a move is found return false
    do this for white turn and black turn seperately
    */
    if (turn) {
        if (whiteKingIsInCheck(b, wkr, wkc)) {
            return false;
        }
    }
    else {
        if (blackKingIsInCheck(b, bkr, bkc)) {
            return false;
        }
    }
    return false;
}
bool makeEnPassentMove(char b[8][8], string m, string pm) {
    // returns true if the move was made and false otherwise

    // if move is not an en passant attempt return false
    if (!enPassantAttempt(b, m, pm)) { return false; }

    int row1 = (int)m[0] - 48, col1 = (int)m[1] - 48;
    int row2 = (int)m[2] - 48, col2 = (int)m[3] - 48;
    char temp = b[row1][col1];             //initial position
    char temp2 = b[row2][col2];            //final position
    int rpm1 = (int)pm[0] - 48, cpm1 = (int)pm[1] - 48;
    int rpm2 = (int)pm[2] - 48, cpm2 = (int)pm[3] - 48;
    char temp3 = b[rpm1][cpm1];             //initial position
    char temp4 = b[rpm2][cpm2];            //final position

    // create copy of board and make move on copy
    char copy[8][8];
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            copy[i][j] = b[i][j];
        }
    }

    bool flag = true;
    // make move on copy 
    //if rpm2==3 we know its white's move
    if (rpm2 == 3) {
        copy[3][cpm2] = '.';
        copy[2][cpm2] = 'p';
        copy[row1][col1] = '.';
        // check if this move revealed check
        // find white's king
        auto pos = findWhiteKing(copy);
        if (whiteKingIsInCheck(copy, pos[0], pos[1])) {
            flag = false;
        }
        else {
            // make move on board since it's valid
            b[3][cpm2] = '.';
            b[2][cpm2] = 'p';
            b[row1][col1] = '.';
        }

    }
    //if rpm2==4 we know its black's move
    else if (rpm2 == 4) {
        copy[4][cpm2] = '.';
        copy[5][cpm2] = 'P';
        copy[row1][col1] = '.';
        // check if this move revealed check
        // find black's king
        auto pos = findBlackKing(copy);
        if (blackKingIsInCheck(copy, pos[0], pos[1])) {
            flag = false;
        }
        else {
            // make move on board since it's valid
            b[4][cpm2] = '.';
            b[5][cpm2] = 'P';
            b[row1][col1] = '.';
        }
    }
    return flag;
}
void makeMove(char b[8][8], string s) {
    int itemp1 = (int)s[0] - 48, itemp2 = (int)s[1] - 48;
    int itemp3 = (int)s[2] - 48, itemp4 = (int)s[3] - 48;
    // move piece to new pos and set old square to empty
    char temp = b[itemp1][itemp2];
    b[itemp3][itemp4] = temp;
    b[itemp1][itemp2] = '.';
}
void menu() {
    cout << "Welcome to ChessApp menu" << endl;
    cout << "-Enter n to play new game " << endl;
    cout << "-Enter r to view the rules" << endl;
    cout << "-Enter s to view saved games" << endl;
    cout << "-Enter h to view how to play" << endl;
    cout << "-Enter q to quit" << endl;

    char x;
    cin >> x;
    if (x == 'n') {
        system("cls");
        newGame();
    }
    else if (x == 'r') {
        system("cls");
        rules();
    }
    else if (x == 's') {
        system("cls");
        savedGames();
    }
    else if (x == 'h') {
        system("cls");
        howToPlay();
    }
    else if (x == 'q') {
        system("cls");
        cout << "Thank you for playing! Good bye.";
    }
}
void newGame() {
    bool gameover = false, checkmate = false, stalemate = false;
    bool gameIsDraw = false, whiteWon=false, blackWon=false;
    bool whiteCanCastle = true, blackCanCastle = true;
    bool whiteTurn = true;
    char board[8][8];
    int whiteKingRow = 7;         // position of kings initially
    int whiteKingCol = 4;         // keep track of these 
    int blackKingRow = 0;         // throughout the game
    int blackKingCol = 4;

    vector<string> moves{};       // after making a move store the move string here

    // initialise board and display it
    initialBoard(board);
    printBoard(board, whiteTurn);
    string s{};                 // move inputs stored in string
    string previousMove = "1111"; // stores previous move for en passant logic

    //play game until gameover
    while (!gameover) {
        cout << "Enter your move: ";
        cin >> s;
        // Resign
        if (s == "R"|| s=="r"){
            whiteTurn ? cout << "White resigned. " << endl : cout << "Black resigned. " << endl;
            whiteTurn ? blackWon = true : whiteWon = true;
            // add move to moves
            moves.push_back(s);
            //gameover
            break;
        }
        while (!validString(s)) {
            cout << "invalid move, try another: ";
            cin >> s;
        }
        // Resign
        if (s == "R" || s == "r") {
            whiteTurn ? cout << "White resigned. " << endl : cout << "Black resigned. " << endl;
            whiteTurn ? blackWon = true : whiteWon = true;
            // add move to moves
            moves.push_back(s);
            // gameover
            break;
        }
        if (makeEnPassentMove(board, s, previousMove)) {
            // en passant move was made 
            // flip whiteTurn
            whiteTurn = !whiteTurn;
            //update previous move
            previousMove = s;
            //clear screen and display board again
            std::system("CLS");
            printBoard(board, whiteTurn);
            //if check display check
            if (whiteKingIsInCheck(board, whiteKingRow, whiteKingCol) ||
                blackKingIsInCheck(board, blackKingRow, blackKingCol)) {
                cout << "\nCheck!";
            }
            // add move to moves
            moves.push_back(s);
        }
        else if (stillInCheck(board, s)) {
            cout << "invalid move, try another: ";
        }
        // special moves castling and en passant
        else if ((s == "7476" || s == "7472")) {
            //if move is valid, castle
            if (whiteCastleMoveIsValid(board, whiteCanCastle, s)) {
                whiteCastle(board, whiteCanCastle, s);
                //white can no longer castle
                whiteCanCastle = false;
                // flip whiteTurn
                whiteTurn = false;
                // set previous move
                previousMove = s;
                //update king position
                int whiteKingRow = 7;
                board[7][6] == 'k' ? whiteKingCol = 6 : whiteKingCol = 2;
                // add move to moves
                moves.push_back(s);
                //clear screen and display board again
                std::system("CLS");
                printBoard(board, whiteTurn);
            }
            else { cout << "invalid move, try another: "; }
        }
        else if ((s == "0406" || s == "0402")) {
            //if move is valid, castle
            if (blackCastleMoveIsValid(board, blackCanCastle, s)) {
                blackCastle(board, whiteCanCastle, s);
                //black can no longer castle
                blackCanCastle = false;
                // flip whiteTurn
                whiteTurn = true;
                // set previous move
                previousMove = s;
                //update king position
                int blackKingRow = 0;
                board[0][6] == 'K' ? blackKingCol = 6 : blackKingCol = 2;
                // add move to moves
                moves.push_back(s);
                //clear screen and display board again
                std::system("CLS");
                printBoard(board, whiteTurn);
            }
            else { cout << "invalid move, try another: "; }
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

            // check for pawn promotion CAN BE MORE EFFICIENT
            bool pawnPromo = pawnPromotionNeeded(board);
            if (pawnPromo) {
                board[r][c] = pawnPromotion(board, whiteTurn);
            }
            // flip whiteTurn
            whiteTurn = !whiteTurn;
            //update previous move
            previousMove = s;
            // add move to moves
            moves.push_back(s);
            //clear screen and display board again
            std::system("CLS");
            printBoard(board, whiteTurn);
            //if check display check
            if (whiteKingIsInCheck(board, whiteKingRow, whiteKingCol) ||
                blackKingIsInCheck(board, blackKingRow, blackKingCol)) {
                cout << "\nCheck!";
            }
        }
        else {
            cout << "invalid move, try another: ";
        }
        // check for checkmate or stalemate or draw
        if (isStalemate(board, whiteTurn,whiteKingRow,whiteKingCol,blackKingRow,blackKingCol)) {
            stalemate = true;
        }
        else if (isDrawByLackOfMaterial(board)) {
            gameIsDraw = true;      
        }
        else if (isCheckmate(board, whiteTurn,whiteKingRow,whiteKingCol,blackKingRow,blackKingCol)) {
            checkmate = true;
        }
        if (checkmate || stalemate|| gameIsDraw) {
            gameover = true;
        }
    }

    cout << endl;

    gameResult(whiteWon,blackWon,gameIsDraw,moves);

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
void printBoard(char b[8][8]) {
    cout << "  -----------------------" << endl;
    for (int i = 0; i < 8; ++i) {
        cout << 8-i;
        for (int j = 0; j < 8; ++j) {
            cout << "|" << b[i][j] << " ";
        }
        cout << "|" << endl;
    }
    cout << "  -----------------------" << endl;
    cout << "  A  B  C  D  E  F  G  H " << endl;
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
void rules() {
    system("cls");
    cout << "Welcome to ChessApp's rules section!" << endl;
    cout << "\n All normal chess rules apply including:" << endl;
    cout << "- Castling" << endl;
    cout << "- En Passant" << endl;
    cout << "- Pawn promotion" << endl;
    cout << "- Draw by insufficient material" << endl;
    cout << "- Stalemate" << endl;
    cout << "- Check & Checkmate" << endl;
    cout << "- Draw by threefold repetition(TODO)" << endl;


    cout << "\nEnter b to go back to menu: ";
    string s{};
    cin >> s;
    system("cls");
    menu();
}
void savedGames() {
    cout << "No saved games" << endl;
    cout << "\nEnter b to go back to menu: " << endl;
    string s{};
    cin >> s;
    system("cls");
    menu();
}
bool stillInCheck(char b[8][8], string str) {
    //copy board and make move on the copy board
    char copy[8][8];
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            copy[i][j] = b[i][j];
        }
    }
    makeMove(copy, str);
    //update king position
    //if last move was a king move we know the kings position
    int r = (int)str[2] - 48, c = (int)str[3] - 48;
    char temp = copy[r][c];
    int wkr{}, wkc{}, bkr{}, bkc{};
    if (temp == 'k') {
        wkr = r;
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
bool validMove(char b[8][8], string s, bool turn) {
    int row1 = (int)s[0] - 48, col1 = (int)s[1] - 48;
    int row2 = (int)s[2] - 48, col2 = (int)s[3] - 48;
    char temp = b[row1][col1];              //initial position
    char temp2 = b[row2][col2];             //final position

    // if white moves during black move or vice versa return false
    if (isupper(temp) && turn) { return false; }
    if (islower(temp) && !turn) { return false; }

    // if tries to move onto piece of same colour return false
    if (isupper(temp) && isupper(temp2)) { return false; }
    if (islower(temp) && islower(temp2)) { return false; }

    // if try move empty square return false
    if (temp == '.') { return false; }

    switch (temp) {
    case 'p': {
        if (row1 == row2 + 1 && col1 == col2 && temp2 == '.') { return true; }
        if (row2==4  && row1 == 6 && col1 == col2 && temp2 == '.' && b[row2 + 1][col1] == '.') { return true; }
        if (row1 == row2 + 1 && col1 == col2 + 1 && isupper(temp2)) { return true; }
        if (row1 == row2 + 1 && col1 == col2 - 1 && isupper(temp2)) { return true; }
        return false; }
    case 'P': {
        if (row2 == row1 + 1 && col1 == col2 && temp2 == '.') { return true; }
        if (row2 == 3 && row1 == 1 && col1 == col2 && temp2 == '.' && b[2][col1] == '.') { return true; }
        if (row2 == row1 + 1 && col1 == col2 + 1 && islower(temp2)) { return true; }
        if (row2 == row1 + 1 && col1 == col2 - 1 && islower(temp2)) { return true; }
        return false;
    }
    case 'n': {
        if (row1 + 1 == row2 && col1 + 2 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 + 1 == row2 && col1 - 2 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 - 1 == row2 && col1 + 2 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 - 1 == row2 && col1 - 2 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }  
        if (row1 + 2 == row2 && col1 + 1 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 + 2 == row2 && col1 - 1 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 - 2 == row2 && col1 + 1 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
        if (row1 - 2 == row2 && col1 - 1 == col2 && (temp2 == '.' || isupper(temp2))) { return true; }
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
    case 'b': {
        // moves must be diagonal
        int dif1 = row1 - row2;
        int dif2 = col1 - col2;
        if (abs(dif1) != abs(dif2)) {
            return false;
        }

        // find which direction the move is heading
        string diag{};
        // south east
        if (row2 > row1&& col2 > col1) {
            diag = getSouthEast(b, row1, col1);
        }
        // north east
        else if (row2 < row1 && col2 > col1) {
            diag = getNorthEast(b, row1, col1);
        }
        // south west
        else if (row2 > row1&& col2 < col1) {
            diag = getSouthWest(b, row1, col1);
        }
        // north west
        else if (row2 < row1 && col2 < col1) {
            diag = getNorthWest(b, row1, col1);
        }

        // find index of first non '.'

        // only search non empty strings
        size_t found = diag.find_first_not_of('.', 0);
        if (found == string::npos) {
            return true;
        }

        // can capture so add 1 to found
        if (isupper(diag[found])) {
            //can capture
            found++;
        }
        // found now tells us how far a bishop can move in direction diag
        if (found == 0) {
            return false;
        }
        if (found >= abs(dif1)) {
            return true;
        }

        // if reach here move is invalid
        return false;

    }
    case 'B': {
        // moves must be diagonal
        int dif1 = row1 - row2;
        int dif2 = col1 - col2;
        if (abs(dif1) != abs(dif2)) {
            return false;
        }

        // find which direction the move is heading
        string diag{};
        // south east
        if (row2 > row1&& col2 > col1) {
            diag = getSouthEast(b, row1, col1);
        }
        // north east
        else if (row2 < row1 && col2 > col1) {
            diag = getNorthEast(b, row1, col1);
        }
        // south west
        else if (row2 > row1&& col2 < col1) {
            diag = getSouthWest(b, row1, col1);
        }
        // north west
        else if (row2 < row1 && col2 < col1) {
            diag = getNorthWest(b, row1, col1);
        }

        // find index of first non '.'

        // only search non empty strings
        size_t found = diag.find_first_not_of('.', 0);
        if (found == string::npos) {
            return true;
        }

        // can capture so add 1 to found
        if (islower(diag[found])) {
            //can capture
            found++;
        }
        // found now tells us how far a bishop can move in direction diag
        if (found == 0) {
            return false;
        }
        if (found >= abs(dif1)) {
            return true;
        }

        // if reach here move is invalid
        return false;


    }
    case 'r': {
        // moves must be horizontal or vertical
        int dif1 = abs(row1 - row2);
        int dif2 = abs(col1 - col2);
        if (dif1 != 0 && dif2 != 0) {
            return false;
        }

        // find which direction the move is heading
        string dir{};
        // east
        if (col2 > col1) {
            dir = getEast(b, row1, col1);
        }
        // west
        else if (col2 < col1) {
            dir = getWest(b, row1, col1);
        }
        // south 
        else if (row2 > row1) {
            dir = getSouth(b, row1, col1);
        }
        // north 
        else if (row2 < row1) {
            dir = getNorth(b, row1, col1);
        }


        // find index of first non '.'

        // only search non empty strings
        size_t found = dir.find_first_not_of('.', 0);
        if (found == string::npos) {
            return true;
        }

        // can capture so add 1 to found
        if (isupper(dir[found])) {
            //can capture
            found++;
        }
        // found now tells us how far a bishop can move in direction diag
        if (found == 0) {
            return false;
        }
        if (found >= abs(dif1)) {
            return true;
        }

        // if reach here move is invalid
        return false;

    }
    case 'R': {
        // moves must be horizontal or diagonal
        int dif1 = abs(row1 - row2);
        int dif2 = abs(col1 - col2);
        if (dif1 != 0 && dif2 != 0) {
            return false;
        }

        // find which direction the move is heading
        string dir{};
        // east
        if (col2 > col1) {
            dir = getEast(b, row1, col1);
        }
        // west
        else if (col2 < col1) {
            dir = getWest(b, row1, col1);
        }
        // south 
        else if (row2 > row1) {
            dir = getSouth(b, row1, col1);
        }
        // north 
        else if (row2 < row1) {
            dir = getNorth(b, row1, col1);
        }

        // find index of first non '.'

        // only search non empty strings
        size_t found = dir.find_first_not_of('.', 0);
        if (found == string::npos) {
            return true;
        }

        // can capture so add 1 to found
        if (islower(dir[found])) {
            //can capture
            found++;
        }
        // found now tells us how far a bishop can move in direction diag
        if (found == 0) {
            return false;
        }
        if (found >= abs(dif1)) {
            return true;
        }

        // if reach here move is invalid
        return false;

    }
    case 'q': {
        // moves must be horizontal, vertical or diagonal
        int dif1 = abs(row1 - row2);
        int dif2 = abs(col1 - col2);
        if (dif1 != 0 && dif2 != 0 && dif1 != dif2) {
            return false;
        }

        // find which direction the move is heading
        string dir{};
        // east
        if (col2 > col1&& row2 == row1) {
            dir = getEast(b, row1, col1);
        }
        // west
        else if (col2 < col1 && row2 == row1) {
            dir = getWest(b, row1, col1);
        }
        // south 
        else if (row2 > row1&& col2 == col1) {
            dir = getSouth(b, row1, col1);
        }
        // north 
        else if (row2 < row1 && col2 == col1) {
            dir = getNorth(b, row1, col1);
        }
        // south east
        if (row2 > row1&& col2 > col1) {
            dir = getSouthEast(b, row1, col1);
        }
        // north east
        else if (row2 < row1 && col2 > col1) {
            dir = getNorthEast(b, row1, col1);
        }
        // south west
        else if (row2 > row1&& col2 < col1) {
            dir = getSouthWest(b, row1, col1);
        }
        // north west
        else if (row2 < row1 && col2 < col1) {
            dir = getNorthWest(b, row1, col1);
        }

        // find index of first non '.'

        // only search non empty strings
        size_t found = dir.find_first_not_of('.', 0);
        if (found == string::npos) {
            return true;
        }

        // can capture so add 1 to found
        if (isupper(dir[found])) {
            //can capture
            found++;
        }
        // found now tells us how far a bishop can move in direction diag
        if (found == 0) {
            return false;
        }
        if (found >= abs(dif1)) {
            return true;
        }

        // if reach here move is invalid
        return false;


    }
    case 'Q': {
        // moves must be horizontal, vertical or diagonal
        int dif1 = abs(row1 - row2);
        int dif2 = abs(col1 - col2);
        if (dif1 != 0 && dif2 != 0 && dif1 != dif2) {
            return false;
        }


        // find which direction the move is heading
        string dir{};
        // east
        if (col2 > col1&& row2 == row1) {
            dir = getEast(b, row1, col1);
        }
        // west
        else if (col2 < col1 && row2 == row1) {
            dir = getWest(b, row1, col1);
        }
        // south 
        else if (row2 > row1&& col2 == col1) {
            dir = getSouth(b, row1, col1);
        }
        // north 
        else if (row2 < row1 && col2 == col1) {
            dir = getNorth(b, row1, col1);
        }
        // south east
        if (row2 > row1&& col2 > col1) {
            dir = getSouthEast(b, row1, col1);
        }
        // north east
        else if (row2 < row1 && col2 > col1) {
            dir = getNorthEast(b, row1, col1);
        }
        // south west
        else if (row2 > row1&& col2 < col1) {
            dir = getSouthWest(b, row1, col1);
        }
        // north west
        else if (row2 < row1 && col2 < col1) {
            dir = getNorthWest(b, row1, col1);
        }

        // find index of first non '.'

        // only search non empty strings
        size_t found = dir.find_first_not_of('.', 0);
        if (found == string::npos) {
            return true;
        }

        // can capture so add 1 to found
        if (islower(dir[found])) {
            //can capture
            found++;
        }
        // found now tells us how far a bishop can move in direction diag
        if (found == 0) {
            return false;
        }
        if (found >= abs(dif1)) {
            return true;
        }

        // if reach here move is invalid
        return false;

    }
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
bool validString(string s) {
    // Resign
    if (s == "R" || s == "r") {
        return true;
    }
    if (s.length() != 4) { return false; }
    if (!isdigit(s[0])) { return false; }
    if (!isdigit(s[1])) { return false; }
    if (!isdigit(s[2])) { return false; }
    if (!isdigit(s[3])) { return false; }
    if (s[0] == '8' || s[1] == '8' || s[2] == '8' || s[3] == '8') { return false; }
    if (s[0] == '9' || s[1] == '9' || s[2] == '9' || s[3] == '9') { return false; }
    if (s[0] == s[2] && s[1] == s[3]) { return false; } //can't move to self
    return true;
}
void whiteCastle(char b[8][8], bool wcc, string move) {
    if (move == "7476") {
        b[7][6] = 'k';
        b[7][5] = 'r';
        b[7][4] = '.';
        b[7][7] = '.';
    }
    else if (move == "7472") {
        b[7][2] = 'k';
        b[7][3] = 'r';
        b[7][0] = '.';
        b[7][4] = '.';
    }
}
bool whiteCastleMoveIsValid(char b[8][8], bool wcc, string move) {
    /* return true if white castle move is valid:
    This means there are no pieces on the kings path or enemy
    pieces attacking the path during the move. Also the king
    cannot be in check while making the move*/
    if (!wcc) { return false; }
    if (whiteKingIsInCheck(b, 7, 4)) {
        return false;
    }
    // check queen side castling and king side castling
    if (move == "7476") {
        //return false if there are pieces in the king's castling path
        string rank = getRank(b, 7);
        auto found = rank.find("k..r");
        if (found == string::npos) {
            return false;
        }
        // create copy of board and place king in each square on castling path and test for check
        char copy[8][8];
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                copy[i][j] = b[i][j];
            }
        }
        makeMove(copy, "7475");
        if (whiteKingIsInCheck(copy, 7, 5)) { return false; }
    }
    else if (move == "7472") {
        //return false if there are pieces in the king's castling path
        string rank = getRank(b, 7);
        auto found = rank.find("r...k");
        if (found == string::npos) {
            return false;
        }
        // create copy of board and place king in each square on castling path and test for check
        char copy[8][8];
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                copy[i][j] = b[i][j];
            }
        }
        makeMove(copy, "7473");
        if (whiteKingIsInCheck(copy, 7, 3)) { return false; }
    }
    return true;
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
    if (r >= 1 && b[r - 1][c] == 'K') { return true; }
    if (r >= 1 && c <= 6 && b[r - 1][c + 1] == 'K') { return true; }
    if (c <= 6 && b[r][c + 1] == 'K') { return true; }
    if (c >= 1 && b[r][c - 1] == 'K') { return true; }
    if (r <= 6 && c <= 6 && b[r + 1][c + 1] == 'K') { return true; }
    if (r <= 6 && c >= 1 && b[r + 1][c - 1] == 'K') { return true; }
    if (r <= 6 && b[r + 1][c] == 'K') { return true; }

    ////start at king position and look out in every direction

    // Diagonals 
    string diag1 = 'k' + getNorthEast(b, r, c);
    string diag2 = 'k' + getNorthWest(b, r, c);
    string diag3 = 'k' + getSouthEast(b, r, c);
    string diag4 = 'k' + getSouthWest(b, r, c);
    string d1{}, d2{}, d3{}, d4{};
    //erase empty spaces and set bishops and queens to 'X'
    // if kX is in the leftover string return true
    for (auto c : diag1) {
        if (c != '.') {
            if (c == 'B' || c == 'Q') {
                d1 += 'X';
            }
            else {
                d1 += c;
            }
        }
    }
    auto found = d1.find("kX");
    if (found != string::npos) {
        return true;
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
    auto found2 = d2.find("kX");
    if (found2 != string::npos) {
        return true;
    }

    for (auto c : diag3) {
        if (c != '.') {
            if (c == 'B' || c == 'Q') {
                d3 += 'X';
            }
            else {
                d3 += c;
            }
        }
    }
    auto found3 = d3.find("kX");
    if (found3 != string::npos) {
        return true;
    }

    for (auto c : diag4) {
        if (c != '.') {
            if (c == 'B' || c == 'Q') {
                d4 += 'X';
            }
            else {
                d4 += c;
            }
        }
    }
    auto found4 = d4.find("kX");
    if (found4 != string::npos) {
        return true;
    }

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
bool whiteKingIsInCheckmate(char b[8][8], int wkr, int wkc) {
    return false;
}

/*
To do list

define CHECKMATE n stalemate
use whiteking n blackking in checkmate functions

threefold repetition
hashmap of 2d arrays mapping to ints
if it maps to 3 return true

save game feature

convert string function to turn e2e4 into 6444

*/

// overloaded operator definitions
template < class T >
inline std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
    // prints vectors in a nice format to display moves

    int n = 1;
    os << "1. [";
    for (auto ii=v.begin(); ii != v.end(); ++ii)
    {
        if (n > 1) {
            os << n << ". [";
        }
        os << *ii;
        ++ii;

        if (ii != v.end()) {
            os << " " << *ii << "]" << endl;
            ++n;
        }
        else {
            os << " ]" << endl;
            --ii;
        }
    }

    return os;
}