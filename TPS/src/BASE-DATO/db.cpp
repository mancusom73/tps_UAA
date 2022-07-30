/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   -
*   Descripcion :   Funciones utilizadas como interface entre C y C++
*                   Para el manejo de base de datos con ODBC
*
*   Autor
*   -----
*   Forray Juan Pablo
********************************************************************************/
extern "C" {
#include <stdio.h>
#include <mensaje.h>
#include <log.h>
#include <get_fech.h>
#include <get_hora.h>
#include <cmemory.h>
}
//#include <LOG.h>
#pragma warning (disable:4103)  //aviso de pragma pack en cr.h
#include <wx/db.h>
#include <wx/dbtable.h>
#include <wx/log.h>
#include <db.h>
#include <cr.h>
#pragma pack(8)
#include <odbc.h>
#include <dbtables.h>
#include <aarch.h>
#include <deftables.h>
#include <stdarg.h>
#include <dbscript.h>
#include <math.h>
// #include <stringt.h>


#if defined(INVEL_L)
#define _snprintf snprintf
#include <wx/string.h>
#include <wx/log.h>
#endif

ODBC *oODBCConect = new ODBC();     //Conexion con ODBC
DbTables *oTablas = new DbTables(); //Manejador de tablas
DbTable *oTablaActual = NULL;       //Puntero a la tabla actual
char * GetAppExePath();

