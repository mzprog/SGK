#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <SDL2/SDL.h>

#define SGK_LINEARLAYOUT 1
#define SGK_TABLELAYOUT  2
#define SGK_RELATIVELAYOUT 3

#define SGK_DIRECTION_LTR 0 //left to right
#define SGK_DIRECTION_RTL 1 //right to left
#define SGK_DIRECTION_TTB 2 // top to bottom
#define SGK_DIRECTION_BTT 3 // bottom to top


#define SGK_ALIGNCENTER 1
#define SGK_ALIGNLEFT   2
#define SGK_ALIGNRIGHT  4
#define SGK_ALIGNMIDDLE 8
#define SGK_ALIGNTOP    16
#define SGK_ALIGNBOTTOM 32

#define SGK_SCROLL_VERTICAL 1
#define SGK_SCROLL_HORIZONTAL 2


//wiget types falgs
#define SGK_TYPE_CONTAINER 0
#define SGK_TYPE_BUTTON 1
#define SGK_TYPE_INPUTBOX 2
#define SGK_TYPE_RADIOBOX 3
#define SGK_TYPE_CHECKBOK 4






typedef struct contelem
{
    void * element;
    int type;
    SDL_Rect * rect;
    SDL_Surface ** surface;
    //element controller
    char align;
    //margin
    //padding
    
    
    struct contelem * next;
} MZSDL_ContElements;

typedef struct {
    int direction;
    int used;
} LinearSpacer;

typedef struct{
    int direction;
    int maxWidth;
    int used;
}TableSpacer;


typedef union spacer
{
    LinearSpacer * linear;
    TableSpacer * table;
} Spacer;

typedef struct{
    SDL_Rect rect;
    SDL_Surface * surface;
    SDL_Rect DispRect;
    SDL_Surface * DispSurface;
    SDL_Texture * texture;
    SDL_Renderer * render;
    
    
    
    char layout;//the type of the container layout
    
    char flags;//additional flags data
    
    int elementCount;
    MZSDL_ContElements * elements;
    Spacer  spacer;
    
    
    
} MZSDL_Container;


MZSDL_Container* MZSDL_InitContainer(SDL_Renderer* ren, int layout, SDL_Rect* rect);

MZSDL_ContElements * MZSDL_ContainerAddElement(MZSDL_Container *cont,void * node, int type);
int MZSDL_ContainerBuild(MZSDL_Container * cont);
int SGK_LinearLayoutBuild(MZSDL_Container * cont);

void SGK_DestroyContainer(MZSDL_Container * cont);

int SGK_Events_MouseDown(MZSDL_Container * cont,int x,int y);


#endif 
