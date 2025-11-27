CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
INCLUDE = -Isrc

SRC = src/main.c src/board.c src/fleet.c src/game.c src/io.c src/rnd.c src/config.c
OBJ = $(SRC:.c=.o)

battleship: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: clean run
clean:
	rm -f $(OBJ) battleship

run: battleship
	./battleship
