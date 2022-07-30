#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dd.h> 
#include <log.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <ram.h> 
#include <rnv.h> 
#include <bridge.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
int __VAL_IMPORTE( int cod, int h, double desde, double hasta );
int VERIFICAR_E_NRO();
int VERIFICAR_E_ACU( int est );
int _VAL_IMPORTE( int cod, int h );
int _VAL_CANTIDAD( int cod, int h );
int _VAL_IMPORTE_Y_CANTIDAD( int cod_imp, int cod_can, int h );
int __VAL_IMPORTE( int cod, int h, double desde, double hasta );
extern int GET_TYPE( int cod );
/*****************************************************************************/
void SET_RAM( int estado )
/*****************************************************************************/
{
    int ok = 1, c = 0;
    if( !_RNV_VIRTUAL ) {
        SET_RNV( estado );
        if( estado == ON ) {
            do {
                if( _CONTROLAR_RNV ) {
                    if( NRO_CAJA && atoi( NRO_NOM_CAJA ) == NRO_CAJA ) {
                        ok = 1;
                    }
                    else {
                        c++;
                        ok = 0;
                        if( c > 10 ) {
                            MENSAJE( "FALLOS DETECTADOS EN LA RAM NO VOLATIL" );
                        }
                    }
                }
            }
            while( !ok );
        }
    }
}
/*****************************************************************************/
int GUARDAR_E_ACU( FILE *f, int est )
/*****************************************************************************/
{
    int r,h,ini_est;
    ini_est = _APUNTAR_STRUCT( est );
    for( h = 0;h < 10;h++ ) {
        r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_ventas_importe ),
                                     EST_VAR( ini_est + o_x_ventas_cantidad ), h );
        fprintf( f, "o_x_ventas_importe ( %d ): %d\n", h, r );
        r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_retiros_importe ),
                                     EST_VAR( ini_est + o_x_retiros_cantidad ), h ); 
        fprintf( f, "o_x_retiros_importe ( %d ): %d\n", h, r );
        r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_cobros_importe ),
                                     EST_VAR( ini_est + o_x_cobros_cantidad ), h );
        fprintf( f, "o_x_cobros_importe ( %d ): %d\n", h, r );
    }
    for( h = 0;h < 10;h++ ) {
        r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_dep_imp ), h );
        fprintf( f, "o_x_dep_imp ( %d ): %d\n", h, r );
        r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_dep_can ), h );
        fprintf( f, "o_x_dep_can ( %d ): %d\n", h, r );
    }
    r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_adelantos_importe ),
                                 EST_VAR( ini_est + o_x_adelantos_cantidad ), 0 );
    fprintf( f, "o_x_adelantos_importe: %d\n", r );
    r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_intereses_importe ),
                                 EST_VAR( ini_est + o_x_intereses_cantidad ), 0 );
    fprintf( f, "o_x_intereses_importe: %d\n", r );
    r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_fondo_fijo_importe ),
                                 EST_VAR( ini_est + o_x_fondo_fijo_cantidad ), 0 );
    fprintf( f, "o_x_fondo_fijo_importe: %d\n", r );
    r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_anulaciones_importe ),
                                 EST_VAR( ini_est + o_x_anulaciones_cantidad ), 0 );
    fprintf( f, "o_x_anulaciones_importe: %d\n", r );
    r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_dev_envases_importe ), 0 );
    fprintf( f, "o_x_dev_envases_importe: %d\n", r );
    r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_devoluciones_importe ),
                                 EST_VAR( ini_est + o_x_devoluciones_cantidad ), 0 );
    fprintf( f, "o_x_devoluciones_importe: %d\n", r );
    r = _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_retiro_fondo_importe ),
                                 EST_VAR( ini_est + o_x_retiro_fondo_cantidad ), 0 );
    fprintf( f, "o_x_retiro_fondo_importe: %d\n", r );
    r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_ventas_gravadas ), 0 );
    fprintf( f, "o_x_ventas_gravadas: %d\n", r );
    r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_ventas_exentas ), 0 );
    fprintf( f, "o_x_ventas_exentas: %d\n", r );
    r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_imp_int ), 0 );
    fprintf( f, "o_x_imp_int: %d\n", r );
    r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_tot_dinero ), 0 );
    fprintf( f, "o_x_tot_dinero: %d\n", r );
    r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_iva1 ), 0 );
    fprintf( f, "o_x_iva1: %d\n", r );
    r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_iva2 ), 0 );
    fprintf( f, "o_x_iva2: %d\n", r );
    for( h = 0;h < 4;h++ ) {
        r = _VAL_IMPORTE( EST_VAR( ini_est + o_x_impuestos ), h );
        fprintf( f, "o_x_impuestos ( %d ): %d\n", h, r );
    }
    return 1;
}
/*****************************************************************************/
int GUARDAR_ESTADO_RAM()
/*****************************************************************************/
{
    // Guarda en el archivo RNV.log los valores de todas
    // las variables de la RNV
    int error = 0, h;
    FILE *f;
    f = fopen( "RNV.log", "wt" );
    // VERIFICAR_E_NRO
    fprintf( f, "NRO_CAJA: %d\n", NRO_CAJA );
    for( h = 0;h < 3;h++ ) {
        fprintf( f, "_NRO_NOM_CAJA ( %d ): %d\n", h, _NRO_NOM_CAJA( h ) );
    }
    fprintf( f, "NRO_CAJA: %d\n", NRO_CAJA );
    fprintf( f, "NRO_CAJA( 3 ): %d\n", _NRO_NOM_CAJA( 3 ) );
    fprintf( f, "NRO_INTERESES: %d\n", NRO_INTERESES );
    fprintf( f, "NRO_Z: %d\n", NRO_Z );
    fprintf( f, "NRO_Y: %d\n", NRO_Y );
    fprintf( f, "NRO_TICKET: %d\n", NRO_TICKET );
    fprintf( f, "NRO_CAJERO: %d\n", NRO_CAJERO );
    fprintf( f, "NRO_CAJERO_ANT: %d\n", NRO_CAJERO_ANT );
    fprintf( f, "NRO_MODO: %c\n", NRO_MODO );
    for( h = 0;h < 20;h++ ) {
        fprintf( f, "_NRO_CUPONES( %d ): %d\n", h, _NRO_CUPONES( h ) );
    }
    GUARDAR_E_ACU( f, _x );
    GUARDAR_E_ACU( f, _z );
    GUARDAR_E_ACU( f, _z_ant );
    fclose( f );
    return 1;
}
/*****************************************************************************/
int VALIDAR_RAM()
/*****************************************************************************/
{
    char mensaje[80];
    int error = 0;
    if( VERIFICAR_RAM ) {
        if( atoi( NRO_NOM_CAJA ) != NRO_CAJA ) {
            _snprintf(mensaje, sizeof(mensaje)-1, "ERROR EN RNV: %i %s", NRO_CAJA, NRO_NOM_CAJA , LOG_ERRORES,2);
            GRABAR_LOG_SISTEMA( mensaje, LOG_VENTAS,1);
            ABORT( mensaje );
        }
        error = VERIFICAR_E_NRO();
        if( error ) {
            GRABAR_LOG_SISTEMA( "HAY ERROR EN RAM: Estructura NRO" , LOG_ERRORES,1);
        }
        error += VERIFICAR_E_ACU( _x );
        if( error ) {
            GRABAR_LOG_SISTEMA( "HAY ERROR EN RAM: Estructura X" , LOG_ERRORES,1);
        }
        error += VERIFICAR_E_ACU( _z );
        if( error ) {
            GRABAR_LOG_SISTEMA( "HAY ERROR EN RAM: Estructura Z" , LOG_ERRORES,1);
        }
        error += VERIFICAR_E_ACU( _z_ant );
        if( error ) {
            GRABAR_LOG_SISTEMA( "HAY ERROR EN RAM: Estructura Z_ANT", LOG_ERRORES,1 );
        }
        if( error ) {
            MENSAJE_STRING( S_HAY_VALORES_ERRONEOS_EN_LA_RAM_NO_VOLATIL );
        }
    }
    return ( error );
}
/*****************************************************************************/
int VERIFICAR_E_NRO()
/*****************************************************************************/
{
    int error = 0, h;
    if( NRO_CAJA < 0 || NRO_CAJA > 999 ) {
        error = 1;
    }
    for( h = 0;h < 3;h++ ) {
        if( _NRO_NOM_CAJA( h ) < '0' || _NRO_NOM_CAJA( h ) > '9' ) {
            error = 1;
        }
    }
    if( _NRO_NOM_CAJA( 3 ) != 0 ) {
        error = 1;
    }
    if( NRO_INTERESES < -100.0 || NRO_INTERESES > 100.0 ) {
        error = 1;
    }
    if( NRO_Z < 0 || NRO_Z > 99999999L ) {
        error = 1;
    }
    if( NRO_Y < 0 || NRO_Y > 99999999L ) {
        error = 1;
    }
    if( MODO_NEGOCIO != SERVICIOS && ( NRO_TICKET < 0 || NRO_TICKET > 999999L ) ) {
        error = 1;
    }
    if( MODO_NEGOCIO == SERVICIOS && ( NRO_TICKET < 0 || NRO_TICKET > 99999999L ) ) {
        error = 1;
    }
    if( NRO_CAJERO < -1 || NRO_CAJERO > 9999 ) {
        error = 1;
    }
    if( NRO_CAJERO_ANT < -1 || NRO_CAJERO_ANT > 9999L ) {
        error = 1;
    }
    if( NRO_MODO != 'Z' && NRO_MODO != 'Y' && NRO_MODO != 'X' && NRO_MODO != 0 ) {
        error = 1;
    }
    for( h = 0;h < 20;h++ ) {
        if( _NRO_CUPONES( h ) < 0 || _NRO_CUPONES( h ) > 9999 ) {
            error = 1;
        }
    }
    return ( error );
}
/*****************************************************************************/
int VERIFICAR_E_ACU( int est )
/*****************************************************************************/
{
    int error = 0, h, ini_est, i;
    ini_est = _APUNTAR_STRUCT( est );
    for( h = 0;h < 10;h++ ) {
        error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_ventas_importe ),
                                          EST_VAR( ini_est + o_x_ventas_cantidad ), h );
        error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_retiros_importe ),
                                          EST_VAR( ini_est + o_x_retiros_cantidad ), h );
        error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_cobros_importe ),
                                          EST_VAR( ini_est + o_x_cobros_cantidad ), h );
    }
    for( h = 0;h < 10;h++ ) {
        error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_dep_imp ), h );
        error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_dep_can ), h );
    }
    error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_adelantos_importe ),
                                      EST_VAR( ini_est + o_x_adelantos_cantidad ), 0 );
    error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_intereses_importe ),
                                      EST_VAR( ini_est + o_x_intereses_cantidad ), 0 );
    error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_fondo_fijo_importe ),
                                      EST_VAR( ini_est + o_x_fondo_fijo_cantidad ), 0 );
    error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_anulaciones_importe ),
                                      EST_VAR( ini_est + o_x_anulaciones_cantidad ), 0 );
    //error +=
    //  _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_dev_envases_importe ),
    //  EST_VAR( ini_est + o_x_dev_envases_cantidad ), 0 );
    error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_dev_envases_importe ), 0 );
    error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_devoluciones_importe ),
                                      EST_VAR( ini_est + o_x_devoluciones_cantidad ), 0 );
    error += _VAL_IMPORTE_Y_CANTIDAD( EST_VAR( ini_est + o_x_retiro_fondo_importe ),
                                      EST_VAR( ini_est + o_x_retiro_fondo_cantidad ), 0 );
    error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_ventas_gravadas ), 0 );
    error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_ventas_exentas ), 0 );
    error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_imp_int ), 0 );
    error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_tot_dinero ), 0 );
    error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_iva1 ), 0 );
    error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_iva2 ), 0 );
    for( i = 0;i < 4;i++ ) {
        error += _VAL_IMPORTE( EST_VAR( ini_est + o_x_impuestos ), 0 );
    }
    return ( error );
}
/*****************************************************************************/
int _VAL_IMPORTE( int cod, int h )
/*****************************************************************************/
{
    double limite;
    limite = 999999999.99;
    return ( __VAL_IMPORTE( cod, h, -limite, limite ) );
}
/*****************************************************************************/
int _VAL_CANTIDAD( int cod, int h )
/*****************************************************************************/
{
    int error = 0;/*, cantidad = 0; */
    double cantidad = 0;
    switch( GET_TYPE( cod ) ) {
        case 1:
            cantidad = ( double )_GET_MEMORY_CHAR( cod, h );
            break;
        case 2:
            cantidad = ( double )_GET_MEMORY_INT( cod, h );
            break;
        case 3:
            cantidad = ( double )_GET_MEMORY_FLOAT( cod, h );
            break;
        case 4:
            cantidad = ( double )_GET_MEMORY_DOUBLE( cod, h );
            break;
        case 8:
            cantidad = ( double )_GET_MEMORY_LONG( cod, h );
            break;
        case 9:
            cantidad = ( double )_GET_MEMORY_UNSIGNED( cod, h );
            break;
    }
    //cantidad = _GET_MEMORY_DOUBLE( cod, h );
    if( cantidad < -9999 || cantidad > 9999 ) {
        error = 1;
    }
    return ( error );
}
/*****************************************************************************/
int _VAL_IMPORTE_Y_CANTIDAD( int cod_imp, int cod_can, int h )
/*****************************************************************************/
{
    return ( _VAL_IMPORTE( cod_imp, h ) + _VAL_CANTIDAD( cod_can, h ) );
}
/*****************************************************************************/
int __VAL_IMPORTE( int cod, int h, double desde, double hasta )
/*****************************************************************************/
{
    int error = 0;
    double importe = 0;

    switch( GET_TYPE( cod ) ) {
        case 1:
            importe = ( double )_GET_MEMORY_CHAR( cod, h );
            break;
        case 2:
            importe = ( double )_GET_MEMORY_INT( cod, h );
            break;
        case 3:
            importe = ( double )_GET_MEMORY_FLOAT( cod, h );
            break;
        case 4:
            importe = ( double )_GET_MEMORY_DOUBLE( cod, h );
            break;
        case 8:
            importe = ( double )_GET_MEMORY_LONG( cod, h );
            break;
        case 9:
            importe = ( double )_GET_MEMORY_UNSIGNED( cod, h );
            break;
    }
    //importe = _GET_MEMORY_DOUBLE( cod, h );
    if( importe < desde || importe > hasta ) {
        error = 1;
    }
    return ( error );
}

