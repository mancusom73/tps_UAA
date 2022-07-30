#pragma warning (disable:4786)
#include "CIMain.h"
#include "MainThread.h"
#include  <string.h>
#include "MainPanel.h"
#include "MainPanel.h"
#include "CConfig.h"
#include "cr.h"

extern CConfig *Config;
using namespace std;
static vector <string> scan_data;
bool hacerping = true;
                                   
extern "C"
{
    extern int imprimiendo;
	extern int printer_status_2;
	extern void /*int*/ GRABAR_LOG_SISTEMA( char *operacion, int origen, int nivel );
	extern void GRABAR_TICKET_PRN( char *cadena );
	extern void GRABAR_VOUCHER_PRN( char *cadena );
    extern void GRABAR_Z_PRN( char *cadena );
	extern void OPEN_COPIA_TICKET_PRN();
	extern void CLOSE_COPIA_TICKET_PRN();
	extern void MENSAJE(char *cadena);
    extern void MENSAJE_STRING( int id );
	extern int grabar_copia_ticket;
    extern int grabar_copia_cierre_z;
	extern char _COPIA_VOUCHER_PRN;
	extern int imprime_copia_voucher;
    extern void GRABAR_Z_PRN( char *cadena );
}
//class ScannerOut///////////////////////////////////////////////////

void ScannerOut::SetEnabled() {
    
    scannerEnabled = true;
}

wxString ScannerOut::GetData()
{
    wxString res;

    if( data_out_scanner.size() ) {
        res = ( data_out_scanner[0] );
        data_out_scanner.erase( data_out_scanner.begin() );
        //return ( res == "true" );
        return res;
    }
    return "false";
}

int ScannerOut::PutData( wxString enable )
{
	if( scannerEnabled != (enable == "true") ? true : false){
		data_out_scanner.push_back( enable );
		data_out_scanner.push_back( enable );
		if( enable == "true" ) {
			scannerEnabled = true;
			//MENSAJE("SCANNER HABILITADO");
		}
		else {
			//MENSAJE("SCANNER DESHABILITADO");
			scannerEnabled = false;
		}
	}
    return 1;
}

bool ScannerOut::GetNotEmpty(){

    return ( data_out_scanner.size() > 0 );
}

/////////////////////////////////////////////////////class ScannerOut
//class CIMain///////////////////////////////////////////////////////

void CIMain::Init( char *ipserv )
{
    char ip[16];
    char puerto[5];
    int cnt = 0,cnt2 = 0;

    while( ipserv[cnt] != ':' && cnt < (int)strlen( ipserv ) ) {
        ip[cnt] = ipserv[cnt];
        cnt++;
    }

    ip[cnt] = 0;
    cnt++;

    if( cnt < (int)strlen( ipserv ) ) {
        while( cnt != strlen( ipserv ) ) {
            puerto[cnt2] = ipserv[cnt];
            cnt++;
            cnt2++;
        }
        puerto[cnt2] = 0;
    }
    else {
        _snprintf( puerto, sizeof(puerto)-1, "%s", "8080" );
    }
    
    connect = ( strcmp( _strlwr( ip ),"no" ) == 0 )? false : true;

    mutex_com = new wxMutex(/*wxMUTEX_RECURSIVE*/);
    jposCom = new PCom( ip, 2748 );

    if( connect ) {
        wxMutexLocker lock( *mutex_com );
		if( jposCom->ConnectToServer( )){
			OpenDevices( "peripheral.xml" );
		}
    }
    print_station = TO_POS_PRINTER_RECEIPT;
}

void * CIMain::Entry()
{
    int comienzo=0, fin=0;
    bool response = false;
    char types[] = {'s','s','i','s','s','i'};//DNC CAMBIADO EL PRIMER DE SCANNER DE 'b' A 's'
    pair<int, string> to;
    wxArrayString arg, args, res, res2;
    StrStrHashMap ress;
    int getStateCounter = 0;

    printer_status = 0;
    printer_empty = 0;
    non_fiscal_ok = false;
    args.Alloc(6);
    args.Add(""); //scanner 
    args.Add(""); //printer
    args.Add(""); //station
    args.Add(""); //display linea 1
    args.Add(""); //display linea 2
    args.Add(""); //col
    /*if( Config->get_modo_negocio() != SERVICIOS ) {
        OpenDevices( "peripheral.xml" );
    }*/
    while( true ) {
		
        TestDestroy();
        response = false;
        
        //Verificamos cada tanto...
        if( getStateCounter++ == 100 ) {
            getStateCounter = 0;
            if( posPrinterEnabled ) {
                if( getErrorPrinter( ) == 1 ) {
			        printer_status_2 = 1234;
		        } else {
			        printer_status_2 = GetPrinterStatus();
		        }
            }
            else if ( fiscalPrinterEnabled ) {
            }
            pingJavaServer();
        }
        if( scanOut.GetNotEmpty() ) {
            args[0] = scanOut.GetData();
        }
        else {
            args[0] = "null";
        }
		//Datos de impresion
        if( IsOutput( TO_POS_PRINTER ) || IsOutput( TO_FISCAL_PRINTER ) ) {

			while( IsOutput( TO_POS_PRINTER ) || IsOutput( TO_FISCAL_PRINTER ) ) {
				pair<int, string> to;
                int stationTmp = 0;
                int seguir = 1;
                
                to = GetDataToPrint();
                stationTmp = to.first;
                while(to.second != "" && seguir){
                    args[2].sprintf("%d", to.first);
				    args[1] += to.second.c_str();
                    if(GetProxStation() == stationTmp) { //si el proximo dato es para la misma estación lee de nuevo
                        to = GetDataToPrint();
                    }else {
                        seguir = 0;
                    }
                }

                        if(strlen(args[1]) > 0 && grabar_copia_cierre_z && stationTmp != 4 && stationTmp != 1 && stationTmp != 31){
                            GRABAR_Z_PRN((char *)args[1].c_str());
                        }
                if(strlen(args[1]) > 0 && grabar_copia_ticket && stationTmp != 4 ){
							//grabar_copia_ticket = 2: para que no grabe en ticket.prn pero si grabe en 
							//voucher tarjeta si es que si es que posee.
                            if( stationTmp != 1 && stationTmp != 31 && grabar_copia_ticket != 2 )//si no es el journal
                                GRABAR_TICKET_PRN((char *)args[1].c_str());
							if( stationTmp != 1 && stationTmp != 31 && imprime_copia_voucher)
					{
				        if(args[1].Contains(BEGIN_VOUCHER)||_COPIA_VOUCHER_PRN)
				        {
					        if(args[1].Contains(BEGIN_VOUCHER) && !args[1].Contains(FINISH_VOUCHER))//tiene el comienzo pero no el fin
					        {
						        GRABAR_VOUCHER_PRN((char *)(args[1].Mid(args[1].Find(BEGIN_VOUCHER),strlen(args[1]))).c_str());
						        _COPIA_VOUCHER_PRN = 1;
					        }
					        if(args[1].Contains(FINISH_VOUCHER) && !args[1].Contains(BEGIN_VOUCHER))//tiene el fin pero no el cominezo
					        {
						        GRABAR_VOUCHER_PRN((char *)(args[1].Mid(0,args[1].Find(FINISH_VOUCHER))).c_str());
						        _COPIA_VOUCHER_PRN = 0;
					        }
					        if(args[1].Contains(FINISH_VOUCHER) && args[1].Contains(BEGIN_VOUCHER))//tiene el fin y el cominezo
					        {
						        if(args[1].Find(BEGIN_VOUCHER) < args[1].Find(FINISH_VOUCHER))
						        {
							        GRABAR_VOUCHER_PRN((char *)(args[1].Mid(args[1].Find(BEGIN_VOUCHER),args[1].Find(FINISH_VOUCHER))).c_str());
                                }
						        else
						        {
							        GRABAR_VOUCHER_PRN((char *)args[1].c_str());//graba el corte de papele entre los los vouchers
							        _COPIA_VOUCHER_PRN = 0;
						        }
					        }
					        if(_COPIA_VOUCHER_PRN && !args[1].Contains(BEGIN_VOUCHER) && !args[1].Contains(FINISH_VOUCHER)) //no tiene ni el comienzo ni el fin pero la bandera de voucher está en 1
					        {
						        GRABAR_VOUCHER_PRN((char *)args[1].c_str());
					        }
				        }
			        }
                }
				args[3] = "";
				args[4] = "";
                args[5] = "0";

    			//mutex_com->Lock();
                response = PCommand( "data", args, types, ress, true );
                //mutex_com->Unlock();
	
                //Lo siguiente funciona con wxString tambien
                args[1] = "";
                args[2] = "0";
				Yield();
                //Sleep( 300 );
			}
		}else {
            args[1] = "";
            args[2] = "0";
        }
        //Datos para los displays
        if( data_out_display.size() >= 2 && displayEnabled ) {
            pair<int, string> d1,d2;

            d1 = data_out_display[0];
            d2 = data_out_display[1];
            data_out_display.erase( data_out_display.begin() );
            data_out_display.erase( data_out_display.begin() );
            args[4] = "";
            args[3] = "";
            args[5] = "0";
            if( d1.first == TO_DISPLAY_LINE_1 ) {
                args[3] = d1.second.c_str();
            }
            else {
                args[4] = d1.second.c_str();
            }
            if( d2.first == TO_DISPLAY_LINE_2 ) {
                args[4] = d2.second.c_str();
            }
            else {
                args[3] = d2.second.c_str();
            }
            response = PCommand( "data", args, types, ress, true );
        }
        else {
            args[3] = "";
            args[4] = "";
            args[5] = "0";
        } 
        if( response ) {
            if( posPrinterEnabled ) {
                int stat = atoi( ress["printerStatus"].c_str() );
                printer_empty = atoi( ress["printerEmpty"].c_str() );
                if( stat == 0 ) {
                    printer_status = 0;
                }
                else if( stat == 202 ) {
                    printer_status = JOURNAL_PAPER_OUT;
                }
                else if( stat == 203 ) {
                    printer_status = RECEIPT_PAPER_OUT;
                }
                else if( stat == 405 ) {
                    printer_status = JOURNAL_RECEIPT_PAPER_OUT;
                }
                else if( stat == 201 ) {
                    printer_status = PRINTER_COVER_OPEN;
                }
            }
            /*if( msrEnabled && ress["msrError"] == "1" ) {
                if( data_in.size() == 0 ) {
                    pair<int, string> from;

                    from.first = FROM_MSR_TRACK1;
                    from.second = ress["msrTrack1"].c_str();
                    data_in.push_back( from );
                    from.first = FROM_MSR_TRACK2;
                    from.second = ress["msrTrack2"].c_str();
                    data_in.push_back( from );
				}
                MainThread::activate_from = 3;
                //nico_signal MainThread::m_condition->Signal();
            }*/
        }
		Yield();
        Sleep( 200 );
		imprimiendo = 0; 
    }
    return 0;
}

bool CIMain::PCommand( const char *command, StrStrHashMap &result, bool mutex ) {

    return PCommand( command, NULL, NULL, result, mutex);
}

