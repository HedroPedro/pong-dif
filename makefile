CC = gcc
LIB_DIR = .\src\lib
INCLUDE_DIR = .\src\include .\src\game.c
DEFAULT_COMPILER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_TTF -std=c99
GAME_PATH = .\game

build:
	$(CC) -Wall -Wextra -L $(LIB_DIR) -I $(INCLUDE_DIR) $(DEFAULT_COMPILER_FLAGS) -o $(GAME_PATH)

build-release:
	$(CC) -L $(LIB_DIR) -I $(INCLUDE_DIR) $(DEFAULT_COMPILER_FLAGS) -o $(GAME_PATH)

run:
	.\game

clean:
	rm game