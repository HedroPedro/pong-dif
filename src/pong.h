#include <SDL2/SDL.h>

typedef struct bar{
    SDL_Rect rect;
    int yDir;
}bar;

int initialize_window(void);

void render(void);

void setup(void);

void update(void);

int isOutOfXBounds(SDL_Rect *rect);
int isOutOfYBounds(SDL_Rect *rect);

void process_input(float *delta_time);

void destroy_window(void);

int hasColidedWithPlayer2(void);

int hasColidedWithPlayer1(void);