bool CIMain::PCommand( const char *command, const wxArrayString &params, const char *types, StrStrHashMap &result, bool mutex )
{   
    bool res= false;
    wxMutexError res_lock;
    wxStringTokenizer *process;
    char /*msg[255]*/ msg[500], *finalCommand;
    static short int recuperando = NO;
    unsigned int i = 0, tam = 0;


    memset(msg,0,sizeof(msg));
    //si no hay conexion se retorna inmediatamente
    if ( !connect ) {
        return true;
    }
    
    if( mutex ) {
        res_lock = mutex_com->Lock();
        if( res_lock != wxMUTEX_NO_ERROR ) {
			_snprintf(msg, sizeof(msg)-1, "CIMain: Lock() %s Err %d", command, res_lock );
            GRABAR_LOG_SISTEMA( msg ,LOG_JAVA_SERVER,4);
        }
    }
    
   /*wxArrayString *test = new wxArrayString();
    wxString *uno = new wxString();
    test->Add( "sdf\rasdf" );
    test->Item(0).Replace( "\r", "" );
    uno->assign(test->Item(0));*/

    //Se reemplazan los saltos de linea y se eliminan los retornos de carro
    //del paquete que va a la impresora, ya que interfieren con el protocolo.
    if( strcmp( command, "data" ) == 0 ) {
        params.Item(1).Replace( "\n", COMM_LINE_BREAK );
        params.Item(1).Replace( "\r", "" );
		params.Item(1).Replace(BEGIN_VOUCHER,"");
		params.Item(1).Replace(FINISH_VOUCHER,"");
    }
    //Tam de todos los elementos
    if( params != NULL ) {
        for(i = 0; i < params.GetCount(); i++) {
            tam += params.Item(i).Len();
        }
    }
    finalCommand = (char*) malloc( tam + strlen( command ) + 2*i + 1 );
    strcpy( finalCommand, command );    
    if( params != NULL ) {
        for(i = 0; i < params.GetCount(); i++) {
            strcat( finalCommand, COMM_SEPARATOR );
            strncat( finalCommand, &types[i], 1 );
            strcat( finalCommand, params.Item(i).c_str() );
        }
    }
    //Vaciamos el buffer de llegada para evitar desincronizarse con el javaServer
    if( jposCom->Discard() > 0 ) {
        GRABAR_LOG_SISTEMA( "CIMAIN: Se corrige desincronizacion con JavaServer" ,LOG_JAVA_SERVER,4);    
    }
    res = jposCom->Send( finalCommand, tam + strlen( command ) + 2*i );
    //jposCom->Yield();
    free( finalCommand );

    //Es necesario esperar antes de leer para que la funcion Read no tenga que
    //releer muchas veces antes de obtener todo el paquete. 1 o 2 milisegundos
    //es suficiente dependiendo de la velocidad de la PC.
    Sleep(1);

    if( res ) {
		long  time;
		if( habilitapingjserver == false ) {
			time = jposCom->GetTimeout();
			jposCom->SetTimeout(30);
		}
        res = jposCom->Read( msg, &tam );
		if( habilitapingjserver == false ) {
			jposCom->SetTimeout(time);
		}
        /*if( strstr(msg,"msrError") ){
            printf("");
        }*/
        result.clear();
        process = new wxStringTokenizer( msg, COMM_SEPARATOR );
        while ( process->HasMoreTokens() ) {
            wxString name = process->GetNextToken();
            wxString data = process->GetNextToken();
            result[name] = data;
        }
        delete( process );
    }

    if( !res || SI == recuperando ) {
        //Notar que se asume ( ( res == true ) => ( Connected() == true ) ) y no al reves

        //Solo guardamos mensaje si 'res' es falso
        if( !res ) { 
            GRABAR_LOG_SISTEMA( "CIMAIN: ERROR DE COMUNICACION CON JAVASERVER. RECONECTANDO...",LOG_JAVA_SERVER,3 );
            GRABAR_LOG_SISTEMA( (char*)jposCom->GetErrorMessage().c_str() ,LOG_JAVA_SERVER,4 );
        }

        if( !jposCom->Connected() ) { //Si se desconecto o aun no se conecta
            //Si no se logro conectar, la proxima vez que intente mandar un comando
            //caera aca nuevamente.
           
            MENSAJE( "ERROR DE COMUNICACION CON JAVASERVER. RECONECTANDO..." );
            jposCom->Close();
            //Como al cerrar se detienen los eventos, no se intentara reconectar solo.
            jposCom->ConnectToServer();
            recuperando = SI;
        }
        //Notar que el siguiente if no se puede reemplazar por 'else' ya que el estado
        //de 'Connected' puede cambiar dentro del if anterior.
        if( jposCom->Connected() && SI == recuperando ) { 
            MENSAJE( "CONECTADO CON JAVASERVER" );
            recuperando = NO;
			OpenDevices( "peripheral.xml" );
        }
    }

    if( mutex ) {
        res_lock = mutex_com->Unlock();
        if( res_lock != wxMUTEX_NO_ERROR ) {
            _snprintf( msg, sizeof(msg)-1, "CIMain: Unlock() %s Err %d", command, res_lock );
            GRABAR_LOG_SISTEMA( msg ,LOG_JAVA_SERVER,4 );
        }
    }
    
    if ( !res ) {
        this->Sleep( 4000 );
    }
    //se maneja el caso en que se produce un error interno en la ejecucion
    //del comando en el JavaServer
    if( result["internalMethodError"] == -1 ) {
        res = false;
    }
    return res;
}

/*bool CIMain::IsEmpty( int from )
{
    for( int i = 0;i < (int)data_in.size();i++ ) {
        if( ( data_in[i] ).first == from ) {
            return false;
        }
    }
    return true;
}
*/

void setHabPing(int hab){

	if( hab== 1){
		//puts("Activo PING");
		hacerping = true;
	} else{
		//puts("chau PING");
		hacerping = false;
	}


}

void CIMain::pingJavaServer() {
    
	if( habilitapingjserver != false && hacerping !=false) {
		StrStrHashMap result;
		PCommand( "pingJavaServer", result, true );
	}

}


bool CIMain::IsOutput( int to )
{
    if( to == TO_POS_PRINTER && data_out_print.size() > 0 ) {
        return ( data_out_print[0].first ) < 5;
    }
    if( to == TO_FISCAL_PRINTER && data_out_print.size() > 0 ) {
        return ( data_out_print[0].first ) > 4;
    }
    return false;
}


int CIMain::OpenDevices( wxString peripherals )
{
    //oneArg[0] = peripherals;
    char types[] = {'s'};
    wxArrayString oneArg;
    StrStrHashMap result;
    bool response = false;
    char buffer[90] = "";
    char *perifericos[] = {"msrStatus", "cashDrawerStatus", "printerStatus",
        "lineDisplay1Status", "lineDisplay2Status", "lineDisplay3Status", 
        "scaleStatus", "fiscalPrinterStatus", "POSKeyLock", "scannerStatus" };
    int iter;

    oneArg.Add( peripherals );
    //Estado inicial siempre:
    scanOut.SetEnabled();
    msrEnabled = false;
    //Dependientes de la respuesta:
    cashDrawerEnabled = false;
    posPrinterEnabled = false;
    displayEnabled = false;
    fiscalPrinterEnabled = false;

    response = PCommand( "open", oneArg, types, result, true ); 
    if( !response ) {
        sglog( "Error en OpenDevices\n" );
        return 1;
    }
    for( iter = 0; iter < sizeof( perifericos )/sizeof( *perifericos ); iter ++ ) {
        if( result.find(perifericos[iter]) != result.end() ) {
            if( result[perifericos[iter]] == "0" ) {
                _snprintf( buffer, sizeof( buffer), "%s: Abierto Ok", perifericos[iter] );
                switch( iter ) {
                    case 1: cashDrawerEnabled = true;
                        break;
                    case 2: posPrinterEnabled = true;
                        break;
                    case 3:
                    case 4:
                    case 5: displayEnabled = true;
                        break;
                    case 6: scaleEnabled = true;
                    case 7: fiscalPrinterEnabled = true;
                        break;
                }
            }
            else {
                _snprintf( buffer, sizeof( buffer), "%s: ERROR: %s", perifericos[iter], result[perifericos[iter]].c_str() );
				//_snprintf( buffer, sizeof( buffer), "%s: ERROR: %s", perifericos[iter], result[perifericos[iter]] );
            }
		sglog( buffer );
		}
    }
    if( posPrinterEnabled ) {
        printerCharset = getCharSet();
    }
    return 0;
}





/*string CIMain::GetScanData()
{
   string res;

   for(int i=0; i < data_in.size(); i++){
      if( (data_in[i]).first == FROM_SCANNER )
         res = (data_in[i]).second;
         data_in.erase(&data_in[i]);
         return res;
   }
   return NULL;
}*/

int CIMain::HabilitarMsr( wxString enable )
{
    bool response = false;
    wxArrayString oneArg;
    StrStrHashMap result;
    char types[] = {'b'};
    oneArg.Add( enable );

    msrEnabled = ( enable == "true" );
    response = PCommand( "setMsrEnabled", oneArg, types, result, true );
    if( !response ) {
        data_in.clear();
        return 1;
    }
    return atoi( result["error"].c_str() );
}


/*string CIMain::GetMsrTrack1()
{
    string res;
    char strTmp[200];
    for( int i = 0;i < (int)data_in.size();i++ ) {
        if( ( data_in[i] ).first == FROM_MSR_TRACK1 ) {
            res = ( data_in[i] ).second;
            data_in.erase( data_in.begin() + i );
            //********************************LOG***********************************************************
            _snprintf(strTmp,200,"LEN TRACK1: %i",res.length());
            strTmp[199] = 0;
            sglog( strTmp );
            _snprintf(strTmp,200,"BUFFER TRACK1: %s",res.c_str());
            strTmp[199] = 0;
            sglog( strTmp );
            //********************************LOG***********************************************************
            return res;
        }
    }
    res = "";
    return res;
}*/


/*string CIMain::GetMsrTrack2()
{
    string res;
    char strTmp[200];
    for( int i = 0;i < (int)data_in.size();i++ ) {
        if( ( data_in[i] ).first == FROM_MSR_TRACK2 ) {
            res = ( data_in[i] ).second;
            data_in.erase( data_in.begin() + i );
            //********************************LOG***********************************************************
            _snprintf(strTmp,200,"LEN TRACK2: %i",res.length());
            strTmp[199] = 0;
            sglog( strTmp );
            _snprintf(strTmp,200,"BUFFER TRACK2: %s",res.c_str());
            strTmp[199] = 0;
            sglog( strTmp );
            //********************************LOG***********************************************************
            return res;
        }
    }
    res = "";
    return res;
}*/

int CIMain::getCharSet()
{
    bool response = false;
    StrStrHashMap result;
    int cs = 0;

    response = PCommand( "getCharacterListSet", result, true );
    if( response ) {
       cs = atoi( result["CharacterListSet"].c_str() );
    }
    else {
        sglog( "Error calling 'getCharacterListSet'\n\n" );
    }
    return cs;
}
int CIMain::getPrinterStation( )
{
     return print_station;
}

int CIMain::getPrinterLetter( )
{
     return print_letter;
}

int CIMain::OpenDrawer( wxString name )
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'s'};

    arg1.Add( name );
    response = PCommand( "openDrawer", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method openDrawer'\n\n" );
        return 1;
    }
    else {
        return atoi( result1["error"].c_str() );
    }
}


bool CIMain::GetDrawerOpened( wxString name )
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'s'};
    arg1.Add( name );

    response = PCommand( "getDrawerOpened", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method getDrawerOpened'\n\n" );
        return false;
    }
    else {
        if( atoi( result1["error"].c_str() ) == 1 ) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}


int CIMain::WaitForDrawerClose( wxString name )
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'s'};

    arg1.Add( name );
    response = PCommand( "CashDrawer.waitForDrawerClose", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method waitForDrawerClose'\n\n" );
        return -1;
    }
    else {
        return 0;
    }
}

int CIMain::isScanner( int d )
{
    bool response = false;
    char number_str[10];
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'i'};

    memset(number_str,0,sizeof(number_str));
	if( !(scan_data.size() > 0) ) {
        _snprintf( number_str, sizeof(number_str)-1, "%d", d );
		arg1.Add( number_str );
        response = PCommand( "isScanner", arg1, types, result1, true );

		if( !response ) {
			sglog( "Error calling 'method isScanner'\n\n" );
			return -1;
		}
		else {
            return ( atoi( result1["isScanner"].c_str() ) );
		}
	} else {
		return 1;
	}

}

int CIMain::getScanner( char *temp )
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'i'};
	
	if( !(scan_data.size() > 0) ) {
        arg1.Add( "1" );
        response = PCommand( "getScanner", arg1, types, result1, true );
		if( !response ) {
			sglog( "Error calling 'method isScanner'\n\n" );
			return 0;
		}
		else {
			if( result1["errorScanner"] == "1" ) {
				scan_data.push_back( ( string )result1["scannerData"] );
			}
			if( result1["errorScanner1"] == "1" ) {
				scan_data.push_back( ( string )result1["scannerData1"] );
			}
			if( result1["errorScanner2"] == "1" ) {
				scan_data.push_back( ( string )result1["scannerData2"] );
			}
			if( result1["errorScanner3"] == "1" ) {
				scan_data.push_back( ( string )result1["scannerData3"] );
			}
			if( result1["errorScanner4"] == "1" ) {
				scan_data.push_back( ( string )result1["scannerData4"] );
			}
			if( result1["errorScanner5"] == "1" ) {
				scan_data.push_back( ( string )result1["scannerData5"] );
			}
			if( scan_data.size() > 0 ) {
				sprintf( temp, "%s", ( scan_data[0] ).c_str() );
				scan_data.erase( scan_data.begin() );
			}
			return 1;
		}
	} else {
		sprintf( temp, "%s", ( scan_data[0] ).c_str() );
		scan_data.erase( scan_data.begin() );
		return 1;
	}
}

