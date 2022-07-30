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
#include <dbrouter.h> 
#include <log.h> 
#include <malloc.h> 
#include <t_pago.h> 
#include <ventas2.h> 
#include <_cr1.h> 
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
#undef EOF
//1 free cobros
//2. aumenta nuffer
//3. no pone  where.
#define SIZE_BUFFER_WHERE  1024*3
static char *where_tmp = NULL;
static int cant_reg_where;
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
int MARCAR_COMO_ENVIADA( int all, int off_lines, int z_anteriores, int cant_a_controlar,
                         int marcar_no_encontradas );
/***************************************************************************/
static INICIALIZAR_BUFFER_WHERE( void )
/***************************************************************************/
{
    if( where_tmp == NULL ) {
        where_tmp = (char *)MALLOC( sizeof( char[SIZE_BUFFER_WHERE] ) );
    }
    memset( where_tmp, 0, sizeof( char[SIZE_BUFFER_WHERE] ) );
    cant_reg_where = 0;
}

/***************************************************************************/
AGREGAR_A_CONTROL( long nro_z, long nro_id )
/***************************************************************************/
{
    char buffer_tmp[100];
    int iRta = 1;
    long caja_z = ( long )( NRO_CAJA )*100000L + ( nro_z % 100000L );
    if( where_tmp != NULL ) {
        /*
        if( where_tmp[0] ) {
            sprintf( buffer_tmp, ", %li)", nro_id );
            if( strlen( where_tmp ) + strlen( buffer_tmp ) < SIZE_BUFFER_WHERE ) {
                strcpy( where_tmp + strlen( where_tmp ) - 1, buffer_tmp );
            } else {
                GRABAR_LOG_DBROUTER( ST( SDBR_BUFFER_CONTROLA_OVERFLOW ) );
                iRta = 0;
            }
        } else {
        sprintf( where_tmp, "WHERE (caja_z = %li) AND id_evento IN (%li)", caja_z,
        nro_id );
        }
        */
        if( where_tmp[0] ) {
            sprintf( buffer_tmp, "OR (caja_z = %li AND id_evento=%li)", caja_z, nro_id );
            if( strlen( where_tmp ) + strlen( buffer_tmp ) < SIZE_BUFFER_WHERE ) {
                strcpy( where_tmp + strlen( where_tmp ), buffer_tmp );
            }
            else {
                GRABAR_LOG_DBROUTER( BUSCA_MENSAJE_STRING( SDBR_BUFFER_CONTROLA_OVERFLOW ) ,LOG_ERRORES,2);
                iRta = 0;
            }
        }
        else {
            //sprintf( where_tmp, "WHERE (caja_z = %li AND id_evento=%li)", caja_z, nro_id );
			sprintf( where_tmp, " (caja_z = %li AND id_evento=%li)", caja_z, nro_id );
        }
    }
    else {
        GRABAR_LOG_SISTEMA( "agre NULL" , LOG_ERRORES,2);
        iRta = 0;
    }
    cant_reg_where++;
    return iRta;
}

// Tomar del Archivo de Control y Marcar las Ventas Como Enviadas
/***************************************************************************/
int MARCAR_COMO_ENVIADA( int all, int off_lines, int z_anteriores, int cant_a_controlar,
                         int marcar_no_encontradas )
