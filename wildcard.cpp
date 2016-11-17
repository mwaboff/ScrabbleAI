#include <iostream>
#include "game.h"
#include <time.h>
using namespace std;

Game::Game(){
    root = new Trie();
    root->LoadDictionary();
    
    for(int x = 0; x < 15; x++){
        for (int y = 0; y < 15; y++){
            HFindValid(x, y);
            VFindValid(x, y);
        }
    }
}

Game::~Game(){
    delete root;
}

// ====================== N A M I N G   R U L E S ========================= //
// ======================================================================== //
// ============ Any function preceeded with a H is horizontal ============= //
// ============= Any function preceeded with a V is vertical ============== //
// =============== Any function preceeded with an E is empty ============== //
// ============== Any function preceeded with a W is wildcard ============= //
// ============= Use handString when a function needs the rack ============ //
// ========= Use string_so_far for whenever you need to hold chars ======== //
// === Connected means the word is touching letters already on the board == //
// ========== Addition means that words were played from the rack ========= //
// ====== Middle means the word goes through the middle of the board ====== //
// ======================================================================== //
// ======================================================================== //


// FindValid checks the board, and finds all tiles on board that could possibly
// have a solution, saves all to valid_horiz and valid_vert boolean arrays
void Game::HFindValid(int x, int y){
    int handSize = board.hand.length();
    if(x > 0){
        if (IsLetter(x-1, y)){
            valid_horiz[y][x] = false;
            return;
        }
    }
    int counter = 0;
    int offset = x;
    while(counter < handSize+1 && offset < 15){
        if(IsLetter(offset, y)){
            valid_horiz[y][x] = true;
            return;
        }
        if(VConnected(offset, y)){
            valid_horiz[y][x] = true;
            return;
        }
        counter++;
        offset++;
    }
    valid_horiz[y][x] = false;
    return;
}

void Game::VFindValid(int x, int y){
    int handSize = board.hand.length();
    if(y > 0){
        if (IsLetter(x, y-1)){
            valid_vert[y][x] = false;
            return;
        }
    }
    int counter = 0;
    int offset = y;
    while(counter < handSize+1 && offset < 15){
        if(IsLetter(x, offset)){
            valid_vert[y][x] = true;
            return;
        }
        if(HConnected(x, offset)){
            valid_vert[y][x] = true;
            return;
        }
        counter++;
        offset++;
    }
    valid_vert[y][x] = false;
    return;
}

void Game::Print(){
    board.Print();
    highest_points.print();
}

// ======================= C A L C U L A T E   P O I N T S ======================= //
// Points helper looks at tile on board and awards points, edits points and multiplier
void Game::PointsHelper(int x, int y, char letter, int &points_so_far, int &multiplier){
    char holder = board.board[y][x];
    switch(holder){
        case '2': points_so_far += (weights.Get(letter) * 2);
        break;
        case '3': points_so_far += (weights.Get(letter) * 3);
        break;
        case '4': multiplier *= 2;
        points_so_far += weights.Get(letter);
        break;
        case '9': multiplier *= 3;
        points_so_far += weights.Get(letter);
        break;
        default: points_so_far += weights.Get(letter);
        break;
    }
    return;
}

// NewWord awards points for all of the extra words created
void Game::HNewWord(int i, int y, char c, int &extra_points){
    if(IsLetter(i, y)){
        return;
    }
    if(!VConnected(i, y)) return;
    int tempY = y-1;
    int multiplier = 1;
    PointsHelper(i, y, c, extra_points, multiplier);
    if(tempY >= 0){
        while (IsLetter(i, tempY) && tempY >= 0){
            PointsHelper(i, tempY, board.board[tempY][i], extra_points, multiplier);
            tempY--;
        }
    }
    tempY = y+1;
    if(tempY <= 14){
        while(IsLetter(i, tempY) && tempY <= 14){
            PointsHelper(i, tempY, board.board[tempY][i], extra_points, multiplier);
            tempY++;
        }
    }
    extra_points *= multiplier;
    
}

