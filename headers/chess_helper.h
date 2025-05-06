// header for chess file
#ifndef __CHESS_H__
#define __CHESS_H__

#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include "chess_symbols.h"

bool are_valid_coordinates(wchar_t* move); 
bool is_diagonal(wchar_t* move); 
bool is_vertical(wchar_t* move); 
bool is_horizontal(wchar_t* move); 
bool bishop_trav(wchar_t* move, wchar_t** board); 
bool rook_trav(wchar_t* move, wchar_t** board);
#endif
