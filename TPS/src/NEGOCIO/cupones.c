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
#include <cio.h> 
#include <cr.h>  
#include <cupones.h> 
#include <dbrouter.h> 
#include <export.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <pos_reg.h> 
#include <tarjetas.h> 
#include <tkt.h> 
#include <cmemory.h>
#include <aarch.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
struct _cupon
{
    char marca;
    char nro_tarjeta[20];
    unsigned short fecha;
    char nro_aut[10];
    unsigned long nro_cupon1;
    unsigned long nro_cupon2;
    double importe;
    char cuotas;
    char moneda;                             /* 0-Pesos  1-Dolar */
    char tipo;
    char nro_comercio[15];
    short int cajero;
    BINARY status;
    /* 0000 0001 lectura banda       */
    /* 0000 0010 fuente aut. on-line */
    /* 0000 0100 financia tarjeta    */
    /* 0000 1000 Boletin vencido     */
    /* 0001 0000 Autoriz en boletn */
    /* 0010 0000 Captura on line     */
    /* 1000 0000 Anulado             */
    int hora;
    unsigned long nro_cupon_int;
    unsigned short nro_boletin;
    char tabla[6];
    unsigned short lote;
    float anticipo;
    unsigned short fecha_vto;
    long nro_ticket;
    unsigned short sucursal;
    unsigned short fecha_proceso;
};
int LEER_CUPONES( int registro, struct _cupon *cupon );
int GRABAR_CUPONES( int registro, struct _cupon *cupon );
/***********************************************************************/
int ABRIR_CUPONES()
/***********************************************************************/
{
    return ( OPEN_O_CREAT( _CUPON_ZZZ, O_RDWR ) );
}
/***********************************************************************/
void CERRAR_CUPONES( int handle )
/***********************************************************************/
{
    if( UTILIZAR_DIA_OPERATIVO ) {
        _SET_FECHA_ARCHIVO( handle, NRO_DIA_OPERATIVO );
    }
    _CLOSE( handle, __LINE__, __FILE__ );
}
/***********************************************************************/
void CREAR_CUPONES()
/***********************************************************************/
{
    int handle;
    /*----------------- hace bakup -----------------------*/
    _FECHA_ARCHIVO = GET_FECHA_BACKUP();
    COPY( _CUPON_ZZZ, _CUPON_BAK );
    if( UTILIZAR_DIA_OPERATIVO ) {
        SETFTIME( _CUPON_BAK, NRO_DIA_OPERATIVO );
    }
    LOG_EXISTE_ARCHIVO_COD( _CUPON_BAK );
    /*----------------- crea el archivo ---------------------*/
    handle = CREAT_( _CUPON_ZZZ, S_IFREG );
    CERRAR_CUPONES( handle );
}
/***********************************************************************/
int LEER_CUPONES( int registro, struct _cupon *cupon )
/***********************************************************************/
{
    int rta,handle;
    handle = ABRIR_CUPONES();
    POSICIONAR_REGISTRO( handle, registro, sizeof( struct _cupon ) );
    rta = _READ( handle, ( char* )cupon, sizeof( struct _cupon ), __LINE__, __FILE__ );
    CERRAR_CUPONES( handle );
    return ( !rta );
}
/***********************************************************************/
int GRABAR_CUPONES( int registro, struct _cupon *cupon )
/***********************************************************************/
{
    int rta,handle;
    handle = ABRIR_CUPONES();
    POSICIONAR_REGISTRO( handle, registro, sizeof( struct _cupon ) );
    rta = _WRITE( handle, ( char* )cupon, sizeof( struct _cupon ), __LINE__, __FILE__ );
    CERRAR_CUPONES( handle );
    return ( !rta );
}
/***********************************************************************/
void COPIAR_CUPONES()
/***********************************************************************/
{
    LOG_EXISTE_ARCHIVO_COD( _CUPON_ZZZ );
    COPY( _CUPON_ZZZ, _CUPON_REMOTO );
    if( UTILIZAR_DIA_OPERATIVO ) {
        SETFTIME( _CUPON_REMOTO, NRO_DIA_OPERATIVO );
    }
    LOG_EXISTE_ARCHIVO_COD( _CUPON_REMOTO );
}
/***********************************************************************/
void GENERAR_CUPON( int p )
/***********************************************************************/
{
    struct _cupon cupon;
    if( !( VAR_TARJETAS_ON_LINE && GENERAR_CUPONES_SI_ON_LINE == 0 ) ) {
        memset( cupon.nro_tarjeta, 0, sizeof( cupon.nro_tarjeta ) );
        strcpy( cupon.nro_tarjeta, _PAGO_NRO_TARJETA( p ) );
        memcpy( cupon.nro_aut, _PAGO_AUTO_TARJETA( p ), sizeof( cupon.nro_aut ) );
        memcpy( cupon.nro_comercio, _PAGO_BUFFER( p ), 15 );
        strcpy( cupon.tabla, _PAGO_TABLA( p ) );
        cupon.marca = _PAGO_TARJETA( p ) + 1;
        cupon.fecha = GET_FECHA_DOS();
        cupon.nro_cupon1 = _PAGO_NRO_CUPON_INT1( p );
        cupon.nro_cupon2 = 0;
        cupon.nro_cupon_int = _PAGO_NRO_CUPON1( p );
        cupon.importe = _PAGO_IMPORTE( p );
        cupon.anticipo = 0;
        if( VAR_TARJETAS_IMPORTE_ENTREGA ) {
            cupon.importe += VAR_TARJETAS_IMPORTE_ENTREGA;
            cupon.anticipo = ( float )VAR_TARJETAS_IMPORTE_ENTREGA;
        }
        cupon.cuotas = _PAGO_CUOTAS( p );
        cupon.moneda = MONEDA_EXTENDIDA();
        cupon.tipo = 0;
        cupon.cajero = NRO_CAJERO;
        cupon.hora = GET_HORA_DOS();
        cupon.status = CUPON_CALCULAR_STATUS();
        cupon.nro_boletin = _PAGO_NRO_BOLETIN( p );
        cupon.lote = VAR_TARJETAS_LOTE;
        cupon.fecha_vto = GET_FECHA_VTO_PRIVADA(  ) ;
        cupon.nro_ticket = NRO_TICKET;
        cupon.sucursal = config.sucursal;
        cupon.fecha_proceso = 0;
        if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
            cupon.importe *= -1;
        }
        if( TARJ[( int )VAR_TARJETAS_TARJETA].diferimiento ) {
            cupon.fecha_proceso = VAR_TARJETAS_PRIMER_VTO;
        }
        else {
            cupon.fecha_proceso = 0;
        }
        GRABAR_CUPONES( 0, &cupon );
    }
}
/***********************************************************************/
int COPIAR_CUPONES_ANTERIOR()
/***********************************************************************/
{
    int rta = 0;
    _FECHA_ARCHIVO = LEE_FECHA( _FECHA_CUP );
    if( _FECHA_ARCHIVO ) {
        rta = !COPY( _CUPON_BAK, _CUPON_REMOTO );
    }
    return ( rta );
}
/***********************************************************************/
int CUPON_CALCULAR_STATUS()
/***********************************************************************/
{
    int status = 0;
    if( VAR_TARJETAS_LECTURA_BANDA == 1 ) {
        status |= 1;
    }
    if( VAR_TARJETAS_ON_LINE ) {
        status |= 2;
    }
    if( VAR_TARJETAS_FINANCIA_TARJETA ) {
        status |= 4;
    }
    if( VAR_TARJETAS_BOLETIN_VENCIDO ) {
        status |= 8;
    }
    if( VAR_TARJETAS_AUT_EN_BOLETIN ) {
        status |= 16;
    }
    if( VAR_TARJETAS_CAPTURA_ON_LINE ) {
        status |= 32;
    }
    return ( status );
}
/***********************************************************************/
void ANULAR_CUPON( int registro )
/***********************************************************************/
{
    struct _cupon cupon;
    LEER_CUPONES( registro, &cupon );
    cupon.status |= 0x80;
    GRABAR_CUPONES( registro, &cupon );
}

