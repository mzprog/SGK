#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SGK.h"
//global msg 
//global widget contorl

typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
    
    SGK_Container * mainContaioner;
    char * ErrorMessage;
    
} SGK_DATA_MAIN;


SGK_DATA_MAIN * MainPrivateData = NULL;

int SGK_Init(Uint32 flags )
{
    if(SDL_Init(flags) < 0)
	{
		puts("INIT ERROR");//TODO Set error message
		return -1;
	}
	
    if(TTF_Init()==-1)
    {
        printf("Can't initailize ttf\n");
        return -1;
    }
    
    MainPrivateData = (SGK_DATA_MAIN *) calloc(1,sizeof(SGK_DATA_MAIN));
    if(MainPrivateData == NULL)
    {
        return -1;
    }
    
    return 1;
}

void SGK_Quit()
{
    //free inside elements
    SGK_WindowDestroy();
    
    
    TTF_Quit();
    SDL_Quit();
}

int SGK_WindowCreate(const char* title, int w, int h, Uint32 WinFlags, Uint32 RenFlags)
{
    if(MainPrivateData == NULL)
    {
        return 0;
    }
    MainPrivateData->window =SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,WinFlags);
    if(MainPrivateData->window==NULL)
    {
        return 0;
    }
    
    MainPrivateData->renderer = SDL_CreateRenderer(MainPrivateData->window,-1,RenFlags);
    if(MainPrivateData->renderer==NULL)
    {
        return 0;
    }
    
    return 1;
}

void SGK_WindowDestroy()
{
    if(MainPrivateData)
    {
        if(MainPrivateData->renderer)
        {
            SDL_DestroyRenderer(MainPrivateData->renderer);
        }
        
        if(MainPrivateData->window)
        {
            SDL_DestroyWindow(MainPrivateData->window);
        }
        
        free(MainPrivateData);
        MainPrivateData = NULL;
    }
}


SDL_Renderer * SGK_GetWindowRenderer()
{
    return MainPrivateData->renderer;
}
