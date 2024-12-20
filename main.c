#include "includes.h"
#include "functions.c"
#include "nodes.c"

APP app;

int countNodes = 0 ;
NODE* node_list = NULL;
NODE* newNode = NULL;
NODE* current_node = NULL;


SDL_Surface* tempSurface = NULL;    
SDL_Surface* backgroundIMG = NULL;
SDL_Rect rectTexts;
SDL_Rect tempRect;

char tempString[1000];

bool running = true;

Uint32 lastTicket, endTicket, beginTicket, frameCount=0, currentFPS;
Uint32 timerCount = 0, timerValue = 1000;

float newPosX = 0.0f, newPosY = 0.0f;
float velX = 0.0f, velY = 1.5f;
float tileSize = 10;
float randomVel = 0;

int main(int argc, char* argv[]){
   
    // inicializando aplicação
    printf("\n ___________________________________");
    printf("\n|                                   |");
    if(initApp(&app) > 0 )
    {
        printf("\n Fechando aplicação");
        return 1;
    }
    printf("\n|___________________________________|\n\n");

    // CARREGANDO CENA

    backgroundIMG = IMG_Load("./assets/backgroundForest.png");
  
    
    SDL_Event event;


    lastTicket = SDL_GetTicks();

    
    
    //loop princpial
    while(running){
        //start fps ticker
        beginTicket = SDL_GetTicks();

        // HANDLE EVENTS
        // Loop no poll(lista) de eventos 
        while(SDL_PollEvent(&event) != 0){
            // verifica sair
            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }

            // verifica click do mouse
            if( event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (event.button.x < 0 && event.button.x > RENDER_WIDTH)
                {
                    printf("clicou fora");
                    continue;
                }
                
                //cria novo objeto a cada click
                newNode = malloc(sizeof(NODE));

                newNode->count = countNodes;
                newNode->node.posX = event.button.x;
                newNode->node.posY = 10;

                newNode->node.color.r =  64;   
                newNode->node.color.g =  128;   
                newNode->node.color.b =  128 + rand() % 128; 

                newNode->node.pixelColor = SDL_MapSurfaceRGB(
                        app.bufferScreen, 
                        newNode->node.color.r,
                        newNode->node.color.g,
                        newNode->node.color.b
                );

                newNode->node.sizeWidth = tileSize/2;  
                newNode->node.sizeHeight = tileSize;  

                newNode->next = node_list;
                node_list = newNode;         

                countNodes++;
                
            }

            // verifica se há tecla pressionada
            if(event.type == SDL_EVENT_KEY_DOWN){

                // tecla cima pressionada
                if (event.key.key == SDLK_UP) {
                    // aumenta gravidade
                    velY += 0.1f;
           
                } 
                
                // tecla baixo pressionada
                if (event.key.key == SDLK_DOWN) {
                    // diminui gravidade
                    velY -= 0.1f;
           
                }
            }

        }
        
        // CRIA UM NOVO OBJETO A CADA CICLO DO TIMER(1 segundo)
        if(timerCount > 0){
            
            newNode = malloc(sizeof(NODE));
            newNode->count = countNodes;
            newNode->node.posX = (tileSize*2) + ( rand() % RENDER_WIDTH-(tileSize*3));
            newNode->node.posY = 10;

            newNode->node.color.r =  32;   
            newNode->node.color.g =  64;   
            newNode->node.color.b =  128 + rand() % 128; 
            
            newNode->node.pixelColor = SDL_MapSurfaceRGB(
                app.bufferScreen, 
                newNode->node.color.r,
                newNode->node.color.g,
                newNode->node.color.b
            );
            
            newNode->node.sizeWidth = tileSize/2;  
            newNode->node.sizeHeight = tileSize;    

            newNode->next = node_list;
            node_list = newNode;         

            countNodes++;

            timerCount--;            
        }

        // UPDATE STATUS
        // loop para realizar as ações de cada objeto 
        // os objetos eles ficam em uma linked list

        // mover cada no 
        current_node = node_list;
           
        while(current_node != NULL){
            // define uma velocidade alatoria para movimentar o objeto
            randomVel = (rand() % 100 ) / 100.0f;           
            
            // move o objeto de acordo com a velocidade aleatoria
            // TODO substituir esse tpo de movimento por movimento vetorial
            newPosX = current_node->node.posX + (velX * randomVel);
            newPosY = current_node->node.posY + (velY * randomVel);

            // checa colisão do nó
            if(newPosY  < RENDER_HEIGHT ){
                current_node->node.posY = newPosY;
            }

            current_node->node.posX = newPosX;

            current_node = current_node->next;
        }   
           
        // DRAW GAME    
        // loop para desenhar os objetos
        
        // 1º limpa o buffer da tela
        SDL_FillSurfaceRect(app.bufferScreen, &app.rectScreen, app.pixelBLACK);
        
        // 2º desenha o background
        
        SDL_BlitSurface(backgroundIMG, NULL, app.bufferScreen, &app.rectScreen);

        // 2º loop que desenha cada objeto presente no poll de objetos
        current_node = node_list;

        while(current_node != NULL){  
                         
            tempRect.h = current_node->node.sizeHeight;
            tempRect.w = current_node->node.sizeWidth;
            tempRect.x = current_node->node.posX-(tileSize/2);
            tempRect.y = current_node->node.posY-(tileSize/2);
            
            if(
                tempRect.x > 0 &&
                tempRect.x < app.rectScreen.w &&
                tempRect.y > 0 &&
                tempRect.y < app.rectScreen.h )
            {
                SDL_FillSurfaceRect( 
                    app.bufferScreen, 
                    &tempRect, 
                    current_node->node.pixelColor
                );
            }

            current_node = current_node->next;
        }   

        // desenha interface de textos    
        sprintf(tempString, "Quantidade de Objetos %d", countNodes);
        
        tempSurface = TTF_RenderText_Solid(app.font, tempString, 0, SDL_COLOR_PALLETE[WHITE]); 
        tempRect.w = tempSurface->w;
        tempRect.h = tempSurface->h;
        tempRect.x = 10;
        tempRect.y = 10;
        
        SDL_BlitSurface(tempSurface, NULL, app.bufferScreen, &tempRect);
        SDL_DestroySurface(tempSurface);
 
        sprintf(tempString, "FPS %d", currentFPS);
        tempSurface = TTF_RenderText_Solid(app.font, tempString, 0, SDL_COLOR_PALLETE[WHITE]);
        
        tempRect.w = tempSurface->w;
        tempRect.h = tempSurface->h;
        tempRect.x = 500;
        tempRect.y = 10;

        SDL_BlitSurface(tempSurface, NULL, app.bufferScreen, &tempRect);
        SDL_DestroySurface(tempSurface);
    
        // cria textura com todo desenho da tela
        app.textureScreen = SDL_CreateTextureFromSurface(app.renderer, app.bufferScreen);

        
        // limpa o render e copia a textura da tela para render
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 255, 255);    
        SDL_RenderClear(app.renderer);
        SDL_RenderTexture(app.renderer, app.textureScreen, NULL, NULL);
        SDL_RenderPresent(app.renderer);
        
        // destroyu a textura da tela para liberar a memoria da GPU
        SDL_DestroyTexture(app.textureScreen);

        // CONTROLA O CLOCK (FPS) DO JOGO
        // controla a quantidade de fps e exibe
        endTicket = SDL_GetTicks();
        
        // VERIFICA SE JOGO ESTÁ ACIMA DE 60 FPS
        if( endTicket - beginTicket < (ONE_SEC_IN_MSEC/MAX_FPS) ){
            // Delay para manter os FPS em 60
            SDL_Delay((1000/100) - ( endTicket - beginTicket ) );
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
  
    
    

    SDL_DestroyTexture(app.textureScreen);
    SDL_DestroySurface(backgroundIMG);
    SDL_DestroySurface(app.bufferScreen);
    
    TTF_CloseFont(app.font);
    

    closeApp(&app);
  

    return 0;


}