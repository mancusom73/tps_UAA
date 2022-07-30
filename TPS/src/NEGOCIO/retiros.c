/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
//#define _DEBUG_RNV_REC
#include <stdlib.h>
#include <stdio.h>
#undef EOF
#include <ctype.h>
#include <math.h>
#pragma pack(1)
#include <cr.h>
#include <panel.h>
#include <dbrouter.h>
#include <aarch.h>
#include <btrv.h>
#include <config.h>
#include <actions.h>
#include <typeacti.h>
#include <myodbc.h>
#include <recupero.h>
#include <path.h>
#include <assert.h>
#include <deftables.h>
#include <db.h>
#include <pant.h>
#include <cmemory.h>
#include <_cr1.h>
#include <clave.h>
#include <cajon.h>
#include <log.h>
#include <retiros.h>
#include <xyz.h>
#include <m_cadena.h>
#include <imp_ret.h>
#include <mensaje.h>
#include <b_mens.h>
#include <comprob.h>
#include <pago_che.h>
#include <bancos.h>
#include <med_rein.h>
#include <lista.h>
#include <pago_tar.h>
#include <pago_cta.h>
#include <pago_tc.h>
#include <pago_mut.h>
#include <ftoc.h>
#include <round.h>
#include <pagos.h>
#include <malloc.h>
#include <cont_sub.h>
#include <_cr0.h>
#include <stack.h>
#include <recup.h>
#include <cr_pant.h>
#include <tarjetas.h>
#define CANT_SUB_RET 50
/*............................. Variables Globales .......................... */

struct {
    char     modo;
    double   importe;
    long     numero;
    char     id_registro;
    char     cod_sub_medio;
    double   importe_me;
    short    banco;
    int      motivo_cheque;
	char     nombre_submedio[20];
    char     numero_cadena[121];
    int      cantidad;
    int      expandir;
} retiro;
double _fondo_retirado;
double _total_retirado;

struct _medios_denominacion 
	{
    BINARY status;//1
	long cod_medio;//5
    long cod_submedio;//9
	double denominacion;//17
	char simbolo[6];//23
	BINARY fin;
	} *medios_denominacion;

extern void agregarDenominacion( char *s, char *s1 );
extern void clearInput();
extern int getRowCursor();
extern void setCellValue(char *row, char *col, char *text);
extern void getCellValue(char *row, char *col, char *text);
extern void MOSTRAR_INPUT_CANT( char *s, char *CaretPosition );
static int _empieza, _anulacion, cabecera;
double efectivo_retirado,total_retiros,acu_retirado[10],acu_retirado_me[10];
double t_compra_retirado[100], mutuales_retirado[100], tarjetas_retirado[50], moneda_extranjera[100];
static int submedio_pos_retiro[10];
double acu_ret[10][CANT_SUB_RET];
struct _motivos *p2_motivo_sic;
static long _codniv1 = 0, _cod = 0;
static struct _ind_pagos2 ind_pagos2;
struct _ind1_pagos2 ind1_pagos2;
extern struct _pagos2 pagos2;
long posicion;
struct _ev_medios submedioRetiro;
struct _ev_medios submedioRetiro_recup;
int posicionTrabajoRetiro;


int _elegir_motivo2( int comando, char *cadena, int dato, struct _browse *b, double monto );

int     CONTROL_IMPORTES( int nro );
int     ANULADO_O_RETIRADO( int en_anulacion, char estado, int *est );
int     MENSAJE_EN_RETIROS( int iRta );
#define _CHQ_ANULADO    1
#define _CHQ_RETIRADO   2
#define _CHQ_REVERSADO  3

#define _RTA							GET_MEMORY_INT( _rta )
#define _ANULACION						GET_MEMORY_CHAR( _anulacion )
#define _EMPIEZA						GET_MEMORY_CHAR( _empieza )
#define _FONDO_RETIRADO					GET_MEMORY_DOUBLE( _fondo_retirado )
#define _TOTAL_RETIRADO					GET_MEMORY_DOUBLE( _total_retirado )

extern int tipo_t_pagos, tipo_ev_dpago;
/*****************************************************************************/
void CR4_RETIROS( void )
/*****************************************************************************/
{
    int       _ptr, _handle, _estado , _rta;
	

   	/*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );
    //_rta = STACK_INS( sizeof( int ) );
	_rta = STACK_INS( sizeof( short ) );
    _anulacion = STACK_INS( sizeof( char ) );
    _empieza = STACK_INS( sizeof( char ) );
    _fondo_retirado = STACK_INS( sizeof( double ) );
	_total_retirado = STACK_INS( sizeof( double ) );
    /*---------------------------- Prepara Pantalla ----------------------------*/

	/*------------------------- Solicita Autorización --------------------------*/
	if( _ESTADO == 0 ) {
	   SET_MEMORY_INT( _rta, SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, SI,
	   PEDIR_SUPERVISOR_PARA_RETIROS, SI, ST( S_RETIROS ), NULL, RAM_COD_FUNCION, SI ) );
	   _SET_ESTADO( 1 );
	}
	if( _RTA && _ESTADO == 1 ) {
	   if( !USUARIO_PUEDE_RETIRAR( USUARIO_CLAVE_ANTERIOR(  ) ) ) {
	      MENSAJE( ST( S_NO_ESTA_AUTORIZADO_PARA_HACER_RETIROS ) );
		  SET_MEMORY_INT( _rta, 0 );
       } else {
		  SET_MEMORY_INT( _rta, ABRIR_CAJON(  ) );
	   }
	   _SET_ESTADO( 2 );
	}
	/*--------------------------- Grabar Log Sistema ---------------------------*/
	if( _RTA && _ESTADO == 2 ) {
	  SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
      GRABAR_LOG_SUPERVISOR( ST( S_RETIROS ), USUARIO_CLAVE_ANTERIOR(  ), LOG_VENTAS,4 );
      _SET_ESTADO( 3 );
   }
   /*--------------------------- Confecciona Pantalla --------------------------*/
   if( _RTA ) {
      ExecuteAction( A_DES_VENTAS_ACT_RETIROS );
      ExecuteSAction( A_MUESTRA_TITULO_RETIROS, "RETIROS" );
      CR4_CONFECCIONA_PANTALLA();
      _ATRIBUTO = 112;
	}
	/*---------------------------- Inicializaciones ----------------------------*/
   if( _RTA && _ESTADO == 3 ) {
      SET_MEMORY_CHAR( _empieza, 1);
	  SET_MEMORY_CHAR( _anulacion, 0 );
	  //ADD_MEMORY_UINT( __nro_nro_retiro, 1 );
	  SET_MEMORY_CHAR( __ram_p_pago, 0 );
	  SET_MEMORY_DOUBLE( __ram_total, 0.0 );
	  LIMPIAR_VARIAS_TARJETAS(  );
      cabecera = 0;
      efectivo_retirado = 0;
      total_retiros = 0;
	  posicionTrabajoRetiro = 0;
	  BORRAR_ARCHIVO_REC( ARCH_REC_RETIRO );
	  memset( &submedioRetiro, 0, sizeof( struct _ev_medios ) );
	  memset( &submedioRetiro_recup, 0, sizeof( struct _ev_medios ) );
	  memset( t_compra_retirado, 0, sizeof( t_compra_retirado ) );
	  memset( mutuales_retirado, 0, sizeof( mutuales_retirado ) );
	  memset( tarjetas_retirado, 0, sizeof( tarjetas_retirado ) );
	  memset( moneda_extranjera, 0, sizeof( moneda_extranjera ) );
	  memset( acu_ret, 0, sizeof( acu_ret ) );
	  _SET_ESTADO( 4 );
   }
   /*----------------------------- Ingresa Retiros -----------------------------*/
   if( _RTA && _ESTADO == 4 ) {
      SETEAR_TIEMPO( _ENTRADA_NO_VENTA );
	  CR4_PIDE_RETIRO(  );
	  SET_MEMORY_DOUBLE( _total_retirado, total_retiros ); 
	  _SET_ESTADO( 5 );
	//Si graba el evento del supervisor tiene que grabar si o si evento del ticket de lo contrario
	//queda el evento supervisor sin un evento ticket asociado y esta mal porque el siguiente evento 
	//que se graba, lo hace con el id_evento del supervisor que fue el ultimo.
	/*if( RAM_P_PAGO == 0 || _TOTAL_RETIRADO <= 0.0)
	     SET_MEMORY_INT( _rta, 0 ); *///Diego
	}
    /*----------------------------- Graba Retiros ------------------------------*/
   if( _RTA && _ESTADO == 5 ) {
	    SETEAR_TIEMPO( _INDEFINIDO );
        SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
        GRABA_EVENTO_CABECERA_RETIRO( _TOTAL_RETIRADO );
        ACUMULAR_TIEMPOS( );
        CEREAR_TIEMPOS( );
        SETEAR_TIEMPO( _ENTRADA_OCIO );
		posicionTrabajoRetiro = 0;
		if( CANTIDAD_REGISTROS_REC( ARCH_REC_RETIRO, sizeof( struct _ev_medios ) ) ) { 
			while( RECUPERAR_ARCHIVO_REC( ARCH_REC_RETIRO,(char *)&submedioRetiro_recup, 
					sizeof( struct _ev_medios ), posicionTrabajoRetiro ) == 0 ) {
				retiro.modo = ( char )submedioRetiro_recup.modo_pago - 1;
				retiro.importe = submedioRetiro_recup.importe;
				retiro.id_registro = ( submedioRetiro_recup.modo_pago - 1 >= 0 )? 
					medios[submedioRetiro_recup.modo_pago - 1].id_registro : 0;
				retiro.cod_sub_medio = submedioRetiro_recup.cod_sub_medio;
				retiro.importe_me = submedioRetiro_recup.importe_me;
				_GRABA_EVENTO_RETIRO( ( retiro.modo >= 0 ) ? retiro.modo + 1 : 0,
						/*( _ANULACION ) ? -retiro.importe : */retiro.importe, retiro.id_registro,
						retiro.cod_sub_medio,
						/*( _ANULACION ) ? -retiro.importe_me : */retiro.importe_me,
						retiro.motivo_cheque, 0,submedioRetiro_recup.expandir );
				posicionTrabajoRetiro++;
			}
      }
	  ACTUALIZAR_X(  );
	  _SET_ESTADO( 6 );
	  //Para que no imprima si el total retirado es 0 cuando se anula con la tecla menos (-) el medio a retirar.
		if( RAM_P_PAGO == 0 || _TOTAL_RETIRADO <= 0.0 ) {
			SET_MEMORY_INT( _rta, 0 ); 
			_SET_ESTADO( 7 );
		}
   }
   /*----------------------------- Imprime Retiros -----------------------------*/
   if( _RTA && _ESTADO == 6 ) {
		posicionTrabajoRetiro = 0;
		CR4_RESET_ACU_RETIRADO(  );
		if( CANTIDAD_REGISTROS_REC( ARCH_REC_RETIRO, sizeof( struct _ev_medios ) ) ) {
			while( RECUPERAR_ARCHIVO_REC( ARCH_REC_RETIRO,(char *)&submedioRetiro_recup, 
					sizeof( struct _ev_medios ), posicionTrabajoRetiro ) == 0 ) {
				if( ( submedioRetiro_recup.modo_pago - 1 ) != -1 
						&& ( submedioRetiro_recup.modo_pago - 1 ) >= 0 
						&& ( submedioRetiro_recup.modo_pago - 1 ) < 10 ) {
					acu_retirado[submedioRetiro_recup.modo_pago - 1] += submedioRetiro_recup.importe;
					acu_retirado_me[submedioRetiro_recup.modo_pago - 1] += submedioRetiro_recup.importe_me;
					if( submedioRetiro_recup.cod_sub_medio > 0 ) {
						if( submedioRetiro_recup.cod_sub_medio < CANT_SUB_RET ) {
							ACUMULA_RETIRADO( submedioRetiro_recup.modo_pago - 1, submedioRetiro_recup.cod_sub_medio, submedioRetiro_recup.importe );
						} else {
							MENSAJE( "ERROR: EL CODIGO DE SUBMEDIO SUPERA EL LIMITE" );
						}
					}
				}
				posicionTrabajoRetiro++;
			}
		}
		if( MODO_NEGOCIO == SERVICIOS && !_EMPIEZA ) {
			IMPRIME_CABECERA_RETIROS( 3, SI );
			IMPRIME_PIE_RETIROS( acu_retirado, _FONDO_RETIRADO, SI, acu_retirado_me, NO );
			setData( ACTION_DOPRINT, NULL, 0 );
		} else {
			CLOSE_COPIA_PRN(  );
			if( !_EMPIEZA ) {
				if( IMPRIME_RETIROS == _ESTACION_JOURNAL_RECEIPT
						|| IMPRIME_RETIROS == _ESTACION_SLIP ) {
					if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
						/*--- comp Supervisor Retiros ---*/
						IMPRIME_CABECERA_RETIROS( 3, SI );
						IMPRIME_COPIA_PRN();
						IMPRIME_PIE_RETIROS( acu_retirado, _FONDO_RETIRADO, SI,
							acu_retirado_me, NO );
					} else {
						IMPRIME_PIE_RETIROS( acu_retirado, _FONDO_RETIRADO, SI,
							acu_retirado_me, NO );
					}
					if( IMPRIME_RETIROS == _ESTACION_SLIP ) {
						EJECT();
					}
				}
				LF( 0 );
			} else {
				 _SET_ESTADO( 7 );
		      }
      }
	  _SET_ESTADO( 7 );
	}
    if( _ESTADO == 7 ) {
		ACTUALIZA_ID_EVENTO_NEW();
		if( !_EMPIEZA ) {
			ADD_MEMORY_UINT( __nro_nro_retiro, 1 );
			ACTUALIZAR_X();
		 } 
		 GRABAR_XYZ( 'X' );
		 CERRAR_CAJON(  );
		 _SET_ESTADO( 8 );
	}
	/*---------------------------------- Fin -----------------------------------*/
	BORRAR_ARCHIVO_REC( ARCH_REC_RETIRO );
    BORRAR_ARCHIVO_REC( ARCH_REC_RET_TARJ_CUP );
    SET_MEMORY_DOUBLE( _fondo_retirado, 0 );
	SET_MEMORY_DOUBLE( _total_retirado, 0 );
	CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL(  );
    ExecuteAction( A_DES_RETIROS_ACT_VENTAS );
	SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
	MUESTRA_SUBTOTAL( NO );

    /*------------------- Termina recupero de memoria -----------------------*/
	CLOSE_RECUPERO( _ptr, _handle );
}

