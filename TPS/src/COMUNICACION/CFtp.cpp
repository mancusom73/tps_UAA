/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   CFtp
*   Descripcion :   Manejador de trasferencias por ftp (Usando wxWindows)
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/

#pragma warning (disable:4786)
#include "CFtp.h"
#include "path.h"
#include <wx/wfstream.h>
#include "MsgEmergentes.h"

/*Posibles Colores del progress (m_gauge)*/
#define RED    255,000,51
#define BLUE   91,102,193

#define ConstBufferFtp 4096

extern MsgEmergentes *MsgEmerg_thread;

CFtp::CFtp(wxPoint pPosPaneles) : wxThread( wxTHREAD_DETACHED )
{
    mConectar = false;
    mIdComandoScript = 0;
    mFinalizado = 0;
    mError = 0;
    mReintento = 0;
    bitmap.LoadFile(IMAGE_DETALLE_FTP, wxBITMAP_TYPE_GIF);
    panel = new wxSplashScreen( bitmap,wxSPLASH_NO_TIMEOUT, 6, NULL, -1,
                                                            pPosPaneles, wxSize(0,0),
                                                            wxSIMPLE_BORDER|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR  );
    
    panel->SetSize(152,32);
    dc = new wxClientDC(panel);
    panel->Show(false);
    panel->Move(pPosPaneles);
    panel->SetEvtHandlerEnabled(false);
};



void * CFtp::Entry()
{
    while(1){
        while(!mConectar){ // espera un pedido
            this->Sleep(100);
        }
        long count = 0;
        bool ok = false;
        long Tam = 0;
        wxString destinoTmp;
        destinoTmp = mDestino + ".ftp";
        //while(mReintento < 3 && !ok){
            if(Conect()){
                if(ftp.FileExists(mOrigen)){ //si existe el archivo
                    ftp.SendCommand("SIZE " + mOrigen); //obtiene el tamanio en bytes
                    ftp.GetLastResult().AfterLast(' ').ToLong(&Tam);
                    if(mModo == 1){
                        ftp.SetBinary();
                    }else{
                        ftp.SetAscii();
                    }
                    wxInputStream *in = ftp.GetInputStream(mOrigen);
                    ////wxRemoveFile(destinoTmp);            //borra el temporal para crear uno nuevo
                    ////wxFileOutputStream out(destinoTmp);  //nombre temporal .ftp
                    FILE *fs;
                    // Crear o sobreescribir el fichero de salida en binario
                    fs = fopen(destinoTmp, "wb"); 
                    if ( !in )
                    {
                        delete in;
                        Desconect();
                        mReintento++;
                        StrError.sprintf("ERROR DE FTP: No se pudo obtener el Archivo: %s en el ftp",mOrigen);
                        mError = 1;
                    }
                    else
                    {
                        char *data = new char[ConstBufferFtp];
                        MostrarDetalle((Tam / ConstBufferFtp));
                        while( !!in->Read(data, ConstBufferFtp) )
                        {
                            IncrementarGauge();
                            ////out.Write(data,in->LastRead()); //escribe lo leido
                            fwrite(data, 1, in->LastRead(), fs);
                            count += in->LastRead();
                        }
                        //out.Write(data,in->LastRead());
                        //count += in->LastRead();
                        fwrite(data, 1, in->LastRead(), fs);
                        count += in->LastRead();
                        delete [] data;
                        delete in;
                        //delete out;
                        fclose(fs);
                        Desconect();
                        Fin();
                        if ( count != Tam ){ //Controla si tiene todos los bytes
                            Desconect();
                            mReintento++;
                            StrError.sprintf("ERROR DE FTP: Bytes Leidos: %ld de: %ld",count,Tam);
                            mError = 1;
                        }else{
                            int cntCpy = 0;
                            while(cntCpy < 3 && !wxCopyFile(destinoTmp, mDestino, true)){ //copia el temporal pisando el original intenta 3 veces
                                cntCpy++;
                            }
                            if(cntCpy < 3){ //si se pudo copiar
                                ok = true;
                                char *msg[2];
                                msg[0] = (char*)malloc(255);
                                strcpy(msg[0],"FTP finalizado Correctamente");
                                MsgEmerg_thread->AddMsg("Transferencia FTP",(char **)msg,1);
                                free(msg[0]);
                                mError = 0; //no hubo ningun error
                            }else{ // si no pudo copiar o reemplazar el archivo
                                StrError.sprintf("ERROR DE FTP: No se pudo copiar el Archivo: %s",mOrigen);
                                Desconect();
                                mReintento = 3;//ya intento 3 veces
                                mError = 1;
                            }
                        }
                    }
                }else{//fin existe archivo
                        StrError.sprintf("ERROR DE FTP: No se encuentra el Archivo: %s en el ftp",mOrigen);
                        Desconect();
                        mReintento++;
                        mError = 1;
                }
            }
            else{ //Fallo la conexion
                Desconect();
                StrError.sprintf("ERROR DE FTP: NO se pudo conectar con el servidor: ",mIpOrNameServer);
                mReintento++;
                mError = 1;
            }
       // }//while
        mFinalizado = 1;
        mConectar = false;
    }
    
}

