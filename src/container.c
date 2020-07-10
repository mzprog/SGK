#include "container.h"

#include "mzsdl.h"

#include <stdlib.h>

MZSDL_Container * MZSDL_InitContainer(SDL_Renderer *ren, int layout, SDL_Rect * rect)
{
    MZSDL_Container * cont = (MZSDL_Container *) calloc(1,sizeof(MZSDL_Container));
    if(cont==NULL)
    {
        return NULL;
    }
    
    cont->layout = layout;
    
    if(rect == NULL)
    {
        cont->rect.x = 0;
        cont->rect.y = 0;
        SDL_GetRendererOutputSize(ren, &cont->rect.w, &cont->rect.h);
    }
    else
    {
        cont->rect.w = rect->w;
        cont->rect.h = rect->h;
        cont->rect.x = rect->x;
        cont->rect.y = rect->y;
    }
    cont->render = ren;
    if(layout == SGK_LINEARLAYOUT)
    {
        cont->spacer.linear = (LinearSpacer *)calloc(1,sizeof(LinearSpacer));
        cont->spacer.linear->direction = SGK_DIRECTION_TTB;//top to bottom defualt dirtection
        /*
     * NOTICE calculate better with overflow term and scrolling
     * but for now the overflow is hidden
     */
    }
    
    return cont;
}

SDL_Rect * SGK_PR_GetRect(void * node, int type)//private function to get rect pointer
{
    switch(type)
    {
        case SGK_TYPE_BUTTON:
            return &((MZSDL_Button *)node)->rect;
        case SGK_TYPE_CHECKBOK:
            return &((MZSDL_CheckBox *)node)->rect;
        case SGK_TYPE_INPUTBOX:
            return &((MZSDL_InputBox *)node)->rect;
        case SGK_TYPE_RADIOBOX:
            return &((MZSDL_RadioButton *)node)->rect;
        case SGK_TYPE_CONTAINER:
            return &((MZSDL_Container *)node)->rect;
        default:
            return NULL;
    }
}

SDL_Surface ** SGK_PR_GetSurface(void * node, int type)//private function to get surface pointer
{
    switch(type)
    {
        case SGK_TYPE_BUTTON:
            return &((MZSDL_Button *)node)->surface;
        case SGK_TYPE_CHECKBOK:
            return &((MZSDL_CheckBox *)node)->surface;
        case SGK_TYPE_INPUTBOX:
            return &((MZSDL_InputBox *)node)->surface;
        case SGK_TYPE_RADIOBOX:
            return &((MZSDL_RadioButton *)node)->surface;
        case SGK_TYPE_CONTAINER:
            return &((MZSDL_Container *)node)->surface;
        default:
            return NULL;
    }
}

MZSDL_ContElements *  MZSDL_ContainerAddElement(MZSDL_Container *cont,void * node, int type)
{
    MZSDL_ContElements *cur=NULL;
    MZSDL_ContElements * elem = (MZSDL_ContElements *)calloc(1, sizeof(MZSDL_ContElements));
    if(elem ==NULL)
    {
        return NULL;
    }
    if(node ==NULL)
    {
        return NULL;
    }
    
    elem->element=node;
    elem->type=type;
    elem->align=SGK_ALIGNCENTER | SGK_ALIGNMIDDLE;//by default
    elem->rect=SGK_PR_GetRect(node,type);
    elem->surface = SGK_PR_GetSurface(node,type);
    
    /* TODO
     * check layout and all the setting
     */
    
    if(cont->elements== NULL)
    {
        cont->elements = elem;
    }
    else
    {
        cur=cont->elements;
        
        while(cur->next)
        {
            cur=cur->next;
        }
        
        cur->next = elem;
    }
    
    cont->elementCount++;
    
    return elem;
}

int MZSDL_ContainerBuild(MZSDL_Container * cont)
{
    if(cont->layout==SGK_LINEARLAYOUT)
    {
        if(!SGK_LinearLayoutBuild(cont))
            return 0;
        else 
            return 1;
    }
    return 0;
}


