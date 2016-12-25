/**
 * @Author               : Tausif Ali <meow-man>
 * @Date                 : 19-Dec-2016
 * @Email                : anodecode@gmail.com
 * @Filename             : consoleMenu.cpp
 * @Last modified by     : Tausif Ali
 * @Last modified time   : 24-Dec-2016
 * @Copyright            : stop stealing code, homo :P
**/

#include "consoleMenu.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>

// short consoleMenu::has_parent = 0;
// const unsigned short inventory_item::cnt;


void quit()
{
  exit(0);
}

void bbk()
{}

void remove_scroll()
{
  COORD c = {80,25};
  SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void gotoxyz(short x, short y)
{
  COORD c = {x,y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void clearScreen(short x, short y, unsigned long t)
{
  COORD c = {x,y};
  FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', t, c, &t);
  FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cGRAY, t,c, &t);
  gotoxyz(x, y);
}

int handleMouse(MOUSE_EVENT_RECORD ir,
                item              *top,
                short              x,
                short              y,
                int                h,
                int                w,
                int               *o,
                int                mode,
                unsigned int       mnBG,
                inventory_item      *stt)
{
  COORD cc;
  WORD flags = 0;
  static short py = -1, f = 0;

  int iy                ;
  if (top!=NULL)
    iy = top->iy;
  else iy = stt->iy;
  static int wasPressed = 0;
  DWORD t               = 1;

  if(!top)
    {
      cc = {x,py};
      if ((ir.dwMousePosition.Y > y + 1) && (ir.dwMousePosition.Y <= y + h)&&
          (ir.dwMousePosition.X >= x) && (ir.dwMousePosition.X < x + w ))
        {
          f = 1;

          *o = ir.dwMousePosition.Y - iy;

          if (py != ir.dwMousePosition.Y)
            {

              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w, cc, &t);
              py = ir.dwMousePosition.Y;
              cc= {x,py};
            }

          flags = cbackDARKBLUE | cWHITE;

          FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flags, w, cc,&t);

        }
      else if (f)
        {
          f  = 0;
          *o = -1;

          if ((py > y + 1) && (py <= y + h ))
            {
              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w, cc, &t);
            }
          py = -1;
        }
    }
  else
    {
      //    if (mode & SELECT_BOX)
      //    {
      //        h-=3;
      //        h*=2;
      //        h+=3;
      //    }
      if (mode & ALIGN_LINE)
        {
          if (ir.dwMousePosition.Y == 0)
            {
              item *ptr = top;
              short ttx = ptr->ix;
              int   xx  = ttx + ptr->length;
              *o = 0;
              f  = 1;

              while (ir.dwMousePosition.X > xx)
                {
                  ptr = ptr->next;
                  *o += 1;

                  if (ptr == NULL) return MENU_EMPTY;

                  ttx = ptr->ix;
                  xx  = ttx + ptr->length;
                }

              if (py != ttx)
                {
                  cc= {0,0};
                  FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, 80,
                                             cc, &t);

                  py = -1;
                  *o = -1;

                  if ((ir.dwMousePosition.X >= ttx) && (ir.dwMousePosition.X < xx))
                    {
                      if (mode & SELECT_HIGHLIGHT)
                        flags = cbackDARKBLUE | cWHITE;
                      else if (mode & SELECT_TEXT)
                        flags = cWHITE | mnBG;
                      cc= {ttx,0};
                      FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flags, ptr->length,cc, &t);
                      py = ttx;
                    }
                }
            }
          else if (f)
            {
              f = 0;

              //            if(py==0)
              cc= {0,0};
              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, 80, cc, &t);

              *o = -1;
              py = -1;
            }
        }
      else if ((ir.dwMousePosition.Y > y + 2) && (ir.dwMousePosition.Y < y + h - 1) &&
               (ir.dwMousePosition.X > x) && (ir.dwMousePosition.X < x + w - 1))
        {
          f = 1;

          if (mode & SELECT_BOX) *o = (ir.dwMousePosition.Y - iy) % 2 ? -1 : (ir.dwMousePosition.Y - iy + 1) / 2;
          else *o = ir.dwMousePosition.Y - iy;
          cc= {x,py};

          if ((py != ir.dwMousePosition.Y) && !(mode & SELECT_BOX))
            {
              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w - 1, cc, &t);
              py = ir.dwMousePosition.Y;
              cc= {x,py};
            }
          x++;

          if (mode & SELECT_HIGHLIGHT)
            flags = cbackDARKBLUE | cWHITE;
          else if (mode & SELECT_TEXT)
            flags = cWHITE | mnBG;

          FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flags, w - 2, cc, &t);
        }
      else if (f)
        {
          f  = 0;
          *o = -1;


          if ((py > y + 2) && (py < y + h - 1))
            {
              cc= {x,py};
              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w - 1, cc, &t);
            }
          py = -1;
        }
    }

  //    if (ir.dwEventFlags == MOUSE_MOVED) gotoxyz(ir.dwMousePosition.X,
  //                ir.dwMousePosition.Y);
  if (!wasPressed && (ir.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED))
    {
      wasPressed = 1;
    }
  else if (wasPressed && (ir.dwButtonState != FROM_LEFT_1ST_BUTTON_PRESSED))
    {
      wasPressed = 0;
      return MOUSE_LEFT_PRESS;
    }

  //    if(ir.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
  //    {
  //        if(!wasPressed)
  //            return MOUSE_LEFT_PRESS;
  //        else wasPressed=1;
  //    }else wasPressed=0;

  return RET_SUCCESS;
}

