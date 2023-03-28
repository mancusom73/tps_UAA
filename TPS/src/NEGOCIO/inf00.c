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
#include <cio.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <eventos.h> 
#include <fiscal.h> 
#include <ftoa.h> 
#include <get_fech.h> 
#include <imp_piey.h> 
#include <imp_tick.h> 
#include <inf.h> 
#include <inf00.h> 
#include <inf_dif.h> 
#include <malloc.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <m_fecha.h> 
#include <pausa.h> 
#include <print.h> 
#include <round.h> 
#include <stack.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <aarch.h>
#include <deftables.h>
#include <db.h>
#include <path.h>
#include <ini.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif

struct institut_donacion
{
    BINARY status;
    int cod_institucion;
    char desc_institucion[31];
    BINARY fin;
};

struct _cab_xyz
{
    unsigned short fecha;
    long nro;
    unsigned short cant_tickets;
    unsigned short dia_operativo;
    double gran_total;
    char reserva[20];
};
extern int GET_INICIO_INFORME_Y(void );
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos, tipo_t_varios;
extern int GET_FIN_INFORME_Y(void );
extern int IMPRIMIR_TKT_INF_Y( int ini_est,int inicio_tkt,int fin_tkt );
struct _datosCierre{
    double tot[10],dif[10],cierre[10],tot_me[10],dif_me[10],cierre_me[10];
};
void INF00_DISCRIMINAR( char *titulo, double tot, int cod_imp, int cod_can, int ini_est,
                        int cod_imp_me );
void INF00_DISCRIMINAR_RETIROS( double tot, int cod_imp, int cod_can, int ini_est,
                        int cod_imp_me, struct _datosCierre datosCierre, double tot_cierre );
