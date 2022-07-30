#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <actual.h> 
#include <btrv.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_displ.h> 
#include <dbrouter.h> 
#include <import_t.h> 
#include <log.h> 
#include <netcom.h> 
#include <pagos.h> 
#include <pant.h> 
#include <stack.h> 
#include <status.h> 
#include <t_reproc.h> 
#include <ventas.h> 
#include <ventas2.h> 
#include <xyz.h> 
#include <_cr1.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#define NO_ENCONTRADO                       0
#define ENCONTRADO_EN_EVENTOS               1
#define ENCONTRADO_EN_HISTORICOS            2
#ifdef COMPILAR_TECLA_REPROCESAR
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos, tipo_t_ev_super, tipo_ev_dpago;
extern int tipo_t_ev_despro, tipo_t_ev_factu, tipo_t_ev_b_promo;
/*****************************************************************************/
void TECLA_REPROCESAR( void )
/*****************************************************************************/
{
    int _ptr,_handle,_estado;
    long id_evento;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    id_evento = RAM_ID_EVENTO;
    //SELECT( AREA_TICKET );
    //GET_EQUAL( ( char* )&id_evento );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_ORDER2("ID_EVENTO");
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
	RUN_QUERY(NO);
    if( RAM_REPROCESANDO == 0 ) {
        if( _ESTADO == 0 ) {
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S__REPROCESAR_PAGO_ ),
                                  ST( S_REPROCESO_DEL_PAGO ), RAM_COD_FUNCION, SI ) ) {
                _SET_ESTADO( 1 );
            }
        }
        if( _ESTADO == 1 ) {
            SET_MEMORY_INT( __ram_bultos, RAM_BULTOS_ANT );
            _SET_ESTADO( 2 );
        }
        if( _ESTADO == 2 ) {
            COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
            _SET_ESTADO( 3 );
        }
        if( _ESTADO == 3 ) {
            COPY_STRUCT( _tot_tick, _tot_tick_ant );
            _SET_ESTADO( 4 );
        }
        if( _ESTADO == 4 ) {
            if( event_ticket.tipo_evento != 1 ) {
                ACTUALIZA_ACUMULADORES_IVA( -1 );
            }
            _SET_ESTADO( 5 );
        }
        if( _ESTADO == 5 ) {
            COPY_STRUCT( _tot_tick, _tot_tick_sv );
            _SET_ESTADO( 6 );
        }
        if( _ESTADO == 6 ) {
            SET_MEMORY_CHAR( __var_tarjetas_on_line, 0 );
            _SET_ESTADO( 7 );
        }
        if( _ESTADO == 7 ) {
            #ifdef COMPILAR_TECLA_REPROCESAR
            PAGOS_REPROCESAR( event_ticket.tipo_evento, SI );
            #endif
            _SET_ESTADO( 8 );
        }
        if( _ESTADO == 8 ) {
            COPY_STRUCT( _tot_tick, _tot_tick_sv );
            _SET_ESTADO( 9 );
        }
        if( _ESTADO == 9 ) {
            SET_MEMORY_CHAR( __ram_reprocesando, 1 );
            _SET_ESTADO( 10 );
        }
        if( _ESTADO == 10 ) {
            SET_MEMORY_INT( __ram_p_pago_ant, RAM_P_PAGO );
        }
    }
    if( RAM_REPROCESANDO == 1 ) {
        PAGOS( _MODO_REPROCESAMIENTO, NULL, NO );
        /*-------------------- Graba modos de pago del ticket -------------------*/
        ACTUALIZA_VENTAS();
        GRABA_EVENTOS_PAGO( RAM_P_PAGO_ANT, NO );
        ACTUALIZA_VENTAS2( SI );
        if( event_ticket.tipo_evento != 1 ) {
            ACTUALIZA_ACUMULADORES_IVA( 1 );
        }
        SET_MEMORY_INT( __ram_bultos, 0 );
        SET_MEMORY_INT( __ram_p_ticket, 0 );
        CEREAR_STRUCT( _var_tarjetas );
        GRABAR_XYZ( 'X' );
        #ifdef COMPILAR_DISPLAY
        CR1_DISPLAY_CAJA_ABIERTA();
        #endif
        SET_MEMORY_CHAR( __ram_reprocesando, 0 );
        RESETEA_TOT_TICK();
        FIN_DE_TICKET( SI );
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
    }
    CLOSE_RECUPERO( _ptr, _handle );
}
#endif