/*****************************************************************************/
void CR4_CONFECCIONA_PANTALLA()
/*****************************************************************************/
{
    int h;
    #if defined(INVEL_W) || defined(INVEL_L)
    char tt[30];
    #endif
    if( !ancho_panel ) {
        ExecuteAction( A_BORRAR_MEDIOS );
        /*------------ muestra retiro fondo fijo ------------*/
        if( RETIRO_DE_FONDO_FIJO ) {
            strcpy( tt, ST( S_FFIJO ) );
            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_0_AGREGAR, " ", tt, NULL, " " );     
            MOSTRAR_ENTERO( 7, 9, "9", 0 );
            //MOSTRAR_CADENA_STRING( 9, 9, S_FFIJO );
        }
        /*------------ muestra medios de retiro ------------*/
        for( h = 0;h < 9;h++ ) {
            if( medios[h].nro && medios[h].permite_retiros == 'S' ) {
                #ifdef ICONO_MEDIO
                memset( tt, ' ', sizeof( tt ) );
                //bam
                strcpy( tt /*+ 10 - strlen( medios[h].nombre )*/, medios[h].nombre );
                switch( medios[h].nro ) {
                    case MEDIO_1:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_1_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_2:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_2_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_3:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_3_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_4:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_4_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_5:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_5_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_6:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_6_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_7:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_7_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_8:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_8_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_9:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_9_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_10:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_10_AGREGAR, " ", tt, NULL, " " );
                        break;
                    default:
                        glog( "MEDIO NO DEFINIDO", LOG_PANTALLAS,2 );
                }
                #endif
                MOSTRAR_ENTERO( 7, 10 + h, "9", medios[h].nro );
                MOSTRAR_CADENA( 9, 10 + h, medios[h].nombre );
            }
        }
    }
}
/*****************************************************************************/
void CR4_PIDE_RETIRO()
/*****************************************************************************/
{
    int seguir = 1, k = 0, h = 0, i = 0, autoriza, autoriza2 ;
	char       aux[50],cadena[100];
	double     imp_med[9], total_retiros = 0;

    Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
    Response = PreparePanel( &pPanel, Orden );
    if( Response ) {
        ShowPanel( pPanel );
    }
	/* Mostrar Retiros que puedan haber quedado despues de un cuelgue */
	if( CANTIDAD_REGISTROS_REC( ARCH_REC_RETIRO, sizeof( struct _ev_medios ) ) ) {
		posicionTrabajoRetiro = 0;
		while( RECUPERAR_ARCHIVO_REC( ARCH_REC_RETIRO,(char *)&submedioRetiro_recup, 
				sizeof( struct _ev_medios ), posicionTrabajoRetiro ) == 0 ) {
			retiro.modo = ( char )submedioRetiro_recup.modo_pago - 1;
			retiro.numero = submedioRetiro_recup.cod_sub_medio;
			retiro.cod_sub_medio = submedioRetiro_recup.cod_sub_medio;
			retiro.importe = submedioRetiro_recup.importe;
			retiro.importe_me = submedioRetiro_recup.importe_me;
			memset( retiro.nombre_submedio, 0, sizeof( retiro.nombre_submedio ) );
			if( retiro.numero > 0 ) {
				if( medios[retiro.modo].mutual ) {
					if( M_ACUMULADO == medios[retiro.modo].modo_arqueo ) {
						_snprintf( retiro.nombre_submedio, sizeof( retiro.nombre_submedio ), "%s", medios[retiro.modo].nombre ); //no se utilizo submedio
					} else {
						NOMBRE_MUTUAL( (char)retiro.numero, retiro.nombre_submedio );
					}
				}
				else if( medios[retiro.modo].ticket_compra ) {
					if( M_ACUMULADO == medios[retiro.modo].modo_arqueo ) {
						_snprintf( retiro.nombre_submedio, sizeof( retiro.nombre_submedio ), "%s", medios[retiro.modo].nombre ); //no se utilizo submedio
					} else {
						NOMBRE_TICKET_COMPRA( retiro.numero, retiro.nombre_submedio );
					}
				}
				else if( medios[retiro.modo].tarjeta ) {
					if( M_ACUMULADO == medios[retiro.modo].modo_arqueo ) {
						_snprintf( retiro.nombre_submedio, sizeof( retiro.nombre_submedio ), "TARJETA" );//no se utilizo submedio
					} else {
						_snprintf( retiro.nombre_submedio, sizeof( retiro.nombre_submedio ), "%s", NOMBRE_TARJETA( retiro.numero - 1 ) );                        
					}		
				}
				retiro.nombre_submedio[sizeof(retiro.nombre_submedio) - 1] = '\0';
			} 
			if(  retiro.importe < 0 || retiro.importe_me < 0 ) {
				SET_MEMORY_CHAR( _anulacion, 1 );
			} else {
				SET_MEMORY_CHAR( _anulacion, 0 );
			}
			CR4_MOSTRAR_RETIRO(  );
			posicionTrabajoRetiro++;
		}
	}

	//SET_MEMORY_CHAR( _anulacion, 0 );
    while( seguir ) {
        k = GETCH();
        switch( k ) {
			/*Se anula el escape. 
            Para cancelar un retiro hay que Anular la operacion ( con tecla menos "-" )
            y confirmar ( espacio ).
            case 27: //-------- ESC ---------
				if( empieza ) {
					seguir = 0;
				}        
				break;
            */
            case 32:
                /*-------- TOTAL -------*/   
               	memset( imp_med, 0, sizeof( imp_med ) );
				posicionTrabajoRetiro = 0;
				total_retiros = 0;				
				if( CANTIDAD_REGISTROS_REC( ARCH_REC_RETIRO, sizeof( struct _ev_medios ) ) ) { 
			       while( RECUPERAR_ARCHIVO_REC( ARCH_REC_RETIRO,(char *)&submedioRetiro_recup, 
					   sizeof( struct _ev_medios ), posicionTrabajoRetiro ) == 0 ) {
						   if( medios[submedioRetiro_recup.modo_pago - 1].efectivo )						   
						      imp_med[submedioRetiro_recup.modo_pago - 1]+= submedioRetiro_recup.importe;	
						   if( medios[submedioRetiro_recup.modo_pago - 1].mutual ) 
							  imp_med[submedioRetiro_recup.modo_pago -1]+= submedioRetiro_recup.importe;	
						   if( medios[submedioRetiro_recup.modo_pago - 1].ticket_compra )
							  imp_med[submedioRetiro_recup.modo_pago -1]+= submedioRetiro_recup.importe;	
						   if( medios[submedioRetiro_recup.modo_pago - 1].tarjeta )
                              imp_med[submedioRetiro_recup.modo_pago -1]+= submedioRetiro_recup.importe;	
						   if( medios[submedioRetiro_recup.modo_pago - 1].moneda_extranjera )
                              imp_med[submedioRetiro_recup.modo_pago -1]+= submedioRetiro_recup.importe;	  
						  /* if( medios[nro].cheque )
							   imp_med[submedioRetiro_recup.modo_pago]+= submedioRetiro_recup.importe;	  
                           if( medios[nro].cta_cte )
						       imp_med[submedioRetiro_recup.modo_pago]+= submedioRetiro_recup.importe;	  
						   */
						   total_retiros+=submedioRetiro_recup.importe;
					   posicionTrabajoRetiro++;
				    }                				
				} 
				//Controla el total de lo retirado
				autoriza2 = 0;
				if( ( config.monto_minimo_retiro > total_retiros || config.monto_maximo_retiro < total_retiros )
				   && ( ( config.monto_minimo_retiro ) || ( config.monto_maximo_retiro ) ) && ( total_retiros ) ) { 	
					   
					   if( config.monto_maximo_retiro < total_retiros ) {
							sprintf( cadena, ST( S_SE_HA_SUPERADO_IMPORTE_MAXIMO_TOTAL_EN_RETIROS ),aux );    
							MENSAJE( cadena );
						} else if( config.monto_minimo_retiro > total_retiros ) {
							sprintf( cadena, ST( S_NO_ALCANZADO_IMPORTE_MINIMO_TOTAL_EN_RETIROS ),aux );
							MENSAJE( cadena );
						}
                        
						//BORRAR_MENSAJE();
						autoriza2 = 1;
						
				}  
				if( autoriza2 ) { 
		           if( !SOLICITAR_CLAVES( 30, 20, NO, SI, SI,
    			      ST(S_RETIROS), ST( S_ANULACI_N_DEL_TICKET ),
					  RAM_COD_FUNCION, SI ) ) {
					     seguir = 1;
						 break;
					} 
			    } 

				//Controla lo retirado por medio				
				autoriza = 0;
				for( i = 0; i < 9; i++ ) {
					//BORRAR_MENSAJE();
					if( imp_med[i ] && ( medios[i].monto_minimo_retiro > imp_med[i ] || medios[i].monto_maximo_retiro < imp_med[i ] )
							&& ( ( medios[i].monto_minimo_retiro ) || ( medios[i].monto_maximo_retiro ) ) ) {	
						strncpy( aux, medios[i].nombre, sizeof( aux ) ); 				  
						if( /*imp_med[i + 1] && */medios[i].monto_maximo_retiro < imp_med[i ] ) {
							autoriza = 1;
							sprintf( cadena, ST( S_SE_HA_SUPERADO_IMPORTE_MAXIMO_EN__S ),aux );    
							MENSAJE( cadena );
						} else if( /*imp_med[i + 1] &&*/ medios[i].monto_minimo_retiro > imp_med[i ] ) {
							autoriza = 1;
							sprintf( cadena, ST( S_NO_ALCANZADO_IMPORTE_MINIMO_EN__S ),aux );
							MENSAJE( cadena );
						}
					} else {
							seguir = 0;
					}
				}
				BORRAR_MENSAJE();
				if( autoriza ) { 
					if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI,
							ST(S_RETIROS), ST( S_ANULACI_N_DEL_TICKET ),
							RAM_COD_FUNCION, SI ) ) {
						seguir = 0;
					} else {
						 seguir = 1;
					}
			    } else {    
                        seguir = 0;                                    
                }
                break;
            case 45:
                /*-------- '-' : Anulacion ------*/
                CR4_ACTIVA_ANULACION();
                break;
            case '0':
                /*-------- fondo fijo ------*/
                if( RETIRO_DE_FONDO_FIJO ) {
                    if( RAM_P_PAGO < 10 ){
                        #if defined(INVEL_W) || defined(INVEL_L)
                        ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__IMPORTE_ ), "", NULL, " " );
                        ExecuteSSAction( A_RESALTAR_MEDIO_PR, 10, "", "", NULL, " " );
                        #endif
                        CR4_INGRESA_RETIRO( -1, posicion++ );
                        #if defined(INVEL_W) || defined(INVEL_L)
                        ExecuteSSAction( A_MUESTRA_TITULO_BOX, 0, ST( S__IMPORTE_ ), "", NULL, " " );
                        ExecuteAction( A_DESELECCIONA_MEDIO );
                        #endif
                    }
                    else
                    {
                        MENSAJE("Numero de retiros excedido.");
                    }
                }
                break;
            case FLECHA_ARRIBA:
                if( Orden > NUMERO_PANEL_MINIMO_EN_RETIROS ) {
                    Orden--;
                    DestroyPanel( pPanel );
                    PreparePanel( &pPanel, Orden );
                    ShowPanel( pPanel );
                };
                break;
            case FLECHA_ABAJO:
                if( Orden < NUMERO_PANEL_MAXIMO_EN_RETIROS ) {
                    Orden++;
                    DestroyPanel( pPanel );
                    Response = PreparePanel( &pPanel, Orden );
                    if( Response == FALSE ) {
                        Orden--;
                        PreparePanel( &pPanel, Orden );
                    };
                    ShowPanel( pPanel );
                }
                break;
			case 27://*-------- ESC ---------*
				ExecuteAction( A_DESACTIVAR_PANEL_DENOMINACIONES );
				break;
            default:
                if( k >= '1' && k <= '9' ) {
                    if( medios[k - '1'].nro && medios[k - '1'].permite_retiros == 'S' ){
                        if( RAM_P_PAGO < 10 ){
                            #if defined(INVEL_W) || defined(INVEL_L)
                            ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__IMPORTE_ ), "", NULL, " " );
                            ExecuteSSAction( A_RESALTAR_MEDIO_PR, k - '1', "", "", NULL, " " );
                            #endif
							if( GetConfig( CONF_TOUCH_SCREEN ) ){
					             /* Informo al touch screen un cambio de pantalla */
								ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "20" );
	                            CR4_INGRESA_RETIRO( k - '1', posicion++ );
								ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "07" );
							}else{
								CR4_INGRESA_RETIRO( k - '1', posicion++ );
							}
                            #if defined(INVEL_W) || defined(INVEL_L)
                            ExecuteSSAction( A_MUESTRA_TITULO_BOX, 0, ST( S__IMPORTE_ ), "", NULL, " " );
                            ExecuteAction( A_DESELECCIONA_MEDIO );
                            #endif
                        }
                        else
                        {
                            MENSAJE("Numero de retiros excedido.");
                        }
                    }
                }
                break;
        }
    }
    //if( USED( AREA_MEDREINT ) ) {
	if( OPENED( T_MEDIOS_REINT, TT_ORIG ) ){
        //CLOSE_DATABASE( AREA_MEDREINT );
		CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
    }

    if( OPENED( T_PAGOS2, TT_ORIG ) ) {
        CLOSE_TABLE( T_PAGOS2, TT_ORIG );
    }
    if( Response ) {
        DestroyPanel( pPanel );
    }
}

