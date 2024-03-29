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
#include <cmemory.h>
#include <comprob.h>
#include <cr.h>
#include <cr_disco.h>
#include <dbrouter.h>
#include <eventos.h>
#include <export.h>
#include <get_fech.h>
#include <get_hora.h>
#include <log.h>
#include <malloc.h>
#include <mensaje.h>
#include <mstring.h>
#include <recupero.h>
#include <remito.h>
#include <round.h>
#include <tkt.h>
#include <t_pago.h>
#include <_cr1.h>
#include <aarch.h>
#include <bridge.h>
#include <path.h>
#include <math.h>
#include <t_pago.h>
#include <driver.h>
#include <fiscal.h>
#include <limits.h>
#include <float.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#else
#endif

#include "actions.h"
#include <path.h>
int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos, tipo_t_varios, tipo_t_ev_super,  tipo_ev_dpago, tipo_ev_cod_barra;
int tipo_t_ev_despro, tipo_t_ev_factu, tipo_t_ev_b_promo, tipo_t_cierre_lote, tipo_t_ev_cierre;
static char *ARRAY_NRO_EVENTO;
long OBTENER_POS_SUPER(long id_evento);
void MARCAR_ITEM_ORIGINAL_EN_E_TICKET( int h );
extern int POSICIONA_EN_TICKET( long nro_z, long id_evento );
extern int POSICIONA_EN_PAGO( long nro_z, long id_evento );
extern int POSICIONA_EN_COD_BARRA( long nro_z, long id_evento );
extern int POSICIONA_EN_E_TICKET( long nro_z, long id_evento );
extern int POSICIONA_EN_CIERRE( long nro_z, long id_evento );
extern int TIPO_EVENTO_A_PROCESAR( char tipo_evento );
extern POSICIONA_EN_SUPER( long nro_z, long id_evento );
extern POSICIONA_EN_EV_B_PROMO( long nro_z, long id_evento );
extern int POSICIONA_EN_E_DESPRO( long nro_z, long id_evento );
extern POSICIONA_EN_SUPER( long nro_z, long id_evento );
extern int POSICIONA_EN_DPAGO( long nro_z, long id_evento );
void CORREGIR_TICKET( int tabla);
void GRABAR_EVENT_TICKET_MINIMO( );	
extern int CantCambioMediosAProcesar;
extern int RECUPERAR_ALICUOTA_DE_ARTICULO( int tabla, int cod_exento, char *cod_barra, long cod_interno );
/***********************************************************************/
void ABRIR_ARCHIVOS_EVENTOS()
/***********************************************************************/
{
    int nError = 0;
    if( event_cierre == NULL ) {
        event_cierre = ( struct _ev_cierre *)MALLOC( sizeof( struct _ev_cierre ) );
    }
    if( event_dpago == NULL ) {
        event_dpago = (struct _ev_dmedios *)MALLOC( sizeof( struct _ev_dmedios ) );
    }
    if( event_super == NULL ) {
        event_super = (struct _ev_super *)MALLOC( sizeof( struct _ev_super ) );
    }
    if( event_despro == NULL ) {
        event_despro = (struct _ev_despro *)MALLOC( sizeof( struct _ev_despro ) );
    }
    if( event_factura == NULL ) {
        event_factura = (struct _ev_factu *)MALLOC( sizeof( struct _ev_factu ) );
    }
    if( event_cie_lote == NULL ) {
        event_cie_lote = (struct _cierre_tarjetas *)MALLOC( sizeof( struct _cierre_tarjetas ) );
    }
    /*if( event_b_promo == NULL ) {
        event_b_promo = MALLOC( sizeof( struct _cierre_tarjetas ) );
    }*/

    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        #ifdef COMPILAR_MODO_ENTRENAMIENTO
        MENSAJE_STRING( S_ABRIENDO_ARCH_ENTRENAM );
        nError = /*USE_DB( AREA_TICKET, _TICKET_BTR_ENTR, ( char* )&event_ticket,
                         sizeof( event_ticket ), NULL, 0, 0 );*/
				 OPEN_TABLE(T_EVENTOS, TT_ENTR,( char* )&event_ticket,sizeof( event_ticket ));
                 //OPEN_TABLE_ENTR( T_TICKET, T_TICKET_ENTR, ( char* )&event_ticket,sizeof( event_ticket ));
		tipo_t_eventos = TT_ENTR;
        nError = /*USE_DB( AREA_E_TICKET, _E_TICKET_BTR_ENTR, ( char* )&event_e_ticket,
                         sizeof( event_e_ticket ), NULL, 0, 0 );*/
				 OPEN_TABLE(T_EV_CONT, TT_ENTR,( char* )&event_e_ticket, sizeof( event_e_ticket ));
		tipo_t_ev_cont = TT_ENTR;
        nError = /*USE_DB( AREA_PAGO, _PAGO_BTR_ENTR, ( char* )&event_pago, sizeof( event_pago ),
                         NULL, 0, 0 );*/
				 OPEN_TABLE(T_PAGOS, TT_ENTR, (char* )&event_pago, sizeof( event_pago ));
		tipo_t_pagos = TT_ENTR;
        nError = /*USE_DB( AREA_VARIOS, _VARIOS_BTR_ENTR, ( char* )event_varios,
                         sizeof( struct _event_varios ), NULL, sizeof( struct _event_varios ), 0 );*/
                 OPEN_TABLE( T_VARIOS, TT_ENTR, ( char* )event_varios, sizeof( struct _event_varios ) );
        tipo_t_varios = TT_ENTR;
        nError = /*USE_DB( AREA_COD_BARRA, _EV_T_COMPRA_SIC_ENTR, ( char* )&event_cod_barra,
                         sizeof( event_cod_barra ), NULL, 0, 0 );*/
				 OPEN_TABLE( T_EV_COD_BARRA, TT_ENTR, ( char* )&event_cod_barra, sizeof( event_cod_barra ) );
        tipo_ev_cod_barra = TT_ENTR;
        nError = /*USE_DB( AREA_DPAGO, _EV_DPAGO_BTR_ENTR, ( char* )event_dpago,
                         sizeof( struct _ev_dmedios ), NULL, 0, 0 );*/
                 OPEN_TABLE( T_EV_DMEDIOS, TT_ENTR, ( char* )event_dpago, sizeof( struct _ev_dmedios ) );
        tipo_ev_dpago = TT_ENTR;
        nError = /*USE_DB( AREA_SUPER, _EV_SUPER_BTR_ENTR, ( char* )event_super,
                         sizeof( struct _ev_super ), NULL, 0, 0 );*/
                 OPEN_TABLE( T_EV_SUPER, TT_ENTR, ( char* )event_super, sizeof( struct _ev_super ) );
        tipo_t_ev_super = TT_ENTR;
        nError = /*USE_DB( AREA_E_DES_PRO, _E_DES_PRO_BTR_ENTR, ( char* )event_despro,
                         sizeof( struct _ev_despro ), NULL, 0, 0 );*/
                 OPEN_TABLE( T_EV_DESPRO, TT_ENTR, ( char* )event_despro, sizeof( struct _ev_despro ) );
        tipo_t_ev_despro = TT_ENTR;
        nError = /*USE_DB( AREA_FACTURA, _EV_FACTU_ENTR, ( char* )event_factura,
                         sizeof( struct _ev_factu ), NULL, 0, 0 );*/
                 OPEN_TABLE( T_EV_FACTU, TT_ENTR, ( char* )event_factura, sizeof( struct _ev_factu ) );
        tipo_t_ev_factu = TT_ENTR;
        nError = /*USE_DB( AREA_CIE_LOTE, _CIERRE_DE_LOTE_TARJ, ( char* )event_cie_lote,
                         sizeof( struct _cierre_tarjetas ), NULL, 0, 0 );*/
				 OPEN_TABLE( T_CIERRE_LOTE, TT_ENTR, ( char* )&event_cie_lote, sizeof( struct _cierre_tarjetas ) );
        tipo_t_cierre_lote = TT_ENTR;
        nError = /*USE_DB( AREA_EV_B_PROMO, _EV_B_PROMO_BTR, ( char* )&event_b_promo,
                         sizeof( struct _ev_bpromo ), NULL, 0, 0 );*/
                 OPEN_TABLE( T_EV_B_PROMO, TT_ENTR, ( char* )&event_b_promo, sizeof( struct _ev_bpromo ) );
        tipo_t_ev_b_promo = TT_ENTR;
        MENSAJE_STRING( S_ARCHIVOS_ABIERTOS );
        #endif
    }
    else {
        if( !nError ) {
            nError = /*USE_DB( AREA_TICKET, _TICKET_BTR, ( char* )&event_ticket,
                             sizeof( event_ticket ), NULL, 0, 0 );*/
					 OPEN_TABLE(T_EVENTOS, TT_ORIG,( char* )&event_ticket,sizeof( event_ticket ));
			tipo_t_eventos = TT_ORIG;
        }
        if( !nError ) {
            nError = /*USE_DB( AREA_E_TICKET, _E_TICKET_BTR, ( char* )&event_e_ticket,
                             sizeof( event_e_ticket ), NULL, 0, 0 );*/
					 OPEN_TABLE(T_EV_CONT, TT_ORIG,( char* )&event_e_ticket, sizeof( event_e_ticket ));
			tipo_t_ev_cont = TT_ORIG;
        }
        if( !nError ) {
            nError = /*USE_DB( AREA_PAGO, _PAGO_BTR, ( char* )&event_pago, sizeof( event_pago ), NULL,
                             0, 0 );*/
					 OPEN_TABLE(T_PAGOS, TT_ORIG,( char* )&event_pago, sizeof( event_pago ));
			tipo_t_pagos = TT_ORIG;
        }
        if( !nError ) {
			memset( event_varios, 0, sizeof(struct _event_varios) );
            nError = /*USE_DB( AREA_VARIOS, _VARIOS_BTR, ( char* )event_varios,
                             sizeof( struct _event_varios ), NULL, sizeof( struct _event_varios ),
                             0 );*/
                     OPEN_TABLE( T_VARIOS, TT_ORIG, ( char* )event_varios, sizeof( struct _event_varios ) );
            tipo_t_varios = TT_ORIG;
        }
        if( !nError ) {
            nError = /*USE_DB( AREA_COD_BARRA, _EV_T_COMPRA_SIC, ( char* )&event_cod_barra,
                             sizeof( event_cod_barra ), NULL, 0, 0 );*/
					 OPEN_TABLE( T_EV_COD_BARRA, TT_ORIG, ( char* )&event_cod_barra, sizeof( struct _ev_cod_barra ) );
    	    tipo_ev_cod_barra = TT_ORIG;
        }
        if( !nError ) {
            nError = /*USE_DB( AREA_CIERRE, _EV_CIERRE_BTR, ( char* )event_cierre,
                             sizeof( struct _event_cierre ), 0, 0, 0 );*/
					 OPEN_TABLE(T_EV_CIERRE, TT_ORIG,( char* )event_cierre,sizeof( struct _ev_cierre ));
        	tipo_t_ev_cierre = TT_ORIG;
        }
        if( !nError ) {
			memset( event_dpago, 0, sizeof(struct _ev_dmedios) );
            nError = /*USE_DB( AREA_DPAGO, _EV_DPAGO_BTR, ( char* )event_dpago,
                             sizeof( struct _ev_dmedios ), NULL, 0, 0 );*/
                     OPEN_TABLE( T_EV_DMEDIOS, TT_ORIG, (char* )event_dpago, sizeof( struct _ev_dmedios ) );
            tipo_ev_dpago = TT_ORIG;
        }
        if( !nError ) {
            nError = /*USE_DB( AREA_SUPER, _EV_SUPER_BTR, ( char* )event_super,
                             sizeof( struct _ev_super ), NULL, 0, 0 );*/
                    OPEN_TABLE( T_EV_SUPER, TT_ORIG, ( char * )event_super, sizeof( struct _ev_super ) );
            tipo_t_ev_super = TT_ORIG;
        }
        if( !nError ) {
			memset( event_despro, 0, sizeof(struct _ev_despro) );
            nError = /*USE_DB( AREA_E_DES_PRO, _E_DES_PRO_BTR, ( char* )event_despro,
                             sizeof( struct _ev_despro ), 0, 0, 0 );*/
                     OPEN_TABLE( T_EV_DESPRO, TT_ORIG, ( char* )event_despro, sizeof( struct _ev_despro ) );
            tipo_t_ev_despro = TT_ORIG;
        }
        if( !nError ) {
            nError = /*USE_DB( AREA_FACTURA, _EV_FACTU_BTR, ( char* )event_factura,
                             sizeof( struct _ev_factu ), 0, 0, 0 );*/
                     OPEN_TABLE( T_EV_FACTU, TT_ORIG, ( char* )event_factura, sizeof( struct _ev_factu ) );
            tipo_t_ev_factu = TT_ORIG;
        }
        if( !nError ) {
        	nError =/* USE_DB( AREA_CIE_LOTE, _CIERRE_DE_LOTE_TARJ, ( char* )event_cie_lote,
                             sizeof( struct _cierre_tarjetas ), NULL, 0, 0 );*/
					 OPEN_TABLE( T_CIERRE_LOTE, TT_ORIG, ( char* )&event_cie_lote, sizeof( struct _cierre_tarjetas ) );
        	tipo_t_cierre_lote = TT_ORIG;
        }
        if( !nError ) {
			memset( &event_b_promo, 0, sizeof(struct _ev_bpromo) );
            nError = /*USE_DB( AREA_EV_B_PROMO, _EV_B_PROMO_BTR, ( char* )&event_b_promo,
                             sizeof( struct _ev_bpromo ), NULL, 0, 0 );*/
                     OPEN_TABLE( T_EV_B_PROMO, TT_ORIG, ( char* )&event_b_promo, sizeof( struct _ev_bpromo ) );
            tipo_t_ev_b_promo = TT_ORIG;
        }
        ABRIR_ARCHIVOS_EVENTOS_HISTORICOS();
    }
    ABRIR_ARCHIVOS_EVENTOS_ALL();

    if( nError ) {
        /*char szText[50];
        sprintf( szText, "ev ERR %d", nError );
        GRABAR_LOG_DBROUTER( szText ,LOG_ERRORES,2);
        MENSAJE( szText );*/
		ABORT( "Error en Tablas de grabacion de Eventos" );
    }
}
/***********************************************************************/
void ABRIR_ARCHIVOS_EVENTOS_ALL()
/***********************************************************************/
{
    #ifdef COMPILAR_EVENTOS_ALL
    if( USAR_EVENTOS_ALL && !GET_ERROR_EN_RED() ) {
        USE_DB( AREA_TICKET_ALL, _TICKET_ALL, ( char* )&event_ticket_all,
                sizeof( event_ticket_all ), NULL, 0, 0 );
        if( !GET_ERROR_EN_RED() ) {
            USE_DB( AREA_E_TICKET_ALL, _E_TICKET_ALL, ( char* )&event_e_ticket_all,
                    sizeof( event_e_ticket_all ), NULL, 0, 0 );
        }
        if( !GET_ERROR_EN_RED() ) {
            USE_DB( AREA_PAGO_ALL, _PAGO_ALL, ( char* )&event_pago_all, sizeof( event_pago_all ),
                    NULL, 0, 0 );
        }
        if( !GET_ERROR_EN_RED() ) {
            USE_DB( AREA_VARIOS_ALL, _VARIOS_ALL, ( char* )event_varios_all,
                    sizeof( struct _event_varios ), NULL, sizeof( struct _event_varios ), 0 );
        }
    }
    #endif
}
/***********************************************************************/
int ABRIR_ARCHIVOS_EVENTOS_LOCAL()
/***********************************************************************/
{
    int error = 0;
    if( event_cierre == NULL ) {
        event_cierre = MALLOC( sizeof( struct _ev_cierre ) );
    }
    if( event_dpago == NULL ) {
        event_dpago = MALLOC( sizeof( struct _ev_dmedios ) );
    }
    if( event_super == NULL ) {
        event_super = MALLOC( sizeof( struct _ev_super ) );
    }
    if( event_despro == NULL ) {
        event_despro = MALLOC( sizeof( struct _ev_despro ) );
    }
    if( event_factura == NULL ) {
        event_factura = MALLOC( sizeof( struct _ev_factu ) );
    }
    /*if( event_b_promo == NULL ) {
        event_b_promo = MALLOC( sizeof( struct _ev_bpromo ) );
    }*/
    error += /*USE_DB( AREA_TICKET, _TICKET_BTR, ( char* )&event_ticket, sizeof( event_ticket ), NULL,
                     0, 0 );*/
			 OPEN_TABLE(T_EVENTOS, TT_ORIG,( char* )&event_ticket,sizeof( event_ticket ));
	tipo_t_eventos = TT_ORIG;
    error += /*USE_DB( AREA_PAGO, _PAGO_BTR, ( char* )&event_pago, sizeof( event_pago ), NULL, 0, 0 );*/
			 OPEN_TABLE(T_PAGOS, TT_ORIG,( char* )&event_pago, sizeof( event_pago ));
	tipo_t_pagos = TT_ORIG;
    error += /*USE_DB( AREA_VARIOS, _VARIOS_BTR, ( char* )event_varios,
                     sizeof( struct _event_varios ), NULL, sizeof( struct _event_varios ), 0 );*/
             OPEN_TABLE( T_VARIOS, TT_ORIG, ( char* )event_varios, sizeof( struct _event_varios ) );
    tipo_t_varios = TT_ORIG;
    error += /*USE_DB( AREA_COD_BARRA, _EV_T_COMPRA_SIC, ( char* )&event_cod_barra,
                     sizeof( event_cod_barra ), NULL, 0, 0 );*/
			 OPEN_TABLE( T_EV_COD_BARRA, TT_ORIG, ( char* )&event_cod_barra, sizeof( struct _ev_cod_barra ) );
	tipo_ev_cod_barra = TT_ORIG;
    error += /*USE_DB( AREA_CIERRE, _EV_CIERRE_BTR, ( char* )event_cierre,
                     sizeof( struct _event_cierre ), 0, 0, 0 );*/
			 OPEN_TABLE(T_EV_CIERRE, TT_ORIG,( char* )event_cierre,sizeof( struct _ev_cierre ));
	tipo_t_ev_cierre = TT_ORIG;
    error += /*USE_DB( AREA_DPAGO, _EV_DPAGO_BTR, ( char* )event_dpago, sizeof( struct _ev_dmedios ),
                     NULL, 0, 0 );*/
             OPEN_TABLE( T_EV_DMEDIOS, TT_ORIG, ( char* )event_dpago, sizeof( struct _ev_dmedios ) );
    tipo_ev_dpago = TT_ORIG;
    error += /*USE_DB( AREA_SUPER, _EV_SUPER_BTR, ( char* )event_super, sizeof( struct _ev_super ),
                     NULL, 0, 0 );*/
             OPEN_TABLE( T_EV_SUPER, TT_ORIG, ( char * )event_super, sizeof( struct _ev_super ) );
    tipo_t_ev_super = TT_ORIG;
    error += /*USE_DB( AREA_E_TICKET, _E_TICKET_BTR, ( char* )&event_e_ticket,
                     sizeof( event_e_ticket ), NULL, 0, 0 );*/
			 OPEN_TABLE(T_EV_CONT, TT_ORIG,( char* )&event_e_ticket, sizeof( event_e_ticket ));
	tipo_t_ev_cont = TT_ORIG;
    error += /*USE_DB( AREA_E_DES_PRO, _E_DES_PRO_BTR, ( char* )event_despro,
                     sizeof( struct _ev_despro ), NULL, 0, 0 );*/
             OPEN_TABLE( T_EV_DESPRO, TT_ORIG, ( char* )event_despro, sizeof( struct _ev_despro ) );
    tipo_t_ev_despro = TT_ORIG;
    error += /*USE_DB( AREA_FACTURA, _EV_FACTU_BTR, ( char* )event_factura,
                     sizeof( struct _ev_factu ), NULL, 0, 0 );*/
             OPEN_TABLE( T_EV_FACTU, TT_ORIG, ( char* )event_factura, sizeof( struct _ev_factu ) );
    tipo_t_ev_factu = TT_ORIG;
    error = /*USE_DB( AREA_EV_B_PROMO, _EV_B_PROMO_BTR, ( char* )&event_b_promo,
                             sizeof( struct _ev_bpromo ), NULL, 0, 0 );*/
             OPEN_TABLE( T_EV_B_PROMO, TT_ORIG, ( char* )&event_b_promo, sizeof( struct _ev_bpromo ) );
    tipo_t_ev_b_promo = TT_ORIG;
    ABRIR_ARCHIVOS_EVENTOS_HISTORICOS();
    return 0;
}
/***********************************************************************/
int ABRIR_ARCHIVOS_EVENTOS_BACKUP()
/***********************************************************************/
{
    int error = 0;
    if( /*USE_DB( AREA_TICKET, _TICKET_BAK_ULTIMO, ( char* )&event_ticket, sizeof( event_ticket ),
                NULL, 0, 0 )*/
		OPEN_TABLE(T_EVENTOS, TT_BAK,( char* )&event_ticket,sizeof( event_ticket )) != 0 ) {
        error = 1;
	}else{
		tipo_t_eventos = TT_BAK;
	}
    if( /*USE_DB( AREA_E_TICKET, _E_TICKET_BAK_ULTIMO, ( char* )&event_e_ticket,
                sizeof( event_e_ticket ), NULL, 0, 0 )*/
		OPEN_TABLE(T_EV_CONT, TT_BAK, ( char* )&event_e_ticket, sizeof( event_e_ticket ))!= 0 ) {
        error = 1;
	}else{
		tipo_t_ev_cont = TT_BAK;
	}
    if( /*USE_DB( AREA_PAGO, _PAGO_BAK_ULTIMO, ( char* )&event_pago, sizeof( event_pago ), NULL, 0,
                0 )*/
		OPEN_TABLE(T_PAGOS, TT_BAK,( char* )&event_pago, sizeof( event_pago ))!= 0 ) {
        error = 1;
    }else{
		tipo_t_pagos = TT_BAK;
	}
    if( /*USE_DB( AREA_VARIOS, _VARIOS_BAK_ULTIMO, ( char* )event_varios,
                sizeof( struct _event_varios ), NULL, sizeof( struct _event_varios ), 0 ) != 0 )*/
        OPEN_TABLE( T_VARIOS, TT_BAK, (char* )event_varios, sizeof( struct _event_varios ) ) != 0 )  {
        error = 1;
    }
    else {
        tipo_t_varios = TT_BAK;
    }
    /*   if( USE_DB( AREA_E_DES_PRO, _E_DES_PRO_BTR_BAK, ( char * ) event_despro,
             sizeof( struct _ev_despro ), NULL, 0, 0 ) != 0 )
          error = 1;
       if( USE_DB( AREA_FACTURA, _EV_FACTU_BAK, ( char * ) event_factura,
             sizeof( struct _ev_factu ), NULL, 0, 0 ) != 0 )
          error = 1;
    */
    return error;
}
/***********************************************************************/
void CERRAR_ARCHIVOS_EVENTOS_LOCAL()
/***********************************************************************/
{
    //CLOSE_DATABASE( AREA_TICKET );
    //CLOSE_DATABASE( AREA_E_TICKET );
	CLOSE_TABLE( T_EVENTOS, tipo_t_eventos );
	CLOSE_TABLE( T_EV_CONT, tipo_t_ev_cont );
    //CLOSE_DATABASE( AREA_PAGO );
	CLOSE_TABLE( T_PAGOS, tipo_t_pagos);
    //CLOSE_DATABASE( AREA_VARIOS );
    CLOSE_TABLE( T_VARIOS, tipo_t_varios );
    CLOSE_TABLE(T_EV_COD_BARRA, tipo_ev_cod_barra );
	//CLOSE_DATABASE( AREA_COD_BARRA );
    //CLOSE_DATABASE( AREA_CIERRE );
	CLOSE_TABLE( T_EV_CIERRE, TT_ORIG );
    //CLOSE_DATABASE( AREA_DPAGO );
    CLOSE_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
    //CLOSE_DATABASE( AREA_SUPER );
    CLOSE_TABLE( T_EV_SUPER, tipo_t_ev_super );
    //CLOSE_DATABASE( AREA_E_DES_PRO );
    CLOSE_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
    //CLOSE_DATABASE( AREA_FACTURA );
    CLOSE_TABLE( T_EV_FACTU, tipo_t_ev_factu );
    //CLOSE_DATABASE( AREA_EV_B_PROMO );
    CLOSE_TABLE( T_EV_B_PROMO, tipo_t_ev_b_promo );

    if( event_cierre != NULL ) {
        FREEMEM( ( char* )event_cierre );
        event_cierre = NULL;
    }
    if( event_dpago != NULL ) {
        FREEMEM( ( char* )event_dpago );
        event_dpago = NULL;
    }
    if( event_super != NULL ) {
        FREEMEM( ( char* )event_super );
        event_super = NULL;
    }
    if( event_despro != NULL ) {
        FREEMEM( ( char* )event_despro );
        event_despro = NULL;
    }
    if( event_factura != NULL ) {
        FREEMEM( ( char* )event_factura );
        event_factura = NULL;
    }
    CERRAR_ARCHIVOS_EVENTOS_HISTORICOS();
}
/***********************************************************************/
void CERRAR_ARCHIVOS_EVENTOS()
/***********************************************************************/
{
    //CLOSE_DATABASE( AREA_TICKET );
    CLOSE_TABLE( T_EVENTOS, tipo_t_eventos );
    //CLOSE_DATABASE( AREA_E_TICKET );
    CLOSE_TABLE( T_EV_CONT, tipo_t_ev_cont );
    //CLOSE_DATABASE( AREA_PAGO );
	CLOSE_TABLE( T_PAGOS, tipo_t_pagos );
    //CLOSE_DATABASE( AREA_VARIOS );
    CLOSE_TABLE( T_VARIOS, tipo_t_varios );
    //CLOSE_DATABASE( AREA_COD_BARRA );
	CLOSE_TABLE(T_EV_COD_BARRA, tipo_ev_cod_barra );
    //CLOSE_DATABASE( AREA_CIERRE );
	CLOSE_TABLE( T_EV_CIERRE, TT_ORIG );
    //CLOSE_DATABASE( AREA_DPAGO );
    CLOSE_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
    //CLOSE_DATABASE( AREA_SUPER );
    CLOSE_TABLE( T_EV_SUPER, tipo_t_ev_super );
    //CLOSE_DATABASE( AREA_E_DES_PRO );
    CLOSE_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
    //CLOSE_DATABASE( AREA_FACTURA );
    CLOSE_TABLE( T_EV_FACTU, tipo_t_ev_factu );
    //CLOSE_DATABASE( AREA_EV_B_PROMO );
    CLOSE_TABLE( T_EV_B_PROMO, tipo_t_ev_b_promo );

    CERRAR_ARCHIVOS_EVENTOS_ALL();
    if( event_cierre != NULL ) {
        FREEMEM( ( char* )event_cierre );
        event_cierre = NULL;
    }
    if( event_dpago != NULL ) {
        FREEMEM( ( char* )event_dpago );
        event_dpago = NULL;
    }
    if( event_super != NULL ) {
        FREEMEM( ( char* )event_super );
        event_super = NULL;
    }
    if( event_despro != NULL ) {
        FREEMEM( ( char* )event_despro );
        event_despro = NULL;
    }
    if( event_factura != NULL ) {
        FREEMEM( ( char* )event_factura );
        event_factura = NULL;
    }
    CERRAR_ARCHIVOS_EVENTOS_HISTORICOS();
    /*SET_FECHA_ARCHIVO( "TICKET.BTR", GET_FECHA_BACKUP() );
    SET_FECHA_ARCHIVO( "E_TICKET.BTR", GET_FECHA_BACKUP() );
    SET_FECHA_ARCHIVO( "PAGO.BTR", GET_FECHA_BACKUP() );
    SET_FECHA_ARCHIVO( "VARIOS.BTR", GET_FECHA_BACKUP() );
    SET_FECHA_ARCHIVO( "E_DESPRO.BTR", GET_FECHA_BACKUP() );
    SET_FECHA_ARCHIVO( "EV_FACTU.BTR", GET_FECHA_BACKUP() );
    SET_FECHA_ARCHIVO( "SUPER.BTR", GET_FECHA_BACKUP() );
    SET_FECHA_ARCHIVO( "DPAGO.BTR", GET_FECHA_BACKUP() );*/
}
/***********************************************************************/
void CERRAR_ARCHIVOS_EVENTOS_ALL()
/***********************************************************************/
{
    #ifdef COMPILAR_EVENTOS_ALL
    if( USAR_EVENTOS_ALL && !GET_ERROR_EN_RED() ) {
        CLOSE_DATABASE( AREA_TICKET_ALL );
        if( !GET_ERROR_EN_RED() ) {
            CLOSE_DATABASE( AREA_E_TICKET_ALL );
        }
        if( !GET_ERROR_EN_RED() ) {
            CLOSE_DATABASE( AREA_PAGO_ALL );
        }
        if( !GET_ERROR_EN_RED() ) {
            CLOSE_DATABASE( AREA_VARIOS_ALL );
        }
    }
    #endif
}
/***********************************************************************/
void ACTUALIZA_ID_EVENTO()
/***********************************************************************/
{
}
/***********************************************************************/
void ACTUALIZA_ID_EVENTO_NEW()
/***********************************************************************/
{
    int /*area,*/ maximoId = 0, ram_cierre_y = 0, cajeros_flotantes = 0;

    if( event_ticket.tipo_evento == 3 || event_ticket.tipo_evento == 4
     || event_ticket.tipo_evento == 5 || event_ticket.tipo_evento == 6
	 ||( event_ticket.tipo_evento == 21 && (event_ticket.tipo_evento_importado >= 3
				  && event_ticket.tipo_evento_importado <= 4 ) ) ) {
        SET_MEMORY_LONG( __ram_id_ultimo_evento_factura, RAM_ID_EVENTO );
    }
    SET_MEMORY_LONG( __ram_id_ultimo_evento, RAM_ID_EVENTO );
    if(DIRECT_DB_QUERY("SELECT max(id_evento) FROM EVENTOS") > 0){ //OBTIENE EL MAXIMO ID EVENTO
        GET_DATO(1,C_INT,(char *)&maximoId,4);
        SET_MEMORY_LONG( __ram_id_evento, maximoId + 1 );
    }
    else {
        SET_MEMORY_LONG( __ram_id_evento, 1 );    // NRO_CAJA * 1000000L
        //Para servicios la transaccion inicial se utiliza para determinar
        //el numero de id_evento del ultimo cierre X o Z
        if( MODO_NEGOCIO == SERVICIOS ) {
            SET_MEMORY_LONG( __x_nro_transaccion_inicial, RAM_ID_EVENTO );
        }
    }
    //SELECT( area );
    GRABAR_LOG_SISTEMA( "fin actualiz id_evento" ,LOG_VENTAS,4);
    if( event_ticket.tipo_evento_importado == _EVENT_TICKET_ESPERA ) {
        MENSAJE_STRING_SIN_PAUSA(S_PROCESANDO_SCRIPT);
        SLEEP(8000); //8 segundos aprox.
    }
	//Se agrega esta condicion para que tome la fecha NRO_FECHA_OPERACION 
	//que se setea en la grabacion del supervisor que interviene en el Tipo de Evento 9. 
	//Con la fecha mencionada grabara tambien el Tipo de evento 8 (del Cierre Y) 
	//y Tipo de evento 9 --> Esto dejo de funcionar cuando cerearon la variable RAM_CIERRE_Y = 0
	ram_cierre_y = RAM_CIERRE_Y;
	cajeros_flotantes = CAJEROS_FLOTANTES;
	if( !( ram_cierre_y && !cajeros_flotantes ) ) {
		//Se agrega esta condicion para que el tipo de evento 8 se grabe con la la fecha NRO_FECHA_OPERACION del tipo de evento 9.
		//Esta situacion esta solucionada (en versiones mas avanzadas) con el seteo de la variable RAM_CIERRE_Y
		if( !( event_ticket.tipo_evento == 8 &&  NRO_MODO == 'Y' )
			//ev_cierre graba despues que eventos de manera que no se puede cerear NRO_FECHA_OPERACION
			&& !( MANTENER_FECHA_Z == 0 && event_ticket.tipo_evento == 9 )//Se agrega esto porque 1ero graba eventos y luego ev_cierre
			&& !( MANTENER_FECHA_Z && ( event_ticket.tipo_evento == 9 || event_ticket.tipo_evento == 27 ) ) ) {//LA ULTIMA CONDICION es para el cierre Z no ceree la fecha operacion
		   SET_MEMORY_INT( __nro_fecha_operacion, 0 );
		   GRABAR_LOG_SISTEMA( "NRO FECHA OPERACION cereado" ,LOG_VENTAS,4);
		}
	}
	
    //SET_MEMORY_INT( __nro_fecha_operacion, 0 );
	SET_MEMORY_INT( __ram_instancia_evento, 0 );
}
/***********************************************************************/
int GRABAR_EVENT_TICKET( int grabar_all )
/***********************************************************************/
{
    int i, error = 0, existe_evento = NO, sec_incrementada = NO, maximaSec = -999;
	int nro_fecha_operacion = 0;
    double puntos = 0.00;
    char aux[15], cadena[100];
    struct _eventos event_ticket_recup; //estructura del ticket recuperado

	memset( aux, 0, sizeof( aux ) );
	memset( cadena, 0, sizeof( cadena ) );
	memset( &event_ticket_recup, 0, sizeof( struct _eventos ) );

    nro_fecha_operacion = NRO_FECHA_OPERACION;
	if( !nro_fecha_operacion )
		SET_MEMORY_INT( __nro_fecha_operacion, _GET_FECHA_DOS(  ) );

    if( USAR_EVENTOS_ALL == 2 ) {
        grabar_all = SI;
    }
    if( ( event_ticket.tipo_evento >= 26 && event_ticket.tipo_evento <= 29 ) ) {
        SET_MEMORY_INT( __ram_hora_inicio_evento, GET_HORA_DOS() );
    }
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    event_ticket.id_evento = RAM_ID_EVENTO;
//	if( CONTROLAR_SERIAL_PRINTER ){
		//numero obtenido
		_snprintf( event_ticket.serial_impresora, 
				sizeof(event_ticket.serial_impresora)-1, "%s",RAM_NRO_SERIE_IMPRESORA );
		//numero configurado y autorizado
		_snprintf( event_ticket.serial_impresora_ant, 
			sizeof(event_ticket.serial_impresora_ant)-1, "%s",RAM_NRO_SERIE_IMPRESORA_ANT );
//	}

    event_ticket.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    // event_ticket.fecha = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
    event_ticket.fecha = NRO_FECHA_OPERACION; //fecha actual
    event_ticket.nro_dia_operativo =( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : NRO_FECHA_OPERACION;
    /*if( NRO_DIA_OPERATIVO ) {
        event_ticket.nro_dia_operativo = NRO_DIA_OPERATIVO;
    }
    else {
        event_ticket.nro
		_dia_operativo = GET_FECHA_DOS();
    }*/

#ifdef FECHA_DEMO
	{
		if( event_ticket.fecha > FECHA( FECHA_FINALIZACION_DIA, FECHA_FINALIZACION_MES, FECHA_FINALIZACION_ANIO )
			||_GET_FECHA_DOS(  )>FECHA( FECHA_FINALIZACION_DIA, FECHA_FINALIZACION_MES, FECHA_FINALIZACION_ANIO )
			){
				GRABAR_LOG_SISTEMA( "LA VERSION HA CADUCADO" , LOG_DEBUG,2 );
				ABORT( "LA VERSION HA CADUCADO" );
		
		}

	}
#endif
    event_ticket.hora = GET_HORA_DOS();
    event_ticket.cajero = NRO_CAJERO;
    event_ticket.sucursal = config.sucursal;
    if( MODO_NEGOCIO != SERVICIOS ) {
        event_ticket.nro_rollo = NRO_ROLLO;
    }
    event_ticket.cod_financiacion = NRO_COD_CUOTA;
	event_ticket.nro_evento_anterior = NRO_EVENTO_ANTERIOR;
    event_ticket.nro_boleta_anterior = RAM_BOLETA_ANTERIOR;
    event_ticket.nro_sucursal_anterior = RAM_SUCURSAL_ANTERIOR;
    event_ticket.nro_caja_anterior = RAM_CAJA_ANTERIOR;
    event_ticket.cambio = ( RAM_CAMBIO ) ? 1 : 0;
    event_ticket.nro_caja_dgi = NRO_CAJA_DGI;
    if( USAR_LEGAJO == RUT ) {
        strcpy( aux, RAM_DOC_CAJERO );
        event_ticket.nro_legajo_cajero = atol( aux );
    }
    else {
        event_ticket.nro_legajo_cajero = NRO_LEGAJO_CAJERO;
    }
    event_ticket.caja = NRO_CAJA;
    event_ticket.nro_z = NRO_Z;
    if( MODO_NEGOCIO != SERVICIOS ) {
        event_ticket.bien_de_uso = ( RAM_BIEN_DE_USO ) ? 'S' : 'N';
    }
    event_ticket.fecha_anterior = ( RAM_FECHA_ANTERIOR > 0 ) ? RAM_FECHA_ANTERIOR : 0;
    event_ticket.tipo_comprobante_anterior = RAM_TIPO_COMP_ANTERIOR;
    event_ticket.diferencia_redondeo = RAM_DIFERENCIA_REDONDEO;
    event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO;
    memcpy( event_ticket.str_cliente_afinidad, RAM_STRING_CLIENTE_AFINIDAD,
            sizeof( event_ticket.str_cliente_afinidad ) );
    event_ticket.puntos_usados = RAM_PUNTOS_USADOS;
    puntos = RAM_ACU_PUNTOS;
    event_ticket.puntos = ( float )puntos;
	if( event_ticket.tipo_evento != 99)
		event_ticket.monto_donacion = ( float )RAM_TOTAL_DONACION;
    switch( RAM_MODO_DEVOLUCION ) {
        case _NC_TOTAL:
        case _NC_RF_INTERME:
        case _DEVOL_TOTAL:
        case _DEVOL_INTERME:
            event_ticket.motivo =  RAM_MOTIVO_DEVOLUCION;
            break;
        default:
        if( event_ticket.tipo_evento == 99 && RAM_MOTIVO_DEVOLUCION ) {
            event_ticket.motivo = RAM_MOTIVO_DEVOLUCION;
        } else {
            //Esta tambien el caso en que el tipo de evento es 99 y no hay motivo conf.
            event_ticket.motivo = 0;
        }
    }

    for( i = 0;i < 5;i++ ) {
        event_ticket.cant_promo_afinidad[i] = _RAM_PROMOCIONES_AFINIDAD_USADAS( i );
    }
    /*for( i = 0;i < 4;i++ ) {
        event_ticket.monto_impuesto[i] = _TOT_TICK_IMPUESTOS( i );
    }*/
    if( event_ticket.tmpo_pago == 0 ) {
        event_ticket.tmpo_pago = RAM_TMPO_PAGO;
    }
    if( event_ticket.tmpo_reg_articulo == 0 ) {
        event_ticket.tmpo_reg_articulo = RAM_TMPO_REG_ARTICULO;
    }
    //memset( event_ticket.reserva, 0, sizeof( event_ticket.reserva ) );
    if( grabar_all && USAR_EVENTOS_ALL ) {
        event_ticket.id_all = RAM_ID_EVENTO_ALL;
    }

    if( CANTIDAD_REGISTROS_REC( ARCH_REC_IMPORT, sizeof( struct _eventos ) ) ) {
        //si existe una importacion
        //recupera la estructura ticket del evento importado
        RECUPERAR_ARCHIVO_REC( ARCH_REC_IMPORT, ( char* )&event_ticket_recup,
                               sizeof( struct _eventos ), 0 );
        event_ticket.cajaz_importado = event_ticket_recup.caja_z;
        event_ticket.id_evento_importado = event_ticket_recup.id_evento;
        if( MODO_DEVOLUCION && event_ticket_recup.tipo_evento == _EVENT_TICKET_ESPERA && event_ticket_recup.tipo_evento_importado == 4 )
          event_ticket.tipo_evento_importado = event_ticket_recup.tipo_evento_importado;
		else{
			event_ticket.tipo_evento_importado = event_ticket_recup.tipo_evento;
			if( event_ticket.tipo_evento == 99 && event_ticket_recup.tipo_evento == _EVENT_TICKET_ESPERA && RAM_COD_FUNCION != 4 ){//RAM_COD_FUNCION == 38 
				//con esta modificacion no marca los recuperados de espera que dan error y quedan disponibles para reimporta dee spera nuevamente,
				//si los recuperan y anula, quedan anulados sin posibilidad de recuperar , por le cambio de funcion en la condicion!!
				event_ticket.id_evento_importado = 0;//--0
			}
		}
		BORRAR_ARCHIVO_REC(ARCH_REC_IMPORT);
    }else{
        event_ticket.cajaz_importado = 0;
        event_ticket.id_evento_importado = 0;
        event_ticket.tipo_evento_importado = 0;
    }
    if( RAM_EN_IMPORTACION_DE_TICKET == 3 ){
        event_ticket.importado = 1;
    }
    if( RAM_TOTAL_DONACION > 0 ){
        event_ticket.cod_institucion = RAM_INSTITUCION_BENEFICIARIA;
    }else{
        event_ticket.cod_institucion = 0;
    }
	
	{//Controlar el tama�o del campo nro_comprobante_new
		char nro[12];//No debe superar los 10 digitos porque es lo maximo que el campo del servidor acepta
		memset(nro,0,sizeof(nro));
		_snprintf( nro, sizeof(nro)-2,"%lf",event_ticket.nro_comprobante_new );
		event_ticket.nro_comprobante_new = atof(nro);
	}

    #ifdef INVEL_L
    //BEGIN_TRANSACTION();
    #endif
	//Recupero despues de un corte de luz
	//Consultamos si fue confirmado la grabacion del evento
	if( !( RAM_INSTANCIA_EVENTO & _INSERT_POS_EVENTO ) ) {
		if( INCREMENTAR_NRO_EVENTO( event_ticket.tipo_evento ) ) {
			//Obtengo la max sec grabada
			if( NRO_EVENTO && DIRECT_DB_QUERY( "SELECT max( NRO_SECUENCIA ) FROM EVENTOS" ) > 0 ) {
				GET_DATO(1,C_INT,(char *)&maximaSec,4);
								//Cuando la consulta no devuelve un registo, GET_DATO no setea maximaSec sino que 
				//conserva su valor original para nuestro caso -999. Esto hace que setee el nro de 
				//secuencia con el valor de NRO_EVENTO ultimo. Para el caso de 1er evento tipo 26 
				//con incremento de secuencia, la secuencia asumiria el valor del NRO_EVENTO ultimo
				//(Tarea 5682), por esa razon se corrige la situacion.			
				if( maximaSec == -999 )
					maximaSec = 0;//No hay registro por consecuencia la secuencia es cero.

			}
		}
		//Consultamos si se grabo el evento sin confirmar
		if( RAM_INSTANCIA_EVENTO & _INSERT_PRE_EVENTO ) {
			//Consulto si existe el evento grabado
			if( DIRECT_DB_QUERY("SELECT * FROM EVENTOS WHERE ID_EVENTO = '%ld'", RAM_ID_EVENTO ) > 0 ) {
				existe_evento = SI;	//Entonces grabo la secuencia normalmente.
				sec_incrementada = SI;
			} else {
				//Si no grabo el evento, puede que la secuencia se haya incrementado
				//Obtengo la max sec grabada
				if( maximaSec != -999 ) {
					if( NRO_EVENTO == maximaSec ) {
						//Si la max sec grabada es igual al NRO_EVENTO
						//entonces no se incremento la secuencia
						sec_incrementada = NO;
					} else {
						if( fabs( NRO_EVENTO - maximaSec ) > 1 ) {
							//Si la max sec grabada es mayor al NRO_EVENTO en mas de 1 
							//entonces se incremento la secuencia 
							sec_incrementada = NO;
						} else {
							//Si la max sec grabada es mayor al NRO_EVENTO en 1 
							//entonces se incremento la secuencia 
							sec_incrementada = SI;
						}
					}
				}
			}
		} else {
			ADD_MEMORY_INT( __ram_instancia_evento, _INSERT_PRE_EVENTO );
		}
		//En corte de luz, si el evento ya existe entonces la secuencia ya se incremento. 
		if( !sec_incrementada && INCREMENTAR_NRO_EVENTO( event_ticket.tipo_evento ) ) {
			#ifdef COMPILAR_RECUPERO
				MENSAJE_TECLA( "Secuencia sin Incrementar, Presione una tecla si desea continuar" );
			#endif
			SET_MEMORY_LONG( __nro_evento, ( maximaSec != -999 )? maximaSec + 1: NRO_EVENTO + 1 );
			if( NRO_EVENTO > 99999999L
					|| ( LIMITE_MAXIMO_NUMERO_DE_EVENTO 
					&& ( NRO_EVENTO > LIMITE_MAXIMO_NUMERO_DE_EVENTO ) ) ) {
				SET_MEMORY_LONG( __nro_evento, 1L );
			}
			#ifdef COMPILAR_RECUPERO
				MENSAJE_TECLA( "Secuencia Incrementada, Presione una tecla si desea continuar" );
			#endif
		}
		
		event_ticket.nro_evento = ( ( INCREMENTAR_NRO_EVENTO( event_ticket.tipo_evento ) ) ? 
					( ( NRO_EVENTO )? ( NRO_EVENTO ) : 1 ) : 0 );
		#ifdef COMPILAR_RECUPERO
			if( !existe_evento )
				MENSAJE_TECLA( "Evento antes de ser grabado , Presione una tecla si desea continuar" );
		#endif
		
		//Informo a JSincro que debe suspender su  actividad hasta
		//que finalice actualice las fechas de los eventos. 
		//Lo hago por cada tipo de evento porque cuando se cambia la fecha,
		//Jsincro da por vencido el tiempo del script_pos y lo elimina
		/*if( ( event_ticket.tipo_evento == 8 &&  NRO_MODO == 'Y' )
			||event_ticket.tipo_evento == 9 
			|| event_ticket.tipo_evento == 27  
			|| event_ticket.tipo_evento == 10 ) {
			char _where[50];
			memset( _where, 0, sizeof( _where ) );
			_snprintf( _where, sizeof( _where ), "%ld",300000 );
			_where[sizeof(_where)-1] = '\0';
			INSERT_SCRIPT_POS( SPOS_PAUSA_JSINCRO, _where );	
		}*/
		if( !existe_evento ) {
			error = INSERT2(NO);
			#ifdef COMPILAR_RECUPERO
				if( !error ) {
					MENSAJE_TECLA( "Evento grabado sin Confirmar, Presione una tecla si desea continuar" );
				}
			#endif
		}
	}

	_snprintf( cadena, sizeof(cadena)-1,"MSJ EVENTO --> ID_EVENTO %ld TIPO_EVENTO %i NRO_SECUENCIA %ld",
			event_ticket.id_evento, event_ticket.tipo_evento, event_ticket.nro_evento );
	GRABAR_LOG_SISTEMA( cadena ,LOG_ERRORES,2);
	if( !error ) {
		ADD_MEMORY_INT( __ram_instancia_evento, _INSERT_POS_EVENTO );
		#ifdef COMPILAR_RECUPERO
			MENSAJE_TECLA( "Evento grabado Confirmado, Presione una tecla si desea continuar" );
		#endif

	} else {
		glog("NO INSERTO EVENTO EN TABLA LA EVENTO ",LOG_DEBUG,1);
		if(error == 2) { //error de campos
			CORREGIR_TICKET(0);
		} else { //clave duplicada
			ACTUALIZA_ID_EVENTO_NEW();                       
			event_ticket.id_evento = RAM_ID_EVENTO;
		}
		error = INSERT2(NO); //reintento
		if( error > 0 ) {
			MENSAJE( ST(S_ERROR_GRABANDO_EVENTO ) );
			GRABAR_LOG_SISTEMA( ST(S_ERROR_GRABANDO_EVENTO ) ,LOG_ERRORES,2);
			PAUSA( 18 );
		}
		if( error == 2 ) { //no alcanzo la correccion grabamos un evento vacio
			memset( &event_ticket, 0, sizeof( event_ticket ) );
		    event_ticket.nro_comprobante_new = 0;
			event_ticket.tipo_evento = 12;  //ANULACION COMPROBANTE  _EVENT_ANULA_COMPROBANTE
		    GRABAR_EVENT_TICKET_MINIMO();
			error = INSERT2(NO); //reintento final
			glog(ST(S_REINTENTAR_S_N), LOG_ERRORES,2);
		}
		memset( cadena, 0, sizeof( cadena ) );
		if( error ) {
			_snprintf( cadena, sizeof(cadena)-1,"NO SE LOGRO INSERTAR EL EVENTO con Error:%i", error );
			glog(cadena,LOG_ERRORES,1);
			GRABAR_LOG_BD(cadena, SI ,LOG_ERRORES,1);
		} else {
			glog("SE LOGRO INSERTAR EL EVENTO",LOG_ERRORES,1);
			GRABAR_LOG_BD("SE LOGRO INSERTAR EL EVENTO", SI ,LOG_ERRORES,1);
		}
	}
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
    #ifdef COMPILAR_EVENTOS_ALL
    if( grabar_all && USAR_EVENTOS_ALL && !GET_ERROR_EN_RED() ) {
        memcpy( &event_ticket_all, &event_ticket, sizeof( event_ticket_all ) );
        event_ticket_all.id_evento = RAM_ID_EVENTO_ALL;
        SELECT( AREA_TICKET_ALL );
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        INSERT();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    #endif
	return ( error );
}
/***********************************************************************/
void GRABAR_EVENT_E_TICKET( int h, int grabar_all, int cod_barra_art, char origen, int tipo, int marcar_item  )
/***********************************************************************/
{
    double signo;
    float cant, cant_dif = 1;
    float tasas;
    int i=0, error = 0,_iva=0;
    char cod_barra[17],	cadena[100];
	char cad[150];

	memset( cod_barra, 0, sizeof( cod_barra ) );
	memset( cadena, 0, sizeof( cadena ) );
	memset( cad, 0, sizeof( cad ) );

    if( !NRO_FECHA_OPERACION )
		SET_MEMORY_INT( __nro_fecha_operacion, _GET_FECHA_DOS(  ) );

     if( USAR_EVENTOS_ALL == 2 ) {
        grabar_all = SI;
    }
    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
        memcpy( cod_barra, _TICKET_COD_BARRA( h ), 17 );
        BUSCA_COD_INTERNO_COD_BARRA( _TICKET_COD_ESPECIAL( h ), cod_barra );
    }
    else {
        BUSCA_CODIGO_ESPECIAL( _TICKET_COD_ESPECIAL( h ) );
    }

		//if( marcar_item ) {	
		//No tiene sentido que actualize el campo excluir_promocion y precio_original si despues
	// lo pone en cero
		//MARCAR_ITEM_ORIGINAL_EN_E_TICKET( h );
	//}

    //SELECT( AREA_E_TICKET );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
   signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
   //para las devoluciones de envase
   if( (_TICKET_TIPO( h ) & _ENVASE) &&
       _TICKET_CANTIDAD( h ) < 0 &&
          ( RAM_MODO_DEVOLUCION != _DEVOL_TOTAL
           && RAM_MODO_DEVOLUCION != _NC_TOTAL ) ) {
           SET_MEMORY_CHAR( __ram_negativo, 1 );
   }

   if(( ( MODO_DEVOLUCION || RAM_NOTA_CR ) && ( _TICKET_TIPO( h ) & _DEV_ENVASE && _TICKET_PRECIO( h ) < 0.00 ) )
         || RAM_NEGATIVO) { //libertad
        signo *= -1;
    }

    if(( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _DEVOL_TOTAL )
       && ( _TICKET_TIPO( h ) & _ENVASE )&&( _TICKET_TIPO( h ) & _ENGANCHADO )&&  _TICKET_CANTIDAD( h ) < 0 ) {//DEVOLUCION DE ENVASE cant - importe - el total debe ser +
      signo *= -1;
    }

	 if(( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _DEVOL_TOTAL )
       && ( _TICKET_TIPO( h ) & _ENVASE )&& !( _TICKET_TIPO( h ) & _RESTA ) &&  _TICKET_CANTIDAD( h ) < 0 ) {//DEVOLUCION DE ENVASE cant - importe - el total debe ser +
      signo *= -1;
    }

    event_e_ticket.id_evento = RAM_ID_EVENTO;
    event_e_ticket.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    event_e_ticket.cod_articulo = _TICKET_COD_ESPECIAL( h );
    //event_e_ticket.cantidad = ( float )( _TICKET_CANTIDAD( h ) * signo );
	 event_e_ticket.cantidad = ( float ) ( _TICKET_CANTIDAD( h ) * ( RAM_NEGATIVO? 1 : signo ) ); //libertad

	//event_e_ticket.cant_presentacion = ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO ) ? signo : _TICKET_PRESENTACION( h );
    event_e_ticket.cant_presentacion =  _TICKET_PRESENTACION( h );
    event_e_ticket.importe_sin_iva = (_TICKET_IMPORTE_SIN_IVA( h ) - _TICKET_IMP_INT( h ))  * signo;
    event_e_ticket.imp_int = _TICKET_IMP_INT( h ) * signo;
    event_e_ticket.costo = _TICKET_COSTO( h ) * signo;
    //  event_e_ticket.recargo_tarjeta =
    //      ( _TICKET_RECARGO( h ) - _TICKET_IVA1_REC( h ) - _TICKET_IVA2_REC( h ) -
    //      _TICKET_ILA_REC( h ) - _TICKET_IHA_REC( h ) ) * signo;
    event_e_ticket.recargo_tarjeta = ( _TICKET_RECARGO( h ) - _TICKET_IVA1_REC( h )
                                     - _TICKET_IVA2_REC( h ) ) * signo;
    event_e_ticket.exento = _TICKET_EXENTO( h ) * signo;
		event_e_ticket.iva1 = ( _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h ) ) * signo;
	event_e_ticket.iva2 = ( _TICKET_IVA2_PRECIO( h ) + _TICKET_IVA2_REC( h ) ) * signo;
	
	_iva = ALICUOTA_MODIFICADO(_TICKET_CANT_EXCL_DESC(h),_TICKET_COD_BARRA( h ), _TICKET_COD_ESPECIAL( h ) );
	if( _iva == -1 ) {
		event_e_ticket.cod_depto_old = 0;
		event_e_ticket.cod_iva = _TICKET_COD_IVA( h );
	} else {
		event_e_ticket.iva1 = 0;
		event_e_ticket.iva2 = 0;
		//Se reutiliza cod_depto_old para indicar el cod_iva de articulo.cod_exento (codigo iva aplicado al articulo)
		event_e_ticket.cod_depto_old = (int)_TICKET_CANT_EXCL_DESC(h);
		//Recupero el iva original de padron
		event_e_ticket.cod_iva = _iva;
	}	

	
    //  event_e_ticket.ila = ( _TICKET_ILA_PRECIO( h ) + _TICKET_ILA_REC( h ) ) * signo;
    //  event_e_ticket.iha = ( _TICKET_IHA_PRECIO( h ) + _TICKET_IHA_REC( h ) ) * signo;
    event_e_ticket.alicuota = _TICKET_DESCUENTO(h) * signo ;//( float ) ( _TICKET_ALICUOTA_PRECIO( h ) * signo );
    event_e_ticket.monto_impuesto[0] = ( float )( _TICKET_IMPUESTO1_PRECIO( h ) * signo );
    event_e_ticket.monto_impuesto[1] = ( float )( _TICKET_IMPUESTO2_PRECIO( h ) * signo );
    event_e_ticket.monto_impuesto[2] = ( float )( _TICKET_IMPUESTO3_PRECIO( h ) * signo );
    event_e_ticket.monto_impuesto[3] = ( float )( _TICKET_IMPUESTO4_PRECIO( h ) * signo );
    event_e_ticket.hab_desc_cliente = _TICKET_HAB_DESC_CLIENTE( h );
    event_e_ticket.cod_presentacion = _TICKET_COD_PRESENT( h );
    event_e_ticket.leido_por_scanner = ( _TICKET_TIPO( h ) & _SCANNER ) ? 1 : 0;
    event_e_ticket.tipo_precio = articulo.tipo_precio;
    event_e_ticket.cod_ext = _TICKET_COD_EXT( h );
    event_e_ticket.nro_vendedor = NRO_VENDEDOR;
    event_e_ticket.cod_clasificacion_old = _TICKET_COD_CLASIFICACION( h );
	_snprintf( event_e_ticket.cod_clasificacion, sizeof( event_e_ticket.cod_clasificacion ), "%s",
					_TICKET_COD_CLASIFICACION_STR(h) );
	event_e_ticket.cod_clasificacion[sizeof( event_e_ticket.cod_clasificacion ) - 1] = 0;
    cant = _TICKET_CANTIDAD( h );
    cant = ( cant > 0 ) ? cant : -cant;

	if( IMPRESORA_FISCAL == FISCAL && ( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1) ) {
		
		double detalle = 0.00, ddff = 0.00;
		detalle = (event_e_ticket.importe_sin_iva +event_e_ticket.exento + event_e_ticket.iva1)  * event_e_ticket.cantidad + (event_e_ticket.imp_int * signo);
		event_e_ticket.total = ROUND( ( _TICKET_PRECIO( h ) + _TICKET_RECARGO( h ) ) * cant * signo,
                                  ENTEROS, DECIMALES + 2  );
		ddff = fabs( (detalle * signo) - event_e_ticket.total) ;
		if( ddff > 0.001 && ddff < 0.05 ){ //grande la diferencia
			event_e_ticket.total = ROUND(  detalle * signo,ENTEROS, DECIMALES + 2  );
			_SET_MEMORY_DOUBLE( __ticket_precio, h, ( event_e_ticket.total - _TICKET_RECARGO( h ) )  / cant * signo );
		}


		
	} else {
		if( RAM_MODO_DEVOLUCION == _NC_TOTAL ||  RAM_MODO_DEVOLUCION == _NC_RF_INTERME ) {
			
			double ticket_precio;
			ticket_precio = _TICKET_PRECIO( h );
			ROUND2(&ticket_precio,ENTEROS, DECIMALES); 
			//ROUND( ( _TICKET_PRECIO( h ), ENTEROS, DECIMALES );//ojo que el round no anda siempre con variables de rnv
			_SET_MEMORY_DOUBLE( __ticket_precio, h, ticket_precio );
		}
		
		event_e_ticket.total = ROUND( ( _TICKET_PRECIO( h ) + _TICKET_RECARGO( h ) ) * cant * signo,
                                  ENTEROS, DECIMALES );
	}
    event_e_ticket.cod_depto = _TICKET_DEPTO( h );
    //event_e_ticket.cod_iva = _TICKET_COD_IVA( h );
    event_e_ticket.cod_movimiento = _TICKET_COD_MOVIMIENTO( h );
	if( /*RAM_DIFERENCIA_PRECIO &&*/ _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO ) {
	   event_e_ticket.origen = _ORIGEN_DIF_PRECIO;
	} else {
        event_e_ticket.origen = origen;
    }
    if( origen == _ORIGEN_PROMOCION ) {
        event_e_ticket.tipo = (unsigned char)_TICKET_TIPO( h );
    }
    else {
        event_e_ticket.tipo = (unsigned char) tipo;
    }
    event_e_ticket.caja = NRO_CAJA;
    event_e_ticket.nro_z = NRO_Z;
    //event_e_ticket.posicion = h; //Ver esto de la version de Luis..
	if( RAM_POS_REV_PROMO ){
		event_e_ticket.posicion = RAM_POS_REV_PROMO;
		SET_MEMORY_INT( __ram_pos_rev_promo, RAM_POS_REV_PROMO + 1 );
	} else {
		event_e_ticket.posicion = h + 1;
	}
    event_e_ticket.descuento = 0.0;
    event_e_ticket.comprobante = ITEM_COMPROBANTE;
    event_e_ticket.motivo = RAM_MOTIVO_DESCUENTO;
    switch( RAM_MODO_DEVOLUCION ) {
        case _NC_LIBRE:
        case _NC_RF_PARCIAL:
        case _DEVOL_LIBRE:
        case _DEVOL_PARCIAL:
            event_e_ticket.motivodev =  _TICKET_MOTIVO_DEVOLUCION( h );
            break;
        default:



            if( ( _TICKET_TIPO( h ) & _RESTA
                        || _TICKET_COD_MOVIMIENTO( h ) == _CAMBIO_DE_MERCADERIA_MENOS )
                    && _TICKET_MOTIVO_DEVOLUCION( h ) ) {
                event_e_ticket.motivodev = _TICKET_MOTIVO_DEVOLUCION( h );
            } else {
                event_e_ticket.motivodev = 0;
            }
    }
    if( cod_barra_art ) {
        strcpy( event_e_ticket.cod_barra, articulo.cod_barra );
    }
    else {
        strcpy( event_e_ticket.cod_barra, ITEM_COD_BARRA );
    }
    event_e_ticket.habilitado_cuotas = ( _TICKET_TIPO2( h ) & _HABILITADO_CUOTAS ) ? 1 : 0;
	event_e_ticket.tipo2 = (unsigned char)_TICKET_TIPO2( h );
    if( articulo.gravado == 'S' ) {
        if( articulo.cod_iva ) {
            tasas = iva[( int )articulo.cod_iva].porc_iva1;
        }
        else {
            tasas = config.porc_iva1;
        }
    }
    else {
        tasas = 0;
    }
    for( i = 0;i < 4;i++ ) {
        if( articulo.cod_impuesto[i] ) {
            tasas += impuestos[( int )articulo.cod_impuesto[i] - 1].tasa;
        }
    }
    for( i = 0;i < 4;i++ ) {
        event_e_ticket.cod_impuesto[i] = articulo.cod_impuesto[i];
    }
    //event_e_ticket.precio_unitario =
    //  ROUND( ( ( event_e_ticket.importe_sin_iva - articulo.imp_int ) * ( tasas / 100 +
    //          1 ) ) + articulo.imp_int, 8, 2 );
    event_e_ticket.precio_unitario = ROUND( fabs( _TICKET_IMPORTE_SIN_IVA( h )
                                                + _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h )
                                                + _TICKET_IMPUESTO1_PRECIO( h )
                                                + _TICKET_IMPUESTO2_PRECIO( h )
                                                + _TICKET_IMPUESTO3_PRECIO( h )
                                                + _TICKET_IMPUESTO4_PRECIO( h )
												+ _TICKET_EXENTO( h ) ), ENTEROS,
                                            DECIMALES );
    event_e_ticket.cod_sucursal = config.sucursal;
