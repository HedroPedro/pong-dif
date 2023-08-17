#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "./constant.h"
#include "./pong.h"

int game_is_running = FALSE;
int last_frame_time = 0;
float delta_time = 0;

SDL_Window *window = NULL;
SDL_Renderer *renderer =  NULL;

int xdir = -1, ydir = 1;

SDL_Rect ball;
bar player;
bar enemy;

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

int main(int argc, char *argv[]){
    game_is_running = initialize_window();

    setup();

    while(game_is_running){
        process_input(&delta_time);
        render();
        update();
    }   

    return 0;
}

int initialize_window(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Erro ao abrir\n");
        return FALSE;
    }
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS);
    if(!window){
        fprintf(stderr, "Erro ao criar uma janela SDL\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Erro ao criar um renderizador\n");
        return FALSE;
    }

    return TRUE;
}

void setup(void){
    ball.x = WIDTH/2, ball.y = HEIGHT/2;
    ball.w = 20, ball.h = 20;
    player.rect.x = 0, player.rect.y = HEIGHT/2;
    player.rect.w = 20, player.rect.h = 80;
    enemy.rect.x = WIDTH-20, enemy.rect.y = HEIGHT/2;
    enemy.rect.w = 20, enemy.rect.h = 80;
    player.speed = 255;
    enemy.speed = 255;
}

void render(void){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderFillRect(renderer, &player.rect);
    SDL_RenderFillRect(renderer, &enemy.rect);
    SDL_RenderPresent(renderer);
}

void update(void){
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks64() - last_frame_time);

    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    
    delta_time = (SDL_GetTicks64() - last_frame_time) / 1000.0f;
    
    last_frame_time = SDL_GetTicks64();
    if(isOutOfYBounds(&ball))
        ydir *= -1;
    if(hasColidedWithPlayer1() || hasColidedWithPlayer2())
        xdir *= -1;
    
    int spdDelta = 120 * delta_time;
    ball.x += (spdDelta) * xdir;
    ball.y += (spdDelta) * ydir;

    if(isOutOfYBounds(&player.rect))
        player.speed = 0;
    else
        player.speed = 255;

    if(!isOutOfYBounds(&enemy.rect) && !isOutOfYBounds(&ball))
        enemy.rect.y += (spdDelta) * ydir;
    else if(isOutOfYBounds(&ball))
        enemy.rect.y += (spdDelta) * -1 * ydir;
    else
        enemy.rect.y = enemy.rect.y;     
}

void process_input(float *delta_time){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type){
    case SDL_QUIT:
        game_is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE){
            game_is_running = FALSE;
            break;
        }
        if(event.key.keysym.sym == SDLK_w){
            player.rect.y -= player.speed * (*delta_time);
            break;    
        }
        if(event.key.keysym.sym == SDLK_s){
            player.rect.y += player.speed * (*delta_time);
            break;
        }
        if(event.key.keysym.sym == SDLK_r){
            setup();
        }
        break;
    default:
        break;
    }
}

void destroy_window(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int isOutOfXBounds(SDL_Rect *rect){
    if(rect->x > WIDTH || rect->x < 0){
        return TRUE;
    }
    return FALSE;
}

int isOutOfYBounds(SDL_Rect *rect){
    if((rect->y+rect->h) > HEIGHT || rect->y < 0){
        return TRUE;
    }
    return FALSE;
}

//Colisões especificas para cada jogador
int hasColidedWithPlayer2(void){
    return (ball.x+enemy.rect.w > enemy.rect.x) && (ball.y >= enemy.rect.y-ball.h && ball.y <= (enemy.rect.y+enemy.rect.h)) ? TRUE : FALSE;
}

int hasColidedWithPlayer1(void){
    return (ball.x < player.rect.x+player.rect.w) && (ball.y >= player.rect.y-ball.h && ball.y <= (player.rect.y+player.rect.h)) ? TRUE : FALSE;
}