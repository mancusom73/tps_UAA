#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
#include <cierre.h> 
#include <clave.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <imp_fond.h> 
#include <imp_tick.h> 
#include <inf.h> 
#include <inf_tar.h> 
#include <lista.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
double importe[CANT_TARJETAS];
int cantida[CANT_TARJETAS];
double tot_importe;
int tot_cantidad;
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
//void EMITIR_INFORME_TARJETAS( int z, int anterior );
//void INFORME_TARJETAS_CABECERA( int anterior );
//void INFORME_TARJETAS_CUERPO( char *nombre, double importe, int cantidad );
//void INFORME_TARJETAS_PIE( double importe, int cantidad );
/*****************************************************************************/
int CONTINUA_BUSCA_EVENTO( void )
/*****************************************************************************/
{
    int rta = 0;
    if( event_ticket.tipo_evento == 9 || event_ticket.tipo_evento == _EVENT_INFORME ) {
        if( MULTIPLES_USUARIOS_PARA_VENTA ) {
            rta = ( event_ticket.cajero == NRO_CAJERO ) ? 0 : 1;
        }
    }
    else {
        rta = 1;
    }
    return ( rta );
}
/*****************************************************************************/
void EMITIR_INFORME_TARJETAS( int z, int anterior )
/*****************************************************************************/
{
    /*int h;*/
    /*--------------------------- Procesa eventos ---------------------------*/
    memset( importe, 0, sizeof( importe ) );
    memset( cantida, 0, sizeof( cantida ) );

	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_WHERE("");
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    if( !VARIOS_MODO_Y_EN_Z ) {
        GO2( BOTTOM );
    }
    if( !z )                                      // entra por la _x
    {
        while( !dbBOF() && !CONTINUA_BUSCA_EVENTO() ) {
            SKIP2( -1 );
        }
    }
    while( !dbBOF() && ( z || CONTINUA_BUSCA_EVENTO() ) ) {
        if( EVENTO_CON_PAGOS() && ( z || event_ticket.cajero == NRO_CAJERO ) ) {
			SELECT_TABLE( T_PAGOS, tipo_t_pagos );
			SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            while( !dbEOF() && event_pago.id_evento == event_ticket.id_evento ) {
                if( ( event_pago.modo_pago - 1 == _medio_tarjeta
                   || event_pago.modo_pago - 1 == _medio_tarjeta2 ) && event_pago.reproceso == 0 ) {
                    importe[( int )event_pago.tarjeta] += event_pago.importe;
                    cantida[( int )event_pago.tarjeta]++;
                }
                SKIP2( 1 );
            }
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
        }
        SKIP2( -1 );
    }
    INFORME_TARJETAS_CABECERA( anterior );
    /*---------------------------- Imprime informe --------------------------*/
    /*   for( h = 0; h < CANT_TARJETAS; h++ )
       {
          if( importe[h] )
          {
             INFORME_TARJETAS_CUERPO( NOMBRE_TARJETA( h ), importe[h], cantida[h] );
             tot_importe += importe[h];
             tot_cantidad += cantida[h];
          }
       }
       INFORME_TARJETAS_PIE( tot_importe, tot_cantidad ); */
}
/*****************************************************************************/
void INFORME_TARJETAS_CABECERA( int anterior )
/*****************************************************************************/
{
    char cadena[50];

	IMPRIME_CABECERA_TICKET( SI );
    IMPRIMIR_NOMBRE_USUARIO( USUARIO_CLAVE_ANTERIOR() );
    IMPRIMIR_NOMBRE_USUARIO( NRO_CAJERO );
    LF( 1 );
	if( IMPRESORA_FISCAL == FISCAL ) {
		memset(cadena,0,sizeof(cadena));
		_snprintf( cadena, sizeof( cadena )," %s", ST( S_INFORME_DE_TARJETAS ) );
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_DOBLE_ANCHO_NEGRITA, cadena, SI );
	}
	else 
	{
		MOSTRAR_CADENA( 11, PRN, ST( S_INFORME_DE_TARJETAS ) );
	}
    IMPRIME_NRO_INFORME( ( char )anterior );
    LF_RECEIPT( 1 );
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
	
}
/*****************************************************************************/
void INFORME_TARJETAS_CUERPO( int h /*char *nombre, double importe, int cantidad*/ )
/*****************************************************************************/
{
    //if (importe[h]) { 
	char cadena[50], cadena1[50];
	int i= 0;

	if( IMPRESORA_FISCAL == IMPRESORA_FISCAL ) {
		memset(cadena,0,sizeof(cadena));
		_snprintf( cadena, sizeof( cadena ) - 1,"%s", NOMBRE_TARJETA( h ) );
		if( strlen( cadena ) ) {
			memset(cadena1, 0, sizeof( cadena1 ) );
			for(i = strlen(cadena); i < 15; i++)
				cadena[i]= ' ';
			_snprintf( cadena1, sizeof( cadena1 ) - 1,"%*.*f", 
				ENTEROS, DECIMALES, importe[h] );
			strncat( cadena, cadena1, sizeof( cadena ) - strlen( cadena ) - 1 );

			memset(cadena1, 0, sizeof( cadena1 ) );
			for(i = strlen(cadena); i < 25; i++)
				cadena[i]= ' ';
			_snprintf( cadena1, sizeof( cadena1 ) - 1,"%d",cantida[h]);
			strncat( cadena, cadena1, sizeof( cadena ) - strlen( cadena ) - 1 );
			
			IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cadena, SI );
		}
	} else {
		MOSTRAR_CADENA( 1, PRN, NOMBRE_TARJETA( h ) );
		MOSTRAR_FLOTANTE( 18, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe[h] );
		MOSTRAR_ENTERO( 36, PRN, "9999", cantida[h] );
		LF( 1 );
		#if defined(INVEL_W) || defined(INVEL_L)
		setData( ACTION_PRINT, NULL, 0 );
		#endif
	}
	tot_importe += importe[h];
    tot_cantidad += cantida[h];
    //}
}
//SIN_LOTE_TARJETA
/*****************************************************************************/
//void INFORME_TARJETAS_LOTE( int cod_marca )
/*****************************************************************************/
/*{
   int k = 0, cantidad = 0, cant_vueltas = 0, j; 
   double subtotal = 0;
   long busca_lote = 0;
   struct _lote_tar *lote;
   
   //if (!importe[cod_marca]) //si no hay ventas con esa tarjeta
      //return;
   lote = OBTENER_IMPORTE_LOTE( &j );
   
   while(cant_vueltas <= j) { //a lo sumo j lotes distintos
      for(k = 0; k<j; k++) {
         if ((lote[k].cod_tar == cod_marca) && (lote[k].ingresado == 'n')) {
            if ( (!busca_lote) || (busca_lote == lote[k].lote) ) {
               busca_lote = lote[k].lote;
               lote[k].ingresado = 's';
               subtotal += lote[k].importe;
               cantidad ++;
            }
         }
      }
      if (cantidad) { 
         MOSTRAR_CADENA( 1, PRN, "  LOTE" );
         MOSTRAR_ENTERO( 8, PRN, "99999999", busca_lote );
         MOSTRAR_FLOTANTE( 18, PRN, ENTEROS + 1, DECIMALES, 1, 0, subtotal );
         MOSTRAR_ENTERO( 36, PRN, "9999", cantidad );
         LF( 1 );
#if defined(INVEL_W) || defined(INVEL_L)
         setData( ACTION_PRINT, NULL, 0 );
#endif
      }
      busca_lote = 0;
      subtotal = 0;
      cantidad = 0;
      cant_vueltas ++;
   }
   if (lote) free(lote);
}
*/
/*****************************************************************************/
void INFORME_TARJETAS_PIE()
/*****************************************************************************/
{
	char cadena[50], cadena1[50];
	int i = 0;

	LF( 1 );
	if( IMPRESORA_FISCAL == IMPRESORA_FISCAL ) {
		memset(cadena,0,sizeof(cadena));
		memset(cadena1, 0, sizeof( cadena1 ) );
		_snprintf( cadena, sizeof( cadena ) - 1,"TOT..          %*.*f", ENTEROS, DECIMALES, tot_importe );
		
		memset(cadena1, 0, sizeof( cadena1 ) );
		for(i = strlen(cadena); i < 25; i++)
			cadena[i]= ' ';
		_snprintf( cadena1, sizeof( cadena1 ) - 1,"%d",tot_cantidad);
		strncat( cadena, cadena1, sizeof( cadena ) - strlen( cadena ) - 1 );
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cadena, SI );
	}
	else {
		MOSTRAR_CADENA( 1, PRN, ST( S_TOTAL ) );
		MOSTRAR_FLOTANTE( 18, PRN, ENTEROS + 1, DECIMALES, 1, 0, tot_importe );
		MOSTRAR_ENTERO( 36, PRN, "9999", tot_cantidad );
		LF( 1 );
		#if defined(INVEL_W) || defined(INVEL_L)
		setData( ACTION_PRINT, NULL, 0 );
		#endif
	}
	PRN_FIN_TICKET();
}


/*****************************************************************************/
int POSICION_TARJETA()
/*****************************************************************************/

{
    int posicion =0;				
	
	SELECT_TABLE( T_PAGOS, TT_ORIG );
	SET_WHERE("TARJETA > 0");
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
	if( FOUND2() ) { //ACA ESTA EL ULTIMO
		GO2( BOTTOM);
		posicion = event_pago.posicion;
	}
	return posicion;
}
//select posicion,ev_medios.* from ev_medios where  tarjeta > 0 order by id_evento desc
