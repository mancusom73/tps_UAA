#pragma pack (push,8) 
#ifndef __CDESKTOP_H
#define __CDESKTOP_H

#pragma warning (disable:4786)

#include <string>
#include <vector>
#include <iostream>

#pragma pack(8)
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "CIcon.h"
#include "CFlag.h"
#include "CGrid.h"
#include "CPanel.h"
#include "CInput.h"
#include "CBrowse.h"
#include "CPanelBrowse.h"

////////////////////////////////////////////////////////////////
//                                                            //
//                  Error Codes                               //
//                                                            //
////////////////////////////////////////////////////////////////
#define INITIALIZE_ERR -1
#define PARSE_ERR -2
#define DOM_ERR -3
#define UNKNOWN_ERR -4


using namespace std;
XERCES_CPP_NAMESPACE_USE

////////////////////////////////////////////////////////////////
//                                                            //
//                  Auxiliar Functions                        //
//                                                            //
////////////////////////////////////////////////////////////////
XMLCh *toXMLCh( string s );
string toString( XMLCh *s );


////////////////////////////////////////////////////////////////
//                                                            //
//                  Class Declaration                         //
//                                                            //
////////////////////////////////////////////////////////////////
class CWindows;
class CText;
class CForms;
class CRepository;
class CImages;
class CWindow;
class CSize;
class CImageRef;
class CLabelRef;
class CComponents;
class CComponent;
class CComponentRef;
class CSkin;
class CImage;
class CColor;
class CIconRef;
class CLang;
class CTLabel;
class CDesktop;
class CConfigTPS;

typedef struct
{
    string type,name;
    int pos;
} DOMNodeIndex;

class CScreencfg
{
  private:
    void getLabelPanelAttribute( CTLabel *label, string &text, string &font, string &color,
                                 string &align );
    CComponent *getComponentAndPos( string name, int &x, int &y );
    CComponent *getComponentAndPos( string name, string type, int &x, int &y );
    string background;

  public:
    CWindow *window;
    CRepository *repository;
    CSkin *def;
    CLang *texts;

    CScreencfg( string winname, string skin, string lang );
    void setIconXML( CIcon *icon, string name );
    void setLabelXML( CLabel *label, string name );
    void setFlagXML( CFlag *flag1, string name, CGrid *datos );
    void setFlagXML( CFlag *flag1, string name );
    void setGridXML( CGrid *&grid, CPanel *panel, string name );
    void setInputXML( CInput *&input, CPanel *panel, string name );
    void setBrowseXML( CBrowse *&browse, CPanelBrowse *panel, CInput *input, string name, int scr );
	
    string getBackgroundXML();
    string getImage( string name );
    string getMessage( string name );

	void setBackground(string filename);
};

class CDesktop
{
  private:

    XercesDOMParser *parser;
    ErrorHandler *errHandler;
    xercesc_2_7::DOMDocument *document;


    int error;

    DOMNodeList *GetNodeByTagName( string name );
    DOMNodeList *GetChildsByTagName( string tagname, string name );
  public:
    CWindows *windows;
    CText *text;
    CForms *forms;
    CRepository *repository;
    CImages *images;


    CDesktop( string xmlname );
    ~CDesktop();

    int GetErrorCode();

    CWindows *GetWindows();
    CText *GetText();
    CForms *GetForms();
    CRepository *GetRepository();
    CImages *GetImages();
};

class CConfigTPS
{
  private:
//aqui van los tags a buscar
	  XMLCh* TAG_root;
	  XMLCh* TAG_VersionXml;
      XMLCh* TAG_SeteosTPS;
	  XMLCh* TAG_Funcionalidades;
	  XMLCh* TAG_TicketFacturaPermanente;
	  XMLCh* TAG_LecturaClienteBarra;
	  XMLCh* TAG_ControlarMediosDePago;
	  XMLCh* TAG_ClienteTarjetaEspecial;
	  XMLCh* TAG_ActivarCashPlus;
	  XMLCh* TAG_CambioMedioAutomatico;
	  XMLCh* TAG_PadronesPorFTP;
 	  XMLCh* TAG_AlicuotasEnArticulo;
	  XMLCh* TAG_NapseModalidad;
//aqui va los atributos
	  XMLCh* ATTR_Version;
	  XMLCh* ATTR_TicketFacturaPermanente;
	  XMLCh* ATTR_LecturaClienteBarra;
      XMLCh* ATTR_NombreCliente;
      XMLCh* ATTR_DniCliente;
	  XMLCh* ATTR_Prefijo_scanner_cliente;
	  XMLCh* ATTR_EncabezadoTF;
	  XMLCh* ATTR_AgruparMediosEnImpresion;
	  XMLCh* ATTR_CantMediosPermitidos;
	  XMLCh* ATTR_InformarMediosExcedidos;
	  XMLCh* ATTR_Codigo_seguridad_cliente;
	  XMLCh* ATTR_Perfil_cliente_especial;
	  XMLCh* ATTR_ValidacionCambioMediosCash;
	  XMLCh* ATTR_MinimoaHabilitar;
	  XMLCh* ATTR_HabilitarCashPlusSolo;
	  XMLCh* ATTR_MontosValidos;
	  XMLCh* ATTR_CambioMedioAutomatico;
	  XMLCh* ATTR_PedirAutorizacionCambioMedioAutomatico;
	  XMLCh* ATTR_ValidacionCambioMedioAutomatico;
	  XMLCh* ATTR_PedirAutorizacionEnCierreConCambioMedio;
	  XMLCh* ATTR_MedioSubmedioEntrante;	
	  XMLCh* ATTR_MedioSubmedioSaliente;	
	  XMLCh* ATTR_RutaFtp;
	  XMLCh* ATTR_ModificarAlicuotaDeArticuloConsFinal;