void Game::VNewWord(int x, int i, char c, int &extra_points){
    if(IsLetter(x, i)){
        return;
    }
    if(!HConnected(x, i)) return;
    int tempX = x-1;
    int multiplier = 1;
    PointsHelper(x, i, c, extra_points, multiplier);
    if(tempX >= 0){
        while (IsLetter(tempX, i) && tempX >= 0){
            PointsHelper(tempX, i, board.board[i][tempX], extra_points, multiplier);
            tempX--;
        }
    }
    tempX = x + 1;
    if(tempX <= 14){
        while(IsLetter(tempX, i) && tempX <= 14){
            PointsHelper(tempX, i, board.board[i][tempX], extra_points, multiplier);
            tempX++;
        }
    }
    extra_points *= multiplier;
}

// Main point counter function, needs work
int Game::CountPoints(int x, int y, string word, bool horizontal, string hand){
    int points_so_far = 0;
    int multiplier = 1;
    int extra_points = 0;
    if(horizontal){
       for(int i = x; i < x + word.length(); ++i){
           PointsHelper(i, y, word[i-x], points_so_far, multiplier);
       }
       points_so_far *= multiplier;
       for(int i = x; i < x + word.length(); i++){
           HNewWord(i, y, word[i-x], extra_points);
           points_so_far += extra_points;
           extra_points = 0;
       }
    } else {
        for(int i = y; i < y + word.length(); ++i){
            PointsHelper(x, i, word[i-y], points_so_far, multiplier);
        }
        points_so_far *= multiplier;
        for(int i = y; i < y + word.length(); i++){
            VNewWord(x, i, word[i-y], extra_points);
            points_so_far += extra_points;
            extra_points = 0;
        }
    }
    if(hand.length() <= 0){
        return points_so_far + 50;
    } else {
        return points_so_far;
    }
}

// ================== L E G A L I T Y   C H E C K ============================ //

// CheckHelper looks at a tile on board and a string_so_far and checks if
// that tile is legal or not
bool Game::CheckHelper(int x, int y, string &temp){
    if(IsLetter(x, y)){
        temp += board.board[y][x];
        return true;
    } else if (temp.length() > 1){
        if (root->hasWord(temp, 0)){
            temp = "";
            return true;
        } else {
            return false;
        }
    } else {
        temp = "";
        return true;
    }
}

// Is the column at (x, y) given char c legal?
bool Game::VCheck(int x, int y, char c){
    int tempY = 0;
    char charHolder = board.board[y][x];
    board.board[y][x] = c;
    string temp = "";
    for(; tempY < 15; tempY++){
        if(!CheckHelper(x, tempY, temp)){
            board.board[y][x] = charHolder;
            return false;
        }
    }
    board.board[y][x] = charHolder;
    if(temp.length() > 1){
        return root->hasWord(temp, 0);
    } else return true;
}

// Is the row at (x, y) given char c legal?
bool Game::HCheck(int x, int y, char c){
    int tempX = 0;
    char charHolder = board.board[y][x];
    board.board[y][x] = c;
    string temp = "";
    for(; tempX <15; tempX++){
        if(!CheckHelper(tempX, y, temp)){
            board.board[y][x] = charHolder;
            return false;
        }
    }
    board.board[y][x] = charHolder;
    if(temp.length() > 1){
        return root->hasWord(temp, 0);
    } else return true;
}

// Is the column at (x, y) legal?
bool Game::VCheckNoChar(int x){
    int tempY = 0;
    string temp = "";
    for(; tempY < 15; tempY++){
        if(!CheckHelper(x, tempY, temp)){
            return false;
        }
    }
    if(temp.length() > 1){
        return root->hasWord(temp, 0);
    } else return true;
}

// Is the row at (x, y) legal?
bool Game::HCheckNoChar(int y){
    int tempX = 0;
    string temp = "";
    for(; tempX < 15; tempX++){
        if(!CheckHelper(tempX, y, temp)){
            return false;
        }
    }
    if(temp.length() > 1){
        return root->hasWord(temp, 0);
    } else return true;
}

// Is the row with given string legal?
bool Game::HCheckString(int x, int y, string c){
    int tempX = 0;
    int maxX = x + c.length();
    string temp = "";
    for(; tempX < x; tempX++){
        if(!CheckHelper(tempX, y, temp)){;
            return false;
        }
    }
    temp += c;
    for(tempX = maxX; tempX < 15; tempX++){
        if(!CheckHelper(tempX, y, temp)){;
            return false;
        }
    }
    if(temp.length() > 1){
        return root->hasWord(temp, 0);
    } else return true;
}

