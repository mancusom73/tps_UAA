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
#include <cierre.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <dbrouter.h> 
#include <fiscal.h> 
#include <ftoc.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <imp_item.h> 
#include <imp_tick.h> 
#include <log.h> 
#include <malloc.h> 
#include <m_cadena.h> 
#include <driver.h> 
#include <pagos.h> 
#include <pant.h> 
#include <print.h> 
#include <promo.h> 
#include <stack.h> 
#include <tkt.h> 
#include <trim.h> 
#include <valida.h> 
#include <_cr0.h> 
#include <_cr1.h> 
#include <remito.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#include <sys/types.h>
#include <unistd.h>
#include <sys/io.h>
#else
#include <io.h>
#endif
#include <config.h>
#include <sys/stat.h>
#if defined(INVEL_W) || defined(INVEL_L)
#include "actions.h"
#endif
#define _NRO_TICKET_EN_PIE            0
#define _NRO_TICKET_DESPUES_DE_TOTAL  1
static char *cabecera, pie[85] = {"         GRACIAS POR SU COMPRA\r\x1A"};
void _IMPRIME_VUELTO( char *nombre, double importe );
void IMPRIME_NRO_TICKET();
void IMPRIME_CABECERA_GENERAL( long nro_evento );
void IMPRIME_CAJERO();
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
extern void CR1_INCOSISTENCIA_FISCAL( int sumar, int origen, int t_espera, int cerear );
extern void ANULA_TICKET_FACTURA_FISCAL( char *desc );
extern void IMPRIME_TKT_INGRESO_EGRESO_CAJERO();
extern int LEE_CAJERO( int nro, struct _cajeros *datos_cajero );
extern int USUARIO_CLAVE_ANTERIOR(void);
extern void IMPRIMIR_NOMBRE_USUARIO( int nro_caj_sup );
extern int CONTROLA_LOS_ENVIOS_Y_REINTENTA(int action, char *buffer, int len, int lim_reintentos , int preg_status);
void MENSAJE( char *cadena );
extern float porc_retenc_dgr;
extern void setHabPing(int hab);
//#define COMPILAR_EMULADOR_FISCAL
/**************************************************************************/
void LEER_DATOS_TICKET()
/**************************************************************************/
{
    int handle;
    int cant;
    /*----------- Lee el archivo de cabecera de ticket -------------*/
    cabecera = malloc( 443 );
    if( cabecera ) {
        handle = OPEN( _CAB_DAT, O_RDONLY );
        if( handle ) {
            #ifdef INVEL_L
            cant = read( handle, cabecera, 443 );
            #else
            cant = _read( handle, cabecera, 443 );
            #endif
            cabecera[cant] = 0;
        }
        close( handle );
        /*----------- Lee el archivo de pie de ticket -------------*/
        handle = OPEN( _PIE_DAT, O_RDONLY );
        if( handle ) {
            #ifdef INVEL_L
            read( handle, pie, 85 );
            #else
            _read( handle, pie, 85 );
            #endif
        }
        close( handle );
    }
}
/**************************************************************************/
int IMPRIME_CABECERA_TICKET( int doc_no_fiscal )
/**************************************************************************/
{
	char modo;
	int rta = 1;
	modo = RAM_MODO_EJECUCION;
	if( IMPRIMIR_TICKET ) {
        if( RAM_IMPRIMIR_CABECERA || !IMPRIMIR_CABECERA_AL_FINAL ) {
			_IMPRIME_CABECERA_TICKET( 0, 0, NO );
        }
        else {
            PRN_HABILITA_AVANCES();
        }
        switch( IMPRESORA_FISCAL ) {
			case FISCAL: 
				rta = COMANDO_LEER_CONTADORES_PRINTER_FISCAL( 1, SI, NULL );
				if(RAM_TICKET_FACTURA && !RAM_NOTA_CR && !doc_no_fiscal )
					IMPRIME_CABECERA_TIQUE_FACTURA();
				else if(RAM_NOTA_CR && !RAM_TICKET_FACTURA && !doc_no_fiscal)
					IMPRIME_CABECERA_NOTA_DE_CREDITO();
				else if(!RAM_TICKET_FACTURA && !RAM_NOTA_CR && !doc_no_fiscal && RAM_MODO_EJECUCION != INVENTARIO  )
					rta =ABRIR_DOCUMENTO_FISCAL();
				else if( RAM_MODO_EJECUCION == INVENTARIO || doc_no_fiscal )
				{
					ABRIR_DOCUMENTO_NO_FISCAL('1');
					IMPRIME_CABECERA_GENERAL( NRO_EVENTO );
					if( RAM_MODO_EJECUCION == INVENTARIO ) {
						IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_DOBLE_ANCHO_ALTO_NEGRITA,ST( S__MODO_INVENTARIO_ ), SI );
					}
				}
				break;
            default:
                if( !TKT_CONFIGURABLE && !IMPRESORA_FISCAL ) {
                    IMPRIME_CABECERA_GENERAL( NRO_EVENTO );
                    LF_RECEIPT( 1 );
                }
                break;
        }
    }
	return rta;
}
/**************************************************************************/
void _IMPRIME_CABECERA_TICKET( int lineas_corte, int lineas, int corte_activado )
/**************************************************************************/
{
    int corte_receipt,h,normal;
    corte_receipt = ( IMPRIMIR_CABECERA_AL_FINAL && !RAM_IMPRIMIR_CABECERA ) ? NO : SI;

    if( TKT_CONFIGURABLE ) {
        IMPRIMIR_TKT_CABECERA( 1 );
	}
	else {
        if( IMPRIMIR_CABECERA_AL_FINAL ) {
            lineas += 2;
            LF( 2 );
        }
        PRN_SELECT( RECEIPT );
        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 && RAM_MODO_EJECUCION == INVENTARIO ) {
            if( _NRO_COMANDO_INVENTARIO > 3 && _NRO_COMANDO_INVENTARIO < 30
             && _NRO_COMANDO_INVENTARIO != 0 ) {
                ADD_MEMORY_LONG( __nro_correlativo_inventario, 1L );
                ABRIR_DOCUMENTO( _NRO_COMANDO_INVENTARIO, NRO_CORRELATIVO_INVENTARIO );
            }
            else {
                _NRO_COMANDO_INVENTARIO = -1;
            }
        }
        PRN_LETRA_GRANDE();
        if( RAM_MODO_EJECUCION == INVENTARIO ) {
            MOSTRAR_CADENA( 11, PRN, ST( S_INVENTARIO ) );
            PRN_LETRA_NORMAL();
            LF( 1 );
            lineas++;
        }
        else {
            h = 0;
            normal = 0;
            while( cabecera[h] != 26 && cabecera[h] ) {
                SEND_BYTE( cabecera[h] );
                if( cabecera[h] == 10 && !normal ) {
                    PRN_LETRA_NORMAL();
                    normal = 1;
                }
                if( cabecera[h] == 10 ) {
                    lineas++;
                    if( !corte_receipt && corte_activado ) {
                        corte_receipt = VERIFICA_CORTE_FIN_TICKET( lineas, lineas_corte );
                    }
                }
                h++;
            }
        }
        PRN_LETRA_NORMAL();
        if( !corte_receipt && corte_activado ) {
            LF( lineas_corte - lineas );
            VERIFICA_CORTE_FIN_TICKET( 999, 0 );
        }
    }
    SET_MEMORY_CHAR( __ram_imprimir_cabecera, ( char )0 );
}
/*****************************************************************************/
void IMPRIME_TITULO_TICKET()
/*****************************************************************************/
{
	if( IMPRIMIR_TICKET ) {
        if( DATOS_CAJERO_EN_TICKET ) {
            MOSTRAR_CADENA( 1, PRN, datos_cajero_actual.nombre );
            MOSTRAR_CADENA( 25, PRN, datos_cajero_actual.documento );
            LF( 1 );
        }
        if( DATOS_VENDEDOR_EN_TICKET && NRO_VENDEDOR_LONG && FORMATO_TICKET != 7 ) {
            if( DATOS_VENDEDOR_EN_TICKET == 2 ) {
                MOSTRAR_CADENA( 1, PRN, ST( S_NRO_SECTOR____ ) );
            }
            else {
                MOSTRAR_CADENA( 1, PRN, ST( S_NRO_VENDEDOR__ ) );
            }
            MOSTRAR_ENTERO( 16, PRN, "99999", NRO_VENDEDOR_LONG );
            LF( 1 );
        }
        if( CLIENTE_OBLIGATORIO ) {
            MOSTRAR_CLIENTE( 1, PRN, RAM_NRO_CLIENTE );
            MOSTRAR_CADENA( 10, PRN, CLIENTE_AFINIDAD_NOMBRE );
            LF( 1 );
        }
        if( FORMATO_TICKET != 7 && !TKT_CONFIGURABLE ) {
            LF( 1 );
            MOSTRAR_CADENA( 1, PRN, ST( S_______________DESCRIPCION____________ ) );
            LF( 2 );
        }
        if( TKT_CONFIGURABLE ) {
            IMPRIMIR_TKT_TITULO_TICKET();
        }
        #if defined(INVEL_W) || defined(INVEL_L)
        setData( ACTION_PRINT, NULL, 0 );
        #endif
    }
}
/*****************************************************************************/
void IMPRIME_INTERESES( double importe, double porc )
/*****************************************************************************/
{
    char caracter_porc[2];
	caracter_porc[0]='%';
	caracter_porc[1]='\0';

	if( IMPRIMIR_TICKET ) {
		if( !TKT_CONFIGURABLE ) {
			MOSTRAR_FLOTANTE( 1, PRN, 3, 2, 1, 0, porc );
			MOSTRAR_CADENA( 8, PRN, caracter_porc );
			MOSTRAR_CADENA( 10, PRN, ST( S_RECARGO ) );
			MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, importe );
			LF( 1 );
			PRN_SELECT( JOURNAL );
			MOSTRAR_FLOTANTE( 1, PRN, 3, 2, 1, 0, porc );
			MOSTRAR_CADENA( 8, PRN, caracter_porc );
			MOSTRAR_CADENA( 10, PRN, ST( S_RECARGO ) );
			MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, importe );
			LF( 1 );
			PRN_SELECT( RECEIPT );
		}
    }
}
/*****************************************************************************/
void IMPRIME_SUBTOTAL( double importe, int incondicional )
/*****************************************************************************/
{
    char c_imp[25];
	char imp[25];
	if( IMPRIMIR_TICKET ) {
		if( DEBE_IMPRIMIR_ITEM() || incondicional ) {
			if( IMPRESORA_FISCAL == FISCAL ) {
				int decimales,signo;
				
				if ( RAM_NOTA_CR || RAM_TICKET_FACTURA ) 
				{
					//sprintf( imp, "%s&%i& .", total,SI );
					sprintf( imp, "%s&%i", fcvt( importe, DECIMALES, &decimales, &signo ),SI );
					setData( ACTION_PRINT_REC_SUBTOTAL_FAC, imp, 0 );//Seba
				} 
				else
				{
					#if defined(INVEL_W) || defined(INVEL_L)
					sprintf( imp, "%s&%i", fcvt( importe, DECIMALES, &decimales, &signo ),SI );
					setData( ACTION_PRINT_REC_SUBTOTAL, imp, 0 );
					#endif
				}
			}
			else 
			{
				if( TKT_CONFIGURABLE ) {
					SET_MEMORY_DOUBLE( __ram_subtotal, importe );
					IMPRIMIR_TKT_SUBTOTAL();
				}
				else {
                    #if defined(INVEL_W) || defined(INVEL_L)
                    PRN_LETRA_GRANDE();
                    MOSTRAR_CADENA( 1, PRN, ST( S_SUBTOTAL___ ) );
                    #else
                    MOSTRAR_CADENA( 1, PRN, ST( S_SUBTOTAL___ ) );
                    #endif
                    switch( FORMATO_TICKET ) {
                        case 8:
                            MOSTRAR_FLOTANTE( 33, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
                            break;
                        default:
                            FTOC( importe, c_imp, ENTEROS, DECIMALES, 0,
                                  ( config.pais == COLOMBIA ) ? 1 : 0 );
                            #if defined(INVEL_W) || defined(INVEL_L)
                            {
                                char aux[20];
                                int i,j;
                                memset( aux, 0, 20 );
                                for( i = 0, j = 0;i < 20;i++ ) {
                                    if( c_imp[i] != ' ' ) {
                                        aux[j] = c_imp[i];
                                        j++;
                                    }
                                }
                                MOSTRAR_CADENA( 13, PRN, aux );
                            }
                            #else
                            IMPRIME_GRANDE( 13, c_imp );
                            #endif
                            break;
                    }
                    LF( 1 );
					if( !( RAM_TICKET_FACTURA || RAM_NOTA_CR ) )
						setData( ACTION_PRINT, NULL, 0 );
                }
            }
        }
    }
}

/*****************************************************************************/
int  PIDE_SUBTOTAL( double *importe )
/*****************************************************************************/
{
	char imp[40];
	int reintentos = 4;
	double verificar_importe = 0.0;
	int rta_print = 0;

	*importe = 0.0;
	if( IMPRESORA_FISCAL == FISCAL ) {
		char reenviar[50];
		int decimales,signo;
		char *endptr;

		setHabPing(0);
		memset(imp,0,sizeof(imp));	
			
		if ( RAM_NOTA_CR || RAM_TICKET_FACTURA ) {
			sprintf( imp, "%s&%i", fcvt( *importe, DECIMALES, &decimales, &signo ),NO );
			
			rta_print = CONTROLA_LOS_ENVIOS_Y_REINTENTA(ACTION_PRINT_REC_SUBTOTAL_FAC, imp, 0, reintentos, SI);
			
			#ifdef COMPILAR_EMULADOR_FISCAL
				verificar_importe = 2.2;
			#else
			verificar_importe = atof( imp );
			#endif COMPILAR_EMULADOR_FISCAL
			while( !verificar_importe && reintentos > 0 ) {
				sprintf( imp, "%s&%i", fcvt( *importe, DECIMALES, &decimales, &signo ),NO );
			
				rta_print = CONTROLA_LOS_ENVIOS_Y_REINTENTA(ACTION_PRINT_REC_SUBTOTAL_FAC, imp, 0, reintentos, SI);

				reintentos --;
				verificar_importe = atof( imp );
				{
					memset(reenviar,0,sizeof(reenviar));
					_snprintf(reenviar, sizeof(reenviar)-1,"Reenviando ACTION_PRINT_REC_SUBTOTAL_FAC R:%i% I:%lf",
						reintentos, verificar_importe);glog(reenviar, LOG_DEBUG, 4);
				}
			}
		} else {
#if defined(INVEL_W) || defined(INVEL_L)
			sprintf( imp, "%s&%i", fcvt( *importe, DECIMALES, &decimales, &signo ),NO );

			rta_print = CONTROLA_LOS_ENVIOS_Y_REINTENTA(ACTION_PRINT_REC_SUBTOTAL, imp, 0, reintentos, SI); //ojo si hay que anular el ticket

			verificar_importe = atof( imp );
			while( !verificar_importe && reintentos > 0 ) {
				sprintf( imp, "%s&%i", fcvt( *importe, DECIMALES, &decimales, &signo ),NO );

				rta_print = CONTROLA_LOS_ENVIOS_Y_REINTENTA(ACTION_PRINT_REC_SUBTOTAL, imp, 0, reintentos , SI);

				reintentos --;
				verificar_importe = atof( imp );
				{
					memset(reenviar,0,sizeof(reenviar));
					_snprintf(reenviar, sizeof(reenviar)-1,"Reenviando ACTION_PRINT_REC_SUBTOTAL R:%i% I:%lf",
						reintentos, verificar_importe);glog(reenviar, LOG_DEBUG, 4);
				}
			}
#endif
		}
		*importe= (double)strtod(imp,&endptr);
		if(*importe > 0 )
			*importe = *importe/100;
	}
	setHabPing(1) ;
	return( rta_print);
}

/*****************************************************************************/
int IMPRIME_TOTAL( double importe, int bultos, int modo )
/*****************************************************************************/
{
    char c_imp[20], cadena2[50];
    double total = 0;
    int _ptr, _handle, _estado, ok = 1, i = 0, continuar = SI;

	if( IMPRIMIR_TICKET ) 
	{
		if( IMPRESORA_FISCAL == FISCAL ) 
		{	//En caso de corte de luz, se consulta si el doc esta abierto al recuperar su estado
			if( DOCUMENTO_ABIERTO( ) == 2 ) {
				if( RAM_IMPRESION_COMPROBANTE ) {//Documento en proceso ya se cerro. Recupera estado
					continuar = NO;
				}
			}
			if( continuar ) {
				if( !( modo == _MODO_COBRANZA && MODO_NEGOCIO == RETAIL ) ) 
				{
					if( IMPRESORA_FISCAL != FISCAL_IBM_4694 && IMPRESORA_FISCAL != FISCAL_IBM_4610 ) 
					{
						if( RAM_PERCEPCION_DGI || RAM_RETENCION_DGR || RAM_PERCEPCION_212_IVA1
							|| RAM_PERCEPCION_212_IVA2 || RAM_PERCEPCION_TACIS ) {
							if( RAM_TICKET_FACTURA || RAM_NOTA_CR ) 
							{
								if( RAM_TICKET_FACTURA ) 
								{
									if( RAM_PERCEPCION_212_IVA1 ) 
										IMPRIME_PERCEPCION_FISCAL( ST( S_PERCEP_212_IVA1 ), 
											RAM_PERCEPCION_212_IVA1,PERCEP_212 );
									if( RAM_PERCEPCION_212_IVA2 ) 
										IMPRIME_PERCEPCION_FISCAL( ST( S_PERCEP_212_IVA2 ),
											RAM_PERCEPCION_212_IVA2,PERCEP_212 );
									if( RAM_RETENCION_DGR ) {
										char  aux[30];

										_snprintf(aux, sizeof(aux)-1,"%s %02.2f%%",/*cant,*/ ST( S_RETENCION_DGR ), porc_retenc_dgr );
										IMPRIME_PERCEPCION_FISCAL( aux, RAM_RETENCION_DGR, RETEN_DGR);
									}
									if( RAM_PERCEPCION_TACIS )
											IMPRIME_PERCEPCION_FISCAL( "PERCEPCION TACIS"/*ST(S_PERCEPCION_TACIS)*/, RAM_PERCEPCION_TACIS, PERCEP_TACIS);
								} 
								else
								{
									if( RAM_PERCEPCION_212_IVA1 ) 
										ENVIAR_PERCEPCION_COMO_ARTICULO( ST( S_PERCEP_212_IVA1 ), 
											RAM_PERCEPCION_212_IVA1 );
									if( RAM_PERCEPCION_212_IVA2 ) 
										ENVIAR_PERCEPCION_COMO_ARTICULO( ST( S_PERCEP_212_IVA2 ), 
											RAM_PERCEPCION_212_IVA2 );
									if( RAM_RETENCION_DGR ) {
										char  aux[30];

										_snprintf(aux, sizeof(aux)-1,"%s %02.2f%%",/*cant,*/ ST( S_RETENCION_DGR ), porc_retenc_dgr );
										ENVIAR_PERCEPCION_COMO_ARTICULO( aux, RAM_RETENCION_DGR );
									}
									if( RAM_PERCEPCION_TACIS ) {
										ENVIAR_PERCEPCION_COMO_ARTICULO( "PERCEPCION TACIS"/*ST(S_PERCEPCION_TACIS)*/, RAM_PERCEPCION_TACIS );
									}
								}

								if( RAM_PERCEPCION_DGI ) 
									IMPRIME_PERCEPCION_FISCAL( ST( S_PERCEPCION_DGI ), RAM_PERCEPCION_DGI,PERCEP_DGI );
							} else {
									MOSTRAR_CADENA( 1, PRN, ST( S_PERCEPCION_DGI__ ) );
									MOSTRAR_FLOTANTE( 18, PRN, ENTEROS, DECIMALES, 1, 0, RAM_PERCEPCION_DGI );
									LF( 1 );
									MOSTRAR_CADENA( 1, PRN, ST( S_RETENCION_DGR___ ) );
									MOSTRAR_FLOTANTE( 18, PRN, ENTEROS, DECIMALES, 1, 0, RAM_RETENCION_DGR );
									LF( 1 );
									MOSTRAR_CADENA( 1, PRN, "PERCEPCION TACIS"/*ST( S_PERCEPCION_TACIS )*/ );
									MOSTRAR_FLOTANTE( 18, PRN, ENTEROS, DECIMALES, 1, 0, RAM_PERCEPCION_TACIS );
									LF( 1 );
									MOSTRAR_CADENA( 1, PRN, "PERCEPCION 212"/*ST( S_PERCEPCION_212 )*/ );
									MOSTRAR_FLOTANTE( 18, PRN, ENTEROS, DECIMALES, 1, 0,
									   RAM_PERCEPCION_212_IVA1 + RAM_PERCEPCION_212_IVA2 );
								 }
							}
							LF( 1 );
							ok = FISCAL_T( importe, modo );
						}
						switch( IMPRESORA_FISCAL ) 
						{
							case FISCAL_IBM_4610:
								PRN_SELECT( RECEIPT );
								FISCAL_T( importe, modo );
								break;
							case FISCAL_IBM_4694:
								OPEN_RECUPERO( &_ptr, &_handle );
								_estado = STACK_INS( sizeof( short ) );
				
								for( i = 0;i < RAM_P_PAGO;i++ ) {
									//totalizo por medio de pago
									total += _PAGO_IMPORTE( i ) - _PAGO_VUELTO( i );
								}
								if( _ESTADO == 0 ) {
									FISCAL_T( importe, modo );
									_SET_ESTADO( 1 );
								}
								if( _ESTADO == 1 ) {
									PRN_SELECT( RECEIPT );
									//Ver esto cuando se implemente esta printer
									//             if( OPT_POS_CAB ) {
									//                cadena2[0] = 0xD8;
									//                sprintf( &cadena2[1], " TOTAL                 %012.0f  ", importe );
									//             } else {
									sprintf( cadena2, ST( S___TOTAL__________________012_0F__ ), importe );
									//             }
									MOSTRAR_CADENA( 1, PRN, cadena2 );
									LF( 1 );
									_SET_ESTADO( 2 );
								}
								CLOSE_RECUPERO( _ptr, _handle );
								break;
					}
				}
			} 
		}
		else
		{
			if( TKT_CONFIGURABLE ) 
			{
				if( !( modo == _MODO_COBRANZA && MODO_NEGOCIO == RETAIL ) ) {
					ok = IMPRIMIR_TKT_TOTAL( RAM_TOTAL, modo );
				}
			} 
			else 
			{
				OPEN_RECUPERO( &_ptr, &_handle );
				_estado = STACK_INS( sizeof( short ) );
				if( FORMATO_TICKET == 7 ) {
					LF( 1 );
				}
				if( IMPRESORA_FISCAL == FISCAL ) {
					#if defined(INVEL_W) || defined(INVEL_L)
					ok = FISCAL_T( importe, modo );
					#else
					if( IMPRESORA_FISCAL != FISCAL_IBM_4694 && IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
						ok = FISCAL_T( importe, modo );
						PRN_SELECT( RECEIPT );
					}
					switch( IMPRESORA_FISCAL ) {
						case FISCAL_IBM_4610:
							PRN_SELECT( RECEIPT );
							FISCAL_T( importe, modo );
							break;
						case FISCAL_IBM_4694:
							for( i = 0;i < RAM_P_PAGO;i++ ) {
								//totalizo por medio de pago
								total += _PAGO_IMPORTE( i ) - _PAGO_VUELTO( i );
							}
							if( _ESTADO == 0 ) {
								FISCAL_T( importe, modo );
								_SET_ESTADO( 1 );
							}
							if( _ESTADO == 1 ) {
								PRN_SELECT( RECEIPT );
								sprintf( cadena2, ST( S___TOTAL__________________012_0F__ ), importe );
								MOSTRAR_CADENA( 1, PRN, cadena2 );
								LF( 1 );
								_SET_ESTADO( 2 );
							}
							break;
					}
					#endif
				}
				else {
					if( modo != _MODO_COBRANZA ) {
						PRN_HABILITA_AVANCES();
					}
					#if defined(INVEL_W) || defined(INVEL_L)
					PRN_LETRA_GRANDE();
					MOSTRAR_CADENA( 1, PRN, ST( S_TOTAL_____ ) );
					FTOC( importe, c_imp, ENTEROS + 1, DECIMALES, 1, 0 );
					{
						char aux[20];
						int i,j;
						memset( aux, 0, 20 );
						for( i = 0, j = 0;i < 20;i++ ) {
							if( c_imp[i] != ' ' ) {
								aux[j] = c_imp[i];
								j++;
							}
						}
						MOSTRAR_CADENA( 13, PRN, aux );
					}
					PRN_LETRA_NORMAL();
					#else
					IMPRIME_GRANDE( 1, ST( S_TOTAL_____ ) );
					FTOC( importe, c_imp, ENTEROS + 1, DECIMALES, 1, 0 );
					IMPRIME_GRANDE( 12, c_imp );
					#endif
				}
				if( RAM_RETENCION_DGR ) {
					if( !IMPRESORA_FISCAL ) {
						LF( 1 );
						MOSTRAR_CADENA( 1, PRN, ST( S_RETENCION_DGR___ ) );
						MOSTRAR_FLOTANTE( 18, PRN, ENTEROS, DECIMALES, 1, 0, RAM_RETENCION_DGR );
					}
				}
				if( RAM_PERCEPCION_DGI ) {
					if( !IMPRESORA_FISCAL ) {
						LF( 1 );
						MOSTRAR_CADENA( 1, PRN, ST( S_PERCEPCION_DGI__ ) );
						MOSTRAR_FLOTANTE( 18, PRN, ENTEROS, DECIMALES, 1, 0, RAM_PERCEPCION_DGI );
					}
				}
				if( bultos && IMPRIMIR_BULTOS_EN_TICKET ) {
					if( !IMPRESORA_FISCAL ) {
						LF( 1 );
						MOSTRAR_CADENA( 1, PRN, ST( S_ARTICULOS_ ) );
						MOSTRAR_ENTERO( 16, PRN, "9999", bultos );
					}
				}
				LF( 1 );
				if( modo != _MODO_COBRANZA && POSICION_NRO_TICKET == _NRO_TICKET_DESPUES_DE_TOTAL
				 && !( modo == _MODO_REPROCESAMIENTO && event_ticket.tipo_evento != 0 )
				 && !TKT_CONFIGURABLE ) {
					IMPRIME_NRO_TICKET();
				}
				CLOSE_RECUPERO( _ptr, _handle );
				#if defined(INVEL_W) || defined(INVEL_L)
				LF( 1 );
				setData( ACTION_PRINT, NULL, 0 );
				#endif
			}
		}

		if( IMPRESORA_FISCAL == FISCAL && ok == 0 ) {
			CR1_INCOSISTENCIA_FISCAL( NO, 0, SI, NO );//Se pasa parametro suma = NO por tarea 4248
			ok = 0;
			return ( ok );
		}
		//Para valores 1, 2 y 3(_COMPROBANTE_POS_CERRADO/_COMPROBANTE_PRE_CERRADO/_DONACION_IMPRESA)	
		if( continuar ) {
			IMPRIME_PIE();
			ok = IMPRIME_FIN_TICKET( );
		}
		//Se imprime donacion una vez cerrado comprobante fiscal.
		if( TKT_CONFIGURABLE && IMPRESORA_FISCAL == FISCAL ) {
			if( RAM_IMPRESION_COMPROBANTE != _DONACION_IMPRESA ) {
				IMPRIMIR_TKT_DONACION( );
			}
		}

	}

    return( ok );
}
/*****************************************************************************/
void IMPRIME_IVA()
/*****************************************************************************/
{
	if( IMPRIMIR_TICKET )
	{
		if( IMPRESORA_FISCAL != FISCAL ) {
			LF( 1 );
			MOSTRAR_CADENA( 1, PRN, ST( S_GRAVADO____ ) );
			MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 0, 0,
							  TOT_TICK_IMPORTE_SIN_IVA - TOT_TICK_IMP_INT );
			LF( 1 );
			MOSTRAR_CADENA( 1, PRN, ST( S_I_V_A__1___ ) );
			MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 0, 0, TOT_TICK_IVA1 );
			LF( 1 );
			MOSTRAR_CADENA( 1, PRN, ST( S_I_V_A__2___ ) );
			MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 0, 0, TOT_TICK_IVA2 );
			LF( 1 );
			MOSTRAR_CADENA( 1, PRN, ST( S_EXENTO_____ ) );
			MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 0, 0, TOT_TICK_EXENTO );
			LF( 1 );
			MOSTRAR_CADENA( 1, PRN, ST( S_IMP_INT____ ) );
			MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 0, 0, TOT_TICK_IMP_INT );
			LF( 1 );
		}
    }
}
/*****************************************************************************/
void IMPRIME_MEDIO_DE_PAGO( char modo, double importe )
/*****************************************************************************/
{
	char cadena_aux[100];
    if( IMPRIMIR_TICKET ) {
		if( IMPRESORA_FISCAL == FISCAL )
		{
			memset(cadena_aux, 0, sizeof(cadena_aux));
			_snprintf(cadena_aux, sizeof(cadena_aux)-1, "%s: %.2f", medios[( int )modo].nombre, importe );
			IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL,cadena_aux, SI );//Seba
		}
		else 
		{
			MOSTRAR_CADENA( 1, PRN, medios[( int )modo].nombre );
			MOSTRAR_CADENA( 10, PRN, ":" );
			if( ( config.pais == CHILE || config.pais == BRASIL ) && importe > 9999999.99 ) {
				MOSTRAR_FLOTANTE( 12, PRN, ENTEROS + 2, DECIMALES, 1, 0, importe );
			}
			else {
				MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 1, 0, importe );
			}
			#if defined(INVEL_W) || defined(INVEL_L)
			LF( 1 );
			setData( ACTION_PRINT, NULL, 0 );
			#endif
		}
    }


}