    XercesDOMParser *parser;
    ErrorHandler *errHandler;
#ifdef INVEL_L
    xercesc_3_1::DOMDocument *xmlDoc;
#else
	xercesc_2_7::DOMDocument *xmlDoc;
#endif
	char* m_OptionA;
	char* m_OptionB;
	char* m_OptionC;
    int error;

  public:
	 struct _CambioMedio
	{
		int MedioEntrante;
		int SubmedioEntrante;
		int MedioSaliente;
		int SubmedioSaliente;
	};

	  struct _config_tps2 
	{
	int versionxml;
	int activarTFpermanente;
	int encabezadoTF;
	int activarLecturaClienteBarra; 
	char NombreCliente[17];
    char DniCliente[15];
	char Prefijo_scanner_cliente[5];
	int ControlarMediosDePago;
	int AgruparMediosEnImpresion;
	int CantMediosPermitidos;
	int InformarMediosExcedidos;
	int ClienteTarjetaEspecial;
	int Codigo_seguridad_cliente;
	int Perfil_cliente_especial;
	int ActivarCashPlus;
	int ValidacionCambioMediosCash;
	int MinimoaHabilitar;
	int HabilitarCashPlusSolo;
	float MontosValidos[10];
	int CambioMedioAutomatico;
	int PedirAutorizacionCambioMedioAutomatico;
	int ValidacionCambioMedioAutomatico;
	int PedirAutorizacionEnCierreConCambioMedio;
	struct _CambioMedio det_CambioMedio[10]; 
	int PadronesPorFTP;
	char RutaFtp[50];
	int AlicuotasEnArticulo;
	int ModificarAlicuotaDeArticuloConsFinal;
	int NapseModalidad;
	}auxconfig;
/*	DOMNodeList *GetNodeByTagName( string name );
    int GetChildsByTagName(  string tagname, string name, string campo1 ,string buscar, string campo2 );
	int GetChildsByTagName2( string tagname, string *name, string campo1 ,string buscar, string campo2 );*/
    CConfigTPS( string xmlname );
    ~CConfigTPS();
    //int GetErrorCode();
	void GetConfig_tps( struct _config_tps *auxi/* struct _config_tps auxi*/ );

	
/*	void write(DOMNode* node);
	void writeElement(DOMElement* element);
	void writeText(DOMText* text) ;*/
};

class CWindows
{
  private:
    DOMNodeList *window1;
  public:
    CWindows( DOMNodeList *list );
    CWindow *GetWindowByName( string name );
    CWindow *GetWindowAt( int index );
};

class CWindow
{
  private:
    DOMNodeList *window1;
  public:
    CWindow( DOMNodeList *list );
    CSize *GetSize();
    CImageRef *GetImageRef();
    CComponents *GetComponents();
};


class CSize
{
  private:
    int width,height;
  public:
    CSize( DOMNode *node );

    int GetWidth();
    int GetHeight();
    string ToString();
};

class CColor
{
  private:
    string background,border,text;
    int hexCharToInt( char c );
    string hexColorToDecColor( string color );
  public:
    CColor( DOMNode *node );

    string GetBackground();
    string GetBorder();
    string GetText();
};

class CFont
{
  private:
    string font,style,size,name;
  public:
    CFont( DOMNode *node );

    string GetFont();
    string GetStyle();
    int GetSize();
    string ToString();
};

class CComponents
{
  private:
    DOMNodeList *components1;
  public:
    CComponents( DOMNodeList *components );
    CComponentRef *GetComponentRefByName( string name );
    CComponentRef *GetComponentAt( int index );
};

