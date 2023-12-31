#include <stdio.h>
#include <stdlib.h>
#include "./pong.h"

int game_is_running = FALSE;
short gameState, desiredGameState;

int nextXdir, nextYdir;
float speedModifier = 1;

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

    window = SDL_CreateWindow("Ponq-diff", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS);
    if(!window){
        fprintf(stderr, "Erro ao criar uma janela SDL\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        fprintf(stderr, "Erro ao criar um renderizador\n");
        return FALSE;
    }

    font = TTF_OpenFont("src/res/arial.ttf", 36);
    if(font == NULL){
        fprintf(stderr, "Erro ao carregar fonte");
        return FALSE;
    }
    return TRUE;
}

void setup(void){
    gameState = MENU;
    initBallAndBars();
    pBall.xDir = 0, pBall.yDir = 0;
    p1Score = 0, p2Score = 0;
    if(rand() & 1){
        nextXdir = 1;
        nextYdir = -1;
    }else{
        nextXdir = -1;
        nextYdir = 1;
    }
}

void render(void){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if(gameState != MENU){
        sprintf(playersScore, "%d-%d", p1Score, p2Score);
        SDL_RenderFillRect(renderer, &pBall.ball);
        SDL_RenderFillRect(renderer, &player.rect);
        SDL_RenderFillRect(renderer, &player2.rect);
        textSurface = TTF_RenderText_Solid(font, playersScore, textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect currentTextPos = {(WIDTH>>1)-75, 100, 150, 75};
        SDL_RenderCopy(renderer, textTexture, NULL, &currentTextPos);
    }else{
        SDL_Rect currentTextPos = {100, 100, 250, 100};
        renderText("Pong-Diff!", &currentTextPos);

        currentTextPos.w = 200;
        currentTextPos.y = HEIGHT>>1;
        renderText("Singleplayer", &currentTextPos);

        currentTextPos.y += 100;
        renderText("Multiplayer", &currentTextPos);

        currentTextPos.x = 25;
        currentTextPos.y = (HEIGHT>>1) + 100 * currentOption;
        currentTextPos.w = 50;
        renderText(">", &currentTextPos);
    }
    SDL_RenderPresent(renderer);
}

void renderText(char *text, SDL_Rect *position){
    textSurface = TTF_RenderText_Solid(font, text, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, position);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}
    
void initBallAndBars(void){
    pBall.ball.x = WIDTH/2, pBall.ball.y = HEIGHT/2;
    pBall.ball.w = 20, pBall.ball.h = 20;
    player.rect.x = 0, player.rect.y = HEIGHT/2;
    player.rect.w = 20, player.rect.h = 80;
    player2.rect.x = WIDTH-20, player2.rect.y = HEIGHT/2;
    player2.rect.w = 20, player2.rect.h = 80;
    player.yDir = 0, player2.yDir = 0;
    speedModifier = 1;
}

void update(void){
    int last_frame_time = SDL_GetTicks64();
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks64() - last_frame_time);

    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    float delta_time = (SDL_GetTicks64() - last_frame_time) / 1000.0f;

    if(gameState != PAUSED && gameState != MENU){
        int spdDelta = 240 * delta_time;
        SDL_Rect p1YNextPos = {player.rect.x, (player.rect.y+(spdDelta * player.yDir)), 20, 80};        
        SDL_Rect p2YNextPos = {player2.rect.x, ((player2.rect.y+(spdDelta * player2.yDir))), 20, 80};
        SDL_Rect ballNextPos = {pBall.ball.x + (spdDelta) * pBall.xDir, (pBall.ball.y+(spdDelta * pBall.yDir * speedModifier)), 20, 20};

        if(isOutOfYBounds(&p1YNextPos))
            player.yDir = 0;

        if(isOutOfYBounds(&ballNextPos))
            pBall.yDir *=-1; 

        if(SDL_HasIntersection(&ballNextPos, &p1YNextPos) || 
            SDL_HasIntersection(&ballNextPos, &p2YNextPos)){
            pBall.xDir *= -1;
            speedModifier += 0.025f;
        }
        
        if(isOutOfXBounds(&pBall.ball))
            victory(&pBall.ball);
            
        pBall.ball.x += (spdDelta) * pBall.xDir * speedModifier;
        pBall.ball.y += (spdDelta) * pBall.yDir * speedModifier;
        player.rect.y += spdDelta * player.yDir;
        switch (gameState){
            case P1:
                player2.yDir = pBall.yDir;
                if(!isOutOfYBounds(&p2YNextPos))
                    player2.rect.y += spdDelta * player2.yDir;
                break;
        
            case P2:
                if(isOutOfYBounds(&p2YNextPos))
                    player2.yDir  = 0;
                player2.rect.y += spdDelta * player2.yDir;
                break;
            }
    }
}

void process_input(void){
    SDL_Event event;
    SDL_PollEvent(&event);

    if(event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE){
        game_is_running = FALSE;
        return;
    }

    if(event.key.keysym.sym == SDLK_r && gameState != MENU)
        setup();

    switch(gameState){
    case MENU:
        menuInput(&event);
        break;
    
    case PAUSED:
        pausedInput(&event);
        break;

    case P1:
        singleInput(&event);
        break;

    case P2:
        coopInput();
        break;

    default:
        break;
    }
}

void destroy_window(void){
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

int isOutOfXBounds(SDL_Rect *rect){
    return rect->x > WIDTH || rect->x < 0 ? TRUE : FALSE;
}

int isOutOfYBounds(SDL_Rect *rect){
    return (rect->y+rect->h) > HEIGHT || rect->y < 0 ? TRUE : FALSE;
}

void menuInput(SDL_Event *event){
    if(event->type == SDL_KEYDOWN){
        switch(event->key.keysym.sym){
            case SDLK_DOWN:
                if(currentOption < 1)
                    currentOption++;
                else
                    currentOption = 0;
            break;

            case SDLK_UP:
                if(currentOption > 0)
                    currentOption--;
                else
                    currentOption = 1;
                break;

            case SDLK_SPACE:
                gameState = PAUSED;
                switch (currentOption)
                {
                case 0:
                    desiredGameState = P1;
                    break;
                
                case 1:
                    desiredGameState = P2;
                    break;
                }
                break;
        }
    }
}

void pausedInput(SDL_Event *event){
    if(event->type == SDL_KEYDOWN){
        pBall.yDir = nextYdir;
        pBall.xDir = nextXdir;
        gameState = desiredGameState;
    }
}

void singleInput(SDL_Event *event){
    if(event->key.keysym.sym == SDLK_w)
        player.yDir = -1;               
    if(event->key.keysym.sym == SDLK_s)
        player.yDir = 1;               
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

void victory(SDL_Rect *rect){
    if(rect->x > WIDTH)
        p1Score++;
    else
        p2Score++;
    initBallAndBars();
    nextYdir = pBall.yDir*-1;
    nextXdir = pBall.xDir*-1;
    desiredGameState = gameState;
    gameState = PAUSED;
}