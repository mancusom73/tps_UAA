#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <archivos.h> 
#include <btrv.h> 
#include <cio.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <dbrouter.h> 
#include <novios.h> 
#include <tkt.h> 
#include <transfer.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_NOVIOS
/*****************************************************************************/
void ENVIAR_TRANSACCIONES_NOVIOS()
/*****************************************************************************/
{
    int handle,h;
    double signo;
    struct
    {
        int cant_registros;
        long nro_secuencia;
    } cab_novios;
    struct
    {
        char cod_barra[17];
        unsigned long cod_interno;
        float cantidad;
        double precio_total;
    } datos_novios;
    /*------------------------ Crea archivo --------------------------*/
    handle = CREAT( _NOVIOS_BIN, S_IFREG );
    if( handle > 0 ) {
        cab_novios.cant_registros = RAM_P_TICKET;
        cab_novios.nro_secuencia = NRO_EVENTO - 1;
        _WRITE( handle, ( char* )&cab_novios, sizeof( cab_novios ), __LINE__, __FILE__ );
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            if( BUSCA_CODIGO_ESPECIAL( _TICKET_COD_ESPECIAL( h ) ) ) {
                signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
                memcpy( datos_novios.cod_barra, articulo.cod_barra, sizeof( articulo.cod_barra ) );
                datos_novios.cod_interno = _TICKET_COD_ESPECIAL( h );
                datos_novios.cantidad = ( float )( _TICKET_CANTIDAD( h ) * signo );
                datos_novios.precio_total = fabs( _TICKET_CANTIDAD( h ) ) * _TICKET_PRECIO( h )
                                          * signo;
                _WRITE( handle, ( char* )&datos_novios, sizeof( datos_novios ), __LINE__,
                        __FILE__ );
            }
        }
        _CLOSE( handle, __LINE__, __FILE__ );
        GRABA_BANDERA_TRANSFER( ( long )_FLAG_NOVIOS );
    }
}
#endif