//    event_e_ticket.fecha_ticket = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
    event_e_ticket.fecha_ticket = NRO_FECHA_OPERACION;
	event_e_ticket.tipo3 = _TICKET_TIPO3( h );
	event_e_ticket.precio_original = 0.0;
	memset( event_e_ticket.cod_articulo_alfa, 0, sizeof( event_e_ticket.cod_articulo_alfa ) );
	event_e_ticket.excluir_promocion = 0;
    //memset( event_e_ticket.reserva, 0, sizeof( event_e_ticket.reserva ) );
		/***************Para marginales*****************/
	event_e_ticket.cod_cobro =  _TICKET_COD_COBRO( h );
	//SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );

	if( !marcar_item )
	{	
		_snprintf( cad, sizeof(cad),
			"MSJ EV_CONT --> (INSERT) ID_EVENTO %ld POSICION %d, COD_ART %ld COD_BARRA %s",
				event_e_ticket.id_evento, event_e_ticket.posicion,
					event_e_ticket.cod_articulo, event_e_ticket.cod_barra );
	} else {
		_snprintf( cad, 150,
			"MSJ EV_CONT --> (UPDATE) ID_EVENTO %ld POSICION %d, COD_ART %ld COD_BARRA %s",
				event_e_ticket.id_evento, event_e_ticket.posicion,
					event_e_ticket.cod_articulo, event_e_ticket.cod_barra );
	

	}
	cad[sizeof( cad ) - 1] = '\0';
	glog(cad,LOG_DEBUG,3);
	#ifdef INVEL_L
    //BEGIN_TRANSACTION();
    #endif
		if( marcar_item ) {
		if( UPDATE2() ) {
			MENSAJE_STRING( S_NO_SE_PUDO_ACTUALIZAR );
			GRABAR_LOG_SISTEMA( ST( S_NO_SE_PUDO_ACTUALIZAR ) , LOG_VENTAS,2 );
        }
	} else {
		error = INSERT2(NO);
	}

	if( error > 0 ) {
        GRABAR_LOG_SISTEMA( BUSCA_MENSAJE_STRING( S_ERROR_GRABANDO_ITEM ) ,LOG_ERRORES,1);
        MENSAJE_STRING( S_ERROR_GRABANDO_ITEM );
		if( error == 1 ) { //clave duplicada puede ser el evento o la posicion
			if( RAM_P_TICKET == 1 ) { //es el primer elemento por lo tanto es el id_evento
				ADD_MEMORY_LONG( __ram_id_evento, 1 );
				event_e_ticket.id_evento = RAM_ID_EVENTO;
			} else { //es la posicion
				event_e_ticket.posicion = event_e_ticket.posicion + 1;
			}
		} else
			CORREGIR_TICKET(1);
		glog(ST(S_REINTENTAR_S_N), LOG_ERRORES,1);
		error = INSERT2(NO); //reintento	
		if( error ) {
			_snprintf( cadena, sizeof(cadena)-1,"NO SE LOGRO INSERTAR EL EVENTO EN EV_CONT con Error:%i", error );
			glog(cadena,LOG_ERRORES,1);
			GRABAR_LOG_BD(cadena, SI ,LOG_ERRORES,1);
		} else {
			glog("SE LOGRO INSERTAR EL EVENTO EN EV_CONT",LOG_ERRORES,1);
			GRABAR_LOG_BD("SE LOGRO INSERTAR EL EVENTO EN EV_CONT", SI ,LOG_ERRORES,1);
		}
    }
	if(_iva != -1)//En caso que se utilice en otra funcion revertimos al iva aplicado
		event_e_ticket.cod_iva = _TICKET_COD_IVA( h );
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
    #ifdef COMPILAR_EVENTOS_ALL
    if( ( grabar_all || DEBE_GRABAR_ALL() ) && USAR_EVENTOS_ALL && !GET_ERROR_EN_RED() ) {
        memcpy( &event_e_ticket_all, &event_e_ticket, sizeof( event_e_ticket_all ) );
        event_e_ticket_all.id_evento = RAM_ID_EVENTO_ALL;
        SELECT( AREA_E_TICKET_ALL );
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        INSERT();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    #endif
}

