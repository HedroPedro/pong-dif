build:
	gcc -Wall -Wextra -std=c99 -L ./src/lib -I ./src/include ./src/game.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_TTF -o ./game 

run:
	./game

clean:
	rm game