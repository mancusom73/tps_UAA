#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <fiscal.h> 
#include <imp_tick.h> 
#include <inf.h> 
#include <inf00.h> 
#include <inf10.h> 
#include <inf_dif.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <aarch.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
//void INFORME_10_PIE( double importe, int porcentaje, int cantidad, int anterior );
extern unsigned char ATRIB_DISCR_DPTO[CANT_DEPTOS];
void IMPRIMIR_USUARIO_Y_SUPERVISOR( void );
//void EMITIR_INFORME_10( int anterior, int cod_imp, int cod_can );
//void INFORME_10_CABECERA( char anterior );
//void INFORME_10_CUERPO( char *nombre, double importe, int porcentaje, int cantidad );
static int cant_tickets;
/*****************************************************************************/
void INFORME_10( int est )
/*****************************************************************************/
{
	//if( config.pais != COLOMBIA && config.pais != ECUADOR ) {
	int modo=NRO_MODO;//para chequear con el tkt que estructura tomar
	int ini_est;

	ini_est = _APUNTAR_STRUCT( est );
	switch( est ) {
            case _x:
				cant_tickets = X_CANTIDAD_TICKETS;
                EMITIR_INFORME_10( NO, __x_dep_imp, __x_dep_can, ini_est );
                break;
            case _z:
				cant_tickets =  Z_CANTIDAD_TICKETS;
                EMITIR_INFORME_10( NO, __z_dep_imp, __z_dep_can, ini_est );
                break;
            case _z_ant:
				cant_tickets =  Z_ANT_CANTIDAD_TICKETS;
                EMITIR_INFORME_10( SI, __z_ant_dep_imp, __z_ant_dep_can, ini_est );
                break;
        }
    //}
}
/*****************************************************************************/
void EMITIR_INFORME_10( int anterior, int cod_imp, int cod_can, int ini_est )
/*****************************************************************************/
{
    double tot_importe = 0, importe, cantidad, tot_importe_no_disc = 0;
    //char cod;
    int h, porc, tot_articulos = 0, tot_articulos_no_disc = 0;
    struct _departamentos depto;
    /*if( USE_DB( AREA_AUX, _DPTO_SIC, ( char* )&depto, sizeof( depto ), NULL, 0, 0 )*/
	 if( OPENED( T_DPTO, TT_ORIG) )
	    CLOSE_TABLE( T_DPTO, TT_ORIG );

	 if( OPEN_TABLE(T_DPTO, TT_ORIG,( char* )&depto,sizeof( depto ))== 0 ) {
        /*--------------------------- Acumulaci¢n --------------------------------*/
        for( h = 0;h < CANT_DEPTOS;h++ ) {
            tot_importe += _GET_MEMORY_DOUBLE( cod_imp, h );
            tot_articulos += ( int )_GET_MEMORY_DOUBLE( cod_can, h );
            if( ATRIB_DISCR_DPTO[h] != 'S' && ATRIB_DISCR_DPTO[h] != 's' ) {
                tot_importe_no_disc += _GET_MEMORY_DOUBLE( cod_imp, h );
                tot_articulos_no_disc += ( int )_GET_MEMORY_DOUBLE( cod_can, h );
            }
        }
        /*---------------------------- Impresi¢n ---------------------------------*/
        MENSAJE_SIN_SONIDO_STRING( S_IMPRIMIENDO_INFORME_DE_VENTAS_POR_DEPARTAMENTO );

		INFORME_10_CABECERA( ( char )anterior );
		if( INFORME_X_Y_NO_CONFIGURABLE ){
			if( !TKT_CONFIGURABLE ) {
				IMPRIMIR_USUARIO_Y_SUPERVISOR();
			}
			if( INFORME_10_REDUCIDO ) {
				INFORME_10_CUERPO( ST( S_SUBTOTAL_NO_DISCRIM_ ), tot_importe_no_disc, -1,
								   ( int )tot_articulos_no_disc );
			}
			for( h = 0;h < CANT_DEPTOS;h++ ) {
				if( ( INFORME_10_REDUCIDO
				   && ( ATRIB_DISCR_DPTO[h] == 'S' || ATRIB_DISCR_DPTO[h] == 's' ) )
				 || !INFORME_10_REDUCIDO ) {
					importe = _GET_MEMORY_DOUBLE( cod_imp, h );
					cantidad = _GET_MEMORY_DOUBLE( cod_can, h );
					//cod = ( char )h;
					//GET_EQUAL( &cod );
					SELECT_TABLE( T_DPTO, TT_ORIG );
					SET_WHERE("COD_DEPTO = '%i'",h);
					RUN_QUERY(NO);
					if( !FOUND2() ) {
						strncpy( depto.nom_depto, ST( S_DESCONOCIDO ), sizeof( depto.nom_depto ) );
					}
					importe = 0.01 ;cantidad = 0.01;
					if( importe != 0 || cantidad != 0 ) {
						porc = ( int )( importe / tot_importe * 100 );
						INFORME_10_CUERPO( depto.nom_depto, importe, porc, ( int )cantidad );
					}
				}
			}
		
		}else{//desde el tkt
			IMPRIMIR_TKT_INF10( ini_est );
		}
		
        INFORME_10_PIE( tot_importe, 100, tot_articulos, anterior );
        CLOSE_TABLE( T_DPTO, TT_ORIG );
    }
    BORRAR_MENSAJE();
}
/*****************************************************************************/
void INFORME_10_CABECERA( char anterior )
/*****************************************************************************/
{
    char cad_aux[50];
	memset(cad_aux,0,sizeof(cad_aux));
	IMPRIME_CABECERA_TICKET( SI );
	if( INFORME_X_Y_NO_CONFIGURABLE ) {
		if( IMPRESORA_FISCAL == FISCAL ) {
		
			_snprintf(cad_aux, sizeof(cad_aux)-1, "     %s",ST( S_INFORME_10 ) );
			IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_DOBLE_ANCHO_NEGRITA, cad_aux, SI );
		}else{
			MOSTRAR_CADENA( 15, PRN, ST( S_INFORME_10 ) );
		}
		IMPRIME_NRO_INFORME( anterior );
	}else{
		//por aca si es configurable desde el tkt
		if( IMPRESORA_FISCAL == FISCAL ) {
			if( anterior && NRO_MODO == 'Z' ) {
				//Para imprimir el NRO_Z anterior si es que se encuentra 
				//configurado en el tkt-caja.cfg
					SET_MEMORY_LONG( __nro_z, NRO_Z - 1 );
			}
			IMPRIMIR_TKT_INF10_CABECERA( );
			if( anterior && NRO_MODO == 'Z' ) {
				SET_MEMORY_LONG( __nro_z, NRO_Z + 1 );
			}
		} 

	}
	LF_RECEIPT( 1 );
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void INFORME_10_CUERPO( char *nombre, double importe, int porcentaje, int cantidad )
/*****************************************************************************/
{
    char cad_aux[60];
	char cad_aux1[50];
	char caracter_porc[2];
	caracter_porc[0]='%';
	caracter_porc[1]='\0';
	memset(cad_aux,0,sizeof(cad_aux));
	if( INFORME_X_Y_NO_CONFIGURABLE ) {
		if( IMPRESORA_FISCAL == FISCAL ) {
			_snprintf(cad_aux, sizeof(cad_aux)-1, "%s  %06.02lf", nombre, importe);
				if( porcentaje != -1 ) {
					memset(cad_aux1,0,sizeof(cad_aux1));
					_snprintf(cad_aux1, sizeof(cad_aux1)-1, "%i%/%",porcentaje );
					strncat( cad_aux, &cad_aux1, sizeof( cad_aux ) - strlen( cad_aux ) - 1 );
				}
				memset(cad_aux1,0,sizeof(cad_aux1));
				_snprintf(cad_aux1, sizeof(cad_aux1)-1, " %i",cantidad );
				strncat( cad_aux, &cad_aux1, sizeof( cad_aux ) - strlen( cad_aux ) - 1 );

				IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cad_aux, SI );
		} else {
			MOSTRAR_CADENA( 1, PRN, nombre );
			MOSTRAR_FLOTANTE( 18, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
			if( porcentaje != -1 ) {
				MOSTRAR_ENTERO( 29, PRN, "999", porcentaje );
				MOSTRAR_CADENA( 32, PRN, caracter_porc );
			}
			MOSTRAR_ENTERO( 36, PRN, "9999", cantidad );
			LF( 1 );
		}	
	}

    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void INFORME_10_PIE( double importe, int porcentaje, int cantidad, int anterior )
/*****************************************************************************/
{
    char cad_aux[60];
	char caracter_porc[2];
	caracter_porc[0]='%';
	caracter_porc[1]='\0';

	if( IMPRESORA_FISCAL == FISCAL ) {
		memset(cad_aux,0,sizeof(cad_aux));
		
		//_snprintf(cad_aux, sizeof(cad_aux)-1, "%s %ld", ST( S_CANT__TICKETS_ ), NRO_TICKET - NRO_ULTIMO_TICKET_ANT );
		_snprintf(cad_aux, sizeof(cad_aux)-1, "%s %ld", ST( S_CANT__TICKETS_ ), CANTIDAD_EVENTOS( 0, anterior ));
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cad_aux, SI );

		memset(cad_aux,0,sizeof(cad_aux));
		_snprintf(cad_aux, sizeof(cad_aux)-1, "%s      %.2lf   %i%\%   %i", ST( S_TOTALES ), importe, porcentaje, cantidad );
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cad_aux, SI );
	} else {
		LF( 1 );
		MOSTRAR_CADENA( 1, PRN, ST( S_CANT__TICKETS_ ) );
//		MOSTRAR_ENTERO( 16, PRN, "9999999", NRO_TICKET - NRO_ULTIMO_TICKET_ANT );
		MOSTRAR_ENTERO( 16, PRN, "9999999", CANTIDAD_EVENTOS( 0, anterior ) );
		LF( 1 );
		LF( 1 );
		MOSTRAR_CADENA( 1, PRN, ST( S_TOTALES ) );
		MOSTRAR_FLOTANTE( 18, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
		MOSTRAR_ENTERO( 29, PRN, "999", porcentaje );
		MOSTRAR_CADENA( 32, PRN, caracter_porc );
		MOSTRAR_ENTERO( 36, PRN, "9999", cantidad );
		LF( 1 );
	}
	
	IMPRIME_DIA_OPERATIVO( anterior );
    PRN_FIN_TICKET();
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}

