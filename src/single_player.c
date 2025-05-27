#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "board.h"
#include "chess_helper.h"
#include "game_state.h"


#define BUFFER_SIZE 128
wchar_t* SIDE[2] = {L"White", L"Black"}; 


void ascii_to_wchar(const char* src, wchar_t* dst) {
    size_t i; 
    for(i = 0; src[i] && i < 6; i++) {
        dst[i] = (wchar_t)src[i];
    }

    dst[i] = L'\0';
}


// strip newline after fgets
void safe_read(char* buf, size_t n) {
    if(!fgets(buf, (int)n, stdin)) { 
        wprintf(L"stdin closed.\n can't read anymore. \n"); 
        exit(1); 
    }
    buf[strcspn(buf, "\n")] = '\0'; 
}

// prompt until the entered move is fully legal 
void get_legal_move(char *ascii_out, wchar_t* wmove_out, wchar_t** board, int side) {
    char tmp[BUFFER_SIZE]; 

    while(1) {
        wprintf(L"%ls to move > ", SIDE[side]); 
        fflush(stdout); 

        safe_read(tmp, sizeof(tmp)); 
        ascii_to_wchar(tmp, wmove_out); 

        if(!are_valid_coordinates(wmove_out)) {
            wprintf(L"bad notation - use algebraic notation (i.e. e2-e4) \n"); 
            continue;
        }
        if(is_collision(wmove_out, board)) {
            wprintf(L"Illegal move for that piece or path blocked. \n"); 
            continue; 
        }

        if(!king_safe_after_my_move(wmove_out, board)) {
            wprintf(L"Illegal: your king would be in check \n"); 
            continue; 
        }
        strcpy(ascii_out, tmp);  // ascii for debug 
        return; 
    }
}

int main() {
    setlocale(LC_ALL, ""); 

    wchar_t **board = create_initial_chess_board(); 
    game_state_current_castling_flags(); 
    reset_draw_trackers(); 

    clear_screen(); 
    draw_board_white(board); 

    int side_to_move = 0; 
    char ascii_move[BUFFER_SIZE]; 
    wchar_t wmove[6]; 

    while(1) {
        get_legal_move(ascii_move, wmove, board, side_to_move); 

        int sx = wmove[0] - L'a', sy = wmove[1] - L'1';
        int dx = wmove[3] - L'a', dy = wmove[4] - L'1'; 

        wchar_t moving_piece = board[sy][sx]; 
        wchar_t captured_piece = board[dy][dx];

        bool pawn_moved   = (moving_piece == white_pawn || moving_piece == black_pawn); 
        bool capture_made =  (captured_piece != empty); 

        bool is_ok = apply_move_on_board(wmove, board, 0); 
        if(!is_ok) {
            fprintf(stderr, "UNPRECEDENTED ERROR HAPPENED! WE CANNOT MAKE TRHIS MOVE!\n"); 
            exit(EXIT_FAILURE);
        }

        game_state_update_castling_on_move(sx, sy, dx, dy, moving_piece, captured_piece );

        update_after_move(pawn_moved, capture_made, board,  1 - side_to_move, game_state_current_castling_flags()); 

        clear_screen(); 
        if(side_to_move == 0) {
            draw_board_black(board);
        }
        else{
            draw_board_white(board);
        }

        if(is_checkmate(board, 1 - side_to_move)) {
            wprintf(L"Checkmate! %ls wins. \n", SIDE[side_to_move]); 
            break; 
        }
        if(is_stalemate(board, 1 - side_to_move)){
            wprintf(L"Stalemate - draw.\n");
            break; 
        }
        if(is_threefold_draw()) {
            wprintf(L"Draw by three-fold repetition. \n"); 
            break; 
        }
        if(is_50move_draw()) {
            wprintf(L"Draw by 50-move rule.\n"); 
            break; 
        }

        side_to_move = 1 - side_to_move; // flip the color;
    }

    free_board(board); 
}