/***********************************************************************/
void GRABAR_EVENT_PAGO( int grabar_all )
/***********************************************************************/
{
    int tmpPos = 0;
    double tmpImporte = 0;

    if( !NRO_FECHA_OPERACION )
		SET_MEMORY_INT( __nro_fecha_operacion, _GET_FECHA_DOS(  ) );
    if( USAR_EVENTOS_ALL == 2 ) {
        grabar_all = SI;
    }
    //SELECT( AREA_PAGO );
	SELECT_TABLE( T_PAGOS, tipo_t_pagos );
    event_pago.id_evento = RAM_ID_EVENTO;
    event_pago.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    event_pago.caja = NRO_CAJA;
    event_pago.nro_z = NRO_Z;
    event_pago.cod_sucursal = config.sucursal;
  //  event_pago.fecha_ticket = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
    event_pago.fecha_ticket = NRO_FECHA_OPERACION;

    //memset( event_pago.reserva, 0, sizeof( event_pago.reserva ) );
    #ifdef INVEL_L
    //BEGIN_TRANSACTION();
    #endif
    INSERT2(NO);
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
    #ifdef COMPILAR_EVENTOS_ALL
    if( grabar_all && USAR_EVENTOS_ALL && !GET_ERROR_EN_RED() ) {
        memcpy( &event_pago_all, &event_pago, sizeof( event_pago_all ) );
        event_pago_all.id_evento = RAM_ID_EVENTO_ALL;
        SELECT( AREA_PAGO_ALL );
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        INSERT();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    #endif
}
/***********************************************************************/
void GRABAR_EVENT_VARIOS( char *buffer, int l_buffer, int grabar_all )
/***********************************************************************/
{
    if( USAR_EVENTOS_ALL == 2 ) {
        grabar_all = SI;
    }
    //if( !USED( AREA_VARIOS ) ) {
    if( !OPENED( T_VARIOS, tipo_t_varios ) ) {
        return;
    }
    //SELECT( AREA_VARIOS );
    SELECT_TABLE( T_VARIOS, tipo_t_varios );
    event_varios->cabecera.id_evento = RAM_ID_EVENTO;
    memset( event_varios->cabecera.reserva, 0, sizeof( event_varios->cabecera.reserva ) );
    if( l_buffer > L_REG_VARIOS ) {
        l_buffer = L_REG_VARIOS;
    }
    memcpy( event_varios->buffer, buffer, l_buffer );
    INSERT_VARIABLE( sizeof( struct _cab_varios ) + l_buffer );
    #ifdef COMPILAR_EVENTOS_ALL
    if( grabar_all && USAR_EVENTOS_ALL && !GET_ERROR_EN_RED() ) {
        SELECT( AREA_VARIOS_ALL );
        memcpy( event_varios_all, event_varios, sizeof( struct _cab_varios ) + l_buffer );
        event_varios_all->cabecera.id_evento = RAM_ID_EVENTO_ALL;
        INSERT_VARIABLE( sizeof( struct _cab_varios ) + l_buffer );
    }
    #endif
}
/***********************************************************************/
void BACKUP_EVENTOS()
/***********************************************************************/
{
    /*int error_copy = 0, error[12];
	char *szText[] = {"","TICKET","E_TICKET","E_DES_PRO","EV_FACTU","PAGO","VARIOS",
						  "EV_CIERRE","EV_T_COMPRA","EV_DPAGO","_EV_SUPER","EV_B_PROMO"};

	memset( error, 0, sizeof( error ) );*/
    GRABAR_LOG_SISTEMA( ST( S_BACKUP_EVENTOS ) ,LOG_VENTAS,2);
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EVENTOS, _EVENTOS_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EV_CONT, _EV_CONT_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_PAGOS, _PAGO_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_VARIOS, _VARIOS_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EV_CIERRE, _EV_CIERRE_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EV_DESPRO, _E_DES_PRO_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EV_FACTU, _EV_FACTU_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EV_COD_BARRA, _EV_COD_BARRA_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EV_DMEDIOS, _EV_DMEDIOS_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EV_SUPER, _EV_SUPER_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_EV_B_PROMO, _EV_B_PROMO_BAK );
	VACIAR_ARCHIVOS_EVENTOS( TT_ORIG );
}