/*****************************************************************************/
void IMPRIME_VUELTO( double importe )
/*****************************************************************************/
{
    char *p_nombre;
    int h, cod_medio, vuelto_otro_medio = 0;
    double v;
    if( IMPRIMIR_TICKET ) {
        /*if( DONACION_DE_VUELTO && RAM_TOTAL_DONACION ) {
                PROCESAR_DONACIONES(  );
                importe -= RAM_TOTAL_DONACION;
                if( RAM_TOTAL_DONACION ) {  //Nuevo total
                    MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE__ ) );
                    MOSTRAR_FLOTANTE( 23, PRN, ENTEROS, DECIMALES, 0, 0,
                        PAGOS_TOTAL( 1 ) + RAM_TOTAL_DONACION );
                    LF( 1 );
                }
        }*/
        if( RAM_RESTO_ME ) {
            _IMPRIME_VUELTO( medios[_medio_moneda_extranjera].nombre, -RAM_RESTO_ME );
        }
        else {
            for( h = 0;h < RAM_P_PAGO;h++ ) {
                v = _PAGO_VUELTO( h );
                if( v ) {
                    cod_medio = _PAGO_MODO( h );
                    p_nombre = medios[cod_medio].nombre;
                }
                else {
                    v = _PAGO_VUELTO_MEDIO( h );
                    cod_medio = _PAGO_COD_MEDIO_VUELTO( h ) - 1;
                    p_nombre = PAGOS_OBTENER_NOMBRE( (char)(_PAGO_COD_MEDIO_VUELTO( h ) ),
                                                     (char)(_PAGO_COD_SUBMEDIO_VUELTO( h ) ) );
                }
                if( v ) {
                    _IMPRIME_VUELTO( p_nombre, v );
                    importe -= v;
                    vuelto_otro_medio = 1;
                }
            }
            if( !vuelto_otro_medio || importe > 0.001 ) {
                _IMPRIME_VUELTO( medios[_medio_efectivo].nombre, importe );
            }
        }
    }
}
/*****************************************************************************/
void _IMPRIME_VUELTO( char *nombre, double importe )
/*****************************************************************************/
{
    char strImp[20], cadena_aux[100];

    if( IMPRESORA_FISCAL == FISCAL )
	{
		memset(cadena_aux, 0, sizeof(cadena_aux));
		_snprintf(cadena_aux, sizeof(cadena_aux)-1, "%s %s: $ %.2f", ST( S__CAMBIO_),nombre, importe);
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, cadena_aux, SI );//Seba
	} 
	else
	{
		if( config.pais == COLOMBIA ) {
			MOSTRAR_CADENA( 1, PRN, ST( S_VUELTO_EN ) );
			MOSTRAR_CADENA( GET_COLUMNA() + 1, PRN, nombre );
			MOSTRAR_CADENA( GET_COLUMNA() + 1, PRN, ":" );
			MOSTRAR_FLOTANTE( GET_COLUMNA() + 2, PRN, ENTEROS, DECIMALES, NO, SI, importe );
		}
		else {
			if( MODO_NEGOCIO == SERVICIOS ) {
				FTOC( importe, strImp, ENTEROS, DECIMALES, SI, SI );
				MOSTRAR_CADENA_ALD( 23, PRN, strImp, 13 ) ;
			}
			else {
				MOSTRAR_CADENA( 17, PRN, ST( S__CAMBIO_));
				LF( 1 );
				MOSTRAR_CADENA( 18, PRN, nombre );
				MOSTRAR_CADENA( 19, PRN, ":" );
				MOSTRAR_FLOTANTE( 20, PRN, ENTEROS, DECIMALES, 0, 0, importe );
			}
		}
		LF(0);
		#if defined(INVEL_W) || defined(INVEL_L)
		setData( ACTION_PRINT, NULL, 0 );
		#endif
	}

}
/*****************************************************************************/
void IMPRIME_TICKET_ANULADO( int suspendido )
/*****************************************************************************/
{
    
	char mensaje[30];
	int rta = NO, reintentos = 0;
	int borrar = NO;

	_snprintf(mensaje, sizeof(mensaje)-1,"%s", ( suspendido ) ? 
				"Ticket en Espera" :"Ticket ANULADO" );

	if( IMPRIMIR_TICKET ) {
		if( IMPRESORA_FISCAL == FISCAL ) {
			if( DOCUMENTO_ABIERTO( ) ) {
				do { 
					//TOMO EL ESTADO ANTERIOR
					if( GET_ESTADO_FISCAL( _FISCAL_offline	         )||
						GET_ESTADO_FISCAL(_FISCAL_paper_out          )||   
						GET_ESTADO_FISCAL(_FISCAL_printer_error      )||   
						//GET_ESTADO_FISCAL(_FISCAL_nec_cierre         )||
						//GET_ESTADO_FISCAL(_FISCAL_desborde_totales   )||
						( GET_ESTADO_FISCAL(_FISCAL_comando_invalido )&&//si se dan los dos entra debe seguir... no imprime
							(GET_ESTADO_FISCAL(_FISCAL_nec_cierre )== NO ))		) {

						rta = NO;
						reintentos++;	
						MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) ); 
						GRABAR_LOG_SISTEMA(  ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) , LOG_ERRORES,2 );
						borrar = SI;
						do{ 
							if( LASTKEY()!=-999 ){//que no muestre tantas veces..
						//		BORRAR_MENSAJE();
								MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) );
							}
						}while( GETCH( ) != 13 );
						//leo el nuevo estado a ver si cambia
						if( borrar == SI) 
						BORRAR_MENSAJE();


						COMANDO_LEER_STATUS_PRINTER_FISCAL(1, NULL);
					} else{
						rta = SI;
						break;
					}
				}while( ( rta != SI )&& reintentos < 10 ); // Jpos error wrong state
				if( borrar == SI)
					BORRAR_MENSAJE();
				

				if( DOCUMENTO_ABIERTO( ) ) {
					if( RAM_TICKET_FACTURA || RAM_NOTA_CR ) {
						ANULA_TICKET_FACTURA_FISCAL( mensaje ); 
					} else {
						ANULA_TICKET_FISCAL( mensaje ); 
					}
					if( GET_ESTADO_FISCAL( _FISCAL_doc_no_fiscal_abierto ) ) {
						CERRAR_DOCUMENTO_NO_FISCAL();
					}
					reintentos =0;
						
					/****************************************************************************/
					/* Es solo si el comprobante abierto es distinto de lo indicado por la RNV 
					sino puede cancelarlo, quedará abierto */
					/****************************************************************************/
					while( DOCUMENTO_ABIERTO( ) == 1  && reintentos < 2) {
						if( RAM_TICKET_FACTURA || RAM_NOTA_CR ){
							ANULA_TICKET_FISCAL( mensaje ); 
						}else{
							ANULA_TICKET_FACTURA_FISCAL( mensaje ); 
						}
						reintentos++;
					};
					/****************************************************************************/
				}
			}
		} else {
			if( !( RAM_FACTURA && !IMPRIMIR_TICKET_SI_ES_REMOTA ) ) {
                PRN_HABILITA_AVANCES();
            }
            else {
                PRN_SELECT( JOURNAL );
            }

			if( ( ( config.pais == COLOMBIA || config.pais == ECUADOR ) ) && suspendido ) {
				MOSTRAR_CADENA( 1, PRN, ST( S___________TICKET_SUSPENDIDO__________ ) );
			} else {
				if( MODO_NEGOCIO == RETAIL ) {
					if( TKT_CONFIGURABLE ) {
						IMPRIME_TKT_IMPORTE_TOTAL( 1 );
					}
					if( RAM_MODO_EJECUCION != INVENTARIO ) {
						IMPRIME_PIE( );
					} else {
					   LF(1);
					   MOSTRAR_CADENA( 1, PRN, ST(S______________TICKET_ANULADO_____________));
				   }
				} else {
					if( IMPRIMIR_TICKET_AL_FINAL ) {
						MOSTRAR_CADENA( 1, PRN, ST( S______________TICKET_ANULADO_____________ ) );
						LF( 2 );
					} else {
						MOSTRAR_CADENA( 1, PRN, ST( S______________TICKET_ANULADO_____________ ) );
					}
				}
			}
		}

        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
            FIN_DOCUMENTO( 1, 0.0 );
        }
        else {
            PRN_FIN_TICKET();
        }
    }
}
/*****************************************************************************/
void IMPRIME_PIE()
/*****************************************************************************/
{
	if( IMPRIMIR_TICKET ) {
        int h;
        if( IMPRESORA_FISCAL != FISCAL )
		{
			if( TKT_CONFIGURABLE ) {
	            IMPRIMIR_TKT_IVAS();
	            IMPRIMIR_TKT_AFINIDAD();
	            IMPRIMIR_TKT_NO_AFINIDAD();
	            IMPRIMIR_TKT_IDENTIFICACION();
				IMPRIMIR_TKT_INFORMACION_COMPARATIVA();
	            IMPRIMIR_TKT_PIE( );
	            if( IMPRESORA_FISCAL == FISCAL_TMT88 )//corte de papel
	            {
	                TKT_CORTE_PAPEL();
	            }
	        }
			else {
	            if( POSICION_NRO_TICKET == _NRO_TICKET_EN_PIE ) {
	                IMPRIME_NRO_TICKET();
	            }
	            if( RAM_BOLETA_ANTERIOR ) {
	                MOSTRAR_CADENA( 1, PRN, ST( S_BOLETA_ANTERIOR ) );
	                MOSTRAR_ENTERO( 18, PRN, "00000000", RAM_BOLETA_ANTERIOR );
	                LF( 1 );
	            }
	            if( RAM_BONOS_A_ENTREGAR ) {
	                LF( 1 );
	                MOSTRAR_CADENA( 4, PRN, ST( S_______BONOS_A_ENTREGAR__ ) );
	                MOSTRAR_ENTERO( 29, PRN, "999", RAM_BONOS_A_ENTREGAR );
	                MOSTRAR_CADENA( 33, PRN, "*****" );
	                LF( 1 );
	            }
	            VALIDAR_CLIENTE_AFINIDAD();
	            VALIDAR_LIMITE_DGI();
	            if( !IMPRESORA_FISCAL || IMPRESORA_FISCAL == FISCAL_POS745x ) {
	                LF( 1 );
	                PRN_INHABILITA_AVANCE_JOURNAL();
	            }
	            if( IMPRESORA_FISCAL == FISCAL_IBM_4610 && MODO_DEBUG == 15 ) {
	                GRABAR_LOG_SISTEMA( pie , LOG_DEBUG,2);
	            }
	            for( h = 0;pie[h] != 26;h++ ) {
	                SEND_BYTE( pie[h] );
	            }
	            PRN_HABILITA_AVANCES();
	            PRN_FIN_TICKET();
	        }
		} else {
			IMPRIMIR_TKT_PIE( );
		}
	}
}
/*****************************************************************************/
int IMPRIME_FIN_TICKET( )
/*****************************************************************************/
{	
	int rta = 1 ;

	if( IMPRIMIR_TICKET ) {
		if( IMPRESORA_FISCAL == FISCAL )//Seba
		{
			#ifdef COMPILAR_RECUPERO
				MENSAJE_TECLA( "Documento Fiscal antes de Cerrar, Presione una tecla si desea continuar" );
			#endif
			if( RAM_TICKET_FACTURA ) {//Seba
				rta = CERRAR_TIQUE_FACTURA(RESP_INSC, clientes.cond_iva);//Seba
				if( rta == 16 || rta == 15 ) { //comando no valido
					CR1_INCOSISTENCIA_FISCAL( SI, 0, SI, NO );
					rta = 0;
				}
			} else if( RAM_NOTA_CR )
				CERRAR_NOTA_DE_CREDITO(RESP_INSC, clientes.cond_iva);//Seba
				else if ( RAM_MODO_EJECUCION == INVENTARIO )
						CERRAR_DOCUMENTO_NO_FISCAL();
				else {
					rta = CERRAR_DOCUMENTO_FISCAL();//Seba
					if( rta == 16) { //comando no valido
						CR1_INCOSISTENCIA_FISCAL( SI, 0, SI, NO );
						rta = 0;
					}
				}
			#ifdef COMPILAR_RECUPERO
				MENSAJE_TECLA( "Documento Fiscal Cerrado, Presione una tecla si desea continuar" );
			#endif
		}
		else//Seba
			if( TKT_CONFIGURABLE ) 
				IMPRIMIR_TKT_FIN_TICKET( );
	}		

	return rta;
}

