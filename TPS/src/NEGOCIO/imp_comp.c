#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h> 
#include <fiscal.h> 
#include <imp_comp.h> 
#include <imp_tick.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
extern int _NRO_COMANDO_FONDO_FIJO;
/**************************************************************************/
void IMPRIME_COMPROBANTE( char *titulo, double importe, int modo, int comp, int comp_no_fiscal )
/**************************************************************************/
{
    char cad_aux[50];
	memset(cad_aux, 0, sizeof(cad_aux));
    if( modo ) {
        IMPRIME_CABECERA_COMPROBANTE( titulo, modo, comp, comp_no_fiscal );
        {
            char aux[15];
            sprintf( aux, "%s $", ST( S_Importe ) );
            MOSTRAR_CADENA( 12, PRN, aux );
        }
       if(IMPRESORA_FISCAL == FISCAL)
		{
			_snprintf(cad_aux, sizeof(cad_aux)-1, " ");
			IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_DOBLE_ANCHO_NEGRITA,cad_aux, SI );
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, "     %s",ST( S_ADELANTOS ));
			IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_DOBLE_ANCHO_NEGRITA,cad_aux, SI );
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, "          Importe: $ %.2f",importe);
			IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_NORMAL,cad_aux, SI);
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, " ");
			IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_DOBLE_ANCHO_NEGRITA,cad_aux, SI );
			CERRAR_DOCUMENTO_NO_FISCAL();
		}
		else
		{
	        MOSTRAR_FLOTANTE( 22, PRN, ENTEROS, DECIMALES, 0, 0, importe );
	        IMPRIME_PIE_COMPROBANTE( modo, importe );
	    }
	}
}
/**************************************************************************/
void IMPRIME_CABECERA_COMPROBANTE( char *titulo, int modo, int comp, int comp_no_fiscal )
/**************************************************************************/
{
    int x;
    if( modo ) {
        x = ( 40 - strlen( titulo ) * 2 ) / 2 + 1;
        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
            if( comp == _FONDO_FIJO ) {
                ADD_MEMORY_LONG( __nro_correlativo_fondo_fijo, 1L );
                ABRIR_DOCUMENTO( _NRO_COMANDO_FONDO_FIJO, NRO_CORRELATIVO_FONDO_FIJO );
            }
        }

        switch( modo ) {
            case 1:
                PRN_CONTINUA();
                IMPRIME_CABECERA_TICKET( comp_no_fiscal );
                LF( 1 );
                break;
            case 2:
                // PRN_SELECT( JOURNAL );
                if( IMPRIMIR_EN_JOURNAL ) {
                    PRN_INHABILITA_AVANCE_RECEIPT();
                    IMPRIME_CABECERA_GENERAL( NRO_EVENTO );
                }
                break;
            case 3:
                IMPRIME_CABECERA_GENERAL( NRO_EVENTO );
                break;
        }
        #if defined(INVEL_W) || defined(INVEL_L)
        setData( ACTION_PRINT, NULL, 0 );
        #endif
        PRN_LETRA_GRANDE();
        #if defined(INVEL_W) || defined(INVEL_L)
        //setData( ACTION_PRINT_CENTER, NULL, 0 );
		MOSTRAR_CADENA( 1, PRN, "      ");
        MOSTRAR_CADENA( 1, PRN, titulo );
        LF( 0 );
        //setData( ACTION_PRINT_NORMAL, NULL, 0 );
        #else
        MOSTRAR_CADENA( x, PRN, titulo );
        #endif
        PRN_LETRA_NORMAL();
        LF( 1 );
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/**************************************************************************/
void IMPRIME_PIE_COMPROBANTE( int modo, double total )
/**************************************************************************/
{
    switch( modo ) {
        case 1:
            PRN_FIN_TICKET();
            break;
        case 2:
            LF( 1 );
            PRN_INHABILITA_AVANCE_JOURNAL();
            break;
        case 3:
            if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
                EJECT();
            }
            else {
                PRN_INHABILITA_SLIP();
            }
            break;
		case 6: // Cobros
			//CEREAR_PIE_FISCAL( );
			PRN_FIN_TICKET();
			break;
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    //setData( ACTION_PRINT, NULL, 0 );
    #endif
}

