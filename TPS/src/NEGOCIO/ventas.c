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
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <export.h> 
#include <ram.h> 
#include <round.h> 
#include <stack.h> 
#include <ventas.h> 
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <math.h>
#include <aarch.h>
#include "actions.h"
/*
struct _vta {
    BINARY      status;
    long      cod_especial;
    float     cantidad;
    double    importe_sin_iva;
    double    imp_int;
    double    importe_iva1;
    double    costo;
    double    recargo_tarjeta;
    double    exento;
    double    importe_iva2;
//  double    importe_ila;
//  double    importe_iha;
    double    importe_impuestos[4];
    char      reserva[33];
};
struct _art_dat {
    BINARY      status;
    unsigned long cod_interno;
    float     stock;
    short int fecha_ult_compra;
    float     cant_ult_compra;
    char      reserva[15];
};
*/
static struct _vta acu_vta1,acu_vta2,acu_vta3;
static struct _art_dat art_dat;
void _ACTUALIZA_VENTAS( int h );
void ACTUALIZA_STOCK( int h, double cant );
/*****************************************************************************/
void ACTUALIZA_VENTAS()
/*****************************************************************************/
{
    int _h;
    //_h = STACK_INS( sizeof( int ) );
    _h = STACK_INS( sizeof( short ) );
    for( ;GET_MEMORY_INT( _h ) < RAM_P_TICKET;ADD_MEMORY_INT( _h, 1 ) ) {
        _ACTUALIZA_VENTAS( GET_MEMORY_INT( _h ) );
    }
    SET_MEMORY_INT( _h, 0 );
    //STACK_DEL( sizeof( int ) );
    STACK_DEL( sizeof( short ) );
}
/*****************************************************************************/
void _ACTUALIZA_VENTAS( int h )
/*****************************************************************************/
{
    double signo,recargo,imp_int,costo;
    double iva1_rec,iva1_precio,cant,iva2_rec,iva2_precio,imp_sin_iva;
    double imp_iva1,imp_iva2,imp_exento,recargo_neto,cantidad;
    //double    ila_precio, ila_rec, imp_ila, iha_precio, iha_rec, imp_iha;
    double imp_pre[4];
    int _p,_h,_estado;
    OPEN_RECUPERO( &_p, &_h );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    /*------------------- Calcula los importes ---------------------------*/
    SET_RAM( ON );
    signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
    cant = _TICKET_CANTIDAD( h ) * signo;
    cantidad = cant * _TICKET_PRESENTACION( h );
    imp_int = ROUND( cant * _TICKET_IMP_INT( h ) * _TICKET_PRESENTACION( h ), 10, 3 );
    recargo = ROUND( cant * _TICKET_RECARGO( h ), 10, 3 );
    iva1_precio = ROUND( cant * _TICKET_IVA1_PRECIO( h ), 10, 3 );
    iva1_rec = ROUND( cant * _TICKET_IVA1_REC( h ), 10, 3 );
    iva2_precio = ROUND( cant * _TICKET_IVA2_PRECIO( h ), 10, 3 );
    iva2_rec = ROUND( cant * _TICKET_IVA2_REC( h ), 10, 3 );
    //ila_precio = ROUND( cant * _TICKET_ILA_PRECIO( h ), 10, 3 );
    //ila_rec = ROUND( cant * _TICKET_ILA_REC( h ), 10, 3 );
    //iha_precio = ROUND( cant * _TICKET_IHA_PRECIO( h ), 10, 3 );
    //iha_rec = ROUND( cant * _TICKET_IHA_REC( h ), 10, 3 );
    imp_pre[0] = ROUND( cant * _TICKET_IMPUESTO1_PRECIO( h ), 10, 3 );
    imp_pre[1] = ROUND( cant * _TICKET_IMPUESTO2_PRECIO( h ), 10, 3 );
    imp_pre[2] = ROUND( cant * _TICKET_IMPUESTO3_PRECIO( h ), 10, 3 );
    imp_pre[3] = ROUND( cant * _TICKET_IMPUESTO4_PRECIO( h ), 10, 3 );
    costo = ROUND( cant * _TICKET_COSTO( h ), 10, 3 );
    imp_sin_iva = ROUND( cant * _TICKET_IMPORTE_SIN_IVA( h ) - imp_int, 10, 3 );
    imp_iva1 = ROUND( iva1_precio + iva1_rec, 10, 3 );
    imp_iva2 = ROUND( iva2_precio + iva2_rec, 10, 3 );
    //  imp_ila = ROUND( ila_precio + ila_rec, 10, 3 );
    //  imp_iha = ROUND( iha_precio + iha_rec, 10, 3 );
    imp_exento = ROUND( cant * _TICKET_EXENTO( h ), 10, 3 );
    if( recargo ) {
        recargo_neto = ROUND( recargo - iva1_rec - iva2_rec, 10, 3 );
    }
    else {
        recargo_neto = 0;
    }
    if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
        cant = -cant;
    }
    /*-------------------- Acumula los art¡culos vendidos --------------------*/
    if( _ESTADO == 0 ) {
        if( /*USED( AREA_ACU_VTA1 )*/OPENED( T_ACU_VTA, TT_ORIG ) ) {
            ACUMULAR_VENTAS_SQL( T_ACU_VTA, TT_ORIG , &acu_vta1, _TICKET_COD_ESPECIAL( h ), cantidad,
                             imp_sin_iva, imp_int, imp_iva1, imp_iva2, imp_pre, costo, recargo_neto,
                             imp_exento );
        }
        _SET_ESTADO( 1 );
    }
    if( _ESTADO == 1 ) {
        /*
		//Se comenta provisoriamente hasta que quede claro para que se usa
		if( USED( AREA_ACU_VTA2 ) && !VARIOS_MARCA_ERROR_RED ) {
            ACUMULAR_VENTAS( AREA_ACU_VTA2, &acu_vta2, _TICKET_COD_ESPECIAL( h ), cantidad,
                             imp_sin_iva, imp_int, imp_iva1, imp_iva2, imp_pre, costo, recargo_neto,
                             imp_exento );
        }*/
        _SET_ESTADO( 2 );
    }
    if( _ESTADO == 2 ) {
        /*
		//Se comenta provisoriamente hasta que quede claro para que se usa
		if( USED( AREA_ACU_VTA3 ) && !VARIOS_MARCA_ERROR_RED ) {
            ACUMULAR_VENTAS( AREA_ACU_VTA3, &acu_vta3, _TICKET_COD_ESPECIAL( h ), cantidad,
                             imp_sin_iva, imp_int, imp_iva1, imp_iva2, imp_pre, costo, recargo_neto,
                             imp_exento );
        }*/
        _SET_ESTADO( 3 );
    }
    /*------------------------ Actualizaci¢n de stock ------------------------*/
    if( _ESTADO == 3 ) {
        if( ACTUALIZAR_STOCK ) {
            ACTUALIZA_STOCK( h, cantidad );
        }
    }
    CLOSE_RECUPERO( _p, _h );
}
/*****************************************************************************/
void ACUMULAR_VENTAS(
     area, vta, cod_interno, cantidad, imp_sin_iva, imp_int, imp_iva1, imp_iva2, impues, costo,
     recargo_neto, imp_exento )
     /*****************************************************************************/
     int          area;
