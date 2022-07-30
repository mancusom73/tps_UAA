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
#include <dbrouter.h> 
#include <fiscal.h> 
#include <ftoc.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <imp_fond.h> 
#include <imp_ret.h> 
#include <imp_tick.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <actions.h>
#include <math.h>
#include <assert.h>
#include <pago_mut.h>
#include <pago_tc.h>
#include <lista.h>
#include <pagos.h>
#include <retiros.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
extern int _NRO_COMANDO_RETIRO,_NRO_COMANDO_FONDO_FIJO;
double importe_retiro_ff;
/**************************************************************************/
void IMPRIME_CABECERA_RETIROS( unsigned tipo, int retiro )
/**************************************************************************/
{
    int prnt_rta = 0;
	char cadena[30];
	char *titulo[] = {ST( S_ESPACIO ),ST( S_CIERRE_DE_CAJA ),ST( S_CAJERO__ ),ST( S_SUPERVISOR__ )};
    int seguir = 1;
	char caracter_sig[2];
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

	if( IMPRIME_RETIROS ) {
		if( MODO_NEGOCIO == SERVICIOS ) {
            char cadaux[100];
            setData( ACTION_BIG_LETTER, NULL, 0 );
            PRN_SELECT( SLIP );
            setData( ACTION_SET_POSITION, "10#20", 0 );
            MOSTRAR_CADENA( 1, PRN, "CAJERO:    " );
			memset(cadena, 0, sizeof(cadena));
            if( IMPRIMIR_LEGAJO_CAJERO ) {
				_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
				MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
            }
            else {
				_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
				MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
            }
            MOSTRAR_CADENA( 1, PRN, caracter_sig );
            MOSTRAR_CADENA( 1, PRN, datos_cajero_actual.nombre );
            setData( ACTION_SET_POSITION, "150#20", 0 );
            MOSTRAR_CADENA( 1, PRN, "CAJA: " );
            //MOSTRAR_ENTERO( 1, PRN, "0000", NRO_CAJA );
			MOSTRAR_ENTERO( 1, PRN, ( USAR_NRO_CAJA_DGI )?
					"0000":"000", NUMERO_CAJA( ) );

            sprintf( cadaux, "---------------------------- %s ------------------------------",
                     ( retiro ) ? ST( S_RETIROS ) : ST( S_CAMBIO ) );
            setData( ACTION_SET_POSITION, "10#30", 0 );
            MOSTRAR_CADENA( 12, PRN, cadaux );
        }
        else {
			int pos;
            if( tipo > 3 ) {
                tipo = 0;
            }
            if( IMPRIME_RETIROS == _ESTACION_JOURNAL ) {
                PRN_SELECT( JOURNAL );
            }
            else if( IMPRIME_RETIROS == _ESTACION_JOURNAL_RECEIPT ) {
                PRN_CONTINUA();
                PRN_HABILITA_AVANCES();
            }
            else if( IMPRIME_RETIROS == _ESTACION_SLIP ) {
                #if defined(INVEL_W) || defined(INVEL_L)
                if( tipo == 2 ) {
                    while( !prnt_rta ) {
                        getData( ACTION_SLP_EMPTY, ( char* )&prnt_rta, 0 );
                    }
                }
                getData( ACTION_SLP_EMPTY, ( char* )&prnt_rta, 0 );
                while( prnt_rta ) {
                    MENSAJE_TECLA_STRING( S_INGRESE_COMPROBANTE_RETIRO_ENTER );                
                    getData( ACTION_SLP_EMPTY, ( char* )&prnt_rta, 0 );
                }
                PRN_HABILITA_SLIP( 1, NO );
                LF( 0 );
                setData( ACTION_PRINT, NULL, 0 );
                #else
                MENSAJE_TECLA_STRING( S_INGRESE_COMPROBANTE_RETIRO_ENTER );                
                #endif
            }
            /*#if defined(INVEL_W) || defined(INVEL_L)
                        if( IMPRESORA_FISCAL )
                           ABRIR_DOCUMENTO_NO_FISCAL( 0 );
            #endif*/
			//alejo
		    pos = 1;
/*			MOSTRAR_CADENA( pos, PRN, "CAJERO:" );
			pos += 7;
			memset( cadena, 0, sizeof( cadena ) );
			sprintf( cadena, "-%s", datos_cajero_actual.nombre );
			
			if( IMPRIMIR_LEGAJO_CAJERO ) {
				MOSTRAR_ENTERO( pos, PRN, "00000000", NRO_LEGAJO_CAJERO ); //8
				pos += 8;
				cadena[15] = 0;
				MOSTRAR_CADENA( pos, PRN, cadena );
				pos += 15;
            } 
			else {
				MOSTRAR_ENTERO( 1, PRN, "0000", NRO_CAJERO ); //4
				pos += 4;
				cadena[19] = 0;
				MOSTRAR_CADENA( pos, PRN, cadena );
				pos += 19;
            }		
			MOSTRAR_CADENA( pos, PRN, "CAJA: " );
			pos += 5;
            MOSTRAR_ENTERO( pos, PRN, "0000", NRO_CAJA );
*/
            PRN_LETRA_GRANDE(); //alejo 856
            MOSTRAR_CADENA( 7, PRN, ( retiro ) ? ST( S_RETIROS ) : ST( S_CAMBIO ) );
            PRN_LETRA_NORMAL();
            LF( 1 );
            if( MEXICO != config.pais ) {
                MOSTRAR_CADENA( ( 40 - strlen( titulo[tipo] ) ) / 2, PRN, titulo[tipo] );
                LF( 1 );
            }
            if( IMPRIME_RETIROS != _ESTACION_JOURNAL ) {
                LF_RECEIPT( 1 );
            }
            IMPRIME_CABECERA_GENERAL( NRO_EVENTO );
            if( IMPRIME_RETIROS != _ESTACION_JOURNAL && MODO_NEGOCIO != RETAIL ) {
                LF_RECEIPT( 1 );
            }
            /*if( PEDIR_SUPERVISOR_PARA_RETIROS ) {
               IMPRIMIR_NOMBRE_USUARIO( USUARIO_CLAVE_ANTERIOR(  ) );
            }
            IMPRIMIR_NOMBRE_USUARIO( NRO_CAJERO );
            */
            if( config.pais == ELSALVADOR && tipo != 1 && retiro ) {
                MOSTRAR_CADENA( 1, PRN, "Nro. Correlativo:" );
                MOSTRAR_ENTERO( 19, PRN, "0000", NRO_NRO_RETIRO - 1 );//???
                LF( 2 );
            }
        }
        #if defined(INVEL_W) || defined(INVEL_L)
        LF( 0 );
        setData( ACTION_PRINT, NULL, 0 );
        #endif
    }
}
/*****************************************************************************/
void IMPRIME_CUERPO_RETIROS( int anulacion, char *nombre, double importe, long numero, int cantidad,
                             double importe_me, int medio )