/***************************************************************************/
{
    struct _eventos event_tmp;
    long nro_ultimo_evento_controlado;
    //int area = SELECTED();
    int tabla_ant = SELECTED2();
    int tipo_ant = SELECTED_TIPO();
    int /*area_ticket,*/ cantidad_encontrada = 0, no_encontrada = 0;
    struct _eventos *ticket;
    // cant_a_controlar = CONTROLAR_VENTAS_EN_SERVER;
    
    if( off_lines ) {
        all = 1;
        nro_ultimo_evento_controlado = 0;
    }
    else {
        nro_ultimo_evento_controlado = NRO_ULTIMO_EVENTO_CONTROLADO;
    }
    //if( USE_DATABASE( AREA_AUX4, "coticket.btr", ( char* )&event_tmp, sizeof( event_tmp ), NULL, 0, -1 ) ) {
    if ( OPEN_TABLE( T_COTICKET, TT_ORIG, ( char* )&event_tmp, sizeof( event_tmp ) ) ) {
        GRABAR_LOG_SISTEMA( "err use data coticket", LOG_ERRORES,2 );
        return ( 0 );
    }
    OBTIENE_PUNTEROS_A_EVENTOS( &ticket, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                z_anteriores ? -1 : NRO_Z );

    if( z_anteriores ) {
        //area_ticket = AREA_TICKET_HIST;
        //SELECT( area_ticket );
        //GET_FIRST();
        //(Nota Unific MySQL-Linux): En este caso es TT_HIST y no tipo_t_eventos ya que la variable
        //esta pensada para alternar entre TT_ORIG y TT_ENTR
        SELECT_TABLE( T_EVENTOS, TT_HIST );
		SET_WHERE("");
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
    }
    else {
        //area_ticket = AREA_TICKET;
        //SELECT( area_ticket );
        //SET_ORDER( 1 );
        //GET_GREATER( ( char* )&nro_ultimo_evento_controlado );
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
		SET_WHERE("ID_EVENTO > '%ld'",nro_ultimo_evento_controlado);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
    }
    while( cant_a_controlar && /*!BtrvEOF()*/ !dbEOF() ) {
        if( ( ticket->status == 1 && TIPO_EVENTO_A_PROCESAR( ticket->tipo_evento ) )
         || ( off_lines && EVENTO_A_PROCESAR( ticket ) ) ) {
            /*struct _id
            {
                long caja_z;
                long id_evento;
            } id;
            id.caja_z = ticket->caja_z;
            id.id_evento = ticket->id_evento;*/
            // Busca en coticket.btr
            //SELECT( AREA_AUX4 );
            SELECT_TABLE( T_COTICKET, TT_ORIG );
            //SET_ORDER( 4 );
            //GET_EQUAL( ( char* )&id );
            SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld'", ticket->caja_z, ticket->id_evento ); 
            if( !all ) {
                cant_a_controlar--;
            }
			if( FOUND2() ) {
                int iGrabar = 0;
                // Si lo encuentra, suma cantidad_encontrada para
                // no buscar en historicos
                cantidad_encontrada++;
                SELECT_TABLE( T_EVENTOS, z_anteriores? TT_HIST : tipo_t_eventos );
                if( ticket->reproceso == EVENTO_REPROCESAR_ENVIADO ) {
                    if( event_tmp.reproceso == 1 ) {
                        ticket->reproceso = EVENTO_REPROCESADO;
                        iGrabar = 1;
                    }
                    else {
                        iGrabar = 0;
                        ticket->reproceso = EVENTO_REPROCESAR_NO_ENVIADO;
                        #ifdef INVEL_L
						//BEGIN_TRANSACTION();
                        #endif
						UPDATE2();
                        #ifdef INVEL_L
                        //END_TRANSACTION();
                        #endif
                        if( !z_anteriores
                         && ( !NRO_EVENTO_A_PROCESAR
                           || ( NRO_EVENTO_A_PROCESAR
                             && event_ticket.id_evento < NRO_EVENTO_A_PROCESAR ) ) ) {
                            SET_MEMORY_LONG( __nro_evento_a_procesar, event_ticket.id_evento );
                            SET_MEMORY_LONG( __nro_ultimo_evento_controlado,
                                             NRO_EVENTO_A_PROCESAR - 1 );
                        }
                    }
                }
                else {
                    if( ticket->reproceso == EVENTO_REPROCESAR_NO_ENVIADO ) {
                        if( !z_anteriores
                         && ( !NRO_EVENTO_A_PROCESAR
                           || ( NRO_EVENTO_A_PROCESAR
                             && ticket->id_evento < NRO_EVENTO_A_PROCESAR ) ) ) {
                            SET_MEMORY_LONG( __nro_evento_a_procesar, event_ticket.id_evento );
                            SET_MEMORY_LONG( __nro_ultimo_evento_controlado,
                                             NRO_EVENTO_A_PROCESAR - 1 );
                        }
                        iGrabar = 0;
                    }
                    else {
                        iGrabar = 1;
                    }
                }
                if( iGrabar ) {
                    ticket->status = 2;
                    #ifdef INVEL_L
					//BEGIN_TRANSACTION();
                    #endif
					UPDATE2();
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                    if( !z_anteriores && ( !NRO_EVENTO_A_PROCESAR ) && !no_encontrada ) {
                        SET_MEMORY_LONG( __nro_ultimo_evento_controlado, ticket->id_evento );
                    }
                }
            }
            else {
                SELECT_TABLE( T_EVENTOS, z_anteriores? TT_HIST : tipo_t_eventos );
                no_encontrada = 1;
                if( marcar_no_encontradas ) {
                    ticket->status = 0;
                    if( ticket->reproceso == EVENTO_REPROCESAR_ENVIADO ) {
                        ticket->reproceso = EVENTO_REPROCESADO;
                    }
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
					UPDATE2();
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                    if( !z_anteriores
                     && ( !NRO_EVENTO_A_PROCESAR
                       || ( NRO_EVENTO_A_PROCESAR && ticket->id_evento < NRO_EVENTO_A_PROCESAR ) ) ) {
                        SET_MEMORY_LONG( __nro_evento_a_procesar, ticket->id_evento );
                        SET_MEMORY_LONG( __nro_ultimo_evento_controlado,
                                         NRO_EVENTO_A_PROCESAR - 1 );
                    }
                }
            }
        }
        SELECT_TABLE( T_EVENTOS, z_anteriores? TT_HIST : tipo_t_eventos );
		SKIP2(1);
    }
    //CLOSE_DATABASE( AREA_AUX4 );
    CLOSE_TABLE( T_COTICKET, TT_ORIG );
    //SELECT( area );
    SELECT_TABLE( tabla_ant, tipo_ant );
    return cantidad_encontrada;
}

