build:
	gcc -Wall -Wextra -std=c99 -L ./src -L ./src/lib -I ./src -I ./src/include -o ./game ./src/game.c -lmingw32 -lSDL2main -lSDL2

run:
	./game

clean:
	rm game