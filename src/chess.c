// in game logic
// -- Validity of moves 
// -- current pieces 
// -- the state of the game 


#include "../headers/chess.h"

bool are_valid_coordinates(wchar_t* move) {
    if(!move || wcslen(move) != 5 || move[2] != L'-') {
        fprintf(stderr, "ERROR (are_valid_coordinates()): we have incorrect coordinates!\n"); 
        return false; 
    }

    int l1 = move[0] - 'a';
    int d1 = move[1] - '1';

    int l2 = move[3] - 'a'; 
    int d2 = move[4] - '1'; 
    if(l1 == l2 && d1 == d2)  {
        fprintf(stderr, "ERROR (are_valid_coordinates()): we cannot move to the same square!\n"); 
        return false; 
    }

    bool first_part  = (0 <= l1 && l1 < 8) && (0 <= d1 && d1 < 8); 
    bool second_part = (0 <= l2 && l2 < 8) && (0 <= d2 && d2 < 8);  
    return first_part && second_part; 
}

bool is_diagonal(wchar_t* move) {
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_diagonal() \n"); 
        return false; 
    }

    int distx = abs(move[0] - move[3]);
    int disty = abs(move[1] - move[4]);
    
    return (distx == disty); 
}

bool is_horizontal(wchar_t* move) {
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_horizontal() \n"); 
        return false; 
    }

    int distx = abs(move[0] - move[3]); 
    int disty = abs(move[1] - move[4]);

    return (disty == 0); 
}

bool is_vertical(wchar_t* move) {
     if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_vertical() \n"); 
        return false; 
    }

    int distx = abs(move[0] - move[3]); 
    int disty = abs(move[1] - move[4]);

    return (distx == 0); 
}

bool is_pawn_move(wchar_t* move, int color) {
    // TODO: need to implement capture mechanism in order to test all possible moves of a pawn
    // TODO: need to implement en passant as a valid move for a pawn
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_pawn_move() \n");
        return false; 
    }

    bool valid_distance = false, valid_direction = false;  
    int distx = 0, disty = 0; 
    distx = move[0] - move[3]; 
    disty = move[4] - move[1]; 
    if(color == BLACK) {
        if(abs(disty) >= 3) {
            valid_distance = false; 
        }
        else if(disty == -2) {
            valid_distance = ((move[1] - L'1') == 6 ? true : false); 
        }
        else if(disty == -1) {
            valid_distance = true; 
        }
    }
    else if(color == WHITE) {
        // here we have 
        if(abs(disty) >= 3) {
            valid_distance = false; 
        }
        else if(disty == 2) {
            valid_distance = ((move[1] - L'1')== 1 ? true : false); 
        }
        else if(disty == 1) {
            valid_distance = true; 
        }
    }
    valid_direction = (!is_horizontal(move) && is_vertical(move)); 
    return valid_direction && valid_distance; 
}

bool is_knight_move(wchar_t* move) {
    int distx = abs(move[0] - move[3]); 
    int disty = abs(move[1] - move[4]); 
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_knight_move() \n");
        return false; 
    }
    
    return ((distx == 1 && disty == 2) || (distx == 2 && disty == 1)); 
}

bool is_king_move(wchar_t* move) {
    // TODO: we need to implement check mechanism for correct king movement  
}