void setclr(unsigned short m = cGRAY,
            long           t = 25 *80,
            short          x = 0,
            short          y = 0)
{
  COORD c= {x,y};
  FillConsoleOutputAttribute(GetStdHandle(
                               STD_OUTPUT_HANDLE), m, t, c,
                             (PDWORD)&t);
}

void handleKey(KEY_EVENT_RECORD ir, item *k)
{}

invMenu::invMenu()
{
  snCount=1;
  x=y=0;
  width=0;
  height=0;
  columnNames=NULL;
  columnWidth=NULL;
  noOfColumns=0;
  top=NULL;
  optSet=0;
  nrec=0;
  viewSet=0;
}

invMenu::~invMenu()
{
  width=0;
  height=0;
  int i=0;
  for (i = 0; i < noOfColumns; i++)
    {
      delete [] columnNames[i];
    }
  delete [] columnNames;
  delete [] columnWidth;
  noOfColumns=0;
  delete top;
}

int invMenu::RegView()
{
  if (optSet)
    {
      return OPT_SET_PREV;
    }
  else
    {
      optSet=1;
      return RET_SUCCESS;
    }
}

int invMenu::setViewOption( char **colNames, short *colW, unsigned short count,
                            short tx,short ty,unsigned short clr,bool slCol)
{
  if(count <= 0)
    return RET_FAILURE;
  if (optSet)
    return OPT_SET_PREV;

  if(tx<0) tx=0;
  if (ty<0) ty=0;


  viewColor=clr;
  snCount=slCol;

  x=tx;
  y=ty;

  setNoOfColumns((const unsigned short)count);
  setColWidth((const short *)colW);
  setColName((const char **)colNames);
  return RET_SUCCESS;
}

void invMenu::setColName(const char **cn)
{
  int i=0;
  columnNames=new char*[noOfColumns];

  while(i < noOfColumns)
    {
      columnNames[i]=new char[strlen(cn[i])+1];
      strcpy(columnNames[i],cn[i]);
      i++;
    }
  height=1;
}
void invMenu::setColWidth(const short *cw)
{
  if (snCount)
    width=2;

  int i=0;
  columnWidth=new short[noOfColumns];
  while(i < noOfColumns)
    {
      width+=columnWidth[i]=cw[i];
      i++;
    }
}

void invMenu::setNoOfColumns(const unsigned short cc)
{
  noOfColumns=cc;
}

