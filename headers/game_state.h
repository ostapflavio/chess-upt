#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>
#include <wchar.h>

/* ─── global flags that other modules inspect ───────────────────── */
extern bool white_king_moved, white_rook_h_moved, white_rook_a_moved;
extern bool black_king_moved, black_rook_h_moved, black_rook_a_moved;

#endif /* GAME_STATE_H */
