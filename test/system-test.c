#include "../src/SGK.h"

int main(int argc, char ** argv)
{
    SGK_Init(SDL_INIT_VIDEO);
    
    SGK_WindowCreate("My Window",600,600,SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED);
    
    
    //NOTICE varaibles to be removed later
    SDL_Event event;
    char QUIT = 0;
    int x,y;
    
    SDL_Renderer * ren =SGK_GetWindowRenderer();//TODO FIX the appearence of the varaible and hide it into a better api
        
    SGK_Button *btn = SGK_AddButton(ren,"my Button",18,100,200);
    SGK_InputBox * inp = SGK_CreateInputBox(ren,"input box",18,200,100,100);
    
    SGK_Container * container=SGK_InitContainer(ren,SGK_LINEARLAYOUT,NULL);
    
    SGK_ContElements * btnElem = SGK_ContainerAddElement(container,btn,SGK_TYPE_BUTTON);
    if(btnElem==NULL)
    {
        puts("cant add node btn");
        return -1;
    }
    btnElem->align=SGK_ALIGNLEFT;
    
    if(SGK_ContainerAddElement(container,inp,SGK_TYPE_INPUTBOX)==NULL)
    {
        puts("cant add node inp");
        return -1;
    }
    
    if(SGK_ContainerBuild(container)==0)
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
        SDL_RenderCopy(ren,container->texture,NULL,&container->rect);
        SDL_RenderPresent(ren);
    }
    
    SGK_Quit();
    
    return 0;
}