int invMenu::addEntry(char **str)
{
  if (!optSet)
    return OPT_NOT_SET;

  inventory_item *temp=new inventory_item;

  int i=0;
  height++;
  nrec++;

  temp->szRecord=new char*[noOfColumns];
  while(i<noOfColumns)
    {
      temp->szRecord[i]=new char[strlen(str[i])+1];
      strncpy(temp->szRecord[i],str[i],columnWidth[i]);
      i++;
    }
  if (snCount)
    temp->ix=x+2;
  else
    temp->ix=x;
  temp->iy=y+height;
  temp->next=NULL;

  if (top!=NULL)
    {
      inventory_item *ptr=top;
      while(ptr->next!=NULL)
        ptr=ptr->next;
      ptr->next=temp;
    }
  else
    {
      top=temp;
    }

  return RET_SUCCESS;
}

int invMenu::paintView()
{
  if (!viewSet)
    {
      return OPT_NOT_SET;
    }
  COORD cc;
  DWORD t=0;
  short *colCnt=columnWidth;
  inventory_item *rcd=top;
  short tx=x,ty=y;
  std::cout << nrec << '\n';
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),viewColor);
  setclr(viewColor,width, x,y);
  if (snCount)
    tx+=2;
  gotoxyz(tx,ty);
  for (size_t i = 0; i < noOfColumns; i++)
    {
      puts(columnNames[i]);
      gotoxyz(tx+=colCnt[i],ty);
    }

  int sk=1;
  char sl[3];

  for (size_t j = 0; j < nrec; j++)
    {
      sprintf(sl,"%d",sk);
      tx=rcd->ix;
      ty=rcd->iy;
      if (snCount)
        {
          cc= {x,ty};
          FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE),sl[0],1,cc,&t);
          // if(sl[1]>'0')
          //   FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE),sl[1],1,(COORD){short(x+1),ty},&t);
          sk++;
        }
      setclr(viewColor,width, x,ty);
      gotoxyz(tx,ty);
      for (size_t i = 0; i < noOfColumns; i++)
        {
          puts(rcd->szRecord[i]);
          gotoxyz(tx+=colCnt[i],ty);
        }
      rcd=rcd->next;
    }
  tx=rcd->ix;
  ty=rcd->iy;
  setclr(viewColor,width, x,ty);
  gotoxyz(tx,ty);
  puts(rcd->szRecord[0]);

  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),cGRAY);
  return RET_SUCCESS;
}

int invMenu::finalizeView()
{
  inventory_item *temp=new inventory_item;
  char str[]="back";
  height++;
  // nrec++;
  temp->szRecord=new char*[1];
  temp->szRecord[0]=new char[5];
  strcpy(temp->szRecord[0],str);

  temp->ix=(width%2)?(width/2)-2:((width+1)/2)-2;
  temp->ix+=x;
  temp->iy=y+height;
  temp->next=NULL;

  if (top!=NULL)
    {
      inventory_item *ptr=top;
      while(ptr->next!=NULL)
        ptr=ptr->next;
      ptr->next=temp;
    }
  else
    {
      top=temp;
    }

  if (optSet)
    {
      viewSet=1;
    }
  else return OPT_NOT_SET;
  return RET_SUCCESS;
}

int invMenu::selectView()
{
  if (!viewSet)
    {
      return OPT_NOT_SET;
    }
  int optn      = -1;
  HANDLE rHwnd = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD ir[512];
  DWORD numOfEntries;


  GetNumberOfConsoleInputEvents(rHwnd, &numOfEntries);

  if (!numOfEntries)
    {
      HANDLE wHwnd = GetStdHandle(STD_INPUT_HANDLE);
      ir[0].EventType = MENU_EVENT;
      WriteConsoleInput(wHwnd, &ir[0], 1, &numOfEntries);
    }
  ReadConsoleInput(rHwnd, ir, 512, &numOfEntries);

  for (register unsigned int i = 0; i < numOfEntries; i++)
    {
      switch (ir[i].EventType)
        {
        case MOUSE_EVENT:
          int tt = handleMouse(ir[i].Event.MouseEvent,NULL,x,y,height,width,& optn,0,viewColor,top);
          if(tt==MOUSE_LEFT_PRESS)
            {
              if (optn!=-1)
                return optn;
            }
        }
    }
  return NOT_ON_MENU;
}

