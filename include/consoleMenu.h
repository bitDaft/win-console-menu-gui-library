
/**
 * @Author               : Tausif Ali
 * @Date                 : 30-Dec-2016
 * @Email                : anodecode@gmail.com
 * @Filename             : consoleMenu.h
 * @Last modified by     : Tausif Ali
 * @Last modified time   : 04-Jan-2017
 * @Copyright            : feel free to use, adding reference appreciated :)
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
#define MENU_SET_PREV         -1
#define HAS_PARENT            -2
#define RET_BACK              -3
#define NOT_ON_MENU           -4
#define MOUSE_LEFT_PRESS      -5
#define RET_CLEAR             -6
#define RET_SUCCESS           -7
#define RET_FAILURE           -8
#define OPT_NOT_SET           -9
#define OPT_SET_PREV          -10
#define MENU_OVERFLOW         -11
#define LINE_MENU_OVERFLOW    -12
#define MENU_NOT_SET          -13
#define MENU_EMPTY            -14

//flags passed to paintBackground func
#define ENABLE_PLAIN          1
#define ENABLE_PIC            2


//menuItemVisual's flags
#define ALIGN_LEFT          1
#define ALIGN_CENTER        2
#define ALIGN_LINE          4
#define SELECT_HIGHLIGHT    8
#define SELECT_TEXT         16
#define SELECT_BOX          32    //TODO printing box around option pending

#define ENABLE_BORDER       128


#define USE_MOUSE           1
#define USE_KEY             2 //not implemented , nor any plans to

//implementing nested menu's
#define IS_PARENT           0
#define IS_CHILD            1

/*
 * opening the application via terminal disables the reading of mouse inputs afaik
 * find method to have a new terminal opened for the application when opening via terminal
 * or find a solid method to change the mode //automatically\\ without disturbing any of the existing code
 * //it should only be limited to the menu,should not affect code or anything outside the library\\
 * may not be able to repair the code if broken
 * even i don't know how exactly most of these things work anymore
 */
class consoleMenu;
struct item;
class invMenu;
struct inventory_item;

class invMenu
{
public:

  invMenu();
  ~invMenu();
//param column names , column width , no of column,x,y,view color,serial number bool.
//use cbackDARKBLUE | cWHITE for last param as default
  int setViewOption( char **,  short *, unsigned short,int,short,short,unsigned short,bool,int =  cbackDARKBLUE | cWHITE);
  int RegView();

  int addEntry(char **);
  int finalizeView();
  int paintView();
  int selectView();

private:

  unsigned short noOfColumns;
  char **columnNames; //holds the title of each column
  short *columnWidth;
  int width,height;
  bool optSet;
  bool snCount;
  bool viewSet;
  unsigned int nrec;
  int highFlag;

  short x,y;
  unsigned short viewColor;

  inventory_item *top;

  int setColName( char **);
  int setColWidth( short *);
  void setNoOfColumns( unsigned short);
};

struct inventory_item
{
  unsigned short col;
  char **szRecord;
  short ix,iy;
  inventory_item *next;

  inventory_item();
  ~inventory_item();
};

//struct holding a menu item i guess

struct item
{
   int         midPos;
   int         length;
   short       ix, iy;
   char        *szMenuItem;

   item        *next;
   consoleMenu *mm;

   item(char*, consoleMenu*, int);
   ~item();
};

void clearScreen(short, short, unsigned long = 80*25);

class consoleMenu
{
public:
  consoleMenu(char[]);
  ~consoleMenu();

  //this is the order of calling the functions setOptions to selectOption
  //divided into groups based on similarity of function

  //options functions
  /** @param x,y coord ,menuBGch ,delay ,setBGf pbj , menuitemvisual , mouseorkey, ischildof **/
  //use cbackDARKBLUE | cWHITE for last param as default
  short setOptions(short, short,int ,int, char, int, int, int, int, int = cbackDARKBLUE | cWHITE);
  int setHW(int,int);
  void setOutcolor(unsigned short);
  void setmnBG(unsigned short);

  int RegisterOptions();
  // int newItem(char *, void (*t)(), consoleMenu*);
  int newItem(char *, consoleMenu*);

  int Mset();

  int paintBackground();
  int paintMenu();
  int selectOption();

protected:

private:
  // friend item::item(char*,void (*t)(),consoleMenu*,int);
  // static short has_parent;
  unsigned short colr, mnBG; //outside color and menu color

  bool Opts;                 //checks whether the options of a menu has been set
  bool Menuset;
  bool isChild;              //check whether it is a child
  bool HWSet;                //check whether height width set
  short x, y;                //coordinates of menu

  char *szName;              // name of menu
  char menuBGch;             // menu bg character

  int ci;
  int pbj;                   //plain or picture or animation
  int MouseOrKey;            // no idea what it does
  int displayDelay;          //menu display delay for the oohs and aahs
  int menuItemVisual;        //menu visual flag
  int cLargestMenuItem;      //count of the largest item in menu
  int menuWidth, menuHeight; // self explanatory
  int highFlag;

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
