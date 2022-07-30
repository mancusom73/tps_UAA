#pragma pack (push,8) 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#ifndef BROWSE1_H
#define BROWSE1_H
#define MAXBROWSE 10
#include "InvelPOSC.h"
#include "corbabrowse.h"
extern    "C" typedef int ( *f_usu ) ( int comando, char *cadena, int dato, struct _browse *b, double monto );

class     Browse1
{
  public:
    int init();
    struct _browse browse;
    Browse1::Browse1( struct _dat *, int );

    ~Browse1();
  private:
    int SelectedField;
    int id;
    int width;
    int numberOfFields;
    Invel::ListAny *pRow;
    //Invel::ListAny row;
    Invel::ListString rowString;
    Invel::ListFields fieldsDefinition;
  public:
    //void initBrowse();
    int BROWSE( int x1, int y1, int x2, int y2, int, f_usu, char *titulo );
    void setFieldValueStringList( int field, char *value );
    void setFieldValue( int field, double value );
    void setFieldValue( int field, float value );
    void setFieldValue( int field, int value );
    void setFieldValue( int field, char *value );
    void Browse1::setRow();
    void Browse1::insertRow( int linea );

    bool browseAny;
};
#endif

#pragma pack (pop) 
