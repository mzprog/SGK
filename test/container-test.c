#include "../src/sgk.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HIEGHT 720


int main(int argc, char *argv[])
{
    SDL_Window * window= NULL;
    SDL_Renderer * ren=NULL;
    SDL_Event event;
    int x,y;//for mouse postion
    
    //aditional 
    SDL_Texture *texture = NULL;
    SDL_Surface *surf = NULL;
    
    int QUIT=0;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		puts("INIT ERROR");
		return -1;
	}
	
    if(TTF_Init()==-1)
    {
        printf("Can't initailize ttf\n");
    }

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
    
    MZSDL_Button *btn = MZSDL_AddButton(ren,"my Button",18,100,200);
    MZSDL_InputBox * inp = MZSDL_CreateInputBox(ren,"input box",18,200,100,100);
    
    MZSDL_Container * container=MZSDL_InitContainer(ren,SGK_LINEARLAYOUT,NULL);
    
    MZSDL_ContElements * btnElem = MZSDL_ContainerAddElement(container,btn,SGK_TYPE_BUTTON);
    if(btnElem==NULL)
    {
        puts("cant add node btn");
        return -1;
    }
    btnElem->align=SGK_ALIGNLEFT;
    
    if(MZSDL_ContainerAddElement(container,inp,SGK_TYPE_INPUTBOX)==NULL)
    {
        puts("cant add node inp");
        return -1;
    }
    
    if(MZSDL_ContainerBuild(container)==0)
    {
        puts("cant abuild cont");
        return -1;
    }
    
    while(!QUIT)
    {
         if(SDL_WaitEvent(&event))//maybe here
        {
            if(event.type == SDL_QUIT)
            {
                QUIT = 1;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_GetMouseState(&x,&y);
                SGK_Events_MouseDown(container,x,y);
            }
        }
        
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,texture,NULL,NULL);
        SDL_RenderCopy(ren,container->texture,NULL,&container->rect);

        SDL_RenderPresent(ren);
    }

    SGK_DestroyContainer(container);
  
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
   
    
    TTF_Quit();
	SDL_Quit();
    
    return 0;
}