/***********************************************************************/
double ELEGIR_DENOMINACION( int grilla, int cant, char *simbolo)
/***********************************************************************/
{ 
    int seguir = 1, k, rta = 0, cont = 1, cantden = 0, cantidad = 0, enter=0, continuar=1;
	long pos;
	char text[5], fila[5], cantant[5], subTotal[10], deno[20], cadena[100];
	double importe = 0.00;
	double total = 0.00;

	
	
    ExecuteSAction( grilla, "0" );            // 0 habilita
	
	_snprintf( cadena, sizeof(cadena), "CONFIRMAR TOTAL - ENTER: SI - ESC: NO");
	cadena[sizeof(cadena)-1]='\0';
	_snprintf(subTotal,sizeof(subTotal),"%s 0.00", simbolo);
	subTotal[sizeof(subTotal)-1]='\0';
	_snprintf(text, sizeof(text),"0");
	text[sizeof(text)-1]='\0';
	ExecuteSAction( A_SUBTOTAL_DENOMINACIONES, subTotal);	


	while( seguir ) {
        k = GETCH();

		switch( k ) {
			
			case 13:
                /*-------- ENTER ---------*/ 
                if(enter)
				{
					MENSAJE( cadena );
					
					do {k = GETCH();}
					while( k == -999 );
					
					if(k == 13)
						{
						seguir = 0;
						ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
						BORRAR_MENSAJE();
						return ( total );
						}
					if(k == 27)
						{
						seguir=1;
						BORRAR_MENSAJE();
						enter=0;
						}
					
				}
				
				
				break;
			case 27: /* ESC */	
				seguir = 0;
				total = 0.00;
                ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
                break;
            case -72:
                /*----- FLECHA ARRIBA -----*/
                ExecuteSAction( grilla, "-1" );
                break;
            case -80:
                /*------ FLECHA ABAJO -----*/
                ExecuteSAction( grilla, "1" ); 
                break;

			default:
				{
					pos = getRowCursor();	
					_snprintf(fila, sizeof(fila),"%d",pos);
					getCellValue(fila, "1", cantant);
					if( k >= '0' && k <= '9' ) 
					{
					cantden = PIDE_CANTIDAD_DENOMINACION(k);
					_snprintf(text, sizeof(text),"%d",cantden);

					pos = getRowCursor();	
					_snprintf(fila, sizeof(fila),"%d",pos);
					setCellValue(fila, "1", text);
					getCellValue(fila, "0", deno);
					importe = atof(deno);
					cantidad = atoi(cantant);
					
					if(cantden < cantidad)
						total -= importe*(cantidad-cantden);
					else
						if(cantden > cantidad)
							total += importe*(cantden-cantidad);
					
					seguir =1;
					cantden = 0;
					clearInput();

					_snprintf(subTotal,sizeof(subTotal),"%s %.2lf", simbolo, total);
					subTotal[sizeof(subTotal)-1]='\0';
					ExecuteSAction( A_SUBTOTAL_DENOMINACIONES, subTotal);

					enter=1;
					}
				}
				break;
        }

	}
    return ( total );
}

/***********************************************************************/
double MOSTRAR_DENOMINACIONES( int cod_medio, int cod_submedio )
/***********************************************************************/
{
    /*char where[100];*/
	int cnt = 0; 
	double importe = 0.00;
	long mult = 1;
    int posx = 56;

	char denominacion[20];
	
	memset(denominacion,0,sizeof(denominacion));
    
	if (!( medios_denominacion = MALLOC( sizeof( struct _medios_denominacion ) ) ) ) return (0);
	
	//DIRECT_DB_QUERY("delete from medios_denominacion"); // Borramos todos los registros de la tabla por si tiene basura

	//_snprintf( where,sizeof(where), " WHERE COD_MEDIO = %d AND COD_SUBMEDIO = %d", cod_medio, cod_submedio);
	//where[sizeof(where)-1]='\0';

	

	//{
	//	if( !OBTENER_TABLA_SERVIDOR( " ", "medios_denominacion", where, 1, NULL)) 
	//	{
	        //COMENTO HASTA QUE SE ARREGLE		
            /*if( !OPENED( T_MED_DENOMINACION, TT_ORIG ) && OPEN_TABLE(T_MED_DENOMINACION, TT_ORIG, (char *)medios_denominacion, sizeof( struct _medios_denominacion )) == 0 ) 
			{
				SELECT_TABLE( T_MED_DENOMINACION, TT_ORIG );
				SET_WHERE("");
				SET_ORDER2( "DENOMINACION" );
				RUN_QUERY( NO );
				cnt = RECCOUNT2(NULL,NULL);
				
				if(cnt)
				{
					ExecuteAction( A_ACTIVAR_PANEL_DENOMINACIONES );	
					while( !dbEOF() )
					{
						_snprintf(denominacion, sizeof(denominacion),"%.2lf",medios_denominacion->denominacion);
						denominacion[sizeof(denominacion)-1]='\0';
						agregarDenominacion( denominacion, "0" );
						SKIP2( 1 );
					}
					importe = ELEGIR_DENOMINACION( A_SCROLL_DENOMINACIONES, cnt, medios_denominacion->simbolo);
				}
			}
		//}
	ExecuteAction( A_DESACTIVAR_PANEL_DENOMINACIONES );
	CLOSE_TABLE( T_MED_DENOMINACION, TT_ORIG );
	
	for( j = 1;j <= DECIMALES;j++ ) {
        mult *= 10;
    }
	MOSTRAR_INPUT_CADENA( "     ", 0 );
	PAGOS_MUESTRA_IMPORTE_TEMPORAL(importe, NO);*/


    return importe;
}

