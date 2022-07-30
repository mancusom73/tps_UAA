#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cio.h> 
#include <cr.h> 
#include <day.h> 
#include <dbrouter.h> 
#include <config.h>
#include <dtoai.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <errno.h> 
#include <htoa.h> 
#include <log.h> 
#include <pos_reg.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <path.h>
#include <cmemory.h>
#include <ini.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <aarch.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifdef INVEL_L
#else
#endif
#include "path.h"
#define STRLEN_LOG     512

#pragma pack(1)

struct _log_btrieve
{
    char fecha[6];
    char hora[8];
    char      error[STRLEN_LOG];
    char lf[2];
};
void _VACIAR_LOG( char *archivo );
struct _log_base_dato
{
    char fecha[8];
    char b1;
    char hora[8];
    char b2;
    char error[1024];
    char lf[2];
};
void GRABAR_LOG_SUPERVISOR_STRING( int id_cadena, int supervisor, int origen, int nivel );
void _GRABAR_LOG_SUPERVISOR( char *operacion, int supervisor );
int DEBE_GRABAR_LOG_SISTEMA1( int origen, int nivel );
/******************************************************************************/
void GRABAR_LOG_SUPERVISOR_STRING( int id_cadena, int supervisor, int origen, int nivel )
/******************************************************************************/
{
	if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		_GRABAR_LOG_SUPERVISOR( ST( id_cadena ), supervisor );
}

/******************************************************************************/
void _GRABAR_LOG_SUPERVISOR_STRING( int id_cadena, int supervisor )
/******************************************************************************/
{
    _GRABAR_LOG_SUPERVISOR( ST( id_cadena ), supervisor );
}

/******************************************************************************/
void GRABAR_LOG_SUPERVISOR( char *operacion, int supervisor, int origen, int nivel )
/******************************************************************************/
{
	if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		_GRABAR_LOG_SUPERVISOR( operacion, supervisor );
}

/******************************************************************************/
void _GRABAR_LOG_SUPERVISOR( char *operacion, int supervisor )
/******************************************************************************/
{
    char      aux[100];

    _snprintf( aux,99, "%s (Sup %i)", operacion, supervisor );
    aux[99] = '\0';
    _GRABAR_LOG_SISTEMA( aux,LOG_SUPERVISOR,3 );
}
/******************************************************************************/
/*int*/ void GRABAR_LOG_SISTEMA( char *operacion ,int origen, int nivel )
/******************************************************************************/
{int rta = 0;
	if( operacion != NULL && strlen(operacion) > 1 && DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		rta = _GRABAR_LOG_SISTEMA( operacion,origen, nivel );
	//return rta;
}

/******************************************************************************/
int _GRABAR_LOG_SISTEMA( char *operacion,int origen, int nivel )
/******************************************************************************/
{
    GRABAR_LOG(operacion,"sistema.log",origen, nivel);
    return ( 0 );
}

/******************************************************************************/
void LOG_EXISTE_ARCHIVO( char *nom_arch , int origen, int nivel )
/******************************************************************************/
{
	if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		_LOG_EXISTE_ARCHIVO( nom_arch );
}

/******************************************************************************/
void _LOG_EXISTE_ARCHIVO( char *nom_arch )
/******************************************************************************/
{
    char aux[100];
    if( access( nom_arch, 0 ) == 0 ) {
        strcpy( aux, "Existe " );
    }
    else {
        strcpy( aux, "NO Existe " );
    }
    strcat( aux, nom_arch );
    _GRABAR_LOG_SISTEMA( aux, LOG_ARCHIVOS,4 );
}
/******************************************************************************/
void GRABAR_LOG_MEMORIA()
/******************************************************************************/
{
    //char aux[40];
    //long mem;
    if( MODO_DEBUG == 1 ) {
        /*
         * mem = coreleft();
         * sprintf( aux, "Memoria : %li", mem );
         * GRABAR_LOG_SISTEMA( aux );
         */
        _GRABAR_LOG_SISTEMA( "coreleft no soportada",LOG_MEMORIA,3 );
    }
}
/******************************************************************************/
void _GRABAR_LOG_MEMORIA()
/******************************************************************************/
{
    char aux[40];
    long mem = 0;
    //mem = coreleft();
    sprintf( aux, "Memoria : %li", mem );
    _GRABAR_LOG_SISTEMA( aux ,LOG_MEMORIA,3);
}
/******************************************************************************/
void GRABAR_LOG_NRO_TICKET()
/******************************************************************************/
{
    char aux[31];
    sprintf( aux, "T %li", ( config.pais == COLOMBIA ) ? NRO_EVENTO : NRO_TICKET + 1 );
    //_GRABAR_LOG_SISTEMA( aux, LOG_VENTAS);
	GRABAR_LOG_SISTEMA( aux, LOG_VENTAS,1);
#ifdef COMPILAR_LOGS_VIA_TCP
	{
      memset(cadena_debug,0,strlen(cadena_debug));
      sprintf(cadena_debug,"Fec:%i Suc:%i Caj:%i Tkt:%s",NRO_FECHA_OPERACION,config.sucursal,NRO_CAJA,aux );
      LOGS_VIA_TCP(cadena_debug);
	}
#endif
}
/******************************************************************************/
int GRABAR_LOG_BTRIEVE( char *cadena, int origen, int nivel )
/******************************************************************************/
{
	if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		_GRABAR_LOG_BTRIEVE(cadena);
    
	return ( 0 );
}
	/******************************************************************************/
	int _GRABAR_LOG_BTRIEVE( char *cadena )
	/******************************************************************************/
	{
		GRABAR_LOG(cadena,"btrieve.log",LOG_BTRIEVE,2 );
		return ( 0 );
	}


/******************************************************************************/
int GRABAR_LOG_BD( char *cadena, int error, int origen, int nivel )
/******************************************************************************/
{
	if( cadena!= NULL && strlen( cadena) > 1 && DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		_GRABAR_LOG_BD(cadena,error ,origen, nivel);
    return ( 0 );
}
/******************************************************************************/
int GetConfIniclienteDbServer(int q_cliente)//tuve que usarlo por razones de visibilidad
/******************************************************************************/

{return (int) (q_cliente==1)?ConfIni.clienteDbServer:ConfIni.clienteDb;}

	/******************************************************************************/
	int _GRABAR_LOG_BD( char *cadena, int error, int origen, int nivel )
	/******************************************************************************/
	{
		GRABAR_LOG(cadena,"base_dato.log",origen, nivel);
		return ( 0 );
	}
#ifdef DEBUG_IMPRESION	
/******************************************************************************/
int GRABAR_LOG_COMPROBANTES( char *cadena )
/******************************************************************************/
{
    if( ConfIni.logComprobantes == 'S' )
		GRABAR_LOG(cadena,"comprob.log",LOG_COMPROBANTES,4);
    return ( 0 );
}
#endif
/******************************************************************************/
double CORTAR_ARCHIVOS_LOG_GRANDES( void )
/******************************************************************************/
{
	return ( ConfIni.CorteArchGrandes );
}
/******************************************************************************/
int ACTUALIZA_PANTALLA_CON_REDONDEO( void )
/******************************************************************************/
{
    return ( ( (ConfIni.ActualizPantRedondeo == 'S' )? 1 : 0 ) );
}
/******************************************************************************/
void GRABAR_LOG_SISTEMA_CONFIG( char *cadena, int origen, int nivel )
/******************************************************************************/
{
    if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel )){
        _GRABAR_LOG_SISTEMA(cadena,origen, nivel);
    }
}


