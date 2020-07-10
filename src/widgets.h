#ifndef _WIDGETS_H_
#define _WIDGETS_H_

#include <SDL2/SDL.h>//or maybe you should remove 'SDL2'


#if SDL_BYTEORDER == SDL_BIG_ENDIAN

#define rmask  0xff000000
#define gmask  0x00ff0000
#define bmask  0x0000ff00
#define amask  0x000000ff

#define rshift 24
#define gshift 16
#define bshift 8
#define ashift 0


#else

#define rmask  0x000000ff
#define gmask  0x0000ff00
#define bmask  0x00ff0000
#define amask  0xff000000

#define rshift 0
#define gshift 8
#define bshift 16
#define ashift 24

#endif

/*
 * the struct of the button
 */
typedef struct {
	SDL_Texture * texture;
	SDL_Surface * surface;
	SDL_Rect rect;
	Uint8 active;
} SGK_Button;
/*
 * the struct of the input box
 */
typedef struct {
	SDL_Texture * texture;
    SDL_Surface * surface;//final surface
	SDL_Surface * bg;
	SDL_Surface * textSurf;
	int fontsize;
	SDL_Rect rect;
	char title[32];
	char text[256];
	Uint8 curPos;
	Uint8 active;
} SGK_InputBox;
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
} SGK_RadioButton;
/*
 * the struct of the check box
 */

typedef struct {
	SDL_Texture * texture;
	SDL_Surface * surface;
	Uint8 lineHeight;
	Uint8 * selected;
	char ** option;
	SDL_Rect rect;
	Uint8 size;
} SGK_CheckBox;

/*
 * SGK_AddButton : to create new button
 * ren : the renderer used in the project
 * title : the title of the button
 * fontSize : the font size of the text
 * x & y : the location of the button depend on the left top corner of the button
 * returned value : the struct pointer of the created button or NULL if fail
 */
SGK_Button * SGK_AddButton(SDL_Renderer *ren, char * title, int fontSize, int x, int y);

/*
 * SGK_EnableButton : to enable or disable the button view
 * ren : the renderer used in the project
 * button : the button that will change
 * flag : 0 to disable or 1 to enable
 */
void SGK_EnableButton(SDL_Renderer * ren,SGK_Button * button, int flag);

/*
 * SGK_ButtonClicked : to check if the button is clicked by mouse
 * x & y : the position of the mouse clicking
 * return : 1 if true , 0 if false
 */
int SGK_ButtonClicked(SGK_Button * button, int x, int y);

/*
 * SGK_FreeButton : to free a button
 * button : the button to free it
 */
void SGK_FreeButton(SGK_Button * button);

/*
 * SGK_CreateInputBox : create input to get text from user
 * ren : the renderer that used in the project
 * title : the title displayed in grey color or NULL to display "Enter text"
 * fontSize : the font size
 * length : the width of the box
 * x & y : the position of the box depend on left top of the box
 * returned value : the pointer of the created input box
 */
SGK_InputBox * SGK_CreateInputBox(SDL_Renderer * ren, char * title, int fontSize, int length, int x, int y);

/*
 * SGK_EnableInputBox : to enable or disable input box
 * ren : the renderer used in the project
 * box : the input box that will change
 * mode : 0 to disable or 1 to enable
 * returned value : the pointer to the input box if succeed 0 if fail
 */
int SGK_EnableInputBox(SDL_Renderer *ren, SGK_InputBox * box, Uint8 mode);

/*
 * SGK_FreeInputBox : to free an input box
 * box : the input box that will be free
 * returned value : 1 if succeed 0 if fail
 */
void SGK_FreeInputBox(SGK_InputBox * box);

/*
 * SGK_UpdateInputBox : to add text for the input box
 * ren : the renderer that used in the project
 * box : the input box that will be changed
 * text : the text that scanned from user to add to the input box
 * returned value: 1 if succeed 0 if fail
 */
int SGK_UpdateInputBox(SDL_Renderer * ren, SGK_InputBox * box, char *text);

/*
 * SGK_InputBoxClicked : to check if the input box clicked or not
 * box : the targeted input box
 * x & y : the position of the mouse when it was clicked
 * returned value : 0 not clicked, 1 clicked
 */
int SGK_InputBoxClicked(SGK_InputBox * box, int x, int y);

/*
 * SGK_EditInputBox : to edit text in the input box (till now it just accept the backspace)
 * ren : the renderer used in the project
 * flag : flag of the edition
 */
void SGK_EditInputBox(SDL_Renderer *ren, SGK_InputBox * box,int flag);

/*
 * SGK_GetTextFromInputBox : to get the enter text in the input box
 * box : the targeted input box
 * returned value : the text in the input box
 */
char * SGK_GetTextFromInputBox(SGK_InputBox * box);

/*
 * SGK_CreateRadioButton : to create a radio button with many choices to let user choose one
 * ren : the renderer used in the project
 * option : array of the options text
 * size : the count of the options
 * Default : the default selected choice
 * fontsize : the size of the font
 * x & y : the position of the radio button depend on the top left corner
 * returned value : the pointer to the created radio button, or 0 if fail
 */
SGK_RadioButton * SGK_CreateRadioButton(SDL_Renderer * ren, char **option, int size, int Default, int fontsize, int x, int y);

/*
 * SGK_RadioButtonClicked : to check if the radio button was clicked and which choice
 * rad : the radio button pointer
 * x & y : the position of the mouse when it was clicked
 * returned value : -1 if not clicked, or the number or the choice
 */
Sint8 SGK_RadioButtonClicked(SGK_RadioButton * rad, int x,int y);

/*
 * SGK_UpdateRadioButton : to change the selected choice
 * rad : the pointer of the radio button
 * opt : the number of the new choice
 */
void SGK_UpdateRadioButton(SDL_Renderer * ren, SGK_RadioButton * rad, Sint8 opt );

/*
 * SGK_FreeRadioButton : to free the radio button
 * rad : the pointer of the radio button
 */
void SGK_FreeRadioButton(SGK_RadioButton * rad);

/*
 * SGK_CreateCheckBox : to create a check box to let user to choose multiple options
 * ren : the renderer used in the project
 * option : is an array of options text name
 * size : the count of the options
 * fontsize : the size of the font
 * x & y : the position of the check box
 * returned value : the pointer to the created check box, or 0 if fail
 */
SGK_CheckBox * SGK_CreateCheckBox(SDL_Renderer * ren, char **option, int size, int fontsize, int x, int y);

/*
 * SGK_FreeCheckBox : to free the check box
 * box : the pointer to the check box
 */
void SGK_FreeCheckBox(SGK_CheckBox * box);

/*
 * SGK_CheckBoxClicked : to check if the check box was clicked and determine which option
 * box : the pointer to the check box
 * x & y : the position of the mouse where the mouse was clicked
 * returned value : the value of the clicked option, or -1 if not clicked
 */
Sint8 SGK_CheckBoxClicked(SGK_CheckBox * box, int x, int y);

/*
 * SGK_UpdateCheckBox : to select or deselect the option
 * ren : the renderer used in the project
 * box : the pointer of the check box
 * opt : the index of the option
 */
void SGK_UpdateCheckBox(SDL_Renderer * ren, SGK_CheckBox * box, Sint8 opt);

/*
 * SGK_PutPixel32 : to put a pixel of 32 bit on the surface
 * surface : the targeted surface
 * x & y : the position of the pixel
 * pixel the pixel that will put it
 */
void SGK_PutPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );

#endif
