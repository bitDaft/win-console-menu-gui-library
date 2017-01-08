
/**
 * @Author               : Tausif Ali
 * @Date                 : 04-Jan-2017
 * @Email                : anodecode@gmail.com
 * @Filename             : consoleMenu.cpp
 * @Last modified by     : Tausif Ali
 * @Last modified time   : 08-Jan-2017
 * @Copyright            : feel free to use, adding reference appreciated :)
**/

#include "consoleMenu.h"
#include <stdio.h>
#include <windows.h>

void quit()
{
  exit(0);
}

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

void clearScreen(short x, short y, unsigned long t )
{
  COORD c = {x,y};
  FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', t, c, &t);
  FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cGRAY, t,c, &t);
  gotoxyz(x, y);
}

int handleMouse(MOUSE_EVENT_RECORD ir,
                item              *top,
                inventory_item    *stt,
                short              x,
                short              y,
                int                h,
                int                w,
                int               *o,
                int                mode,
                unsigned int       mnBG,
                int                flags)
{
  COORD cc;
  static short py = -1, f = 0;

  if (!top && !stt) {
    return RET_FAILURE;
  }

  int iy                ;
  if (top!=NULL)
    iy = top->iy;
  else iy = stt->iy;
  static int wasPressed = 0;
  DWORD t               = 1;

  if(!top)
    {
      cc = {x,py};
      if ((ir.dwMousePosition.Y > y + 1) && (ir.dwMousePosition.Y <= y + h - 1)&&
          (ir.dwMousePosition.X >= x) && (ir.dwMousePosition.X < x + w ))
        {
          f = 1;

          *o = ir.dwMousePosition.Y - iy + 1;
          if (ir.dwMousePosition.Y > y + 1 + mode && ir.dwMousePosition.Y < y + h - 1) {
            *o = -1;
          }
          else if(ir.dwMousePosition.Y == y + h - 1) *o = 0;

          if (py != ir.dwMousePosition.Y)
            {
              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w, cc, &t);
              py = ir.dwMousePosition.Y;
              cc= {x,py};
            }

          if(*o != -1)
          {
            FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flags, w, cc,&t);
          }
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
      if (mode & ALIGN_LINE)
        {
          if (ir.dwMousePosition.Y == y)
            {
              item *ptr = top;
              short ttx = ptr->ix;
              int   xx  = ttx + ptr->length;
              *o = 0;
              f  = 1;

              while (ir.dwMousePosition.X > xx)
                {
                  ptr = ptr->next;

                  if (ptr == NULL) return MENU_EMPTY;
                  *o += 1;

                  ttx = ptr->ix;
                  xx  = ttx + ptr->length;
                }
                *o+=1;

              if (ir.dwMousePosition.X == xx) {
                  ttx=-1;
                  xx=-1;
                  *o=-1;
              }
              if (py != ttx)
                {
                  cc= {x,y};
                  FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w,
                                             cc, &t);

                  py = -1;
                  *o = -1;

                if ((ir.dwMousePosition.X >= ttx) && (ir.dwMousePosition.X < xx))
                  {
                     if (mode & SELECT_TEXT)
                     {
                       flags = 0x0F & flags;
                       flags = flags | (mnBG & 0xF0);
                     }
                    cc= {ttx,y};
                    FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flags, ptr->length,cc, &t);
                    py = ttx;
                  }
                }
            }
          else if (f)
            {
              f = 0;
              cc= {x,y};
              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w, cc, &t);
              *o = -1;
              py = -1;
            }
        }
      else if ((ir.dwMousePosition.Y >= iy ) && (ir.dwMousePosition.Y < y + h - 1) &&
               (ir.dwMousePosition.X > x) && (ir.dwMousePosition.X < x + w - 1))
        {
          f = 1;

          if (mode & SELECT_BOX) *o = (ir.dwMousePosition.Y - iy) % 2 ? -1 : (ir.dwMousePosition.Y - iy + 1) / 2;
          else *o = ir.dwMousePosition.Y - iy;
          *o+=1;
          cc = {x,py};

          if ((py != ir.dwMousePosition.Y) && !(mode & SELECT_BOX))
            {
              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w - 1, cc, &t);
              py = ir.dwMousePosition.Y;
            }
          x++;

         if (mode & SELECT_TEXT)
            flags = (flags & 0x0F) | (mnBG & 0xF0);

          cc = {x,py};
          FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flags, w - 2, cc, &t);
        }
      else if (f)
        {
          f  = 0;
          *o = -1;

          if ((py >= iy) && (py < y + h - 1))
            {
              cc= {x,py};
              FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG, w - 1, cc, &t);
            }
          py = -1;
        }
    }

  if (!wasPressed && (ir.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED))
    {
      wasPressed = 1;
    }
  else if (wasPressed && (ir.dwButtonState != FROM_LEFT_1ST_BUTTON_PRESSED))
    {
      wasPressed = 0;
      return MOUSE_LEFT_PRESS;
    }
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

