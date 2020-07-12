#ifndef _SGK_H_
#define _SGK_H_

//hellooo

#include "widgets.h"
#include "container.h"

/*
 * SGK_Init : Init necessary data for SDL and global variables
 * flags : the SDL Subsystem flags
 * returned value : -1 if fail others if succeed
 */
int SGK_Init(Uint32 flags );

/*
 * SGK_Quit : quit and free all system data
 */
void SGK_Quit();

int SGK_WindowCreate(const char* title, int w, int h, Uint32 WinFlags, Uint32 RenFlags);
void SGK_WindowDestroy();

SDL_Renderer * SGK_GetWindowRenderer();

#endif
