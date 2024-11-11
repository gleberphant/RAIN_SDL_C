#include "includes.h"

#define RENDER_HEIGHT 600
#define RENDER_WIDTH 800
#define MAX_FPS 80
#define ONE_SEC_IN_MSEC 1000

// constantes de cores

typedef struct APP APP;
typedef enum colorPalleteIndex colorPalleteIndex;

enum colorPalleteIndex{ WHITE, BLACK, RED, GREEN, BLUE };

const SDL_Color SDL_COLOR_PALLETE[] = {
    [WHITE] = {255, 255, 255 , 255},
    [BLACK] = {0, 0, 0 , 255},
    [RED] = {255, 0, 0 , 255},
    [GREEN] = {0, 255, 0 , 255},
    [BLUE] = {0, 0, 255 , 255}
};

struct APP
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Surface* bufferScreen;
    SDL_Texture* textureScreen;
    SDL_Rect rectScreen;
    TTF_Font* font;
    Uint32 pixelBLACK;
    
};


int initApp(APP* app){
    
    // inicialização do SDL
    printf("\n|- Inicializando SDL ......... ");

    if(SDL_Init(SDL_INIT_VIDEO) < 0){

        printf("\n >> %s", SDL_GetError());
        return 1;
    }
    printf("[OK] |");

    //criação da janela
    printf("\n|- Criacao de janela ......... ");
   
    app->window = SDL_CreateWindow(
        "RAIN APP",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        RENDER_WIDTH, RENDER_HEIGHT, 
        0
    );
    
    if( app->window == NULL){

        printf("\n >> %s", SDL_GetError());
        SDL_Quit();

        return 1;
    }
    printf("[OK] |");

    //criar renderer
    printf("\n|- Criacao de renderes ....... ");

    app->renderer = SDL_CreateRenderer(
        app->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if( app->renderer == NULL){

        printf("\n Erro inesperado na criação do renderer");
        printf("\n >> %s", SDL_GetError());
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        
        return 1;
    } 
    printf("[OK] |");


    // inicialização do SDL TTF (fontes true type)
    printf("\n|- Inicializando SDL TTF ..... ");

    if(TTF_Init() == -1){

        printf("\n >> %s", SDL_GetError());

        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return 1;

    }

    app->font = TTF_OpenFont("./assets/digital_display_tfb.ttf", 40);

    if(app->font == NULL){
        printf("\n >> %s", SDL_GetError());

        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return 1;
    }

    printf("[OK] |");
    
    printf("\n|- Inicializando SDL Image ... ");
    if(IMG_Init(IMG_INIT_PNG) == 0){
        
        printf("\n >> %s", SDL_GetError());
        
        TTF_Quit();
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return 1;
    };
    printf("[OK] |");

    // inicializando buffer da imagem
    printf("\n|- Criando Buffer Screen ..... ");
    
    app->bufferScreen = SDL_CreateRGBSurface(0, RENDER_WIDTH, RENDER_HEIGHT, 32, 0 , 0, 0 , 0 );
    
    if (app->bufferScreen == NULL){
        printf("\n >> %s", SDL_GetError());
        
        TTF_Quit();
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return 1;
    }

    app->pixelBLACK = SDL_MapRGB(app->bufferScreen->format, 0,0,0);
    
    printf("[OK] |");

    // Definindo retangulo da tela
    printf("\n|- Configurando RECT ......... ");
    app->rectScreen.h = app->bufferScreen->h;
    app->rectScreen.w = app->bufferScreen->h;
    app->rectScreen.x = 0;
    app->rectScreen.y = 0;
    printf("[OK] |");       

    // tudo ok
    printf("\n|- ........................... [OK] |");
    return 0;

}


void closeApp(APP* gameParam){
    
    TTF_Quit();
    SDL_DestroyRenderer( gameParam->renderer);
    SDL_DestroyWindow( gameParam->window);
    SDL_Quit();

}