int CIMain::cleanBufferScanner( )
{
    bool response = false;
    StrStrHashMap result1;
 
    response = PCommand( "cleanBufferScanner", result1, true );

    if( !response ) {
        sglog( "Error calling 'method cleanBufferScanner'\n\n" );
        return 0;
    }
    return 0;
}


int CIMain::getPositionCountKeyLock( int position )
{
    bool response = false;
    char pos_str[10];
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'i'};

    memset(pos_str,0,sizeof(pos_str));
	_snprintf( pos_str, sizeof(pos_str)-1, "%d", position ); 
    arg1.Add( pos_str );
    response = PCommand( "getKeyLockPositionCount", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method getKeyLockPositionCount'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["Keylock"].c_str() ) );
		return 0;
    }
}

int CIMain::printBarCode( int position )
{
    bool response = false;
    char pos_str[10];
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'i'};

    memset(pos_str,0,sizeof(pos_str));
	_snprintf( pos_str, sizeof(pos_str)-1, "%d", position ); 
    arg1.Add( pos_str );
    response = PCommand( "printBarCode", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printBarCode'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["printer"].c_str() ) );
		return 0;
    }
}


int CIMain::getPositionKeyLock( int position )
{
    bool response = false;
    char pos_str[10];
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'i'};

    memset(pos_str,0,sizeof(pos_str));
	_snprintf( pos_str, sizeof(pos_str)-1, "%d", position ); 
    arg1.Add( pos_str );
    response = PCommand( "getKeyLockPosition", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method getKeyLockPosition'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["Keylock"].c_str() ) );
    }
}

int CIMain::WaitForPositionKeyLock( int position )
{
    bool response = false;
    char pos_str[10];
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'i'};
    
    memset(pos_str,0,sizeof(pos_str));
    _snprintf( pos_str, sizeof(pos_str)-1, "%d", position ); 
    arg1.Add( pos_str );
    response = PCommand( "waitKeyPosition", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method waitKeyPosition'\n\n" );
        return -1;
    }
    else {
        return 0;
    }
}

int CIMain::charFilter( unsigned char *c ) {

    unsigned char c_tmp;
    bool accept = ( printerCharset == PTR_CCS_ASCII );

    if( *c >= 32 && *c <= 126 ) {
        //Simbolos comunes, numeros y letras
        return true;
    }
    switch (*c) {
        //Acentos
        case 225: c_tmp = 'a';
            break;
        case 233: c_tmp = 'e';
            break;
        case 237: c_tmp = 'i';
            break;
        case 243: c_tmp = 'o';
            break;
        case 250: c_tmp = 'u';
            break;
        case 241: c_tmp = 'n';
            break;
        default:
            return false;
    }
    if( !accept ) {
        *c = c_tmp;
    }
    return true;
}

int CIMain::SetDataToPrint( int station, string data )
{
    int count;
    unsigned char c;
    string dataAux;

    for( int i = 0;i < (int)data.length();i++ ) {
        c = data[i];
		//if( ( c > 32 && c < 126 ) && c != 13 && c != 10 ) {
		if( c != 10 ) {
			if ( charFilter ( &c ) ) {
				dataAux += c;
			}
		} else {
			dataAux += c;
		}
    }
    count = print_pos - print_data.length();

    if( count > 0 ) {
        string aux( count, ' ' );
        print_data += aux + dataAux;//data;
    }
    else {
        print_data += dataAux;
    } //data;

    return 0;
}


pair <int, string> CIMain::GetDataToPrint()
{
    pair <int, string> res;
    res.first = 0;
    if( data_out_print.size() > 0 ) {
		if( existePosPrinter == true ) {
			res = data_out_print[0]; 
		}
        data_out_print.erase( data_out_print.begin() );
    }
    return res;
}

/*Retorna el numero de la proxima estacion para mandar datos a imprimir*/
int CIMain::GetProxStation()
{
    int rta = 0;
    if( data_out_print.size() > 0 ) {
		if( existePosPrinter == true ) {
			rta = data_out_print[0].first; 
		}
    }
    return rta;
}


void CIMain::Print()
{
    pair <int, string> toprint;

    for( int i = 0;i < (int)lines.size();i++ ) {
        toprint.first = print_station;
        toprint.second += lines[i];
    }

    if( lines.size() > 0 ) {
        data_out_print.push_back( toprint );
        lines.clear();
    }
    print_data = "";
}


void CIMain::Print( int pos )
{
    print_pos = pos - 1;
}


void CIMain::PrintLF( char *buffer, int len )
{
    char aux[20];
    int count = atoi( buffer );
    lines.push_back( print_data );
    print_pos = 0;

    memset(aux,0,sizeof(aux));
    for( int i = 0;i < count;i++ ) {
        _snprintf( aux, sizeof(aux)-1, COMM_LINE_BREAK );
        lines.push_back( aux );
    }
    print_data = "";
}


void CIMain::PrintCutPaper(int imprimir)
{
    char aux[20];
    lines.push_back( print_data );
    sprintf( aux, "!|80fP");
    if(imprimir){
        strcat(aux, "!|1X");
    }
    //sprintf( aux, "&" );
    lines.push_back( aux );
    print_data = "";
}


void CIMain::ForcePrint()
{
    char aux[20];
    lines.push_back( print_data );
    sprintf( aux, "!|1X");
    //sprintf( aux, "&" );
    lines.push_back( aux );
    print_data = "";
}


int CIMain::setModoImpresionFinal(int modo)
{
    bool response = false;
    char modo_str[10];
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'i'};

    memset(modo_str,0,sizeof(modo_str));
    _snprintf( modo_str, sizeof(modo_str)-1, "%d", modo );
    arg1.Add( modo_str );

    response = PCommand( "setModoImpresion", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method setModoImpresion'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["setModoImpresion"].c_str() ) );
    }
    return 0;
}



void CIMain::PrintMode( int mode )
{
    char aux[20];
    lines.push_back( print_data );
    switch( mode ) {
        case LETRA_CHICA:
            sprintf( aux, "!|1C" );//1C
            lines.push_back( aux );
            print_data = "";
            print_letter = 0;
            break;
        case LETRA_GRANDE:
            sprintf( aux, "!|2C" );
            lines.push_back( aux );
            print_data = "";
            print_letter = 1;
            break;
        case LETRA_NEGRITA :
            sprintf( aux, "!|bC" );
            lines.push_back( aux );
            print_data = "";
            print_letter = 2;
            break;
        case DESHABILITA_FORMATO://Saca cualquier formato de letras previo
            //sprintf( aux, "!|!bC" );
            sprintf( aux, "!|N" );
            lines.push_back( aux );
            print_data = "";
            print_letter = 3;
            break;
    }
}

void CIMain::PrintBitmap( char *buffer )
{
    char aux[20];
	
    memset(aux,0,sizeof(aux));
	lines.push_back( print_data );
    _snprintf( aux, sizeof(aux)-1, "!|%sB", buffer );
    lines.push_back( aux );
    print_data = "";
}

int CIMain::SetPrintBitmap( wxString buffer )
{
	bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'s'};

    arg1.Add( buffer );
    response = PCommand( "setPrintBitmap", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method setPrintBitmap'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["bitmap"].c_str() ) );
    }
}

int CIMain::getErrorPrinter( )
{
	bool response = false;
    StrStrHashMap result1;

    response = PCommand( "getErrorPrinter", result1, true );

    if( !response ) {
        sglog( "Error calling 'method getErrorPrinter'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["getErrorPrinter"].c_str() ) );
    }
}
void CIMain::PrintBarCode( )
{
    char aux[20];
	
	lines.push_back( print_data );
    sprintf( aux, "!|1Z" );
    lines.push_back( aux );
    print_data = "";
}

void CIMain::setExistePosPrinter( char *buffer ){
	if( !strcmp( buffer, "YES" ) ) {
		existePosPrinter = true;
	} else {
		existePosPrinter = false;
	}
}

int CIMain::SetPrintBarCode( wxString buffer )
{
	bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'s'};

    arg1.Add( buffer );
    response = PCommand( "setPrintBarCode", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method setPrintBarCode'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["printer"].c_str() ) );
    }
}

void CIMain::PrintSelect( int station )
{
    switch( station ) {
        case 2:
            print_station = TO_POS_PRINTER_RECEIPT;
            break;
        case 1:
            print_station = TO_POS_PRINTER_JOURNAL;
            break;
        case 3:
            print_station = TO_POS_PRINTER_JOURNAL_RECEIPT;
            break;
        case 31:
            print_station = TO_POS_PRINTER_JOURNAL_PART;
            break;
        case 32:
            print_station = TO_POS_PRINTER_RECEIPT_PART;
            break;
        case 4:
            print_station = TO_POS_PRINTER_SLIP;
            break;
        case 5:
            print_station = TO_FISCAL_PRINTER_REC;
            break;
        case 6:
            print_station = TO_FISCAL_PRINTER_JRN;
            break;
        case 7:
            print_station = TO_FISCAL_PRINTER_SLP;
            break;
        case 8:
            print_station = TO_FISCAL_PRINTER_REC_JRN;
            break;
        default:
            print_station = TO_POS_PRINTER_JOURNAL;
            break;
    }
}

void CIMain::PrintAlign( int align )
{
    char aux[10];
    switch( align ) {
        case 100:
            sprintf( aux, "<" );
            lines.push_back( aux );
            print_data = "";
            break;
        case 101:
            sprintf( aux, ">" );
            lines.push_back( aux );
            print_data = "";
            break;
        default:
            break;
    }
}

int CIMain::GetPrinterStatus()
{
	bool response = false;
    StrStrHashMap result1;

	int rta = 0;
    response = PCommand( "getPrinterStatus", result1, true );

    if( !response ) {
        //sglog( "Error calling 'method getPrinterStatus'\n\n" );
        return -1;
    }
    else {
        rta = atoi( result1["PrinterStatus"].c_str() );
		return ( rta );
    }
}

int CIMain::GetSlpEmpty()
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'s'};

    arg1.Add( "printer" );
    response = PCommand( "getSlpEmpty", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method getSlpEmpty'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["error"].c_str() ) );
    }
}

int CIMain::GetPrinterFinish()
{
    if( posPrinterEnabled ) {
        if( !IsOutput( TO_POS_PRINTER ) && !IsOutput( TO_FISCAL_PRINTER ) ) {
            return printer_empty;
        }
    }
    else {
        return 1;
    }
    return 0;
}

void CIMain::clearPrinter()
{
    bool response = false;
    StrStrHashMap result1;
    //wxArrayString arg1;

    response = PCommand( "clearPrinter", result1, true );
    if( !response ) {
        sglog( "Error calling 'method clearPrinter'\n\n" );
    }
}

void CIMain::DisplayText( char *buffer, int linea )
{
    pair<int, string> data;
    data.first = linea;
    data.second = buffer;
    data_out_display.push_back( data );
}

void CIMain::ClearDisplayText()
{
    pair<int, string> data;
    data.first = TO_DISPLAY_CLEAR;
    data.second = "";
    data_out_display.push_back( data );
}

int CIMain::ReadWeight()
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    arg1.Add( "scale" );
    char types[] = {'s'};

    response = PCommand( "readWeight", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method readWeight'\n\n" );
        return -1;
    }
    else {
        return ( atoi( result1["error"].c_str() ) );
    }
}

int CIMain::HabilitarScale( wxString enable )
{
    bool response = false;
    wxArrayString oneArg;
    StrStrHashMap result;
    oneArg.Add( enable );
    char types[] = {'s'};

    scaleEnabled = ( enable == "true" );
    response = PCommand( "setScaleEnabled", oneArg, types, result, true );
    return atoi( result["error"].c_str() );
}

int CIMain::HabilitarMicr( wxString enable )
{
    bool response = false;
    wxArrayString oneArg;
    StrStrHashMap result;
    oneArg.Add( enable );
    char types[] = {'b'};

    micrEnabled = ( enable == "true" );
    response = PCommand( "setMicrEnabled", oneArg, types, result, true );
    return atoi( result["error"].c_str() );
}

wxString CIMain::ReadMicr()
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    arg1.Add( "micr" );
    char types[] = {'s'};

    response = PCommand( "readMicr", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method readMicr'\n\n" );
        return "Error";
    }
    else {
        return ( result1["error"] );
    }
}


/***********************************************************************************************/
void CIMain::BeginFiscalReceipt()
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    arg1.Add( "true" );
    char aux[20];
    int result;
    char types[] = {'b'};
	memset(aux, 0, sizeof(aux));

    response = PCommand( "beginFiscalReceipt", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method beginFiscalReceipt'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en beginFiscalReceipt", result );
            sglog( aux );
        }
    }
    Sleep( 1200 );
}

