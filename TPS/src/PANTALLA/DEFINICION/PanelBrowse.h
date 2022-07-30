#pragma pack (push,8) 
#ifndef __PANEL_BROWSE_H
#define __PANEL_BROWSE_H

#define MAXBROWSE 10
#pragma warning (disable:4786)
#include <string>
#include "CBrowse.h"
#include "CPanel.h"
#include "CInput.h"
#include "tpv_dat.h"
#include "CPanelBrowse.h"

extern    "C" typedef int ( *f_usu ) ( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );

    
enum{
    idBrowseClientes= 16,
    idBrowseTCompra= 16,
    idBrowseBancos= 16,
    idBrowseArticuloCodbarra,// = 22,
    idBrowseConsultaDescrip1,
    idBrowseConsultaDescrip2,
    idBrowseConsultaDescrip3,
    idBrowseConsultaDescrip4,
    idBrowseConsultaDescrip5,
    idBrowseMotivo,
    idBrowseMutuales,
    idBrowsePideCuoMut,
    idBrowsePerfil,
    idBrowsePideTarjeta,
    //idBrowseClientes,
    idBrowseFunciones,
    idBrowseInstituciones,
    idBrowseCuotasTar,
    idBrowseVigencia,
    idBrowseDiferimiento,
    idBrowseTarjeta,
    idBrowseRepactacion,
    idBrowseElegirCuotas,
    idBrowseElegirRemito,
    idBrowseDataEntry,
    idBrowseTipoCliente,
    idBrowseGruposCli,
    idBrowseCliReintegros,
};



class PanelBrowse
{
  public:
    int init();
    struct _browse browse;
    PanelBrowse( struct _dat *, int );
    ~PanelBrowse();

    CBrowse *__browse;

  private:
    int SelectedField;
    int id;
    int width;
    int numberOfFields;

    //Invel::ListAny * pRow;
    vector<string> rowString;
    vector<browse_field> fieldsDefinition;
  public:
    //void initBrowse();
    int BROWSE( int x1, int y1, int x2, int y2, int, f_usu, char *titulo, double monto );
	int BROWSE( int x1, int y1, int x2, int y2, int, f_usu, char *titulo, double monto, unsigned short cod_medio );
	int BROWSE( int x1, int y1, int x2, int y2, int, f_usu, char *titulo, int buscar, double monto, unsigned short cod_medio );
    int BROWSE_MYSQL( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario, char *titulo, double monto );    
    int BROWSE_MYSQL( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario, char *titulo, double monto, unsigned short cod_medio );
    int BROWSE_MYSQL( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario, char *titulo, int buscar, double monto, unsigned short cod_medio );
    void setFieldValueStringList( int field, char *value );
    void setFieldValue( int field, double value );
    void setFieldValue( int field, float value );
    void setFieldValue( int field, int value );
    void setFieldValue( int field, char *value );
    void setRow();
	void setRow2();
    void deleteRows( int rows );
    void insertRow( int linea );
    bool browseAny;
    void SetDefaultBrowseBrowse( CBrowse *b )
    {
        __browse = b;
    };
};
#endif
#pragma pack (pop) 
