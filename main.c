#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "functions.c"

APP game;

// adiciona nó na lista e retorna o novo nó
NODE* addNewNode(NODE* node_list){
        NODE* new_node;
        new_node = malloc(sizeof(NODE));
        new_node->next = node_list;
        node_list =  new_node;
        return new_node;
        
}

int main(int arc, char* argv[]){
   
    // configurando aplicação
    if(initApp(&game) > 0 )
    {
        printf("\n Fechando aplicação");
        return 1;
    }
    
    // configura personagens
    NODE* node_list = NULL;
    NODE* new_node;
    NODE* current_node;
    const int INIT_NODES = 10;
    int countNodes = 0 ;
    
    SDL_Event event;

    bool running = true;
    Uint32 fpsTicket, endTicket, beginTicket, frameCount=0;

    Uint32 timerCount;
    
    
    float newPosX = 0.0f, newPosY = 1.0f;
    float velX = 0.0f, velY = 1.0f;
    float tileSize = 10;
    int colorRed,colorGrenn,colorBlue;
    float randomVel;

    fpsTicket = SDL_GetTicks();

    SDL_Rect tile_rect;
    
    //loop princpial
    while(running){
        //start fps ticker
        beginTicket = SDL_GetTicks();

        // HANDLE EVENTS
        // Loop para checkar o poll de eventos e definir as ações 
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                running = false;
            }

            // cria novo nó a cada click do mouse
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
            new_node = malloc(sizeof(NODE));
            new_node->count = countNodes;
            new_node->node.posX = tileSize + ( rand() % RENDER_WIDTH-(tileSize*2));
            new_node->node.posY = 10;

            new_node->node.color.r =  64;   
            new_node->node.color.g =  64;   
            new_node->node.color.b =  128 + rand() % 128; 
            new_node->node.size = tileSize;  

            new_node->next = node_list;
            node_list = new_node;         

            countNodes++;
                
            }
            // diminui e aumenta velocidade das gotas 
            if(event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_UP) {
                    velY += 0.1f;
           
                } 
                if (event.key.keysym.sym == SDLK_DOWN) {
                    velY -= 0.1f;
           
                }
            }

        }
        
        // cria novo nó a cada ciclo do timer(1 segundo)
        if(timerCount > 0){
            new_node = malloc(sizeof(NODE));
            new_node->count = countNodes;
            new_node->node.posX = tileSize + ( rand() % RENDER_WIDTH-(tileSize*2));
            new_node->node.posY = 10;

            new_node->node.color.r =  64;   
            new_node->node.color.g =  64;   
            new_node->node.color.b =  128 + rand() % 128; 
            new_node->node.size = tileSize;  

            new_node->next = node_list;
            node_list = new_node;         

            countNodes++;

            timerCount--;
        }


        // UPDATE STATUS
        // loop para realizar as ações em cada objeto 
        // os objetos eles ficam em uma linked list

        current_node = node_list;
           
        while(current_node != NULL){
            // define uma velocidade alatoria para movimentar o objeto
            randomVel = (rand() % 101 ) / 100.0f;           
            
            // move o objeto de acordo com a velocidade aleatoria
            // TODO substituir esse tpo de movimento por movimento vetorial
            newPosX = current_node->node.posX + (velX * randomVel );
            newPosY = current_node->node.posY + (velY * randomVel);

            // checa colisão do nó
            if(newPosY < RENDER_HEIGHT ){
                current_node->node.posY = newPosY;
            }

            current_node = current_node->next;
        }   
           
        // DRAW GAME    
        // loop para desenhar os objetos
        
        // 1º limpa a tela
        SDL_SetRenderDrawColor(game.renderer, 0,0,0,255);    
        SDL_RenderClear(game.renderer);

        // 2º loop que desenha cada objeto presente no poll de objetos
        current_node = node_list;

        while(current_node != NULL){  
       
            SDL_SetRenderDrawColor(
                game.renderer, 
                current_node->node.color.r, 
                current_node->node.color.g,
                current_node->node.color.b,
                255);    
                   
            tile_rect.h = new_node->node.size;
            tile_rect.w = new_node->node.size;
            tile_rect.x = current_node->node.posX-(tileSize/2);
            tile_rect.y = current_node->node.posY-(tileSize/2);

            SDL_RenderFillRect( game.renderer, &tile_rect);

            current_node = current_node->next;
        }   

        // desenha o render na tela
        SDL_RenderPresent( game.renderer);


        // CONTROLE DO FPS DO JOGO
        // controla a quantidade de fps e exibe
        endTicket = SDL_GetTicks();
        
        if( beginTicket - endTicket < (1000/60) ){
            SDL_Delay((1000/60) - ( beginTicket - endTicket ) );
        }        
        
        // imprime a quantidade de FPS a cada segundo
        if( ( endTicket - fpsTicket) > 1000 ){              
            printf("\nFPS %d - OBJETOS NA LISTA: %d", frameCount, countNodes);
            
            //contador de tempo, a cada segundo acrescenta 1
            timerCount++;            

            fpsTicket = SDL_GetTicks();
            frameCount = 0 ;


        }
        
        frameCount++;

        
    }

    // ENCERRAMENTO DA APLICAÇÃO
    // fechando aplicação e limpando recursos
    current_node = node_list;
    while( current_node != NULL ){
        
        node_list = current_node->next;     
        
        printf("\n Free Node:  Id [%d] - Address [%p]", current_node->count, current_node);      


        free(current_node);

        current_node = node_list;

    }
  
    SDL_DestroyRenderer( game.renderer);
    SDL_DestroyWindow( game.window);
    SDL_Quit();


}