int CIMain::EndFiscalReceipt(char *buffer)
{
    bool response = false;
    wxArrayString arg1;
    StrStrHashMap result1;
    arg1.Add( "true" );
    char aux[45];
    int result;
    char types[] = {'b'};
	
	//char buffer[100];
    memset(buffer,0,sizeof(buffer));
	memset(aux, 0, sizeof(aux));
	

    result = 1;
    response = PCommand( "endFiscalReceipt", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method endFiscalReceipt'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en endFiscalReceipt", result );
            sglog( aux );
        }
		ObtenerRespuestas(buffer, result1);
		/*{
		char *res1;
		char *res2;
		char *res3;
		char *res4;
	   res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   res4 = ( char* )( result1["Serial"] ).c_str();
	   sprintf( buffer, "%s %s %s %s", res1, res2, res3,res4 );
		}*/
	   
    }
	return result;
}

int CIMain::PrintRecItem( char *buffer )
{
    /*
     *  devuelve el error de Jpos.
     */
    //string description, long price, int quantity,int vatInfo, long unitPrice, String unitName)
    int rta = -1, rta2 =0;
    bool response = false;
    wxArrayString arg6;
    StrStrHashMap result1;
    char aux[40];
    char description[40];
	char description_aux[20];
    long price;
    int quantity;
    long vatInfo;
    long unitPrice;
    char unitName[20];
    int res;
//	char *res1;
//    char *res2;
//    char *res3;
    char temp_str[45];
    char types[] = {'s','i','i','i','i','s'};
	memset(aux, 0, sizeof(aux));

    int i;
    for( i = 0;i < (int)strlen( buffer ) && i < sizeof( description );i++ ) {
        if( buffer[i] == '&' ) {
            break;
        }
        else {
            description[i] = buffer[i];
        }
    }

    description[i] = '\0';


    res = sscanf( &buffer[i], "&%ld&%d&%d&%ld&%s", /*description,*/ &price, &quantity, &vatInfo,
                  &unitPrice, unitName );

    if( strlen( description ) > 20 ) {
        memcpy( description_aux, description, ( sizeof(description_aux)-1 ) );
		description_aux[sizeof(description_aux)-1]='\0';
		arg6.Add( description_aux );
	}
    else {
        arg6.Add( description );
    }
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", price );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", quantity );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", vatInfo );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", unitPrice );

    arg6.Add( temp_str );
    arg6.Add( unitName );

	//glog ("ComThread -> CIMain::PrintRecItem",LOG_JAVA_SERVER,2);
    response = PCommand( "printRecItem", arg6, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method PrintRecItem'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
            _snprintf( aux,sizeof(aux), "Fiscal Error: %d printRecItem", rta );
			aux[sizeof( aux ) - 1] = '\0';
            sglog( aux );
        }
		
		ObtenerRespuestas(buffer, result1);
	   /*res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/
	   //
    }
    return rta;
}
/*PREFERENCIAS EN LA IMPRESORA  TMU 220: 


Posibles valores:

‘D’ 0x44 ASCII(68 Decimal) se envía para indicar que se establecerán las preferencias del dispositivo a utilizar para la impresión.
‘P’ 0x50 ASCII(80 Decimal) se envía para indicar que se establecerán las preferencias del papel a utilizar.
‘T’  0x54 ASCII(84 Decimal) se envía para indicar que se establecerán las preferencias de los comprobantes fiscales.

Si en el Campo 02 se envío ‘D’:

‘S’ 0x53 ASCII(83 Decimal)para indicar que se utilizará como estación seleccionada por el usuario el slip paper (hoja suelta).
‘R’ 0x52 ASCII(82 Decimal)para indicar que se utilizará como estación seleccionada por el usuario la de rollo de papel.
Si en el Campo 02 se envío ‘P’:
‘S’ 0x53 ASCII(83 Decimal)indica que se establecerá el tamaño de papel.
Si en el Campo 02 se envío ‘T’:
‘P’ 0x50 ASCII(80 Decimal) se envía para indicar que se establecerán las preferencias de imprimir las leyendas “Suma de sus pagos” y “Su Vuelto”.
‘Q’ 0x51 ASCII(81 Decimal) se envía para indicar que se establecerán las preferencias de imprimir Precio por Cantidad en cada item facturado.


Posibles Valores:
Si en el Campo 02 se envió ‘D’:
‘O’ 0x4F ASCII (79 Decimal) establece que se imprimirán los Documentos No Fiscales por la estación seleccionada en el Campo 03.

Si en el Campo 03 se envió ‘S’:
‘U’ 0x55 ASCII (85 Decimal) establece que se utilizará un tamaño definido por el usuario.
Si en el campo 03 se envió ‘P’ o ‘Q’:
‘N’ 0x4E ASCII(78 Decimal) deselecciona la preferencia respectiva.
‘S’ 0x53 ASCII(83 Decimal) selecciona la preferencia respectiva.
*/

/***************************************************************************************************/
int CIMain::setPrinterPref( char *buffer ){
/*public void setPrinterPref(boolean setSlipDefaultStation, boolean usarDefaultStationParaNoFiscal) {
CAMPO 02

‘D’ 0x44 ASCII(68 Decimal) se envía para indicar que se establecerán las preferencias del dispositivo a utilizar para la impresión.

CAMPO 03

‘S’ 0x53 ASCII(83 Decimal)para indicar que se utilizará como estación seleccionada por el usuario el slip paper (hoja suelta).
‘R’ 0x52 ASCII(82 Decimal)para indicar que se utilizará como estación seleccionada por el usuario la de rollo de papel.


	*/
	
 char types[] = {'b','b'};
 int SlipDefaultStation = 0, usarDefaultStationParaNoFiscal = 0; 
 wxArrayString arg;
 StrStrHashMap result;
 int res = 0,response =0;
 //arg.Alloc(2);
 
	sscanf( &buffer[0], "&%d&%d&", &SlipDefaultStation, &usarDefaultStationParaNoFiscal );
     
	if(SlipDefaultStation == 1)
		arg.Add( "true" );
	else
		arg.Add( "false" ); 

	if(usarDefaultStationParaNoFiscal == 1 )
		arg.Add( "true" );
	else
		arg.Add( "false" );

	response = PCommand( "setPrinterPref", arg, types, result, true );

return 1;
}

int CIMain::setPaperPref( char *buffer ){
	//    public void setPaperPref(int cantColumnas, int cantFilas)  {
/*
CAMPO 02:
‘P’ 0x50 ASCII(80 Decimal) se envía para indicar que se establecerán las preferencias del papel a utilizar.

CAMPO 03:

Si en el Campo 02 se envío ‘P’:
‘S’ 0x53 ASCII(83 Decimal)indica que se establecerá el tamaño de papel.


*/
 char types[] = {'i','i'};
 int cantColumnas=0; 
 int cantFilas = 0;

 wxArrayString arg;
 StrStrHashMap result;
 int response =0;
 
 arg.Alloc(2);
 arg.Add(""); 
  arg.Add("");

 arg[0]="0" ;
 arg[1]="0" ;

//args[0].sprintf("%d", 0);
//args[1].sprintf("%d", 0);

	sscanf( &buffer[0], "&%d&%d&", &cantColumnas, &cantFilas );
     
      

 if(cantColumnas == 1 )
	arg[0]="1" ;

 if(cantFilas == 1)
	arg[1]="1" ;


  response = PCommand( "setPaperPref", arg, types, result, true );


return 1;
}

int CIMain::setComprPref( char *buffer ){ 
//    public void setComprPref(String leyendaPref, boolean activar) {
/* CAMPO 02 :
‘T’  0x54 ASCII(84 Decimal) se envía para indicar que se establecerán las preferencias de los comprobantes fiscales.

CAMPO 03:

Si en el Campo 02 se envío ‘T’:
‘P’ 0x50 ASCII(80 Decimal) se envía para indicar que se establecerán las preferencias de imprimir las leyendas “Suma de sus pagos” y “Su Vuelto”.
‘Q’ 0x51 ASCII(81 Decimal) se envía para indicar que se establecerán las preferencias de imprimir Precio por Cantidad en cada item facturado.

*/
 char types[] = {'s','b'};
  char leyendaPref[100];
  int activa=0;
  wxArrayString arg;
  StrStrHashMap result;
  int response =0,i = 0;

  arg.Alloc(2);
   arg.Add(""); 
  arg.Add("");

 arg[0]="0" ;
 arg[1]="false" ;

 
 
    for( i = 0;i < (int)strlen( buffer ) && i < sizeof( leyendaPref );i++ ) {
        if( buffer[i] == '&' ) {
            break;
        }
        else {
            leyendaPref[i] = buffer[i];
        }
    }
    leyendaPref[i] = '\0';
	arg[0]= leyendaPref ;
 
	sscanf( &buffer[i], "&%i&", &activa );
     
	if(activa == 1 )
		arg[1]="true" ;

 
  response = PCommand( "setComprPref", arg, types, result, true );



return 1;
}

/***************************************************************************************************/
int CIMain::printRecPercep( char *buffer )
{
	bool response;
    wxArrayString arg3;
    StrStrHashMap result1;
    long monto;
    char tipo[2];
	long tasa;
    char description[25];
	char description_aux[20];
    char aux[45];
    int result, i = 0;
    char temp_str[45];
    char types[] = {'s','l','s','l'};
	memset(aux, 0, sizeof(aux));
	
	for( i = 0;i < (int)strlen( buffer ) && i < sizeof( description );i++ ) {
        if( buffer[i] == '&' ) {
            break;
        }
        else {
            description[i] = buffer[i];
        }
    }
    description[i] = '\0';

    sscanf( &buffer[i], "&%ld&%ld&%s", &monto, &tasa, tipo );
    if( strlen( description ) > 20 ) {
        memcpy( description_aux, description, ( sizeof(description_aux)-1 ) );
		description_aux[sizeof(description_aux)-1]='\0';
		arg3.Add( description_aux );
	}
    else {
        arg3.Add( description );
    }
	_snprintf( temp_str, sizeof(temp_str)-1, "%ld", monto );
    arg3.Add( temp_str );
    arg3.Add( tipo );
	_snprintf( temp_str, sizeof(temp_str)-1, "%ld", tasa );
	arg3.Add( temp_str );

    response = PCommand( "printRecPercep", arg3, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printRecPercep" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecPercep", result );
            sglog( aux );
        }
    }

	return result;
}

/*void CIMain::PrintRecSubtotal( char *amount )
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    int decimales,signo;
    int result;
    char aux[20];
    char types[] = {'l'};
	char *res1;
    char *res2;
    char *res3;
	char buffer[100];
    memset(buffer,0,sizeof(buffer));

    arg1.Add( fcvt( atof( amount ), 2, &decimales, &signo ) );
    response = PCommand( "printRecSubtotal", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method PrintRecSubtotal'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecSubtotal", result );
            sglog( aux );
        }
		res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Fiscal"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );
    }
}*/

