
/**
 * @Author               : Tausif Ali <meow-man>
 * @Date                 : 18-Dec-2016
 * @Email                : anodecode@gmail.com
 * @Filename             : consoleMenu.h
 * @Last modified by     : meow-man
 * @Last modified time   : 21-Dec-2016
 * @Copyright            : stop stealing code, homo :P
 **/

#ifndef CONSOLEMENU_H
#define CONSOLEMENU_H

//colors def
#define   cBLACK              0
#define   cDARKBLUE           1
#define   cDARKGREEN          2
#define   cDARKCYAN           3
#define   cDARKRED            4
#define   cDARKMAGENTA        5
#define   cDARKYELLOW         6
#define   cGRAY               7
#define   cDARKGRAY           8
#define   cBLUE               9
#define   cGREEN              10
#define   cCYAN               11
#define   cRED                12
#define   cMAGENTA            13
#define   cYELLOW             14
#define   cWHITE              15
#define   cbackDARKBLUE       16
#define   cbackDARKGREEN      32
#define   cbackDARKCYAN       48
#define   cbackDARKRED        64
#define   cbackDARKMAGENTA    80
#define   cbackDARKYELLOW     96
#define   cbackGRAY           112
#define   cbackDARKGRAY       128
#define   cbackBLUE           144
#define   cbackGREEN          160
#define   cbackCYAN           176
#define   cbackRED            192
#define   cbackMAGENTA        208
#define   cbackYELLOW         224
#define   cbackWHITE          240

//error return defines
#define MENU_SET_PREV         7
#define HAS_PARENT            6
#define RET_BACK              5
#define NOT_ON_MENU           4
#define MOUSE_LEFT_PRESS      3
#define RET_CLEAR             2
#define RET_SUCCESS           1
#define RET_FAILURE           -1
#define OPT_NOT_SET           -2
#define OPT_SET_PREV          -4
#define MENU_OVERFLOW         -5
#define LINE_MENU_OVERFLOW    -6
#define MENU_NOT_SET          -7
#define MENU_EMPTY            -8

//flags passed to paintBackground func
#define ENABLE_PLAIN          1
#define ENABLE_PIC            2


//menuItemVisual's flags
#define ALIGN_LEFT          1
#define ALIGN_CENTER        2
#define ALIGN_LINE          4
#define SELECT_HIGHLIGHT    8
#define SELECT_TEXT         16
#define SELECT_BOX          32 //printing box around option pending

#define ENABLE_BORDER       128


#define USE_MOUSE           1
#define USE_KEY             2

//implementing nested menu's
#define IS_PARENT           1
#define IS_CHILD            2

/*
 * opening the application via terminal disables the reading of mouse inputs afaik
 * find method to have a new terminal opened for the application when opening via terminal
 * or find a solid method to change the mode //automatically\\ without disturbing any of the existing code
 * //it should only be limited to the menu,should not affect code or anything outside the library\\
 * may not be able to repair the code if broken
 * even i don't know how exactly most of these things work anymore
 */
class consoleMenu;

//struct holding a menu item i guess

struct item
{
   void        (*pf)();

   int         midPos;
   int         length;
   short       ix, iy;
   char        *szMenuItem;

   item        *next;
   consoleMenu *mm;

   item(char*, void(*t)(), consoleMenu*, int);
   ~item();
};

void clearScreen(short x, short y, unsigned long t = 80*25);

class consoleMenu
{
public:
consoleMenu(char[]);
virtual ~consoleMenu();

//this is the order of calling the functions setOptions to selectOption
//divided into groups based on similarity of function

//options functions
/** @param x,y coord ,menuBGch ,delay ,setBGf pbj , menuitemvisual , mouseorkey, ischildof **/
short setOptions(short, short, char, int, int, int, int);
void setOutcolor(unsigned short);
void setmnBG(unsigned short);

int RegisterOptions();
int Mset();

int newItem(char *, void (*t)(), consoleMenu*);

int paintBackground();
int paintMenu();
int selectOption();

protected:

private:
// friend item::item(char*,void (*t)(),consoleMenu*,int);
static short has_parent;
unsigned short colr, mnBG; //outside color and menu color

short x, y;                //coordinates of menu
short Opts;                //checks whether the options of a menu has been set
short Menuset;
short childFlag;           //check whether it is a child

char *szName;              // name of menu
char menuBGch;             // menu bg character

int ci;
int pbj;                   //plain or picture or animation
int MouseOrKey;            // no idea what it does
int displayDelay;          //menu display delay for the oohs and aahs
int menuItemVisual;        //menu visual flag
int cLargestMenuItem;      //count of the largest item in menu
int menuWidth, menuHeight; // self explanatory

item *start;
consoleMenu *isChildOf;

void setMK(int);
void setBGf(int);
void setMBG(char);
void setDelay(int);
void setChildFlag(short);
void setCoord(short, short);
void setSuper(consoleMenu*);
void setMenuItemVisual(int);
void setcolor(unsigned short f, long t, short x, short y);
};

#endif // CONSOLEMENU_H
