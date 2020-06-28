#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../src/mzsdl.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HIEGHT 720

int main(int argc, char ** argv)
{
    
    SDL_Window * window= NULL;
    SDL_Renderer * ren=NULL;
    SDL_Event event;
    
    //aditional 
    SDL_Texture *texture = NULL;
    SDL_Surface *surf = NULL;
    
    int QUIT=0;

	printf("hello sdl\n");
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		puts("INIT ERROR");
		return -1;
	}
	
    if(TTF_Init()==-1)
    {
        printf("Can't initailize ttf\n");
    }


	puts("sdl running");
    
    window =SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HIEGHT,SDL_WINDOW_SHOWN);
    if(window==NULL)
    {
        puts("window init error");
        return -1;
    }
    
    ren = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(ren==NULL)
    {
        puts("ren init error");
        return -1;
    }
    
    
    //create something to view
    
    surf = SDL_CreateRGBSurface(0, 100, 100, 32, rmask, gmask, bmask, amask);
    if(surf==NULL)
    {
        puts("surf init error");
        return -1;
    }
    SDL_FillRect(surf, NULL, 0XFFA0A0FF);
    
    
    texture = SDL_CreateTextureFromSurface(ren,surf);
    if(texture==NULL)
    {
        puts("texture init error");
        return -1;
    }
    
    
    
    
    
    
    
    
    
    
    
    /*
     * REAL WORKS START HERE
     */
    
  
    MZSDL_Button *btn = MZSDL_AddButton(ren,"my Button",18,100,200);
    if(btn == NULL)
    {
        puts("btm init error");
        return -1;
    }
    

 
    /*
     * REAL WORKS ENDS HERE
     */
    
    
    
    
    
    
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren,texture,NULL,NULL);

    SDL_RenderCopy(ren,btn->texture, NULL, NULL);
 
    SDL_RenderPresent(ren);

    
    while(!QUIT)
    {
        if(SDL_WaitEvent(&event))//maybe here
        {
            if(event.type == SDL_QUIT)
            {
                QUIT = 1;
            }
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,texture,NULL,NULL);
        SDL_RenderCopy(ren,btn->texture, NULL, &btn->rect);
        SDL_RenderPresent(ren);
    }
    
    
    

    TTF_Quit();
	SDL_Quit();

	return 0;
}
