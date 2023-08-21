#include <stdio.h>
#include <stdlib.h>
#include "./pong.h"

int game_is_running = FALSE;
short gameState = PAUSED, desiredGameState = P2;

int xdir = 0, yBallDir = 0;

int main(int argc, char *argv[]){
    game_is_running = initialize_window();

    setup();

    while(game_is_running){
        process_input();
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

    if(TTF_Init() != 0){
        fprintf(stderr, "Erro ao inicializar as fontes\n");
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

    font = TTF_OpenFont("src/res/comic.ttf", 36);
    if(font == NULL){
        fprintf(stderr, "Erro ao carregar fonte");
        return FALSE;
    }
    return TRUE;
}

void setup(void){
    gameState = PAUSED;
    xdir = 0, yBallDir = 0;
    ball.x = WIDTH/2, ball.y = HEIGHT/2;
    ball.w = 20, ball.h = 20;
    player.rect.x = 0, player.rect.y = HEIGHT/2;
    player.rect.w = 20, player.rect.h = 80;
    player2.rect.x = WIDTH-20, player2.rect.y = HEIGHT/2;
    player2.rect.w = 20, player2.rect.h = 80;
    player.yDir = 0, player2.yDir = 0;
}

void render(void){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if(gameState != MENU){
        SDL_RenderFillRect(renderer, &ball);
        SDL_RenderFillRect(renderer, &player.rect);
        SDL_RenderFillRect(renderer, &player2.rect);
    }else{
        surface = TTF_RenderText_Solid(font, "Ponq-Diff", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect currentPos = {100, 100, 150, 100};
        SDL_RenderCopy(renderer, textTexture, NULL, &currentPos);
        surface = TTF_RenderText_Solid(font, "Iniciar", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, surface);
        currentPos.y = HEIGHT/2;
        SDL_RenderCopy(renderer, textTexture, NULL, &currentPos);
    }
    SDL_RenderPresent(renderer);
}

void update(void){
    int last_frame_time = SDL_GetTicks64();
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks64() - last_frame_time);

    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    float delta_time = (SDL_GetTicks64() - last_frame_time) / 1000.0f;

    if(gameState != PAUSED){
        int spdDelta = 240 * delta_time;
        SDL_Rect p1YNextPos = {player.rect.x, (player.rect.y+(spdDelta * player.yDir)), 20, 80};        
        SDL_Rect p2YNextPos = {player2.rect.x, (player2.rect.y+(spdDelta * yBallDir)), 20, 80};
        SDL_Rect ballNextPos = {ball.x + (spdDelta) * xdir, (ball.y+(spdDelta * yBallDir)), 20, 20};

        if(isOutOfYBounds(&p1YNextPos))
            player.yDir = 0;

        if(isOutOfYBounds(&p2YNextPos))
            player2.yDir = 0;

        if(isOutOfYBounds(&ballNextPos))
            yBallDir *=-1; 

        if((ballNextPos.x < player.rect.x+player.rect.w && hasColidedWithBar(&ballNextPos, &player.rect)) 
        || (ballNextPos.x + player2.rect.w > player2.rect.x && hasColidedWithBar(&ballNextPos, &player2.rect)))
            xdir *= -1;
        if(isOutOfXBounds(&ball))
            setup();
            
        ball.x += (spdDelta) * xdir;
        ball.y += (spdDelta) * yBallDir;
        player.rect.y += spdDelta * player.yDir;
        player2.rect.y += spdDelta * player2.yDir;     
    }
}

void process_input(void){
    SDL_Event event;
    SDL_PollEvent(&event);

    if(event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE){
        game_is_running = FALSE;
        return;
    }

    if(event.key.keysym.sym == SDLK_r)
        setup();

    if(gameState == PAUSED)
        pausedInput(&event);

    if(gameState == P1)
        singleInput(&event);
    if(gameState == P2)
        coopInput();
}


void destroy_window(void){
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
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

//Checa se o y da bolinha está entre a altura da barra desejada
int hasColidedWithBar(SDL_Rect *obj, SDL_Rect *bar){
    return YCOLISSION(obj, bar) ? TRUE : FALSE;
}

void pausedInput(SDL_Event *event){
    if(event->type == SDL_KEYDOWN){
        yBallDir = -1;
        xdir = 1;
        gameState = P2;
    }
}

void singleInput(SDL_Event *event){
    if(event->key.keysym.sym == SDLK_w){
        player.yDir = -1;               
        }
    if(event->key.keysym.sym == SDLK_s){
        player.yDir = 1;               
        }
}

void coopInput(void){
    Uint8 *inputIndex = SDL_GetKeyboardState(NULL);
    if(inputIndex[SDL_SCANCODE_W])
        player.yDir = -1;
    if(inputIndex[SDL_SCANCODE_S])
        player.yDir = 1;

    if(inputIndex[SDL_SCANCODE_UP])
        player2.yDir = -1;
    if(inputIndex[SDL_SCANCODE_DOWN])
        player2.yDir = 1;
}