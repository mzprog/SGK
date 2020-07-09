#include "mzsdl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define CornerSize 10
#define bColor 0x88888888 | amask



#define textCur '|'

MZSDL_Button * MZSDL_AddButton(SDL_Renderer *ren,char * title, int fontSize, int x, int y)
{
	int i,j;//pixels indexing
	int w, h;//dimention of the button
	int temp;//the temp value for calculation of the corners
	TTF_Font * font;
	SDL_Surface * textS;//the surface for the text
	MZSDL_Button * box;//the returned struct
	SDL_Color black={0,0,0};//black color
	SDL_Surface * surface;//the background surface

	//initialaze the struct of the button
	box = (MZSDL_Button *) malloc(sizeof(MZSDL_Button));
	if (box == NULL)
	{
		return 0;
	}

	//open the font
	font=TTF_OpenFont("arial.ttf",fontSize);
	if(font == NULL)
	{
        printf("%s\n",TTF_GetError());
		free(box);
		return 0;
	}
 
	//create the font surface and close the font
	textS= TTF_RenderText_Blended(font,title,black);
	TTF_CloseFont(font);

	if(textS == NULL)
	{
		free(box);
		return 0;
	}

	//calculate the height and width of the text surface
	w=textS->w;
	h=textS->h;
	//create background surface bigger the the text by 20 width and 10 height
	surface = SDL_CreateRGBSurface(0, w+20, h+10, 32, rmask, gmask, bmask, amask);
	if(surface == NULL)
	{
		free(box);
		SDL_FreeSurface(textS);
		return 0;
	}
 
	//fill the background by a solid color
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format,0xee,0xee,0xee));

	//draw black edges
	for(i=0;i<w+20;i++)
	{
		MZSDL_PutPixel32(surface,i,0,amask);
		MZSDL_PutPixel32(surface,i,h+9,amask);
	}
	for(j=0;j<h+10;j++)
	{
		MZSDL_PutPixel32(surface, 0, j, amask);
		MZSDL_PutPixel32(surface, w+19, j, amask);
	}

	//make round corners
	//left top corner
	for(i=0;i<=CornerSize;i++)
	{
		for(j=0;j<=CornerSize;j++)
		{
			temp=sqrt(pow(CornerSize-i,2) + pow(CornerSize-j,2));
			if(temp>CornerSize)
			{
				MZSDL_PutPixel32(surface, i, j, 0);
			}
			else if(temp == CornerSize)
			{
				MZSDL_PutPixel32(surface, i, j, amask);
			}
		}
	}
	//right top corner
	for(i=w+19-CornerSize;i<w+20;i++)
	{
		for(j=0;j<CornerSize;j++)
		{
			temp=sqrt(pow(CornerSize-(w+19-i),2)+pow(CornerSize-j,2));
			if(temp>CornerSize)
			{
				MZSDL_PutPixel32(surface, i, j, 0);
			}
			else if(temp == CornerSize)
			{
				MZSDL_PutPixel32(surface, i, j, amask);
			}

		}
	}
	//left bottom corner
	for(i=0;i<=CornerSize;i++)
	{
		for(j=h+9-CornerSize;j<h+10;j++)
		{
			temp=sqrt(pow(CornerSize-i,2)+pow(CornerSize-(h+9-j),2));
			if(temp>CornerSize)
			{
				MZSDL_PutPixel32(surface, i, j, 0);
			}
			else if(temp == CornerSize)
			{
				MZSDL_PutPixel32(surface, i, j, amask);
			}
		}
	}
	//right bottom corner
	for(i=w+19-CornerSize;i<w+20;i++)
	{
		for(j=h+9-CornerSize;j<h+10;j++)
		{
			temp=sqrt(pow(CornerSize-(w+19-i),2)+pow(CornerSize-(h+9-j),2));
			if(temp > CornerSize)
			{
				MZSDL_PutPixel32(surface, i, j, 0);
			}
			else if(temp == CornerSize)
			{
				MZSDL_PutPixel32(surface, i, j, amask);
			}
		}
	}
	//initailize the rect for bliting the two surfaces
	box->rect.x=10;
	box->rect.y=5;
	box->rect.h=h;
	box->rect.w=w;
	//bliting the surfaces and create the button texture
	SDL_BlitSurface(textS, NULL, surface, &(box->rect));
	box->texture=SDL_CreateTextureFromSurface(ren,surface);
	box->surface=surface;
	box->active=1;
	//adjust the rect of the button
	box->rect.x=x;
	box->rect.y=y;
	box->rect.w=w+20;
	box->rect.h=h+10;
	//clean the useless elements
	SDL_FreeSurface(textS);

	return box;

}
void MZSDL_EnableButton(SDL_Renderer * ren,MZSDL_Button *button, int flag)
{
	int i;
 	Uint32 *pixels = (Uint32 *)button->surface->pixels;//convert void to Uint32 to use pixels variable

	//if at the same mode nothing to do
	if(button->active==flag)
	{
		return;
	}
	if(flag == 0)//for disable
	{
		for(i=0;i<button->surface->w*button->surface->h;i++)
		{
			//if alpha not null
			if(pixels[i] & amask)
			{
				pixels[i]= (pixels[i]&(~amask)) | (0x77777777 & (amask) );
			}

		}
	}
	else//for enable
	{
		for(i=0;i<button->surface->w*button->surface->h;i++)
		{
			if(pixels[i] & amask)
			{
				//put alpha 100%
				pixels[i] |=amask;
			}
		}
	}
	button->active=flag;
	//destroy old texture and create new one
	SDL_DestroyTexture(button->texture);
	button->texture=SDL_CreateTextureFromSurface(ren,button->surface);
}

