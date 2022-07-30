#pragma pack (push,8) 
#ifndef __BROWSE_H
#define __BROWSE_H

#include <config.h>

#if INVEL_W == INVEL_W32_VISUALC

extern int Menu( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum );
extern void setFieldValueInt( int index, int field, int value );
extern void setFieldValueFloat( int index, int field, float value );
extern void setFieldValueDouble( int index, int field, double value );
extern void setFieldValuepChar( int index, int field, char *value );
#endif
#endif

#pragma pack (pop) 
