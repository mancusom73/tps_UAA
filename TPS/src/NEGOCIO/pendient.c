#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <btrv.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <mstring.h> 
#include <pendient.h> 
#include <stack.h> 
#include <_cr1.h> 
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_PENDIENTES
/*---- Definicin de valores para el campo 'estado' de PEND1.SIC ----*/
#define AGREGANDO  0
#define TOTAL      1
#define CERRADO    2
#define _NRO  GET_MEMORY_LONG( _nro )
#ifdef COMPILAR_VENTA_PENDIENTE
struct _pend1 
{
	BINARY status;			//0
	long nro_venta;			//1
	short nro_vendedor;     //5
	char estado;			//7
	char reserva[12];		//8
};
struct _pend2 
{
	BINARY status;			//0
	long nro_venta;			//1
	long cod_articulo;		//5
	double cantidad;		//9
	double precio_unit;     //17
	char reserva[15];		//25
};
#endif
/*****************************************************************************/
void PEDIR_VENTA_PENDIENTE()
/*****************************************************************************/
{
    #ifdef COMPILAR_VENTA_PENDIENTE
    long nro_venta;
    nro_venta = PEDIR_ENTERO_EN_RECUADRO( ST( S__N__DE_VENTA_ ), SI, 5 );
    if( nro_venta > 0 ) {
        TRAER_VENTA_PENDIENTE( nro_venta );
        MOSTRAR_VENDEDOR();
    }
    #endif
}
#ifdef COMPILAR_VENTA_PENDIENTE
/*****************************************************************************/
void TRAER_VENTA_PENDIENTE( long nro_venta )
/*****************************************************************************/
{
    int _ptr,_handle,_numero;
    long pos1,pos2,nro;
    struct
    {
        long nro_venta;
        long cod_articulo;
    } ind_pend2 = {0,0};
    struct _pend1 pend1;
    struct _pend2 pend2;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _numero = STACK_INS( sizeof( long ) );
    SET_MEMORY_LONG( _numero, nro_venta );
    if( /*USE_DB( AREA_AUX, _PEND1_SIC, ( char* )&pend1, sizeof( pend1 ), NULL, 0, 0 )*/
			OPEN_TABLE(T_PEND1, TT_ORIG,( char* )&pend1, sizeof( pend1 )) == 0 ) {
        if( /*USE_DB( AREA_AUX2, _PEND2_SIC, ( char* )&pend2, sizeof( pend2 ), NULL, 0, 0 )*/
				OPEN_TABLE(T_PEND2, TT_ORIG,( char* )&pend2, sizeof( pend2 ))== 0 ) {
            /*------------------ Busca la venta ----------------------*/
            //SELECT( AREA_AUX );
			SELECT_TABLE( T_PEND1, TT_ORIG );
            nro = _NRO;
            //GET_EQUAL( ( char* )&nro );
			SET_WHERE("NRO_VENTA = '%ld'",nro);
			RUN_QUERY(NO);
            pos1 = GET_POSITION_REG();
            if( !FOUND2() ) {
                MENSAJE_STRING( S_NUMERO_VENTA_INEXISTENTE );
            }
            else if( pend1.estado != CERRADO ) {
                MENSAJE_STRING( S_VENTA_NO_CERRADA );
            }
            else {
                SET_MEMORY_INT( __nro_vendedor, pend1.nro_vendedor );
                /*--------------- Carga la venta -------------------*/
                //SELECT( AREA_AUX2 );
				SELECT_TABLE( T_PEND2, TT_ORIG );
                ind_pend2.nro_venta = _NRO;
				SET_WHERE(/*"CONCAT(NRO_VENTA,COD_ARTICULO) >= CONCAT(%ld,%ld)"*/
					"(NRO_VENTA = %ld and COD_ARTICULO >= %ld) or NRO_VENTA > %ld",
						ind_pend2.nro_venta,
						ind_pend2.cod_articulo,
						ind_pend2.nro_venta);
				RUN_QUERY(NO);
                //GET_GREATER_E( ( char* )&ind_pend2 );
                while( !dbEOF() && pend2.nro_venta == _NRO ) {
                    pos2 = GET_POSITION_REG();
                    AGREGA_ARTICULO_AL_TICKET( pend2.cod_articulo, pend2.precio_unit,
                                               pend2.cantidad, NO );
                    //SELECT( AREA_AUX2 );
					SELECT_TABLE( T_PEND2, TT_ORIG );
                    GET_DIRECT_REG( pos2 , GET_PAGINA_REG());
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    //_DELETE();
					DELETE2();
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
                //SELECT( AREA_AUX );
				SELECT_TABLE( T_PEND1, TT_ORIG );
                GET_DIRECT_REG( pos1 , GET_PAGINA_REG());
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                //_DELETE();
				DELETE2();
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
            }
            CLOSE_TABLE( T_PEND2, TT_ORIG );
        }
        CLOSE_TABLE( T_PEND1, TT_ORIG );
    }
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}
#endif
/*****************************************************************************/
void AGREGA_ARTICULO_AL_TICKET( long codigo, double precio_unit, double cantidad, int anulacion )
/*****************************************************************************/
{
    if( ASIGNA_CODIGO_ESPECIAL( codigo ) ) {
        SET_MEMORY_DOUBLE( __item_precio, precio_unit );
        SET_MEMORY_DOUBLE( __ram_cantidad, cantidad );
        SET_MEMORY_INT( __ram_anulacion, ( anulacion ) ? 1 : 0 );
        CR1_AGREGA_ITEM( NO, _ORIGEN_PENDIENTE, 0, 0 );
    }
    else {
        MENSAJE_STRING( S_CODIGO_INEXISTENTE );
    }
}
#endif

