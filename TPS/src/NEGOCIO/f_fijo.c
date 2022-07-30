#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <b_mens.h> 
#include <cajon.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <ftoc.h> 
#include <f_fijo.h> 
#include <getch.h> 
#include <imp_fond.h> 
#include <log.h> 
#include <malloc.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <m_entero.h> 
#include <pagos.h> 
#include <pago_mut.h> 
#include <pago_tc.h> 
#include <pant.h> 
#include <print.h> 
#include <stack.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <actions.h>
#include <typeacti.h>
#include <DefTables.h>
#include <_cr0.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
//void FONDO_FIJO_MULTIPLE_CONFECCIONA_PANTALLA(  );
int FONDO_FIJO_CARGA_MEDIOS( void );
void FONDO_FIJO_INGRESA_MEDIO_AUT( int medio, int submedio, double importe );
extern double    MUTUALES_ME_A_ML( double importe_me, int h );

/*****************************************************************************/
void CR1_FONDO_FIJO()
/*****************************************************************************/
{
    int _ptr,_handle,_estado;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    /*---------------------------- Actualiza estado --------------------------*/
    SETEAR_TIEMPO( _ENTRADA_NO_VENTA );
    /*------------------------------ Pide importe ----------------------------*/
    SET_MEMORY_DOUBLE( __ram_importe,
                       PEDIR_FLOAT_EN_RECUADRO_CC( ST( S_FFIJO ), SI, ENTEROS, DECIMALES, SI, SI,
                                                   SI, SI, NO, RAM_COD_FUNCION ) );
    if( RAM_IMPORTE > 0.0 ) {
        if( IMPRIME_FONDO_FIJO == 3 ) {
            MENSAJE_TECLA_STRING( S_INGRESE_COMPROBANTE_FONDO_FIJO_ENTER );
            PRN_HABILITA_SLIP( 1, NO );
        }
        GRABAR_LOG_SUPERVISOR_STRING( S_FFIJO, USUARIO_CLAVE_ANTERIOR() ,LOG_VENTAS,4);
        _SET_ESTADO( 1 );
    }
    /*------------------------- actualiza acumuladores -----------------------*/
    if( _ESTADO == 1 ) {
        ADD_MEMORY_DOUBLE( __x_fondo_fijo_importe, RAM_IMPORTE );
        ADD_MEMORY_INT( __x_fondo_fijo_cantidad, 1 );
        ADD_MEMORY_DOUBLE( __x_tot_dinero, RAM_IMPORTE );
        _SET_ESTADO( 2 );
    }
    /*------------------------- Emite un comprobante -------------------------*/
    if( _ESTADO == 2 ) {
        
        memset( &event_ticket, 0, sizeof( event_ticket ) );
        event_ticket.tipo_evento = 25;
		event_ticket.nro_comprobante_new = 0;
		event_ticket.nro_ticket = 0;
        event_ticket.importe_sin_iva = RAM_IMPORTE;
        SETEAR_TIEMPO( _INDEFINIDO );
        SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
        GRABAR_EVENT_TICKET( NO );
        ACUMULAR_TIEMPOS( );
        CEREAR_TIEMPOS( );
        SETEAR_TIEMPO( _ENTRADA_OCIO );
        memset( &event_pago, 0, sizeof( event_pago ) );
        event_pago.importe = RAM_IMPORTE;
        event_pago.modo_pago = _medio_efectivo  + 1;
        GRABAR_EVENT_PAGO( NO );
        ACTUALIZA_ID_EVENTO_NEW();
		ACTUALIZAR_X();
        _SET_ESTADO( 3 );
    }
    if( _ESTADO == 3 ) {
        TICKET_FONDO_FIJO( RAM_IMPORTE, CAJERO );
        if( COMP_SUPERVISOR_FONDO_FIJO ) {
            TICKET_FONDO_FIJO( RAM_IMPORTE, SUPERVISOR );
        }
        if( IMPRIME_FONDO_FIJO == 3 ) {
            PRN_INHABILITA_SLIP();
            PRN_SELECT( RECEIPT );
        }
        _SET_ESTADO( 4 );
    }
    //---------------- Cierra el cajn --------------/
    CERRAR_CAJON();
    //---------- Termina recupero de memoria ------------------/
    CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
int CR1_FONDO_FIJO_MULT_MEDIOS( int fondo_fijo_automatico )
/*****************************************************************************/
{
    int rta,h;
    int _ptr,_handle,_estado;
    //--------------------- Prepara recupero de memoria ----------------------
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    //----------------------- Setea variables globales -----------------------
    if( _ESTADO == 0 ) {
        SET_MEMORY_CHAR( __ram_p_pago, 0 );
        SET_MEMORY_DOUBLE( __ram_importe, 0 );
        _SET_ESTADO( 1 );
    }
    //----------------------------- ingresa pagos ----------------------------
    rta = ( fondo_fijo_automatico ) ? 1 : 
			SOLICITAR_CLAVES( 30, 21, SI, SI, SI, ST( S_FFIJO ), 
				ST( S_FFIJO ), RAM_COD_FUNCION, SI );
    if( rta ) {
        /*---------------------------- Actualiza estado --------------------------*/
        SETEAR_TIEMPO( _ENTRADA_NO_VENTA );
        //-------------------------- Prepara pantalla ----------------------------
        if( !fondo_fijo_automatico ) {
			ExecuteAction( A_DES_VENTAS_ACT_FFIJO );
			ExecuteSAction( A_MUESTRA_TITULO_FFIJO, ST( S_FFIJO ) );
		} else {
			ABRIR_CAJON( );
			//SET_MEMORY_LONG( __nro_legajo_supervisor, 0L );
			rta = FONDO_FIJO_CARGA_MEDIOS( );
        }
        if( !fondo_fijo_automatico ) {
			FONDO_FIJO_MULTIPLE_CONFECCIONA_PANTALLA();
			rta = FONDO_FIJO_PIDE_MEDIO();
		}
        if( rta ) {
			//No puede grabar el evento supervisor y no grabar el evento donde interviene el supervisor
            if( RAM_IMPORTE > 0.0 ) {
                if( IMPRIME_FONDO_FIJO == 3 ) {
                    MENSAJE_TECLA_STRING( S_INGRESE_COMPROBANTE_FONDO_FIJO_ENTER );
                    PRN_HABILITA_SLIP( 1, NO );
                }
                GRABAR_LOG_SUPERVISOR_STRING( S_FFIJO, USUARIO_CLAVE_ANTERIOR(),LOG_VENTAS,4 );
                _SET_ESTADO( 1 );
            }
            //------------------------- actualiza acumuladores -----------------------
            if( _ESTADO == 1 ) {
                
                memset( &event_ticket, 0, sizeof( event_ticket ) );
                event_ticket.tipo_evento = ( fondo_fijo_automatico ) ? 57 : 25;
				event_ticket.nro_comprobante_new = 0;
				event_ticket.nro_ticket = 0;
                event_ticket.importe_sin_iva = RAM_IMPORTE;
                SETEAR_TIEMPO( _INDEFINIDO );
                SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
                GRABAR_EVENT_TICKET( NO );
                ACUMULAR_TIEMPOS( );
                CEREAR_TIEMPOS( );
                SETEAR_TIEMPO( _ENTRADA_OCIO );
                _SET_ESTADO( 2 );
            }
            if( _ESTADO == 2 ) {
                for( h = 0;h < RAM_P_PAGO;h++ ) {
                    _ADD_MEMORY_DOUBLE( __x_fondo_fijo_multiple_importe, _PAGO_MODO( h ),
                                        _PAGO_IMPORTE( h ) );
                    _ADD_MEMORY_DOUBLE( __x_fondo_fijo_multiple_imp_me, _PAGO_MODO( h ),
                                        _PAGO_IMPORTE_ME( h ) );
                    _ADD_MEMORY_INT( __x_fondo_fijo_multiple_cantidad, _PAGO_MODO( h ), 1 );
                    if( medios[_PAGO_MODO( h )].efectivo ) {
                        ADD_MEMORY_DOUBLE( __x_tot_dinero, _PAGO_IMPORTE( h ) );
                        ADD_MEMORY_DOUBLE( __x_fondo_fijo_importe, _PAGO_IMPORTE( h ) );
                        ADD_MEMORY_INT( __x_fondo_fijo_cantidad, 1 );
                    }
                    memset( &event_pago, 0, sizeof( event_pago ) );
                    if( medios[_PAGO_MODO( h )].cheque ) {
                        event_pago.numero_id = _PAGO_NUMERO( h );
                    }
                    else {
                        event_pago.numero_id = _PAGO_CUENTA( h );
                    }
                    event_pago.modo_pago = _PAGO_MODO( h ) + 1;
                    event_pago.importe = _PAGO_IMPORTE( h );
                    event_pago.id_registro = _PAGO_ID_REGISTRO( h );
                    event_pago.posicion = h;
                    event_pago.importe_me = _PAGO_IMPORTE_ME( h );
                    event_pago.banco = _PAGO_BANCO( h );               
                    event_pago.buffer1_long = _PAGO_BUFFER1_LONG( h );
                    GRABAR_EVENT_PAGO( NO );
                }
                _SET_ESTADO( 3 );
                ACTUALIZA_ID_EVENTO_NEW();
            }
            //------------------------- Emite un comprobante -------------------------
            if( _ESTADO == 3 ) {
                TICKET_FONDO_FIJO( RAM_IMPORTE, CAJERO );
                if( COMP_SUPERVISOR_FONDO_FIJO ) {
                    TICKET_FONDO_FIJO( RAM_IMPORTE, SUPERVISOR );
                }
                if( IMPRIME_FONDO_FIJO == _ESTACION_SLIP ) {
                    EJECT();
                    LF( 0 );
                    setData( ACTION_PRINT, NULL, 0 );
                    PRN_INHABILITA_SLIP();
                    PRN_SELECT( RECEIPT );
                }
                ACTUALIZAR_X();
                _SET_ESTADO( 4 );
            }
            //---------------- Cierra el cajn ---------------
            CERRAR_CAJON();
        }                                                    
        ExecuteAction( A_DES_FFIJO_ACT_VENTAS );

    }
    //-------------------- Termina recupero de memoria -----------------------
    CLOSE_RECUPERO( _ptr, _handle );
    return ( rta );
}
/*****************************************************************************/
void FONDO_FIJO_MULTIPLE_CONFECCIONA_PANTALLA()
/*****************************************************************************/
{
    int h;
    char tt[30];
    ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );     
    ExecuteAction( A_BORRAR_MEDIOS );
    for( h = 0;h < 9;h++ ) {
        if( medios[h].nro && medios[h].permite_fondo_fijo == 'S'
         && MEDIO_DE_PAGO_HABILITADO( 0, h, NO ) ) {
            // MODO
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
                    GRABAR_LOG_SISTEMA( "MEDIO NO DEFINIDO" ,LOG_PANTALLAS,3);
            }
            #else
            sprintf( tt, "%2d  ", medios[h].nro );
            strcat( tt, medios[h].nombre );
            ExecuteSAction( A_AGREGAR_MEDIO, tt );
            #endif
            MOSTRAR_ENTERO( 7, 9 + h, "9", medios[h].nro );
            MOSTRAR_CADENA( 9, 9 + h, medios[h].nombre );
        }
    }
    /*------------ Muestra datos ------------*/
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        MOSTRAR_FONDO_FIJO( h );
    }
}
/*****************************************************************************/
int FONDO_FIJO_PIDE_MEDIO()
/*****************************************************************************/
{
    int seguir = 1, k, _k = 0, rta = 0;
    while( seguir ) {
        if( _k ) {
            k = _k;
            _k = 0;
        }
        else {
            k = GETCH();
        }
        switch( k ) {
            case 27:
                /*-------- ESC ---------*/
                seguir = 0;
                rta = 0;
                BORRAR_MENSAJE();
                break;
            case 32:
                /*-------- TOTAL -------*/
				if( RAM_IMPORTE > 0 ) {
                MENSAJE_STRING( S_PRESIONE_ENTER_PARA_ACEPTAR_EL_FONDO_FIJO );
                do{
                    k = GETCH();
                }
                while(k == -999);
                if( k == 13 ) {
                    ABRIR_CAJON();
                    rta = 1;
                    seguir = 0;
                    BORRAR_MENSAJE();
                }
                BORRAR_MENSAJE();
				}
                break;
            default:
                if( k >= '1' && k <= '9' && RAM_P_PAGO < 9 ) {
                    if( medios[k - '1'].nro && medios[k - '1'].permite_fondo_fijo == 'S'
                     && MEDIO_DE_PAGO_HABILITADO( 0, k - '1', NO ) ) {
                        #if defined(INVEL_W) || defined(INVEL_L)
                        ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__IMPORTE_ ), "", NULL, " " );
                        ExecuteSSAction( A_RESALTAR_MEDIO_PFF, k - '1', "", "", NULL, " " );
                        #endif
                        if( GetConfig( CONF_TOUCH_SCREEN ) ) {
                            /* Informo al touch screen un cambio de pantalla */
                            ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "20" );
                            FONDO_FIJO_INGRESA_MEDIO( k - '1' );
                            ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "06" );
						}else{
                            FONDO_FIJO_INGRESA_MEDIO( k - '1' );
                        }
                        #if defined(INVEL_W) || defined(INVEL_L)
                        ExecuteSSAction( A_MUESTRA_TITULO_BOX, 0, ST( S__IMPORTE_ ), "", NULL, " " );
                        ExecuteAction( A_DESELECCIONA_MEDIO );
                        #endif
                    }
                }
                break;
        }
        if( RAM_ANULAR_TICKET ) {
            rta = 0;
            seguir = 0;
        }
    }
    return ( rta );
}
/*****************************************************************************/
void FONDO_FIJO_INGRESA_MEDIO( int nro )
/*****************************************************************************/
{
    int ok = 1, h, pos = -1;
    long autorizacion = 0;
    int cuenta = 0, id_registro;
    double importe, importe_me = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    int habia_tarjeta = VAR_TARJETAS_STATUS;
    #endif
    long numero = 0;
    int banco = 0;
    unsigned fecha_vto = 0;
    int cod_post_cheq = 0;
    int cod_hora_acre = 0;
    char cuit[12],telefono[12],cuenta_banco[20],nro_banco[16],serie[8];
    int plaza = 0;
    int habilitado = 0, cod_valida_cheque;
    /*-------------- valida que no repita efectivo ---------------*/
    if( medios[nro].efectivo ) {
        for( h = 0;h < RAM_P_PAGO;h++ ) {
            if( _PAGO_MODO( h ) == nro ) {
                pos = h;
            }
        }
    }
    /*-------------- valida que no repita tarjetas ---------------*/
    if( medios[nro].tarjeta ) {
        for( h = 0;h < RAM_P_PAGO;h++ ) {
            if( _PAGO_MODO( h ) == nro ) {
                pos = h;
            }
        }
    }
	if( medios[nro].cheque && config.pais != ELSALVADOR ) {
        // Ver!!!. Se pasa por parametro el pedir_fecha_vto = SI pero no se graba el campo en la ev_pago
		// porque no se encuentra __pago_fecha_vto a la hs de la grabación.
		ok = CARGAR_CHEQUE_MANUAL( ( medios[nro].al_dia ? 1 : 0 ), &numero, &banco, &fecha_vto,
                                   &cod_post_cheq, &cod_hora_acre, cuit, telefono, cuenta_banco,
                                   &plaza, nro_banco, habilitado, serie, &cod_valida_cheque, SI );
    }
    /*------------- pide datos ----------------*/
    if( ok ) {
        importe = PAGOS_PIDE_IMPORTE( ST( S_IMPORTE ), ENTEROS, DECIMALES, NO );
        MOSTRAR_INPUT_CADENA( "     ", 0 );
    }
    if( importe > 0.0 ) {
        /*---------------------- Calcula importe -----------------------*/
        PAGOS_MUESTRA_IMPORTE( importe );
        /*--------------- ID del registro ---------------*/
        id_registro = medios[nro].id_registro;
        /*---------------- Si es ticket de compra ----------------*/
        if( medios[nro].ticket_compra && ok && PEDIR_SUBMEDIO_EN_FONDO_FIJO ) {
            /*-------------- pide tipo de ticket ----------------*/
			cuenta = PIDE_TICKET_COMPRA( &id_registro, NULL, NULL, NULL, NULL, NULL, NULL, medios[nro].nro, 
                NO, NULL, SI );
            if( cuenta <= 0 ) {
                ok = 0;
            }
        }
        /*---------------- Si es mutual ----------------*/
        #ifdef COMPILAR_MUTUALES
        if( medios[nro].mutual && ok && PEDIR_SUBMEDIO_EN_FONDO_FIJO ) {
            /*-------------- pide mutual FONDO FIJO -----*/
            if( MUTUAL_CODIGO ) {
                cuenta = MUTUAL_CODIGO;
            }
            else {
                cuenta = PIDE_MUTUAL( &id_registro, NULL, NULL, NULL, 
					NULL, NULL, NULL, medios[nro].nro, 0.0, NO, NULL, SI );
            }
            if( cuenta <= 0 ) {
                ok = 0;
            }
        }
        #endif
        /*---------------- Si es moneda extranjera ----------------*/
        if( medios[nro].moneda_extranjera && ok ) {
            importe_me = importe;
            importe = PAGOS_ME_A_ML( importe_me, nro );
        }
        /*-------------- pide autorizacion ----------------*/
        if( medios[nro].autorizacion == 1 && ok && !medios[nro].cta_cte ) {
            autorizacion = PAGOS_PIDE_AUTORIZACION( ST( S_AUTORIZACION ) );
            if( autorizacion <= 0 ) {
                ok = 0;
            }
        }
        /*------------------ si todo OK agrega el pago --------------------*/
        if( ok ) {
            FONDO_FIJO_AGREGAR_MEDIO( pos, ( char )nro, importe, cuenta, id_registro, importe_me,
                                      banco, numero, autorizacion, SI );
        }
    }
    CR1_MUESTRA_TARJETA();
    /*
    #ifdef INVEL_W
        if( habia_tarjeta )
            GUI_MUESTRA_TARJETA(T_DATOS_TARJETA_REFRESCAR);
        else
            GUI_MUESTRA_TARJETA(T_DATOS_TARJETA_AGREGAR);
    #endif
    */
}
/*****************************************************************************/
void FONDO_FIJO_AGREGAR_MEDIO( int pos, char modo, double importe, unsigned cuenta, int id_registro,
                               double importe_me, int banco, long nro_cheque, long autorizacion, int mostrar )