// void handleKey(KEY_EVENT_RECORD ir, item *k)
// {}
inventory_item::inventory_item()
{
  col = ix = iy = 0;
  szRecord  = NULL;
  next = NULL;
}

inventory_item::~inventory_item()
{
  if (szRecord) {
    for (size_t i = 0; i < col; i++)
      delete [] szRecord[i];
    delete [] szRecord;
  }
  col = ix = iy = 0;
  if (next)
    delete next;
}

invMenu::invMenu()
{
  snCount = 1;
  x = y = 0;
  width = 0;
  height = 2;
  showEnd = 0;
  columnNames = NULL;
  columnWidth = NULL;
  noOfColumns = 0 ;
  top = NULL;
  optSet = 0;
  nrec = 0;
  viewSet = 0;
  highFlag = cbackDARKBLUE | cWHITE;
}

invMenu::~invMenu()
{
  width = 0;
  height = 0;
  if(columnNames)
  {
    for (size_t i = 0; i < noOfColumns; i++)
        delete [] columnNames[i];
    delete [] columnNames;
  }
  if(columnWidth)
    delete [] columnWidth;
  noOfColumns = 0;
  if(top)
    delete top;
}

void invMenu::setHigh(int ff)
{
  highFlag = ff;
}

int invMenu::RegView()
{
  if (optSet)
    {
      return OPT_SET_PREV;
    }
  else
    {
      optSet = 1;
      return RET_SUCCESS;
    }
}

int invMenu::setViewOption(const char **colNames, short *colW, unsigned short count,int hh,
                            short tx,short ty,unsigned short clr,bool slCol,bool shEnd)
{
  if(count <= 0)
    return RET_FAILURE;
  if (optSet)
    return OPT_SET_PREV;

  int error = 0;

  if(tx < 0) tx = 0;
  if (ty < 0) ty = 0;

  viewColor = clr;
  snCount = slCol;

  x = tx;
  y = ty;

  if (y + hh > 25 ) {
    return RET_FAILURE;
  }
  if (hh < 2) {
    hh = 2;
  }
  height = hh;
  showEnd = shEnd;

  setNoOfColumns(count);
  if((error=setColWidth(colW)) != RET_SUCCESS)
    return RET_FAILURE;
  if((error=setColName(colNames)) != RET_SUCCESS)
    return RET_FAILURE;

  return RET_SUCCESS;
}

int invMenu::setColName(const char **cn)
{
  int i = 0,k;
  columnNames = new char*[noOfColumns];

  while(i < noOfColumns)
    {
      k = strlen(cn[i]);
      if(k > columnWidth[i]) k = columnWidth[i];
      columnNames[i] = new char[k+1];
      strncpy(columnNames[i],cn[i],k);
      columnNames[i][k]='\0';
      i++;
    }

  return RET_SUCCESS;
}

int invMenu::setColWidth(short *cw)
{
  if (snCount)
    width = 3;
  else
    width = 0;

  int i = 0;
  columnWidth = new short[noOfColumns];

  while(i < noOfColumns)
    {
      width += columnWidth[i] = cw[i];
      i++;
    }
    if (width > 80){
      return RET_FAILURE;}

    return RET_SUCCESS;
}

void invMenu::setNoOfColumns(unsigned short cc)
{
  noOfColumns = cc;
}

int invMenu::addEntry(const char **str)
{
  if (!optSet)
    return OPT_NOT_SET;

  inventory_item *temp = new inventory_item;

  int i = 0,k;

  temp->szRecord = new char*[noOfColumns];
  while(i < noOfColumns)
    {
      k=strlen(str[i]);
      if (k > columnWidth[i]) k = columnWidth[i];
      temp->szRecord[i] = new char[k+1];
      strncpy(temp->szRecord[i],str[i],k);
      temp->szRecord[i][k]='\0';
      i++;
    }
  if (snCount)
    temp->ix = x + 3;
  else
    temp->ix = x;
  temp->iy = y + 2 + nrec;
  temp->next = NULL;

  nrec++;
  if(temp->iy > y + height - 1)
    height++;

  if (top != NULL)
    {
      inventory_item *ptr = top;
      while(ptr->next != NULL)
        ptr = ptr->next;
      ptr->next = temp;
    }
  else
    {
      top = temp;
    }

  return RET_SUCCESS;
}