/***********************************************************************/
int COPIAR_EVENTOS_ANTERIORES()
/***********************************************************************/
{
    int ok = 0, rta;
    unsigned fecha;
    fecha = LEE_FECHA( _FECHA_EVE );
    if( fecha ) {
        ok = 3;
        //    ok += EXPORTA_ARCHIVO_DE_LA_SEMANA( fecha, "TICKET", "N:TICKET.BTR" );
        //    ok += EXPORTA_ARCHIVO_DE_LA_SEMANA( fecha, "E_TICK", "N:E_TICKET.BTR" );
        //    ok += EXPORTA_ARCHIVO_DE_LA_SEMANA( fecha, "PAGO", "N:PAGO.BTR" );
        //    ok += EXPORTA_ARCHIVO_DE_LA_SEMANA( fecha, "VARIOS", "N:VARIOS.BTR" );
    }
    rta = ( ok == 3 ) ? 1 : 0;
    return ( rta );
}

/***********************************************************************/
int COPIA_EVENTOS_A_PENDIENTES_ANTERIORES( void )
/***********************************************************************/
{
	
	int       error = 0;
	char      debug[80];
	long      pos = 0;

	SELECT_TABLE( T_EVENTOS, TT_ORIG );
	SET_WHERE("STATUS = 0");
	//SET_WHERE("STATUS = 1");
	RUN_QUERY(NO);
	while( !dbEOF() /*&& !error*/ ) {
		if( TIPO_EVENTO_A_PROCESAR( event_ticket.tipo_evento ) ) {
			sprintf( debug,	"Evento pasa al his. Z: %li id_evento %li ( Status %d )",
				event_ticket.nro_z, event_ticket.id_evento, event_ticket.status );
			GRABAR_LOG_BD( debug, NO ,LOG_DEBUG,4);
			pos = GET_POSITION_REG();
			error = !GRABAR_EVENTOS_EN_PENDIENTES_Z_ANTERIOR( event_ticket.nro_z, event_ticket.id_evento );
			if( error ) {
				sprintf( debug,	" Error al pasar Evento a his. Z: %li id_evento %li ( Status %d )",
					event_ticket.nro_z, event_ticket.id_evento, event_ticket.status );
				GRABAR_LOG_BD( debug, SI ,LOG_DEBUG,2);
			}
		}
		SELECT_TABLE( T_EVENTOS, TT_ORIG );
		SET_WHERE("STATUS = 0");
		//SET_WHERE("STATUS = 1");
		RUN_QUERY(NO);
		GET_DIRECT_REG(pos, 0);
		SKIP2( 1 );
	}

	return 1;
}

