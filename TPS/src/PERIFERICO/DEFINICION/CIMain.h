#pragma pack (push,8) 
#ifndef __CI_MAIN_H
#define __CI_MAIN_H
#define Uses_wxThread
//#pragma warning(disable:4786)
#include <wx/wx.h>
#include <string>
#include <vector>
#include <list>
#include <wx/hashmap.h>
#include <wx/tokenzr.h>
#include "PCom.h"

using namespace std;

#define FROM_SCANNER             1
#define FROM_MSR                 2
#define FROM_CASH_DRAWER         3
#define FROM_POS_PRINTER         4
#define FROM_LINE_DISPLAY        5

#define TO_POS_PRINTER_JOURNAL         1
#define TO_POS_PRINTER_RECEIPT         2
#define TO_POS_PRINTER_JOURNAL_RECEIPT 3
#define TO_POS_PRINTER_JOURNAL_PART    31
#define TO_POS_PRINTER_RECEIPT_PART    32
#define TO_POS_PRINTER_SLIP            4
#define TO_FISCAL_PRINTER_JRN          5
#define TO_FISCAL_PRINTER_REC          6
#define TO_FISCAL_PRINTER_REC_JRN      7
#define TO_FISCAL_PRINTER_SLP          8

#define JOURNAL_PAPER_OUT              1
#define RECEIPT_PAPER_OUT              2
#define JOURNAL_RECEIPT_PAPER_OUT      3
#define PRINTER_COVER_OPEN             4
 
#define FROM_MSR_TRACK1          8
#define FROM_MSR_TRACK2          9
#define TO_SCANNER               10
#define TO_POS_PRINTER           11
#define TO_DISPLAY_LINE_1        12
#define TO_DISPLAY_LINE_2        13
#define TO_DISPLAY_CLEAR         14
#define TO_FISCAL_PRINTER        15

#define PTR_CCS_ALPHA    1
#define PTR_CCS_ASCII    998
#define PTR_CCS_KANA     10
#define PTR_CCS_KANJI    11
#define PTR_CCS_UNICODE  997

#define COMM_LINE_BREAK  "}"
#define COMM_SEPARATOR   "{"

WX_DECLARE_STRING_HASH_MAP(wxString, StrStrHashMap);

// 30/04/2009 : Separo los metodos que actuan sobre los buffers en clases auxiliares:

class ScannerOut
{

private:
    
    bool scannerEnabled;
    vector<wxString> data_out_scanner;

public:
    
    int PutData( wxString enable );
    wxString GetData();
    bool GetNotEmpty();
    void SetEnabled();

};


class CIMain : public wxThread
{
  private:
    wxMutex *mutex_com;
    bool msrEnabled;
    bool cashDrawerEnabled;
    bool posPrinterEnabled;
	bool existePosPrinter;
    bool displayEnabled;
    bool scaleEnabled;
    bool micrEnabled;
    bool fiscalPrinterEnabled;

    vector<pair<int, string> > data_in; //MSR
    vector<pair<int, string> > data_out_print;
    vector<pair<int, string> > data_out_display;
    vector<string> lines;

    bool non_fiscal_ok;

    PCom *jposCom;
    string print_data;
    int print_station;
    int print_pos;
    int print_letter;
    int printer_status;
    int printer_empty;
    int x_pos,y_pos;
    int imageWidth, imageHeight;
    int printerCharset;
    bool connect; //determina si se debe conectar al Javaserver

    int charFilter( unsigned char *c );

  public:    
    
    ScannerOut scanOut;
   bool habilitapingjserver ;
    