/*****************************************************************************/
int POSICIONA_EN_TICKET( long nro_z, long id_evento )
/******************************************************************************/
{
    int nError = 0;
    if( nro_z == NRO_Z /*&& nro_z != 0*/ ) {
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
		SET_ORDER2("ID_EVENTO");
		SET_WHERE("ID_EVENTO = '%ld'",id_evento);
		RUN_QUERY(NO);
        if( !FOUND2() ) {
            char debug[100];
            _snprintf(debug, sizeof(debug)-1, "POSTIC evn %li ERR %d (%li,%li)", id_evento, nError, nro_z, NRO_Z );
            GRABAR_LOG_DBROUTER( debug ,LOG_DEBUG,2);
        }
        return ( FOUND2() );
    }
    else {
        struct _id
        {
            long caja_z;
            long id_evento;
        } id;
		SELECT_TABLE( T_EVENTOS, TT_HIST );
		SET_ORDER2("FECHA");
        id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
		SET_WHERE( "ID_EVENTO = '%ld' AND CAJA_Z = '%ld'", id.id_evento, id.caja_z );
		RUN_QUERY(NO);
        if( !FOUND2() ) {
            char debug[100];
            _snprintf(debug, sizeof(debug)-1, "HIST evn %li ERR %d (%li,%li)", id_evento, nError, nro_z, NRO_Z );
            GRABAR_LOG_DBROUTER( debug ,LOG_DEBUG,2);
        }
        return ( FOUND2() );
    }
}
/*****************************************************************************/
int POSICIONA_EN_PAGO( long nro_z, long id_evento )
/******************************************************************************/
{
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_PAGO );
        //SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&id_evento );
		SELECT_TABLE( T_PAGOS, tipo_t_pagos );
		SET_WHERE("ID_EVENTO = '%ld'",id_evento);
		SET_ORDER2("POSICION");
		RUN_QUERY(NO);
    }
    else {
        struct _id
        {
            long caja_z;
            long id_evento;
        } id;
        //SELECT( AREA_PAGO_HIST );
		SELECT_TABLE( T_PAGOS, TT_HIST );
        //SET_ORDER( 4 );
		SET_ORDER2("NUMERO_ID");
        id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
        //GET_EQUAL( ( char* )&id );
		SET_WHERE( "ID_EVENTO = '%ld' AND CAJA_Z = '%ld'", id.id_evento, id.caja_z );
		RUN_QUERY(NO);
    }
	return FOUND2();
}
/*****************************************************************************/
int POSICIONA_EN_FACTURA( long nro_z, long id_evento )
/******************************************************************************/
{
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_FACTURA );
        //SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&id_evento );
        SELECT_TABLE( T_EV_FACTU, tipo_t_ev_factu );
        SET_WHERE( "ID_EVENTO = '%ld'", id_evento );
        SET_ORDER2( "ID_EVENTO" );
        RUN_QUERY( NO );
        return( FOUND2() );
    }
    else {
        struct _id
        {
            long caja_z;
            long id_evento;
        } id;
        //SELECT( AREA_FACTURA_HIST );
		SELECT_TABLE( T_EV_FACTU, TT_HIST );
        //SET_ORDER( 2 );
		SET_ORDER2("CAJA_Z");
        id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
        //GET_EQUAL( ( char* )&id );
		SET_WHERE( "ID_EVENTO = '%ld' AND CAJA_Z = '%ld'", id.id_evento, id.caja_z );
		RUN_QUERY( NO );
        return FOUND2();
    }
    
}
/*****************************************************************************/
int POSICIONA_EN_COD_BARRA( long nro_z, long id_evento )
/******************************************************************************/
{
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_COD_BARRA );
		SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
        SET_ORDER2("ID_EVENTO");
		//SET_ORDER(1);
        //GET_EQUAL( ( char* )&id_evento );
		SET_WHERE("ID_EVENTO = '%ld'",id_evento);
		RUN_QUERY(NO);
		
    }
    else {
        struct _id
        {
            int caja;
            long nro_z;
            long id_evento;
        } id;
        //SELECT( AREA_COD_BARRA_HIST );
		SELECT_TABLE( T_EV_COD_BARRA, TT_HIST );
        //SET_ORDER( 2 );
		SET_ORDER2("NRO_Z");
        id.caja = NRO_CAJA;
        id.nro_z = nro_z;
        id.id_evento = id_evento;
        //GET_EQUAL( ( char* )&id );
		SET_WHERE( "ID_EVENTO = '%ld' AND NRO_Z = '%ld' AND CAJA = '%d'", id.id_evento, id.nro_z, id.caja );
		RUN_QUERY(NO);
    }
    return ( FOUND2() );
}
/*****************************************************************************/
int POSICIONA_EN_E_TICKET( long nro_z, long id_evento )
/******************************************************************************/
{
    struct _id
    {
        long caja_z;
        long id_evento;
    } id;
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_E_TICKET );
		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
		SET_WHERE("ID_EVENTO = '%ld'",id_evento);
        //SET_ORDER2( "ID_EVENTO");
		SET_ORDER2( "POSICION");
        //GET_EQUAL( ( char* )&id_evento );
		RUN_QUERY(NO);
    }
    else {
        //SELECT( AREA_E_TICKET_HIST );
		SELECT_TABLE( T_EV_CONT, TT_HIST );
        //SET_ORDER( 2 );
		SET_ORDER2("COD_ARTICULO");
        id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
        //GET_EQUAL( ( char* )&id );
		SET_WHERE( "ID_EVENTO = '%ld' AND CAJA_Z = '%ld'", id.id_evento, id.caja_z );
		SET_ORDER2( "POSICION");		
		RUN_QUERY( NO );
    }
	return (  FOUND2() );
}
/*****************************************************************************/
int POSICIONA_EN_E_DESPRO( long nro_z, long id_evento )
/******************************************************************************/
{
    struct _id
    {
        long id_evento;
        long caja_z;
    } id;
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_E_DES_PRO );
        SELECT_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
        SET_ORDER2("ID_MOTIVO");
		GO2( TOP );
        //SET_ORDER( 4 );
        //GET_EQUAL( ( char* )&id_evento );
        SET_WHERE( "ID_EVENTO = '%ld'", id_evento );
        RUN_QUERY( NO );
        return ( FOUND2() );
    }
    else {
        //SELECT( AREA_E_DESPRO_HIST );
		SELECT_TABLE( T_EV_DESPRO, TT_HIST );
        //SET_ORDER( 3 );
		SET_ORDER2("TIPO_MOTIVO");
        id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
        //GET_EQUAL( ( char* )&id );
		SET_WHERE( "ID_EVENTO = '%ld' AND CAJA_Z = '%ld'", id.id_evento, id.caja_z );
		RUN_QUERY( NO );
        return( FOUND2() );
    }
}
/*****************************************************************************/
int POSICIONA_EN_CIERRE( long nro_z, long id_evento )
/******************************************************************************/
{
    struct _id
    {
        long caja_z;
        long id_evento;
    } id;
	id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
    id.id_evento = id_evento;
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_CIERRE );
		SELECT_TABLE( T_EV_CIERRE, TT_ORIG );
		SET_WHERE("CAJA_Z = '%ld' and ID_EVENTO = '%ld'",id.caja_z,	id.id_evento);
		SET_ORDER2("CAJA_Z,ID_EVENTO");
		RUN_QUERY(NO);
		return ( FOUND2() );
    }
    else {
        //SELECT( AREA_CIERRE_HIST );
		SELECT_TABLE( T_EV_CIERRE, TT_HIST );
		//SET_ORDER( 1 );
		id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
		SET_ORDER2("CAJA");
		//GO( TOP );
		GO2(TOP);
		SET_WHERE( "ID_EVENTO = '%ld' AND CAJA_Z = '%ld'", id.id_evento, id.caja_z );
		RUN_QUERY( NO );
		//GET_EQUAL( ( char* )&id );
    }
    return ( FOUND2() );
}
/*****************************************************************************/
int POSICIONA_EN_DPAGO( long nro_z, long id_evento )
/******************************************************************************/
{
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_DPAGO );
        //SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&id_evento );
        if( !OPENED( T_EV_DMEDIOS, tipo_ev_dpago) ) {
            if( OPEN_TABLE( T_EV_DMEDIOS, tipo_ev_dpago, ( char* )&event_dpago,
											sizeof( struct _ev_dmedios ) ) ) {
					GRABAR_LOG_DBROUTER( "ERROR DPAGOS" ,LOG_ERRORES,2);
					return (0);
				}

        }
        SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
        SET_WHERE( "ID_EVENTO = '%ld'", id_evento );
		SET_ORDER2("POSICION");
        RUN_QUERY( NO );
        return FOUND2();
    }
    else {
        struct _id
        {
            long caja_z;
            long id_evento;
        } id;
        //SELECT( AREA_DPAGO_HIST );
		SELECT_TABLE( T_EV_DMEDIOS, TT_HIST );
        //SET_ORDER( 2 );
		SET_ORDER2("CAJA_Z");
        id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
        //GET_EQUAL( ( char* )&id );
		SET_WHERE( "ID_EVENTO = '%ld' AND CAJA_Z = '%ld'", id.id_evento, id.caja_z );
		RUN_QUERY( NO );
		SET_ORDER2("POSICION");
        return FOUND2();
    }
    
}
/*****************************************************************************/
POSICIONA_EN_SUPER( long nro_z, long id_evento )
/******************************************************************************/
{
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_SUPER );
        SELECT_TABLE( T_EV_SUPER, tipo_t_ev_super );
        //SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&id_evento );
        SET_WHERE( "ID_EVENTO = '%ld'", id_evento );
        RUN_QUERY( NO );
		SET_ORDER2("POSICION");
        return ( FOUND2() );
    }
    else {
        struct _id
        {
            long caja_z;
            long id_evento;
        } id;
        //SELECT( AREA_SUPER_HIST );
		SELECT_TABLE(T_EV_SUPER, TT_HIST);
        //SET_ORDER( 2 );
		SET_ORDER2("CAJA_Z");
        id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
        //GET_EQUAL( ( char* )&id );
		SET_WHERE("ID_EVENTO = %ld AND CAJA_Z = %ld", id.id_evento, id.caja_z);
		RUN_QUERY(NO);
		SET_ORDER2("POSICION");
        return FOUND2();
    }
}
/*****************************************************************************/
POSICIONA_EN_EV_B_PROMO( long nro_z, long id_evento )
/******************************************************************************/
{
    if( nro_z == NRO_Z ) {
        //SELECT( AREA_EV_B_PROMO );
        //SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&id_evento );
        SELECT_TABLE( T_EV_B_PROMO, tipo_t_ev_b_promo );
        SET_WHERE( "ID_EVENTO = '%ld'", id_evento );
        RUN_QUERY( NO );
        return ( FOUND2() );
    }
    else {
        struct _id
        {
            long caja_z;
            long id_evento;
        } id;
        //SELECT( AREA_EV_B_PROMO_HIST );
		SELECT_TABLE( T_EV_B_PROMO, TT_HIST );
        //SET_ORDER( 2 );
		SET_ORDER2("ID_PROMO");
        id.caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
        id.id_evento = id_evento;
        //GET_EQUAL( ( char* )&id );
		SET_WHERE("ID_EVENTO = %ld AND CAJA_Z = %ld", id.id_evento, id.caja_z);
		RUN_QUERY(NO);
        return FOUND2();
    }
    
}
/*****************************************************************************/
void TECLA_REPROCESAR_REFERENCIADA( void )
/*****************************************************************************/
{
    int _ptr,_handle,_estado;
    //  long      id_evento;
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );

    if( RAM_REPROCESANDO == 0 ) {
        if( _ESTADO == 0 ) {
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S__REPROCESAR_PAGO_ ),
                                  ST( S_REPROCESO_DEL_PAGO ), RAM_COD_FUNCION, SI ) )
                                                      //if( PIDE_DATOS_IMPORTAR_T( 1, "" ) ) 
            {
                if( IMPORTAR_TICKET( SI, 70, NO, "IMPORTAR COBRO", NO, 0, _NO_APLICA ,SI , NO ) ) {
                    //id_evento = RAM_ID_EVENTO;
                    //SELECT( AREA_TICKET );
                    //GET_EQUAL( ( char * ) &id_evento );
                    ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                              &ev_cod_barra, &ev_b_promo );
                    //SELECT( AREA_AUX3 );          //EVENTOS
					SELECT_TABLE( T_EV_TMP, TT_ORIG );
					SET_WHERE("");
					RUN_QUERY(NO);
                    _SET_ESTADO( 1 );
                }
            }
        }
        if( _ESTADO == 1 ) {
            //SET_MEMORY_INT( __ram_bultos, RAM_BULTOS_ANT );
            SET_MEMORY_INT( __ram_bultos, eventos.bultos );
            _SET_ESTADO( 2 );
        }
        if( _ESTADO == 2 ) {
            COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
            _SET_ESTADO( 3 );
        }
        if( _ESTADO == 3 ) {
            COPY_STRUCT( _tot_tick, _tot_tick_ant );
            _SET_ESTADO( 4 );
        }
        if( _ESTADO == 4 ) {
            if( event_ticket.tipo_evento != 1 ) {
                ACTUALIZA_ACUMULADORES_IVA( -1 );
            }
            _SET_ESTADO( 5 );
        }
        if( _ESTADO == 5 ) {
            COPY_STRUCT( _tot_tick, _tot_tick_sv );
            _SET_ESTADO( 6 );
        }
        if( _ESTADO == 6 ) {
            SET_MEMORY_CHAR( __var_tarjetas_on_line, 0 );
            _SET_ESTADO( 7 );
        }
        if( _ESTADO == 7 ) {
            #ifdef COMPILAR_TECLA_REPROCESAR
            PAGOS_REPROCESAR( event_ticket.tipo_evento, SI );
            #endif
            _SET_ESTADO( 8 );
        }
        if( _ESTADO == 8 ) {
            COPY_STRUCT( _tot_tick, _tot_tick_sv );
            _SET_ESTADO( 9 );
        }
        if( _ESTADO == 9 ) {
            SET_MEMORY_CHAR( __ram_reprocesando, 1 );
            _SET_ESTADO( 10 );
        }
        if( _ESTADO == 10 ) {
            SET_MEMORY_INT( __ram_p_pago_ant, RAM_P_PAGO );
        }
    }
    if( RAM_REPROCESANDO == 1 ) {
        PAGOS( _MODO_REPROCESAMIENTO, NULL, NO );
        /*-------------------- Graba modos de pago del ticket -------------------*/
        ACTUALIZA_VENTAS();
        GRABA_EVENTOS_PAGO( RAM_P_PAGO_ANT, NO );
        ACTUALIZA_VENTAS2( SI );
        if( event_ticket.tipo_evento != 1 ) {
            ACTUALIZA_ACUMULADORES_IVA( 1 );
        }
        SET_MEMORY_INT( __ram_bultos, 0 );
        SET_MEMORY_INT( __ram_p_ticket, 0 );
        CEREAR_STRUCT( _var_tarjetas );
        GRABAR_XYZ( 'X' );
        #ifdef COMPILAR_DISPLAY
        CR1_DISPLAY_CAJA_ABIERTA();
        #endif
        SET_MEMORY_CHAR( __ram_reprocesando, 0 );
        RESETEA_TOT_TICK();
        FIN_DE_TICKET( SI );
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
    }
    CLOSE_RECUPERO( _ptr, _handle );
}

