#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h>
#include <asc.h>
#include <btrv.h>
#include <b_mens.h>
#include <cajon.h>
#include <cio.h>
#include <clave.h>
#include <cmemory.h>
#include <cobros.h>
#include <cr.h>
#include <cr_pant.h>
#include <dbrouter.h>
#include <eventos.h>
#include <getch.h>
#include <imp_fond.h>
#include <imp_tick.h>
#include <lista.h>
#include <log.h>
#include <l_valid.h>
#include <mstring.h>
#include <m_cadena.h>
#include <on_line.h>
#include <pagxcaja.h>
#include <pant.h>
#include <print.h>
#include <p_float.h>
#include <stack.h>
#include <stringt.h>
#include <tarjetas.h>
#include <tkt.h>
#include <valida.h>
#include <xyz.h>
#include <_cr1.h>
#include <db.h>
#include <deftables.h>
#include <imp_comp.h>
#include <fiscal.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <ctype.h>
#include <math.h>

#ifdef INVEL_W
#include <dos.h>
#endif
#include <aarch.h>
#include <config.h>
#include <actions.h>
static struct _pagos1 pagos1;
extern struct _pagos2 pagos2;
static struct _ind_pagos2 ind_pagos2;
#define _COMPROBANTE  GET_MEMORY_LONG( _comprobante )
#define _CUENTA       GET_MEMORY_LONG( _cuenta )
#define _IMPORTE      GET_MEMORY_DOUBLE( _importe )
#define __GRAVADO     GET_MEMORY_DOUBLE( _gravado )
#define _EXENTO       GET_MEMORY_DOUBLE( _exento )
#define _IVA          GET_MEMORY_DOUBLE( _iva )
#define _COD_VALIDA   GET_MEMORY_INT( _cod_valida )
#define _COD_NIV1     GET_MEMORY_CHAR( _cod_niv1 )
#define _COD_NIV2     GET_MEMORY_CHAR( _cod_niv2 )
#define _DISCRIMINA   GET_MEMORY_CHAR( _discrimina )
#define _CTA_CMR      GET_MEMORY_STR( _cta_cmr )
#define _COD_VIGENCIA GET_MEMORY_CHAR( _cod_vigencia )
#define _CUOTAS       GET_MEMORY_CHAR( _cuotas )
#define _CMR               25
void CONSULTA_CUENTA_ON_LINE_ADELANTO( char *cta );
extern double PEDIR_FLOAT_EN_RECUADRO_CS( char *titulo, int salvar_pantalla, int enteros,
                                          int decimales, int tipo );
extern int PEDIR_CTA_CMR( char *cta, void f_online( char *cta ),
                          int f_valid( int cod, char *cta ) );
int VALIDAR_CUENTA_ADELANTO( int no_usado, char *cta );
static int _estado,_comprobante,_cuenta,_importe,_gravado,_exento,_iva;
static int _cod_valida,_cod_niv1,_cod_niv2,_discrimina;
int cod_pago_ant[10] = {0,0,0,0,0,0,0,0,0,0};
static int browse = 0;