/*****************************************************************************/
{
    static int imprimir = 0;
    char strImporte[50], largo[50], nombre_tmp[21];
    int cod = 0, sub_m, cant_sub_m = 0; 
    double cotizacion, retirado_tmp;
	char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

    if( IMPRIME_RETIROS ) {
        if( IMPRESORA_FISCAL != FISCAL_IBM_4610
         || ( IMPRESORA_FISCAL == FISCAL_IBM_4610 && _NRO_COMANDO_RETIRO ) ) {
            if( anulacion ) {
                MOSTRAR_CADENA( 1, PRN, caracter_sig );
            }
            assert( strlen( nombre ) < 20 );
            strcpy( largo, nombre );
            strncat( largo, "                    ", 20 - strlen( nombre ) );
            MOSTRAR_CADENA( 1, PRN, largo );
            if( cantidad && !importe_me ) {
                MOSTRAR_FLOTANTE( 8, PRN, ENTEROS, DECIMALES, 0, 0, importe / cantidad );
                MOSTRAR_CADENA( 16, PRN, caracter_por );
                MOSTRAR_ENTERO( 18, PRN, "999", cantidad );
                if( MODO_NEGOCIO == SERVICIOS ) {
                    FTOC( importe, strImporte, ENTEROS + 1 , DECIMALES, SI, SI );
                    MOSTRAR_CADENA_ALD( 1, PRN, strImporte, 13 );
                }
                else {
                    MOSTRAR_FLOTANTE( 28, PRN, ENTEROS, DECIMALES, SI, 0, importe );
                }
            }
            else {
                if( MODO_NEGOCIO == SERVICIOS ) {
                    FTOC( importe, strImporte, ENTEROS + 1, DECIMALES, SI, SI );
                    MOSTRAR_CADENA_ALD( 1, PRN, strImporte, 14 );
                }
                else {
                    MOSTRAR_FLOTANTE( 12, PRN, ENTEROS + 1, DECIMALES, SI, 0, importe );
                }
                if( numero ) {
                    MOSTRAR_ENTERO( 22, PRN, "00000000", numero );
                }
            }
            if( cantidad && importe_me ) {
                LF( 1 );
                MOSTRAR_CADENA( 3, PRN, "[" );
                MOSTRAR_CADENA( 5, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
                MOSTRAR_FLOTANTE( 9, PRN, ENTEROS, DECIMALES, 0, 0, importe_me / cantidad );
                MOSTRAR_CADENA( 19, PRN, caracter_por );
                MOSTRAR_ENTERO( 21, PRN, "999", cantidad );
                MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, SI, 0, importe_me );
                MOSTRAR_CADENA( 33, PRN, "]" );
            }
            else if( !cantidad && medio > 0 && medios[medio].modo_arqueo != M_ACUMULADO ) {
                if( medios[medio].mutual || medios[medio].ticket_compra || medios[medio].tarjeta ) {
                    cant_sub_m = OBTIENE_CANT_SUB_RETIRO();
                    for( sub_m = 1; sub_m < cant_sub_m; sub_m++ ) {
                        retirado_tmp = OBTIENE_RETIRADO( medio, sub_m );
                        if( retirado_tmp != 0 ) {
                            if( medios[medio].mutual ) {
                                NOMBRE_MUTUAL( sub_m, nombre_tmp );
                            }
                            else if( medios[medio].ticket_compra ) {
                                NOMBRE_TICKET_COMPRA( sub_m, nombre_tmp );
                            }
                            else if( medios[medio].tarjeta ) {
                                strcpy( nombre_tmp, NOMBRE_TARJETA( sub_m - 1 ) );
                            }
                            LF(1);
                            MOSTRAR_CADENA( 3, PRN, nombre_tmp );
                            if( medios[medio].mutual && MUTUAL_MONEDA_EXTRANJERA( sub_m ) ) {
                                COTIZACION_MUTUAL( sub_m, &cotizacion, NULL );
                                MOSTRAR_FLOTANTE( 17, PRN, ENTEROS, DECIMALES, NO, NO, cotizacion );
                                MOSTRAR_CADENA( 24, PRN, caracter_por );
                                MOSTRAR_FLOTANTE( 26, PRN, ENTEROS, DECIMALES, NO, NO, retirado_tmp / cotizacion );
                            }
                            else {
                                MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, NO, NO, retirado_tmp  );
                            }
                        }
                    }
                }
                //MOSTRAR_CADENA( 17, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
                //MOSTRAR_FLOTANTE( 9, PRN, ENTEROS, DECIMALES, SI, 0, importe_me );
                //MOSTRAR_CADENA( 20, PRN, "]" );
            }
            LF( 1 );
        }
        #if defined(INVEL_W) || defined(INVEL_L)
        if( imprimir >= 10 ) {
            setData( ACTION_PRINT, NULL, 0 );
            imprimir = 0;
        }
        else {
            imprimir++;
        }
        #endif
    }
}
/*****************************************************************************/
void IMPRIME_PIE_RETIROS( double *acu_retirado, double retiro_fondo_fijo, int retiro,
                          double *acu_retirado_me, int en_cierre )