/*****************************************************************************/
int GRABAR_EVENTOS_EN_PENDIENTES_Z_ANTERIOR( long nro_z, long id_evento )
/*****************************************************************************/
{
    int  error = 0;
	struct _eventos *ticket, *ticket_hist;
	//struct _eventos ticket_pend;
	struct _ev_cont *e_ticket, *e_ticket_hist;
	struct _ev_medios *pago, *pago_hist;
	struct _ev_cod_barra *cod_barra, *cod_barra_hist;
	struct _ev_cierre *cierre, *cierre_hist;
	struct _ev_super *super, *super_hist;
	struct _ev_dmedios *dpago, *dpago_hist;
	struct _ev_despro *e_despro, *e_despro_hist;
    struct _ev_bpromo *bpromo, *bpromo_hist;
	struct _ev_factu *factura, *factura_his;
	char      p[45];

	memset( p, 0, sizeof(p) );
	sprintf( p, "DBR_GRABAR_PENDIENTES %li %li", nro_z, id_evento );
	GRABAR_LOG_BD( p,NO ,LOG_DEBUG,4);

	if( RAM_MODO_EJECUCION == ENTRENAMIENTO )
		return 1;

	OBTIENE_PUNTEROS_A_EVENTOS( &ticket, &e_ticket, &pago, &cod_barra, &cierre,
		&super, &dpago, &e_despro, &bpromo, &factura, nro_z );

	OBTIENE_PUNTEROS_A_EVENTOS( &ticket_hist, &e_ticket_hist, &pago_hist, &cod_barra_hist, &cierre_hist, 
		&super_hist, &dpago_hist,&e_despro_hist, &bpromo_hist,&factura_his, -1 );
	
	memset( p, 0, sizeof(p) );

	//OPEN_TABLE(T_EVENTOS, TT_ORIG,( char* )&ticket_pend,sizeof( struct _eventos ));

	if( POSICIONA_EN_TICKET( nro_z, id_evento ) ) {
		//SELECT( AREA_TICKET_HIST );
		SELECT_TABLE( T_EVENTOS, TT_HIST );
		memset( ticket_hist, 0, sizeof( struct _eventos ) );
		memcpy( ticket_hist, ticket, sizeof( struct _eventos ) );
		//error = INSERT_NO_MODIF_STATUS(  );
		/*error = INSERT2( NO );
		if( error ) {
			sprintf( p, "Ticket Insert %d", error );
			//GRABAR_LOG_DBROUTER( p );
			GRABAR_LOG_BD( p,SI ,LOG_DEBUG,2);
		}*/
		//if( !error ) {
			if( ticket->tipo_evento == 9 || ticket->tipo_evento == 10 ) {
				if( POSICIONA_EN_CIERRE( ticket->nro_z, ticket->id_evento) ) {
					//SELECT( AREA_CIERRE_HIST );
					SELECT_TABLE( T_EV_CIERRE, TT_HIST );
					memset( cierre_hist, 0, sizeof( struct _ev_cierre ) );
					memcpy( cierre_hist, cierre, sizeof( struct _ev_cierre ) );
					error = INSERT2( NO );
					if( error ) {
						sprintf( p, "Cierre Insert %d", error );
						//GRABAR_LOG_DBROUTER( p );
						GRABAR_LOG_BD( p,SI ,LOG_DEBUG,2);
					}
					//GET_NEXT(  );
					SKIP2( 1 );
				}
				if( !error ) {
					// ------------------------------------------------
					// PROCESA PAGOS.
					// ------------------------------------------------
					if( POSICIONA_EN_PAGO( ticket->nro_z, ticket->id_evento) ) {
						while( pago->nro_z == ticket->nro_z && pago->id_evento ==
								ticket->id_evento && !dbEOF() && !error ) {
							//SELECT( AREA_PAGO_HIST );
							SELECT_TABLE( T_PAGOS, TT_HIST );
							memset( pago_hist, 0, sizeof( struct _ev_medios ) );
							memcpy( pago_hist, pago, sizeof( struct _ev_medios ) );
							//error = INSERT_NO_MODIF_STATUS(  );
							error = INSERT2( NO );
							if( error ) {
								sprintf( p, "Ev_medios Insert %d", error );
								//GRABAR_LOG_DBROUTER( p );
								GRABAR_LOG_BD( p,SI,LOG_DEBUG,2 );
							}
							SELECT_TABLE( T_PAGOS, TT_ORIG );
							SKIP2( 1 );
						}
					}
				}
				if( !error ) {
					// ------------------------------------------------
					// PROCESA SUPER.
					// ------------------------------------------------
					if( POSICIONA_EN_SUPER( ticket->nro_z, ticket->id_evento) ) {
						while( super->nro_z == ticket->nro_z && super->id_evento ==
								ticket->id_evento && !dbEOF() && !error ) {
							//SELECT( AREA_SUPER_HIST );
							SELECT_TABLE( T_EV_SUPER, TT_HIST );
							memset( super_hist, 0, sizeof( struct _ev_super ) );
							memcpy( super_hist, super, sizeof( struct _ev_super ) );
							//error = INSERT_NO_MODIF_STATUS(  );
							error = INSERT2( NO );
							if( error ) {
								sprintf( p, "Ev_super Insert %d", error );
								//GRABAR_LOG_DBROUTER( p );
								GRABAR_LOG_BD( p,SI,LOG_DEBUG,2 );
							}
							SELECT_TABLE( T_EV_SUPER, TT_ORIG );
							SKIP2( 1 );
						}
					}
				}
			} else {
				if( POSICIONA_EN_E_TICKET( ticket->nro_z, ticket->id_evento) ) {
					while( e_ticket->nro_z == ticket->nro_z &&
						e_ticket->id_evento == ticket->id_evento && !dbEOF() &&	!error ) {
						//SELECT( AREA_E_TICKET_HIST );
						SELECT_TABLE( T_EV_CONT, TT_HIST );
						memset( e_ticket_hist, 0, sizeof( struct _ev_cont) );
						memcpy( e_ticket_hist, e_ticket, sizeof( struct _ev_cont) );
						//error = INSERT_NO_MODIF_STATUS(  );
						error = INSERT2( NO );
						if( error ) {
							sprintf( p, "Ev_cont Insert %d", error );
							//GRABAR_LOG_DBROUTER( p );
							GRABAR_LOG_BD( p,SI,LOG_DEBUG,2 );
						}
						SELECT_TABLE( T_EV_CONT, TT_ORIG );
						SKIP2( 1 );
					}
				}
				if( !error ) {
					// ------------------------------------------------
					// PROCESA PAGOS.
					// ------------------------------------------------
					if( POSICIONA_EN_PAGO( ticket->nro_z, ticket->id_evento) ) {
						while( pago->nro_z == ticket->nro_z && pago->id_evento ==
								ticket->id_evento && !dbEOF() && !error ) {
							//SELECT( AREA_PAGO_HIST );
							SELECT_TABLE( T_PAGOS, TT_HIST );
							memset( pago_hist, 0, sizeof( struct _ev_medios ) );
							memcpy( pago_hist, pago, sizeof( struct _ev_medios ) );
							//error = INSERT_NO_MODIF_STATUS(  );
							error = INSERT2( NO );
							if( error ) {
								sprintf( p, "Ev_medios Insert %d", error );
								//GRABAR_LOG_DBROUTER( p );
								GRABAR_LOG_BD( p,SI ,LOG_DEBUG,2);
							}
							SELECT_TABLE( T_PAGOS, TT_ORIG );
							SKIP2( 1 );
						}
					}
				}
				if( !error ) {
					// ------------------------------------------------
					// PROCESA SUPER.
					// ------------------------------------------------
					if( POSICIONA_EN_SUPER( ticket->nro_z, ticket->id_evento ) ) {
						while( super->nro_z == ticket->nro_z && super->id_evento ==
								ticket->id_evento && !dbEOF() && !error ) {
							//SELECT( AREA_SUPER_HIST );
							SELECT_TABLE( T_EV_SUPER, TT_HIST );
							memset( super_hist, 0, sizeof( struct _ev_super ) );
							memcpy( super_hist, super, sizeof( struct _ev_super ) );
							//error = INSERT_NO_MODIF_STATUS(  );
							error = INSERT2( NO );
							if( error ) {
								sprintf( p, "Ev_super Insert %d", error );
								//GRABAR_LOG_DBROUTER( p );
								GRABAR_LOG_BD( p,SI,LOG_DEBUG,2 );
							}
							SELECT_TABLE( T_EV_SUPER, TT_ORIG );
							SKIP2( 1 );
						}
					}
				}
            if( !error ) {
					// ------------------------------------------------
					// PROCESA BPROMO.
					// ------------------------------------------------
					if( POSICIONA_EN_EV_B_PROMO( ticket->nro_z, ticket->id_evento ) ) {
						while( bpromo->nro_z == ticket->nro_z && bpromo->id_evento ==
								ticket->id_evento && !dbEOF() && !error ) {
							//SELECT( AREA_BPROMO_HIST );
							SELECT_TABLE( T_EV_B_PROMO, TT_HIST );
							memset( bpromo_hist, 0, sizeof( struct _ev_bpromo ) );
							memcpy( bpromo_hist, bpromo, sizeof( struct _ev_bpromo ) );
							//error = INSERT_NO_MODIF_STATUS(  );
							error = INSERT2( NO );
							if( error ) {
								sprintf( p, "BPROMO INS %d", error );
								GRABAR_LOG_BD( p,SI ,LOG_DEBUG,2);
								//GRABAR_LOG_DBROUTER( p );
							}
							SELECT_TABLE( T_EV_B_PROMO, TT_ORIG );
							SKIP2( 1 );
						}
					}
				}
				if( !error ) {
				// ------------------------------------------------
				// PROCESA E_DESPRO.
				// ------------------------------------------------
					if( POSICIONA_EN_E_DESPRO( ticket->nro_z, ticket->id_evento) ) {
						while( e_despro->nro_z == ticket->nro_z &&
								e_despro->id_evento == ticket->id_evento && !dbEOF() && !error ) {
							//SELECT( AREA_E_DESPRO_HIST );
							SELECT_TABLE( T_EV_DESPRO, TT_HIST );
							memset( e_despro_hist, 0, sizeof( struct _ev_despro ) );
							memcpy( e_despro_hist, e_despro, sizeof( struct _ev_despro ) );
							//error = INSERT_NO_MODIF_STATUS(  );
							error = INSERT2( NO );
							if( error ) {
								sprintf( p, "DESC INS %d", error );
								GRABAR_LOG_BD( p,SI ,LOG_DEBUG,2);
								//GRABAR_LOG_DBROUTER( p );
							}
							SELECT_TABLE( T_EV_DESPRO, TT_ORIG );
							SKIP2( 1 );
						}
					}
				}
				if( !error ) {
					// ------------------------------------------------
					// PROCESA DPAGO.
					// ------------------------------------------------
					if( POSICIONA_EN_DPAGO( ticket->nro_z, ticket->id_evento ) ) {
						while( dpago->nro_z == ticket->nro_z && dpago->id_evento ==
							ticket->id_evento && !dbEOF() && !error ) {
							//SELECT( AREA_DPAGO_HIST );
							SELECT_TABLE( T_EV_DMEDIOS, TT_HIST  );
							memset( dpago_hist, 0, sizeof( struct _ev_dmedios ) );
							memcpy( dpago_hist, dpago, sizeof( struct _ev_dmedios ) );
							//error = INSERT_NO_MODIF_STATUS(  );
							error = INSERT2( NO );
							if( error ) {
								sprintf( p, "DPAG INS %d", error );
								GRABAR_LOG_BD( p,SI ,LOG_DEBUG,2);
								//GRABAR_LOG_DBROUTER( p );
							}
							SELECT_TABLE( T_EV_DMEDIOS, TT_ORIG );
							SKIP2( 1 );
						}
					}
				}
				if( !error ) {
					if( POSICIONA_EN_COD_BARRA( ticket->nro_z, ticket->id_evento) ) {
						while( cod_barra->nro_z == ticket->nro_z &&
							cod_barra->id_evento == ticket->id_evento && !dbEOF() && !error ) {
							//SELECT( AREA_COD_BARRA_HIST );
							SELECT_TABLE( T_EV_COD_BARRA, TT_HIST );
							memset( cod_barra_hist, 0, sizeof( struct _ev_cod_barra ) );
							memcpy( cod_barra_hist, cod_barra, sizeof( struct _ev_cod_barra ) );
							error = INSERT2( NO );
							//error = INSERT_NO_MODIF_STATUS(  );
							if( error ) {
								sprintf( p, "Ev_cod_barra Insert %d", error );
								GRABAR_LOG_BD( p,SI ,LOG_DEBUG,2);
								//GRABAR_LOG_DBROUTER( p );
							}
							SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
							SKIP2( 1 );
						}
					}
				}
			}
			SELECT_TABLE( T_EVENTOS, TT_HIST );
			memset( ticket_hist, 0, sizeof( struct _eventos ) );
			memcpy( ticket_hist, ticket, sizeof( struct _eventos ) );
			//error = INSERT_NO_MODIF_STATUS(  );
			error = INSERT2( NO );
			if( error ) {
				sprintf( p, "Ticket Insert %d", error );
				GRABAR_LOG_BD( p,SI ,LOG_DEBUG,2);
				SELECT_TABLE( T_EV_CIERRE, TT_HIST );
				DIRECT_DB_QUERY("DELETE FROM EV_CIERRE WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld';",
					ticket->caja_z, ticket->id_evento );
				SELECT_TABLE( T_PAGOS, TT_HIST );
				DIRECT_DB_QUERY("DELETE FROM EV_MEDIOS WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld';",
					ticket->caja_z, ticket->id_evento );
				SELECT_TABLE( T_EV_SUPER, TT_HIST );
				DIRECT_DB_QUERY("DELETE FROM EV_SUPER WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld';",
					ticket->caja_z, ticket->id_evento );
				SELECT_TABLE( T_EV_CONT, TT_HIST );
				DIRECT_DB_QUERY("DELETE FROM EV_CONT WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld';",
					ticket->caja_z, ticket->id_evento );
				SELECT_TABLE( T_EV_B_PROMO, TT_HIST );
				DIRECT_DB_QUERY("DELETE FROM EV_BPROMO WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld';",
					ticket->caja_z, ticket->id_evento );
				SELECT_TABLE( T_EV_DESPRO, TT_HIST );
				DIRECT_DB_QUERY("DELETE FROM EV_DESPRO WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld';",
					ticket->caja_z, ticket->id_evento );
				SELECT_TABLE( T_EV_DMEDIOS, TT_HIST  );
				DIRECT_DB_QUERY("DELETE FROM EV_DMEDIOS WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld';",
					ticket->caja_z, ticket->id_evento );
				SELECT_TABLE( T_EV_COD_BARRA, TT_HIST );
				DIRECT_DB_QUERY("DELETE FROM EV_COD_BARRA WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld';",
					ticket->caja_z, ticket->id_evento );
			}
		//}

	}

	return !error;
}