/******************************************************************************/
int DEBE_GRABAR_LOG_SISTEMA1( int origen, int nivel )
/******************************************************************************/
{
	int grabar = NO;
	char nivel_c;

	//itoa( nivel, nivel_c,10);
	nivel_c = (char) nivel;
    switch( origen ) {
        case LOG_PANTALLAS:
            grabar = (ConfIni.logPantallas == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logPantallas >= nivel_c) ? SI : NO;
            break;
        case LOG_PAGOS:
            grabar = (ConfIni.logPagos == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logPagos  >= nivel_c) ? SI : NO;
            break;
		case LOG_VENTAS:
            grabar = (ConfIni.logVentas== '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logVentas >= nivel_c) ? SI : NO;
            break;
		case LOG_COBROS:
            grabar = (ConfIni.logCobros == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logCobros >= nivel_c) ? SI : NO;
            break;
        case LOG_MOTOR_PROMO:
            grabar = (ConfIni.logMotorPromo == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logMotorPromo >= nivel_c) ? SI : NO;
            break;
        case LOG_FONDO_FIJO:
            grabar = (ConfIni.logFondoFijo == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logFondoFijo >= nivel_c) ? SI : NO;
            break;
        case LOG_RETIROS:
            grabar = (ConfIni.logRetiros == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logRetiros >= nivel_c) ? SI : NO;
            break;
        case LOG_CIERRE:
            grabar = (ConfIni.logCierre	== '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logCierre >= nivel_c) ? SI : NO;
            break;
        case LOG_JAVA_SERVER:
            grabar = (ConfIni.logJavaServer	== '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logJavaServer >= nivel_c) ? SI : NO;
            break;
        case LOG_VARIOS:
            grabar = (ConfIni.logVarios	== '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logVarios >= nivel_c) ? SI : NO;
            break;
        case LOG_CAMBIO_MEDIO:
            grabar = (ConfIni.logCambioMedio == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logCambioMedio >= nivel_c) ? SI : NO;
            break;
		case LOG_ODBC			:
			grabar = (ConfIni.logOdbc	== '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logOdbc >= nivel_c) ? SI : NO;
			break;
		case LOG_CON_SERVIDOR	:
			grabar = (ConfIni.logConServidor  == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logConServidor >= nivel_c) ? SI : NO;
			break;
		case LOG_COMUNICACIONES	:
			grabar = (ConfIni.logComunicaciones	== '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logComunicaciones >= nivel_c) ? SI : NO;
			break;
		case LOG_ERRORES		:
			grabar = (ConfIni.logErrores == '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logErrores >= nivel_c) ? SI : NO;
			break;
		case LOG_DEBUG			:
			grabar = (ConfIni.logDebug	== '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logDebug >= nivel_c) ? SI : NO;
			break;
		case LOG_ROBOT			:
			grabar = (ConfIni.logRobot	== '0') ? NO : SI;
			if( grabar == SI)
				grabar = (ConfIni.logRobot >= nivel_c) ? SI : NO;
			break;
		case LOG_INCONDICIONAL:
            grabar = SI;
            break;

			
    }
	
	//Dnc  ticket 2164 Se definira 2 subtipos de logs por tipo de logs uno 
	//será debug que contará con mas detalle de log y que se activará cuando 
	//ocurra algun problema y producción que es el que se encuentra activado siempre.
	//
	// Para nivel de logs
	// ( ( LOG_ROBOT * 10 ) + ConfIni.logRobot ) <= origen esto genera logs de 9 niveles,
	// acumulativos de niveles inferiores, es decir de 9 -> 1, podemos usar 0 o NO para indicar no logear.
	// para ingresar los niveles de logs.. buscar "2164", esta indicado cuando toman el valor los ConfIni.xxxXxx
	// **************************************************

	return grabar;
}

/***************************************************************************/
void VACIAR_LOG()
/***************************************************************************/
{
    _VACIAR_LOG( "sistema" );
    _VACIAR_LOG( "archivos" );
    _VACIAR_LOG( "btrieve" );
    _VACIAR_LOG( "dbrouter" );
    _VACIAR_LOG( "base_dato" );
	_VACIAR_LOG( "comprob" );
	_VACIAR_LOG( "PComDebug1" );
	_VACIAR_LOG( "PComDebug2" );

}
/***************************************************************************/
void _VACIAR_LOG( char *archivo )
/***************************************************************************/
{
    char nom_destino[30];                    // = {"SISTEMA.00"};
    char nom_log[30];                        // = {"SISTEMA.00"};
    char day[4];
    int fecha,handle;
    char strArch_nom_log[60],strArch_destino[60];
    if( VACIAR_LOG_AL_TERMINAR )
    {
        if( UTILIZAR_DIA_OPERATIVO )
        {
            fecha = NRO_DIA_OPERATIVO;
        }
        else
        {
            fecha = GET_FECHA_DOS(  );
        }

        strcpy( nom_log, archivo );
        strcat( nom_log, ".log" );
        strncpy( nom_destino, archivo, 12 );
        sprintf( day, ".%02i", DAY( fecha ) );
        strcat( nom_destino, day );
        sprintf( strArch_destino, "%s%s", PATH_BACKUPS, nom_destino );
        sprintf( strArch_nom_log, "%s%s", PATH_LOGS, nom_log );
        COPIAR_ARCHIVO( strArch_nom_log, strArch_destino, __LINE__, __FILE__ );
        handle = _CREAT( strArch_nom_log, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
}

/*-------------------------------------------------------------------------*/
void ERROR_SISTEMA_ERRNO(void){
	/* funcion que detecta errores del sistema  por causa del codigo.. apertura de files
	bloques excedidos, copias excedidas..*/
/*-------------------------------------------------------------------------*/
int error = errno;

if(error!=0){
	char aux[50];
	memset(aux, 0, sizeof(aux));
	sprintf(aux,"[%02i]___",error);
	switch( error ){
		//case  0:/* EZERO   	*/ sprintf(&aux[4]," errno: Error 0                         ");	break;
		case  1:/* EINVFNC 	*/ sprintf(&aux[4]," errno: Invalid function number         ");   break;
		case  2:/* ENOFILE 	*/ sprintf(&aux[4]," errno: File not found                  ");   break;
		case  3:/* ENOPATH 	*/ sprintf(&aux[4]," errno: Path not found                  ");   break;
		case  7:/* ECONTR  	*/ sprintf(&aux[4]," errno: Memory blocks destroyed         ");   break;
		case  9:/* EINVMEM 	*/ sprintf(&aux[4]," errno: Invalid memory block address    ");   break;
		case 10:/* EINVENV 	*/ sprintf(&aux[4]," errno: Invalid environment             ");   break;
		case 11:/* EINVFMT 	*/ sprintf(&aux[4]," errno: Invalid format                  ");   break;
		case 12:/* EINVACC 	*/ sprintf(&aux[4]," errno: Invalid access code             ");   break;
		case 13:/* EINVDAT 	*/ sprintf(&aux[4]," errno: Invalid data                    ");   break;
		case 15:/* EINVDRV 	*/ sprintf(&aux[4]," errno: Invalid drive specified         ");   break;
		case 16:/* ECURDIR 	*/ sprintf(&aux[4]," errno: Attempt to remove CurDir        ");   break;
		case 17:/* ENOTSAM 	*/ sprintf(&aux[4]," errno: Not same device                 ");   break;
		case 18:/* ENMFILE 	*/ sprintf(&aux[4]," errno: No more files                   ");   break;
		//case  2:/* ENOENT  	*/ sprintf(&aux[4]," errno: No such file or directory       ");   break;
		case  4:/* EMFILE  	*/ sprintf(&aux[4]," errno: Too many open files             ");   break;
		case  5:/* EACCES  	*/ sprintf(&aux[4]," errno: Permission denied               ");   break;
		case  6:/* EBADF   	*/ sprintf(&aux[4]," errno: Bad file number                 ");   break;
		case  8:/* ENOMEM  	*/ sprintf(&aux[4]," errno: Not enough core                 ");   break;
		case 14:/* EFAULT  	*/ sprintf(&aux[4]," errno: Unknown error                   ");   break;
		//case 15:/* ENODEV  	*/ sprintf(&aux[4]," errno: No such device                  ");   break;
		case 19:/* EINVAL  	*/ sprintf(&aux[4]," errno: Invalid argument                ");   break;
		case 20:/* E2BIG   	*/ sprintf(&aux[4]," errno: Arg list too long               ");   break;
		case 21:/* ENOEXEC 	*/ sprintf(&aux[4]," errno: Exec format error               ");   break;
		case 22:/* EXDEV   	*/ sprintf(&aux[4]," errno: Cross-device link               ");   break;
		case 23:/* ENFILE  	*/ sprintf(&aux[4]," errno: Too many open files             ");   break;
		case 24:/* ECHILD  	*/ sprintf(&aux[4]," errno: No child process                ");   break;
		case 25:/* ENOTTY  	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS                ");   break;
		case 26:/* ETXTBSY 	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS                ");   break;
		case 27:/* EFBIG   	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS                ");   break;
		case 28:/* ENOSPC  	*/ sprintf(&aux[4]," errno: No space left on device         ");   break;
		case 29:/* ESPIPE  	*/ sprintf(&aux[4]," errno: Illegal seek                    ");   break;
		case 30:/* EROFS   	*/ sprintf(&aux[4]," errno: Read-only file system           ");   break;
		case 31:/* EMLINK  	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS                ");   break;
		case 32:/* EPIPE   	*/ sprintf(&aux[4]," errno: Broken pipe                     ");   break;
		case 33:/* EDOM    	*/ sprintf(&aux[4]," errno: Math argument                   ");   break;
		case 34:/* ERANGE  	*/ sprintf(&aux[4]," errno: Result too large                ");   break;
		case 35:/* EEXIST  	*/ sprintf(&aux[4]," errno: File already exists             ");   break;
		case 36:/* EDEADLOCK */ sprintf(&aux[4]," errno: Locking violation               ");   break;
		case 37:/* EPERM   	*/ sprintf(&aux[4]," errno: Operation not permitted         ");   break;
		case 38:/* ESRCH   	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS                ");   break;
		case 39:/* EINTR   	*/ sprintf(&aux[4]," errno: Interrupted function call      ");    break;
		case 40:/* EIO     	*/ sprintf(&aux[4]," errno: Input/output error              ");   break;
		case 41:/* ENXIO   	*/ sprintf(&aux[4]," errno: No such device or address      ");    break;
		case 42:/* EAGAIN  	*/ sprintf(&aux[4]," errno: Resource temporarily unavailable"); break;
		case 43:/* ENOTBLK 	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS                ");   break;
		case 44:/* EBUSY   	*/ sprintf(&aux[4]," errno: Resource busy                   ");   break;
		case 45:/* ENOTDIR 	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS                ");   break;
		case 46:/* EISDIR  	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS                ");   break;
		case 47:/* EUCLEAN 	*/ sprintf(&aux[4]," errno: UNIX - not MSDOS         			");   break;
	}
	if( error  == 2)
		GRABAR_LOG_SISTEMA( aux,LOG_ERRORES,4 );
	else
		GRABAR_LOG_SISTEMA( aux,LOG_ERRORES,2 );
	errno=0;
	}
}
