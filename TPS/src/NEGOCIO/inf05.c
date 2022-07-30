#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <b_mens.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <fiscal.h> 
#include <imp_tick.h> 
#include <inf.h> 
#include <inf05.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pausa.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#define COMPILAR_INFORME5
#if defined(INVEL_W) || defined(INVEL_L)
#endif
struct _emision_informes informes;
//void EMITIR_INFORME_05( int ant, int cod_seg, int cod_vta );
//void INFORME_05_CABECERA( char anterior );
//void INFORME_05_CUERPO( int hora, int minutos, int segundos );
//void INFORME_05_PIE();
/*****************************************************************************/
void INFORME_05( int est )
/*****************************************************************************/
{
    #ifdef COMPILAR_INFORME5
	int ini_est;
	
	ini_est = _APUNTAR_STRUCT( est );
    switch( est ) {
        case _x:
            EMITIR_INFORME_05( NO, __x_demora_segundos, __x_demora_ventas, ini_est );
            break;
        case _z:
            EMITIR_INFORME_05( NO, __z_demora_segundos, __z_demora_ventas, ini_est );
            break;
        case _z_ant:
            EMITIR_INFORME_05( SI, __z_ant_demora_segundos, __z_ant_demora_ventas, ini_est );
            break;
    }
    #endif
}
#ifdef COMPILAR_INFORME5
/*****************************************************************************/
void EMITIR_INFORME_05( int ant, int cod_seg, int cod_vta, int ini_est )
/*****************************************************************************/
{
    int h,promedio,min,seg;
    unsigned segundos,ventas;
    /*---------------------------- Impresin ---------------------------------*/
    MENSAJE_SIN_SONIDO_STRING( S_IMPRIMIENDO_INFORME_PRODUCTIVIDAD_DE_CAJEROS );

	if( TKT_CONFIGURABLE ) {
		informes.prod_cod_seg = cod_seg;
		informes.prod_cod_vta = cod_vta;
		if( ant && NRO_MODO == 'Z' ) {
			//Para imprimir el NRO_Z anterior si es que se encuentra configurado en el tkt-caja.cfg
			SET_MEMORY_LONG( __nro_z, NRO_Z - 1 );
		}
        IMPRIMIR_TKT_INF05( ini_est );
		if( ant && NRO_MODO == 'Z' ) {
			SET_MEMORY_LONG( __nro_z, NRO_Z + 1 );
		}
        setData( ACTION_DOPRINT, NULL, 0 );
	} else {
		INFORME_05_CABECERA( ( char )ant );
		for( h = 0;h < 24;h++ ) {
			segundos = _GET_MEMORY_UNSIGNED( cod_seg, h );
			if( segundos ) {
				ventas = _GET_MEMORY_UNSIGNED( cod_vta, h );
				promedio = segundos / ventas;
				min = promedio / 60;
				seg = promedio - min * 60;
				INFORME_05_CUERPO( 0, h, min, seg );
			}
		}
		INFORME_05_PIE();
	}

    BORRAR_MENSAJE();
}
/*****************************************************************************/
void INFORME_05_CABECERA( char anterior )
/*****************************************************************************/
{
	char cad_aux[50];

	IMPRIME_CABECERA_TICKET( SI );
	if( IMPRESORA_FISCAL != FISCAL ) {
		MOSTRAR_CADENA( 15, PRN, ST( S_INFORME_05 ) );
		IMPRIME_NRO_INFORME( anterior );
		LF_RECEIPT( 1 );
		MOSTRAR_CADENA( 12, PRN, ST( S_HORA_____PROMEDIO ) );
		LF( 1 );
		MOSTRAR_CADENA( 12, PRN, "----     --------" );
		LF( 1 );
	} else {
		memset(cad_aux,0,sizeof(cad_aux));
		_snprintf(cad_aux, sizeof(cad_aux)-1, "     %s",ST( S_INFORME_05 ) );
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_DOBLE_ANCHO_NEGRITA, cad_aux, SI );
		
		IMPRIME_NRO_INFORME( anterior );

		memset(cad_aux,0,sizeof(cad_aux));
		_snprintf(cad_aux, sizeof(cad_aux)-1, "            %s",ST( S_HORA_____PROMEDIO ) );
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_RESALTADA, cad_aux, SI );
		
		memset(cad_aux,0,sizeof(cad_aux));
		_snprintf(cad_aux, sizeof(cad_aux)-1, "            %s","----     --------" );
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cad_aux, SI );

	}
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void INFORME_05_CUERPO( int pos_x, int hora, int minutos, int segundos )
/*****************************************************************************/
{
    char cad_aux[50];

	memset( cad_aux, 0, sizeof( cad_aux ) );// h, min, seg
	_snprintf( cad_aux, sizeof( cad_aux ) - 1, "             %d %d : %d", hora, minutos, segundos );

	if( IMPRESORA_FISCAL != FISCAL ) {
		if( TKT_CONFIGURABLE ) {
			MOSTRAR_CADENA( pos_x, PRN, cad_aux );
		} else {
			MOSTRAR_ENTERO( 13, PRN, "99", hora );
			MOSTRAR_ENTERO( 22, PRN, "99", minutos );
			MOSTRAR_CADENA( 24, PRN, ":" );
			MOSTRAR_ENTERO( 25, PRN, "00", segundos );
			LF( 1 );
		}
	} else {
		IMPRIMIR_TEXTO_NO_FISCAL( 1, LETRA_NORMAL, cad_aux, SI );
	}

    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void INFORME_05_PIE()
/*****************************************************************************/
{
    PRN_FIN_TICKET();
}
#endif