item::~item()
{
  delete[] szMenuItem;
  pf=NULL;
  if (next)
    {
      delete next;
    }
  if (mm)
    {
      delete mm;
    }
}

item::item(char p[], void(*func)(), consoleMenu *tt, int len)
{
  length     = len;
  midPos     = length % 2 ? length / 2 : (length + 1) / 2;
  next       = NULL;
  szMenuItem = new char[length + 1];
  strcpy(szMenuItem, p);

  if (tt != NULL)
    {
      mm = tt;
      pf = NULL;
    }
  else
    {
      pf = func;
      mm = NULL;
    }
}


int  consoleMenu::selectOption()
{
  if (!Menuset) return MENU_NOT_SET;

  int opt      = -1;
  HANDLE rHwnd = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD ir[512];
  DWORD numOfEntries;


  //    while(1)
  //    {
  GetNumberOfConsoleInputEvents(rHwnd, &numOfEntries);

  if (!numOfEntries && menuItemVisual & ALIGN_LINE)
    {
      HANDLE wHwnd = GetStdHandle(STD_INPUT_HANDLE);
      ir[0].EventType = MENU_EVENT;
      WriteConsoleInput(wHwnd, &ir[0], 1, &numOfEntries);
    }
  ReadConsoleInput(rHwnd, ir, 512, &numOfEntries);

  for (register unsigned int i = 0; i < numOfEntries; i++)
    {
      //            int kk=0;
      //            if(menuItemVisual&ALIGN_LINE)kk=1;
      //            if(kk)
      //            {
      //                if(ir[i].EventType==MOUSE_EVENT)
      //                {
      //                    if(ir[i].Event.MouseEvent.dwMousePosition.Y!=0)return
      // NOT_ON_MENU;
      //                }
      //                else
      //                {
      //                    return NOT_ON_MENU;
      //                }
      //            }

      switch (ir[i].EventType)
        {
        case MOUSE_EVENT:

          if (MouseOrKey & USE_MOUSE)
            {
              int tt = handleMouse(ir[i].Event.MouseEvent, start, x, y, menuHeight, menuWidth, &opt, menuItemVisual, mnBG,NULL);

              if (tt == MOUSE_LEFT_PRESS)
                {
                  item *ptr = start;

                  if (opt == -1)
                    {
                      if (isChild && (isChildOf->menuItemVisual) & ALIGN_LINE)
                        {
                          // this function is to remove the secondary menu created
                          // after opening from line menu
                          // instead of clearing screen ,have the stored output buffer
                          // restored

                          // clearScreen(0,1);       // change these 3  to store the input
                          // buffer
                          isChildOf->paintBackground(); // and then restore it ,it will
                          // allow to work
                          isChildOf->paintMenu();       // with pictures as backgrounds and
                          // real time
                          // menu's

                          return RET_CLEAR;
                        }
                      break;
                    }

                  // past here proper option has been selected
                  while (opt-- != 0) ptr = ptr->next;

                  // call assosciated function
                  //                        if(menuItemVisual&ALIGN_LINE)
                  //                        {
                  //                            //TODO
                  //                            //save the current output buffer to have
                  // it restored if required
                  //                        }
                  //                        else
                  //                        {
                  //                            //clearScreen(0,0);
                  //                            //do something
                  //                            //also about line selected child menu
                  //                        }
                  if (ptr->mm != NULL)
                    {
                      consoleMenu *temp = ptr->mm;

                      if (!(menuItemVisual & ALIGN_LINE)) temp->paintBackground();

                      short err = temp->paintMenu();

                      if (err != RET_SUCCESS) return err;

                      err = temp->selectOption();

                      if (err == RET_CLEAR) break;
                      else if (err == RET_BACK) break;
                      else if (err != RET_SUCCESS) return err;
                    }
                  else
                    {
                      if (isChild)
                        {
                          if ((isChildOf->menuItemVisual) & ALIGN_LINE)
                            {
                              clearScreen(0, 1); //more like save buufer do function restore buffer
                            }
                          else
                            {
                              if (ptr->next == NULL)
                                {
                                  // clearScreen(0,0);
                                  isChildOf->paintBackground();
                                  isChildOf->paintMenu();
                                  return RET_BACK;
                                }
                            }
                        }
                      ptr->pf();
                    }
                  return RET_SUCCESS;
                }
              break;
            }

        case KEY_EVENT:

          if (MouseOrKey & USE_KEY)
            {
              handleKey(ir[i].Event.KeyEvent, start);
              break;
            }

        default:
          break;
        }

      //        }
    }
  return RET_SUCCESS;
}

