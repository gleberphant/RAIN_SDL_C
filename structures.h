typedef struct OBJECT OBJECT;
typedef struct COLOR COLOR;
typedef struct NODE NODE;
typedef struct APP APP;

struct COLOR{
    int r, g, b;
};


struct OBJECT
{
    float posX, posY, sizeWidth, sizeHeight;
    COLOR color;
    int life;
};



struct NODE{
    int count;
    OBJECT node;
    NODE* next;
};


struct APP
{
    SDL_Renderer* renderer;
    SDL_Window* window;
};




#define RENDER_HEIGHT 600
#define RENDER_WIDTH 800