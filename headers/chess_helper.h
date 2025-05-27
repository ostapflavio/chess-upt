// header for chess file
#ifndef __CHESS_H__
#define __CHESS_H__

#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include "chess_symbols.h"


#define CASTLE_WHITE_K  1
#define CASTLE_WHITE_Q  2
#define CASTLE_BLACK_K  4
#define CASTLE_BLACK_Q  8

extern bool white_king_moved, white_rook_h_moved, white_rook_a_moved;
extern bool black_king_moved, black_rook_h_moved, black_rook_a_moved;
extern const int MAX_HISTORY; 
extern int halfmove_clock, history_len;
extern wchar_t* history[]; 


bool are_valid_coordinates(wchar_t* move); 
bool is_diagonal(wchar_t* move); 
bool is_vertical(wchar_t* move); 
bool is_collision(wchar_t* move, wchar_t** board);
bool king_safe_after_my_move(wchar_t* move, wchar_t** board); 

bool is_horizontal(wchar_t* move); 
bool bishop_trav(wchar_t* move, wchar_t** board); 
bool rook_trav(wchar_t* move, wchar_t** board);
bool queen_trav(wchar_t* move, wchar_t** board); 
bool pawn_trav(wchar_t* move, wchar_t** board); 
bool knight_trav(wchar_t* move, wchar_t** board); 
bool king_trav(wchar_t* move, wchar_t** board);
bool castling_trav(wchar_t* move, 
                    wchar_t** board, 
                    bool white_king_moved, 
                    bool white_rook_h_moved,
                    bool white_rook_a_moved,
                    bool black_king_moved, 
                    bool black_rook_h_moved, 
                    bool black_rook_a_moved);
bool is_checkmate(wchar_t **board, int color); 
bool is_castle_move(wchar_t* move); 
bool square_attacked(wchar_t** board, int x, int y, int attacker_color); 
void pawn_promotion(wchar_t **board, int x, int y); 
bool apply_move_on_board(wchar_t* move, wchar_t** board, char promo_choice); 
void build_move_str(wchar_t* s, int x1, int y1, int x2, int y2); 
bool is_50move_draw(); 
bool is_threefold_draw(); 
bool is_stalemate(wchar_t **board, int side_to_move); 
void update_after_move(bool pawn_moved, bool capture_made, wchar_t** board_after, int side_to_move, unsigned castling_bits); 
void reset_draw_trackers(); 
wchar_t* position_hash(wchar_t** board, int side_to_move, unsigned flags); 
unsigned game_state_current_castling_flags(); 
void game_state_update_castling_on_move(int sx, int sy, int dx, int dy, wchar_t moved, wchar_t taken); 
bool correct_pos(int x, int y, int target_x, int target_y); 

#endif
