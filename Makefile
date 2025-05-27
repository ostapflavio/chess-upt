# makefile to build the whole project
CC       := gcc 
CFLAGS   := -Wall -Iheaders
TARGET   := single_player 

SRCS     := src/single_player.c \
			src/chess_symbols.c \
			src/chess_helper.c \
			src/game_state.c \
			src/board.c


$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