/*****************************************************************************/
{
    int add_p_pago = 0;
    //char       bancotmp[100];
    if( pos == -1 ) {
        pos = RAM_P_PAGO;
        add_p_pago = 1;
    }
    else {
        ADD_MEMORY_DOUBLE( __ram_importe, -_PAGO_IMPORTE( pos ) );
    }
    if( pos < 9 ) {
        _SET_MEMORY_CHAR( __pago_modo, pos, modo );
        _SET_MEMORY_DOUBLE( __pago_importe, pos, importe );
        _SET_MEMORY_INT( __pago_cuenta, pos, cuenta );
        _SET_MEMORY_CHAR( __pago_id_registro, pos, id_registro );
        _SET_MEMORY_DOUBLE( __pago_importe_me, pos, importe_me );
        ADD_MEMORY_DOUBLE( __ram_importe, importe );
        _SET_MEMORY_LONG( __pago_numero, pos, ( long )nro_cheque );
        //_SET_MEMORY_INT(    __pago_cod_submedio,   pos, _COD_SUB_MEDIO );
        //_SET_MEMORY(        __pago_buffer,         pos, pagos2.nro_cheque );
        _SET_MEMORY_INT( __pago_banco, pos, banco );
        _SET_MEMORY_LONG( __pago_buffer1_long, pos, autorizacion );
        if( add_p_pago ) {
            ADD_MEMORY_CHAR( __ram_p_pago, 1 );
        }
    }
	if( mostrar ) {
	    MOSTRAR_FONDO_FIJO( pos );
	}
}
/**************************************************************************/
int MOSTRAR_FONDO_FIJO( int pos )
/**************************************************************************/
{
    int modo = _PAGO_MODO( pos );
    int larray = ENTEROS + 1 + DECIMALES + 5;
    char cadena[41];
    char *pr;
    char *tot = 0, *temp = 0, *temp1 = 0;
    int auxi = sizeof( _PAGO_CUENTA( pos ) );
    cadena[0] = 0;   
    tot = ( char* )calloc( 1, larray );
    pr = ( char* )calloc( 1, larray );
    if( !auxi ) {
        auxi = 80;
    }
    temp = ( char* )calloc( 1, auxi * 4 + 1 );
    temp1 = ( char* )calloc( 1, auxi * 2 + 1 );   
    if( tot != NULL && temp != NULL && temp1 != NULL && pr != NULL ) {
        memset( tot, 0, larray );
        memset( pr, 0, larray );
        memset( temp, 0, auxi * 2 );
        memset( temp1, 0, auxi );
    }
    else {
        GRABAR_LOG_SISTEMA( "Sin memoria para pantallas" ,LOG_PANTALLAS,3);
        return( 0 );
    }
    ExecuteSAction( A_MUESTRA_TIT_TOTAL_FFIJO, ST( S_TOTAL ) );
    FTOC( RAM_IMPORTE, tot, ENTEROS + 1, DECIMALES, 1, 1 );   
    ExecuteSAction( A_PAGO_MUESTRA_TOTAL, tot );
    if( _PAGO_BUFFER1_LONG( pos ) ) {
        SET_ENTERO( "000000", _PAGO_BUFFER1_LONG( pos ), temp1 );
        strcat( temp, temp1 );
    }
    if( _PAGO_CUENTA( pos ) ) {
        SET_ENTERO( "0000", _PAGO_CUENTA( pos ), temp1 );
        strcat( temp, temp1 );
    }       
    if( medios[modo].ticket_compra || medios[modo].mutual ) {
        SET_ENTERO( "0000", _PAGO_CUENTA( pos ), temp1 );
        strcat( temp, temp1 );
    }
    if( medios[modo].moneda_extranjera ) {
        FTOC( _PAGO_IMPORTE_ME( pos ), pr, ENTEROS + 1, DECIMALES, 1, 1 );
    }
    else {
        ;
    }
    FTOC( _PAGO_IMPORTE( pos ), pr, ENTEROS + 1, DECIMALES, 1, 1 );
    agregarPago( medios[modo].efectivo && IMPORTE_CONTADO_ADITIVO_EN_PAGOS, medios[modo].nombre, pr,
                 cadena, temp );
    FREEMEM( tot );
    FREEMEM( pr );
    FREEMEM( temp );
    FREEMEM( temp1 );
    return 1; 
}
/*****************************************************************************/
void FONDO_FIJO_INGRESA_MEDIO_AUT( int medio, int submedio, double importe )
/*****************************************************************************/
{
   int       ok = 1, h, pos = -1;
   char      nro = -1;
   int       cuenta = 0, id_registro;
   double    importe_me = 0;

   //--------------- Ubica el medio ----------------
   nro = -1;
   for( h = 0; ( h < 10 ) && ( nro < 0 ); h++ ) {
      if( medios[h].nro == medio ) {
         nro = h;
      }
   }
   if( nro < 0 ) //Si no encontro el medio
      ok = 0;
   //--------------- valida que no repita efectivo ---------------
   if( ok && medios[nro].efectivo ) {
      for( h = 0; h < RAM_P_PAGO; h++ )
         if( _PAGO_MODO( h ) == nro )
            pos = h;
   }
    //-------------- valida que no repita tarjetas ---------------
   if( ok && medios[nro].tarjeta ) {
      for( h = 0; h < RAM_P_PAGO; h++ )
         if( _PAGO_MODO( h ) == nro )
            pos = h;
   }
   if( ok && importe > 0.0 ) {
        //--------------- ID del registro ---------------
      id_registro = medios[nro].id_registro;
        //---------------- Si es ticket de compra ----------------
      if( medios[nro].ticket_compra && ok ) {
         cuenta = submedio;
      }
        //---------------- Si es mutual ----------------
      if( medios[nro].mutual && ok ) {
         cuenta = submedio;
         //if( _submedio_moneda_extranjera ) {
		 if( cuenta > 0  && MUTUAL_MONEDA_EXTRANJERA( cuenta ) ) {
           importe_me = importe;
           importe = MUTUALES_ME_A_ML( importe_me, cuenta );
         }
      }
        //---------------- Si es moneda extranjera ---------------- 
      if( medios[nro].moneda_extranjera && ok ) {
         importe_me = importe;
         importe = PAGOS_ME_A_ML( importe_me, nro );
      }
        //------------------ si todo OK agrega el pago --------------------
      if( ok ) {
		  FONDO_FIJO_AGREGAR_MEDIO( pos, ( char )nro, importe, cuenta, 
			  id_registro, importe_me, NO, NO, NO, NO );
      }
   }
}