/*****************************************************************************/
void CR4_INGRESA_RETIRO( int nro, int posicion )
/*****************************************************************************/
{
    int ctrl_importe = 1;
    int motivo_cheque = 0;
    short tipo_busqueda;
    char msg_tmp[50];
	double ctrl_importe_total = 0.0;
    
    _codniv1 = 0;
    _cod = 0;

    retiro.modo = nro;
    retiro.importe = 0.0;
    retiro.numero = 0;
    retiro.id_registro = 0;
    retiro.cod_sub_medio = 0;
    retiro.importe_me = 0.0;
    retiro.banco = 0;
    retiro.motivo_cheque = 0;
    retiro.cantidad = 1;    //esta incializado en 1
    retiro.expandir = NO;
	memset( retiro.nombre_submedio, 0, sizeof( retiro.nombre_submedio ) );
	memset( retiro.numero_cadena, 0, sizeof( retiro.importe_me ) );

    tipo_busqueda = ( MODO_NEGOCIO == SERVICIOS && MODO_CHEQUE_COMO_UN_MEDIO ) ? _BUSCAR_EN_DPAGO_BTR : _BUSCAR_EN_PAGO_BTR;
    
   /*if( nro >= 0 ) {
        _SET_MEMORY_CHAR( __pago_id_registro, nro, 0 );               
        _SET_MEMORY_INT( __pago_banco, nro, 0 );     
        _SET_MEMORY_LONG( __pago_numero, nro, 0 ); 
        _CEREAR_MEMORY( __pago_nro_tarjeta, nro );
        _SET_MEMORY_LONG( __pago_nro_cupon1, nro, 0 );
        _SET_MEMORY_LONG( __pago_nro_cupon_int1, nro, 0 );
    }*/

    if( nro >= 0 ) {
        retiro.id_registro = medios[nro].id_registro;
    }
    else {
        retiro.id_registro = 0;
    }
    /*----------- control inicial ------------*/
	if( MEXICO == config.pais && !_ANULACION && ( -1 == nro || medios[nro].efectivo ) ) {
        //no se permite realizar retiros si no hay dinero
        if( ( nro >= 0 ) && ( _X_VENTAS_IMPORTE( nro ) + _X_COBROS_IMPORTE( nro ) 
            - _X_RETIROS_IMPORTE( nro ) <= 0.001 ) ) {
            MENSAJE( "NO HAY DINERO PARA RETIRAR" );
            BORRAR_MENSAJE();
            return;            
        }
        else if( ( -1 == nro ) && ( X_FONDO_FIJO_IMPORTE - X_RETIRO_FONDO_IMPORTE <= 0.001 ) ) {
            MENSAJE( "NO HAY DINERO PARA RETIRAR" );
            BORRAR_MENSAJE();
            return;            
        }
    }
    
    /*----------- prepara pantalla -----------*/
    DestroyPanel( pPanel );
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    ExecuteSAction( A_MUESTRA_TITULO_BROWSE, medios[nro].nombre );
    if( nro == -1 ) {
        /*-------------------- Si es fondo fijo --------------------*/
        Orden = NUMERO_PANEL_BILLETES;
        PreparePanel( &pPanel, Orden );
        ShowPanel( pPanel );
        /*-------------------- Si es fondo fijo --------------------*/
        retiro.importe = MOSTRAR_DENOMINACIONES( 0, 0 ); 
		if(!retiro.importe)
			retiro.importe = CR4_PIDE_IMPORTE();
        DestroyPanel( pPanel );
        if( retiro.importe > 0 ) {
			if( _ANULACION ) {
                if( retiro.importe > _FONDO_RETIRADO ) {
                    MENSAJE_STRING( S_NO_HA_RETIRADO_TANTO_FONDO_FIJO );
                    retiro.importe = 0;
                }
                else {
					ADD_MEMORY_DOUBLE( _fondo_retirado, -retiro.importe );
                }
            }
            else {
				//Control de importes de retiros de fondo fijo
                if( CONTROL_DE_IMPORTES_EN_RETIROS
                 && ( retiro.importe > ( X_FONDO_FIJO_IMPORTE - X_RETIRO_FONDO_IMPORTE ) 
                 || retiro.importe > X_TOT_DINERO ) ) {

                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
                else {
                    SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
					ADD_MEMORY_DOUBLE( _fondo_retirado, retiro.importe );
                }
            }
        }
    }
    else if( medios[nro].efectivo ) { //Control submedios OK
        /*-------------------- Si es efectivo ----------------------*/
        Orden = NUMERO_PANEL_BILLETES;
        PreparePanel( &pPanel, Orden );
        ShowPanel( pPanel );
		retiro.importe = MOSTRAR_DENOMINACIONES( 1, 0 ); 
		if(!retiro.importe)
			retiro.importe = CR4_PIDE_IMPORTE();    // Ahv
        DestroyPanel( pPanel );
        if( retiro.importe > 0 ) {
            retiro.importe *= RAM_CANTIDAD;
            if( _ANULACION ) {
                if( retiro.importe > efectivo_retirado ) {
                    MENSAJE_STRING( S_NO_HA_RETIRADO_TANTO_EFECTIVO );
                    retiro.importe = 0;
                }
                else {
                    efectivo_retirado -= retiro.importe;
                }
            }
            else {
                //Control de importe de retiros de efectivo
                if( CONTROL_DE_IMPORTES_EN_RETIROS
                 && ( (_X_VENTAS_IMPORTE( nro ) + _X_COBROS_IMPORTE( nro ) 
                 - _X_RETIROS_IMPORTE( nro ) - retiro.importe + 0.01 ) <= 0.001 ) ) {
                    //> ( X_TOT_DINERO - X_FONDO_FIJO_IMPORTE + X_RETIRO_FONDO_IMPORTE ) ) ) {
                    // if( retiro.importe > ( X_TOT_DINERO -
                    // X_FONDO_FIJO_IMPORTE + X_RETIRO_FONDO_IMPORTE ) ) {
                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
                else {
                    SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
                    efectivo_retirado += retiro.importe;
                }
            }
        }
        retiro.cod_sub_medio = 0;
    }
    else if( medios[nro].cheque ) {
        //char  bancotmp[20];
        char cuenta_banco[30];
        //_SET_MEMORY_LONG( __pago_nro_cupon1, nro, 0 );
        //_SET_MEMORY_LONG( __pago_nro_cupon_int1, nro, 0 );

        /*-------------------- Si es cheque -----------------------*/
        if( PEDIR_IMPORTE_EN_RETIRO_DE_CHEQU || !PEDIR_SUBMEDIO_EN_RETIROS ) {
            retiro.importe = MOSTRAR_DENOMINACIONES( 3, 0 ); 
			if(!retiro.importe)
				retiro.importe = CR4_PIDE_IMPORTE() * RAM_CANTIDAD;
        }
        //pedir motivo, un cheque rechazado q era pago de reintegro
        if( LASTKEY() != 27 ) {
            motivo_cheque = ELEGIR_MOTIVO_RETIRO( NULL );
        }
        if( motivo_cheque ) {
            MENSAJE( "RETIRO DE CHEQUES CARGADOS PARA PAGO DE REINTEGROS" );        
            tipo_busqueda = _BUSCAR_EN_PAGO_BTR_ESP;
            //retiro.numero = PIDE_NRO_CHEQUE();
            retiro.numero = PIDE_CADENA_NRO_CHEQUE( retiro.numero_cadena );
            if( USAR_MENU_BANCOS ) {
                retiro.banco = PIDE_BANCO( cuenta_banco );
            }
            else {
                retiro.banco = PIDE_NRO_BANCO();
            }

            if( CONTROL_IMPORTES_ESP( 1, NO, &_codniv1, &_cod, nro, ( double* )&retiro.importe, retiro.numero,
                                      retiro.banco, motivo_cheque, _ANULACION,
                                      retiro.numero_cadena ) ) {

                if( retiro.numero > 0 ) {
                    retiro.importe = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, NO,
                                                            ( int* )&retiro.id_registro,
                                                            ( char* )&retiro.cod_sub_medio,
                                                            ( short* )&retiro.banco,
                                                            _BUSCAR_EN_PAGO_BTR_ESP, 
                                                            0,
                                                            NULL,
                                                            _ANULACION, SI );
                }
                else {
                    retiro.importe = 0;
                }
            }
            else {
                retiro.importe = 0;
            }
        }
        else { 
            if( retiro.importe > 0 ) {
                if( _ANULACION || !CONTROL_IMPORTES( nro ) ) {
                    if( tipo_busqueda == _BUSCAR_EN_DPAGO_BTR ) {
                        retiro.numero = PIDE_CADENA_NRO_CHEQUE( retiro.numero_cadena );
                    } else {
                        retiro.numero = PIDE_NRO_CHEQUE( );
                    }
                    if( USAR_MENU_BANCOS )
                        retiro.banco = PIDE_BANCO( cuenta_banco );
                    else
                        retiro.banco = PIDE_NRO_BANCO();

                    if( retiro.numero > 0 ) {
                        retiro.importe = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, NO,
                                                                    ( int* )&retiro.id_registro,
                                                                    ( char* )&retiro.cod_sub_medio,
                                                                    ( short* )&retiro.banco,
                                                                    tipo_busqueda,
                                                                    retiro.importe, 
                                                                    ( tipo_busqueda == _BUSCAR_EN_DPAGO_BTR ) ? retiro.numero_cadena : NULL,
                                                                    _ANULACION, SI );
                    }
                    else {
                        retiro.importe = 0;
                        MENSAJE("Cheque no encontrado");
                    }
                }
                else {
                    retiro.importe = 0;
                }
            }
        }
        if( retiro.importe > 0 ) {
            char cad_aux[16];
            //_SET_MEMORY_CHAR( __pago_id_registro, nro, retiro.id_registro ); 
			submedioRetiro.id_registro = retiro.id_registro;
            //_SET_MEMORY_INT( __pago_banco, nro, retiro.banco ); 
			submedioRetiro.banco = retiro.banco;
            //_SET_MEMORY_LONG( __pago_numero, nro, 0 );
			submedioRetiro.numero_id = 0;
            strncpy( cad_aux, retiro.numero_cadena, 16 );
            //_SET_MEMORY( __pago_nro_tarjeta, nro, cad_aux );
			strncpy( submedioRetiro.nro_tarjeta, cad_aux, 21 );
            retiro.motivo_cheque = motivo_cheque;
            //_SET_MEMORY_LONG( __pago_nro_cupon1, nro, _cod );
			submedioRetiro.nro_cupon1 = _cod;
            //_SET_MEMORY_LONG( __pago_nro_cupon_int1, nro, _codniv1 );
			submedioRetiro.nro_cupon_int1 = _codniv1;
            SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
            //SET_MEMORY_DOUBLE( __pago_valor_cupones, nro,
					//( double ) OBTENER_CODIGO_REINTEG( _codniv1, _cod, (short) nro ) );           
			submedioRetiro.valor_cupones = ( double ) OBTENER_CODIGO_REINTEG( _codniv1, _cod, (short) nro ); 
            //_SET_MEMORY_LONG( __pago_buffer1_long, nro, RAM_NRO_CLIENTE );
			submedioRetiro.buffer1_long = RAM_NRO_CLIENTE;
        }
  
    }
    else if( medios[nro].tarjeta ) {
        /*---------------- Si es tarjeta de credito ----------------*/
        if( PEDIR_IMPORTE_EN_RETIRO_DE_TARJE || !PEDIR_SUBMEDIO_EN_RETIROS ) {
            int tt = 0;
			if( PEDIR_SUBMEDIO_EN_RETIROS ) {
                //Aunque este habilitado "pedir_submedio_en_retiros", si el medio esta configurado
                //para realizar "retiros acumulados", no se pedira seleccionar un submedio.
                retiro.numero = PIDE_TARJETA( &tt, medios[nro].modo_arqueo != M_ACUMULADO );
				retiro.cod_sub_medio = ( char )retiro.numero;
				retiro.id_registro = ( char )tt;
                if( retiro.numero > 0 ){
                    if( M_ACUMULADO == medios[nro].modo_arqueo ) {
                        _snprintf( retiro.nombre_submedio, sizeof( retiro.nombre_submedio ), "TARJETA" );//no se utilizo submedio
                    } else {
                        _snprintf( retiro.nombre_submedio, sizeof( retiro.nombre_submedio ), "%s", NOMBRE_TARJETA( retiro.numero - 1 ) );                        
                    }
                    retiro.nombre_submedio[sizeof(retiro.nombre_submedio) - 1] = '\0';
                    if( MEXICO == config.pais && !_ANULACION ) {
                        ctrl_importe = CONTROL_IMPORTE_SUBMEDIO( nro + 1, retiro.cod_sub_medio, medios[nro].modo_arqueo );
						ctrl_importe_total = (double)ctrl_importe + _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro );
						if( !ctrl_importe_total ) {
                            sprintf( msg_tmp, "NO HAY %s DISPONIBLE PARA RETIRO", _strupr( retiro.nombre_submedio ) );
                            MENSAJE( msg_tmp );
                            BORRAR_MENSAJE();
                        }
                    }
                }
            }
            if( ( retiro.numero > 0 || !PEDIR_SUBMEDIO_EN_RETIROS ) && ctrl_importe ) {
				retiro.importe = MOSTRAR_DENOMINACIONES( 4, (int)retiro.numero ); 
				if(!retiro.importe)
					retiro.importe = CR4_PIDE_IMPORTE() * RAM_CANTIDAD;
			}
			if( _ANULACION ) {
				if( retiro.importe > tarjetas_retirado[retiro.numero] ) {
					MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
                else {
                    tarjetas_retirado[retiro.numero] -= retiro.importe;
                }
			} /*else {	
				double monto_en_caja;
				monto_en_caja = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, 1, NULL, NULL, NULL,
									_BUSCAR_EN_PAGO_BTR, 0, NULL, anulacion, NO  );
				
				if( monto_en_caja - tarjetas_retirado[retiro.numero] < retiro.importe ) {
				//CONTROL_IMPORTES( nro );
					MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
					retiro.importe = 0;
				}
			}*/
            if( !_ANULACION && CONTROL_DE_IMPORTES_EN_RETIROS /*&& MEXICO == config.pais*/ ) {
                double importeTmp;
                importeTmp = _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) + OBTENER_IMPORTE_SUBMEDIO( nro + 1, retiro.cod_sub_medio, medios[nro].modo_arqueo );
                if( retiro.importe > importeTmp ) {
                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
            }
            if( retiro.importe > 0 && !_ANULACION && ctrl_importe ) {
		        tarjetas_retirado[retiro.numero] += retiro.importe;
                SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
            }
        }
        else {//pedir cantidad de cupones
            if(M_ACUMULADO == medios[nro].modo_arqueo){
                if(!_ANULACION){
                    int cnt = 0, i = 0, cantTar = 0;
                    
                    cantTar = _X_VENTAS_CANTIDAD( nro ) - _X_RETIROS_CANTIDAD( nro );
                    if(cantTar > 0){
                        cnt = PEDIR_ENTERO_EN_RECUADRO( ST( S__CANT__CUPONES_ ), SI, 3 );
                        while(cnt != cantTar && MENSAJE_CON_TECLA_S_N("HAY DIFERENCIA EN LA CANT. DE CUPONES, REINTENTAR? <ENTER>=SI <ESC>=NO",13,27) == 13){
                            cnt = PEDIR_ENTERO_EN_RECUADRO( ST( S__CANT__CUPONES_ ), SI, 3 );    
                        }

                        if(MENSAJE_CON_TECLA_S_N("CONFIRMA RETIRO? <ENTER>=SI <ESC>=NO",13,27) == 13){
                            if(cnt != cantTar){
                                retiro.expandir = SI; //indica que hay diferencias
                            }else{
                                retiro.expandir = NO; //indica que hay diferencias   
                            }
                            retiro.importe =  _X_VENTAS_IMPORTE( nro )  -  _X_RETIROS_IMPORTE( nro );
                            retiro.cantidad = cantTar;
                            retiro.numero = 0;              //sumedio 0
                            SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
                            GUARDAR_ARCHIVO_REC( ARCH_REC_RET_TARJ_CUP, (char*)&retiro,sizeof( retiro ) );
                        }
                    }else{
                        MENSAJE("NO HAY TARJETAS PARA RETIRAR!");
                    }
                }else{ // SI ES ANULACION
                    if(RECUPERAR_ARCHIVO_REC(ARCH_REC_RET_TARJ_CUP,(char *)&retiro,sizeof( retiro ),0)==0){
                        if(MENSAJE_CON_TECLA_S_N("ANULAR RETIRO TARJETA? <ENTER>=SI <ESC>=NO",13,27) == 13){
                            
                        }else{
                            memset( &retiro, 0, sizeof( retiro ) );
                        }
                    }else{
                        memset( &retiro, 0, sizeof( retiro ) );
                    }
                }
            }else{ //si no es acumulado
            #if defined(INVEL_W) || defined(INVEL_L)            
                ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__NRO_CUPON_ ), "", NULL, " " );
            #endif
            retiro.numero = PIDE_NRO_CUPON( 9 );
                if( retiro.numero > 0 ) {

                    retiro.importe = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, NO,
                                                            ( int* )&retiro.id_registro,
                                                            ( char* )&retiro.cod_sub_medio, NULL,
                                                            _BUSCAR_EN_PAGO_BTR, 0, NULL,
                                                            _ANULACION, SI );
                }
                if( retiro.importe > 0 && !_ANULACION && ctrl_importe ) {
		            tarjetas_retirado[retiro.numero] += retiro.importe;
                    SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
                }
            }
        }

    }
    else if( medios[nro].cta_cte ) {
        /*--------------------- Si es cuenta corriente -----------------*/
        if( PEDIR_IMPORTE_EN_RETIRO_DE_CTACT || !PEDIR_SUBMEDIO_EN_RETIROS ) {
            retiro.importe = MOSTRAR_DENOMINACIONES( 5, 0 ); 
				if(!retiro.importe)
            retiro.importe = CR4_PIDE_IMPORTE() * RAM_CANTIDAD;
            CONTROL_IMPORTES( nro );
        }
        else {
            retiro.numero = PIDE_CUENTA( 8 );
            if( retiro.numero > 0 ) {
                retiro.importe = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, NO, NULL, NULL, NULL,
                                                        _BUSCAR_EN_PAGO_BTR, 0, NULL, _ANULACION, SI );
            }
        }
        retiro.cod_sub_medio = 0;
        if( retiro.importe > 0 ) {
            SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
        }
    }
    else if( medios[nro].ticket_compra ) {
        /*--------------------- Si es ticket de compra -----------------*/
        retiro.cod_sub_medio = 0;
        if( PEDIR_SUBMEDIO_EN_RETIROS ) {
            //Aunque este habilitado "pedir_submedio_en_retiros", si el medio esta configurado
            //para realizar "retiros acumulados", no se pedira seleccionar un submedio.
            retiro.numero = ( long )PIDE_TICKET_COMPRA( ( int* )&retiro.id_registro, NULL, NULL,
                NULL, NULL, NULL, NULL, medios[nro].nro, NO, NULL, medios[nro].modo_arqueo != M_ACUMULADO );
            retiro.cod_sub_medio = ( char )retiro.numero;
            if( retiro.numero > 0 ) {
                if( M_ACUMULADO == medios[nro].modo_arqueo ) {
                    _snprintf( retiro.nombre_submedio, sizeof( retiro.nombre_submedio ), "%s", medios[nro].nombre ); //no se utilizo submedio
                    retiro.nombre_submedio[ sizeof( retiro.nombre_submedio ) - 1 ] = '\0';
                } else {
                    NOMBRE_TICKET_COMPRA( retiro.numero, retiro.nombre_submedio );
                }
                if ( MEXICO == config.pais && !_ANULACION ) {
                    ctrl_importe = CONTROL_IMPORTE_SUBMEDIO( nro + 1, retiro.cod_sub_medio, medios[nro].modo_arqueo );
                    ctrl_importe_total = (double)ctrl_importe + _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro );
					if( !ctrl_importe_total ) {
                        sprintf( msg_tmp, "NO HAY %s DISPONIBLE PARA RETIRO", _strupr( retiro.nombre_submedio ) );
                        MENSAJE( msg_tmp );
                        BORRAR_MENSAJE();
                    }
                }
            }
        }
        if( ( retiro.numero > 0 || !PEDIR_SUBMEDIO_EN_RETIROS ) && ctrl_importe ) {
            if( PEDIR_IMPORTE_EN_RETIRO_DE_TICKE || !PEDIR_SUBMEDIO_EN_RETIROS ) {
                
				retiro.importe = CR4_PIDE_IMPORTE() * RAM_CANTIDAD;
				
				if( _ANULACION ) {
					if( retiro.importe > t_compra_retirado[retiro.numero] ) {
						MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
						retiro.importe = 0;
					}
					else {
						t_compra_retirado[retiro.numero] -= retiro.importe;
					}
				}/* else {	
					double monto_en_caja;
					monto_en_caja = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, 1, NULL, NULL, NULL,
										_BUSCAR_EN_PAGO_BTR, 0, NULL, anulacion, NO  );
					
					if( monto_en_caja - t_compra_retirado[retiro.numero] < retiro.importe ) {
					//CONTROL_IMPORTES( nro );
						MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
						retiro.importe = 0;
					}
				}*/
                if( !_ANULACION && CONTROL_DE_IMPORTES_EN_RETIROS /*&& MEXICO == config.pais*/ ) {
                    double importeTmp;
                    importeTmp = _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) + OBTENER_IMPORTE_SUBMEDIO( nro + 1, retiro.cod_sub_medio, medios[nro].modo_arqueo );
                    if( retiro.importe > importeTmp ) {
                        MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                        retiro.importe = 0;
                    }
                }
            }
            else {
                retiro.importe = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, 1, NULL, NULL, NULL,
                                                        _BUSCAR_EN_PAGO_BTR, 0, NULL, _ANULACION, SI  );
            }
        }
        if( retiro.importe > 0 && !_ANULACION && ctrl_importe ){
			t_compra_retirado[retiro.numero] += retiro.importe;
            SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
        }
        #ifdef COMPILAR_MUTUALES
    }
    else if( medios[nro].mutual ) {
        /*------------------------ Si es mutual ------------------------*/
        retiro.cod_sub_medio = 0;
        if( PEDIR_SUBMEDIO_EN_RETIROS ) {
            retiro.numero = ( long )PIDE_MUTUAL( ( int* )&retiro.id_registro, NULL, NULL, NULL,
                                                 NULL, NULL, NULL, medios[nro].nro, 0.00, NO, 
                                                 NULL, medios[nro].modo_arqueo != M_ACUMULADO );
            retiro.cod_sub_medio = ( char )retiro.numero;
            if( retiro.numero > 0 ) {
                if( M_ACUMULADO == medios[nro].modo_arqueo ) {
                    _snprintf( retiro.nombre_submedio, sizeof( retiro.nombre_submedio ), "%s", medios[nro].nombre ); //no se utilizo submedio
                    retiro.nombre_submedio[sizeof( retiro.nombre_submedio ) - 1] = '\0';
                } else {
			        NOMBRE_MUTUAL( (char)retiro.numero, retiro.nombre_submedio );
                }
                if( MEXICO == config.pais &&  !_ANULACION ) {
                    ctrl_importe = CONTROL_IMPORTE_SUBMEDIO( nro + 1, retiro.cod_sub_medio, medios[nro].modo_arqueo );
                    ctrl_importe_total = (double)ctrl_importe + _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro );
					if( !ctrl_importe_total ) {
                        sprintf( msg_tmp, "NO HAY %s DISPONIBLE PARA RETIRO", _strupr( retiro.nombre_submedio ) );
                        MENSAJE( msg_tmp );
                        BORRAR_MENSAJE();
                    }
                }
            }
        }
        if( ( retiro.numero > 0 || !PEDIR_SUBMEDIO_EN_RETIROS ) && ctrl_importe ) {
			double cotizacion;
            if( ( PEDIR_IMPORTE_EN_RETIRO_DE_MUTUA || !PEDIR_SUBMEDIO_EN_RETIROS ) ) {
				retiro.importe = MOSTRAR_DENOMINACIONES( 2, (int)retiro.numero ); 
				if(!retiro.importe)
					retiro.importe = CR4_PIDE_IMPORTE() * RAM_CANTIDAD;
				retiro.importe_me = retiro.importe;
				if( COTIZACION_MUTUAL( (char)retiro.numero, &cotizacion, NULL ) ) {
					retiro.importe = ROUND( retiro.importe_me * cotizacion, 10, DECIMALES );
					sprintf( retiro.numero_cadena, "%8.2lf", retiro.importe_me );
				}
                if( _ANULACION ) {
					if( retiro.importe_me > mutuales_retirado[retiro.numero] ) {
						MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
						retiro.importe_me = 0;
						retiro.importe = 0;
					}
					else {
						mutuales_retirado[retiro.numero] -= retiro.importe_me;
					}
				} /*else { //Esta modificacion es para controlar el importe de retiros por submedios
							//pero no esta terminado.
					double monto_en_caja;
					monto_en_caja = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, 1, NULL, NULL, NULL,
										_BUSCAR_EN_PAGO_BTR, 0, NULL, anulacion, NO  );
					
					if( monto_en_caja - mutuales_retirado[retiro.numero] < retiro.importe ) {
					//CONTROL_IMPORTES( nro );
						MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
						retiro.importe = 0;
					}
				}*/
                if( !_ANULACION && CONTROL_DE_IMPORTES_EN_RETIROS /*&& MEXICO == config.pais*/ ) {
                    double importeTmp;
                    importeTmp = _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) + OBTENER_IMPORTE_SUBMEDIO( nro + 1, retiro.cod_sub_medio, medios[nro].modo_arqueo );
                    if( retiro.importe > importeTmp ) {
                        MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                        retiro.importe = 0;
                    }
                }
            }
            else {
                retiro.importe = CR4_BUSCA_COMPROBANTE( nro, retiro.numero, 1, NULL, NULL, NULL,
                                                        _BUSCAR_EN_PAGO_BTR, 0, NULL, _ANULACION, SI );
            }
        }
        if( retiro.importe_me > 0 && !_ANULACION && ctrl_importe){
			mutuales_retirado[retiro.numero] += retiro.importe_me;
            SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
        }
        #endif
    }
	else if( medios[nro].moneda_extranjera ) {
        /*-------------------- Si es moneda extranjera -----------------------*/
        retiro.importe = CR4_PIDE_IMPORTE() * RAM_CANTIDAD;
        if( retiro.importe > 0 ) {
            retiro.importe_me = retiro.importe;
            retiro.importe = PAGOS_ME_A_ML( retiro.importe_me, nro );
            if( _ANULACION ) {
                if( retiro.importe_me > moneda_extranjera[retiro.numero]/*acu_retirado_me[nro] + 0.02*/ ) {
                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe_me = 0;
					retiro.importe = 0;
                } else {
					moneda_extranjera[retiro.numero] -= retiro.importe_me;
				}
            }
            else {
                if( CONTROL_DE_IMPORTES_EN_RETIROS
                 && ( retiro.importe_me > _X_VENTAS_IMPORTE_ME( nro ) + _X_COBROS_IMPORTE_ME( nro )
                    + _X_FONDO_FIJO_MULTIPLE_IMP_ME( nro ) - _X_RETIROS_IMPORTE_ME( nro ) + 0.02 ) ) {
                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
            }
        }
		retiro.cod_sub_medio = 0;
		if( retiro.importe_me > 0 && !_ANULACION ) {
			moneda_extranjera[retiro.numero] += retiro.importe_me/*retiro.importe*/;
            SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
        }
    }
    else {
        /*--------------------- Si es otro medio -----------------------*/
        retiro.importe = CR4_PIDE_IMPORTE();
        retiro.cod_sub_medio = 0;
        if( retiro.importe > 0 ) {
            retiro.importe *= RAM_CANTIDAD;
            if( _ANULACION ) {
                if( retiro.importe > acu_retirado[nro] + 0.02 ) {
                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
            }
            else {
                if( CONTROL_DE_IMPORTES_EN_RETIROS
                 && ( retiro.importe > _X_VENTAS_IMPORTE( nro ) + _X_COBROS_IMPORTE( nro )
                    + _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) - _X_RETIROS_IMPORTE( nro ) + 0.02 ) ) {
                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
            }
        }
        if( retiro.importe > 0 ) {
            SET_MEMORY_CHAR( _empieza, 0 ); // indica que ya se cargo un retiro, no se puede regresar con ESC
        }
    }
    BORRAR_MENSAJE();

	/*------------------ si todo OK agrega el retiro --------------------*/
	if( retiro.importe > 0 && LASTKEY(  ) != 27 ) {  
		CARGAR_MEDIOS_RETIRO( nro + 1, retiro.numero, ( _ANULACION ) ? -retiro.importe : retiro.importe, 
			( _ANULACION ) ? -retiro.importe_me : retiro.importe_me, retiro.expandir );
		ADD_MEMORY_DOUBLE( __ram_total, ( _ANULACION ) ? -retiro.importe : retiro.importe );
		SET_MEMORY_CHAR( __ram_p_pago, 1 );
		if( nro >= 0 && medios[nro].tarjeta ) {
			GRABA_MODIFICACION_TARJETAS( -1, 0 );
		}
        if( medios[nro].cheque && MODO_CHEQUE_COMO_UN_MEDIO && !motivo_cheque && tipo_busqueda == _BUSCAR_EN_PAGO_BTR  ) {
            // Marca como retirado en Pago.btr
            //SELECT( AREA_PAGO );
			SELECT_TABLE( T_PAGOS, tipo_t_pagos );
            event_pago.retirado = ( _ANULACION ) ? 0 : 2;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            UPDATE2();
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
        }
    
        if( motivo_cheque ) {
            CONTROL_IMPORTES_ESP( 1, SI, &_codniv1, &_cod, nro, ( double* )&retiro.importe, retiro.numero,
                                  retiro.banco, motivo_cheque, _ANULACION, retiro.numero_cadena );
        }

        // Agrega el nuevo evento.
        CR4_AGREGAR_RETIRO( nro, posicion ); 
        
        //CR4_IMPRIME_RETIRO(); //Se comenta porque imprime detalles de los retiros al final 
        if(medios[nro].tarjeta && !PEDIR_IMPORTE_EN_RETIRO_DE_TARJE){
            VACIAR_IMPORTE_MEDIO(nro + 1);
        }else{
            //si es anulacion se suma, si es retiro se resta (el importe almacenado es lo que tengo en la caja)
		    SUMAR_IMPORTE_SUBMEDIO( nro + 1, retiro.cod_sub_medio, ( _ANULACION ) ? retiro.importe :
		    -retiro.importe );
        }
    }
    if( _ANULACION ) {
        CR4_ACTIVA_ANULACION();
    }
    Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
    PreparePanel( &pPanel, Orden );
    ShowPanel( pPanel );
#ifdef _DEBUG_RNV_REC
    CONTROL_CONSISTENCA_RNV_REC();
#endif
}

/*****************************************************************************/
int CARGAR_MEDIOS_RETIRO( int medio, int submedio, double importe, double importe_me, int expandir )
/*****************************************************************************/
{
	/** Incrementa el importe de un submedio.
	/*@param medio medio de pago ( comienza en 1 )
	@param submedio el codigo del submedio a modificar
	@param importe el importe a sumar al submedio
	@return 1 si se sumo a un submedio existente. 2 si se creo el registro del submedio*/

	if( POSICIONAR_EN_REGISTRO_RETIRO( medio, submedio ) ) {
		submedioRetiro.importe += importe;
		submedioRetiro.importe_me += importe_me;
		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_RETIRO, (char*)&submedioRetiro,
			sizeof( struct _ev_medios ), posicionTrabajoRetiro );
		return 1;                                                                   
	}  
	//No existe el registro, se crea uno nuevo
	submedioRetiro.importe = importe;
	submedioRetiro.importe_me = importe_me;
	submedioRetiro.modo_pago = medio;
	submedioRetiro.cod_sub_medio = submedio;
    submedioRetiro.expandir = expandir;
	GUARDAR_ARCHIVO_REC( ARCH_REC_RETIRO, (char*)&submedioRetiro,
		sizeof( struct _ev_medios ) );    
	return 2;
}
/*****************************************************************************/
int POSICIONAR_EN_REGISTRO_RETIRO( int medio, int submedio )
/*****************************************************************************/
{
	double importe = 0.0;
	
	importe = ( _ANULACION ) ? -retiro.importe : retiro.importe;

	posicionTrabajoRetiro = 0;
    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_RETIRO, ( char* )&submedioRetiro, 
			sizeof( struct _ev_medios ), posicionTrabajoRetiro ) == 0 ) {
		if( medio == submedioRetiro.modo_pago 
				&& submedio == submedioRetiro.cod_sub_medio 
				&& ( ( submedioRetiro.importe > 0 &&  importe > 0 ) 
					|| ( submedioRetiro.importe < 0 &&  importe < 0 ) ) ) {//retiros anulados
            return 1;
        }
        posicionTrabajoRetiro++;
    }
    return 0;
}