void CIMain::PrintRecSubtotal( char *amount )
{
    bool response;
     int monto;
     int prints;
    wxArrayString arg2;
    StrStrHashMap result1;
//    int decimales,signo;
    int result;
    char aux[45];
    char types[] = {'i','b'};
//	 char *res1;
//    char *res2;
//    char *res3;
	char *res4;
	 char buffer[100];
    memset(buffer,0,sizeof(buffer));
	memset(aux, 0, sizeof(aux));

    sscanf( amount, "%d&%d&", &monto, &prints );
    
    
    	_snprintf( buffer, sizeof(buffer)-1, "%d", monto );
    	arg2.Add( buffer );
    	_snprintf( buffer, sizeof(buffer)-1, "%d", prints );
		if(prints == 1)
    		arg2.Add( "true" );
		else
			arg2.Add( "false" );
    	
    response = PCommand( "printRecSubtotal", arg2, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method PrintRecSubtotal'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecSubtotal", result );
            sglog( aux );
        }
		//ObtenerRespuestas(buffer, result1);
		/*
		res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Fiscal"] ).c_str();
	   sprintf( buffer, "%s %s %s %s", res1, res2, res3, res4 );*/

	   res4 = ( char* )( result1["subtotal"] ).c_str();
	    memset(buffer,0,sizeof(buffer));

	   sprintf( amount, "%s",res4 );
	   
    }
}
int CIMain::PrintRecTotal( char *buffer )
{
    //long total, long payment, String Description

    bool response;
    wxArrayString arg3;
    StrStrHashMap result1;
    int total, i = 0, j = 0, count = 0;
    int payment;
    char description[25];
    char aux[40];
    int result;
    char temp_str[45];
    char types[] = {'i','i','s'};
//	char *res1;
//    char *res2;
//    char *res3;
	memset(aux, 0, sizeof(aux));

	//sscanf( buffer, "%d&%d&%s", &total, &payment, description );
	sscanf( buffer, "%d&%d&", &total, &payment );
	//Para descripcion con espacio ej TICKET CANASTA
	for( i = 0; i < (int)strlen( buffer ) &&  i < sizeof( description );i++ ) {
		if( buffer[i] == '&' && count < 3 ) {
            count++;
        }
        else 
		{
            if( count == 2 )// son la cantidad de &
				description[j++] = buffer[i];
        }
    }
    description[j] = '\0';
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", total );
    arg3.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", payment );
    arg3.Add( temp_str );
    arg3.Add( description );

    response = PCommand( "printRecTotal", arg3, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method PrintRecTotal'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecTotal", result );
            sglog( aux );
        }
	
		ObtenerRespuestas(buffer, result1);
	   /*res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/
	   
    }
	return result;
}

void CIMain::PrintRecVoid( char *description )
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char aux[40];
    arg1.Add( description );
    int result;
    char types[] = {'s'};
	memset(aux, 0, sizeof(aux));

    response = PCommand( "printRecVoid", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method PrintRecVoid'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
           _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecVoid", result );

           sglog( aux );
        }
    }
}

int CIMain::PrintRecVoidItem( char *buffer )
{
    bool response;
    wxArrayString arg6;
    StrStrHashMap result1;
    char aux[40];
    int rta = -1;
    char description[40];
	char description_aux[20];
    int amount;
    int quantity;
    int adjustmentType;
    int adjustment;
    int vatInfo;
    char temp_str[45];
    char types[] = {'s','i','i','i','i','i'};
//	char *res1;
//    char *res2;
//    char *res3;
	memset(aux, 0, sizeof(aux));

    int i;
    for( i = 0;i < (int)strlen( buffer ) && i < sizeof( description );i++ ) {
        if( buffer[i] == '&' ) {
            break;
        }
        else {
            description[i] = buffer[i];
        }
    }
    description[i] = '\0';

    sscanf( &buffer[i], "&%d&%d&%d&%d&%d", &amount, &quantity, &adjustmentType, &adjustment,
            &vatInfo );

    if( strlen( description ) > 20 )
	{
        for(i=0; i < sizeof( description_aux ); i++){
			description_aux[i] = description[i];
        }
		description_aux[sizeof(description_aux)-1] = '\0';
		arg6.Add( description_aux );
	}
    else {
        arg6.Add( description );
    }
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", amount );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", quantity * -1  );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", adjustmentType );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", adjustment );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", vatInfo );

    arg6.Add( temp_str );

    response = PCommand( "printRecVoidItem", arg6, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method PrintRecVoidItem'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printRecVoidItem", rta );
            sglog( aux );
        }
		ObtenerRespuestas(buffer, result1);
	   /*res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/
    }
    return rta;
}

int CIMain::printRecRefund( char *buffer )
{
    bool response;
    wxArrayString arg4;
    StrStrHashMap result1;
    char aux[40];
    int rta = -1;
    char description[40];
    int adjustmentType;
    int amount;
    int vatInfo;
    int i;
    char temp_str[45];
    char types[] = {'i','s','i','i'};
	memset(aux, 0, sizeof(aux));

    for( i = 0;i < (int)strlen( buffer ) && i < sizeof(description);i++ ) {
        if( buffer[i] == '&' ) {
            break;
        }
        else {
            description[i] = buffer[i];
        }
    }
    description[i] = '\0';

    sscanf( &buffer[i], "&%d&%d&%d", &adjustmentType, &amount, &vatInfo );
    
    arg4.Add( "1" );
    arg4.Add( description );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", amount );
    arg4.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", vatInfo );
    arg4.Add( temp_str );

    response = PCommand( "printRecRefund", arg4, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printRecRefund'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printRecRefund", rta);
            sglog( aux );
        }
    }
    return rta;
}

int CIMain::PrintRecMessage( char *message ) //solo 4 lineas consecutivas
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char aux[40];
    int rta = -1;
    arg1.Add( message );
    char types[] = {'s'};
	memset(aux, 0, sizeof(aux));

    response = PCommand( "printRecMessage", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method PrintRecMessage'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printRecMessage", rta);
            sglog( aux );
        }
		ObtenerRespuestas(message, result1);
		/*{//para devolucion de respuestas
		char *res1;
		char *res2;
		char *res3;
		res1 = ( char* )( result1["PrinterState"] ).c_str();
		res2 = ( char* )( result1["FiscalState"] ).c_str();
		res3 = ( char* )( result1["Info"] ).c_str();
		sprintf( message, "%s %s %s", res1, res2, res3 );
		}*/
    }
    return rta;
}

int  CIMain::PrintXReport( char *buffer)
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char aux[40];
    arg1.Add( "1" );
    int result = 1;
	long ex_timeout;
    char types[] = {'i'};
	memset(aux, 0, sizeof(aux));

    //bloquear hasta que no tenga mas que imprimir
    /*while( !GetPrinterFinish() ) {
        Sleep( 600 );
    }*/
	
	ex_timeout = jposCom->GetTimeout();
	jposCom->SetTimeout(10);
	response = PCommand( "printXReport", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method PrintXReport'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printXReport", result );
            sglog( aux );
			ObtenerRespuestas(buffer, result1);
			if( strlen( buffer) < 3)
				strncpy(buffer, "Error",5);
			result = 0;
        } else
			result = 1;
    }
	jposCom->SetTimeout(ex_timeout);
	return result;
    //Sleep( 8000 );
}

int  CIMain::PrintZReport( char *buffer)
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char aux[40];
    arg1.Add( "1" );
    int result = 1;
	long ex_timeout;
    char types[] = {'i'};
	memset(aux, 0, sizeof(aux));

    //bloquear hasta que no tenga mas que imprimir
    /*while( !GetPrinterFinish() ) {
        Sleep( 600 );dd
    }*/
	ex_timeout= jposCom->GetTimeout();
	jposCom->SetTimeout(10);
    response = PCommand( "printZReport", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method PrintZReport'\n\n" );
		result = 0;
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Fiscal Error: %d en printZReport", result );
            sglog( aux );
			ObtenerRespuestas(buffer, result1);
			if( strlen( buffer) < 3)
				strncpy(buffer, "Error",5);
			result = 0;
        }else
			result = 1;
    }
	jposCom->SetTimeout(ex_timeout);
	return( result);
	//Sleep( 8000 );
}
void CIMain::UpdateNonFiscal(char *buffer ){
	int estado_doc_no_fiscal=0;
	sscanf( buffer, "&%d&", &estado_doc_no_fiscal);
	if( estado_doc_no_fiscal == 1 )
		non_fiscal_ok = true;
	else
		non_fiscal_ok = false;
}

void CIMain::BeginNonFiscal()
{
    int result;

	   

    if( !non_fiscal_ok ) {
        bool response;
        StrStrHashMap result1;
        char aux[40];
		memset(aux, 0, sizeof(aux));
        response = PCommand( "beginNonFiscal", result1, true );
        if( !response ) {
            sglog( "Error calling 'method beginNonFiscal'\n\n" );
        }
        else {
            result = atoi( result1["error"].c_str() );
            if( result != 0 ) {
                _snprintf( aux, sizeof(aux)-1,"Fiscal Error: %d en beginNonFiscal", result );
                sglog( aux );
			}
        }
        Sleep( 3000 );
        non_fiscal_ok = true;

    }
}
int  CIMain::FiscalOpenCashDrawer()
{
    int result;

    {
        bool response = false;
        StrStrHashMap result1;
        char aux[40];
		memset(aux, 0, sizeof(aux));
        response = PCommand( "FiscalOpenCashDrawer", result1, true );
        if( !response ) {
            sglog( "Error calling 'method FiscalOpenCashDrawer'\n\n" );
        }
        else {
            result = atoi( result1["error"].c_str() );
            if( result != 0 ) {
                _snprintf( aux, sizeof(aux)-1,"Fiscal Error: %d en FiscalOpenCashDrawer", result );
                sglog( aux );
            }
        }
		return !result;

    }
}

void CIMain::EndNonFiscal()
{
    int aux1;

    if( non_fiscal_ok ) {
        bool response;
        StrStrHashMap result1;
        char aux[40];
		memset(aux, 0, sizeof(aux));

        //bloquear hasta que no tenga mas que imprimir
        /*while( !GetPrinterFinish() ) {
            Sleep( 600 );
        }*/

        response = PCommand( "endNonFiscal", result1, true );
        if( !response ) {
            sglog( "Error calling 'method endNonFiscal'\n\n" );
        }
        else {
            aux1 = atoi( result1["error"].c_str() );
            if( aux1 != 0 ) {
                _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en endNonFiscal", aux1 );
                sglog( aux );
            }
        }
        Sleep( 2000 );
        non_fiscal_ok = false;
    }
}

void CIMain::setDateFechaHora( char *buffer )
{
	bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char types[] = {'s'};
	char fecha_hora[50];
	char *res1;
    char *res2;

    arg1.Add( buffer );
    response = PCommand( "setDate", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method setDate'\n\n" );
        sprintf( fecha_hora, "%s", "Error en method setDateFechaHora" );
    }
    else {
		res1 = ( char* )( result1["Date"] ).c_str();
		res2 = ( char* )( result1["time"] ).c_str();
		_snprintf( fecha_hora, sizeof( fecha_hora )-1, "Fecha:%s Hora:%s", res1, res2 );
    }
	sglog( fecha_hora );
}

void CIMain::getDateFechaHora( char *buffer )
{
    bool response;
    StrStrHashMap result1;
	char fecha_hora[50];
	char *res1;
    char *res2;

	response = PCommand( "getDate", result1, true );
	
    if( !response ) {
        sglog( "Error calling 'method getDate'\n\n" );
		sprintf( buffer, "%s", "Error" );
		_snprintf( fecha_hora, sizeof( fecha_hora )-1, "%s","Error en method getDate" );
    }
    else {
		res1 = ( char* )( result1["Date"] ).c_str();
		res2 = ( char* )( result1["time"] ).c_str();
		_snprintf( fecha_hora, sizeof( fecha_hora )-1, "Fecha:%s Hora:%s", res1, res2 );
		sprintf( buffer, "%s %s", res1, res2 );
    }
	sglog( fecha_hora );
}

int CIMain::GetFiscalData( int info, char *buffer )
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char aux[50];
    char *res1;
    char *res2;
    char *res3;
    char *res4;
    char *res5;
    int result;
    char info_str[10];
    _snprintf( info_str, sizeof(info_str)-1, "%d", info );
    arg1.Add( info_str );
    char types[] = {'i'};
	memset(aux, 0, sizeof(aux));

    response = PCommand( "getData", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling Fiscal Printer 'method getData'\n\n" );
        sprintf( buffer, "%s", "Error" );
    }
    else {
		if(info != 4){
			ObtenerRespuestas(buffer, result1);
			result = atoi( result1["error"].c_str() );
            if( result != 0 ) {
                _snprintf( aux, sizeof(aux)-1,"Fiscal Error: %d en getData", result );
	            sglog( aux );
            }
		}
		else{
			int dd;
			res1 = ( char* )( result1["Nro_Ticket"] ).c_str();
			res2 = ( char* )( result1["Nro_Factura"] ).c_str();
			res3 = ( char* )( result1["Nro_Z"] ).c_str();
			res4 = ( char* )( result1["Nro_NCA"] ).c_str();
			res5 = ( char* )( result1["Nro_NCB"] ).c_str();
			sprintf( buffer, "%s %s %s %s %s", res1, res2, res3, res4, res5 );
			dd = strlen( res1);
			if( dd == 0 || atoi( res1 ) == 0 ) //no devolvio nada
				response = 0;
			//sglog( aux );
		}

    }
	return response;
}