int SGK_LinearLayoutBuild(MZSDL_Container * cont)
{
    int length;
    int DispLength;
    int dw,dh;//display width and hieght
    int direction,sign;
    int lengCurrent;
    SDL_Rect clipRect;
    
    char vertical;//boolean to check if vertical = 1 ,if horizontal = 0
    MZSDL_ContElements *cur;
    
    if(cont->layout != SGK_LINEARLAYOUT)
        return 0;
    
    direction = cont->spacer.linear->direction;
    
    if(direction==SGK_DIRECTION_BTT || direction == SGK_DIRECTION_TTB)
    {
        vertical = 1;
    }
    else if(direction==SGK_DIRECTION_RTL || direction == SGK_DIRECTION_LTR)
    {
        vertical = 0;
    }
    else
    {
        return 0;
    }
    
    if(direction == SGK_DIRECTION_BTT || direction ==SGK_DIRECTION_RTL)
    {
        sign = -1;
    }
    else
    {
        sign = 1;
    }
    
    //count the length
    cur = cont->elements;
    length = 0;
    /*
     * NOTICE calculate better with overflow term and scrolling
     * but for now the overflow is hidden
     */
    if(vertical)
    {
        while(cur)
        {
            length += cur->rect->h;
            cur=cur->next;
        }
        DispLength = length < cont->rect.h?length : cont->rect.h;
        dw=cont->rect.w;
        dh= DispLength;
    }
    else
    {
        while(cur)
        {
            length += cur->rect->w;
            cur=cur->next;
        }
        DispLength = length < cont->rect.w?length : cont->rect.w;
        
        dw= DispLength;
        dh = cont->rect.h;
    }
    
    cont->surface = SDL_CreateRGBSurface(0,dw,dh,32,rmask,gmask,bmask,amask);
    if(cont->surface==NULL)
    {
        return 0;
    }

    
    /*
     * TODO generate an array to check freely the size of each element
     * then distribute  the sizes to fill the surface 
     * and for now work without fill
     */
    
    cur=cont->elements;
    lengCurrent =0;
    if(vertical)
    {
        if(sign == -1)
        {
            lengCurrent = dh;
        }
        while(cur)
        {
            clipRect.w=cur->rect->w;
            clipRect.h=cur->rect->h;
            if(sign ==1)
            {
                clipRect.y=lengCurrent;
            }
            else
            {
                clipRect.y= lengCurrent -clipRect.h;//ignore align data
            }
            
            
            if(cur->align & SGK_ALIGNCENTER)
            {
                clipRect.x= (dw - clipRect.w)/2;
            }
            else if(cur->align & SGK_ALIGNLEFT)
            {
                clipRect.x=0;
            }
            else if(cur->align & SGK_ALIGNRIGHT)
            {
                clipRect.x= dw - clipRect.w;
            }
            else
            {
                return 0;
            }
            //copy the rect to the struct
            cur->rect->x=clipRect.x;
            cur->rect->y=clipRect.y;
            cur->rect->w=clipRect.w;
            cur->rect->h=clipRect.h;
            
            if(SDL_BlitSurface(*cur->surface,NULL,cont->surface,&clipRect)!=0)
            {
                return 0;
            }
            lengCurrent += sign * clipRect.h;
            
            cur=cur->next;
        }
    }
    else
    {
        if(sign == -1)
        {
            lengCurrent = dw;
        }
        while(cur)
        {
            clipRect.w=cur->rect->w;
            clipRect.h=cur->rect->h;
            if(sign ==1)
            {
                clipRect.x=lengCurrent;
            }
            else
            {
                clipRect.x= lengCurrent -clipRect.w;//ignore align data
            }
            
            
            if(cur->align & SGK_ALIGNMIDDLE)
            {
                clipRect.y= (dh - clipRect.h)/2;
            }
            else if(cur->align & SGK_ALIGNTOP)
            {
                clipRect.y=0;
            }
            else if(cur->align & SGK_ALIGNBOTTOM)
            {
                clipRect.y= dh - clipRect.h;
            }
            else
            {
                return 0;
            }
            
            SDL_BlitSurface(*cur->surface,NULL,cont->surface,&clipRect);
            lengCurrent += sign * clipRect.w;
            
            cur=cur->next;
        }
        
    }
    //create final display
    cont->DispSurface = SDL_CreateRGBSurface(0, cont->rect.w, cont->rect.h, 32, rmask, gmask, bmask, amask);
    if(cont->DispSurface == NULL)
    {
        return 0;
    }

    if(vertical)
    {
        clipRect.w=cont->rect.w;
        if(cont->rect.h < cont->surface->h)
        {
            clipRect.h = cont->rect.h;
        }
        else
        {
            clipRect.h = cont->surface->h;
        }
    }
    else
    {
        clipRect.h = cont->rect.h;
        if(cont->rect.w < cont->surface->w)
        {
            clipRect.w = cont->rect.w;
        }
        else
        {
            clipRect.w = cont->surface->w;
        }
    }
    
    
    if(sign==1)
    {
        clipRect.x=0;
        clipRect.y=0;
    }
    else if(vertical)
    {
        clipRect.x=0;
        clipRect.y=cont->rect.h - dh;
    }
    else //if horizontal
    {
        clipRect.x=cont->rect.w - dw;
        clipRect.y=0;
    }
  
    SDL_BlitSurface(cont->surface,NULL,cont->DispSurface,&clipRect);
    //create texture
    cont->texture = SDL_CreateTextureFromSurface(cont->render,cont->DispSurface);
    if(cont->texture ==NULL)
    {
        puts("no texture");
        return 0;
    }
    cont->DispRect.x=clipRect.x;
    cont->DispRect.y=clipRect.y;
    cont->DispRect.w=clipRect.w;
    cont->DispRect.h=clipRect.h;
    
    return 1;
}

