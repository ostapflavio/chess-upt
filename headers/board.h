#ifndef __BOARD_H__
#define __BOARD_H__

wchar_t** create_initial_chess_board(); 
wchar_t** clone_board(wchar_t** src); 
void draw_board_white(wchar_t** board); 
void draw_board_black(wchar_t** board);
void make_move_on_board(wchar_t* move, wchar_t** board);
void free_board(wchar_t** board);
void clear_screen(); 

#endif