// Is the column with given string legal?
bool Game::VCheckString(int x, int y, string c){
    int tempY = 0;
    int maxY = y + c.length();
    string temp = "";
    for(; tempY < y; tempY++){
        if(!CheckHelper(x, tempY, temp)){;
            return false;
        }
    }
    temp += c;
    for(tempY = maxY; tempY < 15; tempY++){
        if(!CheckHelper(x, tempY, temp)){;
            return false;
        }
    }
    if(temp.length() > 1){
        return root->hasWord(temp, 0);
    } else return true;
}

// Is the tile at (x, y) and alphabetical letter
bool Game::IsLetter(int x, int y){
    return ((board.board[y][x] & 64) > 0);
}

// Is the tile touching a letter to above of below it?
bool Game::VConnected(int x, int y){
    if (y==0) return IsLetter(x, y+1);
    if (y==14) return IsLetter(x, y-1);
    return (IsLetter(x, y-1) || IsLetter(x, y+1));
}

// Is the tile touching a letter to the right or left of it?
bool Game::HConnected(int x, int y){
    if(x==0) return IsLetter(x+1, y);
    if(x==14) return IsLetter(x-1, y);
    return (IsLetter(x-1, y) || IsLetter(x+1, y));
}

// ================ A L G O R I T H M ================================= //

// Main function, gets everything started
void Game::MakeWords(){
    if(board.Empty()){
        int x, y;
        y = 7;
        for(x = 1; x <= 7; x++){
            HEHelper(x, y, "", board.hand, false, root, '-');
        }
        
        x = 7;
        for(y = 1; y <= 7; y++){
            VEHelper(x, y, "", board.hand, false, root, '-');
        }
 
        
    } else {
        for(int x = 0; x < 15; x++){
            for(int y = 0; y < 15; y++){
                if(valid_horiz[y][x]) HHelper(x, y, board.hand, "", false, root, false, '-');
                if(valid_vert[y][x]) VHelper(x, y, board.hand, "", false, root, false, '-');
            }
        }
    }
}

// Checks all possible horizontal words at position (x, y)
void Game::HHelper(int x, int y, string handString,
                            string string_so_far, bool connected, Trie* trie, bool addition, char wildcard){
    // When to stop
    if(x + string_so_far.length() > 14){
        return;
    }

    // Grab the character at the spot in question on the board
    char boardChar = board.board[y][x + string_so_far.length()];

    // Check if that character is a letter or not
    if(IsLetter(x + string_so_far.length(), y)){
        string_so_far += boardChar;
        if(HFoundLetter(boardChar, trie, string_so_far, x, y, addition, handString, wildcard)){
            HHelper(x, y, handString, string_so_far, true,
                                trie->children[boardChar], addition, wildcard);
        }
    } else {
        if(handString.length() <= 0) return;
        for(unsigned int i = 0; i < handString.length(); i++){
            bool copyConnected = connected;
            string copyHand = handString;
            string copyString_so_far = string_so_far;
            Trie* copyTrie = trie;
            if(copyHand[i] == '*'){
                copyHand.erase(i, 1);
                copyString_so_far += '*';
                HWLetterNotFound(x, y, copyHand, copyString_so_far, copyConnected, copyTrie, wildcard);
                continue;
            }
            if(HLetterNotFound(copyHand, copyString_so_far, copyConnected,
                                    copyTrie, x, y, i, wildcard)){
                HHelper(x, y, copyHand, copyString_so_far, copyConnected, copyTrie, true, wildcard);
            }
        }
    }
}

// For when we found a letter already on the board
bool Game::HFoundLetter(char c, Trie* trie, string string_so_far,
                                    int x, int y, bool addition, string handString, char wildcard){
    if(!trie->hasChild(c)) return false;
    if(trie->children[c]->isEOW && addition && HCheckString(x, y, string_so_far)){
        Instruction temp = Instruction(string_so_far, x, y, true,
                                CountPoints(x, y, string_so_far, true, handString), wildcard);
        highest_points.insert(temp);
    }
    return true;
}


// For when we encounter a blank space
bool Game::HLetterNotFound(string &handString, string &string_so_far,
                    bool &connected, Trie* &trie, int x, int y, int i, char wildcard){
    char charFromHand = handString[i];
    if(!trie->hasChild(charFromHand)){
        return false;
    } else {
        string_so_far += charFromHand;
        handString.erase(i, 1);
        trie = trie->children[charFromHand];
        if(VConnected(x + string_so_far.length() - 1, y)) connected = true;
        if(!VCheck(x + string_so_far.length() - 1, y, charFromHand)) return false;
        if(trie->isEOW && connected && HCheckString(x, y, string_so_far)){
            Instruction temp = Instruction(string_so_far, x, y, true, CountPoints(x, y, string_so_far, true, handString), wildcard);
            highest_points.insert(temp);
        }
        return true;
    }
}