/*****************************************************************************/
void CR4_MOSTRAR_RETIRO( void )
/*****************************************************************************/
{
    char pr[15];
    char temp[60];
    char cadena[41];
	/*----------*/
	char *tot=0;
	int larray = ENTEROS +1+DECIMALES+5;
	tot = (char*)calloc(1,larray);
	if(tot!=NULL){
		memset(tot,0,larray);
	}
	/*----------*/
    sprintf( temp, "" );
    cadena[0] = 0;
    /*---------- muestra datos ---------*/
    if( _ANULACION ) {
        strcpy( temp, "-" );
    }
    else {
        strcpy( temp, " " );
    }
    if( retiro.modo == -1 ) {
        strcat( temp, ST( S_FFIJO ) );
    }
    else {
        strcat( temp, medios[retiro.modo].nombre );
    }
    if( config.pais != CHILE && config.pais != BRASIL ) {
        FTOC( retiro.importe, pr, ENTEROS + 1, DECIMALES, 0, 1 );
    }
    else {
        FTOC( retiro.importe, pr, ENTEROS + 1, DECIMALES, 0, 0 );
    }
    if( retiro.numero ) {
        if( retiro.numero_cadena ) {
            sprintf( cadena, "%s", retiro.numero_cadena );
        } else {
            sprintf( cadena, "%09ld", retiro.numero );
        }
    }
    if( retiro.importe_me > 0 ) {
        if( config.pais != CHILE && config.pais != BRASIL ) {
            FTOC( retiro.importe, pr, ENTEROS + 1, DECIMALES, 0, 1 );
        }
        else {
            FTOC( retiro.importe, pr, ENTEROS + 1, DECIMALES, 0, 0 );
        }
    }
    // TOLEDO: De aca en mas crear una funcion para registrar el retiro
    // en la RNV que reciba modo, importe y anulacion.

    ADD_MEMORY_LONG( __nro_correlativo_retiro, 1L );// ???

    agregarPago( 0, temp, pr, cadena, retiro.nombre_submedio );

    if( _ANULACION ) {
        total_retiros -= retiro.importe;
    }
    else {
        total_retiros += retiro.importe;
    }
	/*-------------------*/
	
	ExecuteSAction( A_MUESTRA_TIT_TOTAL_RETIROS,ST( S_TOTAL) );
	FTOC(total_retiros,tot, ENTEROS +1,DECIMALES,1 ,1);
	ExecuteSAction( A_PAGO_MUESTRA_TOTAL, tot);
    FREEMEM(tot);
	//*--------------------*/
}