int invMenu::paintView()
{
  if (!viewSet) return OPT_NOT_SET;

  short *colCnt = columnWidth;
  inventory_item *rcd = top;
  short tx = x,ty = y;

  for (int i = 0; i < height; i++) {
    setclr(viewColor,width, tx,ty++);
  }

  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),viewColor);
  if (snCount)
    tx += 3;
  ty = y;
  for (size_t i = 0; i < noOfColumns; i++)
    {
      gotoxyz(tx,ty);
      fputs(columnNames[i],stdout);
      tx += colCnt[i];
    }

  int sk = 1;
  char sl[3];

  for (size_t j = 0; j < nrec; j++)
    {
      sprintf(sl,"%d",sk);
      tx = rcd->ix;
      ty = rcd->iy;
      if (snCount)
        {
          gotoxyz(x,ty);
          fputs(sl,stdout);
          sk++;
        }
      for (size_t i = 0; i < noOfColumns; i++)
        {
          gotoxyz(tx, ty);
          fputs(rcd->szRecord[i],stdout);
          tx += colCnt[i];
        }
      rcd = rcd->next;
    }
  if(rcd){
    tx = rcd->ix;
    ty = rcd->iy;
    // setclr(viewColor,width, x,ty);
    gotoxyz(tx,ty);
    fputs(rcd->szRecord[0],stdout);
  }

  gotoxyz(0,0);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),cGRAY);
  return RET_SUCCESS;
}

int invMenu::finalizeView()
{
  if (!optSet) return OPT_NOT_SET;

  if(showEnd)
  {
    inventory_item *temp = new inventory_item;
    char str[] = "Back";
    // nrec++;
    temp->szRecord = new char*[1];
    temp->szRecord[0] = new char[5];
    strcpy(temp->szRecord[0],str);

    temp->ix = (width%2) ? (width/2)-2 : ((width+1)/2)-2;
    temp->ix += x;
    temp->next = NULL;

    inventory_item *ptr = top;
    if (top != NULL)
      {
        while(ptr->next != NULL)
          ptr = ptr->next;
        ptr->next = temp;
      }
    else
      {
        top = temp;
        ptr = top;
      }
   if ((y + height - 1) <= ptr->iy){
      height++;
    }
    temp->iy = y + height - 1;
  }
  viewSet = 1;
  return RET_SUCCESS;
}

int invMenu::selectView()
{
  if (!viewSet)
    {
      return OPT_NOT_SET;
    }
  int optn      = -1;
  int temp      =  0;
  HANDLE rHwnd = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD ir[1];
  DWORD numOfEntries;
  COORD coo;

  ReadConsoleInput(rHwnd, ir, 1, &numOfEntries);

      switch (ir[0].EventType)
        {
        case MOUSE_EVENT:
        coo = ir[0].Event.MouseEvent.dwMousePosition;
          int tt = handleMouse(ir[0].Event.MouseEvent,NULL,top,x,y,height,width,& optn,nrec,viewColor,highFlag);
          if(tt == MOUSE_LEFT_PRESS)
            {
              if (optn != -1)  {
                if(!optn){
                  temp = 1;
                  inventory_item *pt = top;
                  while(pt->next){pt = pt->next;temp++;}
                  if (!strcmp(pt->szRecord[0],"Back")) return 0;
                  else return temp;
                }
                return optn;
              }
            }
            else if((coo.X >= x && coo.X < x+width)&&(coo.Y>=y && coo.Y < y+height))
            {
              return ON_MENU;
            }
        }
  return NOT_ON_MENU;
}

item::~item()
{
  delete[] szMenuItem;
  if (next)
    {
      delete next;
    }
  if (mm)
    {
      delete mm;
    }
}

item::item(const char* p, consoleMenu *tt, int len)
{
  length     = len;
  midPos     = length % 2 ? length / 2 : (length + 1) / 2;
  next       = NULL;
  szMenuItem = new char[length + 1];
  strcpy(szMenuItem, p);
  mm = tt;
}

