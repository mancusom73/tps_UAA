#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <b_mens.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <imp_tick.h> 
#include <inf.h> 
#include <inf15.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pausa.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_INFORME15
void EMITIR_INFORME_15( int anterior, int cod_imp, int cod_can );
void INFORME_15_CABECERA( char anterior );
void INFORME_15_CUERPO( char *nombre, double importe, int porcentaje, int cantidad );
void INFORME_15_PIE( double importe, int porcentaje, int cantidad );
#endif
#ifdef COMPILAR_INFORME15
/*****************************************************************************/
void INFORME_15( int est )
/*****************************************************************************/
{
    
	int ini_est;
	
	ini_est = _APUNTAR_STRUCT( est );
    switch( est ) {
        case _x:
            EMITIR_INFORME_15( NO, __x_mozos_importe, __x_mozos_cantidad, ini_est );
            break;
        case _z:
            EMITIR_INFORME_15( NO, __z_mozos_importe, __z_mozos_cantidad, ini_est );
            break;
        case _z_ant:
            EMITIR_INFORME_15( SI, __z_ant_mozos_importe, __z_ant_mozos_cantidad, ini_est );
            break;
    }
}
#endif
#ifdef COMPILAR_INFORME15
/*****************************************************************************/
void EMITIR_INFORME_15( int anterior, int cod_imp, int cod_can, int ini_est )
/*****************************************************************************/
{
    double tot_importe = 0, importe, cantidad;
    int h, porc, tot_articulos = 0;
    char nom_mozo[] = {"MOZO ?"};
    /*--------------------------- Acumulacin --------------------------------*/
    for( h = 0;h < CANT_MOZOS;h++ ) {
        tot_importe += _GET_MEMORY_DOUBLE( cod_imp, h );
        tot_articulos += ( int )_GET_MEMORY_DOUBLE( cod_can, h );
    }
    /*---------------------------- Impresin ---------------------------------*/
    MENSAJE_SIN_SONIDO_STRING( S_IMPRIMIENDO_EL_INFORME_15 );
    INFORME_15_CABECERA( ( char )anterior );
	if( INFORME_X_Y_NO_CONFIGURABLE ){
		for( h = 0;h < CANT_MOZOS;h++ ) {
			importe = _GET_MEMORY_DOUBLE( cod_imp, h );
			if( importe ) {
				cantidad = _GET_MEMORY_DOUBLE( cod_can, h );
				porc = ( int )( importe / tot_importe * 100 );
				nom_mozo[5] = h + '0';
				INFORME_15_CUERPO( nom_mozo, importe, porc, ( int )( cantidad ) );
			}
		}
	}else{
		IMPRIMIR_TKT_INF15( ini_est );
	}

    INFORME_15_PIE( tot_importe, 100, tot_articulos );
    // PAUSA(91);
    BORRAR_MENSAJE();
}
/*****************************************************************************/
void INFORME_15_CABECERA( char anterior )
/*****************************************************************************/

{
	IMPRIME_CABECERA_TICKET( SI );
	if( INFORME_X_Y_NO_CONFIGURABLE ){	
		MOSTRAR_CADENA( 15, PRN, ST( S_INFORME_15 ) );
		IMPRIME_NRO_INFORME( anterior );
	}else{
		IMPRIMIR_TKT_INF15_CABECERA( );
	}
    LF_RECEIPT( 1 );
}
/*****************************************************************************/
void INFORME_15_CUERPO( char *nombre, double importe, int porcentaje, int cantidad )
/*****************************************************************************/
{
    char caracter_porc[2];
	caracter_porc[0]='%';
	caracter_porc[1]='\0';

	MOSTRAR_CADENA( 1, PRN, nombre );
    MOSTRAR_FLOTANTE( 18, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
    MOSTRAR_ENTERO( 29, PRN, "999", porcentaje );
    MOSTRAR_CADENA( 32, PRN, caracter_porc );
    MOSTRAR_ENTERO( 36, PRN, "9999", cantidad );
    LF( 1 );
}
/*****************************************************************************/
void INFORME_15_PIE( double importe, int porcentaje, int cantidad )
/*****************************************************************************/
{
    char caracter_porc[2];
	caracter_porc[0]='%';
	caracter_porc[1]='\0';
	if( INFORME_X_Y_NO_CONFIGURABLE ){
		LF( 1 );
		MOSTRAR_CADENA( 1, PRN, ST( S_TOTALES ) );
		MOSTRAR_FLOTANTE( 18, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
		MOSTRAR_ENTERO( 29, PRN, "999", porcentaje );
		MOSTRAR_CADENA( 32, PRN, caracter_porc );
		MOSTRAR_ENTERO( 36, PRN, "9999", cantidad );
	}else{
		//DEBE IR EN EL TKT
		/*if( IMPRESORA_FISCAL == FISCAL ) {
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, "%s %ld", ST( S_CANT__TICKETS_ ), NRO_TICKET - NRO_ULTIMO_TICKET_ANT );
			IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cad_aux, SI );

			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, "%s      %lf   %i%\%   %i", ST( S_TOTALES ), importe, porcentaje, cantidad );
			IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cad_aux, SI );
		}*/
	
	}
    PRN_FIN_TICKET();
}
#endif