/*****************************************************************************/
void CR4_AGREGAR_RETIRO( int nro, int posicion )
/*****************************************************************************/
{

	CR4_MOSTRAR_RETIRO(  );
	CR4_ACUMULAR_RETIRO_EN_X( retiro.modo, retiro.importe, _ANULACION, 
		retiro.importe_me, retiro.motivo_cheque, retiro.cantidad );
}

/*****************************************************************************/
void CR4_ACUMULAR_RETIRO_EN_X( int modo, double importe, int anulacion, double importe_me,
                               int motivo, int cantidad )
/*****************************************************************************/
{
    if( !motivo ) {
        /*--------- actualiza acumuladores -----------*/
        if( modo == -1 ) {
            /*------------------- Fondo fijo -----------------------*/
            if( _ANULACION ) {
                ADD_MEMORY_DOUBLE( __x_retiro_fondo_importe, -importe );
                ADD_MEMORY_INT( __x_retiro_fondo_cantidad, (cantidad * -1) );
                ADD_MEMORY_DOUBLE( __x_tot_dinero, importe );
            }
            else {
                ADD_MEMORY_DOUBLE( __x_retiro_fondo_importe, importe );
                ADD_MEMORY_INT( __x_retiro_fondo_cantidad, cantidad );
                ADD_MEMORY_DOUBLE( __x_tot_dinero, -retiro.importe );
            }
        }
        else {
            /*------------------- Medios de pago --------------------*/
            if( _ANULACION ) {
                _ADD_MEMORY_DOUBLE( __x_retiros_importe, modo, -importe );
                _ADD_MEMORY_DOUBLE( __x_retiros_importe_me, modo, -importe_me );
                _ADD_MEMORY_INT( __x_retiros_cantidad, modo, (cantidad * -1) );
                if( medios[modo].efectivo ) {
                    ADD_MEMORY_DOUBLE( __x_tot_dinero, importe );
                }
            }
            else {
                _ADD_MEMORY_DOUBLE( __x_retiros_importe, modo, importe );
                _ADD_MEMORY_DOUBLE( __x_retiros_importe_me, modo, importe_me );
                _ADD_MEMORY_INT( __x_retiros_cantidad, modo, cantidad );
                if( medios[modo].efectivo ) {
                    ADD_MEMORY_DOUBLE( __x_tot_dinero, -importe );
                }
            }
        }
    }
    else {
        if( medios[modo].cheque ) {
            int signo = ( _ANULACION ) ? 1 : -1;
            
            ADD_MEMORY_DOUBLE( __x_cons_final_importe_exent_tkt, signo * importe );//importe total de cheques en caja
            ADD_MEMORY_INT( __x_cons_final_cantidad_tkt, signo );//cantidad total de cheques en caja
            ADD_MEMORY_DOUBLE( __x_cons_final_importe_grav_tkt, ( signo * -1 ) * importe );//importe total de cheques retirados
        }
    }
}
/*****************************************************************************/
void CR4_ACTIVA_ANULACION()
/*****************************************************************************/
{
    if( _ANULACION ) {
        SET_MEMORY_CHAR( _anulacion, 0 );
    }
    else {
        SET_MEMORY_CHAR( _anulacion, 1 );
        MENSAJE_STRING( S_ANULACION );
    }
}
/*****************************************************************************/
double CR4_PIDE_IMPORTE()
/*****************************************************************************/
{
    double importe;
    long mult = 1;
    int j, posx = 56;
    if( config.pais == COLOMBIA || config.pais == CHILE || config.pais == BRASIL
     || config.pais == ELSALVADOR ) {
        posx = 60;
    }
    for( j = 1;j <= DECIMALES;j++ ) {
        mult *= 10;
    }
    importe = PAGOS_PIDE_IMPORTE( ST( S_IMPORTE ), ENTEROS + 1, DECIMALES, NO );
    MOSTRAR_INPUT_CADENA( "     ", 0 );
    //importe = PEDIR_GRAN_FLOTANTE( 34 - ancho_panel / 2, 22, ENTEROS, DECIMALES );
    if( LASTKEY() == '*' && importe != 0 ) {
        SET_MEMORY_DOUBLE( __ram_cantidad,
                           ( PUNTO_DECIMAL_OBLIGATORIO ) ? importe * mult : importe );
        importe = PEDIR_GRAN_FLOTANTE( 34 - ancho_panel / 2, 22, ENTEROS, DECIMALES );
    }
    return ( importe );
}
/*****************************************************************************/
int PIDE_CANTIDAD_DENOMINACION(int k)
/*****************************************************************************/
{
	char cadena[] = {"               "},
		 cadena_pantalla[16];

    
    int posicion=0;
    int rta = 0;
	char pos[2];
	
	while(k!=13)
	{
    switch(k)
	{
	            case 8:
                /*--------- BACKSPACE ---------*/
                posicion--;
                if( posicion < 0 ) {
                    posicion = 0;
                }
                cadena[posicion] = ' ';
                break;
				
				default:
				if( k >= '0' && k <= '9' ) 
				{

					if( posicion < 3 ) {
                        cadena[posicion++] = k;
                    }
                    /*if( posicion == 3 ) {
                        posicion++;
                    }*/
                }
				break;
	}
	strcpy(cadena_pantalla,cadena);
	_snprintf(pos,sizeof(pos),"%d",posicion);
	pos[sizeof(pos)-1]='\0';
	MOSTRAR_INPUT_CANT( cadena_pantalla , pos );
	rta = atoi( cadena );
	k = GETCH();
	}
return rta;
}
/*****************************************************************************/
void CR4_IMPRIME_RETIRO()
/*****************************************************************************/
{
    int m;
    char nombre[20];
    if( MODO_NEGOCIO != SERVICIOS ) {
        m = retiro.modo;
        if( !cabecera ) {
            cabecera = 1;
            IMPRIME_CABECERA_RETIROS( 2, SI );
            OPEN_COPIA_PRN();
        }
        if( config.pais == COLOMBIA ) {
            strcpy( nombre, "BASES" );
        }
        else {
            strncpy( nombre, ST( S_FONDO_FIJO ), sizeof( nombre ) );
        }
        if( retiro.modo == -1 ) {
            IMPRIME_CUERPO_RETIROS( _ANULACION, nombre, retiro.importe, 0, 0, 0.0, 0 );
        }
        else if( medios[m].cheque || medios[m].tarjeta || medios[m].cta_cte
              || medios[m].ticket_compra || medios[m].mutual ) {
            IMPRIME_CUERPO_RETIROS( _ANULACION, medios[retiro.modo].nombre, retiro.importe,
                                    retiro.numero, 0, retiro.importe_me, 0 );
        }
        else {
            IMPRIME_CUERPO_RETIROS( _ANULACION, medios[retiro.modo].nombre, retiro.importe,
                                    retiro.numero, ( int )RAM_CANTIDAD, retiro.importe_me, 0 );
        }
    }
}
/*****************************************************************************/
double CR4_BUSCA_COMPROBANTE( int modo, long id, int todos, int *id_registro, char *cod_sub_medio,
                              short *banco, short tipo_busqueda, double importe, char *id_cadena, 
                              short anulacion, short marca_usado )