/*****************************************************************************/
{
    double total = 0;
    int lf = 6;
    char cadaux[100];
    int hora,fecha;
    int index = 1;
	char cadena[30];
    hora = GET_HORA_DOS();
    fecha = GET_FECHA_DOS();

	if( TKT_CONFIGURABLE && NRO_MODO != 'Y' )
	{
        if( retiro_fondo_fijo > 0.0 ) {
		    importe_retiro_ff = retiro_fondo_fijo;
        }
		IMPRIME_TKT_RETIRO( NO );
	}
	else 
	{
	    if( acu_retirado != NULL ) {
	        int h;
	        char *titulo = "---------- DETALLE DE RETIROS ---------";
	        char *titulo2 = "----------- TOTAL DE CAMBIOS ----------";
	        if( MODO_NEGOCIO != SERVICIOS ) {
	            MOSTRAR_CADENA( 1, PRN, ( retiro ) ? titulo : titulo2 );
	            LF( 1 );                
	            MOSTRAR_CADENA( 1, PRN, "   MEDIOS               IMPORTES      " );
	            LF( 1 );
	        }
	        if( retiro_fondo_fijo ) {
	            if( MODO_NEGOCIO == SERVICIOS ) {
	                memset( cadaux, 0, 100 );
	                sprintf( cadaux, "10#35" );
	                setData( ACTION_SET_POSITION, cadaux, strlen( cadaux ) );
	                IMPRIME_CUERPO_RETIROS( NO, ST( S_FONDO_FIJO ), retiro_fondo_fijo, 0, 0, 0.0, 0 );
	            }
				else {
	                IMPRIME_CUERPO_RETIROS( NO, ST( S_FONDO_FIJO ), retiro_fondo_fijo, 0, 0, 0.0, 0 );
	            }
	            total += retiro_fondo_fijo;
			}        
	        for( h = 0;h < 10;h++ ) {
	            if( medios[h].nro && ( fabs( acu_retirado[h] ) > 0.01 ) ) {
	                IMPRIME_CUERPO_RETIROS( NO, medios[h].nombre, acu_retirado[h], 0, 0,
	                                        ( acu_retirado_me ) ? acu_retirado_me[h] : 0.0, h );
	                total += acu_retirado[h];
	            }
	        }
	        if( retiro ) {
	            if( MODO_NEGOCIO == SERVICIOS ) {
	                sprintf( cadaux, "10#%d", 45 + 4 * index );
	                setData( ACTION_SET_POSITION, cadaux, strlen( cadaux ) );
	            }
	            LF( 1 );
	            IMPRIME_CUERPO_RETIROS( NO, ST( S_TOTAL_RETIROS ), total, 0, 0, 0, 0 );
	            LF( 1 );
	            if( MODO_NEGOCIO == SERVICIOS ) {
	                setData( ACTION_SET_POSITION, "10#75", 0 );
	                MOSTRAR_CADENA(
	                1, PRN, "-------------------------------------------------------------------" );
	                setData( ACTION_SET_POSITION, "45#83", 0 );
	                MOSTRAR_FECHA_LETRAS( 1, PRN, fecha );
	                setData( ACTION_SET_POSITION, "103#83", 0 );
	                MOSTRAR_HORA( 1, PRN, hora );
	                setData( ACTION_SET_POSITION, "117#83", 0 );
	                MOSTRAR_CADENA( 1, PRN, "Hs." );
				} else {//alejo probar como sale...
					/*LF(1);
	                MOSTRAR_FECHA( 1, PRN, fecha );                
	                MOSTRAR_HORA( 11, PRN, hora );       
	                MOSTRAR_CADENA( 16, PRN, "Hs." );*/
	                MOSTRAR_CADENA( 1, PRN, "CAJERO: ");
					memset(cadena, 0, sizeof(cadena));
					if( IMPRIMIR_LEGAJO_CAJERO ) {
						_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
						MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
					}
					else {
						_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
						MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
					}
	                MOSTRAR_CADENA( 19, PRN, datos_cajero_actual.nombre );
	                LF(1);
	                MOSTRAR_FECHA( 1, PRN, fecha );
	                MOSTRAR_HORA( 10, PRN, hora );
	                MOSTRAR_CADENA( 16, PRN, "SUC.");
	                MOSTRAR_ENTERO( 20, PRN, "000", config.sucursal );
	                MOSTRAR_CADENA( 24, PRN, "TER.");
                    //MOSTRAR_ENTERO( 28, PRN, "000", NRO_CAJA );
				    MOSTRAR_ENTERO( 28, PRN, ( USAR_NRO_CAJA_DGI )?
					    "0000":"000", NUMERO_CAJA( ) );
	                MOSTRAR_CADENA( 32, PRN, "TRA.");
	                MOSTRAR_ENTERO( 36, PRN, "000", NRO_EVENTO - 1);
				}
	        }        
	    }
	    if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
	        if( IMPRIME_RETIROS == _ESTACION_SLIP ) {
	            //  PRN_INHABILITA_SLIP(  );
	        }
	        else if( IMPRIME_RETIROS ) {
	            PRN_FIN_TICKET();
	        }
	    }
	    else {
	        if( IMPRIME_RETIROS && _NRO_COMANDO_RETIRO ) {
	            if( !en_cierre ) {
	                CERRAR_DOCUMENTO( total, SI, lf );
	                if( IMPRIME_RETIROS == 3 ) {
	                    lf = 2;
	                    // PRN_INHABILITA_SLIP(  );
	                }
	            }
	            else {
	                TKT_CORTE_PAPEL();
	            }
	        }
	    }
	    setData( ACTION_PRINT, NULL, 0 );
	}
}
