#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <clave.h> 
#include <cr.h> 
#include <fiscal.h> 
#include <imp_comp.h> 
#include <imp_fond.h> 
#include <inf_dif.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <round.h> 
#include <tkt.h> 
#include <_cr0.h> 
#include <_cr1.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
void IMPRIMIR_NOMBRE_USUARIO( int nro_caj_sup );
void IMPRIMIR_USUARIO_Y_SUPERVISOR( void );
int ES_SUPERVISOR( int cajero );
int LEE_CAJERO( int nro, struct _cajeros *datos_cajero );
/**************************************************************************/
void TICKET_FONDO_FIJO( double importe, int tipo )
/**************************************************************************/
{
    int h, comas = 0;
    char *titulo[] = {" ","CAJERO","SUPERVISOR"};
    double total_ml = 0, total_me = 0;
    char cadaux[20];

	if( IMPRIME_FONDO_FIJO ) {
		if( TKT_CONFIGURABLE ) {
			IMPRIMIR_TKT_FONDO_FIJO();
			setData( ACTION_DOPRINT, NULL, 0 );
		} 
		else 
		{
			if( config.pais == COLOMBIA ) {
				comas = 1;
			}
			if( MODO_NEGOCIO != SERVICIOS ) {
                IMPRIME_CABECERA_COMPROBANTE( ST( S_FONDO_FIJO ), IMPRIME_FONDO_FIJO, _FONDO_FIJO, SI );   
                LF( 1 );
                MOSTRAR_CADENA( ( 40 - strlen( titulo[tipo] ) ) / 2, PRN, titulo[tipo] );
                LF( 1 );
            }
			else {
                PRN_SELECT( SLIP );
                setData( ACTION_BIG_LETTER, NULL, 0 );
                sprintf( cadaux, "%d#%d", 10, 10 );
                setData( ACTION_SET_POSITION, cadaux, 0 );
                memset( cadaux, 0, 20 );
                MOSTRAR_CADENA( 1, PRN, "SWISS MEDICAL GROUP" );
                setData( ACTION_NORMAL_LETTER, NULL, 0 );
                sprintf( cadaux, "%d#%d", 40, 20 );
                setData( ACTION_SET_POSITION, cadaux, 0 );
                memset( cadaux, 0, 20 );
            }
			if( !FONDO_FIJO_MULTIPLES_MEDIOS ) {
                {
                    char aux[15];
                    sprintf( aux, "%s $", ST( S_Importe ) );
                    MOSTRAR_CADENA( 12, PRN, aux );
                }
                MOSTRAR_FLOTANTE( 22, PRN, ENTEROS, DECIMALES, 0, comas, importe );
                LF( 1 );
            }
            else {
                for( h = 0;h < RAM_P_PAGO;h++ ) {
                    if( MODO_NEGOCIO == SERVICIOS ) {
                        sprintf( cadaux, "%d#%d", 10, 30 + h * 3 );
                        setData( ACTION_SET_POSITION, cadaux, 0 );
                        memset( cadaux, 0, 20 );
                    }
                    LF( 1 );
                    MOSTRAR_CADENA( 1, PRN, medios[( int )_PAGO_MODO( h )].nombre );
                    if( MODO_NEGOCIO == SERVICIOS ) {
                        sprintf( cadaux, "%d#%d", 25, 30 + h * 3 );
                        setData( ACTION_SET_POSITION, cadaux, 0 );
                        memset( cadaux, 0, 20 );
                    }
                    if( medios[( int )_PAGO_MODO( h )].ticket_compra
                     || medios[( int )_PAGO_MODO( h )].mutual ) {
                        MOSTRAR_ENTERO( 15, PRN, "0000", _PAGO_CUENTA( h ) );
                    }
                    MOSTRAR_FLOTANTE( 22, PRN, ENTEROS, DECIMALES, 0, comas, _PAGO_IMPORTE( h ) );
                    if( medios[( int )_PAGO_MODO( h )].moneda_extranjera ) {
                        LF( 1 );
                        MOSTRAR_CADENA( 3, PRN, "[" );
                        MOSTRAR_CADENA( 5, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
                        MOSTRAR_FLOTANTE( 9, PRN, ENTEROS, DECIMALES, SI, comas,
                                          _PAGO_IMPORTE_ME( h ) );
                        MOSTRAR_CADENA( 20, PRN, "]" );
                        total_me = ROUND( total_me + _PAGO_IMPORTE_ME( h ), ENTEROS, DECIMALES );
                    }
                    else {
                        total_ml = ROUND( total_ml + _PAGO_IMPORTE( h ), ENTEROS, DECIMALES );
                    }
                }
                if( MODO_NEGOCIO == SERVICIOS ) {
                    sprintf( cadaux, "%d#%d", 10, 30 + ( h + 1 ) * 3 );
                    setData( ACTION_SET_POSITION, cadaux, 0 );
                    memset( cadaux, 0, 20 );
                }
                LF( 2 );
                MOSTRAR_CADENA( 1, PRN, ST( S_TOTAL_FONDO_FIJO_ ) );
                MOSTRAR_FLOTANTE( 28, PRN, ENTEROS, DECIMALES, 0, comas, RAM_IMPORTE );
                LF( 1 );
            }
            if( MODO_NEGOCIO != SERVICIOS ) {
                IMPRIMIR_USUARIO_Y_SUPERVISOR();
            }
            #if defined(INVEL_W) || defined(INVEL_L)
            if( IMPRESORA_FISCAL ) {
                setData( ACTION_PRINT, NULL, 0 );
            }
            #endif
            if( MODO_NEGOCIO != SERVICIOS ) {
                IMPRIME_PIE_COMPROBANTE( IMPRIME_FONDO_FIJO, RAM_IMPORTE );
            }
			#if defined(INVEL_W) || defined(INVEL_L)
			setData( ACTION_PRINT, NULL, 0 );
			if( MODO_NEGOCIO == SERVICIOS ) {
				setData( ACTION_DOPRINT, NULL, 0 );
			}
			#endif
		}
    } 
}
/**************************************************************************/
void IMPRIMIR_USUARIO_Y_SUPERVISOR( void )
/**************************************************************************/
{
    struct _cajeros sup_cajero;
    int rta;
    LEE_CAJERO( USUARIO_CLAVE_ANTERIOR(), &sup_cajero );
    if( IDENTIFICACION_USUARIO == ID_X_TIPO ) {
        rta = ( sup_cajero.tipo == SUPERVISOR ) ? SI : NO;
    }
    else {
        rta = ( USUARIO_CLAVE_ANTERIOR() < config.primer_supervisor ) ? NO : SI;
    }
    if( rta ) {
        IMPRIMIR_NOMBRE_USUARIO( USUARIO_CLAVE_ANTERIOR() );
    }
    rta = 1;
    LEE_CAJERO( NRO_CAJERO, &sup_cajero );
    if( IDENTIFICACION_USUARIO == ID_X_TIPO ) {
        rta = ( sup_cajero.tipo == SUPERVISOR ) ? SI : NO;
    }
    else {
        rta = ( NRO_CAJERO < config.primer_supervisor ) ? NO : SI;
    }
    if( !rta ) {
        IMPRIMIR_NOMBRE_USUARIO( NRO_CAJERO );
    }
    LF( 1 );
}
/**************************************************************************/
void IMPRIMIR_NOMBRE_USUARIO( int nro_caj_sup )
/**************************************************************************/
{
    struct _cajeros sup_cajero;
    int slip = SLIP_HABILITADO();
	int pos_x = 0;
	char cad_aux[50];
	char cad_aux1[100];

	memset(cad_aux1,0,sizeof(cad_aux1));
    if( slip ) {
        return;
    }
    LEE_CAJERO( nro_caj_sup, &sup_cajero );
    if( sup_cajero.tipo == 1 && ( NRO_MODO == 'Z' || ( NRO_MODO == 'Y' && IMPRIMIR_Y_EN_Z ) ) ) {
        return;
    }
    /*    if(IMPRESORA_FISCAL){
             ABRIR_DOCUMENTO_NO_FISCAL( 0 );
        }
    */
	if( sup_cajero.tipo == 1 ) {
		if( IMPRESORA_FISCAL == FISCAL ) {
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, "%s",ST( S_CAJERO_ ) );

		} else {
			MOSTRAR_CADENA( 1, PRN, ST( S_CAJERO_ ) );
		    pos_x = 9;
		}
    }
	else {
		if( IMPRESORA_FISCAL == FISCAL ) {
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, "%s",ST( S_SUPERVISOR_ ) );
		} else {
			MOSTRAR_CADENA( 1, PRN, ST( S_SUPERVISOR_ ) );
		    pos_x = 13;
		}
    }
	if( IMPRIMIR_LEGAJO_CAJERO ) {
		if( IMPRESORA_FISCAL == FISCAL ) {
			strncat( cad_aux1, cad_aux, sizeof( cad_aux1 ) - strlen( cad_aux1 ) - 1 );
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, " %ld",sup_cajero.nro_legajo );
		} else {
            MOSTRAR_ENTERO( pos_x, PRN, "00000000", sup_cajero.nro_legajo );
		}
    }
    else {
		if( IMPRESORA_FISCAL == FISCAL ) {
			strncat( cad_aux1, cad_aux, sizeof( cad_aux1 ) - strlen( cad_aux1 ) - 1 );
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, " %i", nro_caj_sup );
		} else {
            MOSTRAR_ENTERO( pos_x, PRN, "0000000", nro_caj_sup );
		}
    }
	if( IMPRESORA_FISCAL == FISCAL ) {
		strncat( cad_aux1, cad_aux, sizeof( cad_aux1 ) - strlen( cad_aux1 ) - 1 );
		memset(cad_aux,0,sizeof(cad_aux));
		_snprintf(cad_aux, sizeof(cad_aux)-1, "   %s", sup_cajero.nombre );
		strncat( cad_aux1, cad_aux, sizeof( cad_aux1 ) - strlen( cad_aux1 ) - 1 );
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cad_aux1, SI );
	} else {
        MOSTRAR_CADENA( pos_x + 9, PRN, sup_cajero.nombre );
	}	
    LF( 1 );

    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}