void PANTALLA_PAGOS_POR_CAJA();
void PAGOS_POR_CAJA();
void GRABAR_EVENTO_PAGO();
void TICKET_PAGOS_POR_CAJA();
/*****************************************************************************/
void PAGOS_POR_CAJA()
/*****************************************************************************/
{
    int _ptr,_handle;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    _comprobante = STACK_INS( sizeof( long ) );
    _cuenta = STACK_INS( sizeof( long ) );
    _importe = STACK_INS( sizeof( double ) );
    _gravado = STACK_INS( sizeof( double ) );
    _exento = STACK_INS( sizeof( double ) );
    _iva = STACK_INS( sizeof( double ) );
    //  _cod_valida = STACK_INS( sizeof( int ) );
    _cod_valida = STACK_INS( sizeof( short ) );
    _cod_niv1 = STACK_INS( sizeof( char ) );
    _cod_niv2 = STACK_INS( sizeof( char ) );
    _discrimina = STACK_INS( sizeof( char ) );
    if( _ESTADO == 0 ) {
        if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, SI, SI, PEDIR_CLAVE_EN_PAGOS,
                              ST( S__PAGOS_ ), NULL, RAM_COD_FUNCION, SI ) ) {

            SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
            if( OPEN_TABLE( T_PAGOS1, TT_ORIG, ( char* )&pagos1, sizeof( pagos1 ) ) == 0 ) {
                if( OPEN_TABLE( T_PAGOS2, TT_ORIG, ( char* )&pagos2, sizeof( pagos2 ) ) == 0 ) {
                    PANTALLA_PAGOS_POR_CAJA();
                    _ATRIBUTO = 112;
                    /*---------------------- Dibuja la pantalla ----------------------------*/
                    ExecuteSAction( A_MUESTRA_TIT_COMPROB_PAGOS_CAJA, ST( S_Comprobante ) );
                    ExecuteSAction( A_MUESTRA_TIT_CUENTA_PAGOS_CAJA, ST( S_N_Cuenta ) );
                    ExecuteSAction( A_MUESTRA_TIT_IMPORTE_PAGOS_CAJA, ST( S_IMPORTE______ ) );
                    ExecuteSAction( A_MUESTRA_TIT_GRAVADO_PAGOS_CAJA, " " );
                    ExecuteSAction( A_MUESTRA_TIT_EXENTO_PAGOS_CAJA, " " );
                    ExecuteSAction( A_MUESTRA_TIT_IVA_PAGOS_CAJA, " " );
                    ExecuteAction( A_OCULTA_DISCRIMINA_PAGOS_CAJA );
                    GRABAR_LOG_SISTEMA("COMIENZO PAGOS POR CAJA", LOG_VENTAS,2);
                    /*----------- ingresa pagos ------------*/

					if( PIDE_PAGOS_POR_CAJA() ) {
                        _SET_ESTADO( 1 );
                    }
                }
            }
        }
    }
    if( _ESTADO == 1 ) {
        ABRIR_CAJON();
        TICKET_PAGOS_POR_CAJA();
        _SET_ESTADO( 2 );
    }
    if( _ESTADO == 2 ) {
        char mensaje[100];
        char nivel;
        memset( mensaje, 0, sizeof( mensaje ) );
        nivel = _COD_NIV1;
        SELECT_TABLE( T_PAGOS1, TT_ORIG );
        SET_WHERE( "CODIGO = '%d'", nivel );
		  RUN_QUERY( NO );
        if( FOUND2() ) {
            if( _COD_NIV2 ) {
                nivel = _COD_NIV2;
                SELECT_TABLE( T_PAGOS1, TT_ORIG  );
				SET_WHERE( "CODIGO = '%d'", nivel );
				RUN_QUERY( NO );
                _snprintf(mensaje, sizeof(mensaje)-1, "%s %s", pagos1.nombre, pagos2.nombre );
            }
            else {
                strcpy( mensaje, pagos1.nombre );
            }
        }
        else {
            strncpy( mensaje, ST( S_INVALIDO ), sizeof( mensaje ) );
        }
        VALIDAR_PAGOS( _COD_VALIDA, _IMPORTE, _CUENTA, _COMPROBANTE, mensaje );
        _SET_ESTADO( 3 );
    }
    if( _ESTADO == 3 ) {
        #ifdef COMPILAR_ASC
        GENERA_PAGOS_ASC( pagos2.id, _COMPROBANTE, _CUENTA, _IMPORTE, _GRAVADO, _EXENTO, _IVA );
        #endif
        _SET_ESTADO( 4 );
    }
    if( _ESTADO == 4 ) {
        GRABAR_EVENTO_PAGO();
        _SET_ESTADO( 5 );
    }
    if( _ESTADO == 5 ) {
        GRABAR_LOG_SUPERVISOR( "Pagos por caja", USUARIO_CLAVE_ANTERIOR() ,LOG_VENTAS,4);
        GRABAR_XYZ( 'X' );
        _SET_ESTADO( 6 );
    }
    if( _ESTADO == 6 ) {
        /*------------ actualiza acumuladores -------------*/
        ADD_MEMORY_DOUBLE( __x_pagos_por_caja_importe, _IMPORTE );
        ADD_MEMORY_INT( __x_pagos_por_caja_cantidad, 1 );
        ADD_MEMORY_DOUBLE( __x_tot_dinero, -( _IMPORTE ) );
    }
    /*--------------- fin ----------------*/
	//Ya esta el A_DES_TODOS_ACT_VENTAS
	//ExecuteAction( A_DES_PAGOS_CAJA_ACT_VENTAS );
	CERRAR_CAJON();
	//IMPRIME_PIE_COMPROBANTE( COBRO, NO );
    CLOSE_TABLE( T_PAGOS2, TT_ORIG );
	CLOSE_TABLE( T_PAGOS1, TT_ORIG  );
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
void PANTALLA_PAGOS_POR_CAJA()
/*****************************************************************************/
{
    char cad_modo[15];
    ExecuteAction( A_DES_VENTAS_ACT_PAGOS_CAJA );
    strncpy( cad_modo, ST( S__PAGOS_ ), sizeof( cad_modo ) );
    ExecuteSAction( A_MUESTRA_MODO_PAGOS_CAJA, cad_modo );
}
/*****************************************************************************/
int ELEGIR_PAGOS_CAJA_BROWSE()
/*****************************************************************************/
{
  /*  int rta;
    SELECT_TABLE( T_PAGOS1, TT_ORIG );
    GO2( TOP );
    _ATRIBUTO = 120;
    //MOSTRAR_CADENA( _X1 + 1, _Y2 + 2, "    " );
    rta = Consulta_Pagos_Caja( _consulta_pagos_caja );
    return ( rta );*/

    int rta = 1, ok = 1, registros = 0;
	char aux[60];

    browse = 1;
    SELECT_TABLE( T_PAGOS1, TT_ORIG );
    GO2( TOP );
	//_ATRIBUTO = 120;
	do {
		if( browse > 1 ) {
			if( pagos1.subnivel == 'S' ) {
				//En esta funcion se actualiza browse
				ok = !MOSTRAR_SUBNIVEL();
			} else {
				rta = 1;
				ok = 0;
			}
		}
		if( browse > 1 ) {
			SELECT_TABLE( T_PAGOS2, TT_ORIG );
			SET_WHERE("COD_NIVEL1 = '%i' AND CODIGO >= '%d'", cod_pago_ant[browse], 0);
			SET_ORDER2("COD_NIVEL1, CODIGO");
			RUN_QUERY(NO);
			if( !FOUND2() ) {
				ok = 0;
            	GO2( TOP );
				SET_ORDER2("CODIGO");
				SET_WHERE("COD_NIVEL1 = '%i'", cod_pago_ant[--browse] ); 
				RUN_QUERY(NO);
			}
        } else {
               SELECT_TABLE( T_PAGOS1, TT_ORIG );
               SET_WHERE("");
			   SET_ORDER2("CODIGO");
			   RUN_QUERY(NO);
        }

		if( ok ) {
			registros = RECCOUNT2( NO, NULL );
			if( registros > 0 ) {
				SELECT_TABLE( T_PAGOS1, TT_ORIG );
				rta = Consulta_Pagos_Caja( _consulta_pagos_caja );
				/*if( pagos1.subnivel != 'S' )
					ok = 0;*/
			} else {
				ok = 0;
				rta = 0;
				memset( aux, 0, sizeof(aux) );
				_snprintf(aux, sizeof(aux), "%s %s", ST( S_NO_HAY_PAGOS_X_CAJA_CONFIGURADOS ), ST( S____PRESIONE_UNA_TECLA__ ) );
				aux[sizeof( aux ) - 1] = '\0';
				MENSAJE_TECLA( aux );
			}
        }

	} while( browse && ok );

   return ( rta );

}
/*****************************************************************************/
int _consulta_pagos_caja( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y/*, cod*/;
    int _X1,_X2;
    _X1 = 32;
    _X2 = 53;
    y = 5 + dato;
    switch( comando ) {
        /*case _BUSCAR:
            //MOSTRAR_CADENA( _X1 + 1, _Y2 + 2, cadena );
            cod = atoi( cadena );
			SET_WHERE( "CODIGO >= '%ld'", cod );
			RUN_QUERY( NO );
            break;*/
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                _itoa( pagos1.codigo, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, pagos1.nombre );
            }
            #endif
            MOSTRAR_ENTERO( _X1, y, "999", pagos1.codigo );
            MOSTRAR_CADENA( _X1 + 4, y, pagos1.nombre );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
			cod_pago_ant[++browse] = pagos1.codigo;
            rta = 1;
            //browse++;
            break;
        case _COMPARAR:
            rta = 2;
            break;
        case _TECLA:
            /*if( dato == 32 )
                    b->seguir = 0;*/
            /*if( dato == 27 ) {
				   browse --;
            }*/
			if( dato == 32 || dato == -73 || dato == -81 || ( dato >= 48 && dato <= 57 ) ) {
                b->seguir = 0;
			} else if( dato == 27 ) {
				browse --;
				//mostrar_cobro = 1;
            }
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int _consulta_pagos_caja2( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y/*, cod*/;
    int _X1,_X2;
    _X1 = 32;
    _X2 = 53;
    y = 5 + dato;
    switch( comando ) {
        case _BUSCAR:
            //MOSTRAR_CADENA( _X1 + 1, _Y2 + 2, cadena );
            /*cod = atoi( cadena );
            SET_WHERE( "COD_NIVEL1 = '%ld' AND CODIGO >= '%i'", pagos2.cod_nivel1, cod );
            RUN_QUERY( NO );*/
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                _itoa( pagos2.codigo, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, pagos2.nombre );
            }
            #endif
            MOSTRAR_ENTERO( _X1, y, "999", pagos2.codigo );
            MOSTRAR_CADENA( _X1 + 4, y, pagos2.nombre );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            if( pagos2.cod_nivel1 != pagos1.codigo ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
			cod_pago_ant[++browse] = pagos2.codigo;
            rta = 1;
            break;
        case _COMPARAR:
            rta = 2;
            break;
        case _TECLA:
            /*if( dato == 32 )
                    b->seguir = 0;*/
			if( dato == 32 || dato == -73 || dato == -81 || ( dato >= 48 && dato <= 57 ) ) {
                b->seguir = 0;
			} else if( dato == 27 ) {
				browse --;
				//mostrar_cobro = 1;
            }

            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int MOSTRAR_SUBNIVEL()
/*****************************************************************************/
{
    int	rta = 0;

    /*------------ muestra los nombres ------------*/
    SELECT_TABLE( T_PAGOS2, TT_ORIG );
    ind_pagos2.cod_nivel1 = ( char )pagos1.codigo;
    ind_pagos2.codigo = 0;
	SET_ORDER2("COD_NIVEL1 , CODIGO ");
	SET_WHERE( "COD_NIVEL1 = '%ld' AND CODIGO >= '%ld'", ind_pagos2.cod_nivel1, ind_pagos2.codigo );
	//SET_WHERE( "COD_SUBNIVEL1 >= '%ld' AND CODIGO >= '%ld'", ind_pagos2.cod_nivel1, ind_pagos2.codigo );
	RUN_QUERY( NO );
    rta = Consulta_Pagos_Caja( _consulta_pagos_caja2 );
    
	return ( rta );
}
/*****************************************************************************/
int PIDE_PAGOS_POR_CAJA()
/*****************************************************************************/
{
    int seguir = 1, rta = 0;
    seguir = ELEGIR_PAGOS_CAJA_BROWSE();
	if( seguir ) {
         if( pagos1.subnivel == 'S' ) {
            SET_MEMORY_INT( _cod_valida, pagos2.cod_valida );
            SET_MEMORY_CHAR( _cod_niv2, pagos2.codigo );
            SET_MEMORY_CHAR( _discrimina, pagos2.discrimina );
        }
        else {
            pagos2.codigo = pagos1.codigo;
            pagos2.id = pagos1.id;
            pagos2.discrimina = pagos1.discrimina;
            strcpy( pagos2.nombre, pagos1.nombre );
            SET_MEMORY_INT( _cod_valida, pagos1.cod_valida );
            SET_MEMORY_CHAR( _cod_niv1, pagos1.codigo );
            SET_MEMORY_CHAR( _cod_niv2, 0 );
            seguir = 1;
        }
    }

    if( seguir ) {
        rta = PEDIR_DATOS_PAGO();
    }

    BORRAR_MENSAJE();

    return ( rta );
}
/*****************************************************************************/
int ELEGIR_SUBNIVEL()
/*****************************************************************************/
{
    int seguir = 1, k, rta = 0;
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 27:
                /*-------- ESC ---------*/
                seguir = 0;
                break;
            default:
                if( k >= '0' && k <= '9' ) {
                    ind_pagos2.cod_nivel1 = pagos1.codigo;
                    ind_pagos2.codigo = k - '0';
                    SELECT_TABLE( T_PAGOS2, TT_ORIG );
                    SET_WHERE( "COD_SUBNIVEL1 = '%ld' AND CODIGO = '%ld'", ind_pagos2.cod_nivel1,
						ind_pagos2.codigo );
					RUN_QUERY( NO );
                    if( FOUND2() ) {
                        seguir = 0;
                        rta = 1;
                    }
                }
                break;
        }
    }
    BORRAR_MENSAJE();
    return ( rta );
}
/*****************************************************************************/
int PEDIR_DATOS_PAGO()
/*****************************************************************************/
{
    int rta = 0, y = 12;
    int sig = 0, fin = 0;
    if( pagos2.discrimina == 'S' ) {
        y += 3;
    }
    ExecuteAction( A_OCULTA_DISCRIMINA_PAGOS_CAJA );
    if( pagos2.discrimina == 'S' ) {
        ExecuteSAction( A_MUESTRA_TIT_GRAVADO_PAGOS_CAJA, "Gravado :" );
        ExecuteSAction( A_MUESTRA_TIT_EXENTO_PAGOS_CAJA, ST( S_EXENTO_____ ) );
        ExecuteSAction( A_MUESTRA_TIT_IVA_PAGOS_CAJA, "Iva :" );
        ExecuteAction( A_MUESTRA_DISCRIMINA_PAGOS_CAJA );
    }
    /*------------------------ Pide los datos -----------------------------*/
    while( sig == 0 && fin == 0 ) {
        ExecuteAction( A_MUESTRA_DATO_COMPROB_PAGOS_CAJA );
        SET_MEMORY_LONG( _comprobante, PEDIR_ENTERO( 51 - ancho_panel / 2, 9, 4, NO ) );
        if( _COMPROBANTE > 0 ) {
            sig = 1;
        }
        if( _COMPROBANTE == -1 ) {
            fin = 1;
        }
    }
    sig = 0;
    while( sig == 0 && fin == 0 ) {
        ExecuteAction( A_MUESTRA_DATO_CUENTA_PAGOS_CAJA );
        SET_MEMORY_LONG( _cuenta, PEDIR_ENTERO( 51 - ancho_panel / 2, 10, 8, NO ) );
        if( _CUENTA > 0 ) {
            sig = 1;
        }
        if( _CUENTA == -1 ) {
            fin = 1;
        }
    }
    sig = 0;
    while( sig == 0 && fin == 0 ) {
        ExecuteAction( A_MUESTRA_DATO_IMPORTE_PAGOS_CAJA );
        SET_MEMORY_DOUBLE( _importe,
                           PEDIR_FLOTANTE( 51 - ancho_panel / 2, 11, ENTEROS, DECIMALES ) );
        if( _IMPORTE > 0 ) {
            sig = 1;
        }
        if( _IMPORTE == -1 ) {
            fin = 1;
        }
    }
    sig = 0;
    if( _IMPORTE > 0 ) {
        if( _IMPORTE > X_TOT_DINERO ) {
			MENSAJE_CON_PAUSA(ST( S_NO_HAY_EFECTIVO_SUFICIENTE ), 20 );
        }
        else {
            if( pagos2.discrimina == 'S' ) {
                while( sig == 0 && fin == 0 ) {
                    ExecuteAction( A_MUESTRA_DATO_GRAVADO_PAGOS_CAJA );
                    SET_MEMORY_DOUBLE( _gravado,
                                       PEDIR_FLOTANTE( 51 - ancho_panel / 2, 12, ENTEROS,
                                                       DECIMALES ) );
                    if( __GRAVADO > 0 ) {
                        sig = 1;
                    }
                    if( __GRAVADO == -1 ) {
                        fin = 1;
                    }
                }
                sig = 0;
                while( sig == 0 && fin == 0 ) {
                    ExecuteAction( A_MUESTRA_DATO_EXENTO_PAGOS_CAJA );
                    SET_MEMORY_DOUBLE( _exento,
                                       PEDIR_FLOTANTE( 51 - ancho_panel / 2, 13, ENTEROS,
                                                       DECIMALES ) );
                    if( _EXENTO > 0 ) {
                        sig = 1;
                    }
                    if( _EXENTO == -1 ) {
                        fin = 1;
                    }
                }
                sig = 0;
                while( sig == 0 && fin == 0 ) {
                    ExecuteAction( A_MUESTRA_DATO_IVA_PAGOS_CAJA );
                    SET_MEMORY_DOUBLE( _iva,
                                       PEDIR_FLOTANTE( 51 - ancho_panel / 2, 14, ENTEROS,
                                                       DECIMALES ) );
                    if( _IVA > 0 ) {
                        rta = 1;
                        sig = 1;
                    }
                    if( _IVA == -1 ) {
                        fin = 1;
                    }
                }
            }
            else {
                rta = 1;
            }
        }
    }
    return ( rta );
}
#ifdef COMPILAR_ASC
/***********************************************************************/
void CREAR_PAGOS_ASC()
/***********************************************************************/
{
    int handle;
    handle = CREAT( _PAGOS_ASC, FA_ARCH );
    _CLOSE( handle, __LINE__, __FILE__ );
}
/***********************************************************************/
void COPIAR_PAGOS_ASC()
/***********************************************************************/
{
    COPY( _PAGOS_ASC, _PAGOS_ASC_BAK );
    COPY( _PAGOS_ASC, _PAGOS_ASC_REMOTO );
    LOG_EXISTE_ARCHIVO_COD( _PAGOS_ASC_REMOTO );
}
#endif
/**************************************************************************/
void TICKET_PAGOS_POR_CAJA()
/**************************************************************************/
{
    if( IMPRESORA_FISCAL ) {
        //PRN_SELECT( JOURNAL );
		ABRIR_DOCUMENTO_NO_FISCAL('1');
        IMPRIME_CABECERA_GENERAL( NRO_EVENTO + 1 );
    }
    else {
        PRN_CONTINUA();
        IMPRIME_CABECERA_TICKET( NO );
        if( PEDIR_CLAVE_EN_PAGOS ) {
            IMPRIMIR_NOMBRE_USUARIO( USUARIO_CLAVE_ANTERIOR() );
        }
        IMPRIMIR_NOMBRE_USUARIO( NRO_CAJERO );
        LF( 1 );
    }

    PRN_LETRA_GRANDE();
    MOSTRAR_CADENA( 8, PRN, ST( S_PAGOS ) );
    LF( 2 );
	PRN_LETRA_NORMAL();
    MOSTRAR_CADENA( 1, PRN, ST( S_CONCEPTO_____ ) );
    MOSTRAR_CADENA( 15, PRN, pagos2.nombre );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_COMPROBANTE__ ) );
    MOSTRAR_ENTERO( 19, PRN, "9999", _COMPROBANTE );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CUENTA_______ ) );
    MOSTRAR_ENTERO( 15, PRN, "99999999", _CUENTA );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE______ ) );
    MOSTRAR_FLOTANTE( 16, PRN, ENTEROS, DECIMALES, 0, 0, _IMPORTE );
    LF( 1 );
    if( _DISCRIMINA == 'S' ) {
        MOSTRAR_CADENA( 1, PRN, "Gravado     :" );
        MOSTRAR_FLOTANTE( 16, PRN, ENTEROS, DECIMALES, 0, 0, __GRAVADO );
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, "Exento      :" );
        MOSTRAR_FLOTANTE( 16, PRN, ENTEROS, DECIMALES, 0, 0, _EXENTO );
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, "I.V.A.      :" );
        MOSTRAR_FLOTANTE( 16, PRN, ENTEROS, DECIMALES, 0, 0, _IVA );
        LF( 1 );
    }
	PRN_FIN_TICKET();

}
/*****************************************************************************/
void GRABAR_EVENTO_PAGO()
/*****************************************************************************/
{
    
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.nro_cliente = _CUENTA;
    event_ticket.nro_comprobante_new = _COMPROBANTE;
    event_ticket.costo = _IMPORTE;
    event_ticket.importe_sin_iva = __GRAVADO;
    event_ticket.exento = _EXENTO;
    event_ticket.iva1 = _IVA;
    event_ticket.bultos = 0 ;//se comenta porque ya no se conoce para que se uso esta codificacion _COD_NIV1 * 256 + _COD_NIV2;
    event_ticket.tipo_evento = 2;
    GRABAR_EVENT_TICKET( NO );
    ACTUALIZA_ID_EVENTO_NEW();
}
#ifdef COMPILAR_CMR
/*****************************************************************************/
void ADELANTO_CMR()
/*****************************************************************************/
{
    int _ptr,_handle,_cuotas,_cod_vigencia,_cta_cmr;
    if( VENDEDOR_OBLIGATORIO && !NRO_VENDEDOR_LONG ) {
        MENSAJE_STRING( S_FAVOR_INGRESE_VENDEDOR );
        return ( 0 );
    }
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //_estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    _importe = STACK_INS( sizeof( double ) );
    _cuotas = STACK_INS( sizeof( char ) );
    _cod_vigencia = STACK_INS( sizeof( char ) );
    _cta_cmr = STACK_INS( 15 );
    /*---------- prepara pantalla ----------*/
    if( _ESTADO == 0 ) {
        if( SOLICITAR_CLAVES( 30, 21, SI, SI, PEDIR_CLAVE_EN_PAGOS, ST( S__ADELANTOS_ ), NULL,
                              RAM_COD_FUNCION, SI ) ) {
            SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
            if( OPEN_TABLE( T_PAGOS1, TT_ORIG, &pagos1, sizeof( pagos1 ) ) == 0 ) {
                _SET_ESTADO( 1 );
                CLOSE_TABLE( T_PAGOS1, TT_ORIG );
            }
        }
    }
    if( _ESTADO == 1 ) {
        if( PEDIR_DATOS_ADELANTO_CMR( _cta_cmr, _importe, _cuotas, _cod_vigencia ) ) {
            _SET_ESTADO( 2 );
        }
    }
    /*------------------------- Envia el paquete on-line --------------------*/
    if( _ESTADO == 2 ) {
        SET_MEMORY_CHAR( __var_tarjetas_tarjeta, _CMR );
        SET_NUMERO_TARJETA_PRIVADO(  _CTA_CMR );//SET_MEMORY( __var_tarjetas_numero, _CTA_CMR );
        SET_MEMORY_CHAR( __var_tarjetas_cuotas, _CUOTAS );
        SET_MEMORY_CHAR( __var_tarjetas_cod_vigencia, _COD_VIGENCIA );
        if( ON_LINE_ADELANTO_TARJETA( _IMPORTE ) ) {
            _SET_ESTADO( 3 );
        }
    }
    /*----------------- Imprime el ticket  -----------------*/
    if( _ESTADO == 3 ) {
        ABRIR_CAJON();
        TICKET_ADELANTO_CMR( _cta_cmr, _cuotas, _cod_vigencia );
        _SET_ESTADO( 4 );
    }
    if( _ESTADO == 4 ) {
        GRABAR_EVENTO_ADELANTO( _cta_cmr, _importe, _cuotas, _cod_vigencia );
        _SET_ESTADO( 6 );
    }
    /* if( _ESTADO == 5 ) { VALIDAR_PAGOS( pagos1.cod_valida, _IMPORTE, 0 );
     * _SET_ESTADO( 6 ); } */
    if( _ESTADO == 6 ) {
        GRABAR_LOG_SUPERVISOR( "Adelantos por caja", USUARIO_CLAVE_ANTERIOR() );
        GRABAR_XYZ( 'X' );
        _SET_ESTADO( 7 );
    }
    if( _ESTADO == 7 ) {
        /*------------ actualiza acumuladores -------------*/
        ADD_MEMORY_DOUBLE( __x_pagos_por_caja_importe, _IMPORTE );
        ADD_MEMORY_INT( __x_pagos_por_caja_cantidad, 1 );
        ADD_MEMORY_DOUBLE( __x_tot_dinero, -( _IMPORTE ) );
    }
    /*--------------- fin ----------------*/
    CERRAR_CAJON();
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
int PEDIR_DATOS_ADELANTO_CMR( int _cta, int _importe, int _cuotas, int _cod_vigencia )
/*****************************************************************************/
{
    int rta = 0, cuotas;
    char cta[15];
    double importe;
    do {
        rta = PEDIR_CTA_CMR( cta, CONSULTA_CUENTA_ON_LINE_ADELANTO, VALIDAR_CUENTA_ADELANTO );
    }
    while( rta == -1 );
    if( rta == 1 ) {
        rta = 0;
        importe = PEDIR_FLOAT_EN_RECUADRO_CS( ST( S__IMPORTE_A_RETIRAR_ ), NO, ENTEROS, DECIMALES,
                                              _SIMPLE );
        if( LASTKEY() != 27 ) {
            cuotas = ELEGIR_CUOTAS( _CMR, cod_medio );
            if( cuotas ) {
                /*--------------------------- Pide vigencia -----------------------------*/
                if( DEBE_PEDIR_COD_VIGENCIA( _CMR ) ) {
                    SET_MEMORY_CHAR( _cod_vigencia, ELEGIR_VIGENCIA( _CMR ) );
                }
                else {
                    SET_MEMORY_CHAR( _cod_vigencia, 0 );
                }
                SET_MEMORY( _cta, cta );
                SET_MEMORY_DOUBLE( _importe, importe );
                SET_MEMORY_CHAR( _cuotas, cuotas );
                rta = 1;
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
void CONSULTA_CUENTA_ON_LINE_ADELANTO( char *cta )
/*****************************************************************************/
{
    CEREAR_STRUCT( _var_tarjetas );
    SET_MEMORY_CHAR( __var_tarjetas_tarjeta, _CMR );
    SET_NUMERO_TARJETA_PRIVADO(  cta );//SET_MEMORY( __var_tarjetas_numero, cta );
    _CONSULTA_ESTADO_TARJETA_ADELANTO();
    // _CONSULTA_ESTADO_TARJETA_COBRO();
    CEREAR_STRUCT( _var_tarjetas );
}
/*****************************************************************************/
int VALIDAR_CUENTA_ADELANTO( int no_usado, char *cta )
/*****************************************************************************/
{
    int rta;
    rta = EJECUTAR_L_VALID( cta, pagos1.cod_valida );
    if( !rta ) {
        MENSAJE_STRING( S_NRO_CTA_INVALIDO );
    }
    return ( rta );
}
/**************************************************************************/
void TICKET_ADELANTO_CMR( int _cta_cmr, int _cuotas, int _cod_vigencia )
/**************************************************************************/
{
    char nom_vigencia[21];
    if( IMPRESORA_FISCAL ) {
        PRN_SELECT( JOURNAL );
        IMPRIME_CABECERA_GENERAL( NRO_EVENTO + 1 );
    }
    else {
        PRN_CONTINUA();
        IMPRIME_CABECERA_TICKET();
        LF( 1 );
    }
    PRN_LETRA_GRANDE();
    MOSTRAR_CADENA( 1, PRN, ST( S_ADELANTO_EN_EFECTIVO ) );
    PRN_LETRA_NORMAL();
    LF( 2 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CUENTA_______ ) );
    MOSTRAR_CADENA( 15, PRN, _CTA_CMR );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE______ ) );
    MOSTRAR_FLOTANTE( 18, PRN, ENTEROS, DECIMALES, SI, NO, _IMPORTE );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CUOTAS_______ ) );
    MOSTRAR_ENTERO( 23, PRN, "00", _CUOTAS );
    if( BUSCAR_VIGENCIA( _CMR, _COD_VIGENCIA, nom_vigencia ) ) {
        LF( 1 );
        {
            char aux[25];
            sprintf( aux, "%s    :", ST( S_VIGENCIA ) );
            MOSTRAR_CADENA( 1, PRN, aux );
        }
        MOSTRAR_CADENA( 16, PRN, nom_vigencia );
    }
    LF( 1 );
    VALIDAR_VOUCHER( COD_VALIDACION_VOUCHER( _CMR ) );
    LF( 2 );
    MOSTRAR_CADENA( 1, PRN, ST( S_FIRMA_____ ) );
    MOSTRAR_CADENA( 12, PRN, "___________________________" );
    LF( 2 );
    MOSTRAR_CADENA( 1, PRN, ST( S_DOCUMENTO_ ) );
    MOSTRAR_CADENA( 12, PRN, "___________________________" );
    LF( 1 );
    if( !IMPRESORA_FISCAL ) {
        PRN_FIN_TICKET();
    }
}
/*****************************************************************************/
void GRABAR_EVENTO_ADELANTO( int _cta_cmr, int _importe, int _cuotas, int _cod_vigencia )
/*****************************************************************************/
{
    /*------------------------ Graba el evento --------------------------*/
    
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.costo = _IMPORTE;
    event_ticket.tipo_evento = 2;
    GRABAR_EVENT_TICKET( NO );
    memset( &event_pago, 0, sizeof( event_pago ) );
    event_pago.modo_pago = 0;
    event_pago.cuotas = _CUOTAS;
    event_pago.cod_vigencia = _COD_VIGENCIA;
    memcpy( event_pago.nro_tarjeta, _CTA_CMR, SIZEOF_VAR( __pago_nro_tarjeta ) );
    event_pago.importe = _IMPORTE;
    event_pago.posicion = 0;
    GRABAR_EVENT_PAGO( NO );
    ACTUALIZA_ID_EVENTO_NEW();
}
#endif //compilar CMR

