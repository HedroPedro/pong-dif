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

int xdir = -1, yBallDir = 1;

SDL_Rect ball;
bar player;
bar enemy;

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
    player.yDir = 0;
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
    int spdDelta = 120 * delta_time;


    last_frame_time = SDL_GetTicks64();
    if(isOutOfYBounds(&ball))
        yBallDir *= -1;
    if(isOutOfXBounds(&ball))
        setup();
    if(hasColidedWithPlayer1() || hasColidedWithPlayer2())
        xdir *= -1;

    SDL_Rect p1YNextPos = {player.rect.x, (player.rect.y+(spdDelta * player.yDir)), 20, 80};        
    SDL_Rect p2YNextPos = {enemy.rect.x, (enemy.rect.y+(spdDelta * yBallDir)), 20, 80};

    if(isOutOfYBounds(&p1YNextPos)){
        player.yDir = 0;
    }

    if(!isOutOfYBounds(&p2YNextPos))
        enemy.rect.y += (spdDelta) * yBallDir;
    else
        enemy.rect.y = enemy.rect.y;
    
    ball.x += (spdDelta) * xdir;
    ball.y += (spdDelta) * yBallDir;
    player.rect.y += spdDelta * player.yDir;
     
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
            player.yDir = -1;
            break;    
        }else if(event.key.keysym.sym == SDLK_s){
            player.yDir = 1;
            break;
        }else{
            player.yDir = 0;
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
    if((rect->y+rect->h) > HEIGHT-1 || rect->y < 0){
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