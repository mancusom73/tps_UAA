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
#include <log.h> 
#include <recupero.h> 
#include <tkt.h> 
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#endif
/*****************************************************************************/
int GUARDAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes )
/*****************************************************************************/
{
    int error,handle;
    #ifdef INVEL_L
    handle = open( archivo, O_RDWR, __LINE__, __FILE__ );
    #else
    handle = _open( archivo, O_RDWR | O_BINARY/*, __LINE__, __FILE__*/ );
    #endif
    if( handle < 0 ) {
		#ifdef INVEL_L
		handle = _CREAT( archivo, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
		#else
		handle = _CREAT( archivo, /*S_IFREG |*/ S_IWRITE | S_IREAD /*| O_BINARY*/, __LINE__, __FILE__ );
		#endif
        if( handle > 0 ) {
            _CLOSE( handle, __LINE__, __FILE__ );
			#ifdef INVEL_L
            handle = _OPEN( archivo, O_WRONLY, __LINE__, __FILE__ );
			#else
			handle = _OPEN( archivo, O_WRONLY | O_BINARY, __LINE__, __FILE__ );
			#endif
        }
    }
    if( handle >= 0 ) {
        LSEEK( handle, 0, SEEK_END, __LINE__, __FILE__ );
        error = _WRITE( handle, estructura, bytes, __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
        return 1;
    }
    return -1;
}
/*****************************************************************************/
void ACTUALIZAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes, int indice )
/*****************************************************************************/
{
    int error,handle;
    #ifdef INVEL_L
    handle = open( archivo, O_RDWR, __LINE__, __FILE__ );
    #else
    handle = _open( archivo, O_RDWR | O_BINARY/* __LINE__, __FILE__*/ );
    #endif
    if( handle < 0 ) {
		#ifdef INVEL_L
    	handle = _CREAT( archivo, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
		#else
		handle = _CREAT( archivo, S_IFREG | S_IWRITE | S_IREAD | O_BINARY, __LINE__, __FILE__ );
		#endif
    }
    if( handle >= 0 ) {
        LSEEK( handle, ( indice * bytes ), SEEK_SET, __LINE__, __FILE__ );
        error = _WRITE( handle, estructura, bytes, __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
}
/*****************************************************************************/
int RECUPERAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes, int indice )
/*****************************************************************************/
{
    int error, handle, cant, rta = 1;
    

	NRO_ARCHIVO = 0;
    #ifdef INVEL_L
    handle = open( archivo, O_RDWR, __LINE__, __FILE__ );
    #else
    //handle = _open( archivo, O_RDWR | O_BINARY, __LINE__, __FILE__ );
    handle = _OPEN( archivo, O_RDWR | O_BINARY, __LINE__, __FILE__ );
    #endif
    if( handle >= 0 ) {
        cant = filelength( handle ) / bytes;
        if( indice < cant ) {
            LSEEK( handle, ( indice * bytes ), SEEK_SET, __LINE__, __FILE__ );
            error = _READ( handle, estructura, bytes, __LINE__, __FILE__ );
            rta = 0;
        }
        else {
            rta = 1;
        }
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    else {
        //sprintf( cad_aux, "No hay arch rec: %s", archivo );
        //GRABAR_LOG_SISTEMA( cad_aux );
        rta = 1;
    }
    return( rta );
}
/*****************************************************************************/
void BORRAR_ARCHIVO_REC( char *archivo )
/*****************************************************************************/
{
    _unlink( archivo );
}
/*****************************************************************************/
int CANTIDAD_REGISTROS_REC( char *archivo, int bytes )
/*****************************************************************************/
{
    int handle, cant = 0;
    #ifdef INVEL_L
    handle = open( archivo, O_RDWR, __LINE__, __FILE__ );
    #else
    handle = _open( archivo, O_RDWR | O_BINARY/*, __LINE__, __FILE__*/ );
    #endif
    if( handle >= 0 ) {
        cant = filelength( handle ) / bytes;
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return( cant );
}
/* Crea una copia del archivo .rec previamente existente anexando la extension .cpa.
 * @forzar indica que se debe sobreescribir si el archivo .rec.cpa existe
 * Devuelve: 2  si se sobreescribio debido a forzar == 1
 *           1  si se creo la copia de seguridad sin sobreescribir.
 *           -1 si no se altero la copia existente ya que forzar == 0
 *****************************************************************************/
int CREAR_COPIA_ARCHIVO_REC( char *archivo, int forzar )
/*****************************************************************************/
{
    char copia[255];
    int handle;
    strcpy( copia, archivo );
    strcat( copia, ".cpa" );
    if( access( copia, 0 ) != 0 ) {
        if( access( archivo, 0 ) == 0 ) {
            COPIAR_ARCHIVO( archivo, copia, __LINE__, __FILE__ );
        }
        else {
			#ifdef INVEL_L
        	handle = _CREAT( copia, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
			#else
			handle = _CREAT( copia, S_IFREG | S_IWRITE | S_IREAD | O_BINARY, __LINE__, __FILE__ );
			#endif
            if( handle > 0 ) {
                _CLOSE( handle, __LINE__, __FILE__ );
            }
        }
        return 1;
    }
    else if( forzar ) {
        //Ya existe la copia
        _unlink( copia );
        if( access( archivo, 0 ) == 0 ) {
            COPIAR_ARCHIVO( archivo, copia, __LINE__, __FILE__ );
        }
        else {
			#ifdef INVEL_L
			handle = _CREAT( copia, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
			#else
			handle = _CREAT( copia, S_IFREG | S_IWRITE | S_IREAD | O_BINARY, __LINE__, __FILE__ );
			#endif
            if( handle > 0 ) {
                _CLOSE( handle, __LINE__, __FILE__ );
            }
        }
        return 2;
    }
    else {
        //Ya existe la copia y forzar == 0
        return -1;
    }
}
/* Borra el archivo .rec.cpa
 *****************************************************************************/
void BORRAR_COPIA_ARCHIVO_REC( char *archivo )
/*****************************************************************************/
{
    char copia[255];
    strcpy( copia, archivo );
    strcat( copia, ".cpa" );
    _unlink( copia );
}
/* Sobreescribe el archivo .rec con su correspondiente copia .rec.cpa
 * Si la copia no existe, retorna -1
 *****************************************************************************/
int RESTAURAR_COPIA_ARCHIVO_REC( char *archivo, int borrar_copia )
/*****************************************************************************/
{
    char copia[255];
    int handle = 0;
    strcpy( copia, archivo );
    strcat( copia, ".cpa" );
    if( access( copia, 0 ) == 0 ) {
        _unlink( archivo );
        if( borrar_copia ) {
            rename( copia, archivo );
        }
        else {
            COPIAR_ARCHIVO( copia, archivo, __LINE__, __FILE__ );
        }
        return 1;
    }
    return -1;
}