void INF00_ITEM( char *titulo, double tot, int cod_imp, int cod_can, int ini_est );
void _INF00_ITEM( char *titulo, double tot, double importe, int cod_can, int ini_est );
void INFORME_00_CUERPO( char *nombre, double importe, int porcentaje, int cantidad );
void INF00_DISCRIMINAR_IVA( double tot_venta, int ini_est );
void INFORME_00_PIE();
void INFORME_00_SEPARADOR();
void ACUMULAR_X();
void IMPRIME_DIA_OPERATIVO( int anterior );
void IMPRIMIR_USUARIO_Y_SUPERVISOR( void );
int _dif[10],_dif_me[10];
char cadaux[20];
static int print_pos = 0;
int cierre_x, cierre_z;
/*****************************************************************************/
void INFORME_00( int est, char cierre, int anterior )
/*****************************************************************************/
{
    int ini_est;
    ini_est = _APUNTAR_STRUCT( est );
    EMITIR_INFORME_00( anterior, est, cierre, ini_est );
}
/*****************************************************************************/
void EMITIR_INFORME_00( int anterior, int est, char cierre, int ini_est )
/*****************************************************************************/
{
    double tot_ventas = 0,
           tot_retiros = 0,
           tot_cobros = 0,
           gran_total = 0.0, 
           cierre_total = 0.0,
           //Notar q en dif se almacenan los totales de los medios, no las 
           //diferencias. Esto es un poco confuso
           dif[10], tot_fondo_f = 0, tot_cambios = 0, dif_me[10];
    int h,codigo;
    long cant_tickets, nro_ticket = 0;
    double aux,aux_gravado;
    double aux_iva1;
    struct institut_donacion instituciones;
    char mensaje[40], cadena_aux[50];
    int _ptr,_handle,_estado_dos;
    struct _datosCierre datosCierre;

    OPEN_RECUPERO( &_ptr, &_handle );
    _estado_dos = STACK_INS( sizeof( short ) );

	if( X_NRO_PRIMER_TICKET == 0 )
		SET_MEMORY_LONG( __x_nro_primer_ticket, NRO_TICKET - X_CANTIDAD_TICKETS + 1 ); //seba

	if( est ==_z && Z_NRO_PRIMER_TICKET == 0 )
		SET_MEMORY_LONG( __z_nro_primer_ticket, NRO_TICKET - Z_CANTIDAD_TICKETS + 1 ); //seba

	SET_MEMORY_LONG( __x_nro_transaccion_final, NRO_EVENTO - 1 );//seba
	
    MENSAJE_SIN_SONIDO_STRING( S_IMPRIMIENDO_EL_INFORME_DE_VENTAS_Y_RETIROS ); 

	cierre_x = NO;
	cierre_z = NO;

    //---------------------------- puntero ----------------------------------
    if( est == _x ) {
        cant_tickets = X_CANTIDAD_TICKETS;
		cierre_x = SI;
    }
    if( est == _z ) {
        cant_tickets = Z_CANTIDAD_TICKETS;
		cierre_z = SI;
    }
    if( est == _z_ant ) {
        cant_tickets = NRO_CANT_TICKETS_ANT;
    }
	if( _ESTADO_DOS == 0 ) {
		if( IMPRESORA_FISCAL ) {
			if( est == _z ) {
                if( IMPRESORA_FISCAL != FISCAL_IBM_4694 ) {
					FISCAL_Y();
                    _SET_ESTADO_DOS( 1 );
                }
                else {
                    _SET_ESTADO_DOS( 2 );
                }
            }
            else {
                _SET_ESTADO_DOS( 13 );
            }
        }
        else {
            _SET_ESTADO_DOS( 4 );
        }
    }
	if( _ESTADO_DOS == 1 ) {
        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 && IMPRIMIR_PIE_Z ) {
            _SET_ESTADO_DOS( 2 );
        }
        else {
            _SET_ESTADO_DOS( 5 );
        }
    }
	if( _ESTADO_DOS == 5 ) {
        #if (defined INVEL_L) || (defined INVEL_W)
        if( IMPRESORA_FISCAL == FISCAL && !SINCRONIZAR_HORA_CONTROLADOR_FIS 
			||(IMPRESORA_FISCAL == FISCAL && config_tps.ImprimirYdespuesDeZ == 1)) {
            FISCAL_Z();
		} 
        #endif
        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
            FISCAL_Z();
        }
        _SET_ESTADO_DOS( 13 );
    }
    if( _ESTADO_DOS == 13 ) {
        if( est == _x
				|| ( ( IMPRESORA_FISCAL == FISCAL_IBM_4610 || IMPRESORA_FISCAL == FISCAL_TMT88 )
						&& ( est == 55 || est == 8 ) ) ) {
            FISCAL_X();
        }
        _SET_ESTADO_DOS( 2 );
    }
    if( _ESTADO_DOS == 2 ) {
        LF( 1 );
        if( IMPRESORA_FISCAL == FISCAL_DITEMPO ) {
            PAUSA( 100 );
        }
        if( IMPRESORA_FISCAL == FISCAL_IBM_4694 || IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
            PAUSA( SEGUNDOS_ESPERA_EN_INFORME * 18 );
        }
        _SET_ESTADO_DOS( 4 );
    }
    if( _ESTADO_DOS == 4 ) {
        //--------------------------- Acumulacin --------------------------------
		for( h = 0; h < 10; h++ ) {
            tot_ventas += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_ventas_importe ), h );
            tot_retiros += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_retiros_importe ), h );
            tot_cobros += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_cobros_importe ), h );
            tot_fondo_f += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_fondo_fijo_multiple_importe ),
                                               h );
            dif[h] = ( medios[h].efectivo )
                   ? GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_tot_dinero ) )
                   : _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_ventas_importe ), h )
                   - _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_retiros_importe ), h )
                   + _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_cobros_importe ), h )
                   + _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_cambios_importe ), h );
            dif_me[h] = _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_ventas_importe_me ), h )
                      - _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_retiros_importe_me ), h );

            if( FONDO_FIJO_MULTIPLES_MEDIOS && !medios[h].efectivo ) {
                dif[h] += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_fondo_fijo_multiple_importe )
												, h );
                dif_me[h] += _GET_MEMORY_DOUBLE( EST_VAR( ini_est
                                                        + o_x_fondo_fijo_multiple_imp_me ), h );
            }
            if( _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_cambios_importe ), h ) > 0 ) {
                tot_cambios += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_cambios_importe ), h );
            }

            gran_total += dif[h];
        }
        gran_total += GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_diferencia_redondeo ) );
        // Para el Salvador la X e Y son las del sistema, hardcodeadas.. Solo la Z es configurable
		// Para el Salvador se condiciona desde tkt que no imprima ningun informe de la funcion IMPRIMIR_TKT_INF00.
		// Para Nacionales es necesario que pase porque si tiene informe Y para imprimir.
		if( TKT_CONFIGURABLE && ( est == _z || est ==_x /*&& NRO_MODO != 'Y'*/  || est ==_z_ant ) ) {
		//if( TKT_CONFIGURABLE && ( est == _z || ( est ==_x && NRO_MODO != 'Y' ) ) ) {
            if( MODO_NEGOCIO == SERVICIOS ) {
                setData( ACTION_BIG_LETTER, NULL, 0 );
                setData( ACTION_SET_POSITION, "80#20", 0 );
                MOSTRAR_CADENA( 1, PRN, "SWISS MEDICAL GROUP" );
                setData( ACTION_NORMAL_LETTER, NULL, 0 );
            }
			if( IMPRIMIR_TKT_INF00( ini_est, dif, dif_me ) ) {
				int donde = NRO_RECUPERO_Z;
				setData( ACTION_DOPRINT, NULL, 0 );
                BORRAR_MENSAJE();
            }
			if( MODO_NEGOCIO == SERVICIOS ) {
	            if( IMPRIMIR_TKT_INFORME_SUPERVISOR() ) {
		            setData( ACTION_DOPRINT, NULL, 0 );
			    }
            }
			if( IMPRESORA_FISCAL != FISCAL )
				setData( ACTION_PRINT, NULL, 0 );
		}
		if( EMITIR_INFORME_DIFERENCIAS )
		{
			if( tot_ventas == 0 ) {
				tot_ventas = 0.00001;
			}
			if( tot_retiros == 0 ) {
				tot_retiros = 0.00001;
			}
			if( tot_cobros == 0 ) {
				tot_cobros = 0.00001;
			}
		}
		else 
		{
			if( NRO_MODO == 'Y' || NRO_MODO =='X' ) 
			{
				if( TKT_CONFIGURABLE ) 
					_SET_ESTADO_DOS( 7 );
				else 
					_SET_ESTADO_DOS( 6 );
			}
			else
				return;
		}
	}

	if( _ESTADO_DOS == 6 ) {
        //Se recupera archivo REC para tomar lo declarado por el cajero
        if( RECUPERAR_ARCHIVO_REC( ARCH_REC_CIERRE, ( char* )&datosCierre, 
				sizeof( struct _datosCierre ), 0 ) == 0 ) {
            for( h = 0; h < 10; h++ ) {
                cierre_total += datosCierre.cierre[h];
            }
        } else {
            for( h = 0; h < 10; h++ ) {
                datosCierre.cierre[h] = 0.0;
                datosCierre.cierre_me[h] = 0.0;
            }
            cierre_total = 0.0;
        }
        //---------------------------- Impresin REPORTE DE ARQUEO-----------------------------

        if( MODO_NEGOCIO != SERVICIOS ) {
            //IMPRIMIR_USUARIO_Y_SUPERVISOR();
        }
        else {
            /*PRN_SELECT( SLIP );
            setData(ACTION_BIG_LETTER, NULL,0);
            setData(ACTION_SET_POSITION, "10#10",0);
            MOSTRAR_CADENA(  1, PRN, "SWISS MEDICAL GROUP" );
            */
            setData( ACTION_NORMAL_LETTER, NULL, 0 );
            setData( ACTION_SET_POSITION, "40#40", 0 );
            print_pos = 45;
        }
        //INFORME_00_CABECERA( ( char )anterior, cierre );
		PRN_SELECT( JOURNAL_RECEIPT );
		if( NRO_MODO == 'Y' )
		{
			memset(cadena_aux,0, sizeof(cadena_aux));
			_snprintf(cadena_aux, sizeof(cadena_aux)-1,"Cierre de Caja");
			PRN_LETRA_GRANDE();
			MOSTRAR_CADENA(4, PRN, cadena_aux );
			LF(1);
			memset(cadena_aux,0, sizeof(cadena_aux));
			_snprintf(cadena_aux, sizeof(cadena_aux)-1,"Y%08ld", NRO_Y);
			MOSTRAR_CADENA(5, PRN, cadena_aux );
			PRN_LETRA_NORMAL();
			LF(2);

			MOSTRAR_CADENA(1, PRN, "----------------------------------------" );
			LF(1);
			MOSTRAR_CADENA(1, PRN, "  FECHA    HORA     SUC    CAJA   CAJERO" );
			LF(1);
			MOSTRAR_FECHA_2000( 1, PRN, GET_FECHA_DOS() );
			MOSTRAR_HORA( 12, PRN, GET_HORA_DOS() );
			MOSTRAR_ENTERO( 21, PRN, "000", config.sucursal );
			//MOSTRAR_ENTERO( 29, PRN, "999", NRO_CAJA );
            MOSTRAR_ENTERO(28, PRN,( USAR_NRO_CAJA_DGI )?"0000":"000", NUMERO_CAJA( ) );
			MOSTRAR_ENTERO( 37, PRN, "0000", NRO_CAJERO );
			LF(1);
			MOSTRAR_CADENA(1, PRN, "SEQ: #" );
			MOSTRAR_ENTERO( 7, PRN, ( config.pais != ECUADOR ) ? "00000000" : "0000000",	NRO_EVENTO );
			MOSTRAR_CADENA(16, PRN, "REF: " );
			//MOSTRAR_ENTERO(16, PRN, "0000", NRO_CAJA_DGI );
			MOSTRAR_ENTERO(19, PRN,( !USAR_NRO_CAJA_DGI )? "0000":"000", 
					( !USAR_NRO_CAJA_DGI ) ? NRO_CAJA_DGI : NRO_CAJA );
			LF(1);
			MOSTRAR_CADENA(1, PRN, "----------------------------------------" );
		}
		if( NRO_MODO =='X' )
		{
			if( IMPRESORA_FISCAL == FISCAL )  //mmancuso agregado
			   ABRIR_DOCUMENTO_NO_FISCAL('1');

			memset(cadena_aux,0, sizeof(cadena_aux));
			_snprintf(cadena_aux, sizeof(cadena_aux)-1,"Arqueo de Cajero");
			PRN_LETRA_GRANDE();
			MOSTRAR_CADENA(2, PRN, cadena_aux );
			LF(1);
			memset(cadena_aux,0, sizeof(cadena_aux));
			_snprintf(cadena_aux, sizeof(cadena_aux)-1,"( Cierre X )");
			MOSTRAR_CADENA(5, PRN, cadena_aux );
			PRN_LETRA_NORMAL();
            LF(2);
			MOSTRAR_FECHA_2000( 1, PRN, GET_FECHA_DOS() );
			MOSTRAR_HORA( 21, PRN, GET_HORA_DOS() );
		}
		LF(2);

		if( IMPRESORA_FISCAL || config.pais == CHILE || config.pais == BRASIL ) {
            INFORME_00_FISCAL( tot_ventas, tot_retiros, tot_cobros, cant_tickets, gran_total, dif,
                               ini_est );
        }
		else {
			/*if( TKT_CONFIGURABLE &&( IMPRIMIR_TKT_INF_Y( ini_est, GET_INICIO_INFORME_Y(), 
					GET_FIN_INFORME_Y( ) ) ) ) {
				setData( ACTION_DOPRINT, NULL, 0 );
				BORRAR_MENSAJE();
			}
			else {*/
				INF00_DISCRIMINAR( ST( S_VENTAS ), tot_ventas, o_x_ventas_importe, o_x_ventas_cantidad,
					ini_est, o_x_ventas_importe_me );
				LF( 1 );
				/*INF00_DISCRIMINAR( ST( S_RETIROS ), tot_retiros, o_x_retiros_importe,
								o_x_retiros_cantidad, ini_est, o_x_retiros_importe_me );*/
				INF00_DISCRIMINAR_RETIROS( tot_retiros, o_x_retiros_importe,
					o_x_retiros_cantidad, ini_est, o_x_retiros_importe_me,
					datosCierre, cierre_total );
				LF( 1 );
				INF00_DISCRIMINAR( ST( S_COBROS ), tot_cobros, o_x_cobros_importe, o_x_cobros_cantidad,
								   ini_est, -1 );
				LF( 1 );
				if( FONDO_FIJO_MULTIPLES_MEDIOS ) {
					INF00_DISCRIMINAR( ST( S_FONDO_FIJO ), tot_fondo_f, o_x_fondo_fijo_multiple_importe,
									   o_x_fondo_fijo_multiple_cantidad, ini_est,
									   o_x_fondo_fijo_multiple_imp_me );
					LF( 1 );
				}
				INF00_DISCRIMINAR( ST( S_CAMBIOS ), tot_cambios, o_x_cambios_importe,
								   o_x_cambios_cantidad, ini_est, -1 );  
				LF( 1 );
				if( DONACION_DE_VUELTO ) {
					 int  area_ant, tipo_ant;
	 
					 area_ant = SELECTED2();
					tipo_ant = SELECTED_TIPO();
					aux = GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_donaciones_importe ) );
					INF00_ITEM( ST( S_DONACIONES ), aux ? aux : -1, o_x_donaciones_importe,
								o_x_donaciones_cantidad, ini_est );
					if( est == _z ) {
						 int  area_ant, tipo_ant;
						area_ant = SELECTED2();
						tipo_ant = SELECTED_TIPO();
						codigo = RAM_INSTITUCION_BENEFICIARIA;
						//USE_DB( AREA_AUX, _INSTIT, ( char* )&instituciones, sizeof( instituciones ),
						//        NULL, 0, 0 );
						OPEN_TABLE( T_INSTITUT, TT_ORIG, ( char* )&instituciones, sizeof( instituciones ) );
						//SET_ORDER( 1 );
						//GET_EQUAL( ( char* )&codigo );
						SELECT_TABLE( T_INSTITUT, TT_ORIG );
						SET_WHERE("COD_INSTITUCION = '%d'", codigo );
						RUN_QUERY(NO);
						if( FOUND2() ) {
							instituciones.desc_institucion[20] = 0;
						}
						else {
							strncpy( instituciones.desc_institucion, ST( S_DESCONOCIDA ),
									 sizeof( instituciones.desc_institucion ) );
						}
						sprintf( mensaje, "(%s)", instituciones.desc_institucion );
						MOSTRAR_CADENA( 1, PRN, mensaje );
						LF( 2 );
						//CLOSE_DB( _INSTIT );
						CLOSE_TABLE( T_INSTITUT, TT_ORIG );
					}
					//SELECT_TABLE( area_ant, tipo_ant );
				}
				if( config.pais == ELSALVADOR ) {
					INF00_ITEM( "ANULACIONES TICKET", -1, o_x_imp_cancelados, o_x_cant_cancelados,
								ini_est );
					LF( 1 );
				}
				INF00_ITEM( ST( S_ADELANTOS ), tot_ventas, o_x_adelantos_importe,
							o_x_adelantos_cantidad, ini_est );
				INF00_ITEM( ST( S_INTERESES ), tot_ventas, o_x_intereses_importe,
							o_x_intereses_cantidad, ini_est );
				if( !FONDO_FIJO_MULTIPLES_MEDIOS ) {
					INF00_ITEM( ST( S_FONDO_FIJO ), tot_ventas, o_x_fondo_fijo_importe,
								o_x_fondo_fijo_cantidad, ini_est );
				}
				INF00_ITEM( ST( S_RET__F__FIJO ), tot_ventas, o_x_retiro_fondo_importe,
							o_x_retiro_fondo_cantidad, ini_est );
				INF00_ITEM( ST( S_ANULACIONES ), tot_ventas, o_x_anulaciones_importe,
							o_x_anulaciones_cantidad, ini_est );
				INF00_ITEM( ST( S_CORRECCIONES ), tot_ventas, o_x_correccion_importe,
							o_x_correccion_cantidad, ini_est );
				INF00_ITEM( ST( S_DEVOLUCIONES ), tot_ventas, o_x_devoluciones_importe,
							o_x_devoluciones_cantidad, ini_est );
				INF00_ITEM( ST( S_DEV_ENVASES ), tot_ventas, o_x_dev_envases_importe,
							o_x_dev_envases_cantidad, ini_est );
				INF00_ITEM( ST( S_PAGOS_X_CAJA ), tot_ventas, o_x_pagos_por_caja_importe,
							o_x_pagos_por_caja_cantidad, ini_est );
				INF00_ITEM( ST( S_VUELTOS ), tot_ventas, o_x_vueltos_importe, o_x_vueltos_cantidad,
							ini_est );
				INF00_ITEM( ST( S_REDONDEO ), -1, o_x_diferencia_redondeo, -1, ini_est );
				LF( 1 );

				INFORME_00_CUERPO( ST( S_TOTAL_DE_CAJA ), gran_total-cierre_total, -1, -1 );
				for( h = 0;h < 9;h++ ) {
					if( fabs( dif[h]-datosCierre.cierre[h] ) > 0.00001 ) {
						INFORME_00_CUERPO( medios[h].nombre, dif[h]-datosCierre.cierre[h], -1, -1 );
						if( fabs( dif_me[h]-datosCierre.cierre_me[h] ) > 0.00001 ) {
							MOSTRAR_CADENA( 3, PRN, "[" );
							MOSTRAR_CADENA( 5, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
							MOSTRAR_FLOTANTE( 9, PRN, ENTEROS, DECIMALES, SI, SI, dif_me[h]-datosCierre.cierre_me[h] );
							MOSTRAR_CADENA( 20, PRN, "]" );
							LF( 1 );
						}
					}
				}
				LF( 1 );
				//-------------------------- Detalle IVA ---------------------------------
				if( RECALCULAR_IVA_EN_INF_00 ) {
					aux = tot_ventas - GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_ventas_exentas ) )
						- GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_imp_int ) );
					aux -= ( GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_facturas_a_importe ) )
						   + GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_facturas_b_importe ) ) );
					aux -= ( GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_nc_a_importe ) )
						   + GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_nc_b_importe ) ) );
					aux_gravado = aux / ( 1 + config.porc_iva1 / 100 );
					aux_iva1 = aux - aux_gravado;
				}
				else {
					aux_gravado = GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_ventas_gravadas ) );
					// iva con MAX_COD_IVA
					aux_iva1 = 0;
					for( h = 0;h < MAX_COD_IVA;h++ ) {
						aux_iva1 += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_importe_iva ), h );
					}
				}
				if( MODO_NEGOCIO == SERVICIOS ) {
					sprintf( cadaux, "%d#%d", 40, print_pos );
					setData( ACTION_SET_POSITION, cadaux, 0 );
					print_pos = print_pos + 3;
				}
				_INF00_ITEM( ST( S_VENTAS_GRAVADAS ), tot_ventas, aux_gravado, -1, ini_est );
				INF00_ITEM( ST( S_VENTAS_EXENTAS ), tot_ventas, o_x_ventas_exentas, -1, ini_est );
				INF00_ITEM( ST( S_IMP__INTERNOS ), tot_ventas, o_x_imp_int, -1, ini_est );
				INF00_DISCRIMINAR_IVA( tot_ventas, ini_est );
						INFORME_00_CUERPO( ST( S_SERVICIOS_COBRADOS ), 
							GET_MEMORY_DOUBLE( EST_VAR( ini_est+ o_x_cobros_importes_servicios ) ) ,
							-1,
							GET_MEMORY_INT( EST_VAR( ini_est+ o_x_cobros_cantidad_servicios ) ) );

				if( MODO_NEGOCIO == SERVICIOS ) {
					sprintf( cadaux, "%d#%d", 40, print_pos );
					setData( ACTION_SET_POSITION, cadaux, 0 );
				}
				MOSTRAR_CADENA( 1, PRN, ST( S_NRO_DE_TICKETS ) );
				if( MODO_NEGOCIO == SERVICIOS ) {
					sprintf( cadaux, "%d#%d", 80, print_pos );
					setData( ACTION_SET_POSITION, cadaux, 0 );
					print_pos = print_pos + 3;
				}
				MOSTRAR_ENTERO( 35, PRN, "9999",
								GET_MEMORY_LONG( EST_VAR( ini_est + o_x_cantidad_tickets ) ) );
				LF( 1 );
				LF( 1 );
				INF00_ITEM( "DIFERENCIAS", tot_ventas, o_x_diferencia, -1, ini_est );  //_MEC_ SACO REDONDEO
				if( FACTURACION_HABILITADA ) {
					if( TIPO_FACTURA_A == LOCAL_FAC ) {
						INF00_ITEM( ST( S_FACTURAS_A ), tot_ventas, o_x_facturas_a_importe,
									o_x_facturas_a_cantidad, ini_est );
						INF00_ITEM( "FAC A GRAV", tot_ventas, o_x_factura_a_importe_gravado,
									o_x_facturas_a_cantidad, ini_est );                    
						INF00_ITEM( "FAC A IVA ", tot_ventas, o_x_iva_factura_a,
									o_x_facturas_a_cantidad, ini_est );
					}
					if( TIPO_FACTURA_B == LOCAL_FAC ) {
						INF00_ITEM( ST( S_FACTURAS_B ), tot_ventas, o_x_facturas_b_importe,
									o_x_facturas_b_cantidad, ini_est );
					}
					if( TIPO_NC_A == LOCAL_FAC ) {
						INF00_ITEM( ST( S_NC_A ), tot_ventas, o_x_nc_a_importe, o_x_nc_a_cantidad,
									ini_est );
						INF00_ITEM( "NC A GRAV", tot_ventas, o_x_nc_a_importe_gravado,
											o_x_nc_a_cantidad, ini_est );
					}
					if( TIPO_NC_B == LOCAL_FAC ) {
						INF00_ITEM( ST( S_NC_B ), tot_ventas, o_x_nc_b_importe, o_x_nc_b_cantidad,
									ini_est );
					}
					if( config.pais != COLOMBIA ) {
						INFORME_00_CUERPO( ST( S_PERCEPCION_DGI ),
										   GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_percepcion_dgi ) ),
										   -1, -1 );
						INFORME_00_CUERPO( ST( S_RETENCION_DGR ),
										   GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_retencion_dgr ) ),
										   -1, -1 );
						INFORME_00_CUERPO( ST( S_PERCEP_212_IVA1 ),
										   GET_MEMORY_DOUBLE( EST_VAR( ini_est
																	 + o_x_percepcion_212_iva1 ) ), -1,
										   -1 );
						INFORME_00_CUERPO( ST( S_PERCEP_212_IVA2 ),
										   GET_MEMORY_DOUBLE( EST_VAR( ini_est
																	 + o_x_percepcion_212_iva2 ) ), -1,
										   -1 );
					}
				}
				if( NRO_MODO == 'Z' ) {
					INFORME_00_CUERPO( ST( S_TICKETS ), ( double )cant_tickets, -1, -1 );
				}
				//--------------------- Gran total ------------------------
				if( est == _z ) {
					INFORME_00_CUERPO( ST( S_GRAN_TOTAL_ANT ), NRO_GRAN_TOTAL, -1, -1 );
					INFORME_00_CUERPO( ST( S_GRAN_TOTAL____ ), NRO_GRAN_TOTAL + tot_ventas, -1, -1 );
				}
				else if( est == _z_ant ) {
					INFORME_00_CUERPO( ST( S_GRAN_TOTAL_ANT ), NRO_GRAN_TOTAL - tot_ventas, -1, -1 );
					INFORME_00_CUERPO( ST( S_GRAN_TOTAL____ ), NRO_GRAN_TOTAL, -1, -1 );
				}
				//----------------------- Promociones ----------------------
				if( NRO_TOTAL_BONOS ) {
					INFORME_00_CUERPO( ST( S_BONOS_ENTREGADOS ),
									   ( double )GET_MEMORY_UNSIGNED( EST_VAR( ini_est
																			 + o_x_bonos_entregados ) ),
									   -1, -1 );
					INFORME_00_CUERPO( ST( S_TOTAL_DE_BONOS ), ( double )( NRO_TOTAL_BONOS ), -1, -1 );
				}
				IMPRIME_DIA_OPERATIVO( anterior );
			//}
            CLOSE_COPIA_PRN();
            if( NRO_MODO == 'Y' && IMPRIMIR_PIE_Y ) {
                LF( 2 );
                IMPRIME_PIE_Y();
            }
            INFORME_00_PIE();
		}
        _SET_ESTADO_DOS( 3 );
    }

	if( _ESTADO_DOS == 7 ) {
		if( /*TKT_CONFIGURABLE 
				&& */( IMPRIMIR_TKT_INF_Y( ini_est, GET_INICIO_INFORME_Y(), GET_FIN_INFORME_Y( ) ) ) ) {
			setData( ACTION_DOPRINT, NULL, 0 );
			BORRAR_MENSAJE();
		}
			
	}

    if( _ESTADO_DOS == 3 ) {
        BORRAR_MENSAJE();
    }
	
    CLOSE_RECUPERO( _ptr, _handle );

    if( MODO_NEGOCIO == SERVICIOS ) {
        print_pos = 0;
    }
}
/*****************************************************************************/
void INFORME_00_FISCAL( double tot_ventas, double tot_retiros, double tot_cobros, long cant_tickets,
                        double gran_total, double *dif, int ini_est )