void consoleMenu::setcolor(unsigned short m = cGRAY,
                           long           t = 25 *80,
                           short          x = 0,
                           short          y = 0)
{
  COORD cc= {x,y};
  FillConsoleOutputAttribute(GetStdHandle(
                               STD_OUTPUT_HANDLE), m, t, cc,
                             (PDWORD)&t);
  // gotoxyz(0, 0);
}

consoleMenu::consoleMenu(char *str)
{
  Opts = 0;

  /** @param x,y coord ,menuBGch ,delay ,setBGf pbj , menuitemvisual ,
   * mouseorkey, ischildof **/

  // setOptions(0, 0,' ', 0, ENABLE_PLAIN, ALIGN_CENTER | SELECT_HIGHLIGHT,
  // USE_MOUSE);

  start            = NULL;
  cLargestMenuItem = 0;
  menuWidth        = 4;
  menuHeight       = 4;
  colr             = cGRAY;
  mnBG             = cBLUE;
  szName           = new char[strlen(str) + 1];
  strcpy(szName, str);
  isChild=0;
  Menuset   = 0;
  ci        = 0;
  isChildOf = NULL;
  menuBGch  = ' ';
}

int consoleMenu::RegisterOptions()
{
  if (Opts)
    {
      return OPT_SET_PREV;
    }
  else
    {
      Opts = 1;
      return RET_SUCCESS;
    }
}

/** @param x,y coord ,menuBGch ,delay ,setBGf pbj , menuitemvisual , mouseorkey,
 * ischildof **/
short consoleMenu::setOptions(short tx,
                              short ty,
                              char  gh,
                              int   ddelay,
                              int   backFlag,
                              int   toMenuVisual,
                              int   mkey)
{
  if (Opts)
    return OPT_SET_PREV;

  // if (consoleMenu::has_parent) return HAS_PARENT;

  if ( tx < 0 ) tx=0;
  if ( ty < 0 ) ty=0;

  setChildFlag(IS_PARENT);
  setMenuItemVisual(toMenuVisual);
  setCoord(tx, ty);
  setDelay(ddelay);
  setBGf(backFlag);
  setMK(mkey);
  setMBG(gh);
  // consoleMenu::has_parent = 1;

  return RET_SUCCESS;
}

void consoleMenu::setCoord(short tx, short ty)
{
  if (menuItemVisual & ALIGN_LINE)
    {
      x = 0;
      y = 0;
    }
  else
    {
      x = tx;
      y = ty;
    }
}

void consoleMenu::setDelay(int d)
{
  displayDelay = d;
}

void consoleMenu::setBGf(int x)
{
  pbj = x;
}

void consoleMenu::setMK(int x)
{
  MouseOrKey = x;
}

void consoleMenu::setMBG(char gh)
{
  menuBGch = gh;
}

void consoleMenu::setMenuItemVisual(int mode)
{
  menuItemVisual = mode;
}

void consoleMenu::setChildFlag(short k)
{
  isChild = k;
}

void consoleMenu::setOutcolor(unsigned short d)
{
  colr = d;
}

void consoleMenu::setmnBG(unsigned short d)
{
  mnBG = d;
}

void consoleMenu::setSuper(consoleMenu *m)
{
  isChildOf = m;
}

consoleMenu::~consoleMenu()
{
  // dtor
  delete szName;
  isChildOf=NULL;
  if (start)
    delete start;

}

