#include "game_loop.h"
#include "net.h"
#include "board.h"
#include "chess_helper.h"
#include "draw_rules.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSZ 128
static int parse_move(const char *s){return are_valid_coordinates(s);} /* reuse your helper */

static int is_checkmate(wchar_t **bd,int side){
    return is_check(bd,side) && !side_has_escape(bd,side);
}

/* prompt until the local player enters a *legal* move */
static void get_and_validate_move(char *out, wchar_t **board, int my_colour)
{
    while (1) {
        printf("%s to move > ", my_colour == WHITE ? "White" : "Black");
        fflush(stdout);
        fgets(out, BUFSZ, stdin);
        out[strcspn(out, "\n")] = 0;

        if (!parse_move(out)) {                          /* format bad */
            puts("Bad notation.  Use e2-e4 style.");
            continue;
        }
        wchar_t **tmp = clone_board(board);
        make_move_on_board((wchar_t*)out, tmp);          /* try it    */
        if (is_check(tmp, my_colour)) {                  /* king left in check */
            puts("Illegal: your king would be in check.");
            free_board(tmp);
            continue;
        }
        free_board(tmp);
        break;                                           /* legal!    */
    }
}

int game_loop(int sockfd, int my_colour)
{
    wchar_t **board = create_initial_chess_board();
    draw_reset_game();
    int side_to_move = WHITE;              /* White always starts    */
    char buf[BUFSZ];

    while (1) {
        if (side_to_move == my_colour) {   /* ---------- MY TURN --- */
            get_and_validate_move(buf, board, my_colour);

            make_move_on_board((wchar_t*)buf, board);
            sendall(sockfd, buf, strlen(buf)+1);         /* include NUL */

            if (is_checkmate(board, 1-my_colour) ||
                is_stalemate(board, 1-my_colour) ||
                is_threefold_draw()       ||
                is_50move_draw()) {
                sendall(sockfd,"GAME_OVER",10);
                puts("Game finished!");
                break;
            }
        } else {                           /* ------- OPPONENT ----- */
            if (recvall(sockfd, buf, BUFSZ) <= 0) { puts("Connection lost"); break; }
            if (strcmp(buf,"GAME_OVER")==0) { puts("Game over signal"); break; }

            if (!parse_move(buf)) { puts("Peer sent malformed move!"); break; }
            make_move_on_board((wchar_t*)buf, board);
            printf("Opponent played: %s\n", buf);

            if (is_checkmate(board, my_colour) ||
                is_stalemate(board, my_colour) ||
                is_threefold_draw()            ||
                is_50move_draw()) {
                puts("Game finished!");
                break;
            }
        }
        side_to_move = 1 - side_to_move;
    }

    free_board(board);
    close(sockfd);
    return 0;
}