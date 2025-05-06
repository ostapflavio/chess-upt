#include "../header/chess_symbols.h"
#include "../header/chess_helper.h"


bool is_knight_move(wchar_t* move, wchar_t** board, int color) {
    int distx = abs(move[0] - move[3]); 
    int disty = abs(move[1] - move[4]); 
    int final_pos[2] = {move[3] - 'a', move[4] - '1'}; 
    if(!are_valid_coordinates(move)) {
        fprintf(stderr, "source: is_knight_move() \n");
        return false; 
    }
    bool clear_pass = false; 
    bool distance_pass = ((distx == 1 && disty == 2) || (distx == 2 && disty == 1)); 
    int enemy_color = (color == WHITE ? BLACK : WHITE);
    /*
    if(board[final_pos[0]][final_pos[1]] == empty || get_color(board[final_pos[0]][final_pos[1]]) == enemy_color) {
        clear_pass = true; 
    }
    */

    wchar_t piece = (color == WHITE ? white_knight : black_knight);
    if(!is_collision(move, board, color, piece)) {
        clear_pass = true;  
    }

    return distance_pass && clear_pass; 
}

