#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <cio.h> 
#include <cr.h> 
#include <mensaje.h>
#include <mstring.h> 
#include <pausa.h> 
#include <tkt.h> 
#include <t_file.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#include <sys/io.h>
#else
#include <dos.h>
#endif
#include <config.h>
#include <aarch.h>
#if defined(INVEL_W) || defined(INVEL_L)
#include <sys/stat.h>
#endif
#define _NO_COMUNICACION   -1
#define _SERVER_DISPONIBLE  0
#define _ESPERA_ATENCION    1
#define _ATENDIDO           2
#define _HAY_RESPUESTA      3
#define _RESPUESTA_TOMADA   4
#define _CAJA_OFF_LINE      5
/****************************************************************************/
int _ENVIAR_PAQUETE_T_FILE( char *paquete, int l_paquete, char *rta, int l_rta,
                            int time_out_disponible )
/****************************************************************************/
{
    int error = 1;
    #ifdef COMPILAR_ON_LINE
    int handle;
    /*---------------------- Crea el archivo de envio ------------------------*/
    #if defined(INVEL_W) || defined(INVEL_L)
    handle = CREAT_( _SEND, S_IFREG | S_IWRITE | S_IREAD );
    #else
    handle = CREAT( _SEND, FA_ARCH );
    #endif
    /*-------------- Verifica condiciones de la comunicacion ----------------*/
    if( handle < 4 ) {
        MENSAJE_STRING( S_PROBLEMA_ENVIAR_DATOS );
    }
    else if( LEE_BANDERA_T_FILE() == _NO_COMUNICACION ) {
        MENSAJE_STRING( S_SERVER_FUERA_LINEA );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    else if( LEE_BANDERA_T_FILE() == _CAJA_OFF_LINE ) {
        MENSAJE_STRING( S_TPV_FUERA_LINEA );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    else {
        /*--------------- Verifica disponibilidad del server --------------*/
        /*      MENSAJE_SIN_SONIDO( "ESPERANDO ATENCION DEL SERVER ..." );
         * if( !ESPERAR_BANDERA_T_FILE( _SERVER_DISPONIBLE, time_out_disponible, _HAY_RESPUESTA ) ) {
         * MENSAJE( "EL SERVER NO ATENDIO EL LLAMADO !" );
         * _CLOSE( handle , __LINE__, __FILE__ );
         * }
         * else { */
        /*---------------------- Envia el paquete ------------------------*/
        error = 2;
        _WRITE( handle, paquete, l_paquete, __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
        GRABA_BANDERA_T_FILE( _ESPERA_ATENCION );
        /*---------------------- Espera que lo atiendan -----------------------*/
        if( !ESPERAR_BANDERA_T_FILE( _ATENDIDO, 20, _HAY_RESPUESTA ) ) {
            MENSAJE_STRING( S_SERVER_NO_ATIENDE_ATEN );
        }
        else {
            MENSAJE_SIN_SONIDO_STRING( S_ATENDIDO_ESPERANDO_RESPUESTA );
            if( !ESPERAR_BANDERA_T_FILE( _HAY_RESPUESTA, 130, NO ) ) {
                MENSAJE_STRING( S_SERVER_NO_RESPONDIO );
            }
            else {
                /*--------------------- Lee la respuesta ----------------------*/
                #if defined(INVEL_W) || defined(INVEL_L)
                handle = OPEN( _RECEIVE, O_RDWR );
                #else
                handle = OPEN( _RECEIVE, O_RDWR | O_DENYNONE );
                #endif
                if( handle > 3 ) {
                    _READ( handle, rta, l_rta, __LINE__, __FILE__ );
                    error = 0;
                    _CLOSE( handle, __LINE__, __FILE__ );
                    GRABA_BANDERA_T_FILE( _RESPUESTA_TOMADA );
                }
                else {
                    MENSAJE_STRING( S_PROBLEMA_LEER_RESPUEST );
                }
            }
        }
        //      }
    }
    BORRAR_MENSAJE();
    #endif
    return ( error );
}
/*****************************************************************************/
int ESPERAR_BANDERA_T_FILE( int bandera, int segundos, int bandera2 )
/*****************************************************************************/
{
    int h,atendido,b;
    for( h = 0, atendido = 0;h < segundos && !atendido;h++ ) {
        PAUSA( 18 );
        b = LEE_BANDERA_T_FILE();
        if( b == bandera || ( b == bandera2 && bandera2 ) ) {
            atendido = 1;
        }
    }
    return ( atendido );
}
/*****************************************************************************/
int LEE_BANDERA_T_FILE()
/*****************************************************************************/
{
    int bandera = -1, handle;
    //handle = OPEN( _T_FILE_DAT, O_RDWR | O_DENYNONE );
    handle = OPEN( _T_FILE_DAT, O_RDWR );
    if( handle > 3 ) {
        /*-------------- Lee bandera general ----------------*/
        _READ( handle, ( char* )&bandera, 2, __LINE__, __FILE__ );
        if( bandera == 0 ) {
            /*-------------- Lee bandera de la caja ----------------*/
            LSEEK( handle, ( long )( NRO_CAJA * 2 ), SEEK_SET, __LINE__, __FILE__ );
            _READ( handle, ( char* )&bandera, 2, __LINE__, __FILE__ );
        }
        else {
            bandera = _NO_COMUNICACION;
        }
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return ( bandera );
}
/*****************************************************************************/
int GRABA_BANDERA_T_FILE( int bandera )
/*****************************************************************************/
{
    int rta = -1, handle;
    //modo = _MODO_EJECUCION;
    //_MODO_EJECUCION = OPERACION;
    handle = OPEN( _T_FILE_DAT, O_RDWR );
    if( handle > 3 ) {
        LSEEK( handle, ( long )( NRO_CAJA * 2 ), SEEK_SET, __LINE__, __FILE__ );
        rta = _WRITE( handle, ( char* )&bandera, 2, __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    //_MODO_EJECUCION = modo;
    return ( rta );
}