/***********************************************************************/
int COPIAR_EVENTOS_ACTUAL()
/***********************************************************************/
{
    int rta = 0;
    //char strArch[30];
    GRABAR_LOG_SISTEMA( "Va a hacer backups de los eventos" ,LOG_VENTAS,4);
    CERRAR_ARCHIVOS_EVENTOS();
	BACKUP_TABLE(T_EV_CONT);
	BACKUP_TABLE(T_PAGOS);
	BACKUP_TABLE(T_EV_SUPER);
	BACKUP_TABLE(T_EV_DESPRO);
	BACKUP_TABLE(T_EV_FACTU);
	BACKUP_TABLE(T_EV_COD_BARRA);
	BACKUP_TABLE(T_VARIOS);
	BACKUP_TABLE(T_EV_CIERRE);
	BACKUP_TABLE(T_EV_DMEDIOS);
	BACKUP_TABLE(T_EV_B_PROMO);
	BACKUP_TABLE(T_EVENTOS);
    /*sprintf( strArch, "%sTICKET.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "TICKET.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sE_TICKET.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "E_TICKET.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sPAGO.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "PAGO.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sVARIOS.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "VARIOS.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sSUPER.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "SUPER.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sDPAGO.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "DPAGO.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sEV_TCOMP.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "EV_TCOMP.SIC", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sEV_CIERR.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "EV_CIERR.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sE_DESPRO.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "E_DESPRO.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sEV_FACTU.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "EV_FACTU.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    sprintf( strArch, "%sCIE_LOTE.BAK", PATH_BACKUPS );
    if( !COPIAR_ARCHIVO( "CIE_LOTE.BTR", strArch, __LINE__, __FILE__ ) ) {
        rta = 1;
    }
    GRABAR_LOG_SISTEMA( "FIN backups de los eventos" );*/
    return ( rta );
}
/*****************************************************************************/
void REPROCESAR_EVENTOS( int cant )
/*****************************************************************************/
{
    int h;
    long id_evento;
    id_evento = RAM_ID_EVENTO;
    //SELECT( AREA_PAGO );
    //GET_EQUAL( ( char* )&id_evento );
	SELECT_TABLE( T_PAGOS, tipo_t_pagos );
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    for( h = 0;!dbEOF() && event_pago.id_evento == id_evento && h < cant;h++ ) {
        event_pago.reproceso = 1;
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        UPDATE2();
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
        //GET_NEXT();
		SKIP2(1);
    }
}
/*****************************************************************************/
void GRABA_EVENTO_ANULADO()
/*****************************************************************************/
{
		
	memset( &event_ticket, 0, sizeof( event_ticket ) );

	if( IMPRESORA_FISCAL == FISCAL ) {
		if(  RAM_P_TICKET 
				&& DOCUMENTO_ABIERTO( ) != 1 ) {
			//No se imprime ningun comprobante porque da la inconsistencia en el primer articulo que 
			//que ingresa de manera que no abre el comprobante, dando inconsistencia fiscal. 
			//Al no imprimir el comprobante, el contador de la impresora fiscal no se incrementa.
			SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_INCONSISTENCIA_DOC_CERRADO );
			//Hay que verificar cuando el comprobante imprimio ej 4248 o no 3606 para que setee 
			//el nro de ticket correcto
			//Tarea 4248. Sin haber seleccionado el medio, apagar la impresora y terminar el ticket. 
			//Cuando visualize los mensajes de error de impresora, encender la printer.
			//Resultado: grababa el evento como tipo 99 y nro_ticket = 0. Cuando se habia impreso
		}
	}

	if( IMPRESORA_FISCAL == FISCAL 
			&& !( RAM_ANULAR_TICKET == _ANUL_INCONSISTENCIA_DOC_CERRADO ) ) {
		event_ticket.nro_ticket = /*NRO_TICKET*/ RAM_NRO_COMP + 1;
	} else {
		event_ticket.nro_ticket = 0;
	}
    event_ticket.tipo_evento = 99;
	event_ticket.nro_comprobante_new = 0;
    GRABAR_EVENT_TICKET( SI );
    ACTUALIZA_ID_EVENTO_NEW();
}
/*****************************************************************************/
void RETIRAR_TODOS_LOS_COMPROBANTES()
/*****************************************************************************/
{
    //SELECT( AREA_PAGO );
    //GO( TOP );
	SELECT_TABLE( T_PAGOS, tipo_t_pagos );
	SET_WHERE("");
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    while( !dbEOF() ) {
        if( !event_pago.retirado ) {
            event_pago.retirado = 1;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            UPDATE2();
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
        }
        SKIP2( 1 );
    }
}
/***********************************************************************/
int DEBE_GRABAR_ALL()
/***********************************************************************/
{
    int rta = 0;
    if( ( EMITIR_REMITO() || EMITIR_FACTURA_REMOTA() ) && !GET_ERROR_EN_RED() ) {
        rta = 1;
    }
    return ( rta );
}
/****************************************************************************/
int BUSCAR_EVENT_TICKET( long id )
/****************************************************************************/
{
    int /*area_ant,*/rta, tabla_ant, tipo_ant;
    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    //GET_EQUAL( ( char* )&id );
	SET_WHERE("ID_EVENTO = '%ld'",id);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    rta = FOUND2();
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
    return ( rta );
}
/****************************************************************************/
int EVENTO_CON_PAGOS()
/****************************************************************************/
{
    int rta = 0, h;
    char tipos[] = {0,1,3,4,5,6,14,15,16,17,19,20,21,70};
    for( h = 0;h < sizeof( tipos );h++ ) {
        if( event_ticket.tipo_evento == tipos[h] ) {
            rta = 1;
        }
    }
    return ( rta );
}
/****************************************************************************/
void BORRAR_EVENT_TICKET( int borrar_all )
/****************************************************************************/
{
    long id_evento;
    if( USAR_EVENTOS_ALL == 2 ) {
        borrar_all = SI;
    }
    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    id_evento = RAM_ID_EVENTO;
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    //GET_EQUAL( ( char* )&id_evento );
    if( FOUND2() ) {
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        DELETE2();
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
    }
#ifdef COMPILAR_EVENTOS_ALL
 	if( borrar_all && USAR_EVENTOS_ALL && !GET_ERROR_EN_RED() ) {
        id_evento = RAM_ID_EVENTO_ALL;
        SELECT( AREA_TICKET_ALL );
        GET_EQUAL( ( char* )&id_evento );
        if( FOUND() ) {
            #ifdef INVEL_L
            BEGIN_TRANSACTION();
            #endif
            _DELETE();
            #ifdef INVEL_L
            END_TRANSACTION();
            #endif
        }
    }
#endif
}
/****************************************************************************/
void GRABAR_EVENT_SUPERVISOR( int cod_sup, long legajo, int ok, int funcion, int grabar_en_event )
/****************************************************************************/
{
        int /*area*/ tabla_ant, tipo_ant, posicion = 0;
	
	long evento_superv =0; 

	if( RAM_CIERRE_Y && !CAJEROS_FLOTANTES ) {
		if( funcion == 127 ) {//CASHBACK en el mismo Cierre Y
			evento_superv  = RAM_ID_EVENTO;
		} else {
			evento_superv  = RAM_ID_EVENTO + 1 + CantCambioMediosAProcesar;
		}
	} else {
		evento_superv  = RAM_ID_EVENTO;
	}
	
	//hace falta saber cual es el evento a buscar para conocer la proxima posicion 
    posicion = OBTENER_POS_SUPER(evento_superv ) + 1;
	//limpiamos  la estructura porque en la busqueda se obtuvieron los datos del evento consultado si existe
	memset( event_super, 0, sizeof( event_super) );

		// ahora comienza la asignacion de datos
	event_super->id_evento = evento_superv ;
	event_super->posicion = posicion;

    event_super->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    event_super->caja = NRO_CAJA;
    event_super->nro_z = NRO_Z;
    event_super->hora = GET_HORA_DOS();
    event_super->pos_contenido = RAM_P_TICKET;
    event_super->pos_pago = RAM_P_PAGO + 1;
    event_super->cod_funcion = funcion;
    event_super->cod_supervisor = cod_sup;
    event_super->legajo_supervisor = legajo;
    event_super->clave_ok = ok;
    event_super->cod_sucursal = config.sucursal;
    //event_super->fecha_ticket = GET_FECHA_DOS();
	if( !NRO_FECHA_OPERACION )
		SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );
	
	//event_super->fecha_ticket = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : NRO_FECHA_OPERACION;
	event_super->fecha_ticket = NRO_FECHA_OPERACION;

	//Se comenta hasta verificar esto
    //memset( event_super->reserva, 0, sizeof( event_super->reserva ) );
	{	char cad[80];
		_snprintf( cad, 80,
			"MSJ SUPER.BTR --> ID_EVENTO %ld POSICION %ld",
				event_super->id_evento, event_super->posicion );
		cad[79] = '\0';
		glog(cad,LOG_DEBUG,2);
	}
    if( grabar_en_event ) {
        tabla_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();
		//area = SELECTED();
        //SELECT( AREA_SUPER );
        SELECT_TABLE( T_EV_SUPER, tipo_t_ev_super );
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
		if( INSERT2(NO) ) {
			glog("NO INSERTO EVENTO EN SUPER.BTR ",LOG_DEBUG,2);
		}
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
        //SELECT( area );
		SELECT_TABLE( tabla_ant, tipo_ant );
    }
    else {
        /*si no guarda el evento, entonces lo graba en un archivo rec para luego
          pasarlo a los eventos con la funcion GRABAR_SUPER_REC_A_EVENT()*/
        GUARDAR_ARCHIVO_REC( ARCH_REC_SUPERVISOR, ( char* )event_super,
                             sizeof( struct _ev_super ) );
    }
}
/****************************************************************************/
void GRABAR_SUPER_REC_A_EVENT( long idEvento )
/****************************************************************************/
{
    int /*area,*/ tabla_ant, tipo_ant, j = 0;
    long pos;
    //area = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    pos = OBTENER_POS_SUPER(idEvento) + 1;
    //SELECT( AREA_SUPER );
    SELECT_TABLE( T_EV_SUPER, tipo_t_ev_super );
    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_SUPERVISOR, ( char* )event_super,
                                  sizeof( struct _ev_super ), j ) == 0 ) {
        event_super->id_evento = idEvento; //cambia el id evento
        event_super->posicion = pos + j;
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        INSERT2( NO );
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
        j++;
    }
    /*Una vez guardado en los eventos, borra el archivo rec*/
    BORRAR_ARCHIVO_REC( ARCH_REC_SUPERVISOR );
    //SELECT( area );
	SELECT_TABLE( tabla_ant, tipo_ant );
}
/****************************************************************************/
int INCREMENTAR_NRO_EVENTO( int tipo_evento )
/****************************************************************************/
{
    int rta = 0;
    
	if( INCREMENTAR_NRO_EVENTO_CONFIGURA ) {
        if( tipo_evento < 100 ) {
            rta = ARRAY_NRO_EVENTO[tipo_evento];
        }
        else {
            GRABAR_LOG_SISTEMA_STRING( S_DESBORDE_EN_ARRAY_NRO_EVENTO, LOG_ERRORES,1 );
        }
    }
    else {
        rta = 1;
    }

    return ( rta );
}
/*****************************************************************************/
int CARGAR_ARRAY_NRO_EVENTOS()
/*****************************************************************************/
{
    int rta = 0;
    struct _tipo_eventos tipo_eventos;
    if( INCREMENTAR_NRO_EVENTO_CONFIGURA ) {
        //ARRAY_NRO_EVENTO = malloc( 100 );
        ARRAY_NRO_EVENTO = ( char* )MALLOC( sizeof( char ) + 100 );
        if( ARRAY_NRO_EVENTO != NULL ) {
            memset( ARRAY_NRO_EVENTO, 0, 100 );
            //if( USE_DB( AREA_AUX, _T_EVENTOS_SIC, ( char* )&tipo_eventos, sizeof( tipo_eventos ), NULL, 0, 0 ) == 0 ) {
			if( OPEN_TABLE(T_INC_EVENTO, TT_ORIG,( char* )&tipo_eventos,
							sizeof( tipo_eventos))== 0 ) {
				SET_WHERE("");
				RUN_QUERY(NO);
				while(/*!BtrvEOF()*/ !dbEOF() ) {
                    if( tipo_eventos.cod_evento < 100 
							&& ( tipo_eventos.inc_nro_evento == 'S' 
								|| tipo_eventos.inc_nro_evento == 's' ) ) {
                        ARRAY_NRO_EVENTO[( int )tipo_eventos.cod_evento] = 1;
                    }
                    //SKIP( 1 );
					SKIP2( 1 );
                }
                //CLOSE_DATABASE( AREA_AUX );
				CLOSE_TABLE( T_INC_EVENTO, TT_ORIG );
                rta = 1;
            }
        }
    }
    else {
        rta = 1;
    }
    return ( rta );
}
/**************************************************************************/
void GRABAR_EVENTO_DESC( char tipo_motivo, int id_motivo )
/**************************************************************************/
{
    char nombre_archivo[20],promo_acumulada[1000],s_monto[8];
    int handle_activas = 0, handle_descuentos = 0, i, j;
    int /*area_ant,*/ rta, error = 0, signo = 1, borrar = 0, tabla_ant, tipo_ant;
    double monto;
    char aux[30];
    //   Motivos de los descuentos
    //   -------------------------
    //   MOTIVO_DESC_MANUAL_PESOS  1
    //   MOTIVO_DESC_MANUAL_PORC   2
    //   MOTIVO_DESC_PROMOCION     3
    //   MOTIVO_DESC_PERFIL        4
    //   MOTIVO_REC_DESDE_PERFIL   5
    //   MOTIVO_DESC_PROMO_GENERAL 6
    //   MOTIVO_CONCEPTOS_SIN_DTOS 7
    memset( event_despro, 0, sizeof( struct _ev_despro ) );
    memset( nombre_archivo, 0, sizeof( nombre_archivo ) );
    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_E_DES_PRO );
    SELECT_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
    switch( tipo_motivo )                         //Motivos de origen del evento de descuento
    {
        case MOTIVO_DESC_PROMOCION :
            //Procesa todas las promociones que encuentra
            #ifdef INVEL_L
            handle_activas = open( "prom_acu.dat", O_RDWR );
            #else
            //, __LINE__, __FILE__ );
            handle_activas = _open( "prom_acu.dat", O_RDWR );
            #endif
            if( handle_activas > 0 ) {
                _READ( handle_activas, ( char* )promo_acumulada, sizeof( promo_acumulada ),
                       __LINE__, __FILE__ );
                for( i = 0;i < 1000;i++ ) {
                    if( promo_acumulada[i] ) {
                        sprintf( nombre_archivo, "dp_pro.%d", i );
                        #ifdef INVEL_L
                        handle_descuentos = open( nombre_archivo, O_RDWR );
                        #else
                        // | O_DENYNONE);//, __LINE__, __FILE__ );
                        handle_descuentos = _open( nombre_archivo, O_RDWR );
                        #endif
                        if( handle_descuentos > 0 ) {
                            rta = _READ( handle_descuentos, ( char* )s_monto, sizeof( double ),
                                         __LINE__, __FILE__ );
                            j = 0;
                            while( rta == 0 ) {
                                memcpy( &monto, &s_monto, 8 );
                                if( monto != 0 ) {
                                    event_despro->status = 0;
                                    event_despro->id_evento = RAM_ID_EVENTO;
                                    event_despro->posicion = j + 1;
                                    //promo,perfil,etc.
                                    event_despro->tipo_motivo = tipo_motivo;
                                    //cod_promo
                                    event_despro->id_motivo = i;
                                    event_despro->importe = monto;
                                    strcpy( event_despro->codigo_barra, _TICKET_COD_BARRA( j ) );
                                    event_despro->cod_articulo = _TICKET_COD_ESPECIAL( j );
                                    event_despro->cod_sucursal = config.sucursal;
//                                    event_despro->fecha_ticket = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
                                    event_despro->fecha_ticket = GET_FECHA_DOS();
                                    event_despro->caja_z = ( long )( NRO_CAJA )*100000L
                                                         + ( NRO_Z % 100000L );
                                    event_despro->caja = NRO_CAJA;
                                    event_despro->nro_z = NRO_Z;
                                    #ifdef INVEL_L
                                    //BEGIN_TRANSACTION();
                                    #endif
                                    error = INSERT2( NO );
                                    #ifdef INVEL_L
                                    //END_TRANSACTION();
                                    #endif
                                }
                                j++;
                                rta = _READ( handle_descuentos, ( char* )s_monto, sizeof( double ),
                                             __LINE__, __FILE__ );
                            }
                            _CLOSE( handle_descuentos, __LINE__, __FILE__ );
                            _unlink( nombre_archivo );
                        }
                    }
                }
                _CLOSE( handle_activas, __LINE__, __FILE__ );
                _unlink( "prom_acu.dat" );
            }
            break;
        case MOTIVO_DESC_MANUAL_PESOS :
            sprintf( nombre_archivo, "dp5.dat" );
            break;
        case MOTIVO_DESC_MANUAL_PORC  :
            sprintf( nombre_archivo, "dp6.dat" );
            break;
        case MOTIVO_DESC_PERFIL       :
            sprintf( nombre_archivo, "dp_per.dat" );
            break;
        case MOTIVO_REC_DESDE_PERFIL  :
            sprintf( nombre_archivo, "dp_rec.dat" );
            signo = ( RAM_CAMBIO ) ? 1 : -1;
            break;
        case MOTIVO_DESC_PROMO_GENERAL:
            sprintf( nombre_archivo, "dp_pro.dat" );
            borrar = 1;
            break;
        case MOTIVO_CONCEPTOS_SIN_DTOS:
            sprintf( nombre_archivo, "dp_pro.dat" );
            borrar = 1;
            break;
    }
    if( nombre_archivo )                         //Procesa eventos para base de descuento abierta. Porcentual y por Importe
    {
        #ifdef INVEL_L
        handle_descuentos = open( nombre_archivo, O_RDWR );
        #else
        handle_descuentos = _open( nombre_archivo, O_RDWR /*| O_DENYNONE */ );
        #endif
        if( handle_descuentos > 0 ) {
            rta = _READ( handle_descuentos, ( char* )s_monto, sizeof( double ), __LINE__,
                         __FILE__ );
            j = 0;
            while( rta == 0 ) {
                memcpy( &monto, &s_monto, 8 );
                if( monto != 0 ) {
                    event_despro->status = 0;
                    event_despro->id_evento = RAM_ID_EVENTO;
                    event_despro->posicion = j + 1;
                    //promo,perfil,etc.
                    event_despro->tipo_motivo = tipo_motivo;
                    //cod_promo/cod_perfil
                    event_despro->id_motivo = id_motivo;
                    event_despro->importe = signo * monto;
                    event_despro->cod_sucursal = config.sucursal;
                    //event_despro->fecha_ticket = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
                    event_despro->fecha_ticket = GET_FECHA_DOS();
                    event_despro->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
                    event_despro->caja = NRO_CAJA;
                    event_despro->nro_z = NRO_Z;
                    event_despro->no_es_dto = ( tipo_motivo == MOTIVO_CONCEPTOS_SIN_DTOS ) ? 'S'
                                            : 'N';
                    strcpy( event_despro->codigo_barra, _TICKET_COD_BARRA( j ) );
                    event_despro->cod_articulo = _TICKET_COD_ESPECIAL( j );
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    error = INSERT2( NO );
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
                j++;
                rta = _READ( handle_descuentos, ( char* )s_monto, sizeof( double ), __LINE__,
                             __FILE__ );
            }
            _CLOSE( handle_descuentos, __LINE__, __FILE__ );
            if( borrar ) {
                _unlink( "dp_pro.dat" );
            }
        }
    }
    if( error ) {
        GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_ARCHIVO_E_DESPRO_BTR ,LOG_VENTAS,2);
        sprintf( aux, "N %d", error );
        GRABAR_LOG_SISTEMA( aux ,LOG_DEBUG,2);
    }
    SELECT_TABLE( tabla_ant, tipo_ant );
	//SELECT( area_ant );
}
/**************************************************************************/
void REVERTIR_EVENTO_DESC_PROMO( int motivo_descuento, int id_motivo )
/**************************************************************************/
{
    int debe_borrar = 0, /*area_ant,*/ tabla_ant, tipo_ant;
    //memset( indice, 0, sizeof( struct _ind_e_des_pro_btr ) );
    //indice->id_evento = RAM_ID_EVENTO;
    //area_ant = SELECTED();
    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_E_DES_PRO );
    SELECT_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
    //SET_ORDER( 4 );
    //GET_GREATER_E( ( char* )indice );
    SET_WHERE( "ID_EVENTO >= '%ld'", RAM_ID_EVENTO );
    RUN_QUERY( NO );
    while( event_despro->id_evento == RAM_ID_EVENTO && !dbEOF() ) {
        /*debe_borrar  = (!motivo_descuento && !id_motivo);  //Asume que son todos para el RAM_ID_EVENTO actual
        debe_borrar |= ( motivo_descuento && motivo_descuento == event_despro->tipo_motivo );
        debe_borrar |= ( id_motivo && motivo_descuento
                        && id_motivo == event_despro->id_motivo
                        && motivo_descuento == event_despro->tipo_motivo);*/
        debe_borrar = ( motivo_descuento && motivo_descuento == event_despro->tipo_motivo );
        if( debe_borrar ) {
            DELETE2();
        }
        else {
            //GET_NEXT();
            SKIP2( 1 );
        }
    }
    SELECT_TABLE( tabla_ant, tipo_ant );
}
/***********************************************************************/
void GRABAR_EVENT_FACTURA( int nro_cuerpo )
/***********************************************************************/
{
    int k;
    //SELECT( AREA_FACTURA );
    SELECT_TABLE( T_EV_FACTU, tipo_t_ev_factu );
    event_factura->id_evento = RAM_ID_EVENTO;
    event_factura->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    event_factura->caja = NRO_CAJA;
    event_factura->nro_z = NRO_Z;
    if( UTILIZAR_DIA_OPERATIVO ) {
        event_factura->nro_dia_operativo = NRO_DIA_OPERATIVO;
    }
    else {
        event_factura->nro_dia_operativo = 0;
    }
    event_factura->cod_cliente = RAM_NRO_CLIENTE;
    event_factura->fecha_ticket = ( NRO_FECHA_OPERACION ) ? NRO_FECHA_OPERACION : _GET_FECHA_DOS();
    //Se comenta hasta verificar esto
	//memset( event_factura->reserva, 0, sizeof( event_factura->reserva ) );
    event_factura->sucursal = ( short int ) CUERPOS[nro_cuerpo].nro_sucursal;
    event_factura->nro_comprobante_new = CUERPOS[nro_cuerpo].nro_comprobante;
	{//Controlar el tama�o del campo nro_comprobante_new
		char nro[12];//No debe superar los 10 digitos porque es lo maximo que el campo del servidor acepta
		memset(nro,0,sizeof(nro));
		_snprintf( nro, sizeof(nro)-2,"%lf",event_ticket.nro_comprobante_new );
		event_ticket.nro_comprobante_new = atof(nro);
	}
    event_factura->bultos = ( unsigned int ) CUERPOS[nro_cuerpo].bultos;
    event_factura->importe_sin_iva = CUERPOS[nro_cuerpo].neto;
    event_factura->importe_total = CUERPOS[nro_cuerpo].total;
    event_factura->exento = CUERPOS[nro_cuerpo].exento;
    event_factura->costo = CUERPOS[nro_cuerpo].costo;
    event_factura->tipo_factura = ( char )CUERPOS[nro_cuerpo].tipo;
    event_factura->imp_int = CUERPOS[nro_cuerpo].imp_int;
    event_factura->iva1 = CUERPOS[nro_cuerpo].iva1;
    event_factura->iva2 = CUERPOS[nro_cuerpo].iva2;
    /*
    for( k = 0; k < 20; k++ )
      event_factura->impuestos[k] = 0.0 ;
    event_factura->impuestos[0] = CUERPOS[nro_cuerpo].ila;
    event_factura->impuestos[1] = CUERPOS[nro_cuerpo].iha;
    event_factura->impuestos[3] = CUERPOS[nro_cuerpo].alicuota;
    */
    for( k = 0;k < 20;k++ ) {
        event_factura->impuestos[k] = CUERPOS[nro_cuerpo].total_impuestos[k];
    }
    event_factura->importe_descuento = CUERPOS[nro_cuerpo].total_descuento;
    event_factura->nro_orden = nro_cuerpo;
    #ifdef INVEL_L
    //BEGIN_TRANSACTION();
    #endif
    INSERT2( NO );
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
}
/***********************************************************************/
long OBTENER_POS_SUPER(long id_evento)
/***********************************************************************/
{
    int area_ant = 0, tipo_ant = 0;
    long rta;
    
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    
    SELECT_TABLE( T_EV_SUPER, tipo_t_ev_super );
    SET_WHERE("ID_EVENTO='%ld'", id_evento );
    SET_ORDER2( "POSICION" );
    RUN_QUERY( NO );
    GO2( BOTTOM );
    if( !dbEOF() ) {
        rta = event_super->posicion;
    }else {
        rta = 0;
    }
    SELECT_TABLE( area_ant, tipo_ant );

    return rta;
}
/*****************************************************************************/
int LIBERAR_ARRAY_NRO_EVENTOS( void )
/*****************************************************************************/
{
    FREEMEM( ARRAY_NRO_EVENTO );
    return 0;
}