void CIMain::SetTrailerLine( char *buffer )
{
    bool response;
    wxArrayString arg3;
    StrStrHashMap result1;
    char aux[40], text[40],dobleancho[40], temp_str[40];
    int result = 0;
	int res = 0, line= 0, pos2  = 0, pos_aux  = 0;
    char types[] = {'i','s','s'};

	memset(aux,0,sizeof(aux));
	memset(text,0,sizeof(text));
	memset(dobleancho,0,sizeof(dobleancho));
	memset(temp_str,0,sizeof(temp_str));

	res = sscanf( &buffer[0], "%d&%s", &line, aux);

    pos2 = strcspn( buffer, "&" );
	//strncpy( aux, &buffer[pos2 + 1], strlen(buffer)-1 );
	strncpy( aux, &buffer[pos2 + 1], sizeof(aux)-1 );
	pos_aux = strcspn( aux, "&" );
	//Control en tamaño del vector
	pos2 = sizeof(text) - pos_aux;
	pos2 = (pos2 > 0)? pos_aux:sizeof(text)-1;
	strncpy( text, &aux[0], pos2 );
    //strncpy( dobleancho, &aux[pos2 + 1], strlen(aux)-1 );
	strncpy( dobleancho, &aux[pos2 + 1], sizeof(dobleancho)-1 );

	_snprintf( temp_str, sizeof(temp_str)-1, "%d", line );
    arg3.Add( temp_str );
    arg3.Add( text );
    arg3.Add( dobleancho );

    response = PCommand( "setTrailerLine", arg3, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method setTrailerLine'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Fiscal Error: %d en setTrailerLine", result );
            sglog( aux );
        }
    }
}

void CIMain::SetHeaderLine( char *buffer )
{
    bool response;
    wxArrayString arg3;
    StrStrHashMap result1;
    char aux[40], text[40],dobleancho[40], temp_str[40];
    int result;
	int res = 0, line = 0, pos2 = 0, pos_aux = 0;
    char types[] = {'i','s','s'};
	memset(aux,0,sizeof(aux));
	memset(text,0,sizeof(text));
	memset(dobleancho,0,sizeof(dobleancho));
	memset(temp_str,0,sizeof(temp_str));

	res = sscanf( &buffer[0], "%d&%s", &line, aux);

    pos2 = strcspn( buffer, "&" );
	//strncpy( aux, &buffer[pos2 + 1], strlen(buffer)-1 );
	strncpy( aux, &buffer[pos2 + 1], sizeof(aux)-1 );
	pos_aux = strcspn( aux, "&" );
	//Control en tamaño del vector
	pos2 = sizeof(text) - pos_aux;
	pos2 = (pos2 > 0)? pos_aux:sizeof(text)-1;
    strncpy( text, &aux[0], pos2 );
	//strncpy( dobleancho, &aux[pos2 + 1], strlen(aux)-1 );
    if(strlen(&aux[pos2 ]) >= 4 )
		strncpy( dobleancho, &aux[pos2 + 1], sizeof(dobleancho)-1 );
	else
		sprintf(dobleancho,"false");

	_snprintf( temp_str, sizeof(temp_str)-1, "%d", line );
    arg3.Add( temp_str );
    arg3.Add( text );
    arg3.Add( dobleancho );

    response = PCommand( "setHeaderLine", arg3, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method setHeaderLine'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en setHeaderLine", result );
            sglog( aux );
        }
    }
}

void CIMain::SetTamImage( char *buffer )
{
    sscanf( buffer, "%d#%d", &imageWidth, &imageHeight );
}

void CIMain::SetPostion( char *buffer )
{
    wxArrayString arg2;
    StrStrHashMap result1;
    bool response;
    char aux[40];
    int aux1;
    char types[] = {'i','i'};
    int pos_aux = 0, pos = 0;
	memset(aux, 0, sizeof(aux));
	
	pos_aux = strcspn( buffer, "#" );
	//Control en tamaño del vector
	pos = sizeof(aux) - pos_aux;
	pos = (pos > 0)? pos_aux:sizeof(aux)-1;
    strncpy( aux, buffer, pos );
    arg2.Add( aux );
    strcpy( aux, &buffer[pos + 1] );
    arg2.Add( aux );

    response = PCommand( "setPosition", arg2, types, result1, true );
    if( !response ) {
        sglog( "Error calling Fiscal Printer 'method getData'\n\n" );
        sprintf( buffer, "%s", "Error" );
    }
    else {
        aux1 = atoi( result1["error"].c_str() );
        if( aux1 != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Printer Error: %d en setPosition", aux1 );
            sglog( aux );
        }
    }
}

int CIMain::Print( char *buffer )
{
    bool response;
    wxArrayString arg3;
    StrStrHashMap result1;
    char temp_str[45];
    char types[] = {'s','i','i'};

    arg3.Add( buffer );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", x_pos );   
    arg3.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", y_pos ); 
    arg3.Add( temp_str );

    x_pos = 0;
    y_pos = 0;

    if( print_station == 4 && printer_status ) {
        response = PCommand( "print", arg3, types, result1, true );
        if( !response ) {
            sglog( "Error calling 'method print'\n\n" );
            return -1;
        }
        else {
            return ( atoi( result1["error"].c_str() ) );
        }
    }
    return 0;
}

int CIMain::Timbradora( char *buffer )
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    arg1.Add( "2" ); //impresora 2 configurada en peripheral.xml
    char types[] = {'i'};

    if( printer_status ) {
        response = PCommand( "doPrint", arg1, types, result1, true );
        if( !response ) {
            sglog( "Error calling 'method print'\n\n" );
            return -1;
        }
        else {
            return ( atoi( result1["error"].c_str() ) );
        }
    }
    return 0;
}



/*int CIMain::GetchKey()
{
    bool response;
	int a = -999;
    wxArrayString arg1;
    StrStrHashMap result1;
    arg1.Add( "0" );

    response = PCommand( "getchKey", arg1, result1, true );
    if( !response ) {
        sglog( "Error calling 'method getchKey'\n\n" );
        return -1;
    }
    else {
        if( result1["keyData"] != "-999" ) {
			a = atoi( result1["keyData"].c_str() );
            CInput::setKey( a );
            MainThread::activate_from = 1;
            //nico_signal MainThread::m_condition->Signal();
		}
    }

    return a;
}*/

void CIMain::SetLetter( int tam )   //BORRAR(ignorada por JavaServer) 
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char tam_str[30];
    char types[] = {'i'};

    _snprintf( tam_str, sizeof(tam_str)-1, "%d", tam );
    arg1.Add( tam_str );
    if( printer_status ) {
        response = PCommand( "setLetter", arg1, types, result1, true );
    }
}

void CIMain::SetImage( char *buffer )
{
    bool response;
    wxArrayString arg5;
    StrStrHashMap result1;
    char temp_str[45];
    char types[] = {'s','i','i','i','i'};
 
    arg5.Add( buffer );
	_snprintf( temp_str, sizeof(temp_str)-1, "%d", x_pos );
    arg5.Add( temp_str );
	_snprintf( temp_str, sizeof(temp_str)-1, "%d", y_pos );
    arg5.Add( temp_str );
	_snprintf( temp_str, sizeof(temp_str)-1, "%d", imageWidth );
    arg5.Add( temp_str );
	_snprintf( temp_str, sizeof(temp_str)-1, "%d", imageHeight );
    arg5.Add( temp_str );

    x_pos = 0;
    y_pos = 0;
    if( printer_status ) {
        response = PCommand( "printImage", arg5, types, result1, true );
    }
}

void CIMain::SetFont( char *font, int size )
{
    bool response;
    wxArrayString arg3;
    StrStrHashMap result1;
    char temp_str[45];
    char types[] = {'s','i'};
    
    arg3.Add( font );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", size );
    arg3.Add( temp_str );

    if( printer_status ) {
        response = PCommand( "setFont", arg3, types, result1, true );
    }
}

/**
 * Establece el espacio vertical de una linea, este espacio incluye la altura
 * de la linea mas el espacio hasta la siguiente. 
 * @param space el valor del espacio en puntos (depende de la calidad de la impresora)
 */
void CIMain::setLineSpace( char *space ) 
{
    wxString code;

    lines.push_back( print_data );
    code.sprintf( "!|ILS%03s", space );
    lines.push_back( code.c_str() );
    print_data = "";
}

/**
 * Establece la cantidad de caracteres que se imprimen en una linea.
 * @param chars cantidad de caracteres (min y max dependen de la impresora)
 */
void CIMain::setLineChars( char *chars ) 
{
    wxString code;

    lines.push_back( print_data );
    code.sprintf( "!|ILC%03s", chars );
    lines.push_back( code.c_str() );
    print_data = "";
}
/****************************************************/
void CIMain::printVoucher( char *buffer ) 
/****************************************************/
{
    wxArrayString arg;
    StrStrHashMap result1;
    char temp_str[45];
    int rta;
    char aux[40];
    bool response;
    char types[] = {'s','s','s','s','i','i','i','l','s','i','i','s','i','i','i','i','s','i','s','s','s'};

    int i;
    int j=0;

    memset(temp_str,0,sizeof(temp_str));
	memset(aux, 0, sizeof(aux));

    for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str);i++ ) {
        if( buffer[i] == '&' ) {
            arg.Add(temp_str);
            memset(temp_str,0,sizeof(temp_str));
            j=0;
        }
        else {
            temp_str[j] = buffer[i];
            j++;
        }
    }
	temp_str[sizeof(temp_str)-1] = '\0';
    response = PCommand( "printVoucher", arg, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printVoucher'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printVoucher",rta);
            sglog( aux );
		}
	}
		ObtenerRespuestas(buffer, result1);
		/*{
		char *res1;
		char *res2;
		char *res3;
			res1 = ( char* )( result1["PrinterState"] ).c_str();
			res2 = ( char* )( result1["FiscalState"] ).c_str();
			res3 = ( char* )( result1["Info"] ).c_str();
			sprintf( buffer, "%s %s %s", res1, res2, res3 );
		}*/
    
}
/****************************************************/
void CIMain::feedPaper( char *buffer ) 
/****************************************************/
{
    wxArrayString arg1;
    StrStrHashMap result1;
    int rta;
    bool response;
    char aux[40];
    char types[] = {'i'};

    memset(aux, 0, sizeof(aux));
    arg1.Add(buffer);
    response = PCommand( "feedPaper", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printVoucher'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Fiscal Error: %d en feedPaper", rta );
            sglog( aux );
        }
    }
}

/****************************************************/
void CIMain::PrintNormalFiscal(char *buffer) 
/****************************************************/
{
    bool response;
    wxArrayString arg2;
    StrStrHashMap result1;
	int  i, j, sta, res;
	char temp_str[70];
    char types[] = {'i','s'};
//	char *res1;
//    char *res2;
//    char *res3;
	
	res = sscanf(buffer,"%d&",&sta);
	
	_snprintf( temp_str, sizeof(temp_str)-1, "%d", sta);
    arg2.Add(temp_str);
	
    for(i = 0;i < (int)strlen( buffer );i++) {
        if(buffer[i]=='&') {
			break;
        }
    }
    for( j = i; j < (int)strlen(buffer) && j < sizeof( temp_str ) ;j++ ) {
		temp_str[j-i]=buffer[j+i];
    }
	temp_str[sizeof(temp_str)-1] = '\0';

    arg2.Add(temp_str);
	response = PCommand( "printNormalFiscal", arg2, types, result1, true );
	//hmmm desarrollo
		ObtenerRespuestas(buffer, result1);
	   /*res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/

    if( !response ) {
        sglog( "Error calling 'method PrintNormalFiscal'\n\n" );
	}/*else{
	
	   res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );
	
	}*/
}