// Make all possible vertical words at (x, y)
void Game::VHelper(int x, int y, string handString,
                            string string_so_far, bool connected, Trie* trie, bool addition, char wildcard){
    // When to stop
    if(y + string_so_far.length() > 14){
        return;
    }

    // Grab the character at the spot in question on the board
    char boardChar = board.board[y + string_so_far.length()][x];

    // Check if that character is a letter or not
    if(IsLetter(x, y + string_so_far.length())){
        string_so_far += boardChar;
        if(VFoundLetter(boardChar, trie, string_so_far, x, y, addition, handString, wildcard)){
            VHelper(x, y, handString, string_so_far, true,
                                trie->children[boardChar], addition, wildcard);
        }
    } else {
        if(handString.length() <= 0) return;
        for(unsigned int i = 0; i < handString.length(); i++){
            bool copyConnected = connected; // Edit connected without chaning it outside the loop
            string copyHand = handString;
            string copyString_so_far = string_so_far;
            Trie* copyTrie = trie;
            if(copyHand[i] == '*'){
                copyHand.erase(i, 1);
                copyString_so_far += '*';
                VWLetterNotFound(x, y, copyHand, copyString_so_far, copyConnected, copyTrie, wildcard);
                continue;
            }
            if(VLetterNotFound(copyHand, copyString_so_far, copyConnected,
                                    copyTrie, x, y, i, wildcard)){
                VHelper(x, y, copyHand, copyString_so_far, copyConnected, copyTrie, true, wildcard);
            }
        }
    }
}

// When we find a letter on the board
bool Game::VFoundLetter(char c, Trie* trie, string string_so_far,
                                    int x, int y, bool addition, string hand, char wildcard){
    if(!trie->hasChild(c)) return false;
    if(trie->children[c]->isEOW && VCheckString(x, y, string_so_far) && addition){
        Instruction temp = Instruction(string_so_far, x, y, false,
                                CountPoints(x, y, string_so_far, false, hand), wildcard);
        highest_points.insert(temp);
    }
    return true;
}

// When there is an empty space on the board
bool Game::VLetterNotFound(string &handString, string &string_so_far,
                    bool &connected, Trie* &trie, int x, int y, int i, char wildcard){
    char charFromHand = handString[i];

    if(!trie->hasChild(charFromHand)){
        return false;
    } else {
        string_so_far += charFromHand;
        handString.erase(i, 1);
        trie = trie->children[charFromHand];
        if(HConnected(x, y + string_so_far.length() - 1)) connected = true;
        if(!HCheck(x, y + string_so_far.length() - 1, charFromHand)) return false;
        if(trie->isEOW && connected && VCheckString(x, y, string_so_far)){
            Instruction temp = Instruction(string_so_far, x, y, false, CountPoints(x, y, string_so_far, false, handString), wildcard);
            highest_points.insert(temp);
        }
        return true;
    }
}


// ================ WILDCARD BITCHEZ ================================= //


void Game::HWLetterNotFound(int x, int y, string &handString, string &string_so_far, bool &connected, Trie* &trie, char wildcard){
    for(map<char, Trie*>::iterator it = trie->children.begin(); it != trie->children.end(); ++it){
        char letter = it->first;
        if(VConnected(x + string_so_far.length() - 1, y)) connected = true;
        if(!VCheck(x + string_so_far.length() - 1, y, letter)) continue;
        if((it->second->isEOW == true) && (HCheckString(x, y, string_so_far)) &&
        connected){
            Instruction temp = Instruction(string_so_far, x, y, true, CountPoints(x, y, string_so_far, true, handString), letter);
            highest_points.insert(temp);
        }
        HHelper(x, y, handString, string_so_far, connected, it->second, true, letter);
    }
}   



