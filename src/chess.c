// in game logic
// -- Validity of moves 
// -- current pieces 
// -- the state of the game 


#include "../headers/chess.h"

bool are_valid_coordinates(wchar_t* move) {
    if(!move || wcslen(move) != 5 || move[2] != L'-') {
        return false; 
    }

    int l1 = move[0] - 'a';
    int d1 = move[1] - '1';

    int l2 = move[3] - 'a'; 
    int d2 = move[4] - '1'; 
    
    bool first_part  = (0 <= l1 && l1 < 8) && (0 <= d1 && d1 < 8); 
    bool second_part = (0 <= l2 && l2 < 8) && (0 <= d2 && d2 < 8);  
    return first_part && second_part; 
}