/*****************************************************************************/
void IMPRIME_NRO_TICKET()
/*****************************************************************************/
{
    if( IMPRIMIR_TICKET ) {
        ADD_MEMORY_LONG( __nro_ticket, 1L );
        ADD_MEMORY_LONG( __x_cantidad_tickets, 1L );
		if( !IMPRESORA_FISCAL ) {
            if( config.pais == CHILE || config.pais == BRASIL ) {
                MOSTRAR_CADENA( 1, PRN, ST( S_BOLETA ) );
            }
            else {
                MOSTRAR_CADENA( 1, PRN, ST( S_TICKET ) );
            }
            MOSTRAR_ENTERO( 8, PRN, "00000000", NRO_TICKET );
            LF( 2 );
        }
        #if defined(INVEL_W) || defined(INVEL_L)
        setData( ACTION_PRINT, NULL, 0 );
        #endif
    }
}
#ifdef COMPILAR_TECLA_REPROCESAR
/*****************************************************************************/
void IMPRIME_REPROCESAMIENTO()
/*****************************************************************************/
{
    if( IMPRIMIR_TICKET ) {
        PRN_DISCONTINUA();
        PRN_HABILITA_AVANCES();
        MOSTRAR_CADENA( 1, PRN, ST( S_________REPROCESAMIENTO_DEL_PAGO________ ) );
        LF( 2 );
        if( IMPRESORA_FISCAL ) {
            PRN_SELECT( JOURNAL );
            MOSTRAR_CADENA( 1, PRN, ST( S_________REPROCESAMIENTO_DEL_PAGO________ ) );
            LF( 1 );
            PRN_SELECT( RECEIPT );
        }
        if( !IMPRIMIR_TICKET_AL_FINAL ) {
            IMPRIME_CABECERA_TICKET( NO );
            IMPRIME_TITULO_TICKET();
        }
    }
}
#endif
/**************************************************************************/
void IMPRIME_CABECERA_GENERAL( long nro_evento )
/**************************************************************************/
{
    unsigned lf = 0;
	char cad_aux[50];

	if( IMPRIMIR_NRO_EVENTO ) {
        if(IMPRESORA_FISCAL == FISCAL)
		{
			_snprintf(cad_aux, sizeof(cad_aux)-1, "%s%08li",ST( S_SECUENCIA ),nro_evento);
			IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_DOBLE_ANCHO_NEGRITA,cad_aux, SI );
		}
		else
		{
			//SET_TIPO_LETRA( LETRA_DOBLE_ANCHO_NEGRITA );
			MOSTRAR_CADENA( 1, PRN, ST( S_SECUENCIA ) );
			MOSTRAR_ENTERO( 11, PRN, "00000000", nro_evento );
			//NEXT_LINE_PRN( SI );
			//SET_TIPO_LETRA( LETRA_NORMAL );
			lf = 1;
		}
    }
    if( FORMATO_TICKET == 7 && DATOS_VENDEDOR_EN_TICKET && NRO_VENDEDOR_LONG ) {
		if(IMPRESORA_FISCAL == FISCAL)
		{
			_snprintf(cad_aux, sizeof(cad_aux)-1, "%s%05ld ",ST( S_VENDEDOR_ ), NRO_VENDEDOR_LONG);
			IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_DOBLE_ANCHO_NEGRITA,cad_aux, SI );
		}
		else
		{
        MOSTRAR_CADENA( 27, PRN, ST( S_VENDEDOR_ ) );
        MOSTRAR_ENTERO( 36, PRN, "99999", NRO_VENDEDOR_LONG );
        lf = 1;
		}
    }
    if( lf ) {
        LF( 1 );
    }
    if( !TKT_CONFIGURABLE || ( config.pais == ELSALVADOR ) ) {
        IMPRIME_CAJERO();
    }
}
/********************************************************************************/
int DEBE_IMPRIMIR_ITEM()
/********************************************************************************/
{
    int ok = 1;
	ok = !EMITIR_FACTURA() && !EMITIR_REMITO();

	if( ( RAM_FACTURA || RAM_NOTA_CR ) && IMPRESORA_FISCAL == FISCAL ){
		ok = EMITIR_FACTURA();
	}
    if( EMITIR_FACTURA_REMOTA() ) {
        ok = IMPRIMIR_TICKET_SI_ES_REMOTA;
    }
	if( IMPRIMIR_TICKET_AL_FINAL ) {
        ok = 0;
    }
	if( RAM_MODO_EJECUCION == INVENTARIO ){
		ok = 1;
	}

    return ( ok );
}
/********************************************************************************/
int IMPRIMIR_TICKET_COMPLETO( int cabecera )
/********************************************************************************/
{
    struct _acu_art *acu_art;
    int p_art, h, l, imprime_subtotal = IMPRIME_SUBTOTAL_AL_PASAR_A_TOTA,
        ok = 1;
    char nombre[41],nombre_desc[20],cod_barra[20];
    GRABAR_LOG_SISTEMA("COMIENZO IMPRESION DEL TICKET COMPLETO", LOG_VENTAS,4 );
    if( IMPRIMIR_TICKET ) {
		if( cabecera ) {
        IMPRIME_CABECERA_TICKET( SI );
		}
        if( RAM_MODO_EJECUCION != INVENTARIO ) {
            if( IMPRESORA_FISCAL != FISCAL_IBM_4694 ) {
                ok = FISCAL_B();
            }
            GRABAR_LOG_NRO_TICKET();
        }
        if( !IMPRESORA_FISCAL || ok ) {
            IMPRIME_TITULO_TICKET();
            #ifdef COMPILAR_TECLA_CAMBIO
            IMPRIME_CAMBIO();
            #endif
            /*----------------------- Imprime cuerpo -------------------------*/
			IMPRIME_TKT_DESCRIPCION_CUERPO( 1 );
            l = sizeof( struct _acu_art ) * _LARGO_TICKET;
            acu_art = (struct _acu_art *)MALLOC( l );
            p_art = CARGA_ACU_ART( acu_art, l, NO, NULL, SI, NO );
            if( p_art ) {
                for( h = 0;h < p_art;h++ ) {
                    if( acu_art[h].cant ) {
                        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                            BUSCA_COD_INTERNO_COD_BARRA( acu_art[h].cod, acu_art[h].cod_barra );
                            ASIGNA_ITEM( NO );
                        }
                        else {
                            ASIGNA_CODIGO_ESPECIAL( acu_art[h].cod );
                        }
                        SET_MEMORY_DOUBLE( __item_precio, _TICKET_PRECIO( acu_art[h].pos ) );
                        SET_MEMORY_DOUBLE( __item_precio_sin_iva,
                                           _TICKET_IMPORTE_SIN_IVA( acu_art[h].pos )
                                         + _TICKET_EXENTO( acu_art[h].pos ) );
                        SET_MEMORY_DOUBLE( __item_recargo, _TICKET_RECARGO( acu_art[h].pos ) );
                        SET_MEMORY_CHAR( __item_cod_movimiento, _TICKET_COD_MOVIMIENTO( acu_art[h].pos ) );
                        SET_MEMORY_CHAR( __item_origen, _TICKET_ORIGEN( acu_art[h].pos ) );
                        SET_MEMORY_DOUBLE( __ram_cantidad, acu_art[h].cant );
                        SET_MEMORY_DOUBLE( __ram_cantidad_original, 0 );
                        SET_MEMORY_INT( __ram_anulacion, 0 );
                        if( config.pais == ECUADOR
                         && _TICKET_ORIGEN( acu_art[h].pos ) == _ORIGEN_DESC_PORC
                         && ( _TICKET_TIPO2( acu_art[h].pos ) == 9
                           || _TICKET_TIPO2( acu_art[h].pos ) == 0 ) ) {
                            strcpy( nombre, ITEM_NOMBRE_LARGO );
                            TRIM( nombre );
                            sprintf( nombre_desc, " %5.2lf%%", _TICKET_COSTO( acu_art[h].pos ) );
                            strcat( nombre, nombre_desc );
                            nombre[30] = 0;
                            SET_MEMORY( __item_nombre_largo, nombre );
                        }
                        if( imprime_subtotal
                         && _TICKET_ORIGEN( acu_art[h].pos ) == _ORIGEN_PROMOCION ) {
                            IMPRIME_SUBTOTAL( RAM_SUBTOTAL, imprime_subtotal );
                            imprime_subtotal = NO;
                        }
                        memset( cod_barra, 0, sizeof( cod_barra ) );
                        strncpy( cod_barra, _TICKET_COD_BARRA_ORIGINAL( acu_art[h].pos ),
                                 sizeof( cod_barra ) - 1 );
                        if( strlen( cod_barra ) == 0 ) {
                            strncpy( cod_barra, _TICKET_COD_BARRA( acu_art[h].pos ),
                                     sizeof( cod_barra ) - 1 );
                        }
                        SET_MEMORY( __item_cod_barra, cod_barra );
                        IMPRIME_ITEM( acu_art[h].pos );
                    }
                }
                if( imprime_subtotal ) {
                    IMPRIME_SUBTOTAL( RAM_TOTAL, imprime_subtotal );
                }
            }
            FREEMEM( ( char* )acu_art );
        }
    }
    GRABAR_LOG_SISTEMA("FIN IMPRESION DEL TICKET COMPLETO", LOG_VENTAS,4 );
    return( ok );
}
#ifdef COMPILAR_TECLA_CAMBIO
/*****************************************************************************/
void IMPRIME_CAMBIO()
/*****************************************************************************/
{
    /*if( RAM_CAMBIO ) {
        MOSTRAR_CADENA( 11, PRN, ST( S____CAMBIO___ ) );
        LF( 1 );
    }*/
}
#endif
/*****************************************************************************/
void IMPRIMIR_CAJERO_EN_SLIP( int ingreso )
/*****************************************************************************/
{
    char cierre[] = "** CIERRE DE CAJA **";
    char abrir[] = "** APERTURA DE CAJA **";
    int l;
    PRN_INHABILITA_AVANCE_RECEIPT();
    if( ingreso ) {
        l = strlen( abrir );
        MOSTRAR_CADENA( ( 40 - l ) / 2, PRN, abrir );
    }
    else {
        l = strlen( cierre );
        MOSTRAR_CADENA( ( 40 - l ) / 2, PRN, cierre );
    }
    LF( 2 );
    IMPRIME_CAJERO();
    if( IMPRESORA_FISCAL == FISCAL ) {
		IMPRIME_TKT_INGRESO_EGRESO_CAJERO();
    }
    else {
        PRN_HABILITA_AVANCES();
    }
}
/*****************************************************************************/
void IMPRIME_CAJERO()
/*****************************************************************************/
{
    unsigned fecha,hora,cajero;
    unsigned long nro_legajo_cajero;
	struct _cajeros sup_cajero;
	int rta;

	if( !IMPRIMIR_PASILLO_EN_TICKET ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_FECHA ) );
        MOSTRAR_CADENA( 12, PRN, ST( S_HORA ) );
        MOSTRAR_CADENA( 20, PRN, ST( S_SUC_ ) );
        if( IMPRIMIR_LEGAJO_CAJERO ) {
            MOSTRAR_CADENA( 26, PRN, ST( S_CAJA ) );
        }
        else {
            MOSTRAR_CADENA( 27, PRN, ST( S_CAJA ) );
        }
        MOSTRAR_CADENA( 33, PRN, ST( S_CAJERO ) );
    }
	else {
        MOSTRAR_CADENA( 1, PRN, ST( S_FECHA ) );
        MOSTRAR_CADENA( 10, PRN, ST( S_HORA ) );
        MOSTRAR_CADENA( 17, PRN, ST( S_SUC_ ) );
        if( IMPRIMIR_LEGAJO_CAJERO ) {
            MOSTRAR_CADENA( 23, PRN, ST( S_CAJA ) );
        }
        else {
            MOSTRAR_CADENA( 24, PRN, ST( S_CAJA ) );
        }
        MOSTRAR_CADENA( 33, PRN, ST( S_CAJERO ) );
    }
    LF( 1 );
    if( _IMPRIMIENDO_Z_ANT ) {
        fecha = NRO_FECHA_Z;
        hora = VARIOS_HORA_Z;
        cajero = VARIOS_CAJERO_Z;
        nro_legajo_cajero = VARIOS_NRO_LEGAJO_CAJERO_Z;
    }
    else {
        fecha = GET_FECHA_DOS();
        hora = GET_HORA_DOS();
        cajero = NRO_CAJERO;
        nro_legajo_cajero = NRO_LEGAJO_CAJERO;
    }
    if( !IMPRIMIR_PASILLO_EN_TICKET ) {
        MOSTRAR_FECHA( 1, PRN, fecha );
        MOSTRAR_HORA( 12, PRN, hora );
        MOSTRAR_ENTERO( 20, PRN, "000", config.sucursal );
    }
    else {
        MOSTRAR_FECHA( 1, PRN, fecha );
        MOSTRAR_HORA( 10, PRN, hora );
        MOSTRAR_ENTERO( 17, PRN, "000", config.sucursal );
    }
    if( IMPRIMIR_LEGAJO_CAJERO ) {
        if( IMPRIMIR_PASILLO_EN_TICKET ) {
            MOSTRAR_ENTERO( 22, PRN, "0000", NUMERO_CAJA() );
            MOSTRAR_CADENA( 26, PRN, "/" );
            MOSTRAR_ENTERO( 27, PRN, "000", NRO_CAJA );
        }
        else {
            MOSTRAR_ENTERO( 26, PRN, "0000", NUMERO_CAJA() );
        }
        MOSTRAR_ENTERO( 32, PRN, "00000000", nro_legajo_cajero );
    }
    else {
        if( IMPRIMIR_PASILLO_EN_TICKET ) {
            MOSTRAR_ENTERO( 24, PRN, "0000", NUMERO_CAJA() );
            MOSTRAR_CADENA( 28, PRN, "/" );
            MOSTRAR_ENTERO( 29, PRN, "000", NRO_CAJA );
        }
        else {
            MOSTRAR_ENTERO( 27, PRN, "0000", NUMERO_CAJA() );
        }
        MOSTRAR_ENTERO( 35, PRN, "0000", cajero );
    }
	LF(1);
	LEE_CAJERO( USUARIO_CLAVE_ANTERIOR(), &sup_cajero );
	rta = ( sup_cajero.tipo == SUPERVISOR ) ? SI : NO;
    if( rta ) 
	{
		IMPRIMIR_NOMBRE_USUARIO( sup_cajero.cod_cajero );
	}
	IMPRIMIR_NOMBRE_USUARIO( NRO_CAJERO );
	LF(1);

    if( MODO_DEVOLUCION ) {
        LF( 1 );
        MOSTRAR_CADENA( 10, PRN, ST( S______DEVOLUCION_____ ) );
        LF( 1 );
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    LF( 0 );
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void IMPRIMIR_COPIAS_TICKET_VOUCHERS()
/*****************************************************************************/
{
    int copias_t = 0, copias_v = 0, i, h/*, area*/;
	int tabla_ant, tipo_ant;
    long id_evento;
    struct _cant_tickets copiast;
    struct
    {
        int evento;
        int cod_medio;
        int cod_submedio;
    } rel_copiast;
    //area = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    id_evento = RAM_ID_EVENTO - 1;
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
	if( event_ticket.tipo_evento != 99 ) {
        if( OPEN_TABLE( T_COPIAS_T, TT_ORIG, ( char* )&copiast, sizeof( struct _cant_tickets ) ) == 0 ) {
            rel_copiast.evento = event_ticket.tipo_evento;
			for( h = 0;h < RAM_P_PAGO;h++ ) {
                rel_copiast.cod_submedio = 0;
                if( medios[( int )_PAGO_MODO( h )].tarjeta ) {
                    rel_copiast.cod_submedio = _PAGO_TARJETA( h ) + 1;
                }
                if( medios[( int )_PAGO_MODO( h )].ticket_compra ) {
                    rel_copiast.cod_submedio = _PAGO_CUENTA( h );
                }
                if( medios[( int )_PAGO_MODO( h )].mutual ) {
                    rel_copiast.cod_submedio = _PAGO_CUENTA( h );
                }
                rel_copiast.cod_medio = _PAGO_MODO( h ) + 1;
				SET_WHERE( "TIPO_EVENTO = '%i' AND COD_MEDIO = '%i' AND COD_SUBMEDIO = '%i'",
					rel_copiast.evento, rel_copiast.cod_medio, rel_copiast.cod_submedio );
				RUN_QUERY(NO);
                if( FOUND2() ) {
                    if( copias_t < copiast.cant_tickets ) {
                        copias_t = copiast.cant_tickets;
                    }
                    if( copias_v < copiast.cant_vouchers ) {
                        copias_v = copiast.cant_vouchers;
                    }
                }
            }
			CLOSE_TABLE( T_COPIAS_T, TT_ORIG );
            for( i = 0;i < copias_t;i++ ) {
                IMPRIME_COPIA_TICKET( NO );
            }
            for( i = 0;i < copias_v;i++ ) {
                IMPRIME_COPIA_VOUCHER( NO );
            }
        }
        else {
            GRABAR_LOG_SISTEMA( "Error Copias" , LOG_VENTAS,2 );
        }
    }
    else {
        GRABAR_LOG_SISTEMA( "99" , LOG_VENTAS,4 );
    }
    //SELECT( area );
	SELECT_TABLE( tabla_ant, tipo_ant );
}
/**************************************************************************/
void IMPRIME_CABECERA_TIQUE_FACTURA()//Seba
/**************************************************************************/
{
	
	int cond_IVA = 0;
	char nombre[50], nombre_linea1[20], nombre_linea2[20], remitos[5], remitos1[5];

	cond_IVA = OBTENER_COND_IVA_CLIENTE( 0 );

	memset(nombre, 0, sizeof(nombre));
	memset(nombre_linea1, 0, sizeof(nombre_linea1));
	memset(nombre_linea2, 0, sizeof(nombre_linea2));
	memset(remitos, 0, sizeof(remitos));
	memset(remitos1, 0, sizeof(remitos1));

	strcpy(nombre, clientes.nombre);
    strncpy( nombre_linea1, nombre, sizeof(nombre_linea1)-1 );
    strcpy( nombre_linea2, &nombre[19] );
	if(clientes.cond_iva != 5)
		if(strlen(nombre_linea2)==0)
			_snprintf(nombre_linea2, sizeof(nombre_linea2)-1,".");

	if( clientes.perfil_impositivo == 100) { //cambio afip 01/07/2021
				memset(clientes.telefono, 0, sizeof(clientes.telefono));
				strncpy( clientes.telefono, clientes.provincia,	sizeof( clientes.provincia ));
				
				memset( clientes.provincia, 0,sizeof( clientes.provincia) );
				strncpy( clientes.provincia, clientes.localidad,sizeof( clientes.provincia) );
				
				memset( clientes.localidad, 0,sizeof( clientes.localidad) );
				strncpy( clientes.localidad, clientes.domicilio,sizeof( clientes.localidad) );

				clientes.localidad[20]=0;
				
				memset( clientes.domicilio, 0,sizeof( clientes.domicilio) );
				strncpy( clientes.domicilio, "RECEPTOR COMPROBANTE - RESP. MONOTRIBUTO",
                                 sizeof( clientes.domicilio ) );
				

			} else {
				strncpy( clientes.telefono, ".",1);
			}
	
	/*
	parche que soluciona el tema de tipo de documento
	La solucion debe contemplar el tipo de documento informado por el cliente y enviar el nro dedocumento o cuit
	, >>>>>><no debe ser harcodeado por  al condición de IVA<<<<<
	*/

	switch(clientes.cond_iva){
		case 5:{
			  char doc[10];
				memset( doc, ' ', 10 );
				_ltoa( clientes.nro_documento, doc, 10 );
				ABRIR_TIQUE_FACTURA('F',RESP_INSC,cond_IVA,
					nombre_linea1, nombre_linea2, _DNI, doc, 
					clientes.domicilio, clientes.localidad,clientes.provincia,
					remitos,remitos1); 

//					"."/*clientes.empresa*/,"."/*clientes.telefono*/); 
			   }break;
		case 7:{
			ABRIR_TIQUE_FACTURA('F',RESP_INSC,cond_IVA,
					nombre_linea1, nombre_linea2, CUIL, clientes.cuit,
					clientes.domicilio, clientes.localidad,clientes.provincia,
					"."/*clientes.empresa*/,"."/*clientes.telefono*/); 
			   }break;
		default:{
				ABRIR_TIQUE_FACTURA('F',RESP_INSC,cond_IVA,
					nombre_linea1, nombre_linea2, __CUIT, clientes.cuit,
					clientes.domicilio, clientes.localidad,clientes.provincia,
					clientes.telefono,"."/*clientes.telefono*/); 
				};
			
	}
	
	//  if( clientes.cond_iva != 5 && clientes.cond_iva != 7 )
//		ABRIR_TIQUE_FACTURA('F',RESP_INSC,cond_IVA,
//					nombre_linea1, nombre_linea2, __CUIT, clientes.cuit,
//					clientes.domicilio, clientes.localidad,clientes.provincia,
//					"."/*clientes.empresa*/,"."/*clientes.telefono*/); 
//	else {
//		char doc[10];
//		memset( doc, ' ', 10 );
//		_ltoa( clientes.nro_documento, doc, 10 );
//		ABRIR_TIQUE_FACTURA('F',RESP_INSC,cond_IVA,
//					nombre_linea1, nombre_linea2, _DNI, doc, 
//					clientes.domicilio, clientes.localidad,clientes.provincia,
//					"."/*clientes.empresa*/,"."/*clientes.telefono*/); 
//	}

}
/**************************************************************************/
void IMPRIME_CABECERA_NOTA_DE_CREDITO() 
/**************************************************************************/
{
	char nombre[50], nombre_linea1[20], nombre_linea2[20];
	int cond_IVA = 0;

	cond_IVA = OBTENER_COND_IVA_CLIENTE( 0 );

	memset(nombre, 0, sizeof(nombre));
	memset(nombre_linea1, 0, sizeof(nombre_linea1));
	memset(nombre_linea2, 0, sizeof(nombre_linea2));

	strcpy(nombre, clientes.nombre);
	strncpy( nombre_linea1, nombre, sizeof(nombre_linea1)-1 );
	strcpy( nombre_linea2, &nombre[19] );

	if(strlen(nombre_linea2)==0)
		_snprintf(nombre_linea2, sizeof(nombre_linea2)-1,".");

	if( clientes.cond_iva != 5 && clientes.cond_iva != 7 )
		ABRIR_NOTA_DE_CREDITO('F',RESP_INSC,cond_IVA, 
							nombre_linea1, nombre_linea2, __CUIT,clientes.cuit,
  							clientes.domicilio,clientes.localidad,clientes.provincia,
							"abc"/*clientes.banco*/,"abc"/*clientes.nro_cuenta*/); 
	else {
		char doc[10];
		memset( doc, ' ', 10 );
		_ltoa( clientes.nro_documento, doc, 10 );
		ABRIR_NOTA_DE_CREDITO('F',RESP_INSC,cond_IVA, 
							nombre_linea1, nombre_linea2, _DNI, doc, 
  							clientes.domicilio,clientes.localidad,clientes.provincia,
							"abc"/*clientes.banco*/,"."/*clientes.nro_cuenta*/); 
	}
}