/*****************************************************************************/
void VACIAR_ARCHIVOS_EVENTOS( int pTipo )
/*****************************************************************************/
{

        GRABAR_LOG_SISTEMA( "VA A VACIAR EVENTOS", LOG_VENTAS,2 );
		SET_EMPTY_TABLE(T_EVENTOS,pTipo);
		SET_EMPTY_TABLE(T_EV_CONT,pTipo);
		SET_EMPTY_TABLE(T_EV_DESPRO,pTipo);
		SET_EMPTY_TABLE(T_EV_FACTU,pTipo);
		SET_EMPTY_TABLE(T_PAGOS,pTipo);
		SET_EMPTY_TABLE(T_VARIOS,pTipo);
		SET_EMPTY_TABLE(T_EV_CIERRE,pTipo);
		SET_EMPTY_TABLE(T_EV_DMEDIOS,pTipo);
		SET_EMPTY_TABLE(T_EV_SUPER,pTipo);
		SET_EMPTY_TABLE(T_EV_B_PROMO,pTipo);
		SET_EMPTY_TABLE(T_EV_COD_BARRA,pTipo);
}

/*****************************************************************************/
void MARCAR_ITEM_ORIGINAL_EN_E_TICKET( int h )
/*****************************************************************************/
{
    struct
    {
        long caja_z;
        long id_evento;
    } indice;

	int hubo_update = 0, error = 0;
	double precio_unitario_art_actual = 0.0;

	precio_unitario_art_actual = ROUND( fabs( _TICKET_IMPORTE_SIN_IVA( h )
                                                + _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h )
                                                + _TICKET_IMPUESTO1_PRECIO( h )
                                                + _TICKET_IMPUESTO2_PRECIO( h )
                                                + _TICKET_IMPUESTO3_PRECIO( h )
                                                + _TICKET_IMPUESTO4_PRECIO( h )
												+ _TICKET_EXENTO( h ) ), ENTEROS,
                                            DECIMALES );
	//SELECT( AREA_E_TICKET );
	SELECT_TABLE( T_EV_CONT, tipo_t_eventos );
    indice.id_evento = RAM_ID_EVENTO;
    indice.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    //SET_ORDER( 2 );
	//GET_EQUAL( (char*)&indice );
	SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld'", indice.caja_z, indice.id_evento );
	RUN_QUERY(NO);
	while( /*!BtrvEOF()*/ !dbEOF() && event_e_ticket.id_evento == indice.id_evento
					&& event_e_ticket.caja_z == indice.caja_z ) {
		/*if( !event_e_ticket.excluir_promocion
			&& event_e_ticket.cod_articulo == _TICKET_COD_ESPECIAL( h )
			&& ( strcmp( event_e_ticket.cod_barra, _TICKET_COD_BARRA( h ) ) == 0 ) ) {*/

		 if( event_e_ticket.cod_articulo == _TICKET_COD_ESPECIAL( h )
			  && ( strcmp( event_e_ticket.cod_barra, _TICKET_COD_BARRA( h ) ) == 0 )
			  &&  ( !event_e_ticket.excluir_promocion || ( event_e_ticket.excluir_promocion == 1
			        && _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO )  ) ) {

			if( /*RAM_DIFERENCIA_PRECIO &&*/ _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO ) {
				event_e_ticket.excluir_promocion = 2; //marco con -1, indicando q existe una diferencia de precio.
				event_e_ticket.precio_original = event_e_ticket.precio_unitario - precio_unitario_art_actual;
			}

			if( ( _TICKET_TIPO( h ) & _RESTA ) && ( _TICKET_ORIGEN( h ) == _ORIGEN_TICKET ) ) {
				//si se trata de un item de anulaci�n, �ste se debe excluir directamente
				event_e_ticket.excluir_promocion = 1; //marco con 1, indicando q es un anulado
			}

			#ifdef INVEL_L
			//BEGIN_TRANSACTION();
			#endif
			if( UPDATE2() ) {
				MENSAJE_STRING( S_NO_SE_PUDO_ACTUALIZAR );
				GRABAR_LOG_SISTEMA( ST( S_NO_SE_PUDO_ACTUALIZAR ) , LOG_VENTAS,2 );
            }
			#ifdef INVEL_L
			//END_TRANSACTION();
			#endif
			hubo_update = 1;
		}
		SKIP2(1);
    }
    //Se comenta ya que esta de mas
	/*if( hubo_update ) {
		CLOSE_DATABASE( AREA_E_TICKET );
		error = USE_DB( AREA_E_TICKET, _E_TICKET_BTR, ( char* )&event_e_ticket,
						sizeof( event_e_ticket ), NULL, 0, 0 );
		if( error ) {
			GRABAR_LOG_SISTEMA( "Error abriendo e_ticket" );
		}
	}*/
}
/*****************************************************************************/
void MODIFICAR_EN_E_TICKET( int h, int exento )
/*****************************************************************************/
{
    struct
    {
        long caja_z;
        long id_evento;
		int posicion;
    } indice;


	int hubo_update = 0, error = 0;
	double signo;
	float cant;



    //SELECT( AREA_E_TICKET );
    SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	GO2(TOP);
    indice.id_evento = RAM_ID_EVENTO;
    indice.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
	indice.posicion = h+1;
    //SET_ORDER( 3 );
    SET_ORDER2("CAJA_Z, ID_EVENTO, POSICION");
    
	//GET_EQUAL( (char*)&indice );
	SET_WHERE( "CAJA_Z = '%ld' AND ID_EVENTO = '%ld' AND POSICION ='%i'", indice.caja_z, indice.id_evento, indice.posicion );
	RUN_QUERY(NO);	

	if(FOUND2() && event_e_ticket.id_evento == indice.id_evento && event_e_ticket.caja_z == indice.caja_z && event_e_ticket.posicion == indice.posicion ) {
			int signonegativo = 0;
			signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
			cant = _TICKET_CANTIDAD( h );
			cant = ( cant > 0 ) ? cant : -cant;
			if( ( MODO_DEVOLUCION || RAM_NOTA_CR ) && ( _TICKET_TIPO( h ) & _DEV_ENVASE ) ) {
				signo *= -1;
			}
			if(event_e_ticket.total <0.00) 
				signonegativo = 1;

			event_e_ticket.total = ROUND( ( _TICKET_PRECIO( h ) + _TICKET_RECARGO( h ) ) * cant * signo ,
                                  ENTEROS, DECIMALES );
			if((event_e_ticket.total >0.00 && signonegativo ==1 ) || (event_e_ticket.total < 0.00 && signonegativo == 0))
				event_e_ticket.total*= -1;

		
			event_e_ticket.imp_int = _TICKET_IMP_INT( h ) * signo;
			if ( exento == SI)
				event_e_ticket.exento = (_TICKET_EXENTO( h ) - _TICKET_IMP_INT( h ))  * signo;
			else {
				event_e_ticket.importe_sin_iva = (_TICKET_IMPORTE_SIN_IVA( h ) - _TICKET_IMP_INT( h ))  * signo;
			event_e_ticket.iva1 = ( _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h ) ) * signo;
			}
			
			if( IMPRESORA_FISCAL == FISCAL && ( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1) ) {
		
				double detalle = 0.00, ddff = 0.00;
				int signonegativo = 0;
				if(event_e_ticket.total <0.00) 
					signonegativo = 1;
				

				detalle = (event_e_ticket.importe_sin_iva +event_e_ticket.exento + event_e_ticket.iva1)  * event_e_ticket.cantidad + (event_e_ticket.imp_int * signo );
				event_e_ticket.total = ROUND( ( _TICKET_PRECIO( h ) + _TICKET_RECARGO( h ) ) * cant * signo ,
					                ENTEROS, DECIMALES );
				if((event_e_ticket.total >0.00 && signonegativo ==1 ) || (event_e_ticket.total < 0.00 && signonegativo == 0))
					event_e_ticket.total*= -1;

				detalle= fabs(detalle);
				ddff = fabs( detalle - fabs(event_e_ticket.total)) ;
				if( ddff > 0.001 && ddff < 0.05 ){ //grande la diferencia
					
					int mismo = ES_MISMO_SIGNO( event_e_ticket.total, detalle);
					if( mismo == NO )
						detalle = -detalle;
					event_e_ticket.total = ROUND(  detalle,ENTEROS, DECIMALES + 2  );
					_SET_MEMORY_DOUBLE( __ticket_precio, h, ( event_e_ticket.total - _TICKET_RECARGO( h ) )  / cant * signo );

				}
				event_e_ticket.precio_unitario = ROUND( fabs( _TICKET_IMPORTE_SIN_IVA( h )
                                                + _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h )
                                                + _TICKET_IMPUESTO1_PRECIO( h )
                                                + _TICKET_IMPUESTO2_PRECIO( h )
                                                + _TICKET_IMPUESTO3_PRECIO( h )
                                                + _TICKET_IMPUESTO4_PRECIO( h )
												+ _TICKET_EXENTO( h ) ), ENTEROS,
                                            DECIMALES );

				event_e_ticket.alicuota = _TICKET_DESCUENTO(h) * signo;//  _TICKET_ALICUOTA_PRECIO(h); //guardo el importe de la impresora para la impresora en reverso de promo
			} 


			glog("MODIFICAR_EN_E_TICKET",1,1);
			if( UPDATE2() ) {
				MENSAJE_STRING( S_NO_SE_PUDO_ACTUALIZAR );
				GRABAR_LOG_SISTEMA( ST( S_NO_SE_PUDO_ACTUALIZAR ) , LOG_VENTAS,2 );
            }
			hubo_update = 1;

    }
 }

/****************************************************************************/
void BORRAR_EVENT_SUPER( long id_evento )
/****************************************************************************/
{
   SELECT_TABLE( T_EV_SUPER, TT_ORIG );
   SET_WHERE("");
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
	SET_ORDER2("");
	RUN_QUERY(NO);
    //GET_EQUAL( ( char* )&id_evento );
   if( FOUND2() ) {
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        DELETE2();
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
    }
}

