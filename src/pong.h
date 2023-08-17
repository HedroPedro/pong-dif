#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./constant.h"

#define YCOLISSION(A, B) (A->y >= B->y-A->h && A->y <= (B->y+B->h))

typedef enum{MENU, RUNNING, PAUSED} GAME_STATE;

typedef struct bar{
    SDL_Rect rect;
    int yDir;
}bar;

SDL_Window *window = NULL;
SDL_Renderer *renderer =  NULL;

SDL_Rect ball;
bar player;
bar enemy;

int initialize_window(void);

void render(void);

void setup(void);

void update(void);

int isOutOfXBounds(SDL_Rect *rect);
int isOutOfYBounds(SDL_Rect *rect);

void process_input(void);

void destroy_window(void);

int hasColidedWithBar(SDL_Rect *ball, SDL_Rect *bar);