#ifdef __cplusplus
extern "C"
{
#endif
#include <str.h>
#include <abort.h>
int open_nro_tabla = 0, open_tipo_tabla = 0;

/***********************************************************************************/
void ALERTAS_SIN_CARGAR_CONF(char * mensaje, char *titulo)
/***********************************************************************************/
	{
#ifdef INVEL_W
		MessageBox(NULL, mensaje ,titulo, MB_OK | MB_ICONSTOP);
#endif
	}


/***************************************************************************/
int ALERTA_EN_VENTANA( char *mensaje,char *titulo, unsigned int iconos, char* titulo_venta_activa, int pregunta )
/***************************************************************************/
{
    //OJOOOOOOO no mandar caractreres raros en el mensaje ni titulo como acentos y simbolos como <>
	char cadena[250];
    unsigned int icono_final=0;
    int rtapregunta =0;

    memset(cadena,0,sizeof(cadena));
    glog(mensaje,1,3);

	GRABAR_LOG_SISTEMA( mensaje, LOG_INCONDICIONAL, iconos);
		switch(iconos){
			case 1:
				icono_final = MB_ICONHAND;
				break;

			case 2:
				icono_final = MB_ICONQUESTION;
				break;

			case 3:
				icono_final = MB_ICONEXCLAMATION;
				break;

			case 4:
				icono_final = MB_ICONASTERISK;
				break;

			case 5:
				icono_final = MB_ICONWARNING;
				break;

			default:
				icono_final = MB_ICONSTOP;
				break;
		}
		MessageBox(NULL, mensaje ,(titulo==NULL)?"Mensajes T P S":titulo,MB_OK | icono_final | MB_SYSTEMMODAL /*MB_ICONSTOP*/);
		return 0;

}

/***************************************************************************/
int ALERTA_EN_VENTANA2( char *mensaje,char *titulo, unsigned int iconos, char* titulo_venta_activa, int pregunta, int tecla_si, int tecla_no )
/***************************************************************************/
{
	 //OJOOOOOOO no mandar caractreres raros en el mensaje ni titulo como acentos y simbolos como <>
	string mensaje2(mensaje);
	char tecla_s[20] ,tecla_n[20];
	unsigned int icono_final=0;
	int teclapresionada=0;

	   //borrar caracteres que fallan como < >
	   {
		  string::size_type n = mensaje2.length();
	     for (string::size_type i = mensaje2.find("<");
	         i != string::npos;
	         i = mensaje2.find("<"))
	         //mensaje2.erase(i, n);
	     	 mensaje2.erase(i, 1);
	     for (string::size_type i = mensaje2.find(">");
	     	         i != string::npos;
	     	         i = mensaje2.find(">"))
	     	         //mensaje2.erase(i, n);
	     	 	 	 mensaje2.erase(i, 1);

	   }
	   //Convertir teclas en texto para boton
	   {
		   memset(tecla_s,0,sizeof(tecla_s));
		   switch(tecla_si){
		   case 13: //enter
		   	   sprintf(tecla_s,"ENTER");
			   break;
		   case 27: //escape
			   sprintf(tecla_s,"ESCAPE");
			   break;
		   case 32:	//barra espaciadora
			   sprintf(tecla_s,"BARRA ESP.");
			   break;
		   case 0:	//
			   sprintf(tecla_s,"SI");
		   		break;
		   default:
            	sprintf(tecla_s,"%c",toupper(tecla_si));
		   }
		   memset(tecla_n,0,sizeof(tecla_n));
		  		   switch(tecla_no){
		  		   case 13: //enter
		  		   	   sprintf(tecla_n,"ENTER");
		  			   break;
		  		   case 27: //escape
		  			   sprintf(tecla_n,"ESCAPE");
		  			   break;
		  		   case 32:	//barra espaciadora
		  			   sprintf(tecla_n,"BARRA ESP.");
		  			   break;
		  		 case 0:	//
		  			  sprintf(tecla_n,"NO");
		  		 	  break;
		  		   default:
		              	sprintf(tecla_n,"%c",toupper(tecla_no));
		  		   }

	   }
	   		switch(iconos){
			case 1:
				icono_final = MB_ICONHAND;
				break;

			case 2:
				icono_final = MB_ICONQUESTION;
				break;

			case 3:
				icono_final = MB_ICONEXCLAMATION;
				break;

			case 4:
				icono_final = MB_ICONASTERISK;
				break;

			case 5:
				icono_final = MB_ICONWARNING;
				break;

			default:
				icono_final = MB_ICONSTOP;
				break;
		}
		teclapresionada = MessageBox(NULL, mensaje ,(titulo==NULL)?"Mensajes T P S":titulo,MB_YESNO | icono_final | MB_SYSTEMMODAL /*MB_ICONSTOP*/);
		/*#define IDYES               6
		#define IDNO                7
		*/
		return teclapresionada;

}

/*************************************************************************************/
   int initOdbc(char *pDsn,char *pUser, char *pPasswd, int pNroConnect){
/*************************************************************************************/   
		int rta = SI,aux=0;
		char cadena[50];
      //oODBCConect = new ODBC(); (solo una instancia)
      //oTablas = new DbTables(); (no es necesario para la base del servidor)
		if( pDsn[0] == 0 && pNroConnect == ODBC_SERVIDOR ) {// si no hay dbserver devolvemos 0
			rta = 0;
			return (0);
		} else
			rta = oODBCConect->Conect( pDsn, pUser, pPasswd, "", pNroConnect );
	  
	  if( rta ) {
		if( pNroConnect == ODBC_LOCAL  ){//SIN DICCIONARIO PROCESADO, por lo tanto el mensaje debe ser otro
				//MIGRAR A LINUX....
				#ifdef INVEL_W
					MessageBox(NULL,  oODBCConect->getError(pNroConnect) ,"Error de Configuracion", MB_OK | MB_ICONSTOP);
				#endif
		}else{				
			MENSAJE( oODBCConect->getError(pNroConnect) );
			GRABAR_LOG_BD( oODBCConect->getError(pNroConnect), SI,LOG_ODBC,2 );
		}
         return 1;
   	 }
	rta = 0;

	memset(cadena,0,sizeof(cadena));

	if( pNroConnect == ODBC_SERVIDOR )
		sprintf(cadena,"Conex.Al.Serv.:     ");
	else
		sprintf(cadena,"Conex.Local.:     ");

	wxStrncpy(&cadena[16],oODBCConect->getDb(pNroConnect)->dbInf.dbmsName,25);

	oODBCConect->getDb(pNroConnect)->dbInf.dbmsName;
	switch( (int) oODBCConect->getDb(pNroConnect)->Dbms()){
		//enum wxDBMS {
		case dbmsUNIDENTIFIED:break;

		case dbmsORACLE:
			if(GetConfIniclienteDbServer(pNroConnect) == 4/* ORACLE*/)
				rta =1;
			break;

		case dbmsSYBASE_ASA:break;
		case dbmsSYBASE_ASE:break;

		case dbmsMS_SQL_SERVER:
			if(GetConfIniclienteDbServer(pNroConnect) == 3 /*SQLSERVER*/)
				rta =1;
			break;

		case dbmsMY_SQL:
			if(GetConfIniclienteDbServer(pNroConnect) == 2 /* MYSQL */)
				rta =1;
			break;

		case dbmsPOSTGRES:break;
		case dbmsACCESS:break;
		case dbmsDBASE:break;
		case dbmsINFORMIX:break;
		case dbmsVIRTUOSO:break;
		case dbmsDB2:break;
		case dbmsINTERBASE:break;
		case dbmsPERVASIVE_SQL:break;
		case dbmsXBASE_SEQUITER:break;
		default: break;

	}
		if( rta == 0 ) { 
			if( pNroConnect == ODBC_LOCAL  ){//SIN DICCIONARIO PROCESADO, por lo tanto el mensaje debe ser otro
				//MIGRAR A LINUX....
				#ifdef INVEL_W
					MessageBox(NULL,  cadena,"Error de Configuracion", MB_OK | MB_ICONSTOP);
				#endif
			}else{				
				MENSAJE_SIN_PAUSA( cadena );
				GRABAR_LOG_BD( cadena, SI,LOG_ODBC,3 );
			}
			return 1;
		}
	
        return 0; // si todo ok
    }
    /****************************************************************/
    void endOdbc( int pNroConnect )
    {
        oODBCConect->Disconect( pNroConnect );
    }
	/****************************************************************/
    void VERIFICAR_CONEXION( )
    {
#ifndef INVEL_L
		bool conexion = true;
		conexion = !oODBCConect->IsConect(ODBC_LOCAL);

		if( !conexion ) {
			MENSAJE_TECLA( "No hay Conexion con MySQL Server ( Presione una tecla )"  );
			GRABAR_LOG_SISTEMA( "No hay Conexion con MySQL Server ( Presione una tecla )",LOG_ODBC,1 );
			ABORT( "Error en MySQL Server. LLame al Supervisor" );
		}
#endif
	}
	/****************************************************************/
    int VERIFICAR_ESTADO_TABLA( char *sentencia, int repair)         
		/*
		repair = Indica si hay que intentar reparar la tabla
		0= nO  1= si/INSERT 2= si/UPDATE 3= si/DELETE 
		*/
	{
		int db_status = 0, db_nativeError = 0, rta = 0, continuar = SI;
		int fin = 0, i = 0, loguear_lista = SI;
		char mens[520], msj_MySQL[530], nom_tabla_actual[40], NomTablaGrabEventoOri[40];
		int nro_tabla_actual = 0, tipo_tabla_actual = 0, tablas_grab_evento = 0;

		//El tamaño de msj_MySQL no debe ser menor a DB_MAX_ERROR_MSG_LEN  = 512;
		memset( mens, 0, sizeof( mens ) );
		memset( msj_MySQL, 0, sizeof( msj_MySQL ) );
		memset( nom_tabla_actual, 0, sizeof( nom_tabla_actual ) );
		memset( NomTablaGrabEventoOri, 0, sizeof( NomTablaGrabEventoOri ) );

		if( !open_nro_tabla ) {
			if( oTablaActual != NULL ) {
				nro_tabla_actual = oTablaActual->getNroTable();
				tipo_tabla_actual =  oTablaActual->getNroTipo();
			}
		} else {
			nro_tabla_actual = open_nro_tabla;
			tipo_tabla_actual = open_tipo_tabla ;
		}

		db_status = oODBCConect->getDb(ODBC_LOCAL)->DB_STATUS;
		db_nativeError = oODBCConect->getDb(ODBC_LOCAL)->nativeError;
		
		switch( db_status )	{
			//Rtas que no necesitan ser logueadas
			case  DB_ERR_INVALID_CURSOR_STATE:      
			case  DB_ERR_FUNCTION_SEQUENCE_ERROR:      
				loguear_lista = NO;
				break;
			case  DB_ERR_GENERAL_ERROR:
				if( db_nativeError == ER_SPECIFIC_ACCESS_DENIED_ERROR 
						|| db_nativeError == 0 )
					loguear_lista = NO;
				break;
			default:
				_snprintf(mens, sizeof(mens)-1,	"Status:%i ErrorNativo:%i",	db_status, db_nativeError );
				//GRABAR_LOG_BD( mens, SI, LOG_ERRORES, 1 );
				break;
		}
		
		if( !verificar_tablas && db_status && loguear_lista && DICCIO_CARGADO == SI) {
			//Se crea verificar_tablas para definir zona donde en una sincronizacion da errores (Tarea 4980).
			GRABAR_LOG_SISTEMA( "Zona sin verificacion de tablas, verificar archivo base_dato.log", LOG_ERRORES, 1 );
		}

		//  --------  Loguea toda la lista de mensajes de error de mysql que pueden repetirse
		for( i = db_msj_MySQL; i < DB_MAX_ERROR_HISTORY && db_status && loguear_lista && !fin; i++ ) {
			memset( msj_MySQL, 0, sizeof( msj_MySQL ) );
			//Logueamos los mensajes desde el ultimo que quedo logueado 
			_snprintf(msj_MySQL, sizeof(msj_MySQL)-1,	"%s",
				oODBCConect->getDb(ODBC_LOCAL)->errorList[i]);
			if( strcmp( msj_MySQL, "" ) != 0 && DICCIO_CARGADO == SI) {
				//Muestro los msj seteados por mysql
				GRABAR_LOG_BD( msj_MySQL, SI, LOG_ERRORES, 1 );
				db_msj_MySQL ++;
			} else {
				fin = 1;
			}
			if( db_msj_MySQL == DB_MAX_ERROR_HISTORY ) {
				//Inicializo posicion del vector errorList para visualizar los mensaje de rta msyql
				db_msj_MySQL = 0;
			}
		}

		if( db_status 
			&& db_status == db_status_ant 
			&& db_nativeError == db_nativeError_ant
			&& nro_tabla_actual_ant == nro_tabla_actual) {

			// Quedan seteados los valores hasta el proximo seteo de error y a fin de no mostrar
			// varias veces el mismo mensaje con nombres de tablas incorrectas.
			// Lo correcto seria cerear la variable DB_STATUS de la libreria.
			db_status = 0;
		} else {
			db_status_ant = db_status;
			db_nativeError_ant = db_nativeError;
			nro_tabla_actual_ant = nro_tabla_actual;
		}

		if( !verificar_tablas ) {
			//No se valSe crea verificar_tablas para definir zona donde en una sincronizacion da errores (Tarea 4980).
			db_status = 0;
		}

		switch( db_status )
		{
			//case  DB_FAILURE:
			//case  DB_SUCCESS:
			case  DB_ERR_NOT_IN_USE:                                 
			case  DB_ERR_GENERAL_WARNING:                            
			case  DB_ERR_DISCONNECT_ERROR:                           
			case  DB_ERR_DATA_TRUNCATED:                             
			case  DB_ERR_PRIV_NOT_REVOKED:                           
			case  DB_ERR_INVALID_CONN_STR_ATTR:                      
			case  DB_ERR_ERROR_IN_ROW:                               
			case  DB_ERR_OPTION_VALUE_CHANGED:                       
			case  DB_ERR_NO_ROWS_UPD_OR_DEL:                         
			case  DB_ERR_MULTI_ROWS_UPD_OR_DEL:                      
			case  DB_ERR_WRONG_NO_OF_PARAMS:                         
			case  DB_ERR_DATA_TYPE_ATTR_VIOL:                        
			case  DB_ERR_UNABLE_TO_CONNECT:                          
			case  DB_ERR_CONNECTION_IN_USE:                          
			case  DB_ERR_CONNECTION_NOT_OPEN:                        
			case  DB_ERR_REJECTED_CONNECTION:                        
			case  DB_ERR_CONN_FAIL_IN_TRANS:
			case  DB_ERR_INSERT_VALUE_LIST_MISMATCH:                 
			case  DB_ERR_DERIVED_TABLE_MISMATCH:                     
			case  DB_ERR_STRING_RIGHT_TRUNC:                         
			case  DB_ERR_NUMERIC_VALUE_OUT_OF_RNG:                   
			case  DB_ERR_ERROR_IN_ASSIGNMENT:                        
			case  DB_ERR_DATETIME_FLD_OVERFLOW:                      
			case  DB_ERR_DIVIDE_BY_ZERO:                             
			case  DB_ERR_STR_DATA_LENGTH_MISMATCH:  
			case  DB_ERR_INTEGRITY_CONSTRAINT_VIOL:
			case  DB_ERR_INVALID_TRANS_STATE:                        
			case  DB_ERR_INVALID_AUTH_SPEC:                          
			case  DB_ERR_INVALID_CURSOR_NAME:                        
			case  DB_ERR_SYNTAX_ERROR_OR_ACCESS_VIOL:                
			case  DB_ERR_DUPLICATE_CURSOR_NAME:                      
			case  DB_ERR_SERIALIZATION_FAILURE:                      
			case  DB_ERR_SYNTAX_ERROR_OR_ACCESS_VIOL2:               
			case  DB_ERR_OPERATION_ABORTED:                          
			case  DB_ERR_UNSUPPORTED_FUNCTION:                       
			case  DB_ERR_NO_DATA_SOURCE:                             
			case  DB_ERR_DRIVER_LOAD_ERROR:                          
			case  DB_ERR_SQLALLOCENV_FAILED:                         
			case  DB_ERR_SQLALLOCCONNECT_FAILED:                     
			case  DB_ERR_SQLSETCONNECTOPTION_FAILED:                 
			case  DB_ERR_NO_DATA_SOURCE_DLG_PROHIB:                  
			case  DB_ERR_DIALOG_FAILED:                              
			case  DB_ERR_UNABLE_TO_LOAD_TRANSLATION_DLL:             
			case  DB_ERR_DATA_SOURCE_NAME_TOO_LONG:                  
			case  DB_ERR_DRIVER_NAME_TOO_LONG:                       
			case  DB_ERR_DRIVER_KEYWORD_SYNTAX_ERROR:                
			case  DB_ERR_TRACE_FILE_ERROR:                           
			case  DB_ERR_TABLE_OR_VIEW_ALREADY_EXISTS:
			case  DB_ERR_TABLE_NOT_FOUND:
			case  DB_ERR_INDEX_ALREADY_EXISTS:                       
			case  DB_ERR_INDEX_NOT_FOUND:                            
			case  DB_ERR_COLUMN_ALREADY_EXISTS:
			case  DB_ERR_NO_DEFAULT_FOR_COLUMN:
			case  DB_ERR_MEMORY_ALLOCATION_FAILURE:                  
			case  DB_ERR_INVALID_COLUMN_NUMBER:                      
			case  DB_ERR_PROGRAM_TYPE_OUT_OF_RANGE:                  
			case  DB_ERR_SQL_DATA_TYPE_OUT_OF_RANGE:                 
			case  DB_ERR_OPERATION_CANCELLED:                        
			case  DB_ERR_INVALID_ARGUMENT_VALUE:                     
			case  DB_ERR_OPERATION_INVALID_AT_THIS_TIME:             
			case  DB_ERR_INVALID_TRANS_OPERATION_CODE:               
			case  DB_ERR_NO_CURSOR_NAME_AVAIL:                       
			case  DB_ERR_INVALID_STR_OR_BUF_LEN:                     
			case  DB_ERR_DESCRIPTOR_TYPE_OUT_OF_RANGE:               
			case  DB_ERR_OPTION_TYPE_OUT_OF_RANGE:                   
			case  DB_ERR_INVALID_PARAM_NO:                           
			case  DB_ERR_INVALID_SCALE_VALUE:                        
			case  DB_ERR_FUNCTION_TYPE_OUT_OF_RANGE:                 
			case  DB_ERR_INF_TYPE_OUT_OF_RANGE:                      
			case  DB_ERR_COLUMN_TYPE_OUT_OF_RANGE:                   
			case  DB_ERR_SCOPE_TYPE_OUT_OF_RANGE:                    
			case  DB_ERR_NULLABLE_TYPE_OUT_OF_RANGE:                 
			case  DB_ERR_UNIQUENESS_OPTION_TYPE_OUT_OF_RANGE:        
			case  DB_ERR_ACCURACY_OPTION_TYPE_OUT_OF_RANGE:          
			case  DB_ERR_DIRECTION_OPTION_OUT_OF_RANGE:              
			case  DB_ERR_INVALID_PRECISION_VALUE:                    
			case  DB_ERR_INVALID_PARAM_TYPE:                         
			case  DB_ERR_FETCH_TYPE_OUT_OF_RANGE:                    
			case  DB_ERR_ROW_VALUE_OUT_OF_RANGE:                     
			case  DB_ERR_CONCURRENCY_OPTION_OUT_OF_RANGE:            
			case  DB_ERR_INVALID_CURSOR_POSITION:                    
			case  DB_ERR_INVALID_DRIVER_COMPLETION:                  
			case  DB_ERR_INVALID_BOOKMARK_VALUE:                     
			case  DB_ERR_DRIVER_NOT_CAPABLE:                         
			case  DB_ERR_TIMEOUT_EXPIRED:	
				continuar = SI;
				break;
			case  DB_ERR_INVALID_CURSOR_STATE://Fetch without a SELECT. El SELECT (consulta) no devuelve nada.
			case  DB_ERR_FUNCTION_SEQUENCE_ERROR: //Se ignora este error.
				db_status  = 0;
				break;
			case  DB_ERR_COMM_LINK_FAILURE:
				MENSAJE_TECLA( "No hay Conexion con MySQL Server ( Presione una tecla )"  );
				continuar = NO;
				break;
			case  DB_ERR_COLUMN_NOT_FOUND:
			case  DB_ERR_GENERAL_ERROR:
				switch( db_nativeError )
				{
					case  ERROR_NATIVO_126:                    
					case  ERROR_NATIVO_127: 
					case  ERROR_NATIVO_132:
					case  ERROR_NATIVO_134:                     
					case  ERROR_NATIVO_135:                         
					case  ERROR_NATIVO_136:
					case  ERROR_NATIVO_141:                     
					case  ERROR_NATIVO_144:                         
					case  ERROR_NATIVO_145:
					case  ERROR_NATIVO_1034:
					case  ERROR_NATIVO_1035:
					case  ERROR_NATIVO_1062:
					case  ERROR_NATIVO_1194:
					case  ERROR_NATIVO_1459:
						if( RAM_EMITIENDO_Z == 1 )
							continuar = SI;
						else
							continuar = NO;
						break;
					case  ERROR_NATIVO_130:
					case  ERROR_NATIVO_1036:
					case  ERROR_NATIVO_1054:
					case  ERROR_NATIVO_1195:
						continuar = NO;
						repair = -1;
						break;
					case  0://Un error importante no viene con db_nativeError = 0. Esto se da porque arrastra un error anterior ya que no pude cerear DB_STATUS = 0;
					case  ER_SPECIFIC_ACCESS_DENIED_ERROR://Acceso negado. Usted necesita el privilegio %s para esta operaciï¿½n (Ej. de privilegio RELOAD)
						db_status  = 0;
						break;
					default:
						continuar = SI;
						break;
				}
				break;
        }
        if( db_status ) {
			//ojo con LInux que no toma %s a las funciones que devuelven wxString como es oTablaActual->GetTableName().c_str()
#ifdef __WXDEBUG__
			{char mensa[100];
			sprintf(mensa,"Error en tabla db_status %i CONTINUAR %i \n",db_status, continuar);
			wxLogDebug( mensa);
			}
#endif
			GET_TABLE_NAME( nro_tabla_actual, tipo_tabla_actual, nom_tabla_actual, sizeof(nom_tabla_actual) );

			if( sentencia != NULL ) {
				memset( mens, 0, sizeof( mens ) );
				if( db_status != DB_ERR_TABLE_NOT_FOUND) {
					_snprintf(mens, sizeof(mens)-1,
							"Error en sentencia %s Tabla:%s Nro:%i Tipo:%i",
							sentencia,
							nom_tabla_actual,
							nro_tabla_actual,
							tipo_tabla_actual);
				} else {
					_snprintf(mens, sizeof(mens)-1,
							"Error en sentencia %s Tabla:%s Nro:%i Tipo:%i",
							sentencia,
							"Descon" ,
							nro_tabla_actual,
							tipo_tabla_actual);
				}
				GRABAR_LOG_BD( mens, SI, LOG_ERRORES, 1 );
			}

			GRABAR_LOG_SISTEMA( "Se genera informaciÃ³n de MySQL, verificar archivo base_dato.log",
					LOG_ERRORES, 1 );
			if( continuar == NO ) {
				RETCODE retcode = 0;
				memset(mens,0, sizeof(mens));

				/*----------------------------------------------------------------------------------*/
				//Si las tablas a verificar son tablas de grabacion de eventos
				//vamos a enviar el cmd de reparacion siempre que se encuentra en un estado
				//en el cual no debe continuar.
				//Vamos a abortar cuando intente realizar un Insert, Update y Delete
				//y apesar de enviar el cmd de reparacion no se recupere la tabla.
				tablas_grab_evento = ( nro_tabla_actual == T_EVENTOS || nro_tabla_actual == T_EV_CONT ||
						               nro_tabla_actual == T_PAGOS || nro_tabla_actual == T_EV_SUPER ||
						               nro_tabla_actual == T_EV_CIERRE || nro_tabla_actual == T_VARIOS ||
						               nro_tabla_actual == T_EV_DMEDIOS || nro_tabla_actual == T_EV_DESPRO ||
						               nro_tabla_actual == T_EV_FACTU || nro_tabla_actual == T_EV_B_PROMO ||
						               nro_tabla_actual == T_CIERRE_LOTE || nro_tabla_actual == T_EV_COD_BARRA );

				if( tablas_grab_evento
						&& db_nativeError != ERROR_NATIVO_1195
						&& repair == 0 ) {
					repair = 4;
					if( tipo_tabla_actual != TT_ORIG ) {
						GET_TABLE_NAME( nro_tabla_actual,
								TT_ORIG, NomTablaGrabEventoOri,
								sizeof(NomTablaGrabEventoOri) );
						if( strcmp( NomTablaGrabEventoOri, "" ) != 0 ) {
							_snprintf(mens, sizeof(mens)-1,"REPAIR TABLE %s",NomTablaGrabEventoOri);
							if( !DIRECT_DB_QUERY(mens) ) {
								glog("TABLA ORIGINAL REPARADA",LOG_ERRORES,1);
							} else {
								repair = -1;
							}
							memset(mens,0, sizeof(mens));
						}
					}
				}

				/*----------------------------------------------------------------------------------*/

				_snprintf(mens, sizeof(mens)-1,"REPAIR TABLE %s",
						nom_tabla_actual );
				if( db_nativeError != ERROR_NATIVO_1195
						&& repair > 0
						&& !DIRECT_DB_QUERY(mens)) {
					glog("TABLA REPARADA",LOG_ERRORES,1);
				}
				switch( repair ) {
					case 1:
						retcode = oTablaActual->Insert();
						break;
					case 2:
						retcode = oTablaActual->Update();
						break;
					case 3:
						retcode = oTablaActual->Delete();
						break;
					case 4:
						retcode = DB_SUCCESS;
						break;
				}
				if( repair > 0 && retcode == DB_SUCCESS )
					return(0);

				if( db_nativeError ==  ERROR_NATIVO_130
						&& continuar == NO 
						&& repair == -1  
						&& nro_tabla_actual == T_EV_CONT ) {
					ABORT("NO SE PUEDE CONTINUAR!!!.   Error Grabando ITEM en EV_CONT.      Llame al Supervisor");
				} else {
					ABORT( "Error en MySQL Server. LLame al Supervisor" );
				}

			}
        }

		return( db_status );
	}


    /****************************************************************/
    int OPEN_TABLE( int pTabla, int pTipo, char *pEstruct, long pSizeof )
    {
        int rta = 0, cargar = 1;
		char log[80];
		char mNombTablaOrig[30];

//		ERROR_SISTEMA_ERRNO();
		memset( mNombTablaOrig, 0, sizeof(mNombTablaOrig) );
		memset( log, 0, sizeof(log) );

		//Si falla la apertura de la tabla en queda seteado la tabla anterior por esa razon
		//seteamos variable por si falla la misma tengamos la tabla que fallo y no la anterior.
		open_nro_tabla = pTabla;
		open_tipo_tabla = pTipo;

		if( pTipo != TT_ORIG ) {
            char mNombSubTabla[34], mSubTipo[15];
			//Se obtiene el nombre de la tabla original
			if( DIRECT_DB_QUERY(
					"select distinct tablename from pos_dbmap where tablenumber = '%d' limit 1",
					pTabla ) > 0 ) {
				GET_DATO( 1, C_CHAR, mNombTablaOrig, sizeof( mNombTablaOrig ) );
                strcpy( mNombSubTabla, mNombTablaOrig );
                //oTablas->getStrTipo( pTipo, mSubTipo, sizeof( mSubTipo) );
				DbTables::getStrTipo( pTipo, mSubTipo, sizeof( mSubTipo) );
				
                strcat( mNombSubTabla, mSubTipo );
				//where 1=0: sirve para que solo cree la estructura sin datos.
				//where 1=1: sirve para que cree la estructura con datos.
                //rta = DIRECT_DB_QUERY( "create table if not exists %s select * from %s where 1=0", mNombSubTabla, mNombTablaOrig );
                //Usar la de abajo el dia que no se dependa mas de mySQL 3.x -- Se utiliza sentencia
				//porque se migra a mySQL 5.x. Sentecia anterior solo funciona cuando se depende de mySQL 3.x
                rta = DIRECT_DB_QUERY( "create table if not exists %s like %s", mNombSubTabla, mNombTablaOrig );
				//updateStructTable(mNombTablaOrig,0);
				cargar = 0;
			} else {
				rta = -1;//No encuentra tabla en la pos_dbmap
			}
		}

		if( rta != -1 ) {
			rta = oTablas->setTabla(oODBCConect->getDb(ODBC_LOCAL), pTabla, pTipo, pEstruct, pSizeof, NULL);
		}

	    if( rta ) {
			//Se hace esto para consultar tabla original y cargar el estado de la tabla para posterior verificacion
			if( cargar && DIRECT_DB_QUERY( 
					"select distinct tablename from pos_dbmap where tablenumber = '%d' limit 1", 
					pTabla ) > 0 ) {
				
                GET_DATO( 1, C_CHAR, mNombTablaOrig, sizeof( mNombTablaOrig ) );
				DIRECT_DB_QUERY( "select * from %s", mNombTablaOrig );
			}
			//VERIFICAR_CONEXION( );
            //GRABAR_LOG_BD( oTablas->getError(), SI ,LOG_ODBC,2);
			_snprintf( log, sizeof(log),
				"Error al crear TABLA %s. Leer Log para más información", mNombTablaOrig );
			log[sizeof( log ) - 1] = '\0';
			MENSAJE( log );

			memset( log, 0, sizeof(log) );
			_snprintf( log, sizeof(log),
				"Error al crear TABLA %s TIPO: %i TAMAÑO: %i", mNombTablaOrig, pTipo, pSizeof );
			log[sizeof( log ) - 1] = '\0';
			glog(log,LOG_INCONDICIONAL,1);

		}else{
			open_nro_tabla = 0;
			open_tipo_tabla = 0;
			SELECT_TABLE( pTabla, pTipo );
		}
//		ERROR_SISTEMA_ERRNO();
        return rta;
    }
    /****************************************************************/
    int OPEN_TABLE_TMP( int pTabla, char *pNombre, char *pEstruct )
    {
        int rta;
		char log[80];

		memset(log, 0, sizeof(log));
        rta = oTablas->setTabla(oODBCConect->getDb(ODBC_LOCAL), pTabla, 0, pEstruct, 0, pNombre);
        if( rta ) {
			VERIFICAR_ESTADO_TABLA( "OPEN_TABLE_TMP", 0);
            GRABAR_LOG_BD( oTablas->getError(), SI ,LOG_ODBC,1);
			_snprintf( log, sizeof(log),
				"ERROR AL CREAR LA TABLA NRO %i, LEER EL LOG PARA MAS INFORMACION", pTabla  );
			log[sizeof( log ) - 1] = '\0';
			MENSAJE( log );
        }

        return rta;
    }
	/****************************************************************/
	int SET_EMPTY_TABLE(int pTabla, int pTipo)
	{
		char mTableName[40];
		GET_TABLE_NAME(pTabla,pTipo,mTableName,sizeof(mTableName));
		DIRECT_DB_QUERY("truncate table %s",mTableName);
		return 1;
	}
    /****************************************************************/
    int BACKUP_TABLE( int pTabla )
    {
		char mTableName[40];
		char mTableNameBack[40];
		GET_TABLE_NAME(pTabla, TT_ORIG, mTableName, sizeof(mTableName) );
		//GET_TABLE_NAME(pTabla, TT_BAK, mTableNameBack, sizeof(mTableNameBack));
		GET_TABLE_NAME(pTabla, TT_BAK_ULT, mTableNameBack, sizeof(mTableNameBack));
		DIRECT_DB_QUERY("drop table if exists %s", mTableNameBack );
		//La sentencia "create table if not exists %s like %s" no funciona en mysql 3.X. 
		//y en mysql 5 solo crea la tabla pero no el contenido.
		//En mysql 5 si no existe la tabla, la sentencia "create table if not exists %s  select * from %s"
		//da error (Si funciona para mysql 3.X) PARA ALGUNAS TABLAS. Ejemplo: tpvconfig_bak. 
		//Para evitar el error se agrea antes de la sentencia mencionada la sentencia 
		//"create table if not exists %s like %s" para que cree previamente la tabla si no existe.
		DIRECT_DB_QUERY("create table if not exists %s like %s", mTableNameBack, mTableName );
		return DIRECT_DB_QUERY("create table if not exists %s select * from %s", mTableNameBack, mTableName );
    }
    /****************************************************************/
    int BACKUP_TABLE_TYPE( int pTabla, int pTipo )
    {
		char mTableName[40];
		char mTableNameBack[40];
		GET_TABLE_NAME(pTabla, pTipo, mTableName, sizeof( mTableName ) );
		strncpy( mTableNameBack, mTableName, sizeof( mTableNameBack ) );
        strncat( mTableNameBack, "_bak", sizeof( mTableNameBack ) - strlen( mTableNameBack ) -1 );
		DIRECT_DB_QUERY("drop table if exists %s", mTableNameBack );
		//La sentencia "create table if not exists %s like %s" no funciona en mysql 3.X. 
		//y en mysql 5 solo crea la tabla pero no el contenido.
		//En mysql 5 si no existe la tabla, la sentencia "create table if not exists %s  select * from %s"
		//da error (Si funciona para mysql 3.X) PARA ALGUNAS TABLAS. Ejemplo: tpvconfig_bak. 
		//Para evitar el error se agrea antes de la sentencia mencionada la sentencia 
		//"create table if not exists %s like %s" para que cree previamente la tabla si no existe.
		DIRECT_DB_QUERY("create table if not exists %s like %s", mTableNameBack, mTableName );
		return DIRECT_DB_QUERY("create table if not exists %s  select * from %s", mTableNameBack, mTableName );
    }
    /****************************************************************/
    int RESTORE_TABLE_TYPE( int pTabla, int pTipo )
    {
		char mTableName[40];
		char mTableNameBack[40];
		GET_TABLE_NAME(pTabla, pTipo, mTableName, sizeof( mTableName ) );
		strncpy( mTableNameBack, mTableName, sizeof( mTableNameBack ) );
        strncat( mTableNameBack, "_bak", sizeof( mTableNameBack ) - strlen( mTableNameBack ) -1 );
        if( DIRECT_DB_QUERY("select * from %s where 1=0", mTableNameBack ) == 0) {
		    SET_EMPTY_TABLE( pTabla, pTipo );
		    DIRECT_DB_QUERY("insert into %s select * from %s", mTableName, mTableNameBack );
            DIRECT_DB_QUERY("drop table if exists %s", mTableNameBack );
            return 0;
        } else {
            return -1;
        }
    }
    /****************************************************************/
    int TRANSFER_TABLE( int pTabla, int pTipoS, int pTipoD )
    {
		char mTableNameS[40];
		char mTableNameD[40];
        assert( pTipoS != pTipoD );
		GET_TABLE_NAME( pTabla, pTipoS, mTableNameS, sizeof(mTableNameS) );
		GET_TABLE_NAME( pTabla, pTipoD, mTableNameD, sizeof(mTableNameD));
		return DIRECT_DB_QUERY("insert into %s select * from %s", mTableNameD, mTableNameS );
    }
    /****************************************************************/
    int GET_TABLE_NAME( int pTabla, int pTipo, char *pName, int pSizeMaxName )
    {
        int rta = 0;
        char mNombTablaOrig[30], mNombSubTabla[34], mSubTipo[15];

        {char mensa[80];
        memset(mensa, 0, sizeof(mensa));
        _snprintf(mensa, sizeof(mensa)-1,"select distinct tablename from pos_dbmap where tablenumber = %d limit 1", pTabla);
          GRABAR_LOG_BD(mensa,NO,LOG_ODBC,5);}

        if( DIRECT_DB_QUERY( "select distinct tablename from pos_dbmap where tablenumber = '%d' limit 1",
				pTabla ) > 0 ) {

            GET_DATO( 1, C_CHAR, mNombTablaOrig, sizeof( mNombTablaOrig ) );
            strncpy( mNombSubTabla, mNombTablaOrig, sizeof(mNombTablaOrig) );
			DbTables::getStrTipo( pTipo, mSubTipo, sizeof( mSubTipo ) );
            strncat( mNombSubTabla, mSubTipo, sizeof(mNombSubTabla) );
            strncpy( pName, mNombSubTabla, pSizeMaxName );
            //pName[pSizeMaxName - 1] = NULL;
            pName[pSizeMaxName - 1] = '\0';
            rta = 1;
        }
        return rta;
    }
    /****************************************************************/
    int GET_TABLE_NUMBER( char *pName )
    {
        int rta = -1;
        if( DIRECT_DB_QUERY( "select distinct tablenumber from pos_dbmap where tablename = '%s' limit 1",
				pName ) > 0 ) {
            GET_DATO( 1, C_INT, (char *)&rta, sizeof( rta ) );
        }
        return rta;
    }
    /****************************************************************/
    void CLOSE_TABLE( int pTabla, int pTipo )
    {
        char mNombreTabla[28], mTipoStr[15];

        /*Obtiene el nombre de la tabla a partir del numero, para liberar la misma*/
        if( ( pTabla != 0 )
				&& ( DIRECT_DB_QUERY( "select tablename from pos_dbmap where tablenumber = %i limit 1",
                pTabla ) > 0 ) ) {
            GET_DATO( 1, C_CHAR, mNombreTabla, sizeof( mNombreTabla ) );
            /*Convierte a minusculas*/
            _strlwr(mNombreTabla);
            /*Anexa tipo al nombre*/
            //oTablas->getStrTipo( pTipo, mTipoStr, sizeof(mTipoStr) );
			DbTables::getStrTipo( pTipo, mTipoStr, sizeof(mTipoStr) );
            assert( 0 == pTipo || strlen( mTipoStr ) > 0 );
            strcat( mNombreTabla, mTipoStr );
            /*Libera la tabla*/
            DIRECT_DB_QUERY("flush table %s", mNombreTabla);
        }
        oTablas->closeTabla( pTabla, pTipo );
		oTablaActual = NULL;
    }
    /****************************************************************/
    int SELECT_TABLE( int pTabla, int pTipo )
    {

        if( pTabla > -1 && pTipo > -1 ) {
            oTablaActual = oTablas->getTabla( pTabla, pTipo );
            oTablaActual->setNroTable(pTabla);
            //SET_FROM("");
            //SET_LIMIT( NO, NO );
        }
        return( 0 );
    }
    /****************************************************************/
    int SELECTED2( )
    {
        return( (oTablaActual) ? oTablaActual->getNroTable() : -1 );
    }
    /****************************************************************/
    int SELECTED_TIPO( )
    {
        return( (oTablaActual) ? oTablaActual->getNroTipo() : -1 );
    }
    /****************************************************************/
    int GO2( int pRegistro )
    {
        int rta;
        switch( pRegistro ) {
            case TOP:
                rta = oTablaActual->GetFirst();
                break;
            case BOTTOM:
                rta = oTablaActual->GetLast();
                break;
            default:
                break;
        }
        return 0;
    }
    /****************************************************************/
    int SET_ORDER2( char *pCampos )
    {
        oTablaActual->setOrder( pCampos );
        return 0;
    }
    /****************************************************************/
    int SET_WHERE( char *pCondicion, ... )
    {
        wxString    str;
        va_list argptr;
        va_start(argptr, pCondicion);
        str.PrintfV(pCondicion, argptr);
        oTablaActual->setWhere( (char *)str.c_str() );
        va_end(argptr);
        return 0;
    }

    /****************************************************************/
    char * GET_WHERE(  )
    {
        return oTablaActual->getWhere(  );
    }


    /****************************************************************/
    int SET_FROM( char *pTablas )
    {
        oTablaActual->setFrom( pTablas );
        return 0;
    }
    /****************************************************************/
    int SET_LIMIT( int pLimite, int pPaginado )
    {
        oTablaActual->setLimite(pLimite);
        oTablaActual->setPaginado((pPaginado==0) ? false : true);
        return 0;
    }
    /****************************************************************/
    int RUN_QUERY(int pDistinct)
    {
		int rta = 0;

		rta = oTablaActual->runQuery(pDistinct);
		if( rta )
			//Si no existe registro devuelve 0
			VERIFICAR_ESTADO_TABLA( "RUN_QUERY", 0 );

		return rta;
    }
    /****************************************************************/
    int SKIP2( int pCant )
    {
        oTablaActual->moveReg(pCant);
		VERIFICAR_CONEXION( );
        return 0;
    }
    int SKIP_PAGE( int pCant )
    {

		int rta = 0;

		rta = oTablaActual->movePage(pCant);
		VERIFICAR_CONEXION( );

		return rta;

    }
    /****************************************************************/
	int GET_PAGINA_REG()
    {
        return (int) oTablaActual->getPagina();
    }
    /****************************************************************/
    long GET_POSITION_REG()
    {
		long pos;
		pos = ((oTablaActual->getPaginado()) ? oTablaActual->getLimite()*(GET_PAGINA_REG()-1) : 0)
													+ ( long )oTablaActual->GetRowNum();
		return pos;

    }
    /****************************************************************/
    int GET_DIRECT_REG( unsigned long pPosicion, int pPagina )
    {
        long posact,page; 
		int rta = 0;
        /*if(oTablaActual->getPagina() != pPagina)
            oTablaActual->setPagina(pPagina);
        posact = ( long )oTablaActual->GetRowNum();
        if( 1 == pPosicion ) {
            GO2( TOP );
        }
        else {
            SKIP2( pPosicion - posact );
        }*/
		if(oTablaActual->getPaginado()){
			page = (long)(pPosicion / oTablaActual->getLimite());
			if((pPosicion - page*oTablaActual->getLimite()) > 0)
				page++;
			if( page )
				rta = oTablaActual->setPagina((int)page);
			if( rta )
				VERIFICAR_ESTADO_TABLA( "GET_DIRECT_REG", 0 );
			GO2(TOP);
			posact = (oTablaActual->getLimite()*(page - 1) + 1);
		}else{
			posact = GET_POSITION_REG();
		}
		SKIP2( pPosicion - posact );
        return 0;
    }
	/****************************************************************/
	    int dbEOF()
    {
        return (int) oTablaActual->getEof();
    }
    /****************************************************************/
    int dbBOF()
    {
        return (int) oTablaActual->getBof();
    }
    /****************************************************************/
    int FOUND2()
    {
        /*si no es fin de archivo, retorna 1*/
        return (int) !oTablaActual->getEof();
    }
    /****************************************************************/
    int OPENED(int pTabla, int pTipo)
    {
        /* se fija si esta abierta (inicializada)*/
        return oTablas->opened(pTabla, pTipo);
    }
    /****************************************************************/
    /*
        pDistinct: condicion de conteo para los iguales
        pwhere   : condicion where del conteo
                   si pWhere = null entonces no modifica el where de la tabla
        Ej:
        codigo = 3
        RECCOUNT2("DISTINCT NOMBRE", "CODIGO = %i", codigo);
        cuenta todos los nombres distintos que tengan por codigo = 3
    */
    long RECCOUNT2( char *pDistinct, char *pWhere, ... )
    {
        long rta =0;

		wxString    str;
        va_list argptr;
        va_start(argptr, pWhere);
        if(pWhere){
            str.PrintfV(pWhere, argptr);
            oTablaActual->setWhere( (char *)str.c_str() );
        }
        va_end(argptr);

		rta = oTablaActual->getCount(pDistinct);
		if( !rta )
			VERIFICAR_ESTADO_TABLA( "RECCOUNT2", 0 );

        return rta;
    }
    /****************************************************************/
    int INSERT2(int pModifStatus){

        int salida = 0;
		if(pModifStatus){
            oTablaActual->getPtrStruct()[0] = 0;
        }
        RETCODE retcode = oTablaActual->Insert();
		
        switch(retcode)
        {
            case DB_SUCCESS:
                return(0);
            case DB_ERR_INTEGRITY_CONSTRAINT_VIOL:
				
				salida = VERIFICAR_ESTADO_TABLA( "INSERT2", 1);
				if( salida == 0) {
					glog("Insert VERIFICAR_ESTADO_TABLA=0",LOG_ERRORES,1);
					return( 0);
				}
                GRABAR_LOG_BD( "Clave duplicada en el insert", SI ,LOG_ODBC,1);
				{ char mens[256];//es el mismo tañano que el logdel sql de la wx
				memset(mens,0, sizeof(mens));
				_snprintf(mens, sizeof(mens)-1,"Clave duplicada  %s ",oTablaActual->GetTableName().c_str());
				GRABAR_LOG_BD(mens, SI ,LOG_ERRORES,1);
				}
                return(1);
            default:
				salida =VERIFICAR_ESTADO_TABLA( "INSERT2",1);
				if( salida == 0)
					return( 0);
				return(2);
        }
    }
    /***************************************************************/
    int UPDATE2( ){

        RETCODE retcode = oTablaActual->Update();
        switch(retcode)
        {
            case true:
                return(0);
            default:
				//GRABAR_LOG_BD( "Fallo el update por razon inesperada", SI,LOG_ODBC,1 );
				VERIFICAR_ESTADO_TABLA( "UPDATE2", 2 );
                return(1);
        }
    }
    /***************************************************************/
    int DELETE2( ){
        oTablaActual->SetCursor();
        RETCODE retcode = oTablaActual->Delete();
        switch(retcode)
        {
            case true:

				//oTablaActual->moveReg(-1);
				//if(oTablaActual->getBof())
				oTablaActual->moveReg(1);
				return(0);
            default:
				//GRABAR_LOG_BD( "Fallo el delete por razon inesperada", SI,LOG_ODBC,1 );
				VERIFICAR_ESTADO_TABLA( "DELETE2", 3 );
                return(1);
        }
    }
    /****************************************************************/

    /*
        Ejecuta una consulta sql directamente sobre la base, sin tener un objeto tabla
        asociada a esta
        Luego de ejecutar este tipo de consulta, hay que hacer un GET_DATO() especificando
        que columna se quiere obtener
    */
    int DIRECT_DB_QUERY(char *pQuery, ...)
    {
    	/*
    	 * IMPORTANTE!
    	 * Poner siempre el nombre de la tabla en minusculas ya que en linux es case sentive
    	 * ej: SELECT * FROM eventos
    	 *
    	 * */

        wxString    str;
        va_list argptr;
		//char log[150];

        va_start(argptr, pQuery);
        str.PrintfV(pQuery, argptr);

		if( !(oODBCConect->getDb(ODBC_LOCAL)->ExecSql((char *)str.c_str())) ) {
			//_snprintf( log, sizeof(log) - 1,
				//"ERROR AL REALIZAR CONSULTA %s", (char *)str.c_str() );
			//GRABAR_LOG_BD( log, SI ,LOG_ODBC,3);
			VERIFICAR_ESTADO_TABLA( "DIRECT_DB_QUERY" , 0 );
			return -1;
		}

        va_end(argptr);
        if( str.Contains("select") || str.Contains("SELECT") || str.Contains("show") || str.Contains("SHOW")) {
        	return (int)oODBCConect->getDb(ODBC_LOCAL)->GetNext(); //TODO: si la consulta no tiene SELECT, GetNext da error (leve) ya que de la consulta no se obtuvieron resultados
        }

        return 0;
    }
    /****************************************************************/
    int GET_DATO(int pColumna, int pType, char *pData, long pSizeof)
    {
        SDWORD      cb;
        oODBCConect->getDb(ODBC_LOCAL)->GetData(pColumna, pType, pData, pSizeof, &cb);
        return 0;
    }
    /****************************************************************/
    int DIRECT_DB_SKIP( int pCant ) {

        int iter, rta;

        if( pCant > 0 ) {
            for( iter=0; iter<pCant; iter++ ) {
                rta = (int)oODBCConect->getDb(ODBC_LOCAL)->GetNext();
            }
        }
        return rta;
    }
    /****************************************************************/
    int VERIFICAR_SCRIPT()
    {
        int rta;
        DbScript mDbScript;
        if(rta = mDbScript.VerifScripts(oODBCConect) != 0){
            //MENSAJE( "ERROR AL EJECUTAR DBSCRIPT, LEER LOG PARA MAS INFORMACION" );
            //GRABAR_LOG_BD( mDbScript.getError(), SI );
			;
        }
        /*Despues de verificar script, mapea la tabla posdbrouter*/
        if(oTablas->setDbRouter(oODBCConect->getDb(ODBC_LOCAL))){
            VERIFICAR_CONEXION();
			//MENSAJE( "No se pudo crear la tabla pos_dbmap" );
            //GRABAR_LOG_BD( oTablas->getError(), SI ,LOG_ODBC,1);
        }
        return 0;
    }
    /****************************************************************/
	int EJECUTAR_SCRIPT_TOUCH(void)
	{
		int rta;
		DbScript mDbScript;
		rta = mDbScript.RunScripts(oODBCConect,"../TOUCH/distribucion.sql",PATH_TOUCH);
		return rta;
	}
    /****************************************************************/
    int EJECUTAR_SCRIPT( char* pNombre, char* pPath )
    {
		int rta;
		DbScript mDbScript;
		rta = mDbScript.RunScripts(oODBCConect, pNombre, pPath );
		if( rta ) 
			VERIFICAR_CONEXION( );

		return rta;
    }

	/*****************************************************************/
	long SQL_ROW_SIZE(int pTabla)
	{
		wxString query;
		SDWORD cb;
		long mSizeof;
		int rta = 0;

		query.Printf("select sum(DATASIZE) from pos_dbmap where TABLENUMBER = %i", pTabla);
		rta = (oTablaActual->GetDb())->ExecSql(query);
		(oTablaActual->GetDb())->GetNext();
		(oTablaActual->GetDb())->GetData(1, SQL_C_LONG, &mSizeof, sizeof(mSizeof), &cb);
		
		if( !rta )
			VERIFICAR_ESTADO_TABLA( "SQL_ROW_SIZE" , 0);


		return mSizeof;
	}
	/*****************************************************************************/
	int ZAP2(  )
	{
		int rta = 0;
		/*GO2( TOP );
		while( FOUND2(  ) )
		{
			DELETE2(  );
			GO2( TOP );
		}*/
		rta = oTablaActual->DeleteWhere("");
		if( !rta )
			VERIFICAR_ESTADO_TABLA( "ZAP2" , 0);
      
		return rta;
		//return 0;
	}
    /*****************************************************************************/
    int DROP_TABLE(int pTabla, int pTipo ) {

        char mNombreTabla[28], mTipoStr[15];

        /*Obtiene el nombre de la tabla a partir del numero, para liberar la misma*/
        if( ( pTabla != 0 )
				&& ( DIRECT_DB_QUERY( "select tablename from pos_dbmap where tablenumber = %i limit 1",
                pTabla ) > 0 ) ) {
            GET_DATO( 1, C_CHAR, mNombreTabla, sizeof( mNombreTabla ) );
            /*Convierte a minusculas*/
            _strlwr(mNombreTabla);
            /*Anexa tipo al nombre*/
            //oTablas->getStrTipo( pTipo, mTipoStr, sizeof(mTipoStr) );
			DbTables::getStrTipo( pTipo, mTipoStr, sizeof(mTipoStr) );
            assert( 0 == pTipo || strlen( mTipoStr ) > 0 );
            strcat( mNombreTabla, mTipoStr );
            /*Libera la tabla*/
            DIRECT_DB_QUERY("flush table %s", mNombreTabla);
            DIRECT_DB_QUERY( "drop table if exists %s", mNombreTabla );
        }
        oTablas->closeTabla( pTabla, pTipo );
		oTablaActual = NULL;
        return 0;

    }

    /*****************************************************************************/
    int INSERT_SCRIPT_POS(int pCodComando, char* pParametro ) {
		int rta = 0;

		rta = DIRECT_DB_QUERY("INSERT INTO script_pos VALUES (NULL, 0, NULL, %i, 0, '%s', %i, %i, 0, 0, 0, 0)", pCodComando, pParametro, _GET_FECHA_DOS(), GET_HORA_DOS());

        return rta;

    }
#ifdef __cplusplus
}
#endif