void Game::VWLetterNotFound(int x, int y, string &handString, string &string_so_far, bool &connected, Trie* &trie, char wildcard){
    for(map<char, Trie*>::iterator it = trie->children.begin(); it != trie->children.end(); it++){
        char letter = it->first;
        if(HConnected(x, y + string_so_far.length() - 1)) connected = true;
        if(!HCheck(x, y + string_so_far.length() - 1, letter)) continue;
        if((it->second->isEOW == true) && (VCheckString(x, y, string_so_far)) &&
        connected){
            Instruction temp = Instruction(string_so_far, x, y, true, CountPoints(x, y, string_so_far, false, handString), letter);
            highest_points.insert(temp);
        }
        VHelper(x, y, handString, string_so_far, connected, it->second, true, letter);
    }
}    

// ===================== E M P T Y   B O A R D ============================== //

void Game::HEHelper(int x, int y, string string_so_far, string handString, bool middle, Trie *trie, char wildcard){
   if(handString == "") return;
   if(x + string_so_far.length() == 7 && y == 7) middle = true;
 
   for(int i = 0; i < handString.length(); i++){
       char c = handString[i];
       if(!trie->hasChild(c)) continue;
       string copyHand = handString;
       string copyString_so_far = string_so_far;
       bool copyMiddle = middle;
       copyString_so_far += c;
       copyHand.erase(i, 1);
       Trie* copyTrie = trie;
       if(c == '*'){
           HEWHelper(x, y, copyString_so_far, copyHand, copyMiddle, copyTrie, wildcard);
           continue;
       }
       copyTrie = copyTrie->children[c];
       if(copyMiddle && copyTrie->isEOW){
           Instruction temp = Instruction(copyString_so_far, x, y, true, CountPoints(x, y, string_so_far, false, copyHand), wildcard);
            highest_points.insert(temp);
       }
       HEHelper(x, y, copyString_so_far, copyHand, copyMiddle, copyTrie, wildcard);
   }
}

void Game::VEHelper(int x, int y, string string_so_far, string handString, bool middle, Trie *trie, char wildcard){
    if(handString == "") return;
    if(x == 7 && y + string_so_far.length() == 7) middle = true;
    for(int i = 0; i < handString.length(); i++){
        char c = handString[i];
        if(!trie->hasChild(c)) continue;
       string copyHand = handString;
       string copyString_so_far = string_so_far;
       bool copyMiddle = middle;
       copyString_so_far += c;
       copyHand.erase(i, 1);
       Trie* copyTrie = trie;
       if(c == '*'){
           VEWHelper(x, y, copyString_so_far, copyHand, copyMiddle, copyTrie, wildcard);
           continue;
       }
       copyTrie = copyTrie->children[c];
       if(copyMiddle && copyTrie->isEOW){
           Instruction temp = Instruction(copyString_so_far, x, y, false, CountPoints(x, y, string_so_far, false, copyHand), wildcard);
            highest_points.insert(temp);
       }
       VEHelper(x, y, copyString_so_far, copyHand, copyMiddle, copyTrie, wildcard);
    }
}

void Game::HEWHelper(int x, int y, string string_so_far, string handString, bool middle, Trie* trie, char wildcard){
    for(map<char, Trie*>::iterator it = trie->children.begin(); it != trie->children.end(); ++it){
        char letter = it->first;
        if(x + string_so_far.length() - 1 == 7 && y == 7) middle = true;
        if(it->second->isEOW && middle){
            Instruction temp = Instruction(string_so_far, x, y, true, CountPoints(x, y, string_so_far, true, handString), letter);
            highest_points.insert(temp);
        }
        HEHelper(x, y, string_so_far, handString, middle, it->second, letter);
    }
}   

void Game::VEWHelper(int x, int y, string string_so_far, string handString, bool middle, Trie* trie, char wildcard){
    for(map<char, Trie*>::iterator it = trie->children.begin(); it != trie->children.end(); it++){
        char letter = it->first;
        if(x == 7 && y + string_so_far.length() - 1 == 7) middle = true;
        if(it->second->isEOW && middle){
            Instruction temp = Instruction(string_so_far, x, y, false, CountPoints(x, y, string_so_far, false, handString), letter);
            highest_points.insert(temp);
        }
        VEHelper(x, y, string_so_far, handString, middle, it->second, letter);
    }
}    

int main(){
    clock_t t1,t2;
    t1=clock();
    Game game;
    game.MakeWords();
    game.Print();
    t2=clock();
    float diff ((float)t2-(float)t1);
    cout << "The program took " << diff/CLOCKS_PER_SEC << " seconds to run." << endl;
    return 0;
}