class CText
{
  private:
    DOMNodeList *list1;
  public:
    CText( DOMNodeList *list );
    CLang *GetLangByName( string name );
};

class CInputDesc
{
  private:
    DOMNodeList *list1;
  public:
    CInputDesc( DOMNodeList *list );
    CSize *GetSize();
};

class CForm
{
  private:
    DOMNodeList *list1;
  public:
    CSize *size;
    CImageRef *img;
    CLabelRef *comp;
    CInputDesc *input;

    CForm( DOMNodeList *list );
    CSize *GetSize();
    CImageRef *GetImageRef();
    CLabelRef *GetLabelRef();
    CInputDesc *GetInputDesc();
};

class CForms
{
  private:
    DOMNodeList *list1;
  public:
    CForms( DOMNodeList *list );
    CForm *GetFormByName( string name );
};

class CImages
{
  private:
    DOMNodeList *images;
  public:
    CImages( DOMNodeList *list );
    CSkin *GetSkinByName( string name );
    CSkin *GetSkinAt( int index );
};

class CSkin
{
  private:
    DOMNodeList *skin;
  public:
    CSkin( DOMNodeList *list );
    CImage *GetImageByName( string name );
    CImage *GetImageAt( int index );
};

class CMessage
{
  private:
    string message;
  public:
    CMessage( DOMNodeList *list );
    string GetMessage();
};

class CLang
{
  private:
    DOMNodeList *lang;
  public:
    CLang( DOMNodeList *list );
    CMessage *GetMessageByName( string name );
    CMessage *GetMessageAt( int index );
};

class CRepository
{
  private:
    DOMNodeList *list1;
    vector<DOMNodeIndex> gridIndex,inputIndex,labelIndex;
    vector<DOMNodeIndex> flagIndex,iconIndex;

    DOMNodeIndex GetGridIndexElement( string name );
    DOMNodeIndex GetInputIndexElement( string name );
    DOMNodeIndex GetLabelIndexElement( string name );
    DOMNodeIndex GetFlagIndexElement( string name );
    DOMNodeIndex GetIconIndexElement( string name );
    DOMNodeIndex GetIndexElement( string name, string type );
  public:
    CRepository( DOMNodeList *list );
    CComponent *GetComponentByName( string name );
    CComponent *GetComponentByName( string name, string type );
    CComponent *GetComponentAt( int name );
};

class CImage
{
  private:
    string filename;
  public:
    CImage( DOMNode *node );

    string GetFilename();
};

class CStates
{
  private:
    vector<DOMNode*> states;
  public:
    CStates( DOMNodeList *list );
    CIconRef *GetIconRefAt( int i );
    int GetCount();
};

class CColumn
{
  private:
    string align;
    int width;
  public:
    CFont *font;
    CColor *color,*selected;

    CColumn( DOMNode *node );
    CFont *GetFont();
    CColor *GetColor();
    CColor *GetSelectedColor();
    string GetAlign();
    int GetWidth();
};

////////////////////////////////////////////////////////////////
//                                                            //
//                  Reference Classes                         //
//                                                            //
////////////////////////////////////////////////////////////////
class CComponentRef
{
  public:
    string type;
    string id;
    int posx,posy;
    CComponentRef( DOMNode *node );
    void SetType( string t )
    {
        type = t;
    };
    string GetType()
    {
        return type;
    };

    virtual int GetPosx()= 0;
    virtual int GetPosy()= 0;
    virtual string GetID()= 0;
}; 

class CLabelRef : CComponentRef
{
  public:
    CLabelRef( DOMNode *node ) : CComponentRef( node )
    {
        SetType( "TLabelRef" );
    };

    string GetID();
    int GetPosx();
    int GetPosy();
};

class CInputRef : CComponentRef
{
  public:
    CInputRef( DOMNode *node ) : CComponentRef( node )
    {
        SetType( "TInputRef" );
    };

    string GetID();
    int GetPosx();
    int GetPosy();
};

class CImageRef : CComponentRef
{
  public:
    CImageRef( DOMNode *node ) : CComponentRef( node )
    {
        SetType( "TImageRef" );
    };

    string GetID();
    int GetPosx();
    int GetPosy();
};

class CFlagRef : CComponentRef
{
  public:
    CFlagRef( DOMNode *node ) : CComponentRef( node )
    {
        SetType( "TFlagRef" );
    };

    string GetID();
    int GetPosx();
    int GetPosy();
};

class CIconRef : CComponentRef
{
  public:
    CIconRef( DOMNode *node ) : CComponentRef( node )
    {
        SetType( "TIconRef" );
    };