    CIMain( char *ipserv ) : wxThread( wxTHREAD_DETACHED )
    {
        Init( ipserv );
    };
    void Init( char *ipserv );
    void *Entry();
    bool PCommand( const char *command, StrStrHashMap &result, bool mutex );
    bool PCommand( const char *method, const wxArrayString &params, const char *types, StrStrHashMap &result, bool mutex );
    void pingJavaServer();
    bool IsEmpty( int from );
    bool IsOutput( int from );
    //string GetMsrTrack1();
    //string GetMsrTrack2();
    int getCharSet();
    int getPrinterStation();
    int getPrinterLetter();
    int GetScanType();
    int HabilitarMsr( wxString enable );
    int OpenDrawer( wxString name );
    bool GetDrawerOpened( wxString name );
    int WaitForDrawerClose( wxString name );
	int WaitForPositionKeyLock( int position );
	int getPositionKeyLock( int position );
	int getPositionCountKeyLock( int position );
	int cleanBufferScanner( );
	int getScanner( char *temp );
	int isScanner( int d );
	int printBarCode( int position );
	int SetPrintBitmap( /*char *buffer*/ wxString buffer );
	int SetPrintBarCode(/*char *buffer*/ wxString buffer );
	void setExistePosPrinter( char *buffer );
	int getErrorPrinter( );
    int SetDataToPrint( int station, string data );
    pair <int, string> GetDataToPrint();
    int GetProxStation();
    int OpenDevices( wxString peripherals );
    void Print();
    void Print( int pos );
    void PrintLF( char *, int );
    void PrintCutPaper(int imprimir);
    int setModoImpresionFinal(int modo);
    void ForcePrint();
    void PrintMode( int mode );
	void PrintBarCode( );
	void PrintBitmap( char *buffer );
    void PrintSelect( int station );
    void PrintAlign( int align );
    int GetPrinterStatus();
    int GetSlpEmpty();
    int GetPrinterFinish();
    void clearPrinter();
    int ReadWeight();
    int HabilitarScale( wxString enable );
    int HabilitarMicr( wxString enable );
    wxString ReadMicr();

    void DisplayText( char *buffer, int linea );
    void ClearDisplayText();

    void BeginFiscalReceipt();
    int EndFiscalReceipt(char *buffer);
    void BeginNonFiscal();
	void UpdateNonFiscal(char *buffer );
	int FiscalOpenCashDrawer();
    void EndNonFiscal();
    int PrintRecItem( char *buffer );  //devuelve el error Jpos.
	int setPrinterPref( char *buffer );
	int setPaperPref( char *buffer );
	int setComprPref( char *buffer );
	int printRecPercep( char *buffer );  
    void PrintRecSubtotal( char *buffer );
    int PrintRecTotal( char *buffer );
    void PrintRecVoid( char *buffer );
    int PrintRecVoidItem( char *buffer );
	int PrintRecVoidItemFac( char *buffer );
    int PrintRecItemAdjustment( char *buffer );
	int PrintRecItemVoidAdjustment( char *buffer );
    int PrintRecMessage( char *buffer );
    int PrintXReport( char *buffer);
    int PrintZReport( char *buffer);
    void PrintNormalFiscal();
    int GetFiscalData( int info, char *buffer );
    void SetTrailerLine( char *buffer );
    void SetHeaderLine( char *buffer );
    void SetTamImage( char *buffer );
    void SetPostion( char *buffer );
    int Print( char *buffer );
    int Timbradora( char *buffer );
    //int GetchKey();

    void SetLetter( int tam );
    void SetImage( char *buffer );
    void SetFont( char *font, int size );
    void setLineSpace( char *space );
    void setLineChars( char *chars );
	void printVoucher( char *buffer); 
	void feedPaper( char *buffer ); 
	void PrintNormalFiscal(char *buffer);  
	int printRecRefund(char *buffer);  
	int printRecItemFac( char *buffer );  
	int printRecItemVoidFac( char *buffer );  
	int printBonItemFac( char *buffer );  
	int printAnulBonifItemFac( char *buffer );  
	void beginFac(char *buffer);  
	void beginRepaymentNote(char *buffer);  
	int endFac(char *buffer);  
	void endRepaymentNote(char *buffer);  
	int printRecTotalFac(char *buffer);  
	void printRecVoidFac(char *buffer);  
	void PrintRecSubtotalFac( char *buffer); 
	void ObtenerRespuestas(char * buffer, StrStrHashMap rta_prn_hash);
	void printRecBoniDtoFac(char *buffer) ;
	void setDateFechaHora( char *buffer );
	void getDateFechaHora( char *buffer );
	//sets
	int pinpadMostrar(char *buffer);
	int pinpad_select_mk(char *buffer);
	int pinpad_cancel_session(char *buffer);
	int  pinpad_timeout(char *buffer);
	int pinpad_mensajes_rot(char *buffer);
	int pinpad_transfer_mk(char *buffer);
	//gets
	int pinpad_version(char *buffer);
	int pinpad_pedir_pin(char *buffer);
	int pinpad_get_key(char *buffer);
	int pinpad_get_string(char *buffer);
	int pinpad_online(char *buffer);

};
#endif

#pragma pack (pop) 
