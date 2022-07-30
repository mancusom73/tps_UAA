#pragma pack(1)

/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_ASC
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
#include <asc.h> 
#include <cerear.h> 
#include <cio.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <dtoa.h> 
#include <dtoai.h> 
#include <ftoa.h> 
#include <ftoasp.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <htoc.h> 
#include <itoa.h> 
#include <pos_reg.h> 
#include <round.h> 
#include <tarjetas.h> 
#include <tkt.h> 

struct _mutual_asc
{
    char fecha[6];
    char caja[3];
    char marca[2];
    char importe[10];
    char lf;
};
struct _ticket_asc
{
    char fecha[6];
    char caja[3];
    char marca[2];
    char importe[10];
    char lf;
};
struct _totales_asc
{
    char fecha[6];
    char sucursal[2];
    char caja[3];
    char nro_z[4];
    char ticket_inicial[5];
    char ticket_final[5];
    char total[10];
    char imp_int[10];
    char gravado[10];
    char iva1[10];
    char exento[10];
    char iva2[10];
};
struct _sp_dep
{
    char importe[15];
    char cantidad[5];
};
struct _departa_asc
{
    char fecha[6];
    char sucursal[2];
    char caja[3];
    struct _sp_dep tot[15];
};
struct _cheques_asc
{
    char cliente[4];
    char nombre[30];
    char nro_cheque[8];
    char banco[3];
    char importe[8];
    char fecha_vto[6];
    char cod_post_cheque[4];
    char cod_hora_acredit[2];
    char fecha[6];
    char marca;
};
struct _tarjetas_asc
{
    char cod_tarjeta[2];
    char nro_tarjeta[16];
    char nro_cupon[8];
    char cuotas[2];
    char importe[10];
    char fecha[6];
    char autorizacion[10];
    char financia_tarjeta;
    char cajero[4];
    char caja[3];
    char nro_cupon_int[9];
    char moneda;
    char lf;
};
struct _cobros_asc
{
    char id[4];
    char sucursal[2];
    char caja[3];
    char fecha[6];
    char hora[4];
    char cajero[4];
    char comprobante[8];
    char cuenta[9];
    char importe[10];
    char lf;
};
struct _pagos_asc
{
    char id[4];
    char sucursal[2];
    char caja[3];
    char fecha[6];
    char hora[4];
    char cajero[4];
    char comprobante[4];
    char cuenta[8];
    char importe[10];
    char gravado[10];
    char exento[10];
    char iva[10];
};
/***********************************************************************/
static int GRABAR_ARCHIVO( int cod, int registro, char *buffer, int l_reg )
/***********************************************************************/
{
    int rta,handle;
    handle = OPEN( cod, O_WRONLY );
    POSICIONAR_REGISTRO( handle, registro, l_reg );
    rta = _WRITE( handle, buffer, l_reg, __LINE__, __FILE__ );
    _CLOSE( handle, __LINE__, __FILE__ );
    return ( !rta );
}
/****************************************************************************/
int ANULAR_REGISTRO( int cod, int registro, char *buffer, int l_reg, char *buf_cer, int l_buf_cer )
/****************************************************************************/
{
    int rta,handle;
    handle = OPEN( cod, O_RDWR );
    POSICIONAR_REGISTRO( handle, registro, l_reg );
    _READ( handle, buffer, l_reg, __LINE__, __FILE__ );
    memset( buf_cer, '0', l_buf_cer );
    POSICIONAR_REGISTRO( handle, registro, l_reg );
    rta = _WRITE( handle, buffer, l_reg, __LINE__, __FILE__ );
    _CLOSE( handle, __LINE__, __FILE__ );
    return ( !rta );
}
/***********************************************************************/
void GENERA_MUTUAL_ASC( int h )
/***********************************************************************/
{
    struct _mutual_asc mutual_asc;
    DTOA_INVERSA( GET_FECHA_DOS(), mutual_asc.fecha );
    ITOA( NRO_CAJA, mutual_asc.caja, 3 );
    ITOA( _PAGO_CUENTA( h ), mutual_asc.marca, 2 );
    FTOA_SIN_PUNTO( _PAGO_IMPORTE( h ), mutual_asc.importe, 6, 2, 0, 0 );
    CEREAR( ( char* )( &mutual_asc ), sizeof( mutual_asc ) );
    mutual_asc.lf = 10;
    GRABAR_ARCHIVO( _MUTUAL_ASC, 0, ( char* )&mutual_asc, sizeof( mutual_asc ) );
}
/***********************************************************************/
void GENERA_TICKET_ASC( int h )
/***********************************************************************/
{
    struct _ticket_asc ticket_asc;
    DTOA_INVERSA( GET_FECHA_DOS(), ticket_asc.fecha );
    ITOA( NRO_CAJA, ticket_asc.caja, 3 );
    ITOA( _PAGO_CUENTA( h ), ticket_asc.marca, 2 );
    FTOA_SIN_PUNTO( _PAGO_IMPORTE( h ), ticket_asc.importe, 6, 2, 0, 0 );
    CEREAR( ( char* )( &ticket_asc ), sizeof( ticket_asc ) );
    ticket_asc.lf = 10;
    GRABAR_ARCHIVO( _TICKET_ASC, 0, ( char* )&ticket_asc, sizeof( ticket_asc ) );
}
/*****************************************************************************/
void GENERA_TOTALES_ASC()
/*****************************************************************************/
{
    int h;
    double tot_ventas = 0;
    struct _totales_asc totales_asc;
    for( h = 0;h < 10;h++ ) {
        tot_ventas = ROUND( tot_ventas + _Z_VENTAS_IMPORTE( h ), 10, DECIMALES );
    }
    /*--------- TOTALES_ASC ---------*/
    DTOA_INVERSA( GET_FECHA_DOS(), totales_asc.fecha );
    ITOA( config.sucursal, totales_asc.sucursal, 2 );
    ITOA( NRO_CAJA, totales_asc.caja, 3 );
    ITOA( NRO_Z, totales_asc.nro_z, 4 );
    ITOA( 1, totales_asc.ticket_inicial, 5 );
    ITOA( NRO_TICKET, totales_asc.ticket_final, 5 );
    FTOA_SIN_PUNTO( tot_ventas, totales_asc.total, 8, 2, 0, 0 );
    FTOA_SIN_PUNTO( Z_IMP_INT, totales_asc.imp_int, 8, 2, 0, 0 );
    FTOA_SIN_PUNTO( Z_VENTAS_GRAVADAS, totales_asc.gravado, 8, 2, 0, 0 );
    FTOA_SIN_PUNTO( Z_IVA1, totales_asc.iva1, 8, 2, 0, 0 );
    FTOA_SIN_PUNTO( Z_IVA2, totales_asc.iva2, 8, 2, 0, 0 );
    FTOA_SIN_PUNTO( Z_VENTAS_EXENTAS, totales_asc.exento, 8, 2, 0, 0 );
    CEREAR( ( char* )&totales_asc, sizeof( totales_asc ) );
    GRABAR_ARCHIVO( _TOTALES_ASC, 1, ( char* )&totales_asc, sizeof( totales_asc ) );
}
/***********************************************************************/
void GENERA_DEPARTA_ASC()
/***********************************************************************/
{
    int h;
    struct _departa_asc departa_asc;
    DTOA( GET_FECHA_DOS(), departa_asc.fecha, NO );
    ITOA( config.sucursal, departa_asc.sucursal, 2 );
    ITOA( NRO_CAJA, departa_asc.caja, 3 );
    for( h = 0;h < 15;h++ ) {
        FTOA( _Z_DEP_IMP( h + 1 ), departa_asc.tot[h].importe, 12, 2, 0, 0 );
        ITOA( ( long )_Z_DEP_CAN( h + 1 ), departa_asc.tot[h].cantidad, 5 );
    } GRABAR_ARCHIVO( _DEPARTA_ASC, 1, ( char* )&departa_asc, sizeof( departa_asc ) );
}
/***********************************************************************/
void GENERA_CHEQUES_ASC( int h )
/***********************************************************************/
{
    struct _cheques_asc cheques_asc;
    DTOA_INVERSA( GET_FECHA_DOS(), cheques_asc.fecha );
    ITOA( _PAGO_NUMERO( h ), cheques_asc.nro_cheque, 8 );
    ITOA( _PAGO_BANCO( h ), cheques_asc.banco, 3 );
    DTOA_INVERSA( _PAGO_FECHA_VTO( h ), cheques_asc.fecha_vto );
    ITOA( _PAGO_COD_POST_CHEQ( h ), cheques_asc.cod_post_cheque, 4 );
    ITOA( _PAGO_COD_HORA_ACRE( h ), cheques_asc.cod_hora_acredit, 2 );
    memcpy( cheques_asc.cliente, "9996", 4 );
    FTOA_SIN_PUNTO( _PAGO_IMPORTE( h ), cheques_asc.importe, 6, 2, 0, 0 );
    CEREAR( ( char* )( &cheques_asc ), sizeof( cheques_asc ) );
    memset( &cheques_asc.nombre, ' ', 30 );
    cheques_asc.marca = 10;
    GRABAR_ARCHIVO( _CHEQUES_ASC, 0, ( char* )&cheques_asc, sizeof( cheques_asc ) );
}
/***********************************************************************/
void GENERA_TARJETAS_ASC( int h )
/***********************************************************************/
{
    char aux[15];
    int i,l;
    struct _tarjetas_asc tarjetas_asc;
    memset( tarjetas_asc.nro_tarjeta, ' ', sizeof( tarjetas_asc.nro_tarjeta ) );
    l = strlen( _PAGO_NRO_TARJETA( h ) );
    if( l > 16 ) {
        l = 16;
    }
    memcpy( &tarjetas_asc.nro_tarjeta[16 - l], _PAGO_NRO_TARJETA( h ), l );
    ITOA( _PAGO_TARJETA( h ) + 1, tarjetas_asc.cod_tarjeta, 2 );
    ITOA( _PAGO_NRO_CUPON1( h ), tarjetas_asc.nro_cupon, 8 );
    ITOA( _PAGO_NRO_CUPON_INT1( h ), tarjetas_asc.nro_cupon_int, 9 );
    ITOA( VAR_TARJETAS_CUOTAS, tarjetas_asc.cuotas, 2 );
    FTOA( _PAGO_IMPORTE( h ), aux, 8, 2, 0, 0 );
    memcpy( &tarjetas_asc.importe, aux, 8 );
    memcpy( &tarjetas_asc.importe[8], &aux[9], 2 );
    DTOA_INVERSA( GET_FECHA_DOS(), tarjetas_asc.fecha );
    ITOA( NRO_CAJERO, tarjetas_asc.cajero, 4 );
    ITOA( NRO_CAJA, tarjetas_asc.caja, 3 );
    CEREAR( ( char* )( &tarjetas_asc ), sizeof( tarjetas_asc ) );
    strcpy( tarjetas_asc.autorizacion, VAR_TARJETAS_AUTORIZACION );
    for( i = strlen( tarjetas_asc.autorizacion );i < 10;i++ ) {
        tarjetas_asc.autorizacion[i] = ' ';
    }
    tarjetas_asc.financia_tarjeta = ( VAR_TARJETAS_FINANCIA_TARJETA ) ? 'S' : 'N';
    tarjetas_asc.moneda = MONEDA_EXTENDIDA() + '0';
    tarjetas_asc.lf = 10;
    GRABAR_ARCHIVO( _TARJETAS_ASC, 0, ( char* )&tarjetas_asc, sizeof( tarjetas_asc ) );
}
/***********************************************************************/
void GENERA_COBROS_ASC( int id, struct _datos_cobro *datos_cobro )
/***********************************************************************/
{
    struct _cobros_asc cobros_asc;
    ITOA( id, cobros_asc.id, 4 );
    ITOA( config.sucursal, cobros_asc.sucursal, 2 );
    ITOA( NRO_CAJA, cobros_asc.caja, 3 );
    ITOA( NRO_CAJERO, cobros_asc.cajero, 4 );
    ITOA( comprobante, cobros_asc.comprobante, 8 );
    ITOA( cuenta, cobros_asc.cuenta, 9 );
    DTOA_INVERSA( GET_FECHA_DOS(), cobros_asc.fecha );
    HTOC( GET_HORA_DOS(), cobros_asc.hora );
    FTOA_SIN_PUNTO( importe, cobros_asc.importe, 8, 2, 0, 0 );
    cobros_asc.lf = 10;
    GRABAR_ARCHIVO( _COBROS_ASC, 0, ( char* )&cobros_asc, sizeof( cobros_asc ) );
}
/***********************************************************************/
void GENERA_PAGOS_ASC( int id, long comprobante, long cuenta, double importe, double gravado,
                       double exento, double iva )