/*****************************************************************************/
{
    int      salir = 0, i = 0, pos_pago = 0, estado = 0, ok = 1, iRta = -1;
    double   rta = 0.0;   
    //---temporales
    int      i_temp = 0, cod_sol = -1;
    long     registro = -1, id_evento;
    double   d_temp = 0.0;
    short    bank = 0;
    char     indice_pos_en_pago = -1;
    //---
    //struct   _ind_evdpago_5 indice5;
    //struct   _ind_evdpago_6 indice6;
    switch( tipo_busqueda ) {
        case _BUSCAR_EN_PAGO_BTR:
            //SELECT( AREA_PAGO );
            //GO( TOP );
			SELECT_TABLE( T_PAGOS, tipo_t_pagos );
			SET_WHERE("");
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            while( !dbEOF() && !salir ) {
                if( event_pago.modo_pago - 1 == modo && event_pago.numero_id == id
                 && event_pago.reproceso == 0 ) {
                    if( _ANULACION && event_pago.retirado != 0
                     && event_pago.nro_retiro == NRO_NRO_RETIRO ) {
                        if( !todos ) {
                            salir = 1;
                        }
                        event_pago.retirado = 0;
                        event_pago.nro_retiro = 0;
                        rta += event_pago.importe;
                        if( id_registro != NULL ) {
                            *id_registro = event_pago.id_registro;
                        }
                        if( cod_sub_medio != NULL ) {
                            *cod_sub_medio = event_pago.cod_sub_medio;
                        }
                        if( banco != NULL ) {
                            *banco = event_pago.banco;
                        }
                        #ifdef INVEL_L
                        //BEGIN_TRANSACTION();
                        #endif
                        UPDATE2();
                        #ifdef INVEL_L
                        //END_TRANSACTION();
                        #endif
                    }
                    else if( !_ANULACION && !event_pago.retirado ) {
                        if( !todos ) {
                            salir = 1;
                        }
						if( marca_usado ){
							event_pago.retirado = 1;
							event_pago.nro_retiro = NRO_NRO_RETIRO;
						}
                        
						if( id_registro != NULL ) {
                            *id_registro = event_pago.id_registro;
                        }
                        if( cod_sub_medio != NULL ) {
                            *cod_sub_medio = event_pago.cod_sub_medio;
                        }
                        if( banco != NULL ) {
                            *banco = event_pago.banco;
                        }
						if( event_pago.importe_me != 0 ){
							rta += event_pago.importe_me;
						} else {
							rta += event_pago.importe;
						}
						if( marca_usado ){
							#ifdef INVEL_L
							//BEGIN_TRANSACTION();
							#endif
							UPDATE2();
							#ifdef INVEL_L
							//END_TRANSACTION();
							#endif
						}
                    }
                }
                SKIP2( 1 );
            }
            break;
      
        case _BUSCAR_EN_DPAGO_BTR:
            bank = *banco;
            iRta = 0;
            id_evento = 0;
            //SELECT( AREA_PAGO );
            //GO( TOP );
			SELECT_TABLE( T_PAGOS, tipo_t_pagos );
			SET_WHERE("");
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            while( !dbEOF() ) {
                if( event_pago.modo_pago - 1 == modo && event_pago.datafono == 0 
                    && event_pago.reproceso == 0 ) {
                    id_evento = event_pago.id_evento;
                    indice_pos_en_pago = ( char ) event_pago.posicion;
                    cod_sol = medios[modo].cod_solicitud;
                    // Si se encontrò el pago, busca en DPAGO
                    //SELECT( AREA_DPAGO );
                    //SET_ORDER( 1 );
                    //GET_EQUAL( ( char * ) &id_evento );
                    SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
                    SET_WHERE( "ID_EVENTO = '%ld'", id_evento );
                    RUN_QUERY( NO );
                    if( /*!FOUND( )*/ !FOUND2() ) {
                        ok = 0;                        
                    } else {
                        ok = 1;
                    }
                    if( ok ) {
                        iRta = 1; //hay un pago con cheques
                        // Si encontre el evento en dpago
                            
                        // Esto està hardcodeado, es peligroso hay que tener cuidado con la configuracion de la dmedios: 
                        //  nro_orden 1 = cuit del librador
                        //  nro_orden 2 = nro cheque
                        //  nro_orden 3 = banco
                        //  nro_orden 4 = fecha vencimiento
                        //  nro_orden 5 = importe
                        //SELECT( AREA_DPAGO );
                        //SET_ORDER( 5 );
                        //indice5.id_evento = id_evento;
                        //indice5.cod_solicitud = cod_sol;
                        //indice5.nro_orden = 2; //busco el nro cheque
                        //indice5.indice_posicion = indice_pos_en_pago;
                        //GO( TOP );
                        //GET_EQUAL( ( char * ) &indice5 );
                        SET_WHERE( "ID_EVENTO = '%ld' AND COD_SOLICITUD = '%i' AND NRO_ORDEN = '%i' AND INDICE_POSICION = '%i'",
                            id_evento, cod_sol, 2, indice_pos_en_pago);
                        SET_ORDER2( "ID_EVENTO, COD_SOLICITUD, NRO_ORDEN, INDICE_POSICION" );
                        RUN_QUERY( NO );
                        pos_pago = -1; 
                        ok = 0;
                        while( /*!BtrvEOF()*/ !dbEOF() && pos_pago == -1
                            && event_dpago->id_evento == id_evento/*indice5.id_evento*/
                            && event_dpago->cod_solicitud == cod_sol/*indice5.cod_solicitud*/
                            && event_dpago->nro_orden == 2/*indice5.nro_orden*/
                            && event_dpago->indice_posicion == indice_pos_en_pago/*indice5.indice_posicion*/ ) {                             
                            if( !ANULADO_O_RETIRADO( _ANULACION, event_dpago->anulado, &estado ) ) {
                                // Si encontre el nro de cheque, me fijo si coincide el nro con el ingresado por el cajero.                                
                                if( strncmp( id_cadena, event_dpago->valor, strlen( event_dpago->valor ) ) == 0 ) {
                                    //guardo la posicion del pago para comparar el banco y el importe
                                    pos_pago = event_dpago->pos_pago;                                    
                                    ok = 1;
                                } else {
                                    //no coincide el nro de chq
                                    ok = 0;
                                }
                            } else {
                                //el registro està retirado o anulado
                                ok = 0;
                            }
                            //registro = GET_POSITION( );
                            registro = GET_POSITION_REG( );
                            if( ok && pos_pago >= 0 ) { //si coincidiò el nro cheque, me fijo en el banco
							    ok = 0;
                                //SELECT( AREA_DPAGO );
                                SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
                                //SET_ORDER( 6 );
                                //indice6.id_evento = id_evento;
                                //indice6.cod_solicitud = cod_sol;
                                //indice6.nro_orden = 3; //busco el banco
                                //indice6.pos_pago = pos_pago;
                                //GO( TOP );
                                //GET_EQUAL( ( char * ) &indice6 );
                                SET_WHERE( "ID_EVENTO = '%ld' AND COD_SOLICITUD = '%i' AND NRO_ORDEN = '%i' AND POS_PAGO = '%i'", 
                                    id_evento, cod_sol, 3, pos_pago );
                                SET_ORDER2( "ID_EVENTO, COD_SOLICITUD, NRO_ORDEN, POS_PAGO" );
                                RUN_QUERY( NO );
                                if( /*FOUND( )*/ FOUND2( ) ) {
                                    if( !ANULADO_O_RETIRADO( _ANULACION, event_dpago->anulado, &estado ) ) {
                                        // Si encontre el banco, me fijo si coincide el nro con el ingresado por el cajero.
                                        i_temp = atoi( event_dpago->valor );
                                        if( i_temp == bank ) {
                                            ok = 1;
                                        }
                                    }
                                }
                            }
                            if( ok ) { //si coincidiò el nro cheque y el banco, me fijo en el importe
							    ok = 0;
                                //SELECT( AREA_DPAGO );
                                SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
                                //SET_ORDER( 6 );
                                //indice6.id_evento = id_evento;
                                //indice6.cod_solicitud = cod_sol;
                                //indice6.nro_orden = 5; //busco el importe
                                //indice6.pos_pago = pos_pago;                            
                                //GET_EQUAL( ( char * ) &indice6 );
                                SET_WHERE( "ID_EVENTO = '%ld' AND COD_SOLICITUD = '%i' AND NRO_ORDEN = '%i' AND POS_PAGO = '%i'", 
                                    id_evento, cod_sol, 5, pos_pago );
                                SET_ORDER2( "ID_EVENTO, COD_SOLICITUD, NRO_ORDEN, POS_PAGO" );
                                RUN_QUERY( NO );
                                if( /*FOUND( )*/ FOUND2() ) {
                                    if( !ANULADO_O_RETIRADO( _ANULACION, event_dpago->anulado, &estado ) ) {
                                        // Si encontre el importe entonces es el cheque buscado
                                        d_temp = atof( event_dpago->valor );
                                        if( d_temp == importe ) {
                                            ok = 1;
                                            iRta = -1; //no mostrar mensaje. Se encontro el cheque
                                        }
                                    } else {
                                        iRta = 2; //el cheque se encontrò pero està marcado como retirado o anulado
                                    }
                                }
                            }
                            
                            if( !ok ) {
                                pos_pago = -1;                                
                                //iRta = ( iRta != 2 ) ? 3 : iRta; //no hay un cheque igual al ingresado en la dpago
                            }
                            //GET_DIRECT( registro );
                            //SKIP( 1 );
                            GET_DIRECT_REG( registro, 0 );
                            SKIP2( 1 );
                        }
                        if( ok ) {
                            rta = d_temp;
                            iRta = 99;
                            // si el cheque fue encontrado marco en la dpago.btr como retirado                                        
                            //GO ( TOP );
                            //SELECT ( AREA_DPAGO );
                            //SET_ORDER( 1 );
                            SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
                            SET_WHERE( "" );
                            SET_ORDER2("ID_EVENTO");
                            RUN_QUERY( NO );
                            while( /*!BtrvEOF()*/ !dbEOF() ){
                                if ( event_dpago->id_evento == id_evento 
                                    && event_dpago->cod_solicitud == cod_sol 
                                    && event_dpago->pos_pago == pos_pago ) {
                                        // guardo el nuevo estado de anulado
                                        event_dpago->anulado = estado;
                                        #ifdef INVEL_L
                                        //BEGIN_TRANSACTION();
                                        #endif
                                        UPDATE2();
                                        #ifdef INVEL_L
                                        //END_TRANSACTION();
                                        #endif
                                }
                                //SKIP( 1 );
                                SKIP2( 1 );
                            }
                            
                            if( id_registro != NULL ) {
                                *id_registro = pos_pago;
                            }
                            if( cod_sub_medio != NULL ) {
                                *cod_sub_medio = ( char ) 0;
                            }
                        }
                    }
                }
                //SELECT( AREA_PAGO );
				SELECT_TABLE( T_PAGOS, tipo_t_pagos );
                SKIP2( 1 );
            }
            if( !ok ) {
                iRta = MENSAJE_EN_RETIROS( iRta );
            }
            break;
        case _BUSCAR_EN_PAGO_BTR_ESP:
            //SELECT( AREA_PAGO );
            //GO( TOP );
			SELECT_TABLE( T_PAGOS, tipo_t_pagos );
			SET_WHERE("");
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            while( !dbEOF() && !salir ) {
                if( event_pago.modo_pago - 1 == modo && event_pago.id_registro == *id_registro
                 && event_pago.reproceso == 0 ) {
                    //busco el bco y el cheque exacto por medio del 
                    //cod_nivel1 y codigo de la pagos2 grabadas en la mreinteg
                    if( _codniv1 == event_pago.nro_cupon_int1 &&
                        _cod == event_pago.nro_cupon1 ) {

                        if( _ANULACION && event_pago.retirado != 0
                         && event_pago.nro_retiro == NRO_NRO_RETIRO ) {
                            if( !todos ) {
                                salir = 1;
                            }
                            event_pago.retirado = 0;
                            event_pago.nro_retiro = 0;
                            rta += event_pago.importe;
                            if( id_registro != NULL ) {
                                *id_registro = event_pago.id_registro;
                            }
                            if( cod_sub_medio != NULL ) {
                                *cod_sub_medio = event_pago.cod_sub_medio;
                            }
                            if( banco != NULL ) {
                                *banco = event_pago.banco;
                            }
                            #ifdef INVEL_L
                            //BEGIN_TRANSACTION();
                            #endif
                            UPDATE2();
                            #ifdef INVEL_L
                            //END_TRANSACTION();
                            #endif
                        } else {
                            if( !_ANULACION && !event_pago.retirado ) {
                                if( !todos ) {
                                    salir = 1;
                                }
                                event_pago.retirado = 1;
                                event_pago.nro_retiro = NRO_NRO_RETIRO;
                                if( id_registro != NULL ) {
                                    *id_registro = event_pago.id_registro;
                                }
                                if( cod_sub_medio != NULL ) {
                                    *cod_sub_medio = event_pago.cod_sub_medio;
                                }
                                if( banco != NULL ) {
                                    *banco = event_pago.banco;
                                }
                                rta += event_pago.importe;
                                SET_MEMORY_LONG( __ram_nro_cliente, event_pago.buffer1_long );
                                #ifdef INVEL_L
                                //BEGIN_TRANSACTION();
                                #endif
                                UPDATE2();
                                #ifdef INVEL_L
                                //END_TRANSACTION();
                                #endif
                            }
                        }
                    }
                }
                SKIP2( 1 );
            }
            break; 
    }
    if( !rta ) {
        //MENSAJE_STRING( S_COMPROBANTE_NO_ENCONTRADO );
        if( id_registro != NULL ) {
            *id_registro = 0;
        }
        if( cod_sub_medio != NULL ) {
            *cod_sub_medio = 0;
        }
        if( banco != NULL ) {
            *banco = 0;
        }
    }
    MOSTRAR_INPUT_CADENA( "       ", 0 );
    return ( rta );
}
/*****************************************************************************/
void GRABA_EVENTO_RETIRO( int modo )
/*****************************************************************************/
{
    int h;
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = 7;
    event_ticket.nro_ticket = NRO_NRO_RETIRO;
	event_ticket.nro_comprobante_new = 0;
    event_ticket.bultos = modo;   
    GRABAR_EVENT_TICKET( NO );
    _GRABA_EVENTO_RETIRO( 0, _FONDO_RETIRADO, 0, 0, 0, 0, 0, 0 );
    for( h = 0;h < 10;h++ ) {
        _GRABA_EVENTO_RETIRO( h + 1, acu_retirado[h], 0, 0, 0, 0, 0, 0 );
    }
    ADD_MEMORY_UINT( __nro_nro_retiro, 1 );
}
/*****************************************************************************/
void _GRABA_EVENTO_RETIRO( int medio, double importe, int id_registro, char cod_sub_medio,
                           double importe_me, int motivo_cheque, int posicion, int diferencia )
/*****************************************************************************/
{
    //int posicion = 0;
    // Calcula Posicion
    //SELECT( AREA_PAGO );
    //GET_EQUAL( ( char* )&event_ticket.id_evento );
	SELECT_TABLE( T_PAGOS, tipo_t_pagos );
	SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    while( !dbEOF() && event_ticket.id_evento == event_pago.id_evento ) {
        posicion++;
        //GET_NEXT();
		SKIP2(1);
    }
    if( importe ) {
        memset( &event_pago, 0, sizeof( event_pago ) );
        event_pago.modo_pago = medio;
        event_pago.importe = importe;
        event_pago.id_registro = id_registro;
        event_pago.posicion = posicion;
        if( M_ACUMULADO == medios[medio - 1].modo_arqueo ) {
            event_pago.cod_sub_medio = 0;
        } else {
            event_pago.cod_sub_medio = cod_sub_medio;        
        }
        event_pago.importe_me = importe_me;
        //event_pago.id_registro = _PAGO_ID_REGISTRO( medio - 1 );
		//event_pago.numero_id = _PAGO_NUMERO( medio - 1 );      
		event_pago.banco = submedioRetiro.banco; 
		//event_pago.numero_id = _PAGO_NUMERO( medio - 1 );    
		event_pago.numero_id = submedioRetiro.numero_id;    
        event_pago.datafono = ( long )motivo_cheque;
        //event_pago.retirado = 2; //PARA Q NO SE PUEDA RETIRAR
        //retiro de cheques ingresados para reintegros
		//event_pago.nro_cupon1 = _PAGO_NRO_CUPON1( medio - 1 );
		event_pago.nro_cupon1 = submedioRetiro.nro_cupon1;
		//event_pago.nro_cupon_int1 =_PAGO_NRO_CUPON_INT1( medio - 1 );
		event_pago.nro_cupon_int1 = submedioRetiro.nro_cupon_int1;
		//strncpy( event_pago.nro_tarjeta, _PAGO_NRO_TARJETA( medio - 1 ), SIZEOF_VAR( __pago_nro_tarjeta ) );
		strncpy( event_pago.nro_tarjeta, submedioRetiro.nro_tarjeta, 21 );
        event_pago.buffer1_long = 0 /*_PAGO_BUFFER1_LONG( medio - 1  )*/;
        event_pago.expandir = diferencia;
        GRABAR_EVENT_PAGO( NO );
    }
}
/*****************************************************************************/
void CR4_RESET_ACU_RETIRADO()
/*****************************************************************************/
{
    //SET_MEMORY_DOUBLE( _fondo_retirado, 0 );
    memset( acu_retirado, 0, sizeof( acu_retirado ) );
    memset( acu_retirado_me, 0, sizeof( acu_retirado_me ) );
}
/*****************************************************************************/
void CR4_SET_ACU_RETIRADO( int medio, double importe )
/*****************************************************************************/
{
    if( !medio ) {
		SET_MEMORY_DOUBLE( _fondo_retirado, importe );
    }
    else if( medio > 0 && medio < 10 ) {
        acu_retirado[medio - 1] += importe;
    }
}
/*****************************************************************************/
int RETIRO_REMOTO( char *param )
/*****************************************************************************/
{
    int h,c,error,medios[3];
    double importes[3];
    CR4_RESET_ACU_RETIRADO();
	SET_MEMORY_DOUBLE( _fondo_retirado, 0 );
    c = sscanf( param, "%i %lf %i %lf %i %lf", &medios[0], &importes[0], &medios[1], &importes[1],
                &medios[2], &importes[2] );
    for( h = 0;h < c / 2;h++ ) {
        CR4_SET_ACU_RETIRADO( medios[h], importes[h] );
        CR4_ACUMULAR_RETIRO_EN_X( medios[h] - 1, importes[h], NO, 0.0, 0,1 );
    }
    
    GRABA_EVENTO_RETIRO( 1 );
    error = ( c / 2 > 0 ) ? 0 : 1;
    return ( error );
}
/*****************************************************************************/
void GRABA_EVENTO_CABECERA_RETIRO( double importe_retiro )
/*****************************************************************************/
{
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = 7;
	event_ticket.nro_comprobante_new = 0;
	event_ticket.nro_ticket = 0;
	if( importe_retiro ) {
		event_ticket.nro_ticket = NRO_NRO_RETIRO;
	} 
	event_ticket.importe_sin_iva = importe_retiro;
    GRABAR_EVENT_TICKET( NO );
}
extern struct _tarjetas *tarjetas_sic;
/*****************************************************************************/
int PIDE_TARJETA( int *id_registro, int solicitar_submedio )
/*****************************************************************************/
{
    int rta = 0, ok = 0;
    struct _tarjetas tarjeta_sic;
    tarjetas_sic = &tarjeta_sic;
    /*---------------------------- Abre el archivo ---------------------------*/
    if( /*USE_DB( AREA_AUX, _tarjetas, ( char* )&tarjeta_sic, sizeof( tarjeta_sic ), NULL, 0, 0 )*/
    OPEN_TABLE(T_TARJETAS, TT_ORIG,( char* )&tarjeta_sic,sizeof( tarjeta_sic )) == 0 ) {
        /*-------------------------- Prepara la pantalla -------------------------*/
		SELECT_TABLE( T_TARJETAS, TT_ORIG );
        if( solicitar_submedio ) {
            ok = Consulta_elegir_tarjeta( _elegir_tarjeta );
        } else {
            ok = 1;
        }
        if( ok ) {
            rta = tarjeta_sic.cod_tarjeta;
            if( tarjeta_sic.id_registro && *id_registro ) {
                *id_registro = tarjeta_sic.id_registro;
            }
        }
        CLOSE_TABLE( T_TARJETAS, TT_ORIG );
    }
    BORRAR_MENSAJE();

    return ( rta );
}

