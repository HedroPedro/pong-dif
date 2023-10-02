#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./constant.h"

#define YCOLISSION(A, B) (A->y >= B->y-A->h && A->y <= (B->y+B->h))

typedef enum{MENU=1, PAUSED, P1, P2} GAME_STATE;

typedef struct bar{
    SDL_Rect rect;
    int yDir;
}bar;

int p1Score = 0, p2Score = 0, currentOption = 0;
char playersScore[20];

SDL_Window *window = NULL;
SDL_Renderer *renderer =  NULL;
SDL_Surface *surface = NULL;
SDL_Surface *textSurface = NULL;
TTF_Font *font = NULL;
SDL_Texture *textTexture = NULL;

SDL_Rect ball;
SDL_Color textColor = {0, 0, 0, 0};
bar player;
bar player2;

int initialize_window(void);

void render(void);

void renderText(char *text, SDL_Rect *position);

void initBallAndBars(void);

void setup(void);

void update(void);

int isOutOfXBounds(SDL_Rect *rect);

int isOutOfYBounds(SDL_Rect *rect);

void process_input(void);

void destroy_window(void);

int hasColidedWithBar(SDL_Rect *ball, SDL_Rect *bar);

void menuInput(SDL_Event *event);

void singleInput(SDL_Event *event);

void coopInput(void);

void pausedInput(SDL_Event *event);

void victory(SDL_Rect *rect);