/***********************************************************************/
{
    struct _pagos_asc pagos_asc;
    ITOA( id, pagos_asc.id, 4 );
    ITOA( config.sucursal, pagos_asc.sucursal, 2 );
    ITOA( NRO_CAJA, pagos_asc.caja, 3 );
    ITOA( NRO_CAJERO, pagos_asc.cajero, 4 );
    ITOA( comprobante, pagos_asc.comprobante, 4 );
    ITOA( cuenta, pagos_asc.cuenta, 8 );
    DTOA_INVERSA( GET_FECHA_DOS(), pagos_asc.fecha );
    HTOC( GET_HORA_DOS(), pagos_asc.hora );
    FTOA_SIN_PUNTO( importe, pagos_asc.importe, 8, 2, 0, 0 );
    FTOA_SIN_PUNTO( gravado, pagos_asc.gravado, 8, 2, 0, 0 );
    FTOA_SIN_PUNTO( exento, pagos_asc.exento, 8, 2, 0, 0 );
    FTOA_SIN_PUNTO( iva, pagos_asc.iva, 8, 2, 0, 0 );
    GRABAR_ARCHIVO( _PAGOS_ASC, 0, ( char* )&pagos_asc, sizeof( pagos_asc ) );
}
/***********************************************************************/
void ANULA_CHEQUES_ASC( int reg )
/***********************************************************************/
{
    struct _cheques_asc cheques_asc;
    ANULAR_REGISTRO( _CHEQUES_ASC, reg, ( char* )&cheques_asc, sizeof( cheques_asc ),
                     ( char* )&cheques_asc, sizeof( cheques_asc ) - 1 );
}
/***********************************************************************/
void ANULA_TARJETAS_ASC( int reg )
/***********************************************************************/
{
    struct _tarjetas_asc tarjetas_asc;
    ANULAR_REGISTRO( _TARJETAS_ASC, reg, ( char* )&tarjetas_asc, sizeof( tarjetas_asc ),
                     ( char* )&tarjetas_asc.importe, sizeof( tarjetas_asc.importe ) );
}
#endif                                            /*  */