void SGK_DestroyContainer(MZSDL_Container * cont)
{
    MZSDL_ContElements * elem;
    MZSDL_ContElements * tmp;
    
    if(cont==NULL)
    {
        return;
    }
    
    SDL_FreeSurface(cont->DispSurface);
    SDL_FreeSurface(cont->surface);
    
    SDL_DestroyTexture(cont->texture);
    
    if(cont->layout == SGK_LINEARLAYOUT)
    {
        free(cont->spacer.linear);
    }
  
    elem= cont->elements;
    
    while(elem)
    { 
        switch(elem->type)
        {
            case SGK_TYPE_BUTTON:
                MZSDL_FreeButton(elem->element);break;
            case SGK_TYPE_CHECKBOK:
                MZSDL_FreeCheckBox(elem->element);break;
            case SGK_TYPE_INPUTBOX:
                MZSDL_FreeInputBox(elem->element);break;
            case SGK_TYPE_RADIOBOX:
                MZSDL_FreeRadioButton(elem->element);break;
            case SGK_TYPE_CONTAINER:
                SGK_DestroyContainer(elem->element);break;
        }

        
        tmp=elem;
        elem=elem->next;
        free(tmp);   
    }
    
}

int SGK__PR_ElementClicked(MZSDL_ContElements * e, int x, int y)
{
	 if(x>=e->rect->x && y>=e->rect->y && \
			 x<=e->rect->x+e->rect->w && y<=e->rect->y + e->rect->h)
	 {
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }

}

int SGK_PR_UpdateContainerDisplay(MZSDL_Container * cont)
{

    SDL_BlitSurface(cont->surface,NULL,cont->DispSurface,&cont->DispRect);
    //create texture
    cont->texture = SDL_CreateTextureFromSurface(cont->render,cont->DispSurface);
    if(cont->texture ==NULL)
    {
        puts("no texture");
        return 0;
    }
    
    return 1;
}

int SGK_Events_MouseDown(MZSDL_Container * cont,int x,int y)
{
    MZSDL_ContElements * elem;
    char update = 0;//boolean varaible to check if we should update the container texture
    if(cont->rect.x > x || cont->rect.x + cont->rect.w < x)
    {
        return 0;
    }
    if(cont->rect.y > y || cont->rect.y + cont->rect.h < y)
    {
        return 0;
    }   
    //TODO after adding event we should check cont->OnClick()

    /*
     * reset the x & y ,with respect to cont position
     * NOTICE we should pay attention later for scrolling 
     */
    x= x - cont->rect.x;
    y= y - cont->rect.y;
    
    //check event nodes
    elem = cont->elements;
    while(elem)
    {
        if(SGK__PR_ElementClicked(elem,x,y))
        {
            //do system events
            switch(elem->type)
            {
                case SGK_TYPE_BUTTON:
puts("button");                    
                    break;
                case SGK_TYPE_CHECKBOK:
                    
                    break;
                case SGK_TYPE_INPUTBOX:
                    MZSDL_EnableInputBox(cont->render,elem->element,1);
                    //NOTICE add the input box to global varible to disable it when clicked on different part
                    update = 1;
                    break;
                case SGK_TYPE_RADIOBOX:
                    
                    break;
                case SGK_TYPE_CONTAINER:
                    SGK_Events_MouseDown(elem->element,x,y);
            }
            //TODO do user events
            
            
            //final for update
            if(update)
            {
                SDL_BlitSurface(*elem->surface,NULL,cont->surface,elem->rect);
                SGK_PR_UpdateContainerDisplay(cont);
            }
            return 1;
        }
        else
        {
            elem=elem->next;
        }
    }
    return 1;
}



