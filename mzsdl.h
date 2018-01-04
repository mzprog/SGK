#ifndef _MZSDL_H_
#define _MZSDL_H_

#include <SDL2/SDL.h>


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask  0xff000000
#define gmask  0x00ff0000
#define bmask  0x0000ff00
#define amask  0x000000ff
#else
#define rmask  0x000000ff
#define gmask  0x0000ff00
#define bmask  0x00ff0000
#define amask  0xff000000
#endif

/*
 * the struct of the button
 */
typedef struct {
	SDL_Texture * texture;
	SDL_Surface * surface;
	SDL_Rect rect;
	Uint8 active;
} MZSDL_Button;
/*
 * the struct of the input box
 */
typedef struct {
	SDL_Texture * texture;
	SDL_Surface * bg;
	SDL_Surface * textSurf;
	int fontsize;
	SDL_Rect rect;
	char title[32];
	char text[256];
	Uint8 curPos;
	Uint8 active;
} MZSDL_InputBox;
/*
 * the struct of the radio button
 */

typedef struct {
	SDL_Texture * texture;
	SDL_Surface * surface;
	Uint8 lineHeight;
	Sint8 selected;
	char **option;
	SDL_Rect rect;
	Uint8 size;
} MZSDL_RadioButton;

/*
 * MZSDL_AddButton : to create new button
 * ren : the renderer used in the project
 * title : the title of the button
 * fontSize : the font size of the text
 * x & y : the location of the button depend on the left top corner of the button
 * returned value : the struct pointer of the created button or NULL if fail
 */
MZSDL_Button * MZSDL_AddButton(SDL_Renderer *ren, char * title, int fontSize, int x, int y);

/*
 * MZSDL_EnableButton : to enable or disable the button view
 * ren : the renderer used in the project
 * button : the button that will change
 * flag : 0 to disable or 1 to enable
 */
void MZSDL_EnableButton(SDL_Renderer * ren,MZSDL_Button * button, int flag);

/*
 * MZSDL_FreeButton : to free a button
 * button : the button to free it
 */
void MZSDL_FreeButton(MZSDL_Button * button);

/*
 * MZSDL_CreateInputBox : create input to get text from user
 * ren : the renderer that used in the project
 * title : the title displayed in grey color or NULL to display "Enter text"
 * fontSize : the font size
 * length : the width of the box
 * x & y : the position of the box depend on left top of the box
 * returned value : the pointer of the created input box
 */
MZSDL_InputBox * MZSDL_CreateInputBox(SDL_Renderer * ren, char * title, int fontSize, int length, int x, int y);

/*
 * MZSDL_EnableInputBox : to enable or disable input box
 * ren : the renderer used in the project
 * box : the input box that will change
 * mode : 0 to disable or 1 to enable
 * returned value : the pointer to the input box if succeed 0 if fail
 */
int MZSDL_EnableInputBox(SDL_Renderer *ren, MZSDL_InputBox * box, Uint8 mode);

/*
 * MZSDL_FreeInputBox : to free an input box
 * box : the input box that will be free
 * returned value : 1 if succeed 0 if fail
 */
void MZSDL_FreeInputBox(MZSDL_InputBox * box);

/*
 * MZSDL_UpdateInputBox : to add text for the input box
 * ren : the renderer that used in the project
 * box : the input box that will be changed
 * text : the text that scanned from user to add to the input box 
 * returned value: 1 if succeed 0 if fail
 */
int MZSDL_UpdateInputBox(SDL_Renderer * ren, MZSDL_InputBox * box, char *text);

/*
 * MZSDL_InputBoxClicked : to check if the input box clicked or not
 * box : the targeted input box
 * x & y : the position of the mouse when it was clicked
 * returned value : 0 not clicked, 1 clicked
 */
int MZSDL_InputBoxClicked(MZSDL_InputBox * box, int x, int y);

/*
 * MZSDL_EditInputBox : to edit text in the input box (till now it just accept the backspace)
 * ren : the renderer used in the project
 * flag : flag of the edition
 */
void MZSDL_EditInputBox(SDL_Renderer *ren, MZSDL_InputBox * box,int flag);

/*
 * MZSDL_GetTextFromInputBox : to get the enter text in the input box
 * box : the targeted input box
 * returned value : the text in the input box
 */
char * MZSDL_GetTextFromInputBox(MZSDL_InputBox * box);

/*
 * MZSDL_CreateRadioButton : to create a radio button with many choices
 * ren : the renderer used in the project
 * option : array of the options text
 * size : the count of the options
 * Default : the default selected choice
 * fontsize : the size of the font
 * x & y : the position of the radio button depend on the top left corner
 * returned value : the pointer to the created radio button, or 0 if fail
 */
MZSDL_RadioButton * MZSDL_CreateRadioButton(SDL_Renderer * ren, char **option, int size, int Default, int fontsize, int x, int y);

/*
 * MZSDL_RadioButtonClicked : to check if the radio button was clicked and which choice
 * rad : the radio button pointer
 * x & y : the position of the mouse when it was clicked
 * returned value : -1 if not clicked, or the number or the choice
 */
Sint8 MZSDL_RadioButtonClicked(MZSDL_RadioButton * rad, int x,int y);

/*
 * MZSDL_UpdateRadioButton : to change the selected choice
 * rad : the pointer of the radio button
 * opt : the number of the new choice
 */
void MZSDL_UpdateRadioButton(SDL_Renderer * ren, MZSDL_RadioButton * rad, Sint8 opt );

/*
 * MZSDL_FreeRadioButton : to fre the radio button
 * rad : the pointer of the radio button
 */
void MZSDL_FreeRadioButton(MZSDL_RadioButton * rad);

/*
 * MZSDL_PutPixel32 : to put a pixel of 32 bit on the surface
 * surface : the targeted surface
 * x & y : the position of the pixel
 * pixel the pixel that will put it
 */
void MZSDL_PutPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );

#endif
