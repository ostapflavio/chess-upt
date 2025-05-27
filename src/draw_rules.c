#include "draw_rules.h"
#include "chess_helper.h"     /* is_check, side_has_escape           */
#include <wchar.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 1024      /* enlarge if you need deeper search   */

/* ─── module-wide state ───────────────────────────────────────────*/
static int      halfmove_clock;           /* 50-move counter        */
static wchar_t *history[MAX_HISTORY];     /* repetition keys        */
static int      history_len;

/* ─── internal helpers ───────────────────────────────────────────*/

/* Build a unique key: 64 squares + side + castling flags + NUL.   */
static wchar_t *position_key(const wchar_t **B,
                             int side_to_move,
                             unsigned flags_csl)            /* KQkq */
{
    /* 64 + 1 + up to 4 + 1 = 70 … allocate generously            */
    wchar_t *s = malloc(sizeof(wchar_t) * 80);
    wchar_t *p = s;

    /* board in A8 … H1 order                                     */
    for (int r = 7; r >= 0; --r)
        for (int c = 0; c < 8; ++c)
            *p++ = B[r][c];

    /* side to move (w/b)                                          */
    *p++ = side_to_move ? L'b' : L'w';

    /* castling flags                                              */
    if (!flags_csl)
        *p++ = L'-';
    else {
        if (flags_csl & CASTLE_WHITE_K) *p++ = L'K';
        if (flags_csl & CASTLE_WHITE_Q) *p++ = L'Q';
        if (flags_csl & CASTLE_BLACK_K) *p++ = L'k';
        if (flags_csl & CASTLE_BLACK_Q) *p++ = L'q';
    }

    *p = L'\0';
    return s;
}

static void free_history(void)
{
    for (int i = 0; i < history_len; ++i)
        free(history[i]);
    history_len = 0;
}

/*──────────────────────────────────────────────────────────────────*
 *  PUBLIC IMPLEMENTATION
 *──────────────────────────────────────────────────────────────────*/

void draw_reset_game(void)
{
    free_history();
    halfmove_clock = 0;
}

void draw_update_after_move(bool          pawn_moved,
                            bool          capture_made,
                            const wchar_t **board_after,
                            int           side_to_move,
                            unsigned      castling_flags)
{
    /* 50-move counter */
    halfmove_clock = (pawn_moved || capture_made)
                   ? 0
                   : halfmove_clock + 1;

    /* three-fold repetition log */
    if (history_len < MAX_HISTORY)
        history[history_len++] =
            position_key(board_after, side_to_move, castling_flags);
}

bool is_50move_draw(void)
{
    return halfmove_clock >= 100;   /* 100 half-moves = 50 moves */
}

bool is_threefold_draw(void)
{
    if (history_len < 3) return false;

    const wchar_t *last = history[history_len - 1];
    int count = 0;

    for (int i = 0; i < history_len; ++i)
        if (wcscmp(last, history[i]) == 0 && ++count == 3)
            return true;

    return false;
}

/* prototype lives in chess_helper.h */
static bool side_has_escape(const wchar_t **board, int color);

bool is_stalemate(const wchar_t **board_after, int side_to_move)
{
    if (is_check(board_after, side_to_move)) return false;
    return !side_has_escape(board_after, side_to_move);
}