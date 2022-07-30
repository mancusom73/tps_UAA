#ifndef MENU_H
#define MENU_H
extern int MENU( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum );
extern int MENU_PRESENT( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab,
                        int colum );
#endif