    string GetID();
    int GetPosx();
    int GetPosy();
};

class CGridRef : CComponentRef
{
  public:
    CGridRef( DOMNode *node ) : CComponentRef( node )
    {
        SetType( "TGridRef" );
    };

    string GetID();
    int GetPosx();
    int GetPosy();
};
////////////////////////////////////////////////////////////////
//                                                            //
//                  Component Classes                         //
//                                                            //
////////////////////////////////////////////////////////////////
class CComponent
{
  public:

    CComponent()
    {
    };

    //CLabel
    virtual CFont * GetLabelFont()
    {
        return NULL;
    };
    virtual CSize * GetLabelSize()
    {
        return NULL;
    };
    virtual string GetLabelColor()
    {
        return "";
    };
    virtual string GetLabelAlign()
    {
        return "";
    };
    virtual string GetLabelText()
    {
        return "";
    };

    //CInput
    virtual CFont * GetInputFont()
    {
        return NULL;
    };
    virtual CSize * GetInputSize()
    {
        return NULL;
    };
    virtual CColor * GetInputColor()
    {
        return NULL;
    };
    virtual string GetInputAlign()
    {
        return "";
    };

    //CIcon
    virtual CFont * GetIconFont()
    {
        return NULL;
    };
    virtual CSize * GetIconSize()
    {
        return NULL;
    };
    virtual CImageRef * GetIconImageRef()
    {
        return NULL;
    };
    virtual string GetIconText()
    {
        return "";
    };
    virtual string GetIconColor()
    {
        return "";
    };
    virtual string GetIconAlign()
    {
        return "";
    };
    virtual int GetIconPosx()
    {
        return 0;
    };
    virtual int GetIconPosy()
    {
        return 0;
    };

    //CFlag
    virtual CSize * GetFlagSize()
    {
        return NULL;
    };
    virtual CStates * GetFlagStates()
    {
        return NULL;
    };
    virtual int GetFlagCurrentState()
    {
        return 0;
    };
    virtual string GetFlagShow()
    {
        return "";
    };

    //CGrid
    virtual CSize * GetGridSize()
    {
        return NULL;
    };
    virtual int GetGridRowSize()
    {
        return 0;
    };
    virtual CColumn * GetGridColumnAt( int i )
    {
        return NULL;
    };
    virtual int GetGridColumnCount()
    {
        return 0;
    };
};

class CTLabel : CComponent
{
  private:
    string align,text,color;
  public:
    CSize *size;
    CFont *font;

    CTLabel()
    {
    };
    CTLabel( DOMNode *font, DOMNode *size, DOMNode *node );
    ~CTLabel();
    virtual CFont *GetLabelFont();
    virtual CSize *GetLabelSize();
    virtual string GetLabelColor();
    virtual string GetLabelAlign();
    virtual string GetLabelText();
};

class CTInput : CComponent
{
  private:
    string align;

  public:
    CSize *size;
    CColor *color;
    CFont *font;


    CTInput( DOMNode *font, DOMNode *size, DOMNode *color, DOMNode *node );
    ~CTInput();
    virtual CFont *GetInputFont();
    virtual CSize *GetInputSize();
    virtual CColor *GetInputColor();
    virtual string GetInputAlign();
};

class CTIcon : CComponent
{
  private:
    string text,color,align;
    int x,y;
  public:
    CFont *font;
    CSize *size;
    CImageRef *imageRef;

    CTIcon( DOMNode *font1, DOMNode *size1, DOMNode *imageRef1, DOMNode *node );
    ~CTIcon();
    virtual CFont *GetIconFont();
    virtual CSize *GetIconSize();
    virtual CImageRef *GetIconImageRef();
    virtual string GetIconText();
    virtual string GetIconColor();
    virtual string GetIconAlign();
    virtual int GetIconPosx();
    virtual int GetIconPosy();
};

class CTFlag : CComponent
{
  private:
    int state;
    string show;
  public:
    CSize *size;
    CStates *states;

    CTFlag( DOMNode *size1, DOMNodeList *states1, DOMNode *node );
    ~CTFlag();
    virtual CSize *GetFlagSize();
    virtual CStates *GetFlagStates();
    virtual int GetFlagCurrentState();
    virtual string GetFlagShow();
};

class CTGrid : CComponent
{
  private:
    vector<CColumn*> columns;
    int rowSize;
    CSize *size;
  public:

    CTGrid( DOMNode *size, DOMNodeList *list1, DOMNode *node );
    virtual CSize *GetGridSize();
    virtual int GetGridRowSize();
    virtual CColumn *GetGridColumnAt( int i );
    virtual int GetGridColumnCount();
};

#endif


#pragma pack (pop) 
