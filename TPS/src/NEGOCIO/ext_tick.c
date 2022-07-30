#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <ext_tick.h> 
#include <malloc.h> 
#include <db.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#else
#endif
extern int tipo_t_varios;
/*****************************************************************************/
void INCREMENTA_P_EXT_IMPUESTOS()
/*****************************************************************************/
{
    if( RAM_P_EXT_IMPUESTOS < _LARGO_EXT_IMPUESTOS ) {
        ADD_MEMORY_INT( __ram_p_ext_impuestos, 1 );
    }
}
/*****************************************************************************/
void ADD_EXT_IMPUESTOS( int sub1, int sub2, double importe )
/*****************************************************************************/
{
    int subindice;
    subindice = sub1 * 20 + sub2 - 1;
    _ADD_MEMORY_DOUBLE( __ext_impuestos, subindice, importe );
}
/*****************************************************************************/
double GET_EXT_IMPUESTOS( int sub1, int sub2 )
/*****************************************************************************/
{
    int subindice;
    subindice = sub1 * 20 + sub2 - 1;
    return ( _GET_MEMORY_DOUBLE( __ext_impuestos, subindice ) );
}
/*****************************************************************************/
void GRABA_EVENTOS_EXT_IMPUESTOS()
/*****************************************************************************/
{
    int l_buffer,h,i;
    char *buffer;
    double *datos;
    struct _cabecera
    {
        char cod_ext;
    }        *cabecera;
    l_buffer = sizeof( struct _cabecera ) + SIZEOF_VAR( __ext_impuestos ) * 20;
    buffer = MALLOC_CLEAR( l_buffer );
    if( buffer ) {
        cabecera = ( struct _cabecera * )buffer;
        for( h = 0;h < RAM_P_EXT_IMPUESTOS;h++ ) {
            cabecera->cod_ext = h + 1;
            datos = ( double* )( buffer + sizeof( struct _cabecera ) );
            for( i = 0;i < 20;i++ ) {
                *datos = _GET_MEMORY_DOUBLE( __ext_impuestos, h * 20 + i );
                datos++;
            }
            GRABAR_EVENT_VARIOS( buffer, l_buffer, SI );
        }
        free( buffer );
    }
}
/*****************************************************************************/
void CARGA_TOTAL_IMPUESTOS( double *total, char cod_ext )
/*****************************************************************************/
{
    double *importe;
    int h,/*area_ant,*/ tabla_ant, tipo_ant;
    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_VARIOS );
    SELECT_TABLE( T_VARIOS, tipo_t_varios );
    //GET_EQUAL( ( char* )&event_e_ticket.id_evento );
    SET_WHERE( "ID_EVENTO = %ld", event_e_ticket.id_evento );
    SET_ORDER2( "ID_EVENTO" );
    RUN_QUERY( NO );
    importe = ( double* )&event_varios->buffer[1];
    while( event_varios->cabecera.id_evento == event_ticket.id_evento && /*!BtrvEOF()*/ !dbEOF() ) {
        if( event_varios->buffer[0] == cod_ext ) {
            for( h = 0;h < 20;h++ ) {
                total[h] += importe[h];
            }
        }
        //SKIP( 1 );
        SKIP2( 1 );
    }
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
}
/*****************************************************************************/
void DISCRIMINA_IMPUESTOS_EN_ARTICULO( double porc, char *procesar )
/*****************************************************************************/
{
    int h, incrementar = 0;
    for( h = 0;h < RAM_P_TICKET;h++ ) {
        _DISCRIMINA_IMPUESTOS_EN_ARTICULO( porc, h, &incrementar, procesar );
    }
    if( incrementar ) {
        INCREMENTA_P_EXT_IMPUESTOS();
        _SET_MEMORY_CHAR( __ticket_cod_ext, RAM_P_TICKET, RAM_P_EXT_IMPUESTOS );
    }
}
/*****************************************************************************/
void _DISCRIMINA_IMPUESTOS_EN_ARTICULO( double porc, int h, int *incrementar, char *procesar )
/*****************************************************************************/
{
    double mult;
    int signo,i,sub;
    if( ( procesar && procesar[h] ) || !procesar ) {
        signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
        mult = porc * signo * _TICKET_CANTIDAD( h );
        if( _TICKET_COD_EXT( h ) ) {
            for( i = 0;i < 20;i++ ) {
                sub = ( _TICKET_COD_EXT( h ) - 1 ) * 20 + i;
                ADD_EXT_IMPUESTOS( RAM_P_EXT_IMPUESTOS, i + 1, mult * _EXT_IMPUESTOS( sub ) );
            }
            *incrementar = 1;
        }
        else {
            if( _TICKET_COD_IMPUESTO1( h ) ) {
                ADD_EXT_IMPUESTOS( RAM_P_EXT_IMPUESTOS, _TICKET_COD_IMPUESTO1( h ),
                                   mult * ( _TICKET_IMPUESTO1_PRECIO( h ) ) );
                *incrementar = 1;
            }
            if( _TICKET_COD_IMPUESTO2( h ) ) {
                ADD_EXT_IMPUESTOS( RAM_P_EXT_IMPUESTOS, _TICKET_COD_IMPUESTO2( h ),
                                   mult * ( _TICKET_IMPUESTO2_PRECIO( h ) ) );
                *incrementar = 1;
            }
            if( _TICKET_COD_IMPUESTO3( h ) ) {
                ADD_EXT_IMPUESTOS( RAM_P_EXT_IMPUESTOS, _TICKET_COD_IMPUESTO3( h ),
                                   mult * ( _TICKET_IMPUESTO3_PRECIO( h ) ) );
                *incrementar = 1;
            }
            if( _TICKET_COD_IMPUESTO4( h ) ) {
                ADD_EXT_IMPUESTOS( RAM_P_EXT_IMPUESTOS, _TICKET_COD_IMPUESTO4( h ),
                                   mult * ( _TICKET_IMPUESTO4_PRECIO( h ) ) );
                *incrementar = 1;
            }
        }
    }
}