int MZSDL_ButtonClicked(MZSDL_Button * button, int x, int y)
{
     if(x>=button->rect.x && y>=button->rect.y && \
			 x<=button->rect.x+button->rect.w && y<=button->rect.y + button->rect.h)
	 {
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
}

void MZSDL_FreeButton(MZSDL_Button * button)
{
	SDL_DestroyTexture(button->texture);
	SDL_FreeSurface(button->surface);
	free(button);
}

MZSDL_InputBox * MZSDL_CreateInputBox(SDL_Renderer * ren, char * title, int fontSize, int length, int x, int y)
{
	MZSDL_InputBox * inbox;//the returned varaible
	TTF_Font * font;//the font
	SDL_Surface * textS;//the text surface
	SDL_Texture * textT;//multi use for texture
	SDL_Surface * bgS;//for background
	SDL_Surface * bgS2;//another surface for background
	SDL_Color grey= {0x88,0x88,0x88};//grey color
	SDL_Rect r;
	int i;

	//initailize the structure of the input box
	inbox= (MZSDL_InputBox *) malloc(sizeof(MZSDL_InputBox));
	if(inbox == NULL)
	{
		return 0;
	}
	//open the font
	font = TTF_OpenFont("./arial.ttf",fontSize);
	if(font == NULL)
	{
		printf("Error:%s\n",SDL_GetError());
		free(inbox);
		return 0;
	}
	//create the title of the input box
	if(title==NULL)
	{
		//the defualt title
		strcpy(inbox->title,"Enter Text");
	}
	else
	{
		strcpy(inbox->title,title);
	}

	//create font as surface and close the font
	textS= TTF_RenderText_Blended(font,inbox->title,grey);
	TTF_CloseFont(font);
	if(textS == NULL)
	{
		free(inbox);
		return 0;
	}

	//calculate the surface height and width for rect
	r.w=textS->w;
	r.h=textS->h;
	//create the background surface
	bgS = SDL_CreateRGBSurface(0,length,r.h + 10, 32, rmask, gmask, bmask, amask);
	if(bgS == NULL)
	{
		free(inbox);
		SDL_FreeSurface(textS);
		return 0;
	}
	//create the another surface
	bgS2 = SDL_CreateRGBSurface(0, length, r.h +10 , 32, rmask, gmask, bmask, amask);
	if(bgS2 == NULL)
	{
		free(inbox);
		SDL_FreeSurface(textS);
		SDL_FreeSurface(bgS);
		return 0;
	}

	//fill the border gray color for not active state and a white body
	SDL_FillRect(bgS, NULL, SDL_MapRGB(bgS->format,0xff,0xff,0xff));

	//horizontal borders
	for(i=0;i<length;i++)
	{
		MZSDL_PutPixel32(bgS, i, 0, bColor);//top
		MZSDL_PutPixel32(bgS, i, r.h +9, bColor);//bottom
	}
	//vertical borders
	for(i=0;i<r.h + 10;i++)
	{
		MZSDL_PutPixel32(bgS, 0, i, bColor);//left
		MZSDL_PutPixel32(bgS, length-1, i, bColor);//right
	}
	// copy the surface
	SDL_BlitSurface(bgS, NULL, bgS2, NULL);

	//copy the data
	inbox->bg = bgS;
	inbox->rect.x=x;
	inbox->rect.y=y;
	inbox->rect.w=length;
	inbox->rect.h=r.h + 10;
	inbox->active = 0;
	//bliting the text surface on the background surface
	r.x=5;
	r.y=5;
	SDL_BlitSurface(textS,NULL,bgS2,&r);
	//set text with zeros
	memset(inbox->text,0,256);
	inbox->curPos=0;//cursor start at zero

	inbox->textSurf=textS;//copy the text surface
	textT= SDL_CreateTextureFromSurface(ren,bgS2);//create the texture
	if(textT==NULL)
	{
		SDL_FreeSurface(bgS);
		//SDL_FreeSurface(bgS2);
		SDL_FreeSurface(textS);
		free(inbox);
		return 0;

	}
	//copy texture and the font size
	inbox->surface=bgS2;
	inbox->texture=textT;
	inbox->fontsize=fontSize;
	return inbox;
}

int MZSDL_EnableInputBox(SDL_Renderer *ren, MZSDL_InputBox * box, Uint8 mode)
{
	int i;
	TTF_Font * font;
	SDL_Surface * bg2;
	SDL_Surface * textS;

	SDL_Rect r,r2;
	SDL_Color grey={0x88,0x88,0x88};
	SDL_Color black={0,0,0};
	char buf[256];

	//if the same mode nothing to do
	if(box->active == mode)
	{
		return 1;
	}

	if(mode == 0)
	{
		//draw grey color for the border
		//vertical
		for(i=0;i<box->rect.w;i++)
		{
			MZSDL_PutPixel32(box->bg, i, 0, bColor);//top
			MZSDL_PutPixel32(box->bg, i, box->rect.h-1,bColor);//botton
		}
		//horizontal
		for(i=0;i<box->rect.h;i++)
		{
			MZSDL_PutPixel32(box->bg, 0, i,bColor);//left
			MZSDL_PutPixel32(box->bg, box->rect.w-1, i, bColor);//right
		}
		//edit text and remove the cursor from the text
		strcpy(buf,box->text+box->curPos+1);
		memset(box->text+box->curPos,0,strlen(buf)+1);
		strcat(box->text,buf);

		//if no text print the title of the input box
		if(strlen(box->text)==0)
		{
			font= TTF_OpenFont("./arial.ttf",box->fontsize);
			if(font == NULL)
			{
				return 0;
			}
			textS=TTF_RenderText_Blended(font,box->title,grey);//create the title surface
			TTF_CloseFont(font);
			if(textS==NULL)
			{
				return 0;
			}
			//read height and width
			r.w=textS->w;
			r.h=textS->h;
			//create another surface
			bg2= SDL_CreateRGBSurface(0,box->rect.w,box->rect.h,32,rmask,gmask,bmask,amask);
			if(bg2==NULL)
			{
				SDL_FreeSurface(textS);
				return 0;
			}
			//bliting the same  surface data
			SDL_BlitSurface(box->bg,NULL,bg2,NULL);

			r.x=5;
			r.y=5;
			//bliting the text on the background
			SDL_BlitSurface(textS,NULL,bg2,&r);
			//destroy the old texture and create new one
			SDL_DestroyTexture(box->texture);
			box->texture=SDL_CreateTextureFromSurface(ren,bg2);
			if(box->texture == NULL)
			{
				SDL_FreeSurface(textS);
				SDL_FreeSurface(bg2);
				return 0;
			}
			//coping last data and clear useless surface
			box->textSurf=textS;
			SDL_FreeSurface(box->surface);
            box->surface=bg2;
			box->active=0;
			return 1;
		}
		else//if we have text
		{
			font= TTF_OpenFont("./arial.ttf",box->fontsize);
			if(font == NULL)
			{
				return 0;
			}
			//create the text surface and close font
			textS=TTF_RenderText_Blended(font,box->text,black);
			TTF_CloseFont(font);
			if(textS==NULL)
			{

				return 0;
			}
			//read height and width
			r.w=textS->w;
			r.h=textS->h;
			//create new background surface
			bg2= SDL_CreateRGBSurface(0,box->rect.w,box->rect.h,32,rmask,gmask,bmask,amask);
                        if(bg2==NULL)
                        {
				SDL_FreeSurface(textS);

                                return 0;
			}
                        SDL_BlitSurface(box->bg,NULL,bg2,NULL);
                        r.x=5;
                        r.y=5;
			//free old surface and create new one
			SDL_FreeSurface(box->textSurf);
			box->textSurf=textS;
			//blit text on bg
			SDL_BlitSurface(box->textSurf,NULL,bg2,&r);
			//destroy old texture and create new one
                        SDL_DestroyTexture(box->texture);
                        box->texture=SDL_CreateTextureFromSurface(ren,bg2);
                        if(box->texture == NULL)
                        {
				SDL_FreeSurface(textS);
				SDL_FreeSurface(bg2);
				return 0;
			}

			//SDL_FreeSurface(bg2);
			SDL_FreeSurface(box->surface);
            box->surface=bg2;
			box->active=0;
			return 1;

		}
	}
	else//enabling mode
	{
		font= TTF_OpenFont("./arial.ttf",box->fontsize);
		if(font==NULL)
		{
			return 0;
		}
		//draw black borders
		//vertical
		for(i=0;i<box->rect.w;i++)
		{
			MZSDL_PutPixel32(box->bg, i, 0, amask);//top
			MZSDL_PutPixel32(box->bg, i, box->rect.h-1,amask);//bottom
		}
		//horizontal
		for(i=0;i<box->rect.h;i++)
		{
			MZSDL_PutPixel32(box->bg, 0, i,amask);//left
			MZSDL_PutPixel32(box->bg, box->rect.w-1, i, amask);//right
		}
		//add the cursor at the end and get the cursor location
		box->text[strlen(box->text)]=textCur;
		box->curPos=strlen(box->text)-1;
		//create the text surface
		textS=TTF_RenderText_Blended(font,box->text,black);
		TTF_CloseFont(font);
		if(textS==NULL)
		{
			return 0;
		}
		//clear the old surface and assign the new one
		SDL_FreeSurface(box->textSurf);
		box->textSurf=textS;
		//read the height and width
		r.w=textS->w;
		r.h=textS->h;
		//create another bg surface
		bg2=SDL_CreateRGBSurface(0,box->rect.w,box->rect.h,32,rmask,gmask,bmask,amask);
		if(bg2==NULL)
		{
			SDL_FreeSurface(textS);
		}
		SDL_BlitSurface(box->bg,NULL,bg2,NULL);//copy all pixels
		r.x=5;
		r.y=5;
		//if the width of the text greater the the width of box
		//we should render the last part pf the text only
		if(r.w>=box->rect.w)
		{
			r.x=0;
			r2.w=box->rect.w;
			r2.x=r.w - box->rect.w;
			r2.h=r.h;
			r2.y=0;
			SDL_BlitSurface(box->textSurf,&r2,bg2,&r);
		}
		else//render all
		{
			SDL_BlitSurface(box->textSurf,NULL,bg2,&r);
		}
		//destroy the texture and create new one and free surface and return
		SDL_DestroyTexture(box->texture);
		box->texture=SDL_CreateTextureFromSurface(ren,bg2);
		//SDL_FreeSurface(bg2);
        SDL_FreeSurface(box->surface);
        box->surface = bg2;
		if(box->texture==NULL)
		{
			return 0;
		}
		else
		{
			box->active=1;
		       	return 1;
		}
	}
}
int MZSDL_UpdateInputBox(SDL_Renderer * ren, MZSDL_InputBox * box, char *text)
{
	SDL_Surface * bg2;//another background surface
	SDL_Texture * textT;
	TTF_Font * font;
	SDL_Color black={0,0,0};
	SDL_Rect r,r2;
	char buf[256];

	if(box->text[box->curPos+1]==0)//if the cursor at the end of the text
	{
		box->text[box->curPos]=0;//remove cursor
		strcat(box->text,text);//add the text at the end
		box->text[strlen(box->text)]=textCur;//add cursor
		box->curPos+=strlen(text);//calculate cursor location
	}
	else//if the cursor at the middle of the text
	{
		strcpy(buf,box->text +box->curPos +1);//copy all chars after the cursor
		memset(box->text+box->curPos,0,strlen(text)+1);//set cursor and char after to zero
		strcat(box->text,text);//add the text
		box->text[strlen(box->text)]=textCur;//add cursor
		box->curPos+=strlen(text);//calculate the cursor position
		strcat(box->text,buf);//add removed text
	}
	//open the font
	font= TTF_OpenFont("arial.ttf",box->fontsize);
	if(font ==NULL)
	{
		return 0;
	}
	//free text surface and create new one
	SDL_FreeSurface(box->textSurf);
	box->textSurf= TTF_RenderText_Blended(font, box->text,black);
	TTF_CloseFont(font);
	if(box->textSurf==NULL)
	{
		return 0;
	}
	//read rect data
	r.w=box->textSurf->w;
	r.h=box->textSurf->h;
	r.x=5;
	r.y=5;
	//create another bg surface
	bg2=SDL_CreateRGBSurface(0,box->rect.w,box->rect.h,32,rmask,gmask,bmask,amask);
	if(bg2==NULL)
	{
		return 0;
	}
	SDL_BlitSurface(box->bg,NULL,bg2,NULL);//read all pixels
	//calculate the appeared text
	if(r.w>=box->rect.w)
	{
		r.x=0;
		r2.w=box->rect.w;
		r2.x=r.w - box->rect.w;
		r2.h=r.h;
		r2.y=0;
		SDL_BlitSurface(box->textSurf,&r2,bg2,&r);
	}
	else
	{
		SDL_BlitSurface(box->textSurf,NULL,bg2,&r);
	}
	//create the texture
	textT= SDL_CreateTextureFromSurface(ren,bg2);
	if(textT==NULL)
	{
		SDL_FreeSurface(bg2);
		return 0;
	}
	//new surface
	SDL_FreeSurface(box->surface);
    box->surface=bg2;
	//destroy old texture and assign new one
	SDL_DestroyTexture(box->texture);
	box->texture=textT;
	return 1;
}

int MZSDL_InputBoxClicked(MZSDL_InputBox * box, int x, int y)
{
	 if(x>=box->rect.x && y>=box->rect.y && \
			 x<=box->rect.x+box->rect.w && y<=box->rect.y + box->rect.h)
	 {
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }

}

void MZSDL_EditInputBox(SDL_Renderer * ren, MZSDL_InputBox * box, int flag)
{
	char buf[256];
	TTF_Font * font;
	SDL_Color black={0,0,0};
	SDL_Rect r,r2;
	SDL_Texture * textT;
	SDL_Surface * bg2;

	//if the user press backspace and we have text in the box
	if(flag== SDLK_BACKSPACE && box->curPos>0)
	{
		box->text[box->curPos]=0;//remove cursor
		box->text[box->curPos-1]=textCur;//add new cursor at last char position
		box->curPos--;//update cursor positoin
		strcpy(buf,box->text + box->curPos +1);//copy all text after the cursor
		strcat(box->text,buf);//shift it directly after the cursor
	}
	//open font and create new surface for text and free old one
	font=TTF_OpenFont("arial.ttf",box->fontsize);
	if(font == NULL)
	{
		return;
	}

	SDL_FreeSurface(box->textSurf);
	box->textSurf= TTF_RenderText_Blended(font,box->text,black);
	TTF_CloseFont(font);
	if(box->textSurf == NULL)
	{
		return;
	}
	//read rect data
	r.h=box->textSurf->h;
	r.w=box->textSurf->w;
	r.x=5;
	r.y=5;
	//create another bg
	bg2=SDL_CreateRGBSurface(0,box->rect.w,box->rect.h,32,rmask,gmask,bmask,amask);
	if(bg2==NULL)
	{
		return;
	}
	SDL_BlitSurface(box->bg,NULL,bg2,NULL);//read all pixels
	//calculate appeared text
	if(r.w>=box->rect.w)
	{
		r.x=0;
		r2.w=box->rect.w;
		r2.x=r.w - box->rect.w;
		r2.h=r.h;
		r2.y=0;
		r.w=box->rect.w;
		SDL_BlitSurface(box->textSurf,&r2,bg2,&r);
	}
	else
	{

		SDL_BlitSurface(box->textSurf,NULL,bg2,&r);
	}
	//create old texture and destroy old one
	textT= SDL_CreateTextureFromSurface(ren,bg2);
	if(textT==NULL)
	{
		SDL_FreeSurface(bg2);
		return;
	}
	SDL_DestroyTexture(box->texture);
	box->texture=textT;
}
char * MZSDL_GetTextFromInputBox(MZSDL_InputBox * box)
{
	return box->text;
}

void MZSDL_FreeInputBox(MZSDL_InputBox * box)
{
	SDL_DestroyTexture(box->texture);
	SDL_FreeSurface(box->bg);
	SDL_FreeSurface(box->textSurf);
    SDL_FreeSurface(box->surface);
	free(box);
}

MZSDL_RadioButton * MZSDL_CreateRadioButton(SDL_Renderer * ren, char **option, int size, int Default, int fontsize, int x, int y)
{
	int i,j,k;
	int length;
	SDL_Surface ** textS;
	SDL_Surface ** FtextS;
	SDL_Surface * surf;
	MZSDL_RadioButton * rad;
	SDL_Rect r;
	SDL_Color black={0,0,0};
	Uint32 grey=0x77777777 | amask;
	Uint32 LightGrey=0xBBBBBBBB | amask;
	Uint32 green= amask | gmask | (0x33333333 &(rmask | bmask ));
	TTF_Font * font;

	//should be two or more choice
	if(size<2)
	{
		return 0;
	}
	//create the struct
	rad =(MZSDL_RadioButton *) malloc(sizeof(MZSDL_RadioButton)) ;
	if(rad ==NULL)
	{
		return 0;
	}
	//allocate copy the choices text
	rad->option=(char **)malloc(sizeof(char *)* size);
	if(rad->option == NULL)
	{
		free(rad);
		return 0;
	}
	for(i=0;i<size;i++)
	{
		rad->option[i]=(char *)malloc(sizeof(char)*(strlen(option[i])+1));
		if(rad->option[i]==NULL)
		{
			for(j=0;j<i;j++)
			{
				free(rad->option[j]);
			}
			free(rad->option);
			free(rad);
			return 0;
		}
		strcpy(rad->option[i],option[i]);

	}
	//create choises surfaces
	textS = (SDL_Surface **) malloc(sizeof(SDL_Surface * )*size);
	if(textS == NULL)
	{
		for(i=0;i<size;i++)
		{
			free(rad->option[i]);
		}
		free(rad->option);
		free(rad);
		return 0;
	}
	//create the final surfaces
	FtextS = (SDL_Surface **) malloc(sizeof(SDL_Surface *)*size);
	if(FtextS==NULL)
	{
		for(i=0;i<size;i++)
		{
			free(rad->option[i]);
		}
		free(rad->option);
		free(rad);
		free(FtextS);
		return 0;
	}
	//create text surfaces form font
	font = TTF_OpenFont("arial.ttf",fontsize);
	if(font == NULL)
	{
		for(i=0;i<size;i++)
		{
			free(rad->option[i]);
		}
		free(rad->option);
		free(rad);
		free(textS);
		free(FtextS);
		return 0;
	}
	for(i=0;i<size;i++)
	{
		textS[i]=TTF_RenderText_Blended(font, option[i],black);
		if(textS[i]==NULL)
		{
			for(j=0;j<i;j++)
			{
				SDL_FreeSurface(textS[j]);
			}
			for(j=0;j<size;j++)
			{
				free(rad->option[i]);
			}
			free(rad->option);
			free(rad);
			free(textS);
			free(FtextS);
			TTF_CloseFont(font);
			return 0;
		}

	}
	TTF_CloseFont(font);
	//create final choises surface
	for(i=0;i<size;i++)
	{
		FtextS[i]=SDL_CreateRGBSurface(0,textS[i]->w + textS[i]->h+10,textS[i]->h,32,rmask,gmask,bmask,amask);
		if(FtextS==NULL)
		{
			for(j=0;j<i;j++)
			{
				SDL_FreeSurface(FtextS[j]);
			}
			for(j=0;j<size;j++)
			{
				SDL_FreeSurface(textS[j]);
				free(rad->option[i]);
			}
			free(rad->option);
			free(rad);
			free(textS);
			free(FtextS);
			return 0;
		}
		r.x=textS[i]->h + 10;
		r.y=0;
		r.w=textS[i]->w;
		r.h=textS[i]->h;
		SDL_BlitSurface(textS[i], NULL, FtextS[i],&r);
		//create the circle
		for(j=0;j<r.h;j++)
		{
			for(k=0;k<r.h;k++)
			{
				length=(double) sqrt(pow((double)(k-r.h/2),2)+pow((double)(j-r.h/2),2));
				if(length==r.h/2 -3|| length == r.h/2 -2)
				{
					MZSDL_PutPixel32(FtextS[i],j,k,grey);
				}
				else if(length < r.h/2-2)
				{
					if(i==Default)
						MZSDL_PutPixel32(FtextS[i],j,k,green);
					else
						MZSDL_PutPixel32(FtextS[i],j,k,LightGrey);
				}
				else if(length == r.h/2 -1)
				{
					MZSDL_PutPixel32(FtextS[i],j,k,grey ^ (amask & 0x33333333));
				}

			}
		}
	}
	for(i=0;i<size;i++)
	{
		if(r.w< FtextS[i]->w)
			r.w= FtextS[i]->w;
	}
	//create the full surface for all choices
	surf= SDL_CreateRGBSurface(0,r.w,r.h*size,32,rmask,gmask,bmask,amask);
	if(surf == NULL)
	{
		for(i=0;i<size;i++)
		{
			SDL_FreeSurface(textS[i]);
			SDL_FreeSurface(FtextS[i]);
			free(rad->option[i]);
		}
		free(rad->option);
		free(rad);
		free(textS);
		free(FtextS);
		return 0;
	}
	//copy the choices surface at the full surface
	r.x=0;
	for(i=0;i<size;i++)
	{
		r.w=FtextS[i]->w;
		r.y=r.h*i;
		SDL_BlitSurface(FtextS[i],NULL,surf,&r);
	}
	//copy data and create the texture
	rad->lineHeight=r.h;
	rad->selected=Default;
	rad->texture=SDL_CreateTextureFromSurface(ren,surf);
	if(rad->texture==NULL)
	{
		for(i=0;i<size;i++)
		{
			SDL_FreeSurface(textS[i]);
			SDL_FreeSurface(FtextS[i]);
			free(rad->option[i]);
		}
		free(rad->option);
		free(rad);
		free(textS);
		free(FtextS);
		SDL_FreeSurface(surf);
		return 0;

	}
	rad->rect.x=x;
	rad->rect.y=y;
	rad->rect.w=surf->w;
	rad->rect.h=surf->h;
	rad->size=size;
	for(i=0;i<size;i++)
	{
		SDL_FreeSurface(textS[i]);
		SDL_FreeSurface(FtextS[i]);

	}
	rad->surface=surf;
	return rad;
}

Sint8 MZSDL_RadioButtonClicked(MZSDL_RadioButton * rad, int x,int y)
{
	if(x >rad->rect.x && y > rad->rect.y && x < rad->rect.x + rad->rect.w && y <rad->rect.y + rad->rect.h)
	{
		return (y-rad->rect.y)/rad->lineHeight;
	}
	else
	{
		return -1;
	}
}

void MZSDL_UpdateRadioButton(SDL_Renderer * ren, MZSDL_RadioButton * rad, Sint8 opt)
{
	int center,center2,a;
	int i,j;
	int length;
	Uint32 LightGrey=0xBBBBBBBB | amask;
	Uint32 green= amask | gmask | (0x33333333 &(rmask | bmask ));
	//if not an option or the same option nothing to do
	if(opt == -1 || opt == rad->selected)
	{
		return;
	}
	//calculate the of the old and new choices' circle
	center =rad->lineHeight*rad->selected + rad->lineHeight/2;
	center2=rad->lineHeight*opt           + rad->lineHeight/2;
	a=center2 - center;
	for(i=0;i<rad->lineHeight;i++)
	{
		for(j=rad->lineHeight*rad->selected;j<rad->lineHeight*(rad->selected+1);j++)
		{
			length= (double)sqrt(pow((double)(i-rad->lineHeight/2),2)+pow((double)(j-center),2));
			if(length< rad->lineHeight/2 -3)
			{
				MZSDL_PutPixel32(rad->surface, i,j  , LightGrey);//remove
				MZSDL_PutPixel32(rad->surface, i,j+a, green);//add
			}
		}
	}
	rad->selected=opt;
	SDL_DestroyTexture(rad->texture);
	rad->texture=SDL_CreateTextureFromSurface(ren,rad->surface);
}

void MZSDL_FreeRadioButton(MZSDL_RadioButton * rad)
{
	int i;
	SDL_DestroyTexture(rad->texture);
	SDL_FreeSurface(rad->surface);
	for(i=0;i<rad->size;i++)
	{
		free(rad->option[i]);
	}
	free(rad->option);
	free(rad);
}

MZSDL_CheckBox * MZSDL_CreateCheckBox(SDL_Renderer * ren, char **option, int size, int fontsize, int x, int y)
{
	MZSDL_CheckBox * box;
	SDL_Surface ** textS;
	SDL_Surface ** textF;
	SDL_Rect r;
	TTF_Font * font;
	SDL_Color black={0,0,0};
	Uint32 grey=0x77777777|amask;
	int i,j,k;
	int tx,tj;

    //alocating data
	box=(MZSDL_CheckBox *) malloc(sizeof(MZSDL_CheckBox));
	if(box==NULL)
	{
		return 0;
	}
	textS=(SDL_Surface **) malloc(sizeof(SDL_Surface *)*size);
	if(textS==NULL)
	{
		free(box);
		return 0;
	}
	textF=(SDL_Surface **) malloc(sizeof(SDL_Surface *)*size);
	if(textS==NULL)
	{
		free(box);
		free(textS);
		return 0;
	}
	font=TTF_OpenFont("arial.ttf",fontsize);
	if(font==NULL)
	{
		free(box);
		free(textS);
		free(textF);
		return 0;
	}
	
	//create surfaces for options
	for(i=0;i<size;i++)
	{
		textS[i]=TTF_RenderText_Blended(font,option[i],black);
		if(textS[i]==NULL)
		{
			for(j=0;j<i;j++)
			{
				SDL_FreeSurface(textS[j]);
			}
			free(box);
			free(textS);
			free(textF);
			TTF_CloseFont(font);
			return 0;
		}
	}
	TTF_CloseFont(font);
    
    
	for(i=0;i<size;i++)
	{
		textF[i]=SDL_CreateRGBSurface(0,textS[i]->w + textS[i]->h+10,textS[i]->h,32,rmask,gmask,bmask,amask);
		if(textF[i]==NULL)
		{
			for(j=0;j<i;j++)
			{
				SDL_FreeSurface(textF[j]);
			}
			for(i=0;i<size;i++)
			{
				SDL_FreeSurface(textS[i]);
			}
			free(box);
			free(textS);
			free(textF);
			return 0;
		}
		r.h=textS[i]->h;
		r.y=0;
		r.w=textS[i]->w;
		r.x=textS[i]->h +10;
		SDL_BlitSurface(textS[i],NULL,textF[i],&r);

        //draw rectengle with border
		for(j=1;j<r.h;j++)
		{
			for(k=1;k<r.h;k++)
			{
				if(j==1 || k==1 || j==r.h-1 || k==r.h-1 )
				{
					MZSDL_PutPixel32(textF[i],j,k,amask);
				}
				else
				{
					MZSDL_PutPixel32(textF[i],j,k,0xffffffff);
				}
			}
		}
		for(j=2;j<r.h-1;j++)
		{
			for(k=0;k<3;k++)
			{
				if(j<3*r.h/8)
				{
					tj=r.h+j-3*r.h/8-k -1;
					if(tj>1 && tj<r.h-1)
					{
						MZSDL_PutPixel32(textF[i],j,tj,grey);
					}
				}
				tj=r.h-k-5*j/8;
				if(tj>1 && tj<r.h-1)
				{
					MZSDL_PutPixel32(textF[i],tj,j,grey);
				}
			}
		}
	}

	tx=0;
	for(i=0;i<size;i++)
	{
		if(textF[i]->w>tx)
		{
			tx=textF[i]->w;
		}
	}
	tj=textF[0]->h;
	box->surface=SDL_CreateRGBSurface(0, tx, tj*size, 32, rmask, gmask, bmask, amask);
	if(box->surface==NULL)
	{
		for(i=0;i<size;i++)
		{
			SDL_FreeSurface(textS[i]);
			SDL_FreeSurface(textF[j]);
		}
		free(box);
		free(textS);
		free(textF);
		return 0;
	}

	r.x=0;
	r.h=tj;
	for(i=0;i<size;i++)
	{
		r.w=textF[i]->w;
		r.y=i*r.h;
		SDL_BlitSurface(textF[i],NULL,box->surface,&r);
	}

	for(i=0;i<size;i++)
	{
		SDL_FreeSurface(textS[i]);
		SDL_FreeSurface(textF[i]);
	}

	free(textS);
	free(textF);
	box->texture=SDL_CreateTextureFromSurface(ren,box->surface);

	if(box->texture==NULL)
	{
		SDL_FreeSurface(box->surface);
		free(box);
		return 0;
	}
	box->lineHeight=tj;
	box->selected=(Uint8 *)malloc(sizeof(Uint8)*size);
	if(box->selected==NULL)
	{
		SDL_FreeSurface(box->surface);
		SDL_DestroyTexture(box->texture);
		free(box);
		return 0;
	}

	for(i=0;i<size;i++)
	{
		box->selected[i]=0;
	}
	box->size=size;
	box->rect.h=box->surface->h;
	box->rect.w=box->surface->w;
	box->rect.x=x;
	box->rect.y=y;

	box->option=(char **) malloc(sizeof(char * )*size);
	if(box->option==NULL)
	{
		SDL_FreeSurface(box->surface);
		SDL_DestroyTexture(box->texture);
		free(box->selected);
		free(box);
		return 0;
	}
	for(i=0;i<size;i++)
	{
		box->option[i]=(char *) malloc(sizeof(char)*(strlen(option[i])+1));
		if(box->option[i]==NULL)
		{
			for(j=0;j<i;j++)
			{
				free(box->option[j]);
			}
			free(box->option);
			free(box->selected);
			SDL_DestroyTexture(box->texture);
			SDL_FreeSurface(box->surface);
			free(box);
			return 0;
		}
		strcpy(box->option[i],option[i]);
	}
	return box;
}

void MZSDL_FreeCheckBox(MZSDL_CheckBox * box)
{
	int i;
	SDL_FreeSurface(box->surface);
	SDL_DestroyTexture(box->texture);
	free(box->selected);
	for(i=0;i<box->size;i++)
	{
		free(box->option[i]);
	}
	free(box->option);
	free(box);
}

Sint8 MZSDL_CheckBoxClicked(MZSDL_CheckBox * box, int x,int y)
{
	if(x >box->rect.x && y > box->rect.y && x < box->rect.x + box->rect.w && y <box->rect.y + box->rect.h)
	{
		return (y-box->rect.y)/box->lineHeight;
	}
	else
	{
		return -1;
	}
}

void MZSDL_UpdateCheckBox(SDL_Renderer *ren, MZSDL_CheckBox * box, Sint8 opt)
{
	int j,tj,k;
	SDL_Texture * tex;
	Uint32 color;

	if(opt==-1)
	{
		return;
	}
	if(box->selected[opt]==0)
	{
		box->selected[opt]=1;
		color=gmask | amask;
	}
	else
	{
		box->selected[opt]=0;
		color=0x77777777 | amask;
	}
	for(j=2;j<box->lineHeight-1;j++)
	{
		for(k=0;k<3;k++)
		{
			if(j<3*box->lineHeight/8)
			{
				tj=box->lineHeight+j-3*box->lineHeight/8-k -1;
				if(tj>1 && tj<box->lineHeight-1)
				{
					MZSDL_PutPixel32(box->surface,j,tj+opt*box->lineHeight,color);
				}
			}
			tj=box->lineHeight-k-5*j/8;
			if(tj>1 && tj<box->lineHeight-1)
			{
				MZSDL_PutPixel32(box->surface,tj,j+opt*box->lineHeight,color);
			}
		}
	}
	tex=SDL_CreateTextureFromSurface(ren,box->surface);
	if(tex == NULL)
	{
		return;
	}
	box->texture=tex;
}


void MZSDL_PutPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	//Set the pixel
	pixels[ ( y * surface->w ) + x ] = pixel;
}
