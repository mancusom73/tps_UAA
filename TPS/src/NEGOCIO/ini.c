#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h>
#include <ini.h>
#include <trim.h>
#include <path.h>
#include <str.h>
#pragma pack(8)
#define _INI_C
#undef _INI_C
char OBTENER_NIVEL_LOG( char *valor );
/***************************************************************************/
void CARGAR_CONF_INI()
/***************************************************************************/
{
    FILE *archivo;
    char buffer[367];
    char grupo[50];
    char variable[50];
    char valor[256];
    char *pos1,*pos2;
    int rta = 1;
    /* Setea los valores por defecto en la configuracion*/
	
    SET_VALORES_DEF();
    /*--------------- Abrimos el archivo de configuracion ---------------*/
    archivo = fopen( TPVIV_INI, "rt" );
    if( archivo ) {
        /*-------------- Leemos un renglon y los procesamos -----------*/
        while( fgets( buffer, 160, archivo ) && rta ) {
            LTRIM( buffer );                      //saca los espacios que pueda tener al principio de la cadena
            pos1 = strchr( buffer, '\n' );
            if( pos1 ) {
                *pos1 = '\0';
	            // En linux no se saca automaticamente el \r
	            if( '\r' == *(pos1 - 1) && pos1 > buffer ) {
	            	*--pos1 = '\0'; //retocede y borra.
	            }
            }
            _strupr( buffer );
            if( buffer[0] == '[' ) {
                pos1 = &buffer[1];
                pos2 = strchr( pos1, ']' );
                if( pos1 && pos2 ) {
                    *pos2 = '\0';
                    strcpy( grupo, pos1 );           //guarda el nombre del grupo
                    /*saca espacios*/
                    LTRIM( grupo );
                    TRIM( grupo );
                }
            }
            else if( buffer[0] != ';' && buffer[0] != '\0') {
                pos1 = buffer;
                pos2 = strchr( pos1, '=' );
                if( pos1 && pos2 ) {
                    strcpy( valor, &pos2[1] );       //guarda el valor de la variable
                    *pos2 = '\0';
                    strcpy( variable, pos1 );        //guarda el nombre de la variable
                    SETEAR_CONF_INI( grupo, variable, valor );
                }
            }
        }
        fclose( archivo );
    }
    else {
        //MENSAJE_STRING( S_NO_SE_ENCUENTRA_EL_ARCHIVO_tkt_caja );
    }
}
/***************************************************************************/
void SET_VALORES_DEF()
/***************************************************************************/
{
    strcpy( ConfIni.javaserverIp, "localhost" );
	strcpy(ConfIni.FtpIp,"127.0.0.1");
    ConfIni.clienteDb = BTRIEVE;
    ConfIni.odbc[0] = 0;
    ConfIni.password[0] = 0;
    ConfIni.user[0] = 0;
    strcpy( ConfIni.pathDatabase,"../DATABASE/DBTPVIV/" );
    ConfIni.clienteDbServer = SQLSERVER;
    ConfIni.odbcServer[0] = 0;
    ConfIni.passwordServer[0] = 0;
    ConfIni.userServer[0] = 0;
    ConfIni.touch_screen = 0;
    ConfIni.dirNovedades[0] = 0;
    ConfIni.logPantallas = 'N';
    ConfIni.logMotorPromo = 'N';
    ConfIni.logVentas = 'N';
    ConfIni.logCobros = 'N';
    ConfIni.logPagos = 'N';
    ConfIni.logFondoFijo = 'N';
    ConfIni.logRetiros = 'N';
    ConfIni.logCierre = 'N';
    ConfIni.logJavaServer = 'N';
    ConfIni.logVarios = 'N';
    ConfIni.logCambioMedio = 'N';
	ConfIni.logOdbc= 'N';
	ConfIni.logConServidor= 'N';
	ConfIni.logComunicaciones= 'N';
	ConfIni.logErrores= 'S';
	ConfIni.logRobot= 'S';
	ConfIni.logDebug= 'N';
    ConfIni.tiempo_pop_up = 5;
    ConfIni.sucursal = 1;
	ConfIni.logComprobantes = 'N';
	ConfIni.ActualizPantRedondeo = 'N';
	ConfIni.CorteArchGrandes = 0.0;
    ConfIni.reintentos_envio_cierre = 3;
	ConfIni.dirVideosPromos[0] = 0;
	ConfIni.dirVideosVentas[0] = 0;
	ConfIni.dirVideosSsaver[0] = 0;
	ConfIni.RnvDirectoMemoria = 0;
	ConfIni.panelesPosX=0;
	ConfIni.panelesPosY=0;
	// default 5 minutos
	ConfIni.timerScreenSaver = 300;
	strcpy( ConfIni.dirSkin,"..\..\TPVSIS\IMG\VISTA" );
}
/***************************************************************************/
void SETEAR_CONF_INI( char *grupo, char *variable, char *valor )
/***************************************************************************/
{
    /*saca espacios*/
    LTRIM( variable );
    TRIM( variable );
    LTRIM( valor );
    TRIM( valor );
    /*Valor en minuscula siempre*/
    _strlwr( valor );
    if( !strcmp( grupo, "PANTALLA" ) ) {
        if( !strcmp( variable, "TIEMPO_POP_UP" ) ){
            ConfIni.tiempo_pop_up = atoi(valor);
        }
		else if( !strcmp( variable, "ACTUALIZ_PANT_REDONDEO" ) ){
           ConfIni.ActualizPantRedondeo = (!strcmp( valor, "si")) ? 'S' : 'N';
        }
        else if( !strcmp( variable, "SKIN" ) ){
            strcpy(ConfIni.dirSkin,valor);
        }else if( !strcmp( variable, "PANELES_X" ) ){
            ConfIni.panelesPosX = atoi(valor);
	    }else if( !strcmp( variable, "PANELES_Y" ) ){
            ConfIni.panelesPosY= atoi(valor);
	    }
	}else if( !strcmp( grupo, "SISTEMA" ) ) {
        if( !strcmp( variable, "TOUCH_SCREEN" ) ){
            ConfIni.touch_screen = (!strcmp( valor, "si")) ? 1 : 0;
        }
        else if( !strcmp( variable, "DIR_NOVEDADES" ) ){
            strcpy(ConfIni.dirNovedades,valor);
        }
        else if( !strcmp( variable, "FTP_IP" ) ){
            strcpy(ConfIni.FtpIp,valor);
        }
        else if( !strcmp( variable, "SUCURSAL" ) ){
             ConfIni.sucursal = atoi(valor);
        }
        else if( !strcmp( variable, "REINTENTOS_ENVIO_CIERRE" ) ){
            ConfIni.reintentos_envio_cierre = atoi(valor);
        }
        else if( !strcmp( variable, "TIMER_SCREEN_SAVER" ) ){
            ConfIni.timerScreenSaver = atoi(valor);
        }else if( !strcmp( variable, "RNV_DIRECTO_MEMORIA" ) ){
			ConfIni.RnvDirectoMemoria =(!strcmp( valor, "si")) ? 1 : 0;
        }else if( !strcmp( variable, "ACELERACION_PANTALLAS" ) ){
			ConfIni.AceleracionPantallas =(!strcmp( valor, "si")) ? 1 : 0;
        }
        /*falta definir*/
    }else if( !strcmp( grupo, "PERIFERICO" ) ) {
        if( !strcmp( variable, "JAVASERVER" ) ){
           strcpy(ConfIni.javaserverIp,valor);
        }
    }else if( !strcmp( grupo, "DATABASE" ) ) {
        if( !strcmp( variable, "CLIENTE" ) ){
            if(!strcmp( valor, "mysql")){
                ConfIni.clienteDb = MYSQL;
            }else if(!strcmp( valor, "btrieve")){
                ConfIni.clienteDb = BTRIEVE;
            }
        }else if( !strcmp( variable, "ODBC" ) ){
           strcpy(ConfIni.odbc,valor);
        }else if( !strcmp( variable, "USUARIO" ) ){
           strcpy(ConfIni.user,valor);
        }else if( !strcmp( variable, "PASSWORD" ) ){
           strcpy(ConfIni.password,valor);
        } else if( !strcmp( variable, "CLIENTE_SERVER" ) ) {
            if(!strcmp( valor, "oracle")){
                ConfIni.clienteDbServer = ORACLE;
            }else if(!strcmp( valor, "sqlserver")){
                ConfIni.clienteDbServer = SQLSERVER;
            }
        }else if( !strcmp( variable, "ODBC_SERVER" ) ){
           strcpy(ConfIni.odbcServer,valor);
        }else if( !strcmp( variable, "USUARIO_SERVER" ) ){
           strcpy(ConfIni.userServer,valor);
        }else if( !strcmp( variable, "PASSWORD_SERVER" ) ){
           strcpy(ConfIni.passwordServer,valor);
        }else if( !strcmp( variable, "PATH_DATABASE" ) ){
           strcpy(ConfIni.pathDatabase,valor);
        }
    }else if( !strcmp( grupo, "LOG" ) ) {
        if( !strcmp( variable, "LOG_PANTALLAS" ) ){
            ConfIni.logPantallas = OBTENER_NIVEL_LOG( valor );
			//ConfIni.logPantallas = (!strcmp( valor, "si")) ? 'S' : 'N';
        }else if( !strcmp( variable, "LOG_MOTOR_PROMO" ) ){
           ConfIni.logMotorPromo =   OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_PAGOS" ) ){
           ConfIni.logPagos =   OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_VENTAS" ) ){
           ConfIni.logVentas =  OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_COBROS" ) ){
           ConfIni.logCobros =  OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_FONDO_FIJO" ) ){
           ConfIni.logFondoFijo= OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_RETIROS" ) ){
           ConfIni.logRetiros =  OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_CIERRE" ) ){
           ConfIni.logCierre =   OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_JAVA_SERVER" ) ){
           ConfIni.logJavaServer = OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_VARIOS" ) ){
           ConfIni.logVarios =  OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_CAMBIO_MEDIO" ) ){
           ConfIni.logCambioMedio = OBTENER_NIVEL_LOG( valor );

		}else if( !strcmp( variable, "LOG_ODBC" ) ){
           ConfIni.logOdbc = OBTENER_NIVEL_LOG( valor );
		   }else if( !strcmp( variable, "LOG_COM_SERVIDOR" ) ){
           ConfIni.logConServidor = OBTENER_NIVEL_LOG( valor );
		   }else if( !strcmp( variable, "LOG_COMUNICACIONES" ) ){
           ConfIni.logComunicaciones = OBTENER_NIVEL_LOG( valor );
		   }else if( !strcmp( variable, "LOG_ERRORES" ) ){
           ConfIni.logErrores = OBTENER_NIVEL_LOG( valor );
		   }else if( !strcmp( variable, "LOG_ROBOT" ) ){
           ConfIni.logRobot = OBTENER_NIVEL_LOG( valor );
		   }else if( !strcmp( variable, "LOG_DEBUG" ) ){
		   //Dnc  ticket 2164 - niveles de logs
		   //if( atoi(valor)>0 && atoi(valor) < 127 )
		   //ConfIni.logDebug = (char) atoi(valor);
		   //else
		   //ConfIni.logDebug = 0;

           ConfIni.logDebug = OBTENER_NIVEL_LOG( valor );
        }else if( !strcmp( variable, "LOG_IMP_COMPROBANTES" ) ){
           ConfIni.logComprobantes = OBTENER_NIVEL_LOG( valor );
        }else  if( !strcmp( variable, "LOG_CORTE_ARCH_GRANDES" ) ){
			ConfIni.CorteArchGrandes = ((strtod( valor, NULL) > 0 )?strtod( valor, NULL) :0);

        }
    }else if( !strcmp( grupo, "VIDEO" ) ) {
        if( !strcmp( variable, "DIR_REPRO" ) ){
            strcpy(ConfIni.dirRepro,valor);
        }
        else if( !strcmp( variable, "DIR_VIDEOS_PROMOS" ) ){
            strcpy(ConfIni.dirVideosPromos,valor);
        }
        else if( !strcmp( variable, "DIR_VIDEOS_VENTAS" ) ){
            strcpy(ConfIni.dirVideosVentas,valor);
        }
        else if( !strcmp( variable, "DIR_VIDEOS_SSAVER" ) ){
            strcpy(ConfIni.dirVideosSsaver,valor);
        }
        else if( !strcmp( variable, "ANCHO_PROMO" ) ){
            ConfIni.vAncho = atoi(valor);
        }
        else if( !strcmp( variable, "ALTO_PROMO" ) ){
            ConfIni.vAlto = atoi(valor);
        }
        else if( !strcmp( variable, "POS_X_PROMO" ) ){
            ConfIni.vPosx = atoi(valor);
        }
        else if( !strcmp( variable, "POS_Y_PROMO" ) ){
            ConfIni.vPosy = atoi(valor);
	    }
        else if( !strcmp( variable, "ANCHO_SAVER" ) ){
			ConfIni.vAnchoS = atoi(valor);
        }
        else if( !strcmp( variable, "ALTO_SAVER" ) ){
            ConfIni.vAltoS = atoi(valor);
        }
        else if( !strcmp( variable, "POS_X_SAVER" ) ){
            ConfIni.vPosxS = atoi(valor);
        }
        else if( !strcmp( variable, "POS_Y_SAVER" ) ){
            ConfIni.vPosyS = atoi(valor);
	    }
        else if( !strcmp( variable, "ANCHO_VENTA" ) ){
			ConfIni.vAnchoV = atoi(valor);
        }
        else if( !strcmp( variable, "ALTO_VENTA" ) ){
            ConfIni.vAltoV = atoi(valor);
        }
        else if( !strcmp( variable, "POS_X_VENTA" ) ){
            ConfIni.vPosxV = atoi(valor);
        }
        else if( !strcmp( variable, "POS_Y_VENTA" ) ){
            ConfIni.vPosyV = atoi(valor);
	    }
	}
}