/****************************************************************************/
void CORREGIR_TICKET( int tabla )
/****************************************************************************/
{ //intentemos corregir un insert fallido en la mysql
	//Para los binary UTILIZAMOS un rango entre 0 y 127 
	int RANGO_BINARY_MAX = 127;
	int RANGO_BINARY_MIN = 0;
	int error = 0;
	char cadena[80];
	
	memset(cadena,0,sizeof(cadena));
	//campos char[] sin fin de cadena
	if( tabla == 0 ) { //ticket
		 event_ticket.str_cliente_afinidad[16]= 0; 
		 event_ticket.legajo_empleado[15] = 0;	
		 event_ticket.cant_promo_afinidad[4]= 0; 
		 event_ticket.dosificacion_orden[12]= 0;
		 event_ticket.nri[14]= 0;							
		 event_ticket.cuit[20]= 0;
		 event_ticket.serial_impresora[20] = 0;		
		 event_ticket.serial_impresora_ant[20] = 0;  
	} else {
		event_e_ticket.cod_barra[16] = 0;				//87
		event_e_ticket.nro_vendedors[16]= 0;			//221
		event_e_ticket.cod_articulo_alfa[20]= 0;
		event_e_ticket.cod_clasificacion[35]= 0;
	}
	 //los binary los controlamos que no supere los 127
	if( tabla == 0 ) { //ticket
		
		 if( event_ticket.cambio < RANGO_BINARY_MIN 
				|| event_ticket.cambio > RANGO_BINARY_MAX ) {
			 event_ticket.cambio = ( RAM_CAMBIO ) ? 1 : 0;
			 error += 1;
		 }

		if( event_ticket.tipo_comprobante_anterior < RANGO_BINARY_MIN
				|| event_ticket.tipo_comprobante_anterior > RANGO_BINARY_MAX ) {
			event_ticket.tipo_comprobante_anterior = 
				( RAM_TIPO_COMP_ANTERIOR >= RANGO_BINARY_MIN 
					&& RAM_TIPO_COMP_ANTERIOR <= RANGO_BINARY_MAX )? RAM_TIPO_COMP_ANTERIOR : 0 ;
			error += 2;
		}	

		if( event_ticket.reproceso < RANGO_BINARY_MIN 
				|| event_ticket.reproceso > RANGO_BINARY_MAX ) {		
			event_ticket.reproceso = 0;
			error += 4;
		}

		if( event_ticket.importado < RANGO_BINARY_MIN 
			|| event_ticket.importado > RANGO_BINARY_MAX ) {		
			event_ticket.importado = ( RAM_EN_IMPORTACION_DE_TICKET == 3 )? 1:0;
			error += 8;
		}

		if( event_ticket.status_remote < RANGO_BINARY_MIN 
				|| event_ticket.status_remote > RANGO_BINARY_MAX ) {
			event_ticket.status_remote = 0;
			error += 16;
		}

		if( event_ticket.tipo_evento_importado < RANGO_BINARY_MIN
				|| event_ticket.tipo_evento_importado > RANGO_BINARY_MAX ) {
			event_ticket.tipo_evento_importado = 0;
			error += 32;
		}

		if( event_ticket.tipo_evento_orig < RANGO_BINARY_MIN 
				|| event_ticket.tipo_evento_orig > RANGO_BINARY_MAX )  {     
			event_ticket.tipo_evento_orig = 0;
			error += 64;
		}
	}else {
		int h = RAM_P_TICKET-1;

		if( event_e_ticket.cod_presentacion < RANGO_BINARY_MIN
				|| event_e_ticket.cod_presentacion > RANGO_BINARY_MAX )	{
			event_e_ticket.cod_presentacion = 
				(_TICKET_COD_PRESENT( h ) >= RANGO_BINARY_MIN 
					&&  _TICKET_COD_PRESENT( h ) <= RANGO_BINARY_MAX )? _TICKET_COD_PRESENT( h ) :0;
			error += 1;
		}

		if( event_e_ticket.cod_ext < RANGO_BINARY_MIN
				|| event_e_ticket.cod_ext > RANGO_BINARY_MAX ) {
			event_e_ticket.cod_ext = 
				(_TICKET_COD_EXT( h ) >= RANGO_BINARY_MIN 
					&& _TICKET_COD_EXT( h ) <= RANGO_BINARY_MAX )? _TICKET_COD_EXT( h ) :0;
			error += 2;
		}
		
		if( event_e_ticket.cod_depto_old < RANGO_BINARY_MIN 
				|| event_e_ticket.cod_depto_old > RANGO_BINARY_MAX ) {	
			event_e_ticket.cod_depto_old = 0;
			error += 4;
		}

		if( event_e_ticket.cod_iva < RANGO_BINARY_MIN 
				|| event_e_ticket.cod_iva > RANGO_BINARY_MAX ) {
			event_e_ticket.cod_iva =
				(_TICKET_COD_IVA( h ) >= RANGO_BINARY_MIN 
					&& _TICKET_COD_IVA( h ) <= RANGO_BINARY_MAX )? _TICKET_COD_IVA( h ) :0;
			error += 8;
		}

		if( event_e_ticket.excluir_promocion < RANGO_BINARY_MIN 
				|| event_e_ticket.excluir_promocion > RANGO_BINARY_MAX ) {	
			event_e_ticket.excluir_promocion = 0;
			if( _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO )	{	
				event_e_ticket.excluir_promocion = 2;//marco con -1, indicando q existe una diferencia de precio.
			}
			if( ( _TICKET_TIPO( h ) & _RESTA ) && ( _TICKET_ORIGEN( h ) == _ORIGEN_TICKET ) ) {
				//si se trata de un item de anulaci�n, �ste se debe excluir directamente
				event_e_ticket.excluir_promocion = 1; //marco con 1, indicando q es un anulado
			}
			error += 16;
		}

		if( event_e_ticket.cod_movimiento < RANGO_BINARY_MIN 
				|| event_e_ticket.cod_movimiento > RANGO_BINARY_MAX ) {
			event_e_ticket.cod_movimiento = 
				(_TICKET_COD_MOVIMIENTO( h ) >= RANGO_BINARY_MIN 
					&&  _TICKET_COD_MOVIMIENTO( h )<= RANGO_BINARY_MAX )? _TICKET_COD_MOVIMIENTO( h )  :0;
			error += 32;
		}
		
		if( event_e_ticket.tipo3 < RANGO_BINARY_MIN
				|| event_e_ticket.tipo3 > RANGO_BINARY_MAX ) {
			event_e_ticket.tipo3 = 
				(_TICKET_TIPO3( h ) >= RANGO_BINARY_MIN 
					&& _TICKET_TIPO3( h )<= RANGO_BINARY_MAX )? _TICKET_TIPO3( h ) :0;
			error += 64;
		}
	}
	_snprintf( cadena, sizeof(cadena)-1,"CORREGIR_TICKET Tabla:%i Error:%i", tabla, error );
	GRABAR_LOG_BD(cadena, SI ,LOG_ERRORES,1);
	 
}

/****************************************************************************/
void GRABAR_EVENT_TICKET_MINIMO( )	
/****************************************************************************/
//cuando necesitemos un evento con lo minimo
{
    if( !NRO_FECHA_OPERACION )
      SET_MEMORY_INT( __nro_fecha_operacion, _GET_FECHA_DOS(  ) );
    if( ( event_ticket.tipo_evento >= 26 && event_ticket.tipo_evento <= 29 ) ) {
        SET_MEMORY_INT( __ram_hora_inicio_evento, GET_HORA_DOS() );
    }
    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    event_ticket.id_evento = RAM_ID_EVENTO;
    event_ticket.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    // event_ticket.fecha = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
    event_ticket.fecha = NRO_FECHA_OPERACION; //fecha actual
    event_ticket.nro_dia_operativo =( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : NRO_FECHA_OPERACION;
    event_ticket.hora = GET_HORA_DOS();
    event_ticket.cajero = NRO_CAJERO;
    event_ticket.sucursal = config.sucursal;
    event_ticket.nro_caja_dgi = NRO_CAJA_DGI;
    event_ticket.caja = NRO_CAJA;
    event_ticket.nro_z = NRO_Z;
  
}
/****************************************************************************/
int ES_MISMO_SIGNO( double primero, double segundo )
/*Funcion que retorna si es del mismo signo dos valores. Para no cambiar campos de tablas que ya tienen valores previos
y por mal calculo se cambie de signo en una modificacion de redondeo)
*/
/****************************************************************************/
{
	int rta= 0;
	int positivo = SI, positivo2= SI;

	if( primero < 0.00)
		positivo = NO;
	if( segundo < 0.00)
		positivo2 = NO;

	if( positivo == positivo2)
		return SI;
	else
	 return NO;
}

/****************************************************************************/
/* Control de limites para cada tipo de variable por errores en RNV y calculos */
/****************************************************************************/

/****************************************************************************/
void LIMITES_TIPO_ENTERO(int *entero){
/****************************************************************************/
char aux[50];
	//	Los l�mites para los tipos de datos num�ricos est�n definidos en la cabecera <limits>.
	memset(aux,0,sizeof(aux));
	if((*entero!=0)&&!( INT_MIN <= *entero <= INT_MAX )){
		sprintf(aux,"#ERR: %i",*entero);
		*entero =0;
	}
}
/****************************************************************************/
void LIMITES_TIPO_ENTERO_SSIGNO(unsigned int *entero){
/****************************************************************************/
char aux[50];
	//	Los l�mites para los tipos de datos num�ricos est�n definidos en la cabecera <limits>.
	memset(aux,0,sizeof(aux));
	if((*entero!=0)&&!( 0 <= *entero <= UINT_MAX )){
		sprintf(aux,"#ERR: %u",*entero);
		*entero =0;
	}
}
/****************************************************************************/
void LIMITES_TIPO_LONG(long *largo){
/****************************************************************************/
char aux[50];
	//	Los l�mites para los tipos de datos num�ricos est�n definidos en la cabecera <limits>.
	memset(aux,0,sizeof(aux));
	if((*largo!=0)&&!( LONG_MIN <= *largo <= LONG_MAX )){
		sprintf(aux,"#ERR: %li",*largo);
		*largo =0;
		
	}
}
/****************************************************************************/
void LIMITES_TIPO_LONG_SSIGNO(unsigned long *largo){
/****************************************************************************/
	char aux[50];
	//	Los l�mites para los tipos de datos num�ricos est�n definidos en la cabecera <limits>.
	memset(aux,0,sizeof(aux));
	if((*largo!=0)&&!( 0 <= *largo <= ULONG_MAX )){
		sprintf(aux,"#ERR: %lu",*largo);
		*largo =0;
		
	}
}
/****************************************************************************/
void LIMITES_TIPO_FLOTANTE(float *flotante){
/****************************************************************************/
	char aux[50];
	//Para obtener los rangos de los tipos de datos se puede acceder a las cabeceras <cfloat> (para flotantes) y <climits> (para enteros).
	memset(aux,0,sizeof(aux));
	if((*flotante!=0.0)&&!( FLT_MIN <= *flotante <= FLT_MAX )){
		sprintf(aux,"#ERR: %fl",*flotante);
		*flotante =0;
	}
}
/****************************************************************************/
void LIMITES_TIPO_DOBLE(double *doble){
/****************************************************************************/
	char aux[50];
	//Para obtener los rangos de los tipos de datos se puede acceder a las cabeceras <cfloat> (para flotantes) y <climits> (para enteros).
	memset(aux,0,sizeof(aux));
	if((*doble!=0.0)&&!( DBL_MIN <= *doble <= DBL_MAX )){
		sprintf(aux,"#ERR: %lf",*doble);
		*doble =0;
	}
}

/****************************************************************************/
void ACTUALIZAR_FECHA_EVENTOS( )
/****************************************************************************/
{
	//Esta funcion obtiene la fecha de la ev_super para el tipo de evento 9 y setea esta fecha en los 
	//eventos relacionados a este tipo de evento. De igual manera para el tipo de evento 10. Con este
	//procedimiento nos aseguramos q no haya diferencias en las fechas de los Tipo de eventos 8, 9, 27, 10
	int maximoId = 0, caja_z =  0, fecha = 0, dif_fecha_9 = 0, dif_fecha_10 = 0;
	char log[100];

	memset(log,0, sizeof(log));


	/*************** TIPO DE EVENTO 9 - CIERRE Y ***************/
	
	if(DIRECT_DB_QUERY("SELECT max(id_evento) FROM eventos WHERE tipo_evento = 9") > 0 ) { //OBTIENE EL MAXIMO ID EVENTO
        GET_DATO(1,C_INT,(char *)&maximoId,4);
		if( maximoId && DIRECT_DB_QUERY("SELECT fecha_ticket FROM ev_super WHERE id_evento = '%ld'", maximoId ) > 0 ) { 
			GET_DATO(1,C_INT,(char *)&fecha,4);
			if( fecha ) {
				DIRECT_DB_QUERY("UPDATE eventos SET fecha = '%i' WHERE id_evento = '%ld' AND tipo_evento = 8", fecha, maximoId - 1);
				DIRECT_DB_QUERY("UPDATE ev_cierre SET fecha_ticket = '%i' WHERE id_evento = '%ld'", fecha, maximoId);
				DIRECT_DB_QUERY("UPDATE eventos SET fecha = '%i' WHERE id_evento = '%ld'", fecha, maximoId);						
				DIRECT_DB_QUERY("UPDATE eventos SET fecha = '%i' WHERE id_evento = '%ld' AND tipo_evento = 27", fecha, maximoId + 1);
			}
		}
		_snprintf( log, sizeof( log ), "ACTUALIZAR_FECHA_EVENTOS Tipo Evento 9 Id %ld fecha %i", maximoId, fecha );
		log[sizeof( log ) - 1] = 0;
		GRABAR_LOG_SISTEMA( log, LOG_DEBUG, 2 );
	} 

	/*************** TIPO DE EVENTO 10 - CIERRE Z ***************/
	maximoId=0;
	if( DIRECT_DB_QUERY("SELECT max(id_evento) FROM eventos WHERE tipo_evento = 10") > 0 ) { //OBTIENE EL MAXIMO ID EVENTO
		GET_DATO(1,C_INT,(char *)&maximoId,4);
		if( maximoId ) {
			if( MANTENER_FECHA_Z && fecha ) {
				DIRECT_DB_QUERY("UPDATE ev_super SET fecha_ticket = '%i' WHERE id_evento = '%ld'", fecha, maximoId);
				DIRECT_DB_QUERY("UPDATE ev_cierre SET fecha_ticket = '%i' WHERE id_evento = '%ld'", fecha, maximoId);
				DIRECT_DB_QUERY("UPDATE eventos SET fecha = '%i' WHERE id_evento = '%ld'", fecha, maximoId);				
			} else {
				fecha = 0;
				if( DIRECT_DB_QUERY("SELECT fecha_ticket FROM ev_super WHERE id_evento = '%ld'", maximoId ) > 0 ) { 
					GET_DATO(1,C_INT,(char *)&fecha,4);
					if( fecha ) {
						DIRECT_DB_QUERY("UPDATE eventos SET fecha = '%i' WHERE id_evento = '%ld'", fecha, maximoId);				
						DIRECT_DB_QUERY("UPDATE ev_cierre SET fecha_ticket = '%i' WHERE id_evento = '%ld'", fecha, maximoId);
					}
				}
			}
		}
		_snprintf( log, sizeof( log ), "ACTUALIZAR_FECHA_EVENTOS Tipo Evento 10 Id %ld fecha %i", maximoId, fecha );
		log[sizeof( log ) - 1] = 0;
		GRABAR_LOG_SISTEMA( log, LOG_DEBUG, 2 );
	}

	/*
	//VERIFICAR DIFERENCIA EN FECHA TIPO DE EVENTO 9 - CIERRE Y
	if(DIRECT_DB_QUERY("SELECT max(id_evento) FROM eventos WHERE tipo_evento = 9") > 0 ) { //OBTIENE EL MAXIMO ID EVENTO
        GET_DATO(1,C_INT,(char *)&maximoId9,4);
		if( maximoId9 && DIRECT_DB_QUERY( "SELECT caja_z FROM eventos WHERE id_evento = '%ld'", maximoId9 ) > 0 ) {
			GET_DATO(1,C_INT,(char *)&caja_z9,4);
			if( caja_z9 && DIRECT_DB_QUERY(
					"select e.fecha from eventos e, ev_super s where e.id_evento = '%ld'and e.caja_z = '%ld' and s.caja_z = '%ld' and e.id_evento = s.id_evento and e.fecha <> s.fecha_ticket", 
					maximoId9, caja_z9, caja_z9 ) > 0 ) {
				GET_DATO(1,C_INT,(char *)&fecha9,4);
				if( fecha9 ) {
					dif_fecha_9 = 1;
				}
			}
		}
	}

	//VERIFICAR DIFERENCIA EN FECHA TIPO DE EVENTO 10 - CIERRE Z 
	maximoId = 0, caja_z =  0, fecha = 0;
	if(DIRECT_DB_QUERY("SELECT max(id_evento) FROM eventos WHERE tipo_evento = 10") > 0 ) { //OBTIENE EL MAXIMO ID EVENTO
        GET_DATO(1,C_INT,(char *)&maximoId10,4);
		if( maximoId10 && DIRECT_DB_QUERY( "SELECT caja_z FROM eventos WHERE id_evento = '%ld'", maximoId10 ) > 0 ) {
			GET_DATO(1,C_INT,(char *)&caja_z10,4);
			if( caja_z && DIRECT_DB_QUERY(
					"select e.fecha from eventos e, ev_super s where e.id_evento = '%ld'and e.caja_z = '%ld' and s.caja_z = '%ld' and e.id_evento = s.id_evento and e.fecha <> s.fecha_ticket", 
					maximoId10, caja_z10, caja_z10 ) > 0 ) {
				GET_DATO(1,C_INT,(char *)&fecha10,4);
				if( fecha10 ) {
					dif_fecha_10 = 1;
				} 
			}
		}
	}
	
	if( dif_fecha_9 ) {
		if( DIRECT_DB_QUERY("SELECT fecha_ticket FROM ev_super WHERE id_evento = '%ld'", maximoId9 ) > 0 ) { 
			GET_DATO(1,C_INT,(char *)&fecha9,4);
			if( fecha9 ) {
				DIRECT_DB_QUERY("UPDATE eventos SET fecha = '%i' WHERE id_evento = '%ld' AND tipo_evento = 8", fecha9, maximoId9 - 1);
				DIRECT_DB_QUERY("UPDATE eventos SET fecha = '%i' WHERE id_evento = '%ld'", fecha9, maximoId9);				
				DIRECT_DB_QUERY("UPDATE ev_cierre SET fecha_ticket = '%i' WHERE id_evento = '%ld'", fecha9, maximoId9);
				DIRECT_DB_QUERY("UPDATE eventos SET fecha = '%i' WHERE id_evento = '%ld' AND tipo_evento = 27", fecha9, maximoId9 + 1);
			}
		}	
	}
	*/

}
