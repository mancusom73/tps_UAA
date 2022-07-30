#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <archivos.h> 
#include <cio.h> 
#include <cr.h> 
#include <tkt.h> 
#include <transfer.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_NOVIOS
/*****************************************************************************/
long LEE_BANDERA_TRANSFER()
/*****************************************************************************/
{
    char nombre[80];
    int handle;
    long bandera = -1;
    NOM_ARCHIVO( _TRANSFER_BIN, nombre );
    handle = _OPEN( nombre, O_RDWR | O_BINARY, __LINE__, __FILE__ );
    if( handle > 3 ) {
        /*-------------- Lee bandera de la caja ----------------*/
        LSEEK( handle, ( long )( NRO_CAJA * 4 ), SEEK_SET, __LINE__, __FILE__ );
        _READ( handle, ( char* )&bandera, 4, __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return ( bandera );
}
/*****************************************************************************/
int GRABA_BANDERA_TRANSFER( long bandera )
/*****************************************************************************/
{
    int rta = -1;
    long _bandera;
    _bandera = LEE_BANDERA_TRANSFER();
    if( _bandera >= 0 ) {
        _bandera |= bandera;
        rta = _GRABA_BANDERA_TRANSFER( _bandera );
    }
    return ( rta );
}
/*****************************************************************************/
int _GRABA_BANDERA_TRANSFER( long bandera )
/*****************************************************************************/
{
    char nombre[80];
    int rta = -1, handle;
    //modo = _MODO_EJECUCION;
    //_MODO_EJECUCION = OPERACION;
    NOM_ARCHIVO( _TRANSFER_BIN, nombre );
    handle = _OPEN( nombre, O_RDWR | O_BINARY, __LINE__, __FILE__ );
    if( handle > 3 ) {
        LSEEK( handle, ( long )( NRO_CAJA * 4 ), SEEK_SET, __LINE__, __FILE__ );
        rta = _WRITE( handle, ( char* )&bandera, 4, __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    //_MODO_EJECUCION = modo;
    return ( rta );
}
#endif