char * GET_PATH_BINARIO(void){
		return(GetAppExePath());
}


int GET_POSICION_PANTALLAS_X(void){
return ConfIni.panelesPosX;
}
int GET_POSICION_PANTALLAS_Y(void){
return ConfIni.panelesPosY;
}
int SET_POSICION_PANTALLAS_X(int x ){
return ConfIni.panelesPosX = x;
}
int SET_POSICION_PANTALLAS_Y(int y){
return ConfIni.panelesPosY = y;
}

/***************************************************************************/
char OBTENER_NIVEL_LOG( char *valor )
/*
Función que carga el nivel de logs en la variable correspondiente
*/
/***************************************************************************/
{
  char nivel;
  //vemos si es si
  if(strcmp( valor, "si") == 0) //si
   nivel = '5' ;
  else
   if(strcmp( valor, "no") == 0) //no
      nivel = '0' ;
      else
         if( atoi(valor)>0 && atoi(valor)< 10 )
               nivel = atoi(valor) ;
          else
            nivel = '0' ;

 return nivel;
}
/***************************************************************************/
void SET_VALORES_CONFIG_TPS(struct _config_tps auxi)
/***************************************************************************/
{
	int h = 0;

	config_tps.activarTFpermanente = auxi.activarTFpermanente;
	config_tps.activarLecturaClienteBarra = auxi.activarLecturaClienteBarra;
	strncpy(config_tps.NombreCliente, auxi.NombreCliente,sizeof(config_tps.NombreCliente)) ;
	strncpy(config_tps.DniCliente, auxi.DniCliente,sizeof(config_tps.DniCliente)) ;
	strncpy(config_tps.Prefijo_scanner_cliente, auxi.Prefijo_scanner_cliente,sizeof(config_tps.Prefijo_scanner_cliente));
	config_tps.encabezadoTF= auxi.encabezadoTF;
	config_tps.versionxml = auxi.versionxml;
	config_tps.ControlarMediosDePago = auxi.ControlarMediosDePago;
	config_tps.AgruparMediosEnImpresion = auxi.AgruparMediosEnImpresion;
	config_tps.CantMediosPermitidos = auxi.CantMediosPermitidos;
	config_tps.InformarMediosExcedidos = auxi.InformarMediosExcedidos;
	config_tps.ClienteTarjetaEspecial = auxi.ClienteTarjetaEspecial;
	config_tps.Codigo_seguridad_cliente = auxi.Codigo_seguridad_cliente;
	config_tps.Perfil_cliente_especial = auxi.Perfil_cliente_especial;
	config_tps.ActivarCashPlus = auxi.ActivarCashPlus;
	config_tps.ValidacionCambioMediosCash = auxi.ValidacionCambioMediosCash;
	config_tps.MinimoaHabilitar = auxi.MinimoaHabilitar;
	config_tps.HabilitarCashPlusSolo = auxi.HabilitarCashPlusSolo;
	memcpy(config_tps.MontosValidos , auxi.MontosValidos, 10*sizeof(int));
	config_tps.CambioMedioAutomatico = auxi.CambioMedioAutomatico;
	config_tps.PedirAutorizacionCambioMedioAutomatico = auxi.PedirAutorizacionCambioMedioAutomatico;
	config_tps.ValidacionCambioMedioAutomatico = auxi.ValidacionCambioMedioAutomatico;
	config_tps.PedirAutorizacionEnCierreConCambioMedio = auxi.PedirAutorizacionEnCierreConCambioMedio;
	for( h = 0; h < 10; h++ ) {
		if( auxi.det_CambioMedio[h].MedioEntrante
			&& auxi.det_CambioMedio[h].MedioSaliente ) {
			config_tps.det_CambioMedio[h].MedioEntrante = auxi.det_CambioMedio[h].MedioEntrante;
			config_tps.det_CambioMedio[h].SubmedioEntrante = auxi.det_CambioMedio[h].SubmedioEntrante;
			config_tps.det_CambioMedio[h].MedioSaliente = auxi.det_CambioMedio[h].MedioSaliente;
			config_tps.det_CambioMedio[h].SubmedioSaliente = auxi.det_CambioMedio[h].SubmedioSaliente;
		}
	}
	config_tps.PadronesPorFTP = auxi.PadronesPorFTP;
	strncpy(config_tps.RutaFtp , auxi.RutaFtp,sizeof(config_tps.RutaFtp)) ;
	{
		char mensa[30];
		memset(mensa,0, sizeof(30));
		sprintf( mensa, "2Padron? %i",config_tps.PadronesPorFTP);
		glog(mensa,1,1);
	}
	config_tps.AlicuotasEnArticulo= auxi.AlicuotasEnArticulo;
	config_tps.ModificarAlicuotaDeArticuloConsFinal= auxi.ModificarAlicuotaDeArticuloConsFinal;
	config_tps.Informes = auxi.Informes;
	config_tps.ImprimirYdespuesDeZ =auxi.ImprimirYdespuesDeZ;
	config_tps.SaltearErrorFaltaZ = auxi.SaltearErrorFaltaZ;
	config_tps.NapseModalidad= auxi.NapseModalidad;
}