/*****************************************************************************/
{
    double tot_retiros_caja,aux_donaciones;
    int h,porc,codigo;
    struct institut_donacion instituciones;
    char mensaje[60];
    /*---------------------------- Impresin ---------------------------------*/
    INFORME_00_CUERPO( ST( S_VENTAS_X_BOLETAS ),
                       tot_ventas - GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_facturas_a_importe ) )
                     - GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_nc_a_importe ) ), -1, -1 );
    INFORME_00_CUERPO( ST( S_VENTAS_X_FACTURAS ),
                       GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_facturas_a_importe ) ), -1, -1 );
    INFORME_00_CUERPO( ST( S_NOTAS_DE_CREDITO ),
                       GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_nc_a_importe ) ), -1, -1 );
    INF00_DISCRIMINAR( NULL, tot_ventas, o_x_ventas_importe, o_x_ventas_cantidad, ini_est, -1 );
    INFORME_00_SEPARADOR();
    if( IMPRESORA_FISCAL == FISCAL_POS745x || IMPRESORA_FISCAL == FISCAL_IBM_4694
     || IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
        INF00_ITEM( ST( S_ANULACIONES ), tot_ventas, o_x_anulaciones_importe,
                    o_x_anulaciones_cantidad, ini_est );
        INFORME_00_SEPARADOR();
    }
    if( DONACION_DE_VUELTO ) {
		 int  area_ant, tipo_ant;
 
		area_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();
        
		aux_donaciones = GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_donaciones_importe ) );
        INF00_ITEM( ST( S_DONACIONES ), aux_donaciones ? aux_donaciones : -1,
                    o_x_donaciones_importe, o_x_donaciones_cantidad, ini_est );
        codigo = RAM_INSTITUCION_BENEFICIARIA;
        //USE_DB( AREA_AUX, _INSTIT, ( char* )&instituciones, sizeof( instituciones ), NULL, 0, 0 );
        //SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&codigo );
		OPEN_TABLE( T_INSTITUT, TT_ORIG, ( char* )&instituciones, sizeof( instituciones ) );
		//SET_ORDER( 1 );
		//GET_EQUAL( ( char* )&codigo );
		SELECT_TABLE( T_INSTITUT, TT_ORIG );
		SET_WHERE("COD_INSTITUCION = '%d'", codigo );
		RUN_QUERY(NO);	

        if( FOUND2() ) {
            instituciones.desc_institucion[20] = 0;
        }
        else {
            strncpy( instituciones.desc_institucion, ST( S_DESCONOCIDA ),
                     sizeof( instituciones.desc_institucion ) );
        }
        sprintf( mensaje, "(%s)", instituciones.desc_institucion );
        MOSTRAR_CADENA( 1, PRN, mensaje );
        LF( 2 );
        //CLOSE_DB( _INSTIT );
		CLOSE_TABLE( T_INSTITUT, TT_ORIG );
        INFORME_00_SEPARADOR();
		SELECT_TABLE( area_ant, tipo_ant );
    }
    INFORME_00_CUERPO( ST( S_BOLETAS_EMITIDAS ), ( double )cant_tickets, -1, -1 );
    if( FACTURACION_HABILITADA ) {
        INFORME_00_CUERPO( ST( S_FACTURAS_EMITIDAS ),
                           ( double )GET_MEMORY_INT( EST_VAR( ini_est + o_x_facturas_a_cantidad ) ),
                           -1, -1 );
        INFORME_00_CUERPO( ST( S_N_C______EMITIDAS ),
                           ( double )GET_MEMORY_INT( EST_VAR( ini_est + o_x_nc_a_cantidad ) ), -1,
                           -1 );
    }
    INFORME_00_SEPARADOR();
    INF00_ITEM( ST( S_FONDO_FIJO_ ), -1, o_x_fondo_fijo_importe, o_x_fondo_fijo_cantidad, ini_est );
    INF00_ITEM( ST( S_RETIRO_F__FIJO ), -1, o_x_retiro_fondo_importe, o_x_retiro_fondo_cantidad,
                ini_est );
    INFORME_00_SEPARADOR();
    tot_retiros_caja = tot_retiros + GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_adelantos_importe ) )
                     + GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_pagos_por_caja_importe ) )
                     + GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_devoluciones_importe ) );
    INFORME_00_CUERPO( ST( S_RETIROS_DE_CAJA ), tot_retiros_caja, -1, -1 );
    porc = ( int )( tot_retiros / tot_retiros_caja * 100.0 );
    if( tot_retiros_caja < 0.01 ) {
        porc = 0;
    }
    INFORME_00_CUERPO( ST( S_RETIROS ), tot_retiros, porc, -1 );
    if( DISCRIMINAR_RETIROS ) {
        INF00_DISCRIMINAR( NULL, tot_retiros, o_x_retiros_importe, o_x_retiros_cantidad, ini_est,
                           -1 );
    }
    INF00_ITEM( ST( S_ADELANTO_SUELDOS ), tot_retiros_caja, o_x_adelantos_importe,
                o_x_adelantos_cantidad, ini_est );
    INF00_ITEM( ST( S_PAGOS_X_CAJA ), tot_retiros_caja, o_x_pagos_por_caja_importe,
                o_x_pagos_por_caja_cantidad, ini_est );
    if( IMPRESORA_FISCAL == FISCAL_DITEMPO ) {
        INF00_ITEM( ST( S_NOTAS_DE_CREDITO ), tot_retiros_caja, o_x_devoluciones_importe,
                    o_x_devoluciones_cantidad, ini_est );
    }
    INFORME_00_SEPARADOR();
    if( IMPRESORA_FISCAL == FISCAL_DITEMPO ) {
        INFORME_00_CUERPO( ST( S_OTROS_INGRESOS ), tot_cobros, -1, -1 );
        porc = ( fabs( tot_cobros ) > 0.001 ) ? 100 : 0;
        INFORME_00_CUERPO( ST( S_INGRESOS_DE_3_ ), tot_cobros, porc, -1 );
        if( DISCRIMINAR_COBROS ) {
            INF00_DISCRIMINAR( NULL, tot_cobros, o_x_cobros_importe, o_x_cobros_cantidad, ini_est,
                               -1 );
        }
        INFORME_00_SEPARADOR();
    }
    INFORME_00_CUERPO( ST( S_TOTAL_DE_CAJA ), gran_total, -1, -1 );
    for( h = 0;h < 9;h++ ) {
        if( dif[h] ) {
            INFORME_00_CUERPO( medios[h].nombre, dif[h], -1, -1 );
        }
    }
    INFORME_00_SEPARADOR();
    CLOSE_COPIA_PRN();
    if( NRO_MODO == 'Y' && IMPRIMIR_PIE_Y ) {
        LF( 2 );
        IMPRIME_PIE_Y();
    }
    INFORME_00_PIE();
    if( IMPRESORA_FISCAL != FISCAL_TMT88 && IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
        PRN_SELECT( JOURNAL );
        IMPRIME_COPIA_PRN();
        PAUSA( 72 );
    }
    PRN_SELECT( RECEIPT );
    BORRAR_MENSAJE();
}
/*****************************************************************************/
void INFORME_00_CABECERA( char anterior, char cierre )
/*****************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        IMPRIME_CABECERA_TICKET( SI );
    }
    else {
        PRN_SELECT( SLIP );
    }
    OPEN_COPIA_PRN();
    MOSTRAR_CADENA( 15, PRN, ST( S_INFORME_00 ) );
    if( cierre ) {
        LF( 1 );
        MOSTRAR_CADENA( 13, PRN, ST( S_CIERRE_DE_CAJA ) );
    }
    IMPRIME_NRO_INFORME( anterior );
    if( IMPRESORA_FISCAL ) {
        LF( 1 );
    }
    else {
        LF_RECEIPT( 1 );
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void INFORME_00_CUERPO( char *nombre, double importe, int porcentaje, int cantidad )
/*****************************************************************************/
{
    static int imprimir = 0;
	char caracter_porc[2];
	caracter_porc[0]='%';
	caracter_porc[1]='\0';

    if( MODO_NEGOCIO == SERVICIOS ) {
        sprintf( cadaux, "%d#%d", 40, print_pos );
        setData( ACTION_SET_POSITION, cadaux, 0 );
    }
    MOSTRAR_CADENA( 1, PRN, nombre );
    if( MODO_NEGOCIO == SERVICIOS ) {
        sprintf( cadaux, "%d#%d", 80, print_pos );
        setData( ACTION_SET_POSITION, cadaux, 0 );
    }
    MOSTRAR_FLOTANTE( ( config.pais == COLOMBIA ) ? 17 : 18, PRN, ENTEROS + 4, DECIMALES, 1,
                      ( config.pais == COLOMBIA ) ? 1 : 0, importe );
    if( porcentaje >= 0 ) {
        MOSTRAR_ENTERO( 30, PRN, "999", porcentaje );
        MOSTRAR_CADENA( 33, PRN, caracter_porc );
    }
    if( cantidad >= 0 ) {
        MOSTRAR_ENTERO( 35, PRN, "9999", cantidad );
    }
    LF( 1 );
    #if defined(INVEL_W) || defined(INVEL_L)
    if( imprimir >= 10 ) {
        setData( ACTION_PRINT, NULL, 0 );
        imprimir = 0;
    }
    else {
        imprimir++;
    }
    #endif
    if( MODO_NEGOCIO == SERVICIOS ) {
        print_pos = print_pos + 3;
    }
}
/*****************************************************************************/
void INFORME_00_PIE()
/*****************************************************************************/
{
    PRN_FIN_TICKET();
    if( IMPRESORA_FISCAL ) {
        LF( 2 );
    }
    FISCAL_FIN_INFORME();
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void INFORME_00_SEPARADOR()
/*****************************************************************************/
{
    MOSTRAR_CADENA( 1, PRN, "----------------------------------------" );
    LF( 1 );
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void INF00_DISCRIMINAR( char *titulo, double tot, int cod_imp, int cod_can, int ini_est,
                        int cod_imp_me )
/*****************************************************************************/
{
    int h;
    double aux,porc,aux_me;
    if( titulo ) {
        INFORME_00_CUERPO( titulo, tot, -1, -1 );
    }
    for( h = 0;h < 10;h++ ) {
        aux = _GET_MEMORY_DOUBLE( EST_VAR( ini_est + cod_imp ), h );
        if( aux ) {
            porc = aux / tot * 100.0;
            INFORME_00_CUERPO( medios[h].nombre, aux, ( int )porc,
                               _GET_MEMORY_INT( EST_VAR( ini_est + cod_can ), h ) );
        }
        aux_me = ( cod_imp_me != -1 ) ? _GET_MEMORY_DOUBLE( EST_VAR( ini_est + cod_imp_me ), h )
               : 0.0;
        if( aux_me ) {
            MOSTRAR_CADENA( 3, PRN, "[" );
            MOSTRAR_CADENA( 5, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
            MOSTRAR_FLOTANTE( 9, PRN, ENTEROS, DECIMALES, SI, SI, aux_me );
            MOSTRAR_CADENA( 20, PRN, "]" );
			LF(1);
        }
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}

/*****************************************************************************/
void INF00_DISCRIMINAR_RETIROS( double tot, int cod_imp, int cod_can, int ini_est,
                        int cod_imp_me, struct _datosCierre datosCierre, double tot_cierre )
/*****************************************************************************/
{
    int h;
    double aux,porc,aux_me;
    char *titulo = ST( S_RETIROS );
    
    INFORME_00_CUERPO( titulo, tot + tot_cierre, -1, -1 );
	
    for( h = 0;h < 10;h++ ) {
        aux = _GET_MEMORY_DOUBLE( EST_VAR( ini_est + cod_imp ), h ) + datosCierre.cierre[h];
        if( aux ) {
            porc = aux / (tot + tot_cierre) * 100.0;
            INFORME_00_CUERPO( medios[h].nombre, aux, ( int )porc,
                               _GET_MEMORY_INT( EST_VAR( ini_est + cod_can ), h ) );
        }
        aux_me = ( cod_imp_me != -1 ) ? _GET_MEMORY_DOUBLE( EST_VAR( ini_est + cod_imp_me ), h ) + datosCierre.cierre_me[h]
               : 0.0;
        if( aux_me ) {
            MOSTRAR_CADENA( 3, PRN, "[" );
            MOSTRAR_CADENA( 5, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
            MOSTRAR_FLOTANTE( 9, PRN, ENTEROS, DECIMALES, SI, SI, aux_me );
            MOSTRAR_CADENA( 20, PRN, "]" );
			LF(1);
        }
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}


/*****************************************************************************/
void INF00_DISCRIMINAR_IVA( double tot_venta, int ini_est )
/*****************************************************************************/
{
    int h;
    double aux = 0;
    char desc_iva[15] = {"00.00%"};
    char desc[30];
    if( MODO_NEGOCIO == SERVICIOS ) {
        sprintf( cadaux, "%d#%d", 40, print_pos );
        setData( ACTION_SET_POSITION, cadaux, 0 );
        print_pos = print_pos + 3;
    }
    for( h = 0;h < MAX_COD_IVA;h++ ) {
        aux += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_importe_iva ), h );
        aux += _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_importe_sobretasa_iva ), h );
    }
    LF( 1 );
    if( config.pais != COLOMBIA ) {
        _INF00_ITEM( ST( S_TOTAL_IVA_ ), tot_venta, aux, -1, -1 );
        for( h = 0;h < MAX_COD_IVA;h++ ) {
            memset( desc, 32, 30 );
            desc[17] = 0;
            memcpy( desc, iva[h].nombre_iva, strlen( iva[h].nombre_iva ) );
            aux = _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_importe_iva ), h );
            if( iva[h].porc_iva1 || aux ) {
                FTOA( iva[h].porc_iva1, desc_iva, 2, 2, 0, 0 );
                memcpy( &desc[11], desc_iva, 6 );
                INFORME_00_CUERPO( desc, aux, -1, -1 );
            }
            aux = _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_importe_sobretasa_iva ), h );
            if( iva[h].porc_iva2 || aux ) {
                FTOA( iva[h].porc_iva2, desc_iva, 2, 2, 0, 0 );
                memcpy( &desc[11], desc_iva, 6 );
                INFORME_00_CUERPO( desc, aux, -1, -1 );
            }
        }
        LF( 1 );
    }
}
/*****************************************************************************/
void INF00_ITEM( char *titulo, double tot, int cod_imp, int cod_can, int ini_est )
/*****************************************************************************/
{
    double importe;
    importe = GET_MEMORY_DOUBLE( EST_VAR( ini_est + cod_imp ) );
    if( ( config.pais != COLOMBIA ) || ( config.pais == COLOMBIA && importe ) ) {
        _INF00_ITEM( titulo, tot, importe, cod_can, ini_est );
    }
}
/*****************************************************************************/
void _INF00_ITEM( char *titulo, double tot, double importe, int cod_can, int ini_est )
/*****************************************************************************/
{
    double porc;
    int cantidad;
    porc = ( tot == -1 ) ? -1 : importe / tot * 100.0;
    cantidad = ( cod_can == -1 ) ? -1 : GET_MEMORY_INT( EST_VAR( ini_est + cod_can ) );
    INFORME_00_CUERPO( titulo, importe, ( int )porc, cantidad );
}
/**************************************************************************/
void IMPRIME_INFORMES_Y_EN_Z( int z )
/**************************************************************************/
{
    switch( z ) {
        case _z:
            _IMPRIME_INFORMES_Y_EN_Z( NO );
            break;
        case _z_ant:
            // cerrar eventos y abrir eventos remoto caja
            CERRAR_ARCHIVOS_EVENTOS_LOCAL();
            if( !ABRIR_ARCHIVOS_EVENTOS_BACKUP() ) {
                _IMPRIME_INFORMES_Y_EN_Z( SI );
                CERRAR_ARCHIVOS_EVENTOS_LOCAL();
            }
            ABRIR_ARCHIVOS_EVENTOS_LOCAL();
            // cerrar eventos remoto caja y abrir eventos
            break;
    }
}
/**************************************************************************/
int _IMPRIME_INFORMES_Y_EN_Z( int anterior )
/**************************************************************************/
{
    int rta = 0;
    unsigned cajero,_cajero;
    long id_evento;
    struct
    {
        int tipo_evento;
        int cajero;
        int nro_ticket;
    } ind2_ticket;
    SET_MEMORY_CHAR( __varios_modo_y_en_z, 1 );
    SET_MEMORY_CHAR( __nro_modo, 'Y' );
    cajero = NRO_CAJERO;
    //SELECT( AREA_TICKET );
    //SET_ORDER( 2 );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    memset( &ind2_ticket, 0, sizeof( ind2_ticket ) );
    ind2_ticket.tipo_evento = 9;
    //GET_GREATER_E( ( char* )&ind2_ticket );
	/*SET_WHERE("CONCAT(TIPO_EVENTO,CAJERO,NRO_TICKET) >= CONCAT(%i,%i,%i)",
					ind2_ticket.tipo_evento,
					ind2_ticket.cajero,
					ind2_ticket.nro_ticket);*/
    SET_WHERE( "TIPO_EVENTO >= '%i'", ind2_ticket.tipo_evento );
    RUN_QUERY( NO );
    while( !dbEOF() && event_ticket.tipo_evento == 9 ) {
        _cajero = event_ticket.cajero;
        CEREAR_STRUCT( _x );
        ACUMULAR_X();
        // if( INFORME_Y_EN_Z_POR_CAJERO ){
        SKIP2( 1 );
        while( !dbEOF() && event_ticket.tipo_evento == 9 && _cajero == event_ticket.cajero ) {
            ACUMULAR_X();
            SKIP2( 1 );
        }
        if( !dbEOF() ) {
            SKIP2( -1 );
        }
        // }
        MENSAJE_STRING( S_IMPRIMIENDO_Y );
        SET_MEMORY_INT( __nro_cajero, event_ticket.cajero );
        LEE_CAJERO( NRO_CAJERO, &datos_cajero_actual );
        INFORME_00( _x, SI, anterior );
        if( INF_TAR_HABILITADO == 1 ) {
            MENSAJE_STRING( S_IMPRIMIENDO_TARJETAS );
            id_evento = event_ticket.id_evento;
            INFORME_TARJETAS( _x );
            //SELECT( AREA_TICKET );
            //GET_EQUAL( ( char* )&id_evento );
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
			SET_WHERE("ID_EVENTO = '%ld'", id_evento);
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
        }
        SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
        SKIP2( 1 );
    }
    //SET_ORDER( 1 );
    CEREAR_STRUCT( _x );
    SET_MEMORY_INT( __nro_cajero, cajero );
    SET_MEMORY_CHAR( __varios_modo_y_en_z, 0 );
    SET_MEMORY_CHAR( __nro_modo, 'Z' );
    return ( rta );
}
/**************************************************************************/
void ACUMULAR_X()
/**************************************************************************/
{
    //SELECT( AREA_VARIOS );
    SELECT_TABLE( T_VARIOS, tipo_t_varios );
    //GET_EQUAL( ( char* )&event_ticket.id_evento );
    SET_WHERE( "ID_EVENTO = %ld", event_ticket.id_evento );
    SET_ORDER2( "ID_EVENTO" );
    RUN_QUERY( NO );
    //if( FOUND() ) {
    if( FOUND2() ){
        ADD_BUFFER_TO_STRUCT( event_varios->buffer + sizeof( struct _cab_xyz ), _x );
    }
    else {
        MENSAJE_STRING( S_INFORME_Y_NO_ENCONTRADO );
    }
    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
}
/**************************************************************************/
void IMPRIME_DIA_OPERATIVO( int anterior )
/**************************************************************************/
{
    char _dia_oper[20],men_dia_operativo[100];

	memset( _dia_oper, 0, sizeof( _dia_oper ) );
	if( anterior ) {
		DTOC( NRO_DIA_OPERATIVO_ULTIMA_Z, _dia_oper );
	}
	else {
		DTOC( NRO_DIA_OPERATIVO, _dia_oper );
	}
	memset(men_dia_operativo,0,sizeof(men_dia_operativo));
	_snprintf(men_dia_operativo, sizeof(men_dia_operativo)-1, "%s",ST( S_DIA_OPERATIVO___S ), _dia_oper );

    sprintf( men_dia_operativo, ST( S_DIA_OPERATIVO___S ), _dia_oper );
	if( IMPRESORA_FISCAL == FISCAL ) {
		IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, men_dia_operativo, SI );
	} else {
		MOSTRAR_CADENA( 1, PRN, men_dia_operativo );
	}
	LF( 1 );
}
/*****************************************************************************/
void INFORME_00_CAJERO( int est )
/*****************************************************************************/
{
    int handle;
    long l_buffer;
    char archivo[15],*buffer;
    //struct _rend_caj rend_caj;
    struct _ind
    {
        unsigned short cod_cajero;
        unsigned short caja;
        unsigned long nro_y;
    } ind;
	struct _diferencias {
		 BINARY status;			//0
		 long cod_cajero;		//1
		 char cod_medio;		//5
		 double diferencia;     //6
		 int cant_retiros;      //14
		 BINARY fin;
    } diferencias;

    /*struct _ind_def
    {
        long cod_cajero;
        char medio;
    } ind_def;*/

    l_buffer = SIZEOF_STRUCT( _x );
    buffer = (char *)MALLOC( l_buffer );
    CEREAR_STRUCT( est );
    COPY_STRUCT( est, _x );
    if( /*USE_DB( AREA_AUX, _REND_CAJ_SIC, ( char* )&rend_caj, sizeof( rend_caj ), NULL, 0, 0 )*/
			OPEN_TABLE(T_REND_CAJ, TT_ORIG,( char* )&rend_caj, sizeof( rend_caj ))== 0 ) {
		//SELECT_TABLE( T_REND_CAJ, TT_ORIG );
        ind.cod_cajero = (unsigned short)NRO_CAJERO;
        ind.caja = 0;
        ind.nro_y = 0;
		/*SET_WHERE("CONCAT(COD_CAJERO ,CAJA,NUMERO_Y) >= CONCAT(%i,%i,%ld)",
						ind.cod_cajero,ind.caja,ind.nro_y);
		SET_ORDER2("COD_CAJERO,CAJA,NUMERO_Y");*/
        SET_WHERE("COD_CAJERO >= '%i'", ind.cod_cajero );
        //GET_GREATER_E( ( char* )( &ind ) );
		RUN_QUERY(NO);
        while( rend_caj.cod_cajero == ind.cod_cajero && !dbEOF() ) {
            if( rend_caj.estado == 1 )            // Proceso cajas cerradas
            {
                sprintf( archivo, "M:Y%06li.%.3i", rend_caj.nro_y, rend_caj.caja );
                handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
                if( handle > 0 ) {
                    _READ( handle, buffer, l_buffer, __LINE__, __FILE__ );
                    ADD_BUFFER_TO_STRUCT( buffer, est );
                    _CLOSE( handle, __LINE__, __FILE__ );
                }
            }
            SKIP2( 1 );
        }
    }
    CLOSE_TABLE( T_REND_CAJ, TT_ORIG );
    //if( USE_DB( AREA_AUX, _DIF_CAJERO, ( char* )&diferencias, sizeof( diferencias ), NULL, 0, 0 ) == 0 ) {
		if( OPEN_TABLE( T_DIF_CAJERO, TT_ORIG,( char* )&diferencias, sizeof( diferencias ))== 0 ) {
	    /*ind_def.cod_cajero = NRO_CAJERO;
        ind_def.medio = 1;
        GET_EQUAL( ( char* )&ind_def );*/
		SET_WHERE("COD_CAJERO = '%i' AND COD_MEDIO = '%i' ",  NRO_CAJERO, 1 );
		RUN_QUERY(NO);
        //if( FOUND() ) {
		if( FOUND2() ) {
            _ADD_MEMORY_DOUBLE( __xc_retiros_importe, 0, diferencias.diferencia );
            _ADD_MEMORY_INT( __xc_retiros_cantidad, 0, diferencias.cant_retiros );
        }
    }
    //CLOSE_DATABASE( AREA_AUX );
	CLOSE_TABLE( T_DIF_CAJERO, TT_ORIG );
    INFORME_00( est, NO, NO );
    FREEMEM( buffer );
}
/*****************************************************************************/
void IMPRIMIR_Z_FISCAL()
/*****************************************************************************/
{
    double gran_total = 0.0;
    int ini_est,h;
    int _ptr,_handle,_estado_z;
    OPEN_RECUPERO( &_ptr, &_handle );
    //_estado_z = STACK_INS( sizeof( int ) );
    _estado_z = STACK_INS( sizeof( short ) );
    if( IMPRESORA_FISCAL == FISCAL_IBM_4694 || IMPRESORA_FISCAL == FISCAL_IBM_4610
     || IMPRESORA_FISCAL == FISCAL ) {
        ini_est = _APUNTAR_STRUCT( _z );
        for( h = 0;h < 9;h++ ) {
            gran_total += ( medios[h].efectivo )
                        ? GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_tot_dinero ) )
                        : _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_ventas_importe ), h )
                        - _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_retiros_importe ), h )
                        + _GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_cobros_importe ), h );
        }
        if( DONACION_DE_VUELTO ) {
            gran_total += GET_MEMORY_DOUBLE( EST_VAR( ini_est + o_x_donaciones_importe ) );
        }
        if( _ESTADO_Z == 0 ) {
            FISCAL_Y();
            _SET_ESTADO_Z( 1 );
        }
        if( _ESTADO_Z == 1 ) {
            if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
                FISCAL_Z();
                PRN_FIN_TICKET();
            }
            else {
                FIN_DOCUMENTO( NO, NO );
            }
            _SET_ESTADO_Z( 2 );
        }
    }
    CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
