#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include "functions.c"

APP game;

// constantes de cores

enum colorIndex{ WHITE, BLACK, RED, GREEN, BLUE };

const SDL_Color colorPallete[] = {
    [WHITE] = {255, 255, 255 , 255},
    [BLACK] = {0, 0, 0 , 255},
    [RED] = {255, 0, 0 , 255},
    [GREEN] = {0, 255, 0 , 255},
    [BLUE] = {0, 0, 255 , 255}
};

// adiciona nó na lista e retorna o novo nó
NODE* addNewNode(NODE* node_list){
        NODE* new_node;
        new_node = malloc(sizeof(NODE));
        new_node->next = node_list;
        node_list =  new_node;
        return new_node;
        
}

int main(int argc, char* argv[]){
   
    // configurando aplicação
    if(initApp(&game) > 0 )
    {
        printf("\n Fechando aplicação");
        return 1;
    }
    

    // inicialização do SDL TTF (fontes true type)
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("./assets/digital_display_tfb.ttf", 40);

    //SDL_Color colorWhite = {255, 255, 255};
    
    SDL_Surface* tempSurface;
    SDL_Surface* screenSurface;

    screenSurface = SDL_CreateRGBSurface(0, RENDER_WIDTH, RENDER_HEIGHT, 32, 0 , 0, 0 , 0 );

    SDL_Texture* textureScreen;

    SDL_Rect rectTexts;
    SDL_Rect rectScreen = { 0, 0, RENDER_WIDTH, RENDER_HEIGHT};


    // configura personagens
    NODE* node_list = NULL;
    NODE* newNode;
    NODE* current_node;
    
    char *textInterface =  malloc(sizeof(char)*1000);

    int countNodes = 0 ;
    
    SDL_Event event;

    bool running = true;
    Uint32 lastTicket, endTicket, beginTicket, frameCount=0, currentFPS;

     // contador de ciclos, e duração de um ciclo em milisegundos
    Uint32 timerCount = 0, timerValue = 1000;
    
    
    float newPosX = 0.0f, newPosY = 0.0f;
    float velX = 0.0f, velY = 1.5f;
    
    float tileSize = 10;
    
    //int colorRed, colorGrenn, colorBlue;
    float randomVel;

    lastTicket = SDL_GetTicks();

    SDL_Rect rectNode;
    
    //loop princpial
    while(running){
        //start fps ticker
        beginTicket = SDL_GetTicks();

        // HANDLE EVENTS
        // Loop no poll(lista) de eventos 
        while(SDL_PollEvent(&event) != 0){
            // verifica sair
            if(event.type == SDL_QUIT){
                running = false;
            }

            // verifica click do mouse
            if( event.type == SDL_MOUSEBUTTONDOWN)
            {
            
            //cria novo objeto a cada click
            newNode = malloc(sizeof(NODE));
            newNode->count = countNodes;
            newNode->node.posX = tileSize + ( rand() % RENDER_WIDTH-(tileSize*2));
            newNode->node.posY = 10;

            newNode->node.color.r =  64;   
            newNode->node.color.g =  64;   
            newNode->node.color.b =  128 + rand() % 128; 
            newNode->node.size = tileSize;  

            newNode->next = node_list;
            node_list = newNode;         

            countNodes++;
                
            }

            // verifica se há tecla pressionada
            if(event.type == SDL_KEYDOWN){

                // tecla cima pressionada
                if (event.key.keysym.sym == SDLK_UP) {
                    // aumenta gravidade
                    velY += 0.1f;
           
                } 
                
                // tecla baixo pressionada
                if (event.key.keysym.sym == SDLK_DOWN) {
                    // diminui gravidade
                    velY -= 0.1f;
           
                }
            }

        }
        
        // CRIA UM NOVO OBJETO A CADA CICLO DO TIMER(1 segundo)
        if(timerCount > 0){
            
            newNode = malloc(sizeof(NODE));
            newNode->count = countNodes;
            newNode->node.posX = tileSize + ( rand() % RENDER_WIDTH-(tileSize*2));
            newNode->node.posY = 10;

            newNode->node.color.r =  64;   
            newNode->node.color.g =  64;   
            newNode->node.color.b =  128 + rand() % 128; 
            newNode->node.size = tileSize;  

            newNode->next = node_list;
            node_list = newNode;         

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
            newPosX = current_node->node.posX + (velX * randomVel);
            newPosY = current_node->node.posY + (velY * randomVel);

            // checa colisão do nó
            if(newPosY < RENDER_HEIGHT ){
                current_node->node.posY = newPosY;
            }

            current_node->node.posX = newPosX;

            current_node = current_node->next;
        }   
           
        // DRAW GAME    
        // loop para desenhar os objetos
        
        // 1º limpa a tela
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);    
        SDL_FillRect(screenSurface, &rectScreen, SDL_MapRGB(screenSurface->format, 0,0,0));
        SDL_RenderClear(game.renderer);

        // 2º loop que desenha cada objeto presente no poll de objetos
        current_node = node_list;

        while(current_node != NULL){  
                         
            rectNode.h = newNode->node.size;
            rectNode.w = newNode->node.size;
            rectNode.x = current_node->node.posX-(tileSize/2);
            rectNode.y = current_node->node.posY-(tileSize/2);

            SDL_FillRect( 
                screenSurface, 
                &rectNode, 
                SDL_MapRGB(
                    screenSurface->format, 
                    current_node->node.color.r,
                    current_node->node.color.g,
                    current_node->node.color.b));

            current_node = current_node->next;
        }   

        // desenha interface de textos

        
        
        
        sprintf(textInterface, "Quantidade de Objetos %d", countNodes);
        
        tempSurface = TTF_RenderText_Solid(font, textInterface, colorPallete[WHITE]);
        
        rectTexts.w = tempSurface->w;
        rectTexts.h = tempSurface->h;
        rectTexts.x = 10;
        rectTexts.y = 10;
        
        SDL_BlitSurface(tempSurface, NULL, screenSurface, &rectTexts);
        SDL_FreeSurface(tempSurface);
 
        sprintf(textInterface, "FPS %d", currentFPS);
        tempSurface = TTF_RenderText_Solid(font, textInterface, colorPallete[WHITE]);
    
        rectTexts.w = tempSurface->w;
        rectTexts.h = tempSurface->h;
        rectTexts.x = 500;
        rectTexts.y = 10;

    
        SDL_BlitSurface(tempSurface, NULL, screenSurface, &rectTexts);
        SDL_FreeSurface(tempSurface);
    
        textureScreen = SDL_CreateTextureFromSurface(game.renderer, screenSurface);

        SDL_RenderCopy(game.renderer, textureScreen, NULL, &rectScreen);

        // desenha o render na tela
        SDL_RenderPresent( game.renderer);
        
        SDL_DestroyTexture(textureScreen);

        // CONTROLA O CLOCK (FPS) DO JOGO
        // controla a quantidade de fps e exibe
        endTicket = SDL_GetTicks();
        
        // VERIFICA SE JOGO ESTÁ ACIMA DE 60 FPS
        if( beginTicket - endTicket < (1000/60) ){
            // Delay para manter os FPS em 60
            SDL_Delay((1000/60) - ( beginTicket - endTicket ) );
        }        
        
        // VERIFICA SE TRANSCORREU O TEMPO DE UM CICLO (cicleDuration)
        if( ( endTicket - lastTicket ) > ( timerValue ) ){              
            //printf("\nFPS %d - OBJETOS NA LISTA: %d", (int)( frameCount * ( 1000/timerValue )), countNodes);
            
            // acrescenta o contador de segundos
            timerCount++;            

            // inicia novo ciclo de 1 segundo
            lastTicket = SDL_GetTicks();
            currentFPS =  frameCount;

            // reseta contador de frames
            frameCount = 0 ;
        }
        
        // aumentar contador de frames
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
  
    SDL_DestroyTexture(textureScreen);
    SDL_FreeSurface(tempSurface);
    SDL_FreeSurface(screenSurface);
    TTF_CloseFont(font);
    TTF_Quit();
  
    SDL_DestroyRenderer( game.renderer);
    SDL_DestroyWindow( game.window);
    SDL_Quit();

    return 0;


}