/**************** BONIFICACION **********************/
int CIMain::PrintRecItemAdjustment(char *buffer) 
/****************************************************/
{
    /*
     *  devuelve el error de Jpos.
     */
    int rta = -1;
    bool response;
    wxArrayString arg6;
    StrStrHashMap result1;
    char aux[40];
    char description[40];
    long price;
    int quantity;
    long vatInfo;
    long unitPrice;
    char unitName[20];
    int res;
    char temp_str[45];
    char types[] = {'s','l','i','i','l','s'};
//	char *res1;
//    char *res2;
//    char *res3;
    int i;
	memset(aux, 0, sizeof(aux));
	memset(description,0,sizeof(description));
    for( i = 0;i < (int)strlen( buffer ) && i < sizeof(description);i++ ) {
        if( buffer[i] == '&' ) {
            break;
        }
        else {
            description[i] = buffer[i];
        }
    }

    description[sizeof(description)-1] = '\0';


    res = sscanf( &buffer[i], "&%ld&%d&%d&%ld&%s", &price, &quantity, &vatInfo,
                  &unitPrice, unitName );
    
    arg6.Add( description );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", price );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", quantity );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", vatInfo );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1,"%d", unitPrice );
    arg6.Add( temp_str );
    arg6.Add( unitName );

    response = PCommand( "printRecItemAdjustment", arg6, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method PrintRecItemAdjustment'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printRecItemAdjustment", rta);
            sglog( aux );
        }
			ObtenerRespuestas(buffer, result1);
	   /*res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/
    }
    return rta; 
}
/************** CANCELA BONIFICACION ****************/
int CIMain::PrintRecItemVoidAdjustment(char *buffer) 
/****************************************************/
{
    /*
     *  devuelve el error de Jpos.
     */
    int rta = -1;
    bool response;
    wxArrayString arg6;
    StrStrHashMap result1;
    char aux[40];
    char description[40];
    long price;
    int quantity;
    long vatInfo;
    long unitPrice;
    char unitName[20];
    int res;
    char temp_str[45];
    char types[] = {'s','l','i','i','l','s'};
//	char *res1;
//    char *res2;
//    char *res3;


    int i;
	memset(aux, 0, sizeof(aux));
	memset(description,0,sizeof(description));
    for( i = 0;i < (int)strlen( buffer ) && i < sizeof(description);i++ ) {
        if( buffer[i] == '&' ) {
            break;
        }
        else {
            description[i] = buffer[i];
        }
    }

    description[sizeof(description)-1] = '\0';

    res = sscanf( &buffer[i], "&%ld&%d&%d&%ld&%s", &price, &quantity, &vatInfo,
                  &unitPrice, unitName );
    
    arg6.Add( description );
    _snprintf( temp_str, sizeof(temp_str), "%d", price );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str), "%d", quantity );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str), "%d", vatInfo );
    arg6.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str), "%d", unitPrice );
    arg6.Add( temp_str );
    arg6.Add( unitName );

    response = PCommand( "printRecItemVoidAdjustment", arg6, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method PrintRecItemVoidAdjustment'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printRecItemVoidAdjustment", rta);
            sglog( aux );
        }
		
			ObtenerRespuestas(buffer, result1);
	   /*res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/
	   //
    }
    return rta;
}
/*****AGREGAR ITEM EN FACTURA O NOTA DE CREDITO *****/
int CIMain::printRecItemFac(char *buffer) 
/****************************************************/
{
	int rta = -1;
    bool response;
    wxArrayString arg8;
    StrStrHashMap result1;
	//String description, long price, int quantity, int vatInfo, String extra, String extra1, String extra2, long impint
	char aux[40];
    char temp_str[45];
	int i, j=0;
    char types[] = {'s','l','i','i','s','s','s','l','c'};
//    char *res1;
//    char *res2;
//    char *res3;
 
 
	memset(aux, 0, sizeof(aux));

	memset(temp_str,0,sizeof(temp_str));
	 
	for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str);i++ ) 
	{
			if( buffer[i] == '&' ) 
			{
				arg8.Add(temp_str);
				memset(temp_str,0,sizeof(temp_str));
				j=0;
			}
			else 
			{
				temp_str[j] = buffer[i];
				j++;
			}
	}
	temp_str[sizeof(temp_str)-1] = '\0';

	response = PCommand( "printRecItemFac", arg8, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printRecItemFac'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printRecItemFac",rta);
            sglog( aux );
        }
			ObtenerRespuestas(buffer, result1);
      /*res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/
    }
    return rta;
}
/**** CANCELAR ITEM EN FACTURA O NOTA DE CREDITO ****/
int CIMain::printRecItemVoidFac(char *buffer) 
/****************************************************/
{
	int rta = -1;
    bool response;
    wxArrayString arg8;
    StrStrHashMap result1;
	//String description, long price, int quantity, int vatInfo, String extra, String extra1, String extra2, long impint
	char aux[40];
    char temp_str[45];
	int i, j=0;
    char types[] = {'s','l','i','i','s','s','s','l','c'};
//    char *res1;
//    char *res2;
//    char *res3;
 
	memset(temp_str,0,sizeof(temp_str));
	memset(aux, 0, sizeof(aux));
	 
    for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str); i++ ) 
    {
        if( buffer[i] == '&' ) 
        {
            arg8.Add(temp_str);
            memset(temp_str,0,sizeof(temp_str));
            j=0;
        }
        else 
        {
            temp_str[j] = buffer[i];
            j++;
        }
    }
	temp_str[sizeof(temp_str)-1] = '\0';
	response = PCommand( "printRecItemVoidFac", arg8, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printRecItemVoidFac'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printRecItemVoidFac", rta);
            sglog( aux );
        }
		/*
        res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );
	   */
		ObtenerRespuestas(buffer, result1);
    }
    return rta;
}

/****  ITEM BONFICACION EN FACTURA O NOTA DE CREDITO ****/
int CIMain::printBonItemFac(char *buffer) 
/****************************************************/
{
	int rta = -1;
    bool response;
    wxArrayString arg8;
    StrStrHashMap result1;
	//String description, long price, int quantity, int vatInfo, String extra, String extra1, String extra2, long impint
	char aux[40];
    char temp_str[45];
	int i, j=0;
    char types[] = {'s','l','i','i','s','s','s','l','c'};
//    char *res1;
//    char *res2;
//    char *res3;
 
	memset(temp_str,0,sizeof(temp_str));
	memset(aux, 0, sizeof(aux));
	 
    for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str); i++ ) 
    {
        if( buffer[i] == '&' ) 
        {
            arg8.Add(temp_str);
            memset(temp_str,0,sizeof(temp_str));
            j=0;
        }
        else 
        {
            temp_str[j] = buffer[i];
            j++;
        }
    }
	temp_str[sizeof(temp_str)-1] = '\0';
	response = PCommand( "printBonItemFac", arg8, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printBonItemFac'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printBonItemFac", rta);
            sglog( aux );
        }
		/*
      res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/
			ObtenerRespuestas(buffer, result1);
    }
    return rta;
}
/****  ITEM ANULA BONFICACION EN FACTURA O NOTA DE CREDITO ****/
int CIMain::printAnulBonifItemFac(char *buffer) 
/****************************************************/
{
	int rta = -1;
    bool response;
    wxArrayString arg8;
    StrStrHashMap result1;
	//String description, long price, int quantity, int vatInfo, String extra, String extra1, String extra2, long impint
	char aux[40];
    char temp_str[45];
	int i, j=0;
    char types[] = {'s','l','i','i','s','s','s','l','c'};
//    char *res1;
//    char *res2;
//    char *res3;
 
	memset(temp_str,0,sizeof(temp_str));
	memset(aux, 0, sizeof(aux));
	 
    for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str); i++ ) 
    {
        if( buffer[i] == '&' ) 
        {
            arg8.Add(temp_str);
            memset(temp_str,0,sizeof(temp_str));
            j=0;
        }
        else 
        {
            temp_str[j] = buffer[i];
            j++;
        }
    }
	temp_str[sizeof(temp_str)-1] = '\0';
	response = PCommand( "printAnulBonifItemFac", arg8, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printAnulBonifItemFac'\n\n" );
    }
    else {
        rta = atoi( result1["error"].c_str() );
        if( rta != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en printAnulBonifItemFac", rta);
            sglog( aux );
        }
			ObtenerRespuestas(buffer, result1);
			/*
      res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );*/
    }
    return rta;
}


/*********** ABRIR DOCUMENTO TIQUE-FACTURA **********/
void CIMain::beginFac(char *buffer) 
/****************************************************/
{

//char docfiscal, char formtype, char IvaEmi, char IvaComp, String name, String lastname,String docType,
//String DocNum, String address, String address2, String address3, String remito, String remito1
	bool response;
    wxArrayString arg13;
    StrStrHashMap result1;
    char aux[45];
    int result;
    char temp_str[45];
	int i, j=0;
    char types[] = {'c','c','c','c','s','s','s','s','s','s','s','s','s'};
 
	memset(temp_str,0,sizeof(temp_str));
	memset(aux, 0, sizeof(aux));
 
	for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str); i++ ) 
	{
        if( buffer[i] == '&' ) 
		{
            arg13.Add(temp_str);
			memset(temp_str,0,sizeof(temp_str));
			j=0;
		}
        else {
            temp_str[j] = buffer[i];
			j++;
        }
    }
	temp_str[sizeof(temp_str)-1] = '\0';
	response = PCommand( "beginFac", arg13, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method beginFac'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en beginFac",result);
            sglog( aux );
        }
    }
}

/********* ABRIR DOCUMENTO NOTA DE CREDITO **********/
void CIMain::beginRepaymentNote(char *buffer) 
/****************************************************/
{

//char docfiscal, char formtype, char IvaEmi, char IvaComp, String name, String lastname,String docType,
//String DocNum, String address, String address2, String address3, String remito, String remito1

	bool response;
    wxArrayString arg13;
    StrStrHashMap result1;
    char aux[45];
    int result;
    char temp_str[45];
	int i, j=0;
    char types[] = {'c','c','c','c','s','s','s','s','s','s','s','s','s'};
 
	memset(temp_str,0,sizeof(temp_str));
	memset(aux, 0, sizeof(aux));
 
	for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str);i++ ) 
	{
        if( buffer[i] == '&' ) 
		{
            arg13.Add(temp_str);
			arg13.GetStringArray();
			memset(temp_str,0,sizeof(temp_str));
			j=0;
		}
        else {
            temp_str[j] = buffer[i];
			j++;
        }
    }
	temp_str[sizeof(temp_str)-1] = '\0';
	response = PCommand( "beginRepaymentNote", arg13, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method beginRepaymentNote'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
			_snprintf(aux, sizeof(aux)-1, "Fiscal Error: %d en beginRepaymentNote",result);
            sglog( aux );
        }
    }
}
/********* CERRAR DOCUMENTO TIQUE-FACTURA ***********/
int CIMain::endFac(char *docfiscal) 
/****************************************************/
{
	//char docfiscal
	bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
	int result;
	char aux[37];
    char types[] = {'c'};
	memset(aux, 0, sizeof(aux));

	result = 1;
	arg1.Add( docfiscal );

    response = PCommand( "endFac", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method endFac'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en endFac", result );
			aux[36]= '\0';
            sglog( aux );
        }
    }
	return result;
}
/******** CERRAR DOCUMENTO NOTA DE CREDITO **********/
void CIMain::endRepaymentNote(char *docfiscal) 
/****************************************************/
{
	bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
	int result;
	char aux[40];
    char types[] = {'c'};
	memset(aux, 0, sizeof(aux));

	arg1.Add( docfiscal );

    response = PCommand( "endRepaymentNote", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method endRepaymentNote'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en endRepaymentNote", result );
            sglog( aux );
        }
    }

}

/* TOTAL o PAGO en  NOTA DE CREDITO o TIQUE-FACTURA**/
int  CIMain::printRecTotalFac(char *buffer) 
/****************************************************/
{
	bool response;
    wxArrayString arg3;
    StrStrHashMap result1;
    int total, i = 0, j = 0, count = 0;
    int payment, modoPago;
    char description[25];
    char aux[40];
    int result = -1;
    char temp_str[45];
    char types[] = {'l','l','s','i'};
//	char *res1;
//    char *res2;
//    char *res3;

	memset(aux, 0, sizeof(aux));

    //sscanf( buffer, "%d&%d&%s", &total, &payment, description );
	sscanf( buffer, "%d&%d&", &total, &payment );
	//Para descripcion con espacio ej TICKET CANASTA
	for( i = 0; i < (int)strlen( buffer ) &&  i < sizeof( description );i++ ) {
		if( buffer[i] == '&' && count < 3 ) {
            count++;
        }
        else 
		{
            if( count == 2 )// son la cantidad de &
				description[j++] = buffer[i];
        }
    }
	description[j] = '\0';
	count = 0;
	for( i = 0; i < (int)strlen( buffer );i++ ) {
		if( buffer[i] == '&' && count < 4 ) {
            count++;
        }
        else 
		{
			if( count == 3 ){// son la cantidad de &
				modoPago = (int) buffer[i] - '0';
				break;
			}

        }
    }

    _snprintf( temp_str, sizeof(temp_str)-1, "%d", total );
    arg3.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", payment );
    arg3.Add( temp_str );
    arg3.Add( description );
	_snprintf( temp_str, sizeof(temp_str)-1, "%d", modoPago );
    arg3.Add( temp_str );

    response = PCommand( "printRecTotalFac", arg3, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printRecTotalFac'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecTotalFac", result );
            sglog( aux );
        }
		ObtenerRespuestas(buffer, result1);
		/*
	   res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Info"] ).c_str();
	   sprintf( buffer, "%s %s %s", res1, res2, res3 );
	   */
	
    }
	return result;
}