int consoleMenu::Mset()
{
  if (!Opts) return OPT_NOT_SET;

  if (Menuset) return MENU_SET_PREV;

  if (menuItemVisual & ALIGN_LINE)
    {
      menuItemVisual &= ~ALIGN_CENTER;
      menuItemVisual &= ~ALIGN_LEFT;
      menuItemVisual &= ~SELECT_BOX;
      menuItemVisual &= ~ENABLE_BORDER;

      if (menuItemVisual & SELECT_HIGHLIGHT) menuItemVisual &= ~SELECT_TEXT;
      else menuItemVisual |= SELECT_TEXT;
    }
  else
    {
      if (menuItemVisual & ALIGN_CENTER) menuItemVisual &= ~ALIGN_LEFT;
      else menuItemVisual |= ALIGN_LEFT;


      if (menuItemVisual & SELECT_HIGHLIGHT)
        {
          menuItemVisual &= ~SELECT_TEXT;
          menuItemVisual &= ~SELECT_BOX;
        }
      else if (menuItemVisual & SELECT_TEXT)
        {
          menuItemVisual &= ~SELECT_BOX;
        }
      else
        {
          menuItemVisual |= SELECT_BOX;
        }
    }

  char p[5] = "exit";

  Menuset = 1;

  if (isChild)
    {
      if (!(isChildOf->menuItemVisual & ALIGN_LINE))
        {
          strcpy(p, "back");
          newItem(p, bbk, NULL);
        }
    }
  else
    {
      newItem(p, quit, NULL);
    }

  if (menuItemVisual & ALIGN_LINE)
    {
      if (ci > 10) return LINE_MENU_OVERFLOW;
    }
  else
    {
      if (((menuWidth + x) > 80) || ((menuHeight + y) > 25)) return MENU_OVERFLOW;
    }

  remove_scroll();
  return RET_SUCCESS;
}

int consoleMenu::newItem(char *p, void (*t)(), consoleMenu *mm)
{
  if (!Opts) return OPT_NOT_SET;

  if ((t == NULL) && (mm == NULL)) return RET_FAILURE;

  if (mm && !mm->Opts)  return OPT_NOT_SET;

  item *ptr;

  int length = strlen(p);

  if (menuItemVisual & ALIGN_LINE)
    if (length > 9) length = 9;

  if (start == NULL)
    {
      start = new item(p, t, mm, length);
      ptr   = start;

      if (menuItemVisual & ALIGN_LINE)
        {
          ptr->iy = ptr->ix = 0;
        }
      else
        {
          ptr->iy = y + 3;
          ptr->ix = x;
        }
    }
  else
    {
      ptr = start;

      while (ptr->next != NULL) ptr = ptr->next;

      ptr->next = new item(p, t, mm, length);

      if (menuItemVisual & ALIGN_LINE) ptr->next->iy = 0;
      else if (menuItemVisual & SELECT_BOX) ptr->next->iy = ptr->iy + 2;
      else ptr->next->iy = ptr->iy + 1;

      if (menuItemVisual & ALIGN_LINE) ptr->next->ix = ptr->ix + ptr->length + 1;
      else ptr->next->ix = x;
      ptr = ptr->next;
    }

  if (mm != NULL)
    {
      mm->setOutcolor(colr);
      mm->setChildFlag(IS_CHILD);
      mm->setSuper(this);
      mm->setBGf(pbj);
      mm->setMK(MouseOrKey);
      mm->setmnBG(mnBG);
      mm->setMBG(menuBGch);
      mm->setDelay(displayDelay);

      if (menuItemVisual & ALIGN_LINE)
        {
          mm->setMenuItemVisual(ALIGN_LEFT | SELECT_HIGHLIGHT);
          mm->setCoord(ptr->ix, 1);
        }
      else
        {
          mm->setMenuItemVisual(menuItemVisual);
          mm->setCoord(x, y);
        }
    }

  ci++;

  if (menuItemVisual & ALIGN_LINE)
    {
      menuHeight = 1;
    }
  else
    {
      if (menuItemVisual & SELECT_BOX) menuHeight += 2;
      else menuHeight++;

      if ((y + menuHeight) > 25)
        {
          int   df = 25 - menuHeight, ff;
          item *ptrq = start;
          ff = y - df;
          y  = df;

          while (ptrq != NULL)
            {
              ptrq->iy -= ff;
              ptrq      = ptrq->next;
            }
        }
    }

  if (menuItemVisual & ALIGN_LINE)
    {
      menuWidth = 80;
    }
  else if (cLargestMenuItem < ptr->length)
    {
      cLargestMenuItem = ptr->length;
      menuWidth        = cLargestMenuItem + 4;

      int diff = 0;
      diff = x + menuWidth;

      if (diff > 80)
        {
          diff = 80 - menuWidth;
          x    = diff;
          ptr  = start;

          while (ptr != NULL)
            {
              ptr->ix = diff;
              ptr     = ptr->next;
            }
        }
    }

  return RET_SUCCESS;
}

