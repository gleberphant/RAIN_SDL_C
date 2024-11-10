#include "includes.h"
#include "structures.h"

int initApp(APP* gameParam){
    // inicialização do SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("não foi possível iniciar o SDL");
        return 1;
    }

    //criação da janela
    gameParam->window = SDL_CreateWindow(
        "titulo",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        RENDER_WIDTH, RENDER_HEIGHT, 
        0
    );
    
    if( gameParam->window == NULL){
        printf("Erro inesperado na criação da janela");
        SDL_Quit();
        return 1;
    }

    //criar renderer
    gameParam->renderer = SDL_CreateRenderer(
        gameParam->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if( gameParam->renderer == NULL){
        printf("Erro inesperado na criação do renderer");
        SDL_DestroyWindow(gameParam->window);
        SDL_Quit();
        return 1;
    } 
    
    
    


    // tudo ok retorne 0
    return 0;

}