/* TOTAL o PAGO  para bonificacion o recargo en  NOTA DE CREDITO o TIQUE-FACTURA**/
void CIMain::printRecBoniDtoFac(char *buffer) 
/****************************************************/
{
	bool response;
    wxArrayString arg3;
    StrStrHashMap result1;
    int total;
    int payment;
    char description[25];
    char aux[40];
    int result;
    char temp_str[45];
    char types[] = {'l','l','s'};

    memset(aux, 0, sizeof(aux));
    sscanf( buffer, "%d&%d&%s", &total, &payment, description );

    _snprintf( temp_str, sizeof(temp_str)-1, "%d", total );
    arg3.Add( temp_str );
    _snprintf( temp_str, sizeof(temp_str)-1, "%d", payment );
    arg3.Add( temp_str );
    arg3.Add( description );

    response = PCommand( "printRecBoniDtoFac", arg3, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printRecBoniDtoFac'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecBoniDtoFac", result );
            sglog( aux );
        }
    }
	/* descuento
	Pedido:29/04/2010 03:55:59 a.m..46464 (+0.0000 seconds)

 06 02 C1 0B 04 1C 00 00 1C 45 6E 76 69 6F 20 61   ..Á......Envio a
 20 44 6F 6D 69 63 69 6C 69 6F 1C 31 30 30 30 03    Domicilio.1000.
 30 38 32 35                                       0825

	recargo
	Pedido:29/04/2010 03:59:11 a.m..82764 (+0.0000 seconds)

 06 02 C7 0B 04 1C 00 01 1C 45 6E 76 69 6F 20 61   ..Ç......Envio a
 20 44 6F 6D 69 63 69 6C 69 6F 1C 35 30 03 30 37    Domicilio.50.07
 44 30                                             D0


	*/
}

/***** CANCELAR  NOTA DE CREDITO o TIQUE-FACTURA *****/
void CIMain::printRecVoidFac(char *buffer) 
/****************************************************/
{
	bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char description[25];
    char aux[45];
    int result;
    char types[] = {'s'};

    memset(aux, 0, sizeof(aux));
    sscanf( buffer, "%s", description );

    arg1.Add( description );

    response = PCommand( "printRecVoidFac", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method printRecVoidFac'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Fiscal Error: %d en printRecVoidFac", result );
            sglog( aux );
        }
    }
}
/*
SUBTOTAL EN NOTA DE CREDITO o TIQUE-FACTURA
void CIMain::PrintRecSubtotalFac( char *amount ) 

{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    int decimales,signo;
    int result;
    char types[] = {'l'};

    arg1.Add( fcvt( atof( amount ), 2, &decimales, &signo ) );

    char aux[20];

    response = PCommand( "printRecSubtotalFac", arg1, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method PrintRecSubtotalFac'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecSubtotalFac", result );
            sglog( aux );
        }
    }
}*/

void CIMain::PrintRecSubtotalFac( char *amount )
{
    bool response;
     int monto;
     int prints;
    wxArrayString arg2;
    StrStrHashMap result1;
//    int decimales,signo;
    int result;
    char aux[40];
    char types[] = {'i','b'};
//	 char *res1;
//    char *res2;
//    char *res3;
	char *res4;
	 char buffer[100];
    memset(buffer,0,sizeof(buffer));
	memset(aux, 0, sizeof(aux));

    sscanf( amount, "%d&%d&", &monto, &prints );
    
    
    	_snprintf( buffer, sizeof(buffer)-1, "%d", monto );
    	arg2.Add( buffer );
    	_snprintf( buffer, sizeof(buffer)-1, "%d", prints );
		if(prints == 1)
    		arg2.Add( "true" );
		else
			arg2.Add( "false" );
    	
    response = PCommand( "printRecSubtotalFac", arg2, types, result1, true );

    if( !response ) {
        sglog( "Error calling 'method printRecSubtotalFac'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1, "Fiscal Error: %d en printRecSubtotalFac", result );
            sglog( aux );
        }
			ObtenerRespuestas(buffer, result1);
		/*res1 = ( char* )( result1["PrinterState"] ).c_str();
	   res2 = ( char* )( result1["FiscalState"] ).c_str();
	   res3 = ( char* )( result1["Fiscal"] ).c_str();
	   res4 = ( char* )( result1["subtotal"] ).c_str();
	   sprintf( buffer, "%s %s %s %s", res1, res2, res3, res4 );*/
	    memset(buffer,0,sizeof(buffer));
		res4 = ( char* )( result1["subtotal"] ).c_str();
	   sprintf( amount, "%s",res4 );
    }
}
void CIMain::ObtenerRespuestas(char * buffer, StrStrHashMap rta_prn_hash){
		char *res1=0;
		char *res2=0;
		char *res3=0;
		char *res4=0;
	
	   res1 = ( char* )( rta_prn_hash["PrinterState"] ).c_str();
	   res2 = ( char* )( rta_prn_hash["FiscalState"] ).c_str();
	   res3 = ( char* )( rta_prn_hash["Info"] ).c_str();
	   res4 = ( char* )( rta_prn_hash["Serial"] ).c_str();
	   if( buffer != NULL)
	   sprintf( buffer, "%s %s %s %s", res1, res2, res3,res4 );
}
///Pinpad
int CIMain::pinpadMostrar(char *buffer) 
{
    bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char description[17];
    char aux[45];
    int result;
    char types[] = {'s'};

    memset(aux, 0, sizeof(aux));
	memset( description, 0, sizeof(description) );
    sscanf( buffer, "%s", description );
	_snprintf( description, sizeof(description),buffer);
	description[16]= '\0';

    arg1.Add( description );

    response = PCommand( "pinMostrar", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method pinMostrar'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en pinMostrar", result );
            sglog( aux );
        }
    }
	return (result); //despues devolver la respuesta
}
int CIMain::pinpad_select_mk(char *buffer) {
	bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char aux[45];
    int result = 1;
    char types[] = {'i'};

    memset(aux, 0, sizeof(aux));
    arg1.Add(buffer);

    response = PCommand( "pinSelectMasterKey", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method pinSelectMasterKey'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en pinSetTimeout", result );
            sglog( aux );
        }
    }
	return (result);
}
int  CIMain::pinpad_cancel_session(char *buffer) {
	char *res1=0;
	int result = 1;
	char aux[50];
	bool response;

	memset(aux,0, sizeof(aux));
	StrStrHashMap result1;
    response = PCommand( "pinCancelSession", result1, true );
	if( !response ) {
        sglog( "Error calling 'method pinCancelSession('\n\n" );
    }
    else {
	    result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en cancel_session", result );
            sglog( aux );
		}
	}
	return ( result); // 0 no error otro valor es error
}
int CIMain::pinpad_timeout(char *buffer) {
	bool response;
    wxArrayString arg1;
    StrStrHashMap result1;
    char aux[45];
    int result = 1;
    char types[] = {'i'};

    memset(aux, 0, sizeof(aux));
    arg1.Add(buffer);

    response = PCommand( "pinSetTimeout", arg1, types, result1, true );
    if( !response ) {
        sglog( "Error calling 'method pinSetTimeout'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en pinSetTimeout", result );
            sglog( aux );
        }
    }
	return (result);
}
int  CIMain::pinpad_mensajes_rot(char *buffer) 
{
//pinMensajesRotativos(String mensaje1, String mensaje2, String mensaje3, String mensaje4, String mensaje5, String mensaje6, String mensaje7)
	bool response;
     wxArrayString arg2;
     StrStrHashMap result1;
    int result= 1;
    char aux[40];
	char temp_str[100];
    char types[7] = {'s','s','s','s','s','s','s'};
	int i,j= 0;
    
	memset(temp_str,0,sizeof(temp_str));
	memset(aux, 0, sizeof(aux));
	memset(aux, 0, sizeof(aux));
 
	for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str);i++ ) 
	{
        if( buffer[i] == '&' ) 
		{
            arg2.Add(temp_str);
			arg2.GetStringArray();
			memset(temp_str,0,sizeof(temp_str));
			j=0;
		}
        else {
            temp_str[j] = buffer[i];
			j++;
        }
    }
	temp_str[sizeof(temp_str)-1] = '\0';
	response = PCommand( "pinMensajesRotativos", arg2, types, result1, true );
	if( !response ) {
        sglog( "Error calling 'method pinVersion'\n\n" );
	} else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en pinpad_mensajes_rot", result );
            sglog( aux );
		}
	}
	return ( result); // 0 no error otro valor es error
}
int CIMain::pinpad_transfer_mk(char *buffer) {
	return (0);
}
int CIMain::pinpad_version(char *buffer)
{
	char *res1=0;
	int result = 1;
	char aux[50];
	bool response;

	memset(aux,0, sizeof(aux));
	StrStrHashMap result1;
    response = PCommand( "pinVersion", result1, true );
	if( !response ) {
        sglog( "Error calling 'method pinVersion'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en pinpad_version", result );
            sglog( aux );
		}else {
			res1 = ( char* )( result1["version"] ).c_str();
			if( res1 != NULL)
				_snprintf( buffer,49, "%s", res1);
		}
	}
	return ( result); // 0 no error otro valor es error
}

int CIMain::pinpad_pedir_pin(char *buffer){
//Método pinPedirPin(String nro_tarjeta, String working_key, String mensaje1, String mensaje2, String mensaje3)
	bool response;
     wxArrayString arg2;
     StrStrHashMap result1;
    int result= 1, res;
    char aux[40];
	char temp_str[100];
    char types[7] = {'s','s','s','s','s','i'};
	int i,j= 0, timeout;
	char *res1=0, *res2=0;
    
	memset(temp_str,0,sizeof(temp_str));
	memset(aux, 0, sizeof(aux));


	for( i = 0;i < (int)strlen( buffer ) && j < sizeof(temp_str);i++ ) 
	{
        if( buffer[i] == '&' ) 
		{
            arg2.Add(temp_str);
			arg2.GetStringArray();
			memset(temp_str,0,sizeof(temp_str));
			j=0;
		}
        else {
            temp_str[j] = buffer[i];
			j++;
        }
    }
   //temp_str[j] = '\0';
   res = sscanf( &buffer[i-2], "%ld", &timeout);
   _snprintf( temp_str, sizeof(temp_str), "%d", timeout );
    arg2.Add( temp_str );
    
   
   
   habilitapingjserver = false ;

	response = PCommand( "pinPedirPin", arg2, types, result1, true );
	habilitapingjserver = true ;

	if( !response ) {
        sglog( "Error calling 'method pinpad_pedir_pin'\n\n" );
	} else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en pinpad_pedir_pin", result );
            sglog( aux );
		}else {
			res1 = ( char* )( result1["pin"] ).c_str(); //stirng hexa de 16 bytes
			res2 = ( char* )( result1["pinlen"] ).c_str(); //stirng hexa de 16 bytes
			if( res1 != NULL && res2 != NULL )
				_snprintf( buffer,49, "%s&%s", res1, res2);
		}
	}
	return ( result); // 0 no error otro valor es error

}
int CIMain::pinpad_get_key(char *buffer)
{
    char *res1=0;
	int result = 1;
	char aux[50];
	bool response;
	wxArrayString arg1;
	char types[] = {'i'};
//Key Pressed 1 2 3 4 5 6 7 8 9  *  0 #  /a F1 F2  F3
//   Key Code 1 2 3 4 5 6 7 8 9  *  0 #  /  A  B   C
	memset(aux,0, sizeof(aux));
	arg1.Add(buffer);
	StrStrHashMap result1;

	response = PCommand( "pinGetKey", result1, true );
	if( !response ) {
        sglog( "Error calling 'method pinGetKey'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en pinGetKey", result );
            sglog( aux );
		}else {
			result1["key"] = "#";
			res1 = ( char* )( result1["key"] ).c_str();
			if( res1 != NULL)
				_snprintf( buffer,49, "%s", res1);
		}
	}
	return ( result); // 0 no error otro valor es error

}
int CIMain::pinpad_get_string(char *buffer){
	return (0);
}
int CIMain::pinpad_online(char *buffer)
{
	char *res1=0;
	int result = 1;
	char aux[50];
	bool response;

	memset(aux,0, sizeof(aux));
	StrStrHashMap result1;
    response = PCommand( "pinVerificarOnline", result1, true );

	if( !response ) {
        sglog( "Error calling 'method pinVerificarOnline'\n\n" );
    }
    else {
        result = atoi( result1["error"].c_str() );
        if( result != 0 ) {
            _snprintf( aux, sizeof(aux)-1,"Error Pinpad: %d en pinpad_version", result );
            sglog( aux );
		}else {
			res1 = ( char* )( result1["online"] ).c_str();
			if( res1 != NULL)
				_snprintf( buffer,49, "%s", res1);
		}
	}
	return ( result); // 0 no error --- >0 error de comunicacion 
}