struct   _vta *vta;
long cod_interno;
double cantidad,imp_sin_iva,imp_int,imp_iva1,imp_iva2,costo,recargo_neto;
double impues[4],imp_exento;
{
    int nuevo = 0, i;
    /*------------------------- Busca registro ---------------------------*/
    SELECT( area );
    GET_EQUAL( ( char* )&cod_interno );
    if( !FOUND() ) {
        memset( vta, 0, sizeof( struct _vta ) );
        nuevo = 1;
    }
    /*------------------------ Graba registro ---------------------------*/
    vta->cod_especial = cod_interno;
    vta->cantidad = ( float )ROUND( vta->cantidad + cantidad, 10, 3 );
    vta->importe_sin_iva = ROUND( vta->importe_sin_iva + imp_sin_iva, 10, 3 );
    vta->imp_int = ROUND( vta->imp_int + imp_int, 10, 3 );
    vta->importe_iva1 = ROUND( vta->importe_iva1 + imp_iva1, 10, 3 );
    vta->importe_iva2 = ROUND( vta->importe_iva2 + imp_iva2, 10, 3 );
    //vta->importe_ila = ROUND( vta->importe_ila + imp_ila, 10, 3 );
    //vta->importe_iha = ROUND( vta->importe_iha + imp_iha, 10, 3 );
    for( i = 0;i < 4;i++ ) {
        vta->importe_impuestos[i] = ROUND( vta->importe_impuestos[i] + impues[i], 10, 3 );
    }
    vta->costo = ROUND( vta->costo + costo, 10, 3 );
    vta->recargo_tarjeta = ROUND( vta->recargo_tarjeta + recargo_neto, 10, 3 );
    vta->exento = ROUND( vta->exento + imp_exento, 10, 3 );
    if( nuevo ) {
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        INSERT();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    else {
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        UPDATE();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
}
/*****************************************************************************/
void ACUMULAR_VENTAS_SQL(
     tabla, tipo, vta, cod_interno, cantidad, imp_sin_iva, imp_int, imp_iva1, imp_iva2, impues, costo,
     recargo_neto, imp_exento )
/*****************************************************************************/
int tabla, tipo;
struct   _vta *vta;
long cod_interno;
double cantidad,imp_sin_iva,imp_int,imp_iva1,imp_iva2,costo,recargo_neto;
double impues[4],imp_exento;
{
    int nuevo = 0, i;
    /*------------------------- Busca registro ---------------------------*/
    SELECT_TABLE( tabla, tipo );
    //GET_EQUAL( ( char* )&cod_interno );
	SET_WHERE("COD_INTERNO = '%ld'",cod_interno);
	RUN_QUERY(NO);
    if( !FOUND2() ) {
        memset( vta, 0, sizeof( struct _vta ) );
        nuevo = 1;
    }
    /*------------------------ Graba registro ---------------------------*/
    vta->cod_especial = cod_interno;
    vta->cantidad = ( float )ROUND( vta->cantidad + cantidad, 10, 3 );
    vta->importe_sin_iva = ROUND( vta->importe_sin_iva + imp_sin_iva, 10, 3 );
    vta->imp_int = ROUND( vta->imp_int + imp_int, 10, 3 );
    vta->importe_iva1 = ROUND( vta->importe_iva1 + imp_iva1, 10, 3 );
    vta->importe_iva2 = ROUND( vta->importe_iva2 + imp_iva2, 10, 3 );
    //vta->importe_ila = ROUND( vta->importe_ila + imp_ila, 10, 3 );
    //vta->importe_iha = ROUND( vta->importe_iha + imp_iha, 10, 3 );
    for( i = 0;i < 4;i++ ) {
        vta->importe_impuestos[i] = ROUND( vta->importe_impuestos[i] + impues[i], 10, 3 );
    }
    vta->costo = ROUND( vta->costo + costo, 10, 3 );
    vta->recargo_tarjeta = ROUND( vta->recargo_tarjeta + recargo_neto, 10, 3 );
    vta->exento = ROUND( vta->exento + imp_exento, 10, 3 );
    if( nuevo ) {
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        INSERT2(NO);
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
    }
    else {
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        UPDATE2();
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
    }
}
/*****************************************************************************/
void ACTUALIZA_STOCK( int h, double cant )
/*****************************************************************************/
{
    long cod;
    /*------------------------- Busca registro ---------------------------*/
	SELECT_TABLE( T_ART_DAT, TT_ORIG );
    cod = _TICKET_COD_ESPECIAL( h );
	SET_WHERE( "COD_INTERNO = '%ld'", cod );
	RUN_QUERY( NO );
    if( FOUND2() ) {
        /*-------------------- actualiza datos de stock ------------------*/
        art_dat.stock -= ( float )cant;
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        UPDATE2();
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
    }
}
/***********************************************************************/
int ABRIR_VTA()
/***********************************************************************/
{
    int error = 0, modo = 0;
    if( !/*USED( AREA_ACU_VTA1 )*/OPENED( T_ACU_VTA, TT_ORIG ) && USAR_ACU_VTA_LOCAL ) {
        modo = ( USAR_ACU_VTA_LOCAL == 2 ) ? -1 : 0;
        error = /*USE_DB( AREA_ACU_VTA1, _ACU_VTA_SIC, ( char* )&acu_vta1, sizeof( acu_vta1 ), NULL,
                        0, modo );*/
				OPEN_TABLE(T_ACU_VTA, TT_ORIG,( char* )&acu_vta1, sizeof( acu_vta1 ));
    }
    /*
	//Se comenta provisoriamente hasta que quede claro para que se usa
	if( !USED( AREA_ACU_VTA2 ) && USAR_ACU_VTA_CAJA ) {
        modo = ( USAR_ACU_VTA_CAJA == 2 ) ? -1 : 0;
        USE_DB( AREA_ACU_VTA2, _ACU_VTA_REMOTO, ( char* )&acu_vta2, sizeof( acu_vta2 ), NULL, 0,
                modo );
    }
    if( !USED( AREA_ACU_VTA3 ) && USAR_ACU_VTA_ALL ) {
        modo = ( USAR_ACU_VTA_ALL == 2 ) ? -1 : 0;
        USE_DB( AREA_ACU_VTA3, _ACU_VTA_ALL, ( char* )&acu_vta3, sizeof( acu_vta3 ), NULL, 0,
                modo );
        SET_LOCKS( 1, 1, 0 );
    }*/
    if( ACTUALIZAR_STOCK && !OPENED( T_ART_DAT, TT_ORIG ) ) {
        error += OPEN_TABLE( T_ART_DAT, TT_ORIG, ( char* )&art_dat, sizeof( art_dat ) );
    }
    return ( error );
}
/***********************************************************************/
void CERRAR_VTA()
/***********************************************************************/
{
    CLOSE_TABLE( T_ACU_VTA, TT_ORIG );
    /*
	if( UTILIZAR_DIA_OPERATIVO ) {
        SETFTIME( _ACU_VTA_SIC, NRO_DIA_OPERATIVO );
    }
	
	//Se comenta provisoriamente hasta que quede claro para que se usa
    if( USED( AREA_ACU_VTA2 ) ) {
        CLOSE_DATABASE( AREA_ACU_VTA2 );
        if( UTILIZAR_DIA_OPERATIVO ) {
            SETFTIME( _ACU_VTA_REMOTO, NRO_DIA_OPERATIVO );
        }
    }
    if( USED( AREA_ACU_VTA3 ) ) {
        CLOSE_DATABASE( AREA_ACU_VTA3 );
    }*/
    if( OPENED( T_ART_DAT, TT_ORIG ) ) {
        CLOSE_TABLE( T_ART_DAT, TT_ORIG );
    }
}
/***********************************************************************/
void COPIAR_VENTAS( int hacer_bak )
/***********************************************************************/
{
    /*if( !UTILIZA_DBROUTER ) {
        CERRAR_VTA();
        if( hacer_bak ) {
            COPY( _ACU_VTA_REMOTO, _ACU_VTA_REMOTO_BAK );
        }
        COPY( _ACU_VTA_SIC, _ACU_VTA_REMOTO );
        LOG_EXISTE_ARCHIVO_COD( _ACU_VTA_REMOTO );
        ABRIR_VTA();
    }*/
}
/***********************************************************************/
void COPIAR_VENTAS_A_VENTAS_Z()
/***********************************************************************/
{
    CERRAR_VTA();
    COPY( _ACU_VTA_SIC, _ACU_VTA_VENTAS_Z );
    LOG_EXISTE_ARCHIVO_COD( _ACU_VTA_VENTAS_Z );
    ABRIR_VTA();
}
/***********************************************************************/
void BACKUP_VENTAS()
/***********************************************************************/
{
    CERRAR_VTA();
	BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_ACU_VTA, _ACU_VTA_BAK );
    //_FECHA_ARCHIVO = GET_FECHA_BACKUP();
    //COPY( _ACU_VTA_SIC, _ACU_VTA_BAK );
    //LOG_EXISTE_ARCHIVO_COD( _ACU_VTA_BAK );
    ABRIR_VTA();
}
/***********************************************************************/
int COPIAR_VENTAS_ANTERIOR()
/***********************************************************************/
{
    int rta = 0, fecha, arch;
    arch = ( UTILIZAR_DIA_OPERATIVO ) ? _ACU_VTA_VENTAS_Z : _ACU_VTA_REMOTO;
    fecha = LEE_FECHA( _FECHA_VTA );
    if( fecha ) {
        rta = EXPORTA_ARCHIVO_DE_LA_SEMANA( fecha, _ACU_VTA_BAK, arch );
    }
    return ( rta );
}
/*****************************************************************************/
void CEREAR_VENTAS()
/*****************************************************************************/
{
    /*if( MODO_NEGOCIO != SERVICIOS ) {
        CERRAR_VTA();
        COPY( _ACU_VTA_000, _ACU_VTA_SIC );
        if( !UTILIZA_DBROUTER ) {
            if( UTILIZAR_DIA_OPERATIVO || VENTAS_A_VENTAS_Z ) {
                COPY( _ACU_VTA_000, _ACU_VTA_REMOTO );
            }
        }
        ABRIR_VTA();
    }*/
}

