#include <stdbool.h>
#include <wchar.h>

// #include <../headers/game_loop.h>

// compile all files in one 
bool white_king_moved = false, white_rook_h_moved = false, white_rook_a_moved = false;
bool black_king_moved = false, black_rook_h_moved = false,  black_rook_a_moved = false;


const int MAX_HISTORY  = 1024; 


/* 50-move counter (half-moves) */
int halfmove_clock = 0;

/* repetition log */
wchar_t* history[1024];
int      history_len = 0;

