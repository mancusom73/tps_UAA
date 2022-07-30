#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
//#include <browse.h> 
#include <btrv.h> 
#include <cmemory.h> 
#include <cr.h>  
#include <cus.h> 
#include <dbrouter.h> 
#include <mstring.h> 
#include <pant.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_CUS
struct _tmp_tran
{
    BINARY status;
    short int pos;
    long transaccion;
    long ticket;
    long fecha;
    long hora;
    char tipo;
    char surtidor;
    char manguera;
    char producto;
    long cod_interno;
    double precio;
    double importe;
    double volumen;
    char estado;                  // 0-Pendiente  1-Cobrado      2-Ejecutado
    char proceso;             // 0-Prepago    1-Habilitacion
    char reserva[20];
};
struct _tmp_tran tmp_tran;
#endif
/*****************************************************************************/
void TRAE_VENTA_DEL_CUS()
/*****************************************************************************/
{
    #ifdef COMPILAR_CUS
    if( SELECCIONAR_VENTAS_DEL_CUS() ) {
        SET_MEMORY_DOUBLE( __ram_cantidad, tmp_tran.volumen );
        AGREGAR_ARTICULO( tmp_tran.cod_interno, tmp_tran.precio, _ORIGEN_CUS, 0 );
    }
    #endif
}
#ifdef COMPILAR_CUS
/*****************************************************************************/
int _elegir_tmp_tran( int comando, char *cadena, int dato, struct _browse *b )
/*****************************************************************************/
{
    int rta = 0;
    int x = 30;
    switch( comando ) {
        case _MOSTRAR:
            MOSTRAR_ENTERO( x + 1, 8 + dato, "99", tmp_tran.surtidor );
            MOSTRAR_ENTERO( x + 4, 8 + dato, "9", tmp_tran.manguera );
            MOSTRAR_FLOTANTE( x + 6, 8 + dato, ENTEROS, DECIMALES, NO, SI, tmp_tran.importe );
            break;
        case _SKIP:
            SKIP( dato );
            rta = BtrvEOF() || BOF();
            if( !rta && tmp_tran.estado != 0 ) {
                if( dato > 0 ) {
                    GO_TMP_TRAN( BOTTOM );
                }
                else {
                    GO_TMP_TRAN( TOP );
                }
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? BOF() : BtrvEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
        case _COMPARAR:
            rta = 2;
            break;
    }
    return ( rta );
}
#endif
/****************************************************************************/
int SELECCIONAR_VENTAS_DEL_CUS()
/****************************************************************************/
{
    int rta = 0;
    #ifdef COMPILAR_CUS
    /*---------------------------- Abre el archivo ---------------------------*/
    if( USE_DB( 0, _TMP_TRAN_BTR, &tmp_tran, sizeof( tmp_tran ), NULL, 0, 0 ) == 0 ) {
        //      SET_ORDER( 2 );
        GO( TOP );
        if( tmp_tran.estado != 0 ) {
            MENSAJE_STRING( S_NO_HAY_VENTAS_PENDIENTES );
        }
        else {
            if( BROWSE( 30, 8, 44, 13, 1, _elegir_tmp_tran ) ) {
                rta = SI;
                SET_MEMORY_LONG( __ram_transac_en_cobro, GET_POSITION() );
            }
        }
    }
    CLOSE_DB( _TMP_TRAN_BTR );
    #endif
    return ( rta );
}
#ifdef COMPILAR_CUS
/*****************************************************************************/
void GO_TMP_TRAN( int modo )
/*****************************************************************************/
{
    char ind = 1;
    if( modo == TOP ) {
        //      GET_GREATER_E( (char *)&ind2_eventos_all );
        GO( TOP );
    }
    else if( modo == BOTTOM ) {
        GET_GREATER_E( &ind );
        if( !BtrvEOF() ) {
            SKIP( -1 );
        }
    }
}
#endif
/*****************************************************************************/
void CUS_GRABA_PAGO()
/*****************************************************************************/
{
    #ifdef COMPILAR_CUS
    if( RAM_TRANSAC_EN_COBRO ) {
        if( USE_DB( 0, _TMP_TRAN_BTR, &tmp_tran, sizeof( tmp_tran ), NULL, 0, 0 ) == 0 ) {
            if( GET_DIRECT( RAM_TRANSAC_EN_COBRO ) == 0 ) {
                tmp_tran.estado = 1;    /* Cobrado */
                #ifdef INVEL_L
                BEGIN_TRANSACTION();
                #endif
                UPDATE();
                #ifdef INVEL_L
                END_TRANSACTION();
                #endif
                SET_MEMORY_LONG( __ram_transac_en_cobro, 0 );
            }
            CLOSE_DB( _TMP_TRAN_BTR );
        }
    }
    #endif
}