/*****************************************************************************/
int CONTROL_IMPORTES( int nro )
/*****************************************************************************/
{
    if( CONTROL_DE_IMPORTES_EN_RETIROS
     && ( retiro.importe
        > ( _X_VENTAS_IMPORTE( nro ) + _X_COBROS_IMPORTE( nro ) 
		+ _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) - _X_RETIROS_IMPORTE( nro )) + 0.02 ) ) {
        MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
        BORRAR_MENSAJE();
        retiro.importe = 0;
        return( 1 );
    }
    return( 0 );
}

/*****************************************************************************/
int CONTROL_IMPORTES_ESP( int indice, int actualiza, long *codnivel1, long *codigo, 
                         int nro, double *importe, long nro_cheque, int banco, 
                         int motivo, short anulacion, char *nro_cheque_str )
/*****************************************************************************/
{
    #if !defined (ALTA_RAPIDA_REINTEGROS)  
    double total_en_caja = 0.0;
    total_en_caja = _X_VENTAS_IMPORTE( nro ) + _X_COBROS_IMPORTE( nro )
                  + _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) - X_PAGOS_POR_CAJA_IMPORTE
                  - _X_RETIROS_IMPORTE( nro ) + 0.02;
    if( *importe > total_en_caja ) {
        MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
        GRABAR_LOG_SISTEMA_STRING( S_IMPORTE_EXCEDIDO );
        BORRAR_MENSAJE();
        *importe = 0;
        return 0;
    }
    #else
    
    int rta = 0;
    //int area_ant = 0;
    //area_ant = SELECTED();  
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
	
    if( !OPENED( T_PAGOS2, TT_ORIG ) ) {
        OPEN_TABLE(T_PAGOS2, TT_ORIG,(char *)&pagos2, sizeof(pagos2));
    }
    SELECT_TABLE( T_PAGOS2, TT_ORIG );
    /* Si es reintegro en efectivo */
    //if( nro_cheque == 0 || strlen( nro_cheque_str ) == 0 ) {
    if( !medios[nro].cheque ) {
        SET_WHERE( "COD_NIVEL1 = %ld and CODIGO = %ld", *codnivel1, *codigo );
        RUN_QUERY(NO);
        /*SET_ORDER( 1 );
        ind1_pagos2.cod_nivel1 = *codnivel1;
        ind1_pagos2.codigo = *codigo;
        GET_EQUAL( ( char* )&ind1_pagos2 );*/
        if( FOUND2() ) {
            rta = 1;
        }
    }
    else {
        if( medios[nro].cheque ) {
            rta = 0;
            if( *codnivel1 && *codigo ) { //estoy en pagos_esp
                rta = 1;
            } else { //estoy en retiros
                struct _ind_pagos2_4 ind4;                
                rta = -1;
                //busco el cheque en la pagos2 pos si desde la base de datos me lo eliminaron
                ind4.cod_nivel1 = 1; //hardcodeado porq por ahora hay solo un registro en la pagos1
                ind4.banco      = banco;
                SET_WHERE( "COD_NIVEL1 = %i and banco = %i", ind4.cod_nivel1, ind4.banco );
                RUN_QUERY(NO);
                ////SET_ORDER( 4 ); //cod_nivel1+banco
                ////GO( TOP );
                ////GET_GREATER_E( ( char* )&ind4 );
                while( rta == -1 && !dbEOF() && ( pagos2.cod_nivel1 == ind4.cod_nivel1 && pagos2.banco == ind4.banco ) ) {                    
                    if( strncmp( pagos2.nro_chq_str, nro_cheque_str, strlen( nro_cheque_str ) ) == 0 ) {                        
                        rta = 1;
                        *codnivel1 = pagos2.cod_nivel1;
                        *codigo    = pagos2.codigo;
                        break;
                    }
                    if( rta == -1 ) {
                        //si lo encuentro me posiciono para el caso de la anulacion esto es necesario
                        ////GET_NEXT();
                        SKIP2( 1 );
                    }
                }
            }
        }
    }
    if( rta <= 0 ) {
        if( rta == 0 ) {
            MENSAJE_CON_PAUSA( "NO HAY INFORMACION PARA LOS DATOS INGRESADOS", 25 );
            BORRAR_MENSAJE();
            return( 0 );
        } else {
            MENSAJE_CON_PAUSA( "INCONSISTENCIA EN LOS DATOS", 25 );
            rta = 1; //sigo para poder retirar de la mreinteg
        }
        BORRAR_MENSAJE();        
    }   
    //SELECT( area_ant ); 
	SELECT_TABLE( tabla_ant, tipo_ant );
    if( medios[nro].cheque ) {   
        int ok = 1;
        struct _medios_para_reintegros medios_para_reintegros; 
        struct _ind_mreintegro_1 ind_mreintegro_1;
        
        switch( indice ) {
            case 1:
                ind_mreintegro_1.cod_nivel1 = *codnivel1;
                ind_mreintegro_1.codigo = *codigo;
                break;
        }
        //if( USED( AREA_MEDREINT ) ) {
        //    CLOSE_DATABASE( AREA_MEDREINT );
        //}
		if( OPENED( T_MEDIOS_REINT, TT_ORIG )) {
			CLOSE_TABLE(T_MEDIOS_REINT, TT_ORIG );
		}
        
      //  if( USE_DB( AREA_MEDREINT, _MEDIOS_P_REINTEGROS, ( char* )&medios_para_reintegros,
       //     sizeof( medios_para_reintegros ), NULL, 0, 0 ) == 0 ) {
		if( OPEN_TABLE( T_MEDIOS_REINT, TT_ORIG, ( char* )&medios_para_reintegros, sizeof( medios_para_reintegros ) ) == 0 ) {			
            ok = 1;
        } else {
            ok = 0;
            return( 0 );
        }
        if( ok && !_ANULACION ) {
                    //SELECT( AREA_MEDREINT );
                    //GO( TOP );
                    //SET_ORDER( 1 );         
                    //GET_EQUAL( ( char* )&ind_mreintegro_1 );
					SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
					SET_WHERE("COD_NIVEL1 = '%ld' and CODIGO = '%ld'", ind_mreintegro_1.cod_nivel1, ind_mreintegro_1.codigo );
					SET_ORDER2( "COD_NIVEL1, CODIGO" );	
					RUN_QUERY(NO);
					ok = 0;

                    if( FOUND2() ) {
                        ok = 1;
                    }
                    if( !ok ) {
                        MENSAJE_CON_PAUSA( "EL MEDIO NO FUE CARGADO EN EL POS", 25 );
                        //CLOSE_DATABASE( AREA_MEDREINT );
						CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
                        return( 0 );
                    }
                    else {
                        if( medios_para_reintegros.estado == _CHQ_ENVIADO ) {
                            if( actualiza ) {
                                #ifdef INVEL_L
                               // BEGIN_TRANSACTION();
                                #endif                     
                                DELETE2();    
                                #ifdef INVEL_L
                               // END_TRANSACTION();
                                #endif
                            }
                        }
                        else {
                            MENSAJE_CON_PAUSA( "EL REINTEGRO YA FUE ENTREGADO", 25 );
                            return( 0 );
                        }
                    }
        } else { //en anulacion de cheques para reintegros
            if( ok && _ANULACION && actualiza ) {
               // SELECT( AREA_MEDREINT );
				SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
                medios_para_reintegros.banco = pagos2.banco;
                medios_para_reintegros.cod_medio = pagos2.cod_medio;
                medios_para_reintegros.cod_nivel1 = pagos2.cod_nivel1;
                medios_para_reintegros.cod_sub_medio = pagos2.cod_sub_medio;
                medios_para_reintegros.codigo = pagos2.codigo;        
                medios_para_reintegros.idlong = pagos2.idlong;        
                medios_para_reintegros.importe = pagos2.importe;
                medios_para_reintegros.nro_cheque = pagos2.nro_cheque;
                medios_para_reintegros.estado = _CHQ_ENVIADO;//en la caja y enviado al dbrouter,        
                memset( medios_para_reintegros.nro_chq_str, 0, strlen( medios_para_reintegros.nro_chq_str ) );
                strcpy( medios_para_reintegros.nro_chq_str, pagos2.nro_chq_str );        
        
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                INSERT2( NO );
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
            }
        }
    }
    else {
        if( medios[nro].moneda_extranjera ) {
           /* if( CONTROL_DE_IMPORTES_EN_RETIROS
             && ( retiro.importe_me > _X_VENTAS_IMPORTE_ME( nro ) + _X_COBROS_IMPORTE_ME( nro )
                + _X_FONDO_FIJO_MULTIPLE_IMP_ME( nro ) - X_PAGOS_POR_CAJA_IMPORTE
                - _X_RETIROS_IMPORTE_ME( nro ) + 0.02 ) ) {
               MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                retiro.importe = 0;
            }*/
        }
        else {
            //efectivo y otros medios que no sean cheques ni moneda extranjera
            double total_en_caja = 0.0;
            if( *importe > 0 ) {
                if( *importe > X_TOT_DINERO ) {
                    MENSAJE_STRING( S_NO_HAY_EFECTIVO_SUFICIENTE );
                    GRABAR_LOG_SISTEMA_STRING( S_NO_HAY_EFECTIVO_SUFICIENTE, LOG_VENTAS,2 );
                    *importe = 0;
                    BORRAR_MENSAJE();
                    return( 0 );
                }
            }
        }
    }
    #endif
    BORRAR_MENSAJE();
    return 1;
}
/***************************************************************************/
int ELEGIR_MOTIVO_RETIRO( int *cod_solicitud )
/***************************************************************************/
{
    int rta = 0;
    struct _motivos motivo_sic;
    p2_motivo_sic = &motivo_sic;
    /*---------------------------- Abre el archivo ---------------------------*/
	if( OPEN_TABLE( T_MOTIVOS2, TT_ORIG, ( char* )&motivo_sic, sizeof( struct _motivos ) ) ) {
        /*-------------------------- Prepara la pantalla -------------------------*/
        rta = Consulta_elegir_motivo( _elegir_motivo2 );
        if( rta ) {
            rta = motivo_sic.cod_motivo;
            if( cod_solicitud != NULL ) {
                *cod_solicitud = motivo_sic.cod_solicitud;
            }
        }
		CLOSE_TABLE( T_MOTIVOS2, TT_ORIG );
    }
    return ( rta );
}
/*****************************************************************************/
int _elegir_motivo2( int comando, char *cadena, int dato, struct _browse *b, double monto )
/*****************************************************************************/
{
    int rta = 0;
    int cod;
    switch( comando ) {
        case _BUSCAR:
            MOSTRAR_CADENA( 29, 20, cadena );
            cod = atoi( cadena );
			SET_WHERE( "COD_MOTIVO >= '%i'", cod );
			RUN_QUERY( NO );
            break;
        case _MOSTRAR:
            {
                char xx[10];
                _itoa( p2_motivo_sic->cod_motivo, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, p2_motivo_sic->des_motivo );
            }   
            break;
        case _SKIP:
			SKIP2( dato );
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
			rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
        case _TECLA:
            //if( b->tecla == 27 ) {
            //  b->tecla = 0;
            //  rta = 1;
            //}
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int ANULADO_O_RETIRADO( int en_anulacion, char estado, int *est  )
/*****************************************************************************/
{
    int rta = 0;
    if( estado == _CHQ_REVERSADO ) {
        //no lo puedo retirar porq esta reversado
        return( 1 );
    }
// Si encontre el evento en dpago, me fijo si no fue retirado o anulado.
    if ( en_anulacion ) {
        if ( estado == _CHQ_ANULADO ) {
            rta = 1;
        } else {
            if ( estado == _CHQ_RETIRADO ) {
                //LO PUEDO ANULAR SI LO RETIRÈ
                *est = _CHQ_ANULADO;
            } else {
                rta = 1;
            }
        }
    } else {
        if ( estado == _CHQ_RETIRADO ) {
            //No lo puedo retirar porq fue retirado antes
            rta = 1;
        } else {
            *est = _CHQ_RETIRADO;
        }
    }
    return( rta );
}
/*****************************************************************************/
int MENSAJE_EN_RETIROS( int iRta )
/*****************************************************************************/
{
    switch( iRta ) {
    case -1:
        //no mostrar mensaje porq se encontro el cheque
        break;
    case 0:        
        //no se encontro un pago con cheques
        //MENSAJE( "NO SE ENCONTRO PAGOS CON CHEQUES" );
        //GRABAR_LOG_SISTEMA( "NO SE ENCONTRO PAGOS CON CHEQUES" );
        break;
    case 1:
        //hay pagos con cheque pero no se encontro el buscado
        MENSAJE( "NO SE ENCONTRO EL CHEQUE" );
        break;
    case 2:
        //el cheque se encontrò pero està marcado como retirado o anulado
        MENSAJE( "EL CHEQUE ESTA PROCESADO" );
        break;
    case 3://no hay un cheque igual al ingresado en la dpago
        MENSAJE( "NO SE ENCONTRO EL CHEQUE" );
        break;
    default:
        //todo ok
        GRABAR_LOG_SISTEMA( "CHEQUE OK", LOG_RETIROS,4 );
    }                                       
    return ( iRta );
}

/***************************************************************************/
void ACUMULA_RETIRADO( int medio, int submedio, double importe )
/***************************************************************************/
{
    acu_ret[medio][submedio] += importe;
}

/***************************************************************************/
double OBTIENE_RETIRADO( int medio, int submedio )
/***************************************************************************/
{
    return acu_ret[medio][submedio];
}

/** La cantidad de submedios que maneja retiros.
/* Se debe consultar esto siempre que se utilicen ACUMULA_RETIRADO u OBTIENE_RETIRADO
/* @return la cantidad de submedios.
/***************************************************************************/
int OBTIENE_CANT_SUB_RETIRO()
/***************************************************************************/
{
    return CANT_SUB_RET;
}
