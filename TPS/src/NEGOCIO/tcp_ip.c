#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <b_mens.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <pausa.h> 
#include <tcp_ip.h> 
#include <tkt.h> 
#include <cmemory.h>
#include <ini.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
int SOCKET_ABIERTO = NO;
#define TAM_BUFFER_FTP 24
struct _p_ftp
{
    int rta;
    char *ip;
    char *user_name;
    char *password;
    char *file_origen;
    char *file_destino;
    int modo;                               // 0 ASCII, 1 BINARIO
    ///////// 24.
    char d_ip[32];
    char d_user_name[32];
    char d_password[32];
    char d_file_origen[64];
    char d_file_destino[64];
}        *p_ftp;
int FTP_ARCHIVO( char *origen, char *destino, int get_put, int modo );
int TCP_ftp( char *argv[] );
/****************************************************************************/
int _ENVIAR_PAQUETE_TCP_IP( char *paquete, int l_paquete, char *rta, int l_rta, int timeout_nodo,
                            char espera_rta, int canal )
/****************************************************************************/
{
    int error = 1, bytes = 0, h;
    char aux[50];
    struct
    {
        int rta;
        char *buffer;
        int l_buffer;
    } rta_drv;
    rta_drv.rta = 0;
    #ifdef INVEL_L
    {
        int i;
        printf( "\nPaquete a enviar - largo (%d):\n", l_paquete );
        for( i = 0;i < l_paquete;i++ ) {
            if( isalpha( ( char )paquete[i] ) ) {
                printf( "%c ", ( char )paquete[i] );
            }
            else {
                printf( "%x ", ( char )paquete[i] & 0xFF );
            }
        }
    }
    #endif
    //GRABAR_LOG_SISTEMA_STRING( S_ANTES_DE_ENVIAR_PAQUETE );
    if( timeout_nodo == 0 ) {
        timeout_nodo = 40;
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    rta_drv.buffer = paquete;
    rta_drv.l_buffer = l_paquete;
    DRIVER( _DRV_tcp, _TCP_enviar, canal, &rta_drv, sizeof( rta_drv ) );
    #else
    rta_drv.buffer = paquete;
    rta_drv.l_buffer = l_paquete;
    memcpy( puntero_memoria_driver_invel, paquete, l_paquete );
    rta_drv.buffer = puntero_memoria_real_driver_invel;
    puntero_memoria_driver_invel[rta_drv.l_buffer] = 0;
    DRIVER( _DRV_tcp, _TCP_enviar, canal, &rta_drv, sizeof( rta_drv ) );
    memcpy( paquete, puntero_memoria_driver_invel, l_paquete );
    #endif
    {
        char aux[100];
        _snprintf(aux, sizeof(aux)-1, "Envio: %i Bytes Rta: %i BufRta: %d TOut: %d", l_paquete, rta_drv.rta,
                 ( rta == NULL ) ? ( int )0 : ( int )1, timeout_nodo );
        GRABAR_LOG_DBROUTER( aux ,LOG_VENTAS,2);
    }
    error = rta_drv.rta;
    if( !error && rta ) {
        if( espera_rta ) {
            error = 2;
            for( h = 0;h < timeout_nodo * 4 && !bytes;h++ ) {
                PAUSA( 9 );
                DRIVER( _DRV_tcp, _TCP_status, canal, &bytes, sizeof( bytes ) );
            }
            if( bytes ) {
                #if defined(INVEL_W) || defined(INVEL_L)
                rta_drv.buffer = rta;
                rta_drv.l_buffer = l_rta;
                DRIVER( _DRV_tcp, _TCP_recibir, canal, &rta_drv, sizeof( rta_drv ) );
                #else
                rta_drv.buffer = rta;
                rta_drv.l_buffer = l_rta;
                memcpy( puntero_memoria_driver_invel, rta, l_rta );
                rta_drv.buffer = puntero_memoria_real_driver_invel;
                DRIVER( _DRV_tcp, _TCP_recibir, canal, &rta_drv, sizeof( rta_drv ) );
                memcpy( rta, puntero_memoria_driver_invel, l_rta );
                #endif
                if( !rta_drv.rta ) {
                    error = 0;
                }
                else {
                    error = rta_drv.rta;
                }
            }
            if( error ) {
                _snprintf(aux, sizeof(aux)-1, "error al recibir. error %d", error );
                GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
                memset( aux, 0, strlen( aux ) );
            }
        }
    }
    else {
        /* Para el canal touch no verifico el ACK por eso no grabo log de error */
        if( ( error ) && ( canal != _CANAL_TOUCH_SCREEN ) ) {
            _snprintf(aux, sizeof(aux)-1, "error al enviar paquete. error %d", error );
            GRABAR_LOG_SISTEMA( aux ,LOG_COMUNICACIONES,3);
            //memset( aux, 0, strlen( aux ) );
        }
    }
    //GRABAR_LOG_SISTEMA_STRING( S_DESPUES_DE_ENVIAR_PAQUETE );
    return ( error );
}
/****************************************************************************/
int INIT_DRV_TCP_IP( void )
/****************************************************************************/
{
    int    rta_d = 0, rta_t = 0, rta_j = 0;
	char   mens_error[40];
    {
        /*DRIVER( _DRV_tcp, _TCP_inicializacion, _CANAL_DBROUTER, &rta_d, sizeof( rta_d ) );
        if( rta_d ) {
            sprintf( mens_error, "ERROR DBROUTER - Cod.%d", rta_d );
            GRABAR_LOG_DBROUTER( mens_error );
        }*/
    }
	if( AUTORIZACION_ON_LINE_HABILITADA ) {
		DRIVER( _DRV_tcp, _TCP_inicializacion, _CANAL_TARJETAS, &rta_t, sizeof( rta_t ) );
		if( rta_t ) {
			_snprintf(mens_error, sizeof(mens_error)-1, "ERROR TARJETAS - Cod.%d", rta_t );
			GRABAR_LOG_DBROUTER( mens_error,LOG_VENTAS,2 );
		}
	}
	if( strcmp( DIRECCION_IP_JGATEWAY, "" ) != 0 && strcmp( DIRECCION_IP_JGATEWAY," ") != 0 ) {
        DRIVER( _DRV_tcp, _TCP_inicializacion, _CANAL_JGATEWAY, &rta_j, sizeof( rta_j ) );
		if( rta_j ) {
			_snprintf(mens_error, sizeof(mens_error)-1, "ERROR TARJETAS - Cod.%d", rta_j );
			GRABAR_LOG_DBROUTER( mens_error ,LOG_ERRORES,2);
		}
    }
	return( ( !rta_d || !rta_t || !rta_j ) ? 1 : 0 );
}
/****************************************************************************/
int ABRIR_CANAL_TCP_IP( int canal, char *ip, int formato_mensaje )
/****************************************************************************/
{
    char aux[50];
    struct
    {
        int rta;
        char *buffer;
        int l_buffer;
        int formato_mensaje;
    } rta_drv;
    if( HABILITA_PING ) {
        PING_CLOSE_SOCKET();
    }
    //GRABAR_LOG_SISTEMA( "ANTES DE ABRIR EL CANAL" );
    #if defined(INVEL_W) || defined(INVEL_L)
    rta_drv.buffer = ip;
    rta_drv.l_buffer = strlen( ip );
    rta_drv.formato_mensaje = formato_mensaje; 
    rta_drv.rta = 0;
    DRIVER( _DRV_tcp, _TCP_abrir_canal, canal, &rta_drv, sizeof( rta_drv ) );
    #else
    memcpy( puntero_memoria_driver_invel, ip, strlen( ip ) );
    rta_drv.buffer = puntero_memoria_real_driver_invel;
    rta_drv.l_buffer = strlen( ip );
    rta_drv.formato_mensaje = formato_mensaje;
    puntero_memoria_driver_invel[rta_drv.l_buffer] = 0;
    DRIVER( _DRV_tcp, _TCP_abrir_canal, canal, &rta_drv, sizeof( rta_drv ) );//aca tira 2
    #endif
    if( !rta_drv.rta ) {
        _snprintf(aux, sizeof(aux)-1, "ABRIR CANAL OK (ip: %s canal: %d)", ip, canal );
        GRABAR_LOG_DBROUTER( aux ,LOG_VENTAS,2);
    }
    else {
        _snprintf(aux, sizeof(aux)-1, "ERROR %d ABRIR CANAL IP: %s %i", rta_drv.rta, ip, canal );
		//Log para robot
		GRABAR_LOG_DBROUTER( "ERROR AL ABRIR CANAL",LOG_ROBOT,1 );
		GRABAR_LOG_DBROUTER( aux,LOG_ROBOT,2 );
        GRABAR_LOG_SISTEMA( aux ,LOG_ROBOT,2);
        MENSAJE_SIN_SONIDO( aux,NO );
        memset( aux, 0, strlen( aux ) );
        if( HABILITA_PING ) {
            sprintf( aux, DIRECCION_IP_SERVIDOR );
            PING_ABRIR_SOCKET( aux );
            memset( aux, 0, strlen( aux ) );
        }
    }
    //GRABAR_LOG_SISTEMA( "DESPUES DE ABRIR EL CANAL" );
    return ( !rta_drv.rta );
}
/****************************************************************************/
int CERRAR_CANAL_TCP_IP( int canal )
/****************************************************************************/
{
    int rta = 0;
    char aux[50];
    GRABAR_LOG_SISTEMA( "ANTES DE CERRAR CANAL" ,LOG_COMUNICACIONES,4);
    DRIVER( _DRV_tcp, _TCP_cerrar_canal, canal, &rta, sizeof( rta ) );
    if( rta ) {
        _snprintf(aux, sizeof(aux)-1, "error cerrar canal %d con rta %d", canal, rta );
        MENSAJE( aux );
        GRABAR_LOG_SISTEMA( aux ,LOG_COMUNICACIONES,3);
        memset( aux, 0, strlen( aux ) );
    }
    GRABAR_LOG_SISTEMA( "DESPUES DE CERRAR CANAL" ,LOG_COMUNICACIONES,4);
    if( HABILITA_PING ) {
        sprintf( aux, DIRECCION_IP_SERVIDOR );
        PING_ABRIR_SOCKET( aux );
        memset( aux, 0, strlen( aux ) );
    }
    return ( !rta );
}
/****************************************************************************/
int PING()
/****************************************************************************/
{
    int error = 0;
    if( HABILITA_PING ) {
        if( SOCKET_ABIERTO ) {
            DRIVER( _DRV_tcp, _TCP_ping, 0, &error, sizeof( error ) );
            if( error ) {
                DRIVER( _DRV_tcp, _TCP_ping, 0, &error, sizeof( error ) );
            }
        }
    }
    return !error;
}
/****************************************************************************/
int PING_ABRIR_SOCKET( char *ip_addres )
/****************************************************************************/
{
    int error = 0;
    struct
    {
        int rta;
        char *buffer;
        int l_buffer;
        int time_out_ping;
    } rta_drv;
    if( HABILITA_PING ) {
        rta_drv.rta = 1;
        rta_drv.buffer = ip_addres;
        rta_drv.l_buffer = strlen( ip_addres );
        rta_drv.time_out_ping = TIME_OUT_PING;
        #if !defined(INVEL_W) && !defined(INVEL_L)
        memcpy( puntero_memoria_driver_invel, ip_addres, rta_drv.l_buffer );
        rta_drv.buffer = puntero_memoria_real_driver_invel;
        #endif
        DRIVER( _DRV_tcp, _TCP_abrir_socket_ping, 0, &rta_drv, sizeof( rta_drv ) );
        error = rta_drv.rta;
        if( error ) {
            SOCKET_ABIERTO = NO;
        }
        else {
            SOCKET_ABIERTO = SI;
        }
    }
    return !error;
}
/****************************************************************************/
int PING_CLOSE_SOCKET()
/****************************************************************************/
{
    int error = 0;
    if( SOCKET_ABIERTO ) {
        DRIVER( _DRV_tcp, _TCP_close_socket_ping, 0, &error, sizeof( error ) );
    }
    return !error;
}


/****************************************************************************/
int FTP_ARCHIVO_ASCII( char *origen, char *destino, int get_put )
/****************************************************************************/
{
    return ( FTP_ARCHIVO( origen, destino, get_put, ASCII ) );
}


/****************************************************************************/
int FTP_ARCHIVO_BIN( char *origen, char *destino, int get_put )
/****************************************************************************/
{
    return ( FTP_ARCHIVO( origen, destino, get_put, BINARIO ) );
}


/****************************************************************************/
int FTP_ARCHIVO( char *origen, char *destino, int get_put, int modo )
/****************************************************************************/
{
    char *ftp_str[10];
    char str_modo[30];
    char ip[30];
    char str_get_put[30];
    char login[30];
    char password[30];
    int iRta;

    if( get_put == GET ) {
        strcpy( str_get_put, "get" );
        MENSAJE_STRING( S_OBTENIENDO_ARCHIVO_DE_SERVER_FTP );
    }
    else if( get_put == PUT ) {
        strcpy( str_get_put, "put" );
        MENSAJE_STRING( S_ENVIANDO_ARCHVIO_AL_SERVER_FTP );
    }
    else {
        return -1;
    }

    if( modo == BINARIO ) {
        strcpy( str_modo, "binary" );
    }
    else if( modo == ASCII ) {
        strcpy( str_modo, "ascii" );
    }
    else {
        return -2;
    }

    ftp_str[0] = "";
    ftp_str[1] = str_get_put;
    ftp_str[2] = origen;
    ftp_str[3] = destino;
    ftp_str[4] = str_modo;
    strcpy( ip, ConfIni.FtpIp/*DIRECCION_IP_SERVIDOR*/ );
    ftp_str[5] = ip;

    if( strcmp( SERVER_LOGIN, "NRO_CAJA" ) == 0 || strcmp( SERVER_LOGIN, "nro_caja" ) == 0 ) {
		_snprintf(login, sizeof(login)-1, "caja%s", NRO_NOM_CAJA );
		/*( !USAR_NRO_CAJA_DGI ) {
		   sprintf( login, "caja%s", NRO_NOM_CAJA );
		} else {
	       sprintf( login, "caja%04d", NRO_CAJA_DGI );
	    }*/
    }
    else {
        strcpy( login, SERVER_LOGIN );
    }
    ftp_str[6] = login;

	if( strcmp( SERVER_PASSWORD, "NRO_CAJA" ) == 0 || strcmp( SERVER_PASSWORD, "nro_caja" ) == 0 ) {
		_snprintf(password, sizeof(password)-1, "caja%s", NRO_NOM_CAJA );        
		/*( !USAR_NRO_CAJA_DGI ) {
		   sprintf( password, "caja%s", NRO_NOM_CAJA );
		} else {
	       sprintf( password, "caja%04d", NRO_CAJA_DGI );
	    }*/
    }
    else {
        strcpy( password, SERVER_PASSWORD );
    }

	if( MODO_DEBUG == 50 ) {
      char aa[100];

	   _snprintf(aa, sizeof(aa)-1,"___login:%s password:%s",login, password);
	   GRABAR_LOG_SISTEMA( aa ,LOG_COMUNICACIONES,4);
	}

    ftp_str[7] = password;

    iRta = TCP_ftp( ftp_str );

    BORRAR_MENSAJE();

    return iRta;
}


/****************************************************************************/
int TCP_ftp( char *argv[] )
/****************************************************************************/
{
    int funcion,resp;
    char trace[128];
    int offset = TAM_BUFFER_FTP;
    char *buffer_ftp = NULL;

    _snprintf(trace, sizeof(trace)-1, "FTP Servidor %s User %s Pass: %s", argv[5], argv[6], argv[7] );
    GRABAR_LOG_DBROUTER( trace ,LOG_DEBUG,4);
    _snprintf(trace, sizeof(trace)-1, "FTP %s Source %s Target: %s (%s)", argv[1], argv[2], argv[3], argv[4] );
    GRABAR_LOG_DBROUTER( trace ,LOG_DEBUG,4);

    #if !defined(INVEL_W) && !defined(INVEL_L)

    p_ftp = puntero_memoria_driver_invel;
    // IP
    strcpy( p_ftp->d_ip, argv[5] );
    p_ftp->ip = puntero_memoria_real_driver_invel + offset;
    offset += 32;                                 // sizeof( p_ftp->d_ip);
    // USER
    strcpy( p_ftp->d_user_name, argv[6] );
    p_ftp->user_name = puntero_memoria_real_driver_invel + offset;
    offset += 32;                                 // sizeof(struct _p_ftp.d_user_name);
    // PASSWORD
    strcpy( p_ftp->d_password, argv[7] );
    p_ftp->password = puntero_memoria_real_driver_invel + offset;
    offset += 32;                                 // sizeof(struct _p_ftp.d_password);
    // ORIGEN
    strcpy( p_ftp->d_file_origen, argv[2] );
    p_ftp->file_origen = puntero_memoria_real_driver_invel + offset;
    offset += 64;                                 // sizeof(struct _p_ftp.d_file_origen);
    // DESTINO
    strcpy( p_ftp->d_file_destino, argv[3] );
    p_ftp->file_destino = puntero_memoria_real_driver_invel + offset;
    offset += 64;                                 // sizeof(struct _p_ftp.d_file_destino);
    // MODO
    p_ftp->modo = ( strcmp( argv[4], "binary" ) == 0 ) ? 1 : 0;
    funcion = ( strcmp( argv[1], "put" ) == 0 ) ? _FTP_put : _FTP_get;

    #else

    buffer_ftp = malloc( MAXIMO_BUFFER_TRANSACCION );

    p_ftp = ( struct _p_ftp * )buffer_ftp;

    // IP
    strcpy( p_ftp->d_ip, argv[5] );
    //sprintf( p_ftp->d_ip, "%s", argv[5] );
    //p_ftp->ip = argv[5] + offset;
    p_ftp->ip = p_ftp->d_ip;
    offset += 32;                     // sizeof( p_ftp->d_ip);
    // USER
    strcpy( p_ftp->d_user_name, argv[6] );
    p_ftp->user_name = p_ftp->d_user_name;
    offset += 32;                     // sizeof(struct _p_ftp.d_user_name);
    // PASSWORD
    strcpy( p_ftp->d_password, argv[7] );
    p_ftp->password = p_ftp->d_password;
    offset += 32;                     // sizeof(struct _p_ftp.d_password);
    // ORIGEN
    strcpy( p_ftp->d_file_origen, argv[2] );
    p_ftp->file_origen = p_ftp->d_file_origen;
    offset += 64;                     // sizeof(struct _p_ftp.d_file_origen);
    // DESTINO
    strcpy( p_ftp->d_file_destino, argv[3] );
    p_ftp->file_destino = p_ftp->d_file_destino;
    offset += 64;                     // sizeof(struct _p_ftp.d_file_destino);
    // MODO
    p_ftp->modo = ( strcmp( argv[4], "binary" ) == 0 ) ? 1 : 0;
    funcion = ( strcmp( argv[1], "put" ) == 0 ) ? _FTP_put : _FTP_get;

    #endif
    // llamada al driver
    DRIVER( _DRV_tcp, funcion, _CANAL_FTP_1, p_ftp, sizeof( struct _p_ftp ) );
    _snprintf(trace, sizeof(trace)-1, "FTP: %s (%i)", ( p_ftp->rta ) ? "Error" : "Ok", p_ftp->rta );
    GRABAR_LOG_DBROUTER( trace ,LOG_DEBUG,4);

    resp = p_ftp->rta;
    FREEMEM( buffer_ftp );

    return resp;
    //return 1;
}


/****************************************************************************/
unsigned TCP_canal_puerto( int canal, int puerto )
/****************************************************************************/
{
    unsigned rta;
    rta = ( canal & 0x07 ) + ( ( puerto & 0x1fff ) << 3 );
    return ( rta );
}

