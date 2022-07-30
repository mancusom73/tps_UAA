#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
//#include <browse.h> 
#include <btrv.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <d_entry.h> 
#include <eventos.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <log.h> 
#include <mensaje.h> 
#include <m_cadena.h> 
#include <pagos.h> 
#include <pant.h> 
#include <recup.h> 
#include <stringt.h> 
#include <valida.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <aarch.h>
#include <db.h>
#include <deftables.h>
#include <str.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma warning (disable:4103)  //aviso de pragma pack en cr.h
int _consulta_d_entry( int comando, char *cadena, int dato, struct _browse *b, double monto );
static struct _data_entry *d_entry;
extern struct _valida valida;
extern int tipo_ev_dpago;
/***************************************************************************/
int DATA_ENTRY( void )
/***************************************************************************/
{
    int rta = 0, cod_solicitud = 0;
    double importe = 0.0;
    struct  _data_entry d_entry_sic;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	memset(&d_entry_sic,0,sizeof(struct  _data_entry));
    d_entry = &d_entry_sic;
    //if( USE_DB( AREA_ENTRY, _D_ENTRY_SIC, ( char* )&d_entry_sic, sizeof( d_entry_sic ), NULL, 0, 0 )
    // == 0 ) {
	 if( OPEN_TABLE( T_D_ENTRY, TT_ORIG, ( char* )&d_entry_sic, sizeof( struct _data_entry ) ) == 0 ) {
		//SELECT_TABLE( T_D_ENTRY, TT_ORIG ); 
		RUN_QUERY(NO);
		
		if( RECCOUNT2( NULL, NULL ) > 1 ) {
            rta = Consulta_data_entry( _consulta_d_entry );
        }
        else {
            cod_solicitud = d_entry_sic.cod_solicitud;
			rta = 1;
        }
        if( rta ) {
            cod_solicitud = d_entry_sic.cod_solicitud;
        }
        //CLOSE_DATABASE( AREA_ENTRY );
		CLOSE_TABLE( T_D_ENTRY, TT_ORIG );
        if( cod_solicitud > 0 ) {
            rta = PAGOS_PEDIR_DATOS_ADICIONALES( 0, 0, cod_solicitud, &importe, D_ENTRY, NULL, 1,
                                                 NULL, NULL , '\0', '\0');
            if( rta ) {
                GRABAR_EVENTO_DATA_ENTRY();
                if( d_entry_sic.cod_validacion ) {
                    valida.fecha = GET_FECHA_DOS();
                    valida.hora = GET_HORA_DOS();
                    VALIDAR( d_entry_sic.cod_validacion, 0, 0, 0, 0 );
                }
                ACTUALIZA_ID_EVENTO_NEW();
            }
            else {
                BORRAR_DATOS_ADICIONALES( 0, D_ENTRY );
            }
            CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL();
        }
    }

	SELECT_TABLE( area_ant, tipo_ant );

    return ( rta );
}
/***************************************************************************/
void GRABAR_EVENTO_DATA_ENTRY( void )
/***************************************************************************/
{
    /*-------------------------- Graba eventos -----------------------------*/
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = 59;
    GRABAR_EVENT_TICKET( NO );
    AGREGA_BIN_EV_DMEDIO_A_BTRV( 0, D_ENTRY );
    /*--------------------------- Otros seteos -----------------------------*/
    FIN_DE_TICKET( NO );
}
/*****************************************************************************/
int _consulta_d_entry( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y, cod;
    y = 10 + dato;
    switch( comando ) {
		case _BUSCAR:
			MOSTRAR_CADENA( 23, 15, cadena );
            cod = atoi( cadena );
            //GET_GREATER_E( ( char* )&cod );
			SET_WHERE("CODIGO >= '%d'", cod );
			SET_ORDER2("CODIGO");
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                //_itoa( d_entry->codigo, xx, 10 );
				memset( xx, 0, sizeof( xx ) );
				_snprintf( xx, sizeof( xx ) - 1,"%d", d_entry->codigo);
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, d_entry->descripcion );
            }
            #endif
            MOSTRAR_ENTERO( 23, y, "99", d_entry->codigo );
            MOSTRAR_CADENA( 26, y, d_entry->descripcion );
            break;
        case _SKIP:
            //SKIP( dato );
            //rta = BtrvEOF() || BOF();
			SKIP2( dato );
            rta = dbBOF() || dbEOF();
            break;
        case _EXTREMO:
            //rta = ( dato == -1 ) ? BOF() : BtrvEOF();
			 rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
        case _COMPARAR:
            rta = 1;
            break;
        case _TECLA:
            if( dato == 32 ) {
                b->tecla = 0;
            }
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
    }
    return ( rta );
}
/******************************************************************************/
void BUSCAR_DATO_ADICIONAL_D_ENTRY( int orden, char *respuesta, int tam_resp )
/******************************************************************************/
{
    int area;
    /*struct
    {
        long id_evento;
        int orden;
    } indice;*/
    area = SELECTED();
    /*indice.id_evento = RAM_ID_EVENTO;
    indice.orden = orden;*/
    //SELECT( AREA_DPAGO );
    SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
    //SET_ORDER( 3 );
    //GET_EQUAL( ( char* )&indice );
    SET_WHERE( "ID_EVENTO = '%ld' AND NRO_ORDEN = '%d'", RAM_ID_EVENTO, orden );
    RUN_QUERY( NO );
    if( /*FOUND()*/ FOUND2() ) {
        memcpy( respuesta, event_dpago->valor, tam_resp );
    }
    else {
        char aux[60];
        _snprintf(aux, sizeof(aux)-1, "Nro de Orden %d, NO ENCONTRADO", orden );
        MENSAJE( aux );
        GRABAR_LOG_SISTEMA( aux ,LOG_DEBUG,2 );
    }
    SELECT( area );
}