/*****************************************************************************/
int FONDO_FIJO_CARGA_MEDIOS( void )
/*****************************************************************************/
{
   int       ok = 0;
   unsigned  cajero = 0;
   int       rta = 0;
   char      cod_ff_aut = 0;
   
//Busco el codigo del fondo fijo automatico.
   //if(  USE_DB( AREA_CAJEROS, _CAJEROS_SIC, ( char* )&datos_cajero, sizeof( datos_cajero ), NULL, 0, 0 ) == 0 ) {
   if( OPEN_TABLE( T_CAJEROS, TT_ORIG, ( char* )&datos_cajero, sizeof( datos_cajero ) ) == 0 ) {
      cajero = NRO_CAJERO;
      //GET_EQUAL( ( char * ) &cajero );
	  SET_WHERE("COD_CAJERO='%d'", cajero );
	  RUN_QUERY(NO);
      if( FOUND2( ) ){
		  cod_ff_aut = datos_cajero.cod_ff;
         ok = 1;
      }
      //CLOSE_DATABASE( AREA_CAJEROS );
	  CLOSE_TABLE( T_CAJEROS, TT_ORIG );
   } else {
      MENSAJE_STRING( S_ERROR_ARCHIVO_CAJEROS );
   }
   if( ok &&
		// !USE_DB( AREA_AUX2, _D_F_FIJO_SIC, ( char * )&det_ff, sizeof( struct d_f_fijo_sic ), NULL, 0, 0 ) ) {
		OPEN_TABLE(T_DETALLE_FONDOFIJO , TT_ORIG,( char* )&detalle_fondofijo,
							sizeof( struct _detalle_fondofijo))== 0 ) {
   //El procedimiento se basa en que esta validado un cajero...
      
      /*ind_det_ff.cod_ff = cod_ff_aut;
      GET_GREATER_E( ( char* ) &ind_det_ff );*/
	  SET_WHERE("COD_FF = '%i' AND COD_MEDIO >= %d AND COD_SUBMEDIO >= %i", cod_ff_aut, 0, 0 );
	  RUN_QUERY(SI);
      if( /*FOUND( )*/ FOUND2( )&& ( detalle_fondofijo.cod_ff == cod_ff_aut ) ) {
         while( /*!BtrvEOF( )*/ !dbEOF() && ( detalle_fondofijo.cod_ff == cod_ff_aut ) ) {

            FONDO_FIJO_INGRESA_MEDIO_AUT( detalle_fondofijo.cod_medio, detalle_fondofijo.cod_submedio, detalle_fondofijo.monto );
            //SELECT( AREA_AUX2 );
			SELECT_TABLE( T_DETALLE_FONDOFIJO , TT_ORIG );
            //SKIP( 1 );
			SKIP2( 1 );
         }
         rta = 1;
      }
      //CLOSE_DATABASE( AREA_AUX2 );
	  CLOSE_TABLE( T_DETALLE_FONDOFIJO, TT_ORIG );
   }

   return( rta );
}