int consoleMenu::paintBackground()
{
  if (!Menuset) return MENU_NOT_SET;

  char ch = menuBGch;

  if (pbj & ENABLE_PLAIN)
    {
      HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
      DWORD  t = 80 * 25;
      COORD  d = { 0, 0 };

      setcolor(colr);
      FillConsoleOutputCharacter(h, ch, t, d, &t);

      return RET_SUCCESS;
    }
  else if (pbj & ENABLE_PIC)
    {
      gotoxyz(0, 0);
      return RET_SUCCESS;
    }
  else
    {
      return RET_FAILURE;
    }
}

int consoleMenu::paintMenu()
{
  if (!Menuset) return MENU_NOT_SET;

  item *ptr = start;

  if (!ptr) return MENU_EMPTY;

  register short tx = x, ty = y;
  DWORD t = 1;
  COORD cc;

  if (menuItemVisual & ALIGN_LINE)
    {
      tx = ptr->ix;
      ty = ptr->iy;
      setcolor(mnBG, 80, tx, ty);
      cc= {tx,ty};
      FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80, cc,&t);
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG);
      char q[30] = "";

      while (ptr != NULL)
        {
          tx = ptr->ix;
          gotoxyz(tx, ty);
          strcpy(q, ptr->szMenuItem);
          strcat(q, " ");
          puts(q);
          ptr = ptr->next;
        }

      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cGRAY);
      gotoxyz(0, 1);
      return RET_SUCCESS;
    }
  else if (menuItemVisual & (ALIGN_CENTER | ALIGN_LEFT))
    {
      int menuCenter = menuWidth / 2;
      setcolor(mnBG, menuWidth, tx, ty);

      cc= {tx,ty};
      FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', menuWidth,
                                 cc,&t);
      ty++;
      setcolor(mnBG, menuWidth, tx, ty);
      cc= {tx,ty};
      FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', menuWidth,
                                 cc, &t);

      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG);
      gotoxyz(tx, ty);
      char q[20] = " ";
      strcat(q, szName);
      puts(q);
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cGRAY);
      ty++;
      setcolor(mnBG, menuWidth, tx, ty);
      cc= {tx,ty};
      FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', menuWidth,
                                 cc, &t);
      ty++;
      setcolor(mnBG, menuWidth, tx, ty);
      cc= {tx,ty};
      FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', menuWidth,
                                 cc, &t);

      int itemStart;
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG);

      while (ptr != NULL)
        {
          gotoxyz(tx, ty);
          Sleep(displayDelay);

          itemStart = menuItemVisual & ALIGN_CENTER ? (menuCenter - ptr->midPos) +
                      tx : tx + 1;
          gotoxyz(itemStart, ty);
          puts(ptr->szMenuItem);
          ty++;

          //            gotoxyz(tx,ty);
          setcolor(mnBG, menuWidth, tx, ty);
          cc= {tx,ty};
          FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', menuWidth,cc, &t);
          if (menuItemVisual & SELECT_BOX)
            {
              ty++;
              setcolor(mnBG, menuWidth, tx, ty);
              cc= {tx,ty};
              FillConsoleOutputCharacter(GetStdHandle(
                                           STD_OUTPUT_HANDLE), ' ', menuWidth, cc, &t);
            }
          ptr = ptr->next;
        }
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cGRAY);

      if (menuItemVisual & ENABLE_BORDER)
        {
          // TODO border drawing
        }
      gotoxyz(0, 0);

      return RET_SUCCESS;
    }
  else
    {
      return RET_FAILURE;
    }
}