int  consoleMenu::selectOption()
{
  if (!Menuset) return MENU_NOT_SET;

  int opt      = -1;
  HANDLE rHwnd = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD ir[1];
  DWORD numOfEntries;

  ReadConsoleInput(rHwnd, ir, 1, &numOfEntries);

      switch (ir[0].EventType)
        {
        case MOUSE_EVENT:

          if (MouseOrKey & USE_MOUSE)
            {
              int tt = handleMouse(ir[0].Event.MouseEvent, start,NULL, x, y,
                 (start->iy-y)+(menuItemVisual&SELECT_BOX?ci*2:ci+1),
                                  menuWidth, &opt, menuItemVisual, mnBG,highFlag);

              if (tt == MOUSE_LEFT_PRESS)
                {
                  if (opt == -1)
                    {
                      if (isChild && (isChildOf->menuItemVisual) & ALIGN_LINE)
                        {
                          return RET_CLEAR;
                        }
                      break;
                    }
                  // past here proper option has been selected
                  item *ptr = start;
                  int itNo = opt;
                  opt--;
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

                      int err = temp->paintMenu();

                      if (err != RET_SUCCESS) return err;

                       do{
                        err = temp->selectOption();
                        char chh = err | 0x20;
                        if (chh >= 'a' || chh <= 'z') {
                          return err & 0x5F;
                        }
                        if (err == RET_CLEAR) break;
                        else if (err == RET_BACK) break;
                      }while (err == NOT_ON_MENU);

                      if (!isChild||err==RET_BACK||err==RET_CLEAR) {
                        paintBackground();
                        paintMenu();
                      }
                      if (err == RET_CLEAR) break;
                      else if (err == RET_BACK) break;
                      itNo = err * 10 + itNo;

                      return itNo;
                    }
                  else
                    {
                      if (isChild)
                        {

                          if ((isChildOf->menuItemVisual) & ALIGN_LINE)
                            {
                              // clearScreen(0, 1); //more like save buufer do function restore buffer
                            }
                          else  if (ptr->next == NULL)
                            {
                              return RET_BACK;
                            }
                        }
                        else  if (!ptr->next) {
                          if (!strcmp(ptr->szMenuItem,endName)) {
                            return 0;
                          }
                        }
                      return itNo;
                    }
                }
              break;
            }

        case KEY_EVENT:

          if (MouseOrKey & USE_KEY)
            {
              // handleKey(ir[i].Event.KeyEvent, start);
              if(ir[0].Event.KeyEvent.bKeyDown)
              {
                return (int)ir[0].Event.KeyEvent.uChar.AsciiChar;
              }
              break;
            }
        }
  return NOT_ON_MENU;
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
}