void CFtp::ObtenerFtp(wxString pUser,wxString pPassword,wxString pOrigen,wxString pDestino, int pModo)
{
    mUser = pUser;
    mPassword = pPassword;
    mOrigen = pOrigen;
    mDestino = pDestino;
    mModo = pModo;
    //mReintento = 0;
    mFinalizado = 0;
    mConectar = true;
}


int CFtp::Conect()
{
    ftp.SetUser(mUser);
    ftp.SetPassword(mPassword);

    if ( !ftp.Connect(mIpOrNameServer) )
    {
        return 0;
    }
    return 1;
}

void CFtp::Desconect()
{
    ftp.Close();
}

void CFtp::MostrarDetalle(int pMaxValue)
{
    m_gauge = new wxGauge( panel, -1, pMaxValue, wxPoint( 10, 15 ), wxSize( 130, 12 ),
                        wxGA_HORIZONTAL | wxGA_SMOOTH  );
    m_gauge->SetForegroundColour( wxColor( BLUE ) );
    panel->Show(true);
    wxFocusEvent(wxEVT_KILL_FOCUS,panel->GetId());
    font.SetPointSize( 8 );
    font.SetStyle( wxNORMAL );
    font.SetWeight( wxNORMAL );
    dc->SetFont(font);
    dc->DrawBitmap( bitmap, 0, 0 );
    dc->DrawText( "FTP: " + mOrigen, 5, 3 );
    m_gauge->SetValue( 0 );
    this->Yield();
}

void CFtp::IncrementarGauge()
{
    m_gauge->SetValue(  m_gauge->GetValue() + 1 );
    this->Yield();
}

void CFtp::Fin()
{
    panel->Show(false);
    wxDELETE(m_gauge);
}

unsigned long CFtp::getIdComandoScript( )
{
    return mIdComandoScript;
}

void CFtp::setIdComandoScript( unsigned long pIdComandoScript )
{
    mIdComandoScript = pIdComandoScript;
}

int CFtp::getError( )
{
    return mError;
}

const char *CFtp::getStrError( )
{
    mError = 0; //una vez que se lee el error cerea la bandera
    return StrError.c_str();
}

int CFtp::getFinalizado( )
{
    return mFinalizado;
}

void CFtp::Dummy( wxFocusEventFunction& event )
{
}

int CFtp::getReintento()
{
    return mReintento;
}

void CFtp::SetClearReintentos()
{
    mReintento = 0;
}

unsigned long CFtp::getIdNumeroSecuencia( )
{
    return mpIdNumeroSecuencia;
}

void CFtp::setgetIdNumeroSecuencia( unsigned long pIdNumeroSecuencia )
{
    mpIdNumeroSecuencia = pIdNumeroSecuencia;
}