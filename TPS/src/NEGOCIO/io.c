/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <config.h>
#include <stdlib.h>
#include <fcntl.h>
#undef MK_FP
#ifdef INVEL_W
#include <STRING.H>
#include <SYS\TYPES.H>
#include <SYS\UTIME.H>
#include <STDIO.H> 
#include <DOS.H>
#include <SYS\STAT.H>
#include <IO.H>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <unistd.h>
#include <utime.h>
#endif
#include <errno.h>
#include <aarch.h>
#include "path.h"
/*------------------------- Defines -------------------------------------*/
#define INTENTOS  3
#define OP_READ   1
#define OP_WRITE  2
#define OP_LSEEK  3
#define OP_CLOSE  4
#define OP_OPEN   5
#define OP_CREAT  6
#define OP_MALLOC 7
#define SEGUNDOS_PARA_REINTENTO 60
/*------------------------ Globales --------------------------*/
static char SENAL_ERROR_INT24,ERROR_EN_RED,*FILE_,op_truncada;
static unsigned long timer_inicial;
static unsigned LINEA;
static int codigo_error;
static struct
{
    int ax;
    int di;
} INT24;
static struct
{
    int copiar_arch;
    int arch_activo;
} cop_arch;
#ifdef INVEL_L
int filelength( int handle )
{
    int c = 0;
    char ch;
    while( read( handle, &ch, 1 ) > 0 ) {
        c++;
    }
    lseek( handle, 0, SEEK_SET );
    return c;
}
#endif
int CAMBIAR_ATRIBUTO_ARCHIVOS( char *nom_destino );
void VERIFICAR_REINTENTO();
void GRABAR_ERROR( int operacion, int error, int intentos, char *nombre, int handle );
void SET_ERROR_EN_RED( void );
void _SET_ERROR_EN_RED( void );
void _SET_FECHA_ARCHIVO( int handle, unsigned fecha );
/***********************************************************************/
int _READ( int handle, char *buffer, int cant, int linea, char *file )
/***********************************************************************/
{
    int intentos, error = -1, salir, c, red;
    op_truncada = 0;
    if( handle >= 3 && handle != 999 ) {
        FILE_ = file;
        LINEA = linea;
        red = ARCH_RED( NRO_ARCHIVO );
        if( ERROR_EN_RED && red ) {
            VERIFICAR_REINTENTO();
        }
        salir = ( ERROR_EN_RED && red ) ? 1 : 0;
        // if( ERROR_EN_RED ) VERIFICAR_REINTENTO();
        // salir = (ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED)? 1 : 0;
        error = 1;
        intentos = 0;
        while( error && intentos < INTENTOS && !salir ) {
            SENAL_ERROR_INT24 = 0;
            #ifdef INVEL_L
            c = read( handle, buffer, cant );
            #else
            c = _read( handle, buffer, cant );
            #endif
            if( c == cant && !SENAL_ERROR_INT24 ) {
                error = 0;
            }
            else {
                codigo_error = errno;
                if( c != cant ) {
                    op_truncada = 1;
                }
            }
            intentos++;
            salir = ( ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED ) ? 1 : 0;
        }
        if( error || intentos > 1 || ( ERROR_EN_RED && ARCH_RED( NRO_ARCHIVO ) ) ) {
            GRABAR_ERROR( OP_READ, error, intentos, NULL, handle );
        }
    }
    return ( error );
}
/***********************************************************************/
int _WRITE( int handle, char *buffer, int cant, int linea, char *file )
/***********************************************************************/
{
    int intentos, error = 1, salir, c,red;
    op_truncada = 0;
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        error = 0;
    }
    else if( handle >= 3 && handle != 999 ) {
        FILE_ = file;
        LINEA = linea;
        red = ARCH_RED( NRO_ARCHIVO );
        if( ERROR_EN_RED && red ) {
            VERIFICAR_REINTENTO();
        }
        salir = ( ERROR_EN_RED && red ) ? 1 : 0;
        // if( ERROR_EN_RED ) VERIFICAR_REINTENTO();
        // salir = (ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED)? 1 : 0;
        error = 1;
        intentos = 0;
        while( error && intentos < INTENTOS && !salir ) {
            SENAL_ERROR_INT24 = 0;
            #ifdef INVEL_L
            c = write( handle, buffer, cant );
            #else
            c = _write( handle, buffer, cant );
            #endif
            if( c == cant && !SENAL_ERROR_INT24 ) {
                error = 0;
            }
            else {
                codigo_error = errno;
                if( c != cant ) {
                    op_truncada = 1;
                }
            }
            intentos++;
            salir = ( ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED ) ? 1 : 0;
        }
        if( error || intentos > 1 || ( ERROR_EN_RED && ARCH_RED( NRO_ARCHIVO ) ) ) {
            GRABAR_ERROR( OP_WRITE, error, intentos, NULL, handle );
        }
    }
    return ( error );
}
/***********************************************************************/
int LSEEK( int handle, long pos, int tipo, int linea, char *file )
/***********************************************************************/
{
    int intentos, error = 1, salir, red;
    if( handle >= 3 && handle != 999 ) {
        FILE_ = file;
        LINEA = linea;
        red = ARCH_RED( NRO_ARCHIVO );
        if( ERROR_EN_RED && red ) {
            VERIFICAR_REINTENTO();
        }
        salir = ( ERROR_EN_RED && red ) ? 1 : 0;
        error = 1;
        intentos = 0;
        while( error && intentos < INTENTOS && !salir ) {
            SENAL_ERROR_INT24 = 0;
            if( lseek( handle, pos, tipo ) >= 0L && !SENAL_ERROR_INT24 ) {
                error = 0;
            }
            else {
                codigo_error = errno;
            }
            intentos++;
            salir = ( ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED ) ? 1 : 0;
        }
        if( error || intentos > 1 || ( ERROR_EN_RED && ARCH_RED( NRO_ARCHIVO ) ) ) {
            GRABAR_ERROR( OP_LSEEK, error, intentos, NULL, handle );
        }
    }
    return ( error );
}
/***********************************************************************/
int _CLOSE( int handle, int linea, char *file )
/***********************************************************************/
{
    int intentos, error = 1, salir, red;
    if( handle >= 3 && handle != 999 ) {
        FILE_ = file;
        LINEA = linea;
        red = ARCH_RED( NRO_ARCHIVO );
        if( ERROR_EN_RED && red ) {
            VERIFICAR_REINTENTO();
        }
        salir = ( ERROR_EN_RED && red ) ? 1 : 0;
        // if( ERROR_EN_RED ) VERIFICAR_REINTENTO();
        // salir = (ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED)? 1 : 0;
        error = 1;
        intentos = 0;
        while( error && intentos < INTENTOS && !salir ) {
            SENAL_ERROR_INT24 = 0;
            #ifdef INVEL_L
            if( close( handle ) == 0 && !SENAL_ERROR_INT24 ) {
                error = 0;
            }
            else {
                codigo_error = errno;
            }
            #else
            if( _close( handle ) == 0 && !SENAL_ERROR_INT24 ) {
                error = 0;
            }
            else {
                codigo_error = errno;
            }
            #endif
            intentos++;
            salir = ( ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED ) ? 1 : 0;
        }
        if( error || intentos > 1 || ( ERROR_EN_RED && ARCH_RED( NRO_ARCHIVO ) ) ) {
            GRABAR_ERROR( OP_CLOSE, error, intentos, NULL, handle );
        }
    }
    return ( error );
}
/***********************************************************************/
int _OPEN( char *nombre, int tipo, int linea, char *file )
/***********************************************************************/
{
    int intentos,error,handle,salir,red;
    FILE_ = file;
    LINEA = linea;
    red = ARCH_RED( NRO_ARCHIVO );
    if( ERROR_EN_RED && red ) {
        VERIFICAR_REINTENTO();
    }
    salir = ( ERROR_EN_RED && red ) ? 1 : 0;
    error = -1;
    intentos = 0;
    while( error < 0 && intentos < INTENTOS && !salir ) {
        SENAL_ERROR_INT24 = 0;
        #ifdef INVEL_L
        handle = open( nombre, tipo );
        #else
        handle = _open( nombre, tipo );
        #endif
        if( handle > 0 && !SENAL_ERROR_INT24 ) {
            error = handle;
        }
        else {
            codigo_error = errno;
            if( ( int )codigo_error == -128 ) {
                SET_ERROR_EN_RED();
            }
        }
        intentos++;
        salir = ( ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED ) ? 1 : 0;
    }
    if( error < 0 || intentos > 1 || ( ERROR_EN_RED && ARCH_RED( NRO_ARCHIVO ) ) ) {
        GRABAR_ERROR( OP_OPEN, ( error < 0 ) ? 1 : 0, intentos, nombre, 0 );
    }
    return ( error );
}
/***********************************************************************/
int _CREAT( char *nombre, int tipo, int linea, char *file )
/***********************************************************************/
{
    int intentos,error,handle,salir,red;
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        error = 999;
    }
    else {
        FILE_ = file;
        LINEA = linea;
        red = ARCH_RED( NRO_ARCHIVO );
        if( ERROR_EN_RED && red ) {
            VERIFICAR_REINTENTO();
        }
        salir = ( ERROR_EN_RED && red ) ? 1 : 0;
        // if( ERROR_EN_RED ) VERIFICAR_REINTENTO();
        // salir = (ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED)? 1 : 0;
        error = -1;
        intentos = 0;
        while( error < 0 && intentos < INTENTOS && !salir ) {
            SENAL_ERROR_INT24 = 0;
            #ifdef INVEL_L
            handle = creat( nombre, tipo );
            #else
            //handle = _creat( nombre, tipo );
			handle = _open( nombre, tipo | _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY , _S_IREAD | _S_IWRITE );
            #endif
            if( handle > 0 && !SENAL_ERROR_INT24 ) {
                error = handle;
            }
            else {
                codigo_error = errno;
                if( ( int )codigo_error == -128 ) {
                    SET_ERROR_EN_RED();
                }
            }
            intentos++;
            salir = ( ARCH_RED( NRO_ARCHIVO ) && ERROR_EN_RED ) ? 1 : 0;
        }
        if( error < 0 || intentos > 1 || ( ERROR_EN_RED && ARCH_RED( NRO_ARCHIVO ) ) ) {
            GRABAR_ERROR( OP_CREAT, ( error < 0 ) ? 1 : 0, intentos, nombre, 0 );
        }
    }
    return ( error );
}
/***************************************************************************/
int error_disco()
/***************************************************************************/
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    INT24.ax = _AX;
    INT24.di = _DI;
    SENAL_ERROR_INT24 = 1;
    if( INT24.di == 12 ) {
        SET_ERROR_EN_RED();
    }
    hardretn( 5 );
    #endif
    return 1;
}
/***************************************************************************/
void SET_ERROR_EN_RED( void )
/***************************************************************************/
{
    if( !GET_ERROR_EN_RED() ) {
        _SET_ERROR_EN_RED();
    }
}
/***************************************************************************/
void _SET_ERROR_EN_RED( void )
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    ERROR_EN_RED = 1;
    timer_inicial = GET_TIME();
    #else
    if( !UTILIZA_DBROUTER ) {
        ERROR_EN_RED = 1;
        timer_inicial = GET_TIME();
    }
    else {
        ERROR_EN_RED = 0;
    }
    #endif
}
/***************************************************************************/
int GET_ERROR_EN_RED()
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    if( !ERROR_EN_RED ) {
        if( !PING() ) {
            _SET_ERROR_EN_RED();
        }
    }
    #else
    if( !UTILIZA_DBROUTER ) {
        if( !ERROR_EN_RED ) {
            if( !PING() ) {
                _SET_ERROR_EN_RED();
            }
        }
    }
    else {
        ERROR_EN_RED = 0;
    }
    #endif
    return ( ERROR_EN_RED );
}
/***************************************************************************/
int _GET_ERROR_EN_RED()
/***************************************************************************/
{
    return ( ERROR_EN_RED );
}
/***********************************************************************/
int COPIAR_ARCHIVO( char *nom_origen, char *nom_destino, int linea, char *file )
/***********************************************************************/
{
    #ifdef INVEL_L
    char *nom_origen1,*nom_destino1;
    nom_origen1 = ( char* )strlwr( nom_origen );
    nom_destino1 = ( char* )strlwr( nom_destino );
    return ( _COPIAR_ARCHIVO( 0, nom_origen1, nom_destino1, linea, file ) );
    #else
    return ( _COPIAR_ARCHIVO( 0, nom_origen, nom_destino, linea, file ) );
    #endif
}
/***********************************************************************/
int APPEND_ARCHIVO( char *nom_origen, char *nom_destino, int linea, char *file )
/***********************************************************************/
{
    #ifdef INVEL_L
    char *nom_origen1,*nom_destino1;
    nom_origen1 = ( char* )strlwr( nom_origen );
    nom_destino1 = ( char* )strlwr( nom_destino );
    return ( _COPIAR_ARCHIVO( 1, nom_origen1, nom_destino1, linea, file ) );
    #else
    return ( _COPIAR_ARCHIVO( 1, nom_origen, nom_destino, linea, file ) );
    #endif
}
#ifdef INVEL_L
/***********************************************************************/
int _COPIAR_ARCHIVO( int modo, char *nom_origen, char *nom_destino, int linea, char *file )
/***********************************************************************/
{
    int origen, destino, bufflen = BUFFER_COPIAR_ARCHIVO, len, wlen;
    char *buff = malloc( BUFFER_COPIAR_ARCHIVO );
    origen = open( nom_origen, O_RDONLY );
    if( modo == 1 ) {
        destino = open( nom_destino, O_WRONLY | O_APPEND );
    }
    else {
        destino = open( nom_destino, O_WRONLY | O_CREAT );
    }
    if( ( origen >= 0 ) && ( destino >= 0 ) ) {
        while( ( len = read( origen, buff, bufflen ) ) ) {
            wlen = write( destino, buff, len );
        }
        close( origen );
        close( destino );
        chmod( nom_destino, S_IWRITE | S_IREAD );
    }
    else {
        return -1;
    }
    free( buff );
    return 0;
}
#endif
#ifdef INVEL_W
/***********************************************************************/
int _COPIAR_ARCHIVO( int modo, char *nom_origen, char *nom_destino, int linea, char *file )
/***********************************************************************/
{
    int origen,destino,bloques,i,error,resto,ok;
    char *buffer;
    long tamano;
    char aux[100];
    #if !defined(INVEL_W) && !defined(INVEL_L)
    struct ftime t;
    unsigned attrib;    
    #endif
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        //esto es imposible
        error = 0;
    }
    else {
        buffer = malloc( BUFFER_COPIAR_ARCHIVO );
        if( !buffer ) {
            MENSAJE_STRING( S_NO_HAY_MEMORIA_PARA_COPIAR_ARCHIVO );
            GRABAR_ERROR( OP_MALLOC, 1, 1, NULL, 0 );
        }
        else {
            error = 0;
            #if defined(INVEL_W) || defined(INVEL_L)
            //00 Existence only 
            //02  Write permission 
            //04 Read permission 
            //06 Read and write permission 
            #ifdef INVEL_L
            ok = ( access( nom_destino, 0 ) );
            #else
            ok = ( _access( nom_destino, 0 ) );
            #endif
            if( ok != -1 ) {
                #ifdef INVEL_L
                ok = ( access( nom_destino, 6 ) );
                #else
                ok = ( _access( nom_destino, 6 ) );
                #endif
                if( ok != -1 ) {
                    //printf( "File has read and write permission\n" );
                }
                else {
                    /* Change back to read/write: */
                    #ifdef INVEL_L
                    ok = chmod( nom_destino, S_IWRITE ); 
                    #else
                    ok = _chmod( nom_destino, _S_IWRITE );
                    #endif
                    if( ok == -1 ) {
                        sprintf( aux, "No se encuentra archivo. No se pudo setear atributos %s",
                                 nom_destino );
                        GRABAR_LOG_SISTEMA( aux );
                        return -1;
                    }
                    else {
                        sprintf( aux, "Cambiando atributo a -r %s", nom_destino );
                        GRABAR_LOG_SISTEMA( aux );
                    }
                }
            }
            #else
            if( access( nom_destino, 0 ) == 0 ) {
                if( _dos_getfileattr( nom_destino, &attrib ) != 0 ) {
                    sprintf( aux, "No se pudo tomar atributos %s", nom_destino );
                    GRABAR_LOG_SISTEMA( aux );
                    return -1;
                }
                if( attrib & _A_RDONLY ) {
                    // esta en  read-only  , cambiando a read-write
                    attrib &= ~ _A_RDONLY;
                    sprintf( aux, "Cambiando atributo a -r %s", nom_destino );
                    GRABAR_LOG_SISTEMA( aux );
                }
                if( _dos_setfileattr( nom_destino, attrib ) != 0 ) {
                    // Unable to set file attributes
                    sprintf( aux, "No se pudo setear atributos %s", nom_destino );
                    GRABAR_LOG_SISTEMA( aux );
                    return -1;
                }
            }
            #endif
            if( !error ) {
                cop_arch.copiar_arch = 1;
                cop_arch.arch_activo = 1;
                #ifdef INVEL_L
                origen = open( nom_origen, O_RDWR, S_IREAD );
                #else
                origen = _open( nom_origen, _O_BINARY, _S_IREAD );
                #endif
                error = 1;
                if( origen > 0 ) {
                    cop_arch.arch_activo = 2;
                    /*---------- Si es COPIAR crea; si es APPEND abre ----------*/
                    if( modo == 0 ) {
                        #if defined(INVEL_W) || defined(INVEL_L)
                        destino = _CREAT( nom_destino, S_IFREG | S_IWRITE | S_IREAD, linea, file );
                        #else
                        destino = _CREAT( nom_destino, FA_ARCH, linea, file );
                        #endif
                    }
                    else {
                        #ifdef INVEL_L
                        destino = _OPEN( nom_destino, O_RDWR, linea, file );
                        #else
                        destino = _OPEN( nom_destino, O_RDWR | O_BINARY, linea, file );
                        #endif
                        LSEEK( destino, 0, SEEK_END, linea, file );
                    }
                    _CLOSE( destino, linea, file );
                    #ifdef INVEL_L
                    destino = open( nom_destino, O_RDWR | O_CREAT, S_IREAD | S_IWRITE ); 
                    #else
                    destino = _open( nom_destino, _O_RDWR | _O_CREAT | _O_BINARY,
                                     _S_IREAD | _S_IWRITE );
                    #endif
                    if( destino != -1 ) {
                        //if( destino > 0 ) {                  
                        /*-------- Copia bloques -------*/
                        tamano = filelength( origen );
                        bloques = ( int )( tamano / BUFFER_COPIAR_ARCHIVO );
                        resto = ( int )( tamano - bloques * BUFFER_COPIAR_ARCHIVO );
                        for( i = 0, error = 0;i < bloques && !error;i++ ) {
                            cop_arch.arch_activo = 1;
                            //error = _READ( origen, buffer, BUFFER_COPIAR_ARCHIVO, linea, file );
                            #ifdef INVEL_L
                            error = read( origen, buffer, BUFFER_COPIAR_ARCHIVO );
                            #else
                            error = _read( origen, buffer, BUFFER_COPIAR_ARCHIVO );
                            #endif
                            //if( !error ) {
                            //  cop_arch.arch_activo = 2;
                            if( error > 0 ) {
                                cop_arch.arch_activo = 2;
                                #if defined(INVEL_W) || defined(INVEL_L)
                                error = _WRITE( destino, buffer, error, linea, file );
                                #else
                                error = _WRITE( destino, buffer, BUFFER_COPIAR_ARCHIVO, linea,
                                                file );
                                #endif
                            }
                            error = 0;
                        }
                        /*------- Copia el resto ------*/
                        if( !error ) {
                            cop_arch.arch_activo = 1;
                            error = _READ( origen, buffer, resto, linea, file );
                            if( !error ) {
                                cop_arch.arch_activo = 2;
                                error = _WRITE( destino, buffer, resto, linea, file );
                            }
                        }
                        if( error ) {
                            MENSAJE_STRING( S_ERROR_AL_COPIAR_ARCHIVO );
                        }
                        cop_arch.arch_activo = 2;
                        /*--------- setea la fecha del archivo y cierra ---------*/
                        #ifdef INVEL_W
                        {
                            struct _utimbuf t;
                            struct _stat buf;
                            int result;
                            //getftime( origen, &t );
                            result = _fstat( origen, &buf );
                            t.actime = buf.st_atime;
                            t.modtime = buf.st_ctime;
                            //setftime( destino, &t );
                            _futime( destino, &t );
                        }
                        #endif
                        #ifdef INVEL_L
                        {
                            struct utimbuf t;
                            struct stat buf;
                            int result;
                            //getftime( origen, &t );
                            result = fstat( origen, &buf );
                            t.actime = buf.st_atime;
                            t.modtime = buf.st_ctime;
                            //setftime( destino, &t );
                            utime( nom_destino, &t );
                        }
                        #endif
                        #if !defined(INVEL_L) && !defined(INVEL_W)
                        getftime( origen, &t );
                        setftime( destino, &t );
                        #endif
                        _CLOSE( destino, linea, file );
                    }
                    else {
                        MENSAJE_STRING( S_ERROR_AL_CREAR_DESTINO );
                    }
                    cop_arch.arch_activo = 1;
                    _CLOSE( origen, linea, file );
                }
                else {
                    MENSAJE_STRING( S_ERROR_AL_ABRIR_ORIGEN );
                }
                free( buffer );
                cop_arch.copiar_arch = 0;
            }
        }
    }
    return ( error );
}
#endif
/***************************************************************************/
void GRABAR_ERROR( int operacion, int error, int intentos, char *nombre, int handle )
/***************************************************************************/
{
    int handle_log;
    char *op[] = {"","LEER","ESCRIBIR","LSEEK","CERRAR","ABRIR","CREAR","MALLOC"};
    char buffer[400],aux[150];
    char strArch[20];
    sprintf( strArch, "%sarchivos.log", PATH_LOGS );
    #if defined(INVEL_W) || defined(INVEL_L)   
    handle_log = open( strArch, O_RDWR );
    #else
    handle_log = _open( strArch, O_RDWR );
    #endif
    if( handle_log > 0 ) {
        /*-------------- Fecha y hora -------------*/
        DTOC( GET_FECHA_DOS(), buffer );
        strcat( buffer, " " );
        HTOAL( GET_HORA_COMPLETA_DOS(), aux );
        strcat( buffer, aux );
        /*--------------- Operacin ----------------*/
        strcat( buffer, " " );
        strcat( buffer, op[operacion] );
        /*---------------- Archivo -----------------*/
        strcat( buffer, " " );
        if( nombre ) {
            strcat( buffer, nombre );
        }
        else if( handle ) {
            // NOMBRE_ARCHIVO( handle, aux );
            // strcat( buffer, aux );
        }
        sprintf( aux, " [%i]", NRO_ARCHIVO );
        strcat( buffer, aux );
        /*----------- Durante la copia --------------*/
        if( cop_arch.copiar_arch ) {
            if( cop_arch.arch_activo == 1 ) {
                strcat( buffer, " (Origen de una copia)" );
            }
            else {
                strcat( buffer, " (Destino de una copia)" );
            }
        }
        /*------------ Error e intentos -------------*/
        if( !error ) {
            strcat( buffer, " [No hubo error]" );
        }
        sprintf( aux, " [En %i intentos]", intentos );
        strcat( buffer, aux );
        /*------------ Codigo de error --------------*/
        if( op_truncada ) {
            strcat( buffer, " [Datos insuficientes]" );
        }
        // if( codigo_error < _sys_nerr ) {
        #if defined(INVEL_W) || defined(INVEL_L)   
        sprintf( aux, " [Error n %i %s]", codigo_error,
                 ( ( int )codigo_error > 0 ) ? strerror( codigo_error ) : " " );
        #else
        sprintf( aux, " [Error n %i %s]", codigo_error,
                 ( ( int )codigo_error > 0 ) ? sys_errlist[codigo_error] : " " );
        #endif
        strcat( buffer, aux );
        // }
        /*----------------- Int 24 ------------------*/
        if( SENAL_ERROR_INT24 ) {
            sprintf( aux, " [Error en INT24 AX=%04x DI=%04x]", INT24.ax, INT24.di );
            strcat( buffer, aux );
        }
        /*--------------- Lnea y Fuente ------------*/
        sprintf( aux, " [Fuente: %s Lnea: %i]", FILE_, LINEA );
        strcat( buffer, aux );
        /*--------------- Fin de registro -----------*/
        strcat( buffer, "\r\n" );
        lseek( handle_log, 0L, SEEK_END );
        #if defined(INVEL_W) || defined(INVEL_L)   
        write( handle_log, buffer, strlen( buffer ) );
        #else
        _write( handle_log, buffer, strlen( buffer ) );
        #endif

    #if defined(INVEL_W) || defined(INVEL_L)   
    close( handle_log );
    #else
    _close( handle_log );
    #endif
	}
}
/***************************************************************************/
void VERIFICAR_REINTENTO()
/***************************************************************************/
{
    long timer;
    timer = GET_TIME();
    if( ( timer - timer_inicial ) / 18.2 >= SEGUNDOS_PARA_REINTENTO ) {
        timer_inicial = GET_TIME();
        #ifdef BLINKER
        _VERIFICAR_REINTENTO();
        #else
        ERROR_EN_RED = 0;
        #endif
    }
}
/***************************************************************************/
void _VERIFICAR_REINTENTO()
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    if( PING() ) {
        if( access( "connect.bat", 0 ) == 0 ) {
            MENSAJE_SIN_SONIDO_STRING( S_RECONECTANDO );
            GRABAR_LOG_SISTEMA( ST( S_RECONECTANDO ) );
            if( SWPRUNCMD( "connect.bat", 0, ".", "" ) ) {
                if( SWPERRLEV() == 0 ) {
                    ERROR_EN_RED = 0;
                    INICIALIZA_ARCHIVOS_RECONECCION();
                    GRABAR_LOG_SISTEMA( ST( S_RECONECCION_OK ) );
                    MENSAJE_STRING( S_RECONECCION_OK );
                }
            }
            else {
                GRABAR_LOG_SISTEMA( "RECONECCION fallo" );
            }
            INIT_SCANNER();
        }
        else {
            ERROR_EN_RED = 0;
        }
    }
    else {
        _SET_ERROR_EN_RED();
    }
    #endif
}
/***************************************************************************/
int ARCH_RED( int nro )
/***************************************************************************/
{
    int rta = 0;
    if( nro && ( nro >= 1000 || !ARCHIVO_LOCAL( nro ) ) ) {
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
int SET_FECHA_ARCHIVO( char *nom_archivo, unsigned fecha )
/***************************************************************************/
{
    int handle, rta = 0;
    #ifdef INVEL_L
    handle = _OPEN( nom_archivo, O_RDWR, __LINE__, __FILE__ );
    #else
    handle = _OPEN( nom_archivo, O_RDWR | O_BINARY, __LINE__, __FILE__ );
    #endif
    if( handle > 3 ) {
        _SET_FECHA_ARCHIVO( handle, fecha );
        _CLOSE( handle, __LINE__, __FILE__ );
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
void _SET_FECHA_ARCHIVO( int handle, unsigned fecha )
/***************************************************************************/
{
    #ifdef INVEL_W
    struct _utimbuf t;
    struct _stat buf;
    int result;
    time_t time1;
    struct tm time_check;
    result = _fstat( handle, &buf );
    time_check.tm_year = YEAR( fecha ) - 1900;
    time_check.tm_mon = MON( fecha ) - 1;
    time_check.tm_mday = DAY( fecha );
    time_check.tm_hour = 0;
    time_check.tm_min = 0;
    time_check.tm_sec = 1;
    time_check.tm_isdst = -1;
    time1 = mktime( &time_check );
    t.actime = buf.st_atime;
    t.modtime = time1;
    _futime( handle, &t );
    #endif
    #ifdef INVEL_L
    struct utimbuf t;
    struct stat buf;
    int result;
    time_t time1;
    struct tm time_check;
    result = fstat( handle, &buf );
    time_check.tm_year = YEAR( fecha ) - 1900;
    time_check.tm_mon = MON( fecha ) - 1;
    time_check.tm_mday = DAY( fecha );
    time_check.tm_hour = 0;
    time_check.tm_min = 0;
    time_check.tm_sec = 1;
    time_check.tm_isdst = -1;
    time1 = mktime( &time_check );
    t.actime = buf.st_atime;
    t.modtime = time1;
    //utime( handle, &t );
    #endif
    #if !defined(INVEL_L) && !defined(INVEL_W)
    struct ftime t;
    getftime( handle, &t );
    t.ft_day = DAY( fecha );
    t.ft_month = MON( fecha );
    t.ft_year = YEAR( fecha ) - 1980;
    setftime( handle, &t );
    #endif
}
/***********************************************************************/
int _OPEN_O_CREAT( char *nombre, int tipo, int linea, char *file )
/***********************************************************************/
{
    int handle;
    CAMBIAR_ATRIBUTO_ARCHIVOS( nombre );
    handle = _OPEN( nombre, tipo, linea, file );
    if( handle < 0 ) {
        handle = _CREAT( nombre, S_IFREG | S_IWRITE | S_IREAD, linea, file );
    }
    return ( handle );
}
/***********************************************************************/
int TEST_RED()
/***********************************************************************/
{
    int handle, ok = 1;
    if( !UTILIZA_DBROUTER ) {
        handle = OPEN( _FLAGS2_DAT, O_RDWR );
        if( handle > 0 ) {
            _CLOSE( handle, __LINE__, __FILE__ );
        }
        else {
            ok = 0;
        }
    }
    if( GET_ERROR_EN_RED() ) {
        ok = 0;
    }
    return ( ok );
}
/***********************************************************************/
int CAMBIAR_ATRIBUTO_ARCHIVOS( char *nom_destino )
/***********************************************************************/
{
    int error = 0, ok = 0;
    char aux[100];
    //00 Existence only 
    //02  Write permission 
    //04 Read permission 
    //06 Read and write permission 
    #ifdef INVEL_L
    ok = ( access( nom_destino, 0 ) );
    #else
    ok = ( _access( nom_destino, 0 ) );
    #endif
    if( ok != -1 ) {
        #ifdef INVEL_L
        ok = ( access( nom_destino, 6 ) );
        #else
        ok = ( _access( nom_destino, 6 ) );
        #endif
        if( ok != -1 ) {
            //printf( "File has read and write permission\n" );
        }
        else {
            /* Change back to read/write: */
            #ifdef INVEL_L
            ok = chmod( nom_destino, S_IWRITE ); 
            #else
            ok = _chmod( nom_destino, _S_IWRITE );
            #endif
            if( ok == -1 ) {
                sprintf( aux, "No se encuentra archivo. No se pudo setear atributos %s",
                         nom_destino );
                GRABAR_LOG_SISTEMA( aux );
                return( -1 );
            }
            else {
                sprintf( aux, "Cambiando atributo a -r %s", nom_destino );
                GRABAR_LOG_SISTEMA( aux );
            }
        }
    }
    return( 1 );
}