consoleMenu::consoleMenu(const char *str)
{
  colr             = cGRAY;
  mnBG             = cBLUE;

  pHead            = 0;
  showEnd          = 0;
  Opts             = 0;
  Menuset          = 0;
  isChild          = 0;
  HWSet            = 0;
  x                = 0;
  y                = 0;

  endName          = NULL;
  szName           = new char[strlen(str) + 1];
  strcpy(szName, str);
  menuBGch         = ' ';

  ci               = 0;
  pbj              = 1;
  MouseOrKey       = 1;
  displayDelay     = 0;
  menuItemVisual   = 0;
  cLargestMenuItem = 0;
  menuWidth        = 4;
  menuHeight       = 4;
  highFlag         = cbackDARKBLUE | cWHITE;

  start            = NULL;
  isChildOf        = NULL;
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

int consoleMenu::setHW(int hgt,int wid)
{
  if(HWSet)
    return OPT_SET_PREV;

  menuHeight = hgt;
  menuWidth = wid;

  HWSet=1;
  return RET_SUCCESS;
}

/** @param x,y coord ,menuBGch ,delay ,setBGf pbj , menuitemvisual , mouseorkey,
 * ischildof **/
short consoleMenu::setOptions(short tx,
                              short ty,
                              int   hgt,
                              int   wid,
                              int   ddelay,
                              int   backFlag,
                              int   toMenuVisual,
                              int   mkey,
                              bool  header,
                              bool  headpos,
                              bool  shEnd,
                              const char  *end
                              )
{
  if (Opts)
    return OPT_SET_PREV;

  if(hgt > 25 || hgt < 4) hgt = 4;
  if(wid > 80 || wid < 4) wid = 4;

  if ( tx < 0 ) tx = 0;
  if ( ty < 0 ) ty = 0;

  if (tx+wid>80) wid=80-tx;
  if (ty+hgt>25) hgt=25-ty;

  if (toMenuVisual&ALIGN_LINE) {
    setHW(1,wid);
  }
  else
  {
    if (toMenuVisual&SELECT_BOX) hgt -= hgt & 1 ? 1 : 0;
    if(setHW(hgt,wid) != RET_SUCCESS) return OPT_SET_PREV;
  }

  setChildFlag(IS_PARENT);
  setMenuItemVisual(toMenuVisual);
  setCoord(tx, ty);
  setDelay(ddelay);
  setBGf(backFlag);
  setMK(mkey);

  endName = new char[strlen(end)+1];
  strcpy(endName,end);

  posHead  = headpos;
  showEnd  = shEnd;
  pHead    = header;

  return RET_SUCCESS;
}

void consoleMenu::setHigh(int ff)
{
  highFlag = ff;
}

void consoleMenu::setCoord(short tx, short ty)
{
      x = tx;
      y = ty;
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

void consoleMenu::setOutcolor(unsigned short d,unsigned char  gh)
{
  setMBG(gh);
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
      if (ci > 8) return LINE_MENU_OVERFLOW;
    }
  else
    {
      if(x<0||y<0)return MENU_OVERFLOW;
    }
  if (!(isChild && isChildOf->menuItemVisual&ALIGN_LINE))
    if (((menuWidth + x) > 80) || ((menuHeight + y) > 25)) return MENU_OVERFLOW;

  if (menuItemVisual & ALIGN_LINE)
    {
      menuItemVisual &= ~ALIGN_CENTER;
      menuItemVisual &= ~ALIGN_LEFT;
      menuItemVisual &= ~SELECT_BOX;
      menuItemVisual &= ~ENABLE_BORDER;

      if (menuItemVisual & SELECT_TEXT) menuItemVisual &= ~SELECT_HIGHLIGHT;
      else menuItemVisual |= SELECT_HIGHLIGHT;
    }
  else
    {
      if (menuItemVisual & ALIGN_LEFT) menuItemVisual &= ~ALIGN_CENTER;
      else menuItemVisual |= ALIGN_CENTER;

      if (menuItemVisual & SELECT_BOX)
        {
          menuItemVisual &= ~SELECT_TEXT;
          menuItemVisual &= ~SELECT_HIGHLIGHT;
        }
      else if (menuItemVisual & SELECT_TEXT)
        {
          menuItemVisual &= ~SELECT_HIGHLIGHT;
        }
      else
        {
          menuItemVisual |= SELECT_HIGHLIGHT;
        }
    }

  if (isChild)
    {
      if (isChildOf->menuItemVisual & ALIGN_LINE)
        {
          item *ptr=start;
          menuHeight = ci + 2;

          if (y+menuHeight>25) {
            y = y - menuHeight -1;
          }

          if (!ptr) return MENU_EMPTY;
          int i=1;
          while (ptr!=NULL)
            {
              ptr->iy = y + i;
              ptr=ptr->next;
              i++;
            }
        }
        else{
          strcpy(endName, "Back");
          newItem(endName, NULL);
        }
    }
  else
    {
      if(showEnd)
        newItem(endName, NULL);
    }

  Menuset = 1;
  remove_scroll();
  return RET_SUCCESS;
}

int consoleMenu::newItem(const char *p, consoleMenu *mm)
{
  if (!Opts) return OPT_NOT_SET;

  if (!HWSet) return OPT_NOT_SET;

  if (mm && !mm->Opts)  return OPT_NOT_SET;
  if (mm == this) return RET_FAILURE;
  if (mm && mm->isChildOf) return RET_FAILURE;

  item *ptr;

  int length = strlen(p);

  if (menuItemVisual & ALIGN_LINE)
    if (length > 9) length = 9;

  if (start == NULL)
    {
      start = new item(p, mm, length);
      ptr   = start;

      if (menuItemVisual & ALIGN_LINE)
        {
          ptr->iy = y;
        }
      else
        {
          ptr->iy = y + 3;//menuheight
        }
        ptr->ix = x;
    }
  else
    {
      ptr = start;

      while (ptr->next != NULL) ptr = ptr->next;

      ptr->next = new item(p, mm, length);

      if (menuItemVisual & ALIGN_LINE) ptr->next->iy = y;
      else if (menuItemVisual & SELECT_BOX) ptr->next->iy = ptr->iy + 2;
      else ptr->next->iy = ptr->iy + 1;

      if (menuItemVisual & ALIGN_LINE) ptr->next->ix = ptr->ix + ptr->length + 1;
      else ptr->next->ix = x;
      ptr = ptr->next;
    }

  if (mm != NULL)
    {
      mm->setOutcolor(colr,menuBGch);
      mm->setChildFlag(IS_CHILD);
      mm->setSuper(this);
      mm->setBGf(pbj);
      mm->setMK(MouseOrKey);
      mm->setmnBG(mnBG);
      mm->setDelay(displayDelay);

      if (menuItemVisual & ALIGN_LINE)
        {
          mm->setMenuItemVisual(ALIGN_LEFT | SELECT_HIGHLIGHT);
          mm->setCoord(ptr->ix, ptr->iy+1);
        }
      else
        {
          mm->setMenuItemVisual(menuItemVisual);
          mm->setCoord(x, y);
        }
      if (!(menuItemVisual & ALIGN_LINE))
      {
        if (mm->start) {
          if (mm->menuHeight < menuHeight) {
            mm->menuHeight = menuHeight;
          }
          if (mm->menuWidth < menuWidth) {
            mm->menuWidth = menuWidth;
          }
        }
        else{
          if(mm->HWSet)
          {
            mm->menuWidth = menuWidth;
            mm->menuHeight = menuHeight;
          }
          else
            mm->setHW(menuHeight,menuWidth);
        }
      }else{
        if(!mm->start)
        {
          mm->setHW(4,4);
        }
      }
    }

  ci++;

  if (menuItemVisual & ALIGN_LINE)
    {
      menuHeight = 1;
    }
  else
    {
        if (menuItemVisual & SELECT_BOX)
          {if (menuHeight-4<ci*2)
            menuHeight += 2;}
        else if(menuHeight-4<ci)menuHeight++;

    if (!(isChild && isChildOf->menuItemVisual&ALIGN_LINE))
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
      int t = ptr->ix-start->ix+length+1;
      if( t > menuWidth)
        menuWidth = t-1;
      // menuWidth += ptr->length + 1;
    }
  else if (cLargestMenuItem < ptr->length)
    {
      cLargestMenuItem = ptr->length;
      int aS = menuItemVisual&ALIGN_CENTER?4:2;
      if (cLargestMenuItem + aS > menuWidth)
      {
        menuWidth        = cLargestMenuItem + aS;
      }

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

  if (pbj & ENABLE_PLAIN)
    {
      char ch = menuBGch;
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
      cc = {tx,ty};

      setcolor(mnBG, menuWidth, tx, ty);
      FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', menuWidth, cc,&t);
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG);

      while (ptr != NULL)
        {
          tx = ptr->ix;
          gotoxyz(tx, ty);
          fputs(ptr->szMenuItem,stdout);
          ptr = ptr->next;
        }

      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cGRAY);
      gotoxyz(0, 1);
      return RET_SUCCESS;
    }
  else if (menuItemVisual & (ALIGN_CENTER | ALIGN_LEFT))
    {
      int menuCenter = menuWidth / 2;
      int yt = ty;
      if (!pHead) {
        yt+=2;
      }

      for (int i = yt; i < ty+menuHeight; i++)
      {
        setcolor(mnBG,menuWidth,tx,i);
        cc= {tx,(short)i};
          FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', menuWidth,cc,&t);
      }

      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mnBG);
      if (!(isChild && isChildOf->menuItemVisual&ALIGN_LINE) && pHead)
      {
        int headLen = strlen(szName);
        if(posHead)
          gotoxyz(((menuCenter-(headLen % 2 ? headLen / 2 : (headLen + 1)/2))+tx),ty+1);
        else
          gotoxyz(tx+1,ty+1);
        fputs(szName,stdout);
      }
      int itemStart;
      while(ptr!=NULL)
      {
        itemStart = menuItemVisual & ALIGN_CENTER ? (menuCenter - ptr->midPos) +
                        tx : tx + 1;
        ty=ptr->iy;

        Sleep(displayDelay);
        gotoxyz(itemStart,ty);
        fputs(ptr->szMenuItem,stdout);
        ptr=ptr->next;
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