void IMPRIMIR_REPORTE_TRANSACCIONES()
/*****************************************************************************/
{
    double total;
    //SELECT( AREA_TICKET );
    //GO( TOP );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_WHERE("");
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    MOSTRAR_CADENA( 1, PRN, ST( S_INFORME_DE_TRANSACCIONES_AL ) );
    MOSTRAR_FECHA_2000( 29, PRN, GET_FECHA_DOS() );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_HORA___TRANS_______BOL__TRA____MONTO ) );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, "------------------------------------" );
    LF( 1 );
    while( !dbEOF() ) {
        switch( event_ticket.tipo_evento ) {
            case 0:
                // Boleta
                //total = ROUND( event_ticket.importe_sin_iva + event_ticket.iva1 + event_ticket.ila +
                //              event_ticket.iha + event_ticket.alicuota + event_ticket.exento, 8, DECIMALES );
                total = ROUND( event_ticket.importe_sin_iva + event_ticket.iva1
                             + event_ticket.monto_impuesto[0] + event_ticket.monto_impuesto[1]
                             + event_ticket.monto_impuesto[2] + event_ticket.monto_impuesto[3]
                             + event_ticket.exento, 8, DECIMALES );
                MOSTRAR_HORA( 1, PRN, event_ticket.hora );
                MOSTRAR_CADENA( 8, PRN, ST( S_VENTA ) );
                MOSTRAR_ENTERO( 15, PRN, "999999", event_ticket.nro_ticket );
                MOSTRAR_ENTERO( 22, PRN, "999999", event_ticket.nro_evento );
                MOSTRAR_FLOTANTE( 29, PRN, 8, 0, NO, NO, total );
                LF( 1 );
                break;
            case 10:
                // Zeta
                MOSTRAR_HORA( 1, PRN, event_ticket.hora );
                MOSTRAR_CADENA( 8, PRN, ST( S_CIERRE ) );
                MOSTRAR_ENTERO( 22, PRN, "999999", event_ticket.nro_evento );
                LF( 1 );
                break;
        }
        SKIP2( 1 );
    }
    PRN_FIN_TICKET();
}

