#include "includes.h"

typedef struct OBJECT OBJECT;
typedef struct COLOR COLOR;
typedef struct NODE NODE;

struct COLOR{
    int r, g, b;
};

struct OBJECT
{
    float posX, posY, sizeWidth, sizeHeight;
    COLOR color;
    Uint32 pixelColor;
    int life;
};

struct NODE{
    int count;
    OBJECT node;
    NODE* next;
};

// adiciona nó na lista e retorna o novo nó
NODE* addNewNode(NODE* node_list){
        NODE* new_node = NULL;
        new_node = malloc(sizeof(NODE));
        new_node->next = node_list;
        node_list =  new_node;
        return new_node;
        
}