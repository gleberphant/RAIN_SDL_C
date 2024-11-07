

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "variables.h"




int main(int arc, char* argv[]){

    APP game;

    // Inicializa o gerador de números aleatórios com o tempo atual
    srand(time(NULL));

    // inicialização do SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("não foi possível iniciar o SDL");
        return 1;
    }

    //criação da janela
    SDL_Window* window = SDL_CreateWindow(
        "titulo",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        RENDER_WIDTH, RENDER_HEIGHT, 
        0
    );

    if(window == NULL){
        printf("Erro inesperado");
        SDL_Quit();
        return 1;
    }

    //criar renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if(renderer == NULL){
        printf("Erro inesperado na criação do renderer");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

 
  

    
    // configura personagens

    NODE* node_list = NULL;
    NODE* new_node;
    NODE* current_node;
    const int INIT_NODES = 0;
    int count_nodes = 0 ;
    
    // CRIA OS NÓS
    while(count_nodes < INIT_NODES ){
        new_node = malloc(sizeof(NODE));
        new_node->count = count_nodes;
        new_node->next = node_list;
        node_list = new_node;
        count_nodes++;
    }
  

    // INICIALIZA OS NÓS
    current_node = node_list;

    while(current_node != NULL){

        current_node->node.posX = current_node->count * (INIT_NODES);
        current_node->node.posY = 10;
        current_node->node.color.r =  156 + rand() % 100;   
        current_node->node.color.g =  128 + rand() % 128;   
        current_node->node.color.b =  50;   

        printf("\n iniciando nó %d", current_node->count);

        current_node = current_node->next;
    }
    
    printf("\n");
    SDL_Event event;

    bool running = true;

    Uint32 fpsTicket, endTicket, beginTicket, frameCount=0;
    
    
    float velX = 0.0f, velY = 1.0f;
    float tileSize = 10;
    int colorRed,colorGrenn,colorBlue;
    float randomVel;

    fpsTicket = SDL_GetTicks();

    //loop princpial
    while(running){
        //start fps ticker
        beginTicket = SDL_GetTicks();
        frameCount++;

        // DRAW BACKGROUND
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);    
        SDL_RenderClear(renderer);

        // DRAW PLAYERS
        current_node = node_list;

        while(current_node != NULL){  
            
            colorRed =  current_node->node.color.r;          
            colorGrenn =  current_node->node.color.g;
            colorBlue =  current_node->node.color.b;

            SDL_SetRenderDrawColor(renderer, colorRed,colorGrenn,colorBlue,255);    
            
            SDL_Rect tile_rect;
    
            tile_rect.h = tileSize;
            tile_rect.w = tileSize;
            tile_rect.x = current_node->node.posX-(tileSize/2);
            tile_rect.y = current_node->node.posY-(tileSize/2);

            
            SDL_RenderFillRect(renderer, &tile_rect);

            current_node = current_node->next;
        }   


        SDL_RenderPresent(renderer);

        // UPDATE STATUS
        current_node = node_list;
           
        while(current_node != NULL){
            randomVel = (rand() % 101 ) / 100.0f;           

            current_node->node.posX += velX ;
            current_node->node.posY += velY * randomVel;

            if(current_node->node.posY > RENDER_HEIGHT ){
                current_node->node.posY -= velY * randomVel;
            }

            current_node = current_node->next;
        }   
        


        // HANDLE EVENTS
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                running = false;
            }
        }

        //controle os frames
        endTicket = SDL_GetTicks();

        if( beginTicket - endTicket < (1000/60) ){
            SDL_Delay((1000/60) - ( beginTicket - endTicket ) );
        }


        //MEDE O FPS
        //  a cada 1 segundo imprime o contador de FPS e depois zera
        if( ( endTicket - fpsTicket) > 1000 ){            
            
            // cria novo no a cada segundo

            new_node = malloc(sizeof(NODE));
            new_node->count = count_nodes;
            new_node->node.posX = tileSize + ( rand() % RENDER_WIDTH-40);
            new_node->node.posY = 10;
            new_node->node.color.r =  156 + rand() % 100;   
            new_node->node.color.g =  128 + rand() % 128;   
            new_node->node.color.b =  50;   

            new_node->next = node_list;
            node_list = new_node;         

            count_nodes++;
            printf("\nFPS %d", frameCount);
            fpsTicket = SDL_GetTicks();
            frameCount = 0 ;
        }

       //printf("\n ticker %d", startTicket);



    }

    // fechando aplicação e limpando recursos
    current_node = node_list;
    while( current_node != NULL ){
        
        node_list = current_node->next;     
        
        printf("\n limpando char list %p, posX %d e posY", current_node, current_node->node.posX);      


        free(current_node);

        current_node = node_list;

  

    